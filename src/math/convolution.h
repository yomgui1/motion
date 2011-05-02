#ifndef MAT_CONVOLUTION_H
#define MAT_CONVOLUTION_H

extern double MAT_ZMGaussian(double x, double sigma);
extern double MAT_InvZMGaussian(double y, double sigma);
extern double MAT_ZMGaussianDerivative(double x, double sigma);
extern int MAT_ZMGaussianKernel(double sigma, float **p_kernel, float **p_kernel_der);
extern void MAT_ArrayHorizConvolve(float *kernel, int kw, float *array, int aw, int ah);
extern void MAT_ArrayVertConvolve(float *kernel, int kw, float *array, int aw, int ah);
extern int MAT_GaussianConvolve(double sigma, float *in, float *out, int w, int h, float *kernel, int kw);

#endif /* MAT_CONVOLUTION_H */
