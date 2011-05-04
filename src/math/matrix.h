#ifndef MAT_MATRIX_H
#define MAT_MATRIX_H

#include <math/array.h>

/* WARNING: unsafe macro */
#define MAT_SAMEMATRIXTYPE(left, right) MAT_SAMEARRAYTYPE((MAT_Array *)(left), (MAT_Array *)(right))

#define MAT_MATRIXTYPE_FLOAT MAT_ARRAYTYPE_FLOAT
#define MAT_MATRIXTYPE_DOUBLE MAT_ARRAYTYPE_DOUBLE

/* Dense matrix (Implemented using Array) */
typedef struct MAT_Matrix
{
    MAT_Array array;
    unsigned int nrows;
    unsigned int ncols;
} MAT_Matrix;

extern MAT_Matrix *MAT_InitMatrix(
    MAT_Matrix *mat,
    unsigned int nrows,
    unsigned int ncols,
    int type,
    void *data);
extern MAT_Matrix *MAT_AllocMatrix(
    unsigned int nrows,
    unsigned int ncols,
    int type,
    int clear,
    MAT_Array *data);
extern void MAT_FreeMatrix(MAT_Matrix *mat);
extern void MAT_ZeroMat(MAT_Matrix *mat);
extern void MAT_SetIdentity(MAT_Matrix *mat);

#endif /* MAT_MATRIX_H */
