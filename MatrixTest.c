/***
* Asher Etherington
* MatrixTest.c
* Personal testing script for Matrix.c
***/ 
// compile:   gcc -std=c17 -Wall -o test List.c Matrix.c MatrixTest.c

#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Matrix.h"


int main() {
    Matrix matrix1 = newMatrix(3);
    Matrix matrix2 = newMatrix(3);
    printf("Matrix has been made.\n");
    makeZero(matrix1);
    printf("matrix1 = \n");
    
    printMatrix(stdout, matrix1);
    if(equals(matrix1, matrix2)){
        printf("The matrices are equal.\n");
    }else{
        printf("The matrices are not equal.\n");
    }
    
    changeEntry(matrix2, 1, 1, 1);
    changeEntry(matrix2, 1, 2, 2);
    changeEntry(matrix2, 1, 3, 3);
    changeEntry(matrix2, 2, 1, 4);
    changeEntry(matrix2, 2, 2, 5);
    changeEntry(matrix2, 2, 3, 6);
    changeEntry(matrix2, 3, 1, 7);
    changeEntry(matrix2, 3, 2, 8);
    changeEntry(matrix2, 3, 3, 0);
    printf("\n\n");
    printf("matrix2 = \n");
    printMatrix(stdout, matrix2);
    changeEntry(matrix2, 1, 2, 2);
    changeEntry(matrix2, 1, 3, 3);
    changeEntry(matrix2, 1, 1, 1);
    changeEntry(matrix2, 2, 1, 4);
    changeEntry(matrix2, 2, 2, 5);
    changeEntry(matrix2, 2, 3, 6);
    changeEntry(matrix2, 3, 1, 7);
    changeEntry(matrix2, 3, 2, 8);
    changeEntry(matrix2, 3, 3, 0);
    printf("matrix2 = \n");
    printMatrix(stdout, matrix2);
    printf("\n\n");
    printf("NNZ of matrix 2 = %d\n", NNZ(matrix2));

    Matrix matrix3 = scalarMult(1.5, matrix2);
    printf("matrix3 = \n");
    printMatrix(stdout, matrix3);

    if(equals(matrix3, matrix2)){
        printf("The matrices are equal.\n");
    }else{
        printf("The matrices are not equal.\n");
    }

    Matrix matrix4 = product(matrix3, matrix2);
    printf("matrix4 = \n");
    printMatrix(stdout, matrix4);

    /*extra equals test because it was failing*/
    Matrix A = newMatrix(10);
    Matrix B = newMatrix(10);

    changeEntry(A, 1, 1, 1);
    changeEntry(A, 1, 3, 1);
    changeEntry(B, 1, 1, 1);
    changeEntry(B, 1, 3, 1);

    changeEntry(A, 1, 3, 0);

    changeEntry(A, 1, 1, 0);
    makeZero(B);
    changeEntry(A, 10, 10, 10);
    changeEntry(B, 10, 10, 10);
    printMatrix(stdout, A);
    printMatrix(stdout, B);
    if(equals(A, B)){
        printf("\n\n\n\nA and B are equal\n");
    }else{
        printf("\n\n\n\nA and B are NOT equal\n");
    }

    freeMatrix(&matrix1);
    freeMatrix(&matrix2);
    freeMatrix(&matrix3);
    freeMatrix(&matrix4);
    freeMatrix(&A);
    freeMatrix(&B);

    printf("matrix has been succesfully freed.\n");
    return 0;
}
