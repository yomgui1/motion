#ifndef MAT_MATRIX_H
#define MAT_MATRIX_H

#include <math/array.h>

/* Dense matrix (Implemented using Array) */
typedef struct MAT_Matrix
{
    MAT_Array array;
    unsigned int ncols;
    unsigned int nrows;
} MAT_Matrix;

#endif /* MAT_MATRIX_H */
