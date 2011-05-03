#ifndef IMT_IMAGE_H
#define IMT_IMAGE_H

#include <math/array.h>

#define UBYTE2FLOAT(v) ((unsigned char)(v)/255.f)
#define FLOAT2UBYTE(v) ((unsigned char)((float)(v)*255.f))

typedef enum IMT_Format
{
    IMT_PIXFMT_UNKNOWN=0,
    
    /* Integer channels */
    IMT_PIXFMT_GRAY8,
    IMT_PIXFMT_GRAYA16,
    IMT_PIXFMT_RGB24,
    IMT_PIXFMT_RGBA32,

    /* Float channels */
} IMT_Format;

typedef struct IMT_Image
{
    void *data;
    IMT_Format format;
    unsigned int width;
    unsigned int height;
    unsigned int bpp;
    unsigned int stride;
    unsigned int channels;
    
    MAT_Array *float_array;   /* Same image but using float channels, NULL if not used. */
} IMT_Image;

extern int IMT_AllocImage(IMT_Image **p_image,
                          IMT_Format fmt,
                          unsigned int width,
                          unsigned int height,
                          unsigned int padding,
                          void *options);
extern void IMT_FreeImage(IMT_Image *image);
extern int IMT_Grayscale(IMT_Image *src, IMT_Image **p_dst);
extern int IMT_GenerateFloatData(IMT_Image *image, int empty);
extern int IMT_FromFloatData(IMT_Image *image);
extern int IMT_ImageConvolve(MAT_Array *kernel, IMT_Image *input, IMT_Image *output);

#endif /* IMT_IMAGE_H */
