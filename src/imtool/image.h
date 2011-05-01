#ifndef IMT_IMAGE_H
#define IMT_IMAGE_H

typedef enum IMT_Format
{
    IMT_PIXFMT_UNKNOWN=0,
    IMT_PIXFMT_GRAY8,
    IMT_PIXFMT_GRAYA16,
    IMT_PIXFMT_RGB24,
    IMT_PIXFMT_RGBA32,
    IMT_PIXFMT_ARGB32,
    IMT_PIXFMT_BGRA32,
    IMT_PIXFMT_ABGR32,
} IMT_Format;

typedef struct IMT_Image
{
    void *data;
    IMT_Format format;
    unsigned int width;
    unsigned int height;
    unsigned int bpp;
    unsigned int stride;
} IMT_Image;

extern IMT_Image *IMT_AllocImage(IMT_Format fmt, unsigned int width, unsigned int height, unsigned int bpp, unsigned int stride, void *options);
extern void IMT_FreeImage(IMT_Image *image);
extern int IMT_Grayscale(IMT_Image *src, IMT_Image **p_dst);

#endif /* IMT_IMAGE_H */
