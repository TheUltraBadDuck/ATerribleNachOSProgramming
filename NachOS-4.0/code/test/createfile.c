#include "syscall.h"


int main() {
    OpenFileId i;
    OpenFileId j;
    
    if (Create("wow.txt") == -1) {
        PrintNum(100);
    }
    else {
        if (Create("wow2.txt") == -1) {
            PrintNum(100000);
        }
        else {
            i = Open("wow.txt");
            j = Open("wow2.txt");
            Open("wow.txt");
        }
    }

//    PrintNum(i);
//    PrintNum(j);

    PrintNum(Close(i));
    PrintNum(Close(i));
    PrintNum(Close(j));
    PrintNum(Close(j));
    Halt();
}
