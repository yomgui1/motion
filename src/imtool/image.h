#ifndef IMT_IMAGE_H
#define IMT_IMAGE_H

typedef enum IMT_Format
{
    IMT_PIXFMT_UNKNOWN=0,
    IMT_PIXFMT_GREYSCALE8,
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
    unsigned int stride;
} IMT_Image;

#endif /* IMT_IMAGE_H */
