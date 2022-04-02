// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
#include "synchconsole.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------

char* UserToSystem(int virtual_address, int limit) {
	int i;
	int one_char;
	char* kernel_buffer = new char[limit + 1];
	if (kernel_buffer == NULL)
		return kernel_buffer;
	memset(kernel_buffer, 0, limit + 1);
	for (i = 0; i < limit; i++) {
		kernel->machine->ReadMem(virtual_address + i, 1, &one_char);
		kernel_buffer[i] = (char)one_char;
		if (one_char == 0)
			break;
	}
	return kernel_buffer;
}

int SystemToUser(int virtual_address, int length, char* buffer) {
	if (length < 0)
		return -1;
	if (length == 0)
		return length;
	int i = 0;
	int one_char = 0;
	do {
		one_char = (int)buffer[i];
		kernel->machine->WriteMem(virtual_address + i, 1, one_char);
		i++;
	} while (i < length and one_char != 0);
	return i;
}

void IncreasePC() {
	int counter = kernel->machine->ReadRegister(PCReg);
   	kernel->machine->WriteRegister(PrevPCReg, counter);
    counter = kernel->machine->ReadRegister(NextPCReg);
    kernel->machine->WriteRegister(PCReg, counter);
   	kernel->machine->WriteRegister(NextPCReg, counter + 4);
}

const int max_int_length = 32;
const int max_str_length = 255;

