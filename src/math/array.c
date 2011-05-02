#include <math.h>

/* Manhattan distance */
float MAT_ArrayNormL1(float *array, int w, int h)
{
    int i, j;
    float sum=0., *pa = array;

    for (i=0; i < h; i++)
    {
        for (j=0; j < w; j++)
        {
            sum += fabsf(*pa);
            pa++;
        }
    }

    return sum;
}

/* Euclidian distance */
float MAT_ArrayNormL2(float *array, int w, int h)
{
    int i, j;
    float sum=0., *pa = array;

    for (i=0; i < h; i++)
    {
        for (j=0; j < w; j++)
        {
            float v = fabsf(*pa);

            sum += v*v;
            pa++;
        }
    }

    return sum;
}

float MAT_ArrayNormalizeL1(float *array, int w, int h)
{
    int i, j;
    float norm = MAT_ArrayNormL1(array, w, h);
    float *pa = array;

    for (i=0; i < h; i++)
    {
        for (j=0; j < w; j++)
        {
            *pa /= norm;
            pa++;
        }
    }

    return norm;
}

float MAT_ArrayNormalizeL2(float *array, int w, int h)
{
    int i, j;
    float norm = MAT_ArrayNormL2(array, w, h);
    float *pa = array;

    for (i=0; i < h; i++)
    {
        for (j=0; j < w; j++)
        {
            *pa /= norm;
            pa++;
        }
    }

    return norm;
}
