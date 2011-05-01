#include "image.h"
#include "error.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

IMT_Image *IMT_AllocImage(IMT_Format fmt, unsigned int width, unsigned int height, unsigned int bpp, unsigned int stride, void *options)
{
	IMT_Image *image;
	
	printf("[DBG] alloc image: %ux%u, %u bpp, stride: %u, fmt: %u\n", width, height, bpp, stride, fmt);
	
	image = malloc(sizeof(*image) + (height*stride));
	if (image)
	{
		image->format = fmt;
		image->width = width;
		image->height = height;
		image->bpp = bpp;
		image->stride = stride;
		image->data = ((char *)image) + sizeof(*image);
	}
	
	return image;
}

void IMT_FreeImage(IMT_Image *image)
{
	if (image)
		free(image);
}

int IMT_Grayscale(IMT_Image *src, IMT_Image **p_dst)
{
	IMT_Image *dst;
	unsigned int y, i;
	char *pix_dst;
	
	*p_dst = dst = IMT_AllocImage(IMT_PIXFMT_GRAY8, src->width, src->height, 1, src->width, NULL);
	if (NULL == dst)
		return IMT_ERR_MEM;
		
	pix_dst = dst->data;
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
				memcpy(pix_dst, src->data + y*src->stride, dst->stride);
				pix_dst += dst->stride;
			}
			break;
			
		default:
			fprintf(stderr, "%s: unsupported image source format (%u)\n", __FUNCTION__, src->format);
			IMT_FreeImage(dst);
			*p_dst = NULL;
			return IMT_ERR_FORMAT;
	}

	return IMT_ERR_NOERROR;
}