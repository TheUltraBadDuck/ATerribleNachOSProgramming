#include "syscall.h"


void string_copy(int l, char old[], char new[], int size);


int main() {
    int l;
    OpenFileId i;
    char buffer[100 + 1];
    int size;
    int real_size;

    i = Open("wow.txt");
    size = 68;

    real_size = Read(buffer, size, i);

    Write(buffer, real_size, i);

    Close(i);
    Halt();
}


void string_copy(int l, char old[], char new[], int size) {
    for (l = 0; l < size; l++) {
        new[l] = old[l];
    }
}
