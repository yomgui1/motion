#include "array.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

int MAT_SameArrayType(MAT_Array *left, MAT_Array *right)
{
	return MAT_SAMEARRAYTYPE(left, right);
}

MAT_Array *MAT_AllocArray(unsigned int width, int type, int clear)
{
    MAT_Array *array;
    unsigned int datatype_size=0;
    unsigned int data_offset=(sizeof(MAT_Array)+15) & -16;
    
    switch(type)
    {
		case MAT_ARRAYTYPE_FLOAT: datatype_size = sizeof(float); break;
		case MAT_ARRAYTYPE_DOUBLE: datatype_size = sizeof(double); break;
    }

	if (clear)
		array = calloc(data_offset + datatype_size*width, 1);
	else
		array = malloc(data_offset + datatype_size*width);
		
    if (array)
    {
		array->type = type;
		array->width = width;
		array->datatype_size = datatype_size;
        array->data.void_ptr = (void *)array + data_offset;
    }
    
    return array;
}

void MAT_FreeArray(MAT_Array *array)
{
    free(array);
}

MAT_Array *MAT_DupArray(MAT_Array *src)
{
    MAT_Array *dst;

    dst = MAT_AllocArray(src->width, src->type, 0);
    if (NULL != dst)
        memcpy(dst->data.void_ptr, src->data.void_ptr, src->datatype_size * src->width);

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
				float fvalue = value;
				
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
				float fvalue = value;
				
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
				float fvalue = value;
				
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
