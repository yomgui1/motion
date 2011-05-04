#ifndef MAT_CONVOLUTION_H
#define MAT_CONVOLUTION_H

#include <math/matrix.h>

extern double MAT_ZMGaussian(double x, double sigma);
extern double MAT_InvZMGaussian(double y, double sigma);
extern double MAT_ZMGaussianDerivative(double x, double sigma);
extern int MAT_ZMGaussianKernel(double sigma, MAT_Array **p_kernel, MAT_Array **p_derivatives);
extern int MAT_ArrayConvolve(
    MAT_Array *kernel,
    MAT_Array *input,
    MAT_Array *output,
    int increment);
extern int MAT_MatrixConvolveRows(MAT_Array *kernel, MAT_Matrix *input, MAT_Matrix *output);
extern int MAT_MatrixConvolveCols(MAT_Array *kernel, MAT_Matrix *input, MAT_Matrix *output);
extern int MAT_MatrixConvolve(MAT_Array *kernel, MAT_Matrix *input, MAT_Matrix *output);
extern int MAT_MatrixConvolveAndDerivative(
    MAT_Array *gaussian,
    MAT_Array *derivatives,
    MAT_Matrix *input,
    MAT_Matrix *output);

#endif /* MAT_CONVOLUTION_H */
