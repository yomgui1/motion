#include "array.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

MAT_Array *MAT_Array2DfNew(int w, int h)
{
    MAT_Array *array;
    unsigned int base_size = (sizeof(MAT_Array)+15) & -16;

    array = malloc(base_size + h*w*sizeof(float));
    if (array)
    {
        array->data = (void *)array + base_size;
        array->width = w;
        array->height = h;
    }
    
    return array;
}

void MAT_Array2DfFree(MAT_Array *array)
{
    free(array);
}

MAT_Array *MAT_Array2DfDup(MAT_Array *src)
{
    MAT_Array *dst;

    dst = MAT_Array2DfNew(src->width, src->height);
    if (NULL != dst)
        memcpy(dst->data, src->data, src->width*src->height*sizeof(float));

    return dst;
}

/* Manhattan distance */
float MAT_Array2DfNormL1(MAT_Array *array)
{
    int i, j;
    float sum=0., *pa=array->data;

    for (i=0; i < array->height; i++)
    {
        for (j=0; j < array->width; j++)
        {
            sum += fabsf(*pa);
            pa++;
        }
    }

    return sum;
}

/* Euclidian distance */
float MAT_Array2DfNormL2(MAT_Array *array)
{
    int i, j;
    float sum=0., *pa=array->data;

    for (i=0; i < array->height; i++)
    {
        for (j=0; j < array->width; j++)
        {
            float v = fabsf(*pa);

            sum += v*v;
            pa++;
        }
    }

    return sum;
}

float MAT_Array2DfNormalizeL1(MAT_Array *array)
{
    int i, j;
    float norm = MAT_Array2DfNormL1(array);
    float *pa = array->data;

    for (i=0; i < array->height; i++)
    {
        for (j=0; j < array->width; j++)
        {
            *pa /= norm;
            pa++;
        }
    }

    return norm;
}

float MAT_Array2DfNormalizeL2(MAT_Array *array)
{
    int i, j;
    float norm = MAT_Array2DfNormL2(array);
    float *pa = array->data;

    for (i=0; i < array->height; i++)
    {
        for (j=0; j < array->width; j++)
        {
            *pa /= norm;
            pa++;
        }
    }

    return norm;
}
