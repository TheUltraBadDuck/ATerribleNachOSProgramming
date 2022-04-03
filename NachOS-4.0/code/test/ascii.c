#include "syscall.h"


int main() {
    int i;
    char a;

    i = 0;
    a = 0;

    for (i = 0; i < 255; i++) {
        PrintNum(i);
        PrintChar(')');
        PrintChar('\t');
        PrintChar(a++);
        PrintChar('\n');
    }
    Halt();
}
