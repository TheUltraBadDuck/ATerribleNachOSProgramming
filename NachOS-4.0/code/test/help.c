#include "syscall.h"

int main() {
    PrintString("Information:\n");
    PrintString("sort      : Running bubble sort\n");
    PrintString("ascii     : Running a list of ascii characters\n");
    PrintString("createfile: Make two file 'wow.txt' and 'wow2.txt'\n");
    PrintString("cat       : Read the file 'existenz.txt' (exist) and 'null.txt' (non-exist)\n");
    PrintString("copy      : Copy the data from 'from.txt' to 'to.txt'\n");
    PrintString("delete    : Create binary file then delete it\n");
    PrintChar('\n');
    PrintString("syntax: ~/nachos/NachOS-4.0/code$\n");
    PrintString("                                  ./build.linux/nachos -x ./test/<any existed file such as 'help'>\n");
    PrintChar('\n');
    
    Halt();
}