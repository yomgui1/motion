#ifndef MAT_CONVOLUTION_H
#define MAT_CONVOLUTION_H

#include <math/array.h>

extern double MAT_ZMGaussian(double x, double sigma);
extern double MAT_InvZMGaussian(double y, double sigma);
extern double MAT_ZMGaussianDerivative(double x, double sigma);
extern int MAT_ZMGaussianKernel(double sigma, MAT_Array **p_kernel, MAT_Array **p_derivatives);
extern int MAT_ArrayConvolve(MAT_Array *kernel,
                             MAT_Array *input,
                             MAT_Array *output,
                             int increment);
extern int MAT_ArrayConvolve2DHoriz(MAT_Array *kernel,
                                    MAT_Array *input,
                                    MAT_Array *output,
                                    unsigned int width,
                                    unsigned int height);
extern int MAT_ArrayConvolve2DVert(MAT_Array *kernel,
                                   MAT_Array *input,
                                   MAT_Array *output,
                                   unsigned int width,
                                   unsigned int height);
extern int MAT_ArrayConvolve2D(MAT_Array *kernel,
                               MAT_Array *input,
                               MAT_Array *output,
                               unsigned int width,
                               unsigned int height);
extern int MAT_ArrayConvolveAndDerivative2D(MAT_Array *gaussian,
                                            MAT_Array *derivatives,
                                            MAT_Array *input,
                                            MAT_Array *output,
                                            unsigned int width,
                                            unsigned int height);

#endif /* MAT_CONVOLUTION_H */
