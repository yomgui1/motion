#ifndef MAT_MATRIX_H
#define MAT_MATRIX_H

#include <math/array.h>
#include <math/vector.h>

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
extern MAT_Matrix *MAT_AllocMatrixLike(const MAT_Matrix *friendmatrix, int copydata);
extern void MAT_FreeMatrix(MAT_Matrix *mat);
extern void MAT_ZeroMatrix(MAT_Matrix *mat);
extern void MAT_SetIdentityMatrix(MAT_Matrix *mat);
extern void MAT_InPlaceTransposeMatrix(MAT_Matrix *mat);
extern MAT_Matrix *MAT_TransposeMatrix(MAT_Matrix *mat);
extern void MAT_InPlaceScalarMultiplyMatrix(double value, MAT_Matrix *mat);
extern MAT_Matrix *MAT_ScalarMultiplyMatrix(double value, MAT_Matrix *mat);
extern int MAT_InPlaceMultiplyMatrix(MAT_Matrix *mat1, MAT_Matrix *mat2);
extern MAT_Matrix *MAT_MultiplyMatrix(MAT_Matrix *mat1, MAT_Matrix *mat2);
extern int MAT_InPlaceMultiplyMatrixVector(MAT_Matrix *mat, MAT_Vector *vec);
extern MAT_Vector *MAT_MultiplyMatrixVector(MAT_Matrix *mat, MAT_Vector *vec);

#endif /* MAT_MATRIX_H */
