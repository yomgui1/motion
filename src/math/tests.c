#include "math/convolution.h"
#include "math/array.h"

#include <stdio.h>
#include <stdlib.h>

void print_array(const float *array, int w, int h)
{
    int i, j;
    float *pa = (float *)array;

    for (i=0; i < h; i++)
    {
        printf("%+.3f", pa[0]);
        pa++;

        for (j=1; j < w; j++)
        {
            printf(" %+.3f", pa[0]);
            pa++;
        }

        putchar('\n');
    }
}

int main(int argc, char **argv)
{
    float *kernel, *kernel_der;
    int width;
    float sigma = .9;
    float array_out[7*7];
    float array_test[7*7] = {
        0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,
        0,0,1,1,1,0,0,
        0,0,1,0,1,0,0,
        0,0,1,1,1,0,0,
        0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,
    };

    printf("Computing Gaussian kernel for sigma=%g", sigma);
    width = MAT_ZMGaussianKernel(sigma, &kernel, &kernel_der);
    if (width)
    {
        printf(" [OK] (width=%u)\n", width);
        putchar('\n');
        print_array(kernel, width, 1);
        putchar('\n');
        print_array(kernel_der, width, 1);
    }
    else
        printf(" [FAILED]\n");

    width = MAT_GaussianConvolve(sigma, array_test, array_out, 7, 7, kernel, width);
    printf("MAT_GaussianConvolve() resulted with %d\n", width);
    if (width)
    {
        putchar('\n');
        print_array(array_test, 7, 7);
        putchar('\n');
        print_array(array_out, 7, 7);

        printf("Norm of array_test: %f\n", MAT_ArrayNormL1(array_test, 7, 7));
        printf("Norm of array_out: %f\n", MAT_ArrayNormL1(array_out, 7, 7));
    }

    //free(kernel);
    free(kernel_der);

    return 0;
}
