#include "math/convolution.h"

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
    MAT_Array *kernel, *derivatives;
    int width;
    float sigma = .9;
    MAT_Array *output, *test_7x7;
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
    width = MAT_ZMGaussianKernel(sigma, &kernel, &derivatives);
    if (!width || !kernel || !derivatives)
    {
        printf(" [FAILED]\n");
        return 1;
    }
    
    printf(" [OK] (width=%u)\n", width);
    printf("\nKernel data:\n");
    print_array(kernel->data.float_ptr, width, 1);
    
    printf("\nDerivatives data:\n");
    print_array(derivatives->data.float_ptr, width, 1);

	MAT_FreeArray(kernel);
	MAT_FreeArray(derivatives);

    return 0;
}
