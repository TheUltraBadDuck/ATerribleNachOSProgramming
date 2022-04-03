#include "syscall.h"

int main() {
    char buffer[1024 + 1];
    int size;

    int l;
    OpenFileId id;
    size = 68;

    id = Open("exist.txt");

    if (id == 0) {
        PrintString("Error opening file exist.txt");
        Halt();
    }

    Read(buffer, size, id);
    PrintString("Data from exist.txt:\n");
    PrintString(buffer);
    PrintChar('\n');

    Close(id);
    Halt();
}