void
ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {

    case SyscallException:

    	switch(type) {
			
    	case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
			SysHalt();
			ASSERTNOTREACHED();
			break;

		case SC_ReadNum:
{
			DEBUG(dbgSys, "Prepare to input the number\n");
			// Initialize
			char* buffer = new char[max_int_length + 1];
			int size = 0;
			int new_number = 0;
			bool is_positive = 0;
			int i = 0;
			
			// Read char array
			while (size < max_int_length) {
				char c = kernel->synchConsoleIn->GetChar();
				if ((c == '\0') or (c == '\n')) 
					break;
				else if ((c == '\t') and (size > 0)) {
					buffer[size--] = 0;
				}
				else {
					buffer[size++] = c;
				}
			}

			if (buffer == NULL) {
				DEBUG(dbgFile, "Not enought memory in the system");
				std::cerr << "Not enought memory in the system\n";
				kernel->machine->WriteRegister(2, -1);
				delete buffer;
				return;
			}

			// Convert to number
			// Firstly, check negative number
			if (buffer[0] == '-') {
				is_positive = false;
				i = 1;
				if (size == 1) {	// Only subtract symbol
					DEBUG(dbgAddr, "Invalid number\n");
					std::cerr << "\nInvalid number\n";
					delete buffer;
					kernel->machine->WriteRegister(2, 0);
					IncreasePC();
					return;
				}
			}
			else if ((buffer[0] >= '0') and (buffer[0] <= '9')) {
				is_positive = true;
				i = 0;
			}
			else {	// Other characters
				DEBUG(dbgAddr, "Invalid number\n");
				std::cerr << "\nInvalid number\n";
				delete buffer;
				kernel->machine->WriteRegister(2, 0);
				IncreasePC();
				return;
			}

			// char[] to int
			for (; i < size; i++) {
				if ((buffer[i] >= '0') and (buffer[i] <= '9')) {
					new_number = new_number * 10 + (buffer[i] - '0');
				}
				else {
					DEBUG(dbgAddr, "Invalid number\n");
					std::cerr << "\nInvalid number\n";
					delete buffer;
					kernel->machine->WriteRegister(2, 0);
					IncreasePC();
					return;
				}
			}

			// negative number
			if (not is_positive)
				new_number = -new_number;

			delete buffer;
			kernel->machine->WriteRegister(2, new_number);
			IncreasePC();
			break;
}

		case SC_PrintNum:
{
			DEBUG(dbgAddr, "Prepare to output the number\n");

			// Initialize
			char* buffer = new char[max_int_length + 1];
			int size = 0;
			int number = 0;
			bool is_positive;
			int i = 0;

			if (buffer == NULL) {
				DEBUG(dbgFile, "Not enought memory in the system");
				std::cerr << "NNot enought memory in the system\n";
				kernel->machine->WriteRegister(2, -1);
				delete buffer;
				return;
			}

			// Get number
			number = kernel->machine->ReadRegister(4);

			if (number == 0) {
				kernel->synchConsoleOut->PutChar('0');
				delete buffer;
				kernel->machine->WriteRegister(2, 0);
				IncreasePC();
				return;
			}

			// Get negative number
			if (number < 0) {
				is_positive = false;
				number = -number;
			}
			else {
				is_positive = true;
			}

			// int to char[]
			while (number != 0) {
				buffer[i++] = (number % 10) + '0';
				number /= 10;
				size++;
			}
			if (not is_positive) {
				buffer[i++] = '-';
				size++;
			}

			// convert
			for (i = 0; i < size / 2; i++) {
				int swap = buffer[i];
				buffer[i] = buffer[size - 1 - i];
				buffer[size - 1 - i] = swap;
			}

			// write
			for (i = 0; i < size; i++) {
				kernel->synchConsoleOut->PutChar(buffer[i]);
			}

			delete buffer;
			kernel->machine->WriteRegister(2, 0);
			IncreasePC();
			break;
}

		case SC_ReadChar:
{
			DEBUG(dbgSys, "Prepare to input the character\n");
			char* buffer = new char[max_str_length + 1];
			char c = 0;
			int size = 0;
			while (true) {
				c = kernel->synchConsoleIn->GetChar();
				if ((c == '\0') or (c == '\n')) 
					break;
				else if ((c == '\t') and (size > 0)) {
					buffer[size--] = 0;
				}
				else {
					buffer[size++] = c;
				}
			}

			if(size >= 2) {
				std::cerr << "Just input one character from the keyboard\n";
			}

			c = buffer[0];

			if ((c == '\0') or (c == '\n')) {
				std::cerr << "No character were input\n";
			}

			kernel->machine->WriteRegister(2, c);
			IncreasePC();
			break;
}

		case SC_PrintChar:
{
			DEBUG(dbgAddr, "Prepare to output the character\n");
			char c = kernel->machine->ReadRegister(4);
			kernel->synchConsoleOut->PutChar(c);
			IncreasePC();
			break;
}

		case SC_RandomNum:
{
			DEBUG(dbgAddr, "Prepare to output the random number \n");
			int randomNumber;
			randomNumber = RandomNumber();
			kernel->machine->WriteRegister(2, randomNumber);
			IncreasePC();
			break;
}

		case SC_Create:
{
			DEBUG(dbgFile, "Create a new file");		
			int virtual_address;
			char* file_name;

			virtual_address = kernel->machine->ReadRegister(4);		
			file_name = UserToSystem(virtual_address, max_str_length + 1);

			if (file_name == NULL) {
				DEBUG(dbgFile, "Not enought memory in the system");
				std::cerr << "NNot enought memory in the system\n";
				kernel->machine->WriteRegister(2, -1);
				delete file_name;
				IncreasePC();
				return;
			}

			DEBUG(dbgFile, "Finish reading file named " << file_name);

			if (not kernel->fileSystem->Create(file_name)) {
				std::cerr << "Error create file " << file_name << "\n";
				kernel->machine->WriteRegister(2, -1);
				delete file_name;
				IncreasePC();
				return;
			}

			kernel->machine->WriteRegister(2, 0);
			IncreasePC();
			delete file_name;
			break;
}

		case SC_Open:
{
			DEBUG(dbgFile, "Open file");
			int virtual_address;
			char* file_name;

			virtual_address = kernel->machine->ReadRegister(4);
			file_name = UserToSystem(virtual_address, max_str_length + 1);

			OpenFile* p_address = kernel->fileSystem->Open(file_name);

			DEBUG(dbgFile, "Find the file named " << file_name);

			if (p_address == NULL) {
				DEBUG(dbgFile, "\nError in finding the file named " << file_name);
				std::cerr << "Error in finding the file named " << file_name << "\n";
			}

			delete file_name;
			kernel->machine->WriteRegister(2, (OpenFileId)p_address);
			IncreasePC();
			break;
}

		case SC_Read:
{
			DEBUG(dbgFile, "Read the data in the file");
			// initialize
			char* buffer;
			int virtual_address;
			int size;
			OpenFileId virtual_id;

			// from int Read(char *buffer, int size, OpenFileId id);
			// virtual adderss = (int)buffer
			// size
			// virtual id = id
			virtual_address = kernel->machine->ReadRegister(4);
			buffer = UserToSystem(virtual_address, max_str_length + 1);
			size = kernel->machine->ReadRegister(5);
			virtual_id = kernel->machine->ReadRegister(6);

			// lack of memory
			if (buffer == NULL) {
				DEBUG(dbgFile, "Not enought memory in the system");
				std::cerr << "Not enought memory in the system\n";
				kernel->machine->WriteRegister(2, -1);
				delete buffer;
				IncreasePC();
				return;
			}

			// start reading
			OpenFile* p_address = (OpenFile*)virtual_id;

			if (p_address == NULL) {
				DEBUG(dbgFile, "The file does not exist");
				std::cerr << "The file does not exist\n";
				kernel->machine->WriteRegister(2, -1);
				delete buffer;
				IncreasePC();
				return;
			}

			// touch the write-only file
			if (virtual_id == 1) {
				DEBUG(dbgFile, "Write only file!");
				std::cerr << "Write only file!\n";
				kernel->machine->WriteRegister(2, -1);
				delete buffer;
				IncreasePC();
				return;
			}
			int get = p_address->Read(buffer, size);

			SystemToUser(virtual_address, get, buffer);
			kernel->machine->WriteRegister(2, get);
			delete buffer;
			IncreasePC();
			break;
}

		case SC_Write:
{
			DEBUG(dbgFile, "Read the data in the file");
			// initialize
			char* buffer;
			int virtual_address;
			int size;
			OpenFileId virtual_id;

			// from int Write(char *buffer, int size, OpenFileId id);
			// virtual adderss = (int)buffer
			// size
			// virtual id = id
			virtual_address = kernel->machine->ReadRegister(4);
			buffer = UserToSystem(virtual_address, max_str_length + 1);
			size = kernel->machine->ReadRegister(5);
			virtual_id = kernel->machine->ReadRegister(6);

			// lack of memory
			if (buffer == NULL) {
				DEBUG(dbgFile, "Not enought memory in the system");
				std::cerr << "Not enought memory in the system\n";
				kernel->machine->WriteRegister(2, -1);
				delete buffer;
				IncreasePC();
				return;
			}

			// start writing
			OpenFile* p_address = (OpenFile*)virtual_id;
			if (p_address == NULL) {
				DEBUG(dbgFile, "The file does not exist");
				std::cerr << "The file does not exist\n";
				kernel->machine->WriteRegister(2, -1);
				delete buffer;
				IncreasePC();
				return;
			}

			// touch the read-only file
			if (virtual_id == 0) {
				DEBUG(dbgFile, "Read only file!");
				std::cerr << "Read only file!\n";
				kernel->machine->WriteRegister(2, -1);
				delete buffer;
				IncreasePC();
				return;
			}

			int get = p_address->Write(buffer, size);

			SystemToUser(virtual_address, get, buffer);
			kernel->machine->WriteRegister(2, get);
			delete buffer;
			IncreasePC();
			break;
}

// 		case SC_Seek:
// 			DEBUG(dbgFile, "Seek the position of the file");
// {
// 			// initialize
// 			int position;
// 			OpenFileId virtual_id;

// 			// from int Seek(int position, OpenFileId id);
// 			// position
// 			// virtual id = id
// 			position = kernel->machine->ReadRegister(4);
// 			virtual_id = kernel->machine->ReadRegister(5);

// 			// Start seeking
// 			OpenFile* p_address = (OpenFile*)virtual_id;
// 			if (position == -1) {
// 				Lseek(virtual_id, position, max_str_length);
// 			}
// 			else {
// 				Lseek(virtual_id, position, 0);
// 			}

// 			kernel->machine->WriteRegister(2, 0);
// 			IncreasePC();
// }
// 			break;

		case SC_Close:
{
			DEBUG(dbgFile, "Close file");
			// initialize
			OpenFileId virtual_id;
			virtual_id = kernel->machine->ReadRegister(4);

			// get pointer to the address then delete it
			OpenFile *p_address = (OpenFile*)virtual_id;

			// Problem here: p_address should have NULL value, but this one gets a deleted address instead
			// So you can see: delete p_address is after p_address = NULL (this is wrong)
			// I have no idea how to fix this thing!

			if (p_address != NULL) {
				p_address = NULL;
				delete p_address;
				kernel->machine->WriteRegister(2, 0);
			}
			else {
				DEBUG(dbgFile, "Close the unexisted file");
				std::cerr << "Close the unexisted file\n";
				kernel->machine->WriteRegister(2, -1);
			}
			IncreasePC();
			break;
}

		case SC_Remove:
{
			DEBUG(dbgFile, "Delete a file");		
			int virtual_address;
			char* file_name;

			virtual_address = kernel->machine->ReadRegister(4);		
			file_name = UserToSystem(virtual_address, max_str_length + 1);

			if (file_name == NULL) {
				DEBUG(dbgFile, "Not enought memory in the system");
				std::cerr << "NNot enought memory in the system\n";
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				delete file_name;
				return;
			}

			// Check existed file by opening it
			// The file can be opened more than once, so this is OK
			// The purpose of this is to delete the address (if the file is open)
			OpenFile* p_address = kernel->fileSystem->Open(file_name);

			if (p_address == NULL) {
				DEBUG(dbgFile, "Non-existed file " << file_name);
				std::cerr << "Non-existed file " << file_name << "\n";
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				delete file_name;
				return;
			}

			if (not kernel->fileSystem->Remove(file_name)) {
				DEBUG(dbgFile, "Error deleting file " << file_name);
				std::cerr << "Error deleting file " << file_name << "\n";
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				delete file_name;
				if (p_address != NULL) {
					p_address = NULL;	// I know this is a wrong syntax!
					delete p_address;	// See SC_Remove for understanding why
				}
				return;
			}

			kernel->machine->WriteRegister(2, 0);
			IncreasePC();
			delete file_name;
			if (p_address != NULL) {
				p_address = NULL;
				delete p_address;
			}
			break;
}

    	case SC_Add:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4), /* int op2 */(int)kernel->machine->ReadRegister(5));
			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);
			/* Modify return point */
			IncreasePC();
			break;

    	default:
			cerr << "Unexpected system call " << type << "\n";
			break;
      	}
      	break;

    case NoException:
		DEBUG(dbgSys, "Everything ok!");
		break;

	case PageFaultException:
		DEBUG(dbgSys, "No valid translation found");
		break;

	case ReadOnlyException:
		DEBUG(dbgSys, "Write attempted to page marked \"read-only\"");
		break;

	case BusErrorException:
		DEBUG(dbgSys, "Translation resulted in an invalid physical address");
		break;

	case AddressErrorException:
		DEBUG(dbgSys, "Unaligned reference or one that was beyond the end of the address space");
		break;

	case OverflowException:
		DEBUG(dbgSys, "Integer overflow in add or sub.");
		break;

	case IllegalInstrException:
		DEBUG(dbgSys, "Unimplemented or reserved instr.");
		break;

	case NumExceptionTypes:
		DEBUG(dbgSys, "Get Num Exception Types.");
		break;
    }
    //ASSERTNOTREACHED();
}
