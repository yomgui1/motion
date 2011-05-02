#include "image.h"
#include "error.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

int IMT_GetBytesPerPixel(IMT_Format fmt)
{
    switch (fmt)
    {
        case IMT_PIXFMT_GRAY8:
            return 1;
        case IMT_PIXFMT_GRAYA16:
            return 2;
        case IMT_PIXFMT_RGB24:
            return 3;
        case IMT_PIXFMT_RGBA32:
            return 4;

        default: return -1;
    }
}

int IMT_GetChannels(IMT_Format fmt)
{
    switch (fmt)
    {
        case IMT_PIXFMT_GRAY8:
            return 1;
        case IMT_PIXFMT_GRAYA16:
            return 2;
        case IMT_PIXFMT_RGB24:
            return 3;
        case IMT_PIXFMT_RGBA32:
            return 4;

        default: return -1;
    }
}

int IMT_AllocImage(IMT_Image ** p_image,
                   IMT_Format   fmt,
                   unsigned int width,
                   unsigned int height,
                   unsigned int padding,
                   void *       options)
{
	IMT_Image *image;
    int channels, bpp;
    unsigned int stride;
	
    channels = IMT_GetChannels(fmt);
    if (channels < 0)
        return IMT_ERR_FORMAT;

    bpp = IMT_GetBytesPerPixel(fmt);
    stride = bpp * width + padding;

    printf("[DBG] alloc image: %ux%u, fmt=%u, bpp=%u, stride=%xu\n", width, height, fmt, bpp, stride);

	image = malloc(sizeof(*image) + (height*stride));
	if (image)
	{
		image->format = fmt;
		image->width = width;
		image->height = height;
		image->bpp = bpp;
        image->channels = channels;
		image->stride = stride;
		image->data = ((char *)image) + sizeof(*image);
        image->float_data = NULL;
        image->float_stride = 0;

        *p_image = image;
	}
	
	return IMT_ERR_NOERROR;
}

void IMT_FreeImage(IMT_Image *image)
{
	if (image)
    {
        free(image->float_data);
		free(image);
    }
}

int IMT_GenerateFloatData(IMT_Image *image)
{
    unsigned int x, y;
    unsigned char *sp;
    float *dp;

    /* Clear previous */
    free(image->float_data);

    /* Float image stride (aligned on 16-bytes for SIMD requirements) */
    image->float_stride = image->width * image->channels * sizeof(float);
    image->float_stride = (image->float_stride + 15) & -16;

    image->float_data = malloc(image->height * image->float_stride);
    if (NULL == image->float_data)
        return IMT_ERR_MEM;

    switch (image->channels)
    {
        case 1:
            for (y=0; y < image->height; y++)
            {
                sp = image->data + (y * image->stride);
                dp = (void *)image->float_data + (y * image->stride);
                
                for (x=0; x < image->width; x++)
                {
                    dp[0] = UBYTE2FLOAT(sp[0]);
                    
                    sp += 1;
                    dp += 1;
                }
            }
            break;

        case 2:
            for (y=0; y < image->height; y++)
            {
                sp = image->data + (y * image->stride);
                dp = (void *)image->float_data + (y * image->stride);

                for (x=0; x < image->width; x++)
                {
                    dp[0] = UBYTE2FLOAT(sp[0]);
                    dp[1] = UBYTE2FLOAT(sp[1]);

                    sp += 2;
                    dp += 2;
                }
            }
            break;

        case 3:
            for (y=0; y < image->height; y++)
            {
                sp = image->data + (y * image->stride);
                dp = (void *)image->float_data + (y * image->stride);

                for (x=0; x < image->width; x++)
                {
                    dp[0] = UBYTE2FLOAT(sp[0]);
                    dp[1] = UBYTE2FLOAT(sp[1]);
                    dp[2] = UBYTE2FLOAT(sp[2]);

                    sp += 3;
                    dp += 3;
                }
            }
            break;

        case 4:
            for (y=0; y < image->height; y++)
            {
                sp = image->data + (y * image->stride);
                dp = (void *)image->float_data + (y * image->stride);

                for (x=0; x < image->width; x++)
                {
                    dp[0] = UBYTE2FLOAT(sp[0]);
                    dp[1] = UBYTE2FLOAT(sp[1]);
                    dp[2] = UBYTE2FLOAT(sp[2]);
                    dp[3] = UBYTE2FLOAT(sp[3]);
                    
                    sp += 4;
                    dp += 4;
                }
            }
            break;
    }

    return IMT_ERR_FORMAT;
}

int IMT_Grayscale(IMT_Image *src, IMT_Image **p_dst)
{
    int err;
	unsigned int y, i;
	char *pix_dst;
	
	err = IMT_AllocImage(p_dst, IMT_PIXFMT_GRAY8, src->width, src->height, 0, NULL);
	if (err)
		return err;

	pix_dst = (*p_dst)->data;

	switch(src->format)
	{
		case IMT_PIXFMT_RGB24:
			for (y=0; y < src->height; y++)
			{
				for (i=0; i < src->stride; i += src->bpp)
				{
					char *pixel = src->data + (y*src->stride + i);
					unsigned int sum;
					
					sum  = pixel[0]*6969;
					sum += pixel[1]*23434;
					sum += pixel[2]*2365;
					
					pix_dst[0] = sum / 32768;
					pix_dst++;
				}
			}
			break;
			
		case IMT_PIXFMT_RGBA32:
			for (y=0; y < src->height; y++)
			{
				for (i=0; i < src->stride; i += src->bpp)
				{
					char *pixel = src->data + (y*src->stride + i);
					unsigned int sum;
					
					sum  = pixel[0]*6969;
					sum += pixel[1]*23434;
					sum += pixel[2]*2365;
					
					/* Need to round here, because if we just shift by 23 bits, (255, 255, 255) becomes 254 with alpha = 255. */
					pix_dst[0] = roundf((sum * pixel[3]) / 8388608.f + .5f);
					pix_dst++;
				}
			}
			break;
			
		case IMT_PIXFMT_GRAYA16:
			for (y=0; y < src->height; y++)
			{
				for (i=0; i < src->stride; i += src->bpp)
				{
					char *pixel = src->data + (y*src->stride + i);
					
					/* Same issue as for RGBA32 */
					pix_dst[0] = roundf(((int)pixel[0] * (int)pixel[1]) / 8.f + .5f);
					pix_dst++;
				}
			}
			break;
			
		case IMT_PIXFMT_GRAY8:
			for (y=0; y < src->height; y++)
			{
				memcpy(pix_dst, src->data + y*src->stride, (*p_dst)->stride);
				pix_dst += (*p_dst)->stride;
			}
			break;
			
		default:
			fprintf(stderr, "%s: unsupported image source format (%u)\n", __FUNCTION__, src->format);
			IMT_FreeImage(*p_dst);
			*p_dst = NULL;
			return IMT_ERR_FORMAT;
	}

	return IMT_ERR_NOERROR;
}

