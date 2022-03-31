#include "syscall.h"


int main() {
    OpenFileId i;
    if (Create("wow.txt") == -1) {
        PrintNum(100);
    }
    else {
//        PrintNum(-100);
        i = Open("wow.txt");
    }

    Close(i);
    Halt();
}
