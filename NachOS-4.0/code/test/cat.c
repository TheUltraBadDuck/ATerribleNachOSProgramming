#include "syscall.h"

int main() {
    char buffer[1024 + 1];
    int size;

    int l;
    OpenFileId id;
    //PrintString("Input file name: ");
    //ReadString(file_name, size);
    size = 68;

    id = Open("wow.txt");

    Read(buffer, size, id);
    //PrintString(file_name_size);

    Close(id);
    Halt();
}
