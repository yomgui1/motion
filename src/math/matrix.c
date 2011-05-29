#include "math/matrix.h"

#include <stdlib.h>
#include <string.h>

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
        MAT_ZeroMatrix(mat);
        
    return mat;
}

MAT_Matrix *MAT_AllocMatrixLike(const MAT_Matrix *friend, int copydata)
{
	MAT_Matrix *mat = MAT_AllocMatrix(friend->nrows, friend->ncols, friend->array.type, !copydata, NULL);
	
	if (copydata && (NULL != mat))
		memcpy(mat->array.data.void_ptr, friend->array.data.void_ptr, friend->array.width * friend->array.datatype_size);
		
	return mat;
}

void MAT_FreeMatrix(MAT_Matrix *mat)
{
    if (NULL != mat)
        MAT_FreeArray(&mat->array);
}

void MAT_ZeroMatrix(MAT_Matrix *mat)
{
    MAT_ZeroArray(&mat->array);
}

void MAT_SetIdentityMatrix(MAT_Matrix *mat)
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

void MAT_InPlaceTransposeMatrix(MAT_Matrix *mat)
{
    unsigned int tmp;

	tmp = mat->ncols;
	mat->ncols = mat->nrows;
	mat->nrows = tmp;

#define TRANSPOSE(mat, T) { \
	unsigned int i, j; \
	for (i=0; i < mat->nrows; j++) \
	{ \
		for (j=0; j < mat->ncols; j++) \
		{ \
			T tmp; \
			unsigned int n, m; \
			\
			if (i == j) continue; \
			\
			n = i * mat->ncols + j; \
			m = j * mat->nrows + i; \
			\
			tmp = mat->array.data.T##_ptr[n]; \
			mat->array.data.T##_ptr[n] = mat->array.data.T##_ptr[m]; \
			mat->array.data.T##_ptr[m] = tmp; \
		} \
	} }
	
    switch (mat->array.type)
    {
        case MAT_MATRIXTYPE_FLOAT:
            TRANSPOSE(mat, float);
            break;

        case MAT_MATRIXTYPE_DOUBLE:
            TRANSPOSE(mat, double);
            break;
    }
}

MAT_Matrix *MAT_TransposeMatrix(MAT_Matrix *mat)
{
    MAT_Matrix *matT = MAT_AllocMatrixLike(mat, 1);
    
    if (NULL != matT)
		MAT_InPlaceTransposeMatrix(matT);
		
    return matT;
}

MAT_Matrix *MAT_MultiplyMatrix(MAT_Matrix *mat1, MAT_Matrix *mat2)
{
    MAT_Matrix *res;
    
    if ((mat1->array.type != mat2->array.type) ||
		(mat1->ncols != mat2->nrows) ||
		(mat1->nrows != mat2->ncols))
		return NULL;
    
    res = MAT_AllocMatrixLike(mat2, 0);
    if (NULL != res)
    {
#define MULTIPLY(res, mat1, mat2, T) { \
	unsigned int i, j, k; \
	T *ptr = res->array.data.T##_ptr; \
	for (i=0; i < mat2->nrows; i++) \
	{ \
		unsigned int n = i * mat1->ncols; \
		for (j=0; j < mat2->ncols; j++) \
		{ \
			T sum = 0.0; \
			for (k=0; k < mat1->nrows; k++) \
				sum += mat1->array.data.T##_ptr[n + k] * mat2->array.data.T##_ptr[k*mat2->ncols + j]; \
			*ptr++ = sum; \
		} \
	} }
	
		switch (mat1->array.type)
		{
			case MAT_MATRIXTYPE_FLOAT:
				MULTIPLY(res, mat1, mat2, float);
				break;

			case MAT_MATRIXTYPE_DOUBLE:
				MULTIPLY(res, mat1, mat2, double);
				break;
		}
    }
		
    return res;
}

int MAT_InPlaceMultiplyMatrix(MAT_Matrix *mat1, MAT_Matrix *mat2)
{
	MAT_Matrix *res;
	
	res = MAT_MultiplyMatrix(mat1, mat2);
	if (NULL == res) return 1;
	
	memcpy(mat2->array.data.void_ptr, res->array.data.void_ptr, res->array.width * res->array.datatype_size);
	MAT_FreeMatrix(res);
	return 0;
}

void MAT_InPlaceScalarMultiplyMatrix(double value, MAT_Matrix *mat)
{
#define SCALAR_MULTIPLY(v, mat, T) { \
	unsigned int i; \
	T *ptr = mat->array.data.T##_ptr; \
	for (i=0; i < mat->nrows * mat->ncols; i++) \
		*ptr++ *= v; \
	}
	
    switch (mat->array.type)
    {
        case MAT_MATRIXTYPE_FLOAT:
            SCALAR_MULTIPLY(value, mat, float);
            break;

        case MAT_MATRIXTYPE_DOUBLE:
            SCALAR_MULTIPLY(value, mat, double);
            break;
    }
}

MAT_Matrix *MAT_ScalarMultiplyMatrix(double value, MAT_Matrix *mat)
{
    MAT_Matrix *res = MAT_AllocMatrixLike(mat, 1);
    
    if (NULL != res)
		MAT_InPlaceScalarMultiplyMatrix(value, res);
		
    return res;
}

MAT_Vector *MAT_MultiplyMatrixVector(MAT_Matrix *mat, MAT_Vector *vec)
{
	MAT_Vector *res;
	
	if ((mat->array.type != vec->type) || (mat->ncols != vec->width))
		return NULL;
		
	res = MAT_AllocArray(mat->nrows, vec->type, 0, NULL);
	if (NULL != res)
	{
#define MULTIPLY_MAT_VEC(mat, vec, T) { \
	unsigned int i, j; \
	T *ptr = res->data.T##_ptr; \
	for (i=0; i < res->width; i++) \
	{ \
		unsigned int n = i * mat->ncols; \
		T sum = 0.0; \
		for (j=0; j < mat->ncols; j++) \
			sum += mat->array.data.T##_ptr[n + j] * vec->data.T##_ptr[j]; \
		*ptr++ = sum; \
	} }
	
		switch (vec->type)
		{
			case MAT_TYPE_FLOAT:
				MULTIPLY_MAT_VEC(mat, vec, float);
				break;

			case MAT_TYPE_DOUBLE:
				MULTIPLY_MAT_VEC(mat, vec, double);
				break;
		}
	}

	return res;
}

int MAT_InPlaceMultiplyMatrixVector(MAT_Matrix *mat, MAT_Vector *vec)
{
    MAT_Vector *res;
    
    res = MAT_MultiplyMatrixVector(mat, vec);
    if (NULL == res) return 1;
    
	memcpy(vec->data.void_ptr, res->data.void_ptr, res->width * res->datatype_size);
	MAT_FreeArray(res);
    return 0;
}
