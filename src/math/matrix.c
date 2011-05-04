#include "math/matrix.h"

#include <stdlib.h>

MAT_Matrix *MAT_InitMatrix(
    MAT_Matrix *mat,
    unsigned int nrows,
    unsigned int ncols,
    int type,
    void *data)
{
    mat->nrows = nrows;
    mat->ncols = ncols;
    return (MAT_Matrix *)MAT_InitArray(&mat->array, nrows*ncols, type, data);
}

MAT_Matrix *MAT_AllocMatrix(
    unsigned int nrows,
    unsigned int ncols,
    int type,
    int clear,
    MAT_Array *data)
{
    MAT_Matrix *mat;

    mat = malloc(sizeof(*mat));
    if (NULL == mat) return NULL;
    
    if (NULL == MAT_InitMatrix(mat, nrows, ncols, type, data))
    {
        free(mat);
        return NULL;
    }

    if (clear)
        MAT_ZeroMat(mat);
        
    return mat;
}

void MAT_FreeMatrix(MAT_Matrix *mat)
{
    if (NULL != mat)
        MAT_FreeArray(&mat->array);
}

void MAT_ZeroMat(MAT_Matrix *mat)
{
    MAT_ZeroArray(&mat->array);
}

void MAT_SetIdentity(MAT_Matrix *mat)
{
    unsigned int i, inc = mat->ncols+1;

    switch (mat->array.type)
    {
        case MAT_MATRIXTYPE_FLOAT:
            for (i=0; i < mat->array.width; i += inc)
            {
                mat->array.data.float_ptr[i] = 1.0f;
            }
            break;

        case MAT_MATRIXTYPE_DOUBLE:
            for (i=0; i < mat->array.width; i += inc)
            {
                mat->array.data.double_ptr[i] = 1.0;
            }
            break;
    }
}
