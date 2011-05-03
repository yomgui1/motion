#include "math/array.h"

#include <math.h>
#include <stdlib.h>

/* Gaussian formulae from http://en.wikipedia.org/wiki/Gaussian_function */

double MAT_ZMGaussian(double x, double sigma)
{
    return exp(-(x*x) / (2 * sigma*sigma)) / (sigma * sqrt(2*M_PI));
}

double MAT_InvZMGaussian(double y, double sigma)
{
    return sqrt(-2 * sigma*sigma * log(y * sigma * sqrt(2*M_PI)));
}

double MAT_ZMGaussianDerivative(double x, double sigma)
{
    return -MAT_ZMGaussian(x, sigma) * x / (sigma*sigma);
}

/* Generate a ero mean normalized gaussian kernel (MAT_Array(float)) and its derivatives (MAT_Array(float)).
 * Return the size of the kernel.
 * In case of error (i.e. array allocation failed) returns 0 and given pointed values are NULL.
 */
int MAT_ZMGaussianKernel(double sigma, MAT_Array **p_kernel, MAT_Array **p_derivatives)
{
    const double _gaussian = 0.0044318; /* MAT_ZeroMeanGaussian(3, 1) */
    MAT_Array *kernel, *derivatives; /* used for code simplification */
	int i, width, halfwidth;
	float *kernel_data, *derivatives_data;

    /* Compute kernel size (odd value), we suppose that the int cast rounds */
    width = ((int)(2 * MAT_InvZMGaussian(_gaussian, sigma)) & -2) + 1;
    
	/* Arrays allocations  */
    kernel = *p_kernel = MAT_AllocArray(width, MAT_ARRAYTYPE_FLOAT, 0);
    derivatives = *p_derivatives = MAT_AllocArray(width, MAT_ARRAYTYPE_FLOAT, 0);
    
    if ((NULL == kernel) || (NULL == derivatives))
    {
		MAT_FreeArray(kernel);
		MAT_FreeArray(derivatives);
        return 0;
    }

    halfwidth = width / 2;
    kernel_data = kernel->data.float_ptr;
    derivatives_data = derivatives->data.float_ptr;
    
    for (i=-halfwidth; i <= halfwidth; i++)
    {
        *kernel_data++ = MAT_ZMGaussian(i, sigma);
        *derivatives_data++ = MAT_ZMGaussianDerivative(i, sigma);
    }

    return width;
}

int MAT_ArrayConvolve(MAT_Array *kernel, MAT_Array *input, MAT_Array *output, int increment)
{
    int i, k, width, k_width, k_halfwidth;
    
    if (kernel->type != input->type) return 1;
	if (!MAT_SAMEARRAYTYPE(input, output)) return 1;
	
	width = input->width;
	k_width = kernel->width;
	k_halfwidth = k_width / 2;

#define _CONVOLVE(T) \
	{ \
		T *pai = input->data.T##_ptr; \
		T *pao = output->data.T##_ptr; \
		for (i=0; i < width; i++) \
		{ \
			T sum; \
			for (sum=0,k=0; k < k_width; k++) \
			{ \
				int ii = i + (k - k_halfwidth) * increment; \
				if ((ii >= 0) && (ii < width)) \
					sum += kernel->data.T##_ptr[k] * (pai + ii)[0]; \
			} \
			pao[i] = sum; \
		} \
	}
    
    switch (input->type)
	{
		case MAT_ARRAYTYPE_FLOAT:
			_CONVOLVE(float);
			break;
		
		case MAT_ARRAYTYPE_DOUBLE:
			_CONVOLVE(double);
			break;
	}
	
#undef _CONVOLVE

	return 0;
}

