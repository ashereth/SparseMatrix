/***
* Asher Etherington
* Sparse.c
* Performs matrix operations on sparse matrices
***/

#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"
#include "List.h"

int main(int argc, char * argv[]){
    // check command line for correct number of arguments
    if( argc != 3 ){
      printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(1);
    }


    //open infile for reading
    FILE *infile = fopen(argv[1], "r");
    //open outfile for writing
    FILE *outfile = fopen(argv[2], "w");

    //read first line and make matrices
    int n, NNZA, NNZB;
    fscanf(infile, "%d %d %d", &n, &NNZA, &NNZB);

    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);
    //populate matrix A
    int row, col;
    double val;
    for(int i=0; i<NNZA; i++){
        fscanf(infile, "%d %d %le", &row, &col, &val);
        changeEntry(A, row, col, val);
    }
    fprintf(outfile, "A has %d non-zero entries:\n", NNZA);
    printMatrix(outfile, A);
    fprintf(outfile, "\n");
    //populate matrix B
    for(int i=0; i<NNZB; i++){
        fscanf(infile, "%d %d %le", &row, &col, &val);
        changeEntry(B, row, col, val);
    }
    fprintf(outfile, "B has %d non-zero entries:\n", NNZB);
    printMatrix(outfile, B);
    fprintf(outfile, "\n");

    fprintf(outfile, "(1.5) *A =\n");
    Matrix A15 = scalarMult(1.5, A);
    printMatrix(outfile, A15);
    fprintf(outfile, "\n");

    fprintf(outfile, "A+B =\n");
    Matrix AplusB = sum(A, B);
    printMatrix(outfile, AplusB);
    fprintf(outfile, "\n");

    fprintf(outfile, "A+A =\n");
    Matrix AplusA = sum(A, A);
    printMatrix(outfile, AplusA);
    fprintf(outfile, "\n");


    fprintf(outfile, "B-A =\n");
    Matrix BminA = diff(B, A);
    printMatrix(outfile, BminA);
    fprintf(outfile, "\n");

    fprintf(outfile, "A-A =\n");
    Matrix AminA = diff(A, A);
    printMatrix(outfile, AminA);
    fprintf(outfile, "\n");

    fprintf(outfile, "Transpose(A) =\n");
    Matrix AT = transpose(A);
    printMatrix(outfile, AT);
    fprintf(outfile, "\n");

    fprintf(outfile, "A*B =\n");
    Matrix AB = product(A, B);
    printMatrix(outfile, AB);
    fprintf(outfile, "\n");

    fprintf(outfile, "B*B =\n");
    Matrix BB = product(B, B);
    printMatrix(outfile, BB);

    //free and close everything
    fclose(infile);
    fclose(outfile);
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&AB);
    freeMatrix(&BB);
    freeMatrix(&AT);
    freeMatrix(&AminA);
    freeMatrix(&AplusA);
    freeMatrix(&BminA);
    freeMatrix(&A15);
    freeMatrix(&AplusB);
}