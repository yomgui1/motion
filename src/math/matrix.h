#ifndef MAT_MATRIX_H
#define MAT_MATRIX_H

#include <math/array.h>

/* Dense matrix (Implemented using Array) */
typedef struct MAT_Matrix
{
    MAT_Array array;
    unsigned int width;
    unsigned int height;
} MAT_Matrix;

#endif /* MAT_MATRIX_H */
