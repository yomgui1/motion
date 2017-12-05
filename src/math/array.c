#include "array.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

int MAT_SameArrayType(MAT_Array *left, MAT_Array *right)
{
	return MAT_SAMEARRAYTYPE(left, right);
}

MAT_Array *MAT_InitArray(MAT_Array *array, unsigned int width, int type, void *data)
{
    unsigned int datatype_size=0;
    unsigned int data_size;

    switch(type)
    {
		case MAT_ARRAYTYPE_FLOAT: datatype_size = sizeof(float); break;
		case MAT_ARRAYTYPE_DOUBLE: datatype_size = sizeof(double); break;
    }

    data_size = datatype_size*width;

    if (NULL == data)
    {
        array->_alloc_data = malloc(data_size);
        if (NULL == array->_alloc_data) return NULL;
        array->data.void_ptr = array->_alloc_data;
    }
    else
    {
        array->_alloc_data = NULL;
        array->data.void_ptr = data;
    }

    array->type = type;
    array->datatype_size = datatype_size;
    array->width = width;

    return array;
}

MAT_Array *MAT_AllocArray(
    unsigned int width,
    int type,
    int clear,
    void *data)
{
    MAT_Array *array;

    array = malloc(sizeof(*array));
    if (NULL == array) return NULL;

    if (NULL == MAT_InitArray(array, width, type, data))
    {
        free(array);
        return NULL;
    }

    if (clear)
        MAT_ZeroArray(array);

    return array;
}

void MAT_FreeArray(MAT_Array *array)
{
    free(array->_alloc_data);
    free(array);
}

void MAT_ZeroArray(MAT_Array *array)
{
    bzero(array->data.void_ptr, array->datatype_size * array->width);
}

MAT_Array *MAT_DupArray(const MAT_Array *src)
{
    MAT_Array *dst;

    dst = MAT_AllocArray(src->width, src->type, 0, NULL);
    if (NULL != dst)
        memcpy(dst->data.void_ptr, src->data.void_ptr, src->datatype_size * src->width);

    return dst;
}

MAT_Array *MAT_ExtractArrayPlan(const MAT_Array *src, unsigned int max, unsigned int index)
{
    MAT_Array *dst;
    unsigned int i;

    dst = MAT_AllocArray(src->width/max, src->type, 0, NULL);
    if (NULL != dst)
    {
        switch (src->type)
        {
            case MAT_ARRAYTYPE_FLOAT:
                for (i=0; i < dst->width; i++)
                    dst->data.float_ptr[i] = src->data.float_ptr[i*max+index];
                break;

            case MAT_ARRAYTYPE_DOUBLE:
                for (i=0; i < dst->width; i++)
                    dst->data.double_ptr[i] = src->data.double_ptr[i*max+index];
                break;
        }
    }

    return dst;
}

void MAT_ScalarAddArray(MAT_Array *array, double value)
{
    int i;

	switch (array->type)
	{
		case MAT_ARRAYTYPE_FLOAT:
			{
				float *pa=array->data.float_ptr;
				float fvalue=value;

                for (i=0; i < array->width; i++, pa++)
                    *pa += fvalue;
			}
			break;

		case MAT_ARRAYTYPE_DOUBLE:
			{
				double *pa=array->data.double_ptr;

				for (i=0; i < array->width; i++, pa++)
					*pa += value;
			}
			break;
	}
}

void MAT_ScalarMulArray(MAT_Array *array, double value)
{
    int i;

	switch (array->type)
	{
		case MAT_ARRAYTYPE_FLOAT:
			{
				float *pa=array->data.float_ptr;
				float fvalue=value;

				for (i=0; i < array->width; i++, pa++)
					*pa *= fvalue;
			}
			break;

		case MAT_ARRAYTYPE_DOUBLE:
			{
				double *pa=array->data.double_ptr;

				for (i=0; i < array->width; i++, pa++)
					*pa *= value;
			}
			break;
	}
}

void MAT_ScalarDivArray(MAT_Array *array, double value)
{
    int i;

	switch (array->type)
	{
		case MAT_ARRAYTYPE_FLOAT:
			{
				float *pa=array->data.float_ptr;
				float fvalue=value;

				for (i=0; i < array->width; i++, pa++)
					*pa /= fvalue;
			}
			break;

		case MAT_ARRAYTYPE_DOUBLE:
			{
				double *pa=array->data.double_ptr;

				for (i=0; i < array->width; i++, pa++)
					*pa /= value;
			}
			break;
	}
}

/* Manhattan distance */
double MAT_NormL1Array(MAT_Array *array)
{
	int i;

	switch (array->type)
	{
		case MAT_ARRAYTYPE_FLOAT:
			{
				float sum=0.0f, *pa=array->data.float_ptr;

				for (i=0; i < array->width; i++, pa++)
					sum += fabsf(*pa);

				return sum;
			}
			break;

		case MAT_ARRAYTYPE_DOUBLE:
			{
				double sum=0.0, *pa=array->data.double_ptr;

				for (i=0; i < array->width; i++, pa++)
					sum += fabs(*pa);

				return sum;
			}
	}

	return 0.0;
}

double MAT_NormalizeL1Array(MAT_Array *array)
{
    double norm=MAT_NormL1Array(array);
    MAT_ScalarDivArray(array, norm);
    return norm;
}

/* Euclidian distance */
double MAT_NormL2Array(MAT_Array *array)
{
    int i;

	switch (array->type)
	{
		case MAT_ARRAYTYPE_FLOAT:
			{
				float sum=0.0f, *pa=array->data.float_ptr;

				for (i=0; i < array->width; i++, pa++)
					sum += (*pa) * (*pa);

				return sum;
			}

		case MAT_ARRAYTYPE_DOUBLE:
			{
				double sum=0.0, *pa=array->data.double_ptr;

				for (i=0; i < array->width; i++, pa++)
					sum += (*pa) * (*pa);

				return sum;
			}
	}

    return 0.0;
}

double MAT_NormalizeL2Array(MAT_Array *array)
{
    double norm=MAT_NormL2Array(array);
    MAT_ScalarDivArray(array, norm);
    return norm;
}
