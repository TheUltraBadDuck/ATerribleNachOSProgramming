#include "syscall.h"


int main() {
    if (Create("streich.bin") == -1) {
        //PrintString("Error create file\n");
        PrintNum(111);
        Halt();
        return;
    }

    //PrintString("See the file streich.bin\n");
    //PrintString("Input anything to continue: ");
    PrintChar(':');
    ReadNum();

    //Open("streich.bin");

    if (Remove("streich.bin") == -1) {
        //PrintString("Error deleting file\n");
        PrintNum(222);
        Halt();
        return;
    }

    PrintChar(':');
    ReadNum();

    if (Remove("streich.bin") == -1) {
        //PrintString("Error deleting file\n");
        PrintNum(333);
        Halt();
        return;
    }
    
    //PrintString("Successfully delete file streich.bin");

    Halt();
}
