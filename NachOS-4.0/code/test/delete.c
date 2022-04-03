#include "syscall.h"


int main() {
    
    char buffer[255];
    int size;

    PrintString("Input file name: ");
    ReadString(buffer, size);

    if (Create(buffer) == -1) {
        PrintString("Error create file ");
        PrintString(buffer);
        PrintChar('\n');
        Halt();
        return;
    }

    PrintString("See the file ");
    PrintString(buffer);
    PrintString(" First\n");
    PrintString("Input anything to continue: ");
    ReadChar();

    Open(buffer);

    if (Remove(buffer) == -1) {
        PrintString("Error remove once file ");
        PrintString(buffer);
        PrintChar('\n');
        Halt();
        return;
    }

    if (Remove(buffer) == -1) {
        PrintString("Error remove twice file ");
        PrintString(buffer);
        PrintChar('\n');
        Halt();
        return;
    }
    
    PrintString("Successfully delete file ");
    PrintString(buffer);
    PrintChar('\n');

    Halt();
}
