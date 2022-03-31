#include "syscall.h"

int main() {
    int i;
    i = ReadNum();
    PrintNum(i);
    Halt();
}