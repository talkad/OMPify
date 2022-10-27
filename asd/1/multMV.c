#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <multMV.h>

#define IND_mult(x, y, z) ((x) + (y)*nx + (z)*ny*nx)

void __attribute__((inline)) initSpMat(SpMatrix *mat, int nz, int nRows) {
    mat->nz = nz;
    mat->nRows = nRows;
    mat->value = (TYPE *) aligned_alloc(32, sizeof(TYPE) * nz);
    mat->col = (int *) aligned_alloc(32, sizeof(int) * nz);
    mat->rowIndex = (int *) calloc(nRows + 1, sizeof(int));
}
