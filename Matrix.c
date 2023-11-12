/***
* Asher Etherington
* Matrix.c
* includes implemetations of functions defined in Matrix.h
***/ 
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include "Matrix.h"
#include "List.h"

// typedefs
typedef struct EntryObj* Entry;

typedef struct MatrixObj* Matrix;

typedef struct EntryObj{
    int col;
    double val;
}EntryObj;

typedef struct MatrixObj{
    int size;// matrix is n x n 
    int NNZ;// counter for the number of nonzero elements
    List* row; //pointer to 0th element of the first row
}MatrixObj;

// initialition  procedures

//newEntry()
Entry newEntry(int col, double val){
    Entry E = malloc(sizeof(EntryObj));
    assert(E!=NULL);
    E->col = col;
    E->val = val;
    return E;
}

//freeEntry()
void freeEntry(Entry* E){
    if( E!=NULL && *E!=NULL ){
      free(*E);
      *E = NULL;
   }
}


// newMatrix()
// Returns a reference to a new nXn Matrix object in the zero state.
Matrix newMatrix(int n){
    //initialize new matrix
    Matrix M;
    M = malloc(sizeof(MatrixObj));
    //allocate memory for array of rows
    //  size n+1 because we skip 0th index
    M->row = (List *)malloc((n+1) * sizeof(List));
    //initialize all parts of the matrix
    M->size = n;
    M->NNZ = 0;
    //loop through every row of the array making a new list for each
    // row skip 0th entries
    for(int i = 1; i<=n; i++){
        //initialize a new list for the row
        List row = newList();
        //add row to matrix
        M->row[i] = row;
    }
    return M;
}

