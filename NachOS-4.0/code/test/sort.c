// /* sort.c 
//  *    Test program to sort a large number of integers.
//  *
//  *    Intention is to stress virtual memory system.
//  *
//  *    Ideally, we could read the unsorted array off of the file system,
//  *	and store the result back to the file system!
//  */


/*
#define UNIX
#define UNIX_DEBUG
*/

#ifdef UNIX
#include <stdio.h>
#define Exit exit
#else
#include "syscall.h"
#endif /* UNIX */

#define SIZE (20)

int A[SIZE];	/* size of physical memory; with code, we'll run out of space!*/

int main()
{
    int i, j, tmp;

    /* first initialize the array, in reverse sorted order */
    for (i = 0; i < SIZE; i++) {
        A[i] = (SIZE - 1) - i;
    }

    /* then sort! */
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < (SIZE-1); j++) {
	   if (A[j] > A[j + 1]) {	/* out of order -> need to swap ! */
	      tmp = A[j];
	      A[j] = A[j + 1];
	      A[j + 1] = tmp;
    	   }
        }
    }

#ifdef UNIX_DEBUG
    for (i = 0; i < SIZE; i++) {
        printf("%4d ", A[i]);
	if (((i+1) % 15) == 0) {
		printf("\n");
        }
        if (A[i] != i) {
            fprintf(stderr, "Out of order A[%d] = %d\n", i, A[i]);
            Exit(1);
        }   
    }
    printf("\n");
#else
    for (i = 0; i < SIZE; i++) {
        PrintNum(A[i]);
        PrintChar(' ');
        if (((i + 1) % 15) == 0)
            PrintChar('\n');
        if (A[i] != i) {
            PrintChar('!');
            PrintChar('!');
            PrintChar('!');
//            PrintString("Out of order A[");
            PrintNum(i);
            PrintString("] = ");
            PrintNum(A[i]);
            PrintString("\n");
        }
    }

#endif /* UNIX_DEBUG */

    for (i=0; i<SIZE; i++) {
        if (A[i] != i) {
            Exit(1);
        }   
    }

    Exit(0);
}

// #include "syscall.h"
// #define MAX_CHAR_SIZE 20


// void printArray(int arr[], int i) {
//     PrintChar('\n');
//     for (i = 0; i < MAX_CHAR_SIZE; i++) {
//         PrintNum(arr[i]);
//         PrintChar('\t');
//         if ((i + 1) % 8 == 0)
//             PrintChar('\n');
//     }
//     PrintChar('\n');
// }


// void bubbleSort(int arr[], char order) {       // true: from smallest to largest, false: from largest to smallest
//     int i, j;
//     int c;
//     char swapping;
//     for (i = 0; i < MAX_CHAR_SIZE - 1; i++) {
//         swapping = 0;
//         for (j = 0; j < MAX_CHAR_SIZE - i - 1; j++) {
//             if (order) {
//                 if (arr[j] > arr[j + 1]) {
//                     c = arr[j];
//                     arr[j] = arr[j + 1];
//                     arr[j + 1] = c;
//                     swapping = 1;
//                 }
//             }
//             else {
//                 if (arr[j] < arr[j + 1]) {
//                     c = arr[j];
//                     arr[j] = arr[j + 1];
//                     arr[j + 1] = c;
//                     swapping = 1;
//                 }
//             }
//         }

//         if (swapping == 0) {
//             break;
//         }
//     }
// }


// int main() {
//     int arr[MAX_CHAR_SIZE];
//     int i;
//     char c;
//     char order;

//     for (i = 0; i < MAX_CHAR_SIZE; i++) {
//         arr[i] = RandomNum() % 255;
//     }

//     //PrintString("Before: ");
//     printArray(arr, i);

//     //PrintString("Sorting from smallest to largest (Y) or sorting from smallest to largest (N)? Default Y. ");
//     c = ReadChar();
//     if ((c == 'N') || (c == 'n'))
//         order = 0;
//     else
//         order = 1;

//     bubbleSort(arr, order);

//     //PrintString("After: ");
//     printArray(arr, i);

//     Halt();
// }
