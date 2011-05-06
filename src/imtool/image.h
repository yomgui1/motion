#ifndef IMT_IMAGE_H
#define IMT_IMAGE_H

#include <math/matrix.h>

#define UBYTE2FLOAT(v) ((unsigned char)(v)/255.f)
#define FLOAT2UBYTE(v) ((unsigned char)((float)(v)*255.f))

/* Only 8bits per channels is supported */
typedef enum IMT_Format
{
    IMT_PIXFMT_UNKNOWN=0,
    IMT_PIXFMT_GRAY8,
    IMT_PIXFMT_GRAYA16,
    IMT_PIXFMT_RGB24,
    IMT_PIXFMT_RGBA32,
} IMT_Format;

typedef struct IMT_Image
{
    IMT_Format   format;
    unsigned int width;
    unsigned int height;
    unsigned int bpp;
    unsigned int stride;
    unsigned int levels;        /* Number-1 of MAT_Array stored by subimages array, not used if subimages is NULL */
    
    void *        _alloc_data;
    void *        data;         /* Raw image pixels data */
    MAT_Matrix *  floatimage;   /* Same image but all channels converted as float, NULL if not used. */
    MAT_Matrix ** subimages;    /* Array of levels 'MAT_Matrix *'. Used for pyramidal representation, NULL if not used */
} IMT_Image;

extern int IMT_InitImage(
    IMT_Image *  image,
    IMT_Format   fmt,
    unsigned int width,
    unsigned int height,
    unsigned int padding,
    void *       data);
extern int IMT_AllocImage(
    IMT_Image **p_image,
    IMT_Format fmt,
    unsigned int width,
    unsigned int height,
    unsigned int padding,
    void *data);
extern void IMT_FlushImage(IMT_Image *image);
extern void IMT_FreeImage(IMT_Image *image);
extern int IMT_Grayscale(IMT_Image *src, IMT_Image **p_dst);
extern MAT_Matrix *IMT_GetFloatImage(IMT_Image *image, int empty);
extern int IMT_FromFloatData(IMT_Image *image);
extern int IMT_AllocImageFromFloat(
    IMT_Image **p_image,
    IMT_Format format,
    unsigned int width,
    unsigned int height,
    float *data);
extern int IMT_ImageConvolve(MAT_Array *kernel, IMT_Image *input, IMT_Image *output);
extern int IMT_GeneratePyramidalSubImages(IMT_Image *image, unsigned int max_level, double sigma);

#endif /* IMT_IMAGE_H */