// freeMatrix()
// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix* pM){
    makeZero(*pM);
    if (pM != NULL && *pM != NULL){
        // loop through all rows of matrix and free the coressponding lists
        for(int i = 1; i<=(*pM)->size; i++){
            //loop through the list freeing all entries
            List L = (*pM)->row[i];
            //free the list entirely
            freeList(&L);
        }
        free((*pM)->row);
        free(*pM);
        *pM = NULL;
    }else{
        printf("Matrix Error: calling freeMatrix() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
}

// Access functions
// size()
// Return the size of square Matrix M.
int size(Matrix M){
    if( M==NULL ){
      printf("Matrix Error: calling size() on NULL Matrix reference\n");
      exit(EXIT_FAILURE);
   }
   return(M->size);
}

// NNZ()
// Return the number of non-zero elements in M.
int NNZ(Matrix M){
    if( M==NULL ){
      printf("Matrix Error: calling NNZ() on NULL Matrix reference\n");
      exit(EXIT_FAILURE);
   }
   return(M->NNZ);
}

// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B){
    // make sure A and B exist
    if(A==NULL || B==NULL){
        printf("Matrix Error: calling equals() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    //make sure both matrices are the same size
    if(B->size!=A->size){
        return 0;
    }

    // loop through every row of the matrices
    for(int i = 1; i<=A->size; i++){
        List rowA = A->row[i];
        List rowB = B->row[i];

        //make sure the rows are the same length
        if(length(rowA)!=length(rowB)){
            return 0;
        }else{//if the rows are the same length iterate down them and check each entry col and val
            moveFront(rowA);
            moveFront(rowB);
            while(index(rowA)!=-1){
                Entry entryA = get(rowA);
                Entry entryB = get(rowB);
                //compare the val and col of each entry and if they dont match return false
                if(entryA->val!=entryB->val || entryA->col!=entryB->col){
                    return 0;
                }
                moveNext(rowA);
                moveNext(rowB);
            }
        }
    }
    return 1;
}


// Manipulation procedures

// makeZero()
/** free all entries dont just set them to 0 **/ 
// Re-sets M to the zero Matrix state.
void makeZero(Matrix M){
     // make sure M exists
    if(M==NULL){
        printf("Matrix Error: calling makeZero() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    // loop through every row of the matrix
    for(int i = 1; i<=M->size; i++){
        List row = M->row[i];
        //loop through each column of each row
        moveFront(row);
        while(index(row)!=-1){
            //get the specific entry
            Entry E = get(row);
            freeEntry(&E);
            //set the value for that entry to 0
            //increment to next entry
            moveNext(row);
        }
        //clear the freed entries from the list
        clear(row);
    }
    //set the number of non zero entries to 0
    M->NNZ=0;
}

// changeEntry()
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x){
    if(1>i || i>size(M) || 1>j || j>size(M)){
        printf("Matrix Error: calling changeEntry with indices that are out of bounds\n");
        exit(EXIT_FAILURE);
    }
    //get the specified row
    List row = M->row[i];
    //if length of row is 0 and x!=0 then prepend entry
    
    if(x!=0){
        if(length(row)==0){//correct
                
                Entry E = newEntry(j, x);
                append(row, E);
                M->NNZ++;
                return;
        }else{//if x!=0 and length of row>0
            //keep track on if entry has been added yet
            int added = 0;
            //walk down row
            moveFront(row);
            while(index(row)!=-1){
                Entry E = get(row);
                if(E->col==j){//if the columns line up change the value of that entry
                    E->val=x;
                    added++;
                    M->NNZ++;
                    return;
                    //if we have passed the column that it should be then insert before
                }else if(E->col>j){
                    Entry new = newEntry(j, x);
                    insertBefore(row, new);
                    added++;
                    M->NNZ++;
                    return;
                }
                //increment to next column
                moveNext(row);
            }
            //if we havent added entry yet just append the entry
            if(added==0){
                Entry E = newEntry(j, x);
                append(row, E);
            }
            M->NNZ++;
        }
    }else{//if x==0
        //if the length of the row is 0 do nothing
        if(length(row)>0){
            //walk down the row to find a column that matches
            // if there isnt a column that matches then theres no need to do anything
            moveFront(row);
            while(index(row)!=-1){
                Entry E = get(row);
                if(E->col==j){//if the columns line up change the value of that entry
                    //delete the entry at that column and break out of loop
                    //delete(row);
                    E->val = x;
                    M->NNZ--;//decriment number of nonzeros
                    return;
                }
                moveNext(row);
            }
        }
    }
}

// Matrix Arithmetic operations

// copy()
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A){
    if(A==NULL){
        printf("Matrix Error: calling copy() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    //create a new matrix
    Matrix new = newMatrix(A->size);
    // loop through every entry in A
    for(int i = 1; i<=A->size; i++){
        List row = A->row[i];
        //loop through each column of each row
        moveFront(row);
        while(index(row)!=-1){
            //get the specific entry
            Entry entry = get(row);
            //set the value for that entry to 0
            changeEntry(new, i, entry->col, entry->val);
            //increment to next entry
            moveNext(row);
        }
    }
    return new;
}

// transpose()
// Returns a reference to a new Matrix object representing the transpose
// of A.
Matrix transpose(Matrix A){
    if(A==NULL){
        printf("Matrix Error: calling transpose() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    Matrix new = newMatrix(A->size);
    // loop through every entry in A
    for(int i = 1; i<=A->size; i++){
        List row = A->row[i];
        //loop through each column of each row
        moveFront(row);
        while(index(row)!=-1){
            //get the specific entry
            Entry entry = get(row);
            changeEntry(new, entry->col, i, entry->val);
            //increment to next entry
            moveNext(row);
        }
    }
    return new;
}

// scalarMult()
// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A){
    if(A==NULL){
        printf("Matrix Error: calling scalar() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    //create a new matrix
    Matrix new = newMatrix(A->size);
    // loop through every entry in A
    for(int i = 1; i<=A->size; i++){
        List row = A->row[i];
        //loop through each column of each row
        moveFront(row);
        while(index(row)!=-1){
            //get the specific entry
            Entry entry = get(row);
            //set the value for that entry to 0
            changeEntry(new, i, entry->col, entry->val * x);
            //increment to next entry
            moveNext(row);
        }
    }
    return new;
}

//helper funtion for sum and diff returns value at a given column given the row
double getVal(List row, int col){
    //set value to be 0 initially
    double value = 0;
    //loop through list comparing column values and if they match set value
    moveFront(row);
    while(index(row)!=-1){
        Entry current = get(row);
        if(current->col==col){
            value = current->val;
        }
        moveNext(row);
    }
    return value;
}

// sum()
// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B){
     // make sure A and B exist
    if(A==NULL || B==NULL){
        printf("Matrix Error: calling sum() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    //make sure both matrices are the same size
    if(B->size!=A->size){
        printf("Matrix Error: calling sum() on Matrices of differing size\n");
        exit(EXIT_FAILURE);
    }
    //create a new matrix thats the same size as A and B
    Matrix new = newMatrix(A->size);

    // Loop through every entry in A and add it to the corresponding entry in B
    for (int i = 1; i <= A->size; i++) {
        List rowA = A->row[i];
        List rowB = B->row[i];
        //loop through every column checking values
        for(int j = 0; j<=size(A); j++){
            //get values at A and B
            double valA = getVal(rowA, j);
            double valB = getVal(rowB, j);
            double sum = valA + valB;
            //if the sum isn't 0 add it to the new matrix
            if(sum!=0){
                changeEntry(new, i, j, sum);
            }
        }
    }
    return new;   
}



// diff()
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B){
    // make sure A and B exist
    if(A==NULL || B==NULL){
        printf("Matrix Error: calling diff() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    //make sure both matrices are the same size
    if(B->size!=A->size){
        printf("Matrix Error: calling diff() on Matrices of differing size\n");
        exit(EXIT_FAILURE);
    }
    //create a new matrix thats the same size as A and B
    Matrix new = newMatrix(A->size);

    // Loop through every entry in A and add it to the corresponding entry in B
    for (int i = 1; i <= A->size; i++) {
        List rowA = A->row[i];
        List rowB = B->row[i];
        //loop through every column checking values
        for(int j = 0; j<=size(A); j++){
            //get values at A and B
            double valA = getVal(rowA, j);
            double valB = getVal(rowB, j);
            double sum = valA - valB;
            //if the sum isn't 0 add it to the new matrix
            if(sum!=0){
                changeEntry(new, i, j, sum);
            }
        }
    }
    return new; 
}

//helper function for product, calculates the dot product of 2 lists/rows
double vectorDot(List A, List B, int length){
    double value = 0;
    //loop through the length of the lists
    for(int i = 1; i<=length; i++){
        value += getVal(A, i) * getVal(B, i);
    }
    return value;
}

// product()
// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B){
    // make sure A and B exist
    if(A==NULL || B==NULL){
        printf("Matrix Error: calling product() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    //make sure both matrices are the same size
    if(B->size!=A->size){
        printf("Matrix Error: calling product() on Matrices of differing size\n");
        exit(EXIT_FAILURE);
    }
    //initialize needed matrices and variables
    Matrix M = newMatrix(size(A));
    Matrix T = transpose(B);
    double value;
    for(int x=1;x<=size(A); x++){
        if(length(A->row[x])==0){
            continue;
        }else{
            for(int i=1;i<=size(A);i++){
                if(length(T->row[i])==0){
                    continue;
                }else{
                    value=vectorDot(A->row[x], T->row[i], size(A));
                    changeEntry(M, x, i, value); 
                }
            }
        }
    }
    //free unneeded matrix
    freeMatrix(&T);
    return M;
}

// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows
// are not printed. Each non-zero row is represented as one line consisting
// of the row number, followed by a colon, a space, then a space separated
// list of pairs "(col, val)" giving the column numbers and non-zero values
// in that row. The double val will be rounded to 1 decimal point.
void printMatrix(FILE* out, Matrix M){
    // loop through every row of the matrix
    for(int i = 1; i<=M->size; i++){
        
        List row = M->row[i];
        //loop through each column of each row
        if(length(row)==0){
            continue;
        }
        fprintf(out, "%d: ", i);
        moveFront(row);            
        while(index(row)!=-1){
            //get the specific entry
            Entry entry = get(row);
            if(entry->val!=0){
                fprintf(out, "(%d, %.1lf) ", entry->col, entry->val);
                moveNext(row);
            }
        }
        fprintf(out, "\n");
    }
        
}