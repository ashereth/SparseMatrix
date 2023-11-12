/***
* Asher Etherington
* ListTest.c
* Personal testing script for List.c
***/ 
// compile:   gcc -std=c17 -Wall -o test List.c ListTest.c

#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Matrix.h"
 
int main() {
    List myList = newList(); // Create a new empty list
    printf("list made\n");

        // create array to be added to myList via pointer

    int size = 5; // Size of the array
    int *dynamicArray; // Pointer to the dynamically allocated array

    // Allocate memory for the array
    dynamicArray = (int *)malloc(size * sizeof(int));

    if (dynamicArray == NULL) {
        printf("Memory allocation failed. Exiting...\n");
        return 1;
    }

    // Initialize the elements of the array
    for (int i = 0; i < size; i++) {
        dynamicArray[i] = i * 2;
    }
        //finished making array to be added to list via pointer

    append(myList, &dynamicArray);
    printf("successfully appended array to list\n");
    printf("value of array pointer = %p\n", &dynamicArray);
    printList(stdout, myList);
    printf("\n");
    freeList(&myList);
    printf("List successfully freed.\n");
    return 0;
}
