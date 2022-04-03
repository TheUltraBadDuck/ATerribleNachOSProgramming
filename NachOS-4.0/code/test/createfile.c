#include "syscall.h"


int main() {
    OpenFileId i;
    OpenFileId j;

    PrintString("Prepare to create a file 'wow.txt'\n");
    if (Create("wow.txt") == -1) {
        PrintString("Error create file 'wow.txt'\n");
    }

    else {
        PrintString("Successfully create 'wow.txt'\n");

        PrintString("Prepare to create a file 'wow2.txt'\n");
        if (Create("wow2.txt") == -1) {
            PrintString("Error create file 'wow.txt'\n");
        }

        else {
            PrintString("Successfully create 'wow.txt'\n");

            PrintString("Open 'wow.txt' once and 'wow2.txt' twice\n");
            i = Open("wow.txt");
            j = Open("wow2.txt");
            Open("wow.txt");
        }
    }

    PrintString("Close both 'wow.txt' and 'wow2.txt' twice\n");
    PrintNum(Close(i));
    PrintNum(Close(i));
    PrintNum(Close(j));
    PrintNum(Close(j));

    Halt();
}
