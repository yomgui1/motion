#include "math.h"

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

/* Zero mean normalized gaussian kernel and derivatives */
int MAT_ZMGaussianKernel(double sigma, float **p_kernel, float **p_kernel_der)
{
    const double _gaussian = 0.0044318; /* MAT_ZeroMeanGaussian(3, 1) */
    int w, w2, i;
    float *kernel, *kernel_der;

    /* Kernel size (odd value), we suppose that the int cast rounds */
    w = ((int)(2 * MAT_InvZMGaussian(_gaussian, sigma)) & -2) + 1;

    kernel = *p_kernel = malloc(w*sizeof(*kernel));
    if (NULL == kernel)
        return 0;

    kernel_der = *p_kernel_der = malloc(w*sizeof(*kernel_der));
    if (NULL == kernel_der)
    {
        free(kernel);
        return 0;
    }

    w2 = w / 2;
    for (i=-w2; i <= w2; i++)
    {
        kernel    [w2+i] = MAT_ZMGaussian(i, sigma);
        kernel_der[w2+i] = MAT_ZMGaussianDerivative(i, sigma);
    }

    return w;
}

void MAT_ArrayHorizConvolve(float *kernel, int kw, float *in, float *out, int aw, int ah)
{
    int i, j, k, kw2;

    kw2 = kw/2;

    for (i=0; i < ah; i++)
    {
        float *pai = &in[i*aw];
        float *pao = &out[i*aw];

        for (j=0; j < aw; j++)
        {
            float sum;
            int r;

            for (sum=0,k=0,r=0; k < kw; k++, r++)
            {
                int jj = j - kw2 + r;
                
                if ((jj >= 0) && (jj < aw))
                    sum += kernel[k] * (pai + jj)[0];
            }

            pao[0] = sum;

            pao++;
        }
    }
}

void MAT_ArrayVertConvolve(float *kernel, int kw, float *in, float *out, int aw, int ah)
{
    int i, j, k, kw2;

    kw2 = kw/2;

    for (j=0; j < aw; j++)
    {
        float *pai = &in[j];
        float *pao = &out[j];

        for (i=0; i < ah; i++)
        {
            float sum;
            int r;

            for (sum=0,k=0,r=0; k < kw; k++, r++)
            {
                int ii = i - kw2 + r;

                if ((ii >= 0) && (ii < ah))
                    sum += kernel[k] * (pai + ii*aw)[0];
            }

            pao[0] = sum;

            pao += aw;
        }
    }
}

int MAT_GaussianConvolve(double sigma, float *in, float *out, int w, int h, float *kernel, int kw)
{
    float *tmp, *kernel_der=NULL;

    if (NULL == kernel)
    {
        kw = MAT_ZMGaussianKernel(sigma, &kernel, &kernel_der);
        if (!kw)
            return 0;
    }

    tmp = malloc(w*h*sizeof(*in));
    if (NULL == tmp)
    {
        kw = 0;
        goto bye;
    }

    MAT_ArrayHorizConvolve(kernel, kw, in, tmp, w, h);
    MAT_ArrayVertConvolve(kernel, kw, tmp, out, w, h);

    free(tmp);

bye:
    free(kernel);
    free(kernel_der);

    return kw;
}
