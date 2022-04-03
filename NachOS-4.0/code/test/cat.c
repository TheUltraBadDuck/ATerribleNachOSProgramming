#include "syscall.h"

int main() {
    char buffer[1024 + 1];
    int size;

    int l;
    OpenFileId id;
    size = 68;

    id = Open("exist.txt");

    Read(buffer, size, id);
    PrintString("Data from exist.txt: ");
    PrintString(buffer);
    PrintChar('\n');

    Close(id);
    Halt();
}
