#include "syscall.h"

int main() {
    int l;
    OpenFileId i, j;
    char buffer[100 + 1];
    int size;
    int real_size;

    i = Open("from.txt");
    i = Open("from.txt");
    j = Open("to.txt");
    size = 68;

    PrintString("Check from.txt and to.txt, then input any key to continue\n");
    ReadChar();

    real_size = Read(buffer, size, i);
    Write(buffer, real_size, j);

    Close(i);
    Close(j);
    Close(j);
    Halt();
}
// int main() {
//     int success_from;
//     int success_to;

//     OpenFileId id_from;

//     int size;
//     char text[] = "A terrible NachOS Programming";

//     success_from = Create("from.txt");
//     success_to = Create("to.txt");

//     if ((success_from == -1) || (success_to == -1)) {
//         Halt();
//         return 0;
//     }

//     id_from = Open("from.txt");
    
//     //PrintString("Id of from.txt: ");
//     PrintNum(id_from);
//     //PrintChar('\n');

//     size = sizeof(text) / sizeof(text[0]) - 1;

//     Write(text, size, id_from);

//     Close(id_from);

//     Halt();
// }
