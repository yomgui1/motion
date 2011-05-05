#include "math/matrix.h"

#include <math.h>
#include <stdlib.h>

#define CLAMP(a, v, b) (a<=v?(v<=b?v:b):a)

/* Gaussian formulae from http://en.wikipedia.org/wiki/Gaussian_function */

double MAT_ZMGaussian(double x, double sigma)
{
    return exp(-x*x / (2*sigma*sigma)) / (sigma*sqrt(2*M_PI));
}

double MAT_InvZMGaussian(double y, double sigma)
{
    return sqrt(-2*sigma*sigma * log(y*sigma*sqrt(2*M_PI)));
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
    const double _gaussian = 0.004; /* MAT_ZeroMeanGaussian(3, 1) */
    MAT_Array *kernel, *derivatives; /* used for code simplification */
	int i, width, halfwidth;
	float *kernel_data, *derivatives_data;

    /* Compute kernel size (odd value), we suppose that the int cast rounds */
    width = (((int)lround(2 * MAT_InvZMGaussian(_gaussian, sigma))) & -2) + 1;
    
	/* Arrays allocations  */
    kernel = *p_kernel = MAT_AllocArray(width, MAT_ARRAYTYPE_FLOAT, 0, NULL);
    derivatives = *p_derivatives = MAT_AllocArray(width, MAT_ARRAYTYPE_FLOAT, 0, NULL);
    
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

int MAT_ArrayConvolve(
    MAT_Array *kernel,
    MAT_Array *input,
    MAT_Array *output,
    int increment)
{
    int i, k, width, k_width, k_halfwidth;
    
    if (kernel->type != input->type) return 1;
	if (!MAT_SAMEARRAYTYPE(input, output)) return 1;
	
	width = input->width;
	k_width = kernel->width;
	k_halfwidth = k_width / 2;
    
#define _CONVOLVE(T)                                                \
	{                                                               \
		T *pai = input->data.T##_ptr;                               \
		T *pao = output->data.T##_ptr;                              \
		for (i=0; i < width; i++)                                   \
		{                                                           \
			T sum;                                                  \
			for (sum=0, k=0; k < k_width; k++)                      \
			{                                                       \
				int ii = i + (k - k_halfwidth) * increment;         \
				if ((ii >= 0) && (ii < width))                      \
					sum += kernel->data.T##_ptr[k] * (pai + ii)[0]; \
			}                                                       \
			pao[i] = sum;                                           \
		}                                                           \
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
	return 0;
}

int MAT_MatrixConvolveHoriz(MAT_Array *kernel, MAT_Matrix *input, MAT_Matrix *output)
{
    int x, y, k, k_width, k_halfwidth;
    
    if (!MAT_SAMEMATRIXTYPE(input, output)) return 1;
    if (kernel->type != input->array.type) return 1;

	k_width = kernel->width;
	k_halfwidth = k_width / 2;

#define _CONVOLVE_H(T, in, out, inc)                                    \
	{                                                                   \
		T *pai = (T *)(in);                                             \
		T *pao = (T *)(out);                                            \
        int ncols = input->ncols;                                       \
		for (y=0; y < input->nrows; y++, pai+=ncols)                    \
		{                                                               \
            for (x=0; x < input->ncols; x++, pao+=inc)                  \
            {                                                           \
                T sum=0.0;                                              \
                for (k=k_width-1; k >= 0; k--)                          \
                {                                                       \
                    int xx = x+k_halfwidth-k;                           \
                    int n = CLAMP(0, xx, ncols-1);                      \
                    sum += kernel->data.T##_ptr[k] * (pai+n)[0];        \
                }                                                       \
                *pao = sum;                                             \
            }                                                           \
		}                                                               \
	}

    switch (input->array.type)
	{
		case MAT_ARRAYTYPE_FLOAT:
			_CONVOLVE_H(float, input->array.data.float_ptr, output->array.data.float_ptr, 1);
			break;
		
		case MAT_ARRAYTYPE_DOUBLE:
			_CONVOLVE_H(double, input->array.data.double_ptr, output->array.data.double_ptr, 1);
			break;
	}
	return 0;
}

int MAT_MatrixConvolveVert(MAT_Array *kernel, MAT_Matrix *input, MAT_Matrix *output)
{
    int x, y, k, k_width, k_halfwidth;
    
    if (!MAT_SAMEMATRIXTYPE(input, output)) return 1;
    if (kernel->type != input->array.type) return 1;

	k_width = kernel->width;
	k_halfwidth = k_width / 2;

#define _CONVOLVE_V(T, in, out, inc)                                    \
	{                                                                   \
        T *pao = (T *)(out);                                            \
        int ncols = input->ncols;                                       \
        for (y=0; y < input->nrows; y++)                                \
		{                                                               \
            T *pai = (T *)(in);                                         \
            for (x=0; x < ncols; x++, pai++, pao+=inc)                  \
            {                                                           \
                T sum=0.0;                                              \
                for (k=k_width-1; k >= 0; k--)                    		\
                {                                                       \
                    int n = CLAMP(0, y+k_halfwidth-k, input->nrows-1);  \
                    sum += kernel->data.T##_ptr[k] * (pai + n*ncols)[0]; \
                }                                                       \
                *pao = sum;                                             \
            }                                                           \
		}                                                               \
	}

    switch (input->array.type)
	{
		case MAT_ARRAYTYPE_FLOAT:
			_CONVOLVE_V(float, input->array.data.float_ptr, output->array.data.float_ptr, 1);
			break;
		
		case MAT_ARRAYTYPE_DOUBLE:
			_CONVOLVE_V(double, input->array.data.double_ptr, output->array.data.double_ptr, 1);
			break;
	}

	return 0;
}

int MAT_MatrixConvolve(MAT_Array *kernel, MAT_Matrix *input, MAT_Matrix *output)
{
    MAT_Array *tmp;
    int x, y, k, k_width, k_halfwidth;

    if (!MAT_SAMEMATRIXTYPE(input, output)) return 1;
    if (kernel->type != input->array.type) return 1;

    tmp = MAT_AllocArray(input->array.width, input->array.type, 0, NULL);
    if (NULL == tmp) return 1;

	k_width = kernel->width;
	k_halfwidth = k_width / 2;

    switch (input->array.type)
	{
		case MAT_ARRAYTYPE_FLOAT:
            _CONVOLVE_H(float, input->array.data.float_ptr, tmp->data.float_ptr, 1);
			_CONVOLVE_V(float, tmp->data.float_ptr, output->array.data.float_ptr, 1);
			break;
		
		case MAT_ARRAYTYPE_DOUBLE:
			_CONVOLVE_H(double, input->array.data.double_ptr, tmp->data.double_ptr, 1);
			_CONVOLVE_V(double, tmp->data.double_ptr, output->array.data.double_ptr, 1);
			break;
	}

    MAT_FreeArray(tmp);
	return 0;
}

int MAT_MatrixConvolveAndDerivative(
    MAT_Array *gaussian,
    MAT_Array *derivatives,
    MAT_Matrix *input,
    MAT_Matrix *output)
{
    MAT_Array *tmp, *kernel;
    int x, y, k, k_width, k_halfwidth;
    
    if (gaussian->type != input->array.type) return 1;
    if (gaussian->type != derivatives->type) return 1;
    if (gaussian->width != derivatives->width) return 1;
	if (output->array.type != input->array.type) return 1;
    if (output->ncols < 3*input->ncols) return 1;

#if 0
	{
		int i,j;
		MAT_Matrix *mat = input;
		printf("Dumping input matrix @ %p, data=%p\n", output, output->array.data.void_ptr);
		for (i = 0; i < mat->nrows; ++i)  {
			printf("%p [", &mat->array.data.float_ptr[i*mat->ncols]);
			for (j = 0; j < mat->ncols; j++)  {
				printf(" %f", mat->array.data.float_ptr[i*mat->ncols+j]);
			}
			printf(" ]\n");
		}
	}
#endif

    tmp = MAT_AllocArray(input->array.width, input->array.type, 0, NULL);
    if (NULL == tmp) return 1;

	k_width = gaussian->width;
	k_halfwidth = k_width / 2;
//	printf("Gaussian width: %u\n", k_width);

    switch (input->array.type)
	{
		case MAT_ARRAYTYPE_FLOAT:
            /* Gaussian */
            kernel = gaussian;
            _CONVOLVE_V(float, input->array.data.float_ptr, tmp->data.float_ptr, 1);
			_CONVOLVE_H(float, tmp->data.float_ptr, output->array.data.float_ptr, 3);
            
            /* X-Derivatives */
            kernel = derivatives;
			_CONVOLVE_H(float, tmp->data.float_ptr, output->array.data.float_ptr+1, 3);

            /* Y-Derivatives */
            kernel = gaussian;
            _CONVOLVE_H(float, input->array.data.float_ptr, tmp->data.float_ptr, 1);
            kernel = derivatives;
			_CONVOLVE_V(float, tmp->data.float_ptr, output->array.data.float_ptr+2, 3);
			break;
		
		case MAT_ARRAYTYPE_DOUBLE:
			/* Gaussian */
            kernel = gaussian;
            _CONVOLVE_V(double, input->array.data.double_ptr, tmp->data.double_ptr, 1);
			_CONVOLVE_H(double, tmp->data.double_ptr, output->array.data.double_ptr, 3);
           
            /* X-Derivatives */
            kernel = derivatives;
			_CONVOLVE_H(double, tmp->data.double_ptr, output->array.data.double_ptr+1, 3);

            /* Y-Derivatives */
            kernel = gaussian;
            _CONVOLVE_H(double, input->array.data.double_ptr, tmp->data.double_ptr, 1);
            kernel = derivatives;
			_CONVOLVE_V(double, tmp->data.double_ptr, output->array.data.double_ptr+2, 3);
			break;
	}
	
	MAT_FreeArray(tmp);

#if 0
	{
		int i,j,k;
		MAT_Matrix *mat = output;
		printf("Dumping output matrix @ %p, data=%p\n", output, output->array.data.void_ptr);
		for (k=0; k < 3; k++)
		{
			printf("Index %u\n", k);
			for (i = 0; i < mat->nrows; ++i)  {
				printf("%p [", &mat->array.data.float_ptr[i*mat->ncols+k]);
				for (j = 0; j < mat->ncols; j+=3)  {
					float v = mat->array.data.float_ptr[i*mat->ncols+j+k];
					if (v)
					{
						if (k==0)
							printf(" %02x", (int)(v*255.));
						else
							printf(" %+04d", (int)(v*128.));
					}
					else if (k)
						printf("     ");
					else
						printf("   ");
				}
				printf(" ]\n");
			}
		}
	}
#endif
    
	return 0;
}

void MAT_BoxFilterHoriz(const MAT_Matrix *input, MAT_Matrix *output, int depth, int window_size)
{
	int halfwidth = (window_size - 1) / 2;
	unsigned int i,j,k,ncols;
	float *in = input->array.data.float_ptr;
	float *out = output->array.data.float_ptr;
  
	ncols = input->ncols / depth;
	
	for (k=0; k < depth; k++)
	{
		for (i=0; i < input->nrows; i++)
		{
			float sum = 0;

			/* Init. sum */
			for (j=0; j < halfwidth; j++)
				sum += in[(i*ncols + j)*depth + k];
				
			/* Fill left border */
			for (j=0; j < halfwidth + 1; j++)
			{
				sum += in[(i*ncols + j + halfwidth)*depth + k];
				out[(i*ncols + j)*depth + k] = sum;
			}
			
			/* Fill interior */
			for (j=halfwidth+1; j < ncols-halfwidth; j++)
			{
				sum -= in[(i*ncols + j - halfwidth - 1)*depth + k];
				sum += in[(i*ncols + j + halfwidth)*depth + k];
				out[(i*ncols + j)*depth + k] = sum;
			}
			
			/* Fill right border */
			for (j=ncols-halfwidth; j < ncols; j++)
			{
				sum -= in[(i*ncols + j - halfwidth - 1)*depth + k];
				out[(i*ncols + j)*depth + k] = sum;
			}
		}
	}
}

void MAT_BoxFilterVert(const MAT_Matrix *input, MAT_Matrix *output, int depth, int window_size)
{
	int halfwidth = (window_size - 1) / 2;
	unsigned int i,j,k,ncols;
	float *in = input->array.data.float_ptr;
	float *out = output->array.data.float_ptr;
  
	ncols = input->ncols / 3;
  
	for (k=0; k < depth; k++)
	{
		for (j=0; j < ncols; j++)
		{
			float sum = 0;

			/* Init. sum */
			for (i=0; i < halfwidth; i++)
				sum += in[(i*ncols + j)*depth + k];
				
			/* Fill left border */
			for (i=0; i < halfwidth + 1; i++)
			{
				sum += in[((i + halfwidth)*ncols + j)*depth + k];
				out[(i*ncols + j)*depth + k] = sum;
			}
			
			/* Fill interior */
			for (i=halfwidth+1; i < input->nrows-halfwidth; i++)
			{
				sum -= in[((i - halfwidth - 1)*ncols + j)*depth + k];
				sum += in[((i + halfwidth)*ncols + j)*depth + k];
				out[(i*ncols + j)*depth + k] = sum;
			}
			
			/* Fill right border */
			for (i=input->nrows-halfwidth; i < input->nrows; i++)
			{
				sum -= in[((i - halfwidth - 1)*ncols + j)*depth + k];
				out[(i*ncols + j)*depth + k] = sum;
			}
		}
	}
}

void MAT_BoxFilter(const MAT_Matrix *input, MAT_Matrix *output, int depth, int window_size)
{
	MAT_Matrix *tmp = MAT_AllocMatrixLike(input, 0);
	
	if (NULL == tmp) return;
	
	MAT_BoxFilterHoriz(input, tmp, depth, window_size);
	MAT_BoxFilterVert(tmp, output, depth, window_size);
	
	MAT_FreeMatrix(tmp);
}

