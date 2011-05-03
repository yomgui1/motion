#include "imtool/image.h"
#include "imtool/error.h"
#include "math/convolution.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

static int merge_from_array(MAT_Array *src, IMT_Image *image)
{
    unsigned int x, y;

    switch (image->bpp)
    {
        case 1:
            for (y=0; y < image->height; y++)
            {
                float *sp = src->data.float_ptr + (y * image->width);
                char *dp = image->data + (y * image->stride);
                
                for (x=0; x < image->width; x++)
                {
                    dp[0] = FLOAT2UBYTE(sp[0]);
                    
                    sp += 1;
                    dp += 1;
                }
            }
            break;

        case 2:
            for (y=0; y < image->height; y++)
            {
                float *sp = src->data.float_ptr + (y * image->width);
                char *dp = image->data + (y * image->stride);
                
                for (x=0; x < image->width; x++)
                {
                    dp[0] = FLOAT2UBYTE(sp[0]);
                    dp[1] = FLOAT2UBYTE(sp[1]);

                    sp += 2;
                    dp += 2;
                }
            }
            break;

        case 3:
            for (y=0; y < image->height; y++)
            {
                float *sp = src->data.float_ptr + (y * image->width);
                char *dp = image->data + (y * image->stride);
                
                for (x=0; x < image->width; x++)
                {
                    dp[0] = FLOAT2UBYTE(sp[0]);
                    dp[1] = FLOAT2UBYTE(sp[1]);
                    dp[2] = FLOAT2UBYTE(sp[2]);

                    sp += 3;
                    dp += 3;
                }
            }
            break;

        case 4:
            for (y=0; y < image->height; y++)
            {
                float *sp = src->data.float_ptr + (y * image->width);
                char *dp = image->data + (y * image->stride);
                
                for (x=0; x < image->width; x++)
                {
                    dp[0] = FLOAT2UBYTE(sp[0]);
                    dp[1] = FLOAT2UBYTE(sp[1]);
                    dp[2] = FLOAT2UBYTE(sp[2]);
                    dp[3] = FLOAT2UBYTE(sp[3]);
                    
                    sp += 4;
                    dp += 4;
                }
            }
            break;
            
        default:
			for (y=0; y < image->height; y++)
            {
                float *sp = src->data.float_ptr + (y * image->width);
                char *dp = image->data + (y * image->stride);
                
                for (x=0; x < image->width; x++)
                {
                    int n;

                    for (n=0; n < image->bpp; n++)
                        *dp++ = FLOAT2UBYTE(*sp++);
                }
            }
    }

    return IMT_ERR_NOERROR;
}

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

        default: return 0;
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

        default: return 0;
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
    int bpp;
    unsigned int stride;
	
    bpp = IMT_GetBytesPerPixel(fmt);
    if (bpp <= 0)
        return IMT_ERR_FORMAT;

    stride = bpp * width + padding;

    //printf("\n[DBG] alloc image: %ux%u, fmt=%u, bpp=%u, stride=%u\n", width, height, fmt, bpp, stride);

	image = malloc(sizeof(*image) + (height*stride));
	if (image)
	{
		image->format = fmt;
		image->width = width;
		image->height = height;
        image->bpp = bpp;
		image->stride = stride;
		image->data = ((char *)image) + sizeof(*image);
        image->levels = 0;
        image->floatimage = NULL;
        image->subimages = NULL;
        image->derivatives = NULL;

        *p_image = image;
	}
	
	return IMT_ERR_NOERROR;
}

void IMT_FlushImage(IMT_Image *image)
{
    int i;

    if (NULL != image->subimages)
    {
        for (i=0; i < image->levels; i++)
            MAT_FreeArray(image->subimages[i]);
        free(image->subimages);
        image->levels = 0;
    }

    MAT_FreeArray(image->floatimage);
    MAT_FreeArray(image->derivatives);
}

void IMT_FreeImage(IMT_Image *image)
{
	if (image)
    {
        IMT_FlushImage(image);
		free(image);
    }
}

int IMT_ImageFromFloatArray(MAT_Array *src,
                            IMT_Image **p_image,
                            IMT_Format format,
                            unsigned int width,
                            unsigned int height)
{
    int err;

    if ((src->type != MAT_ARRAYTYPE_FLOAT) || (src->width < width*height))
        return IMT_ERR_FORMAT;

    err = IMT_AllocImage(p_image, format, width, height, 0, NULL);
    if (err)
        return err;

    return merge_from_array(src, *p_image);
}

int IMT_Grayscale(IMT_Image *src, IMT_Image **p_dst)
{
    int err;
	unsigned int y, i;
	char *pix_dst;
	
	err = IMT_AllocImage(p_dst, IMT_PIXFMT_GRAY8,
                         src->width, src->height, 0, NULL);
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

MAT_Array *IMT_GetFloatImage(IMT_Image *image, int empty)
{
    unsigned int x, y;
    unsigned char *sp;
    float *dp;

    /* Float array allocation */
    if (NULL == image->floatimage)
    {
        image->floatimage = MAT_AllocArray(image->width * image->height,
                                           MAT_ARRAYTYPE_FLOAT, empty);
        if (NULL == image->floatimage)
            return NULL;
    }
    else
        return image->floatimage;

	if (empty)
		return image->floatimage;
	
    /* Image data convertion */
    switch (image->bpp)
    {
        case 1:
            for (y=0; y < image->height; y++)
            {
                sp = image->data + (y * image->stride);
                dp = image->floatimage->data.float_ptr + (y * image->width);
                
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
                dp = image->floatimage->data.float_ptr + (y * image->width);

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
                dp = image->floatimage->data.float_ptr + (y * image->width);

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
                dp = image->floatimage->data.float_ptr + (y * image->width);

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
            
        default:
            for (y=0; y < image->height; y++)
            {
                sp = image->data + (y * image->stride);
                dp = image->floatimage->data.float_ptr + (y * image->width);

                for (x=0; x < image->width; x++)
                {
                    int n;

                    for (n=0; n < image->bpp; n++)
                        *dp++ = UBYTE2FLOAT(*sp++);
                }
            }
            break;
    }

    return image->floatimage;
}

int IMT_FromFloatData(IMT_Image *image)
{
    if (NULL == image->floatimage)
        return IMT_ERR_SYS;
	
    return merge_from_array(image->floatimage, image);
}

int IMT_ImageConvolve(MAT_Array *kernel, IMT_Image *input, IMT_Image *output)
{
	int res;
	MAT_Array *tmp;

    /* Works only on gray 8bits images */
    if (input->format != IMT_PIXFMT_GRAY8)
        return -1;
	
	/* Make sure the images have float data arrays */
	if (NULL == IMT_GetFloatImage(input, 0)) return 1;
	if (NULL == IMT_GetFloatImage(output, 1)) return 1;
	
	tmp = MAT_AllocArray(input->width * input->height, input->floatimage->type, 0);
	if (NULL == tmp) return 1;
	
    res = MAT_ArrayConvolve(kernel, input->floatimage, tmp, 1);
    res |= MAT_ArrayConvolve(kernel, tmp, output->floatimage, output->width);

	MAT_FreeArray(tmp);
	return res;
}

int IMT_GeneratePyramidalSubImages(IMT_Image *image, unsigned int max_level, double sigma)
{
    MAT_Array *level0, *subimage, *kernel=NULL;
    unsigned int x, y, i, sub_width, sub_height, level=0;
    float *src, *dst;

    /* Works only on gray 8bits images */
    if (image->format != IMT_PIXFMT_GRAY8)
        return -1;

    /* Make sure that level-0 exists, same image with float channels */
    level0 = IMT_GetFloatImage(image, 0);
    if (NULL == level0)
        return -1;

    /* Check if subimages already exist and alloc more rooms if needed */
    if (image->levels < max_level)
    {
        void *mem = realloc(image->subimages, max_level * sizeof(MAT_Array*));

        if (NULL == mem)
            return image->levels;

        image->subimages = mem;
    }

    /* Start with the highest level stored (we suppose non-null image dimensions) */
    sub_width = image->width >> image->levels;
    sub_height = image->height >> image->levels;
    if (image->levels > 0)
        src = image->subimages[image->levels-1]->data.float_ptr;
    else
        src = level0->data.float_ptr;

    /* if gaussian blur needed, compute kernels */
    if (sigma > 0.0)
        MAT_ZMGaussianKernel(sigma, &kernel, &image->derivatives);

    /* Down-sample successive levels while subimage's dimensions are greater than 1
     * or reach the maximal requested level.
     */
    level = image->levels;
    //printf("\nStarting at level %u, size=%ux%u, max_level=%u\n", level, sub_width, sub_height, max_level);
    while ((level < max_level) && (sub_width > 1) && (sub_height > 1))
    {
        unsigned int width=sub_width;

        sub_width >>= 1;
        sub_height >>= 1;

        subimage = MAT_AllocArray(sub_width*sub_height, MAT_ARRAYTYPE_FLOAT, 0);
        if (NULL == subimage)
        {
            image->levels = level;
            return IMT_ERR_MEM;
        }

        level++;
        image->subimages[level-1] = subimage;

        dst = subimage->data.float_ptr;
        //printf("downsampling @ %ux%u...", sub_width, sub_height);
        for (y=0; y < sub_height; y++, src += 2*width, dst += sub_width)
        {
            float *src_pixel = src;
            float *dst_pixel = dst;

            for (x=0; x < sub_width; x++, src_pixel += 2, dst_pixel++)
            {
                float sum;

                /* average source pixels to obtain sub-sampled pixel */
                sum  = src_pixel[0];
                sum += src_pixel[1];
                sum += src_pixel[0+width];
                sum += src_pixel[1+width];

                *dst_pixel = sum / 4.f;
            }
        }
        //printf("done\n");

        if (NULL != kernel)
        {
            MAT_Array *tmp;

            //printf("Blur...");

            tmp = MAT_AllocArray(subimage->width, subimage->type, 0);
            if (NULL != tmp)
            {
                MAT_ArrayConvolve(kernel, subimage, tmp, 1);
                MAT_ArrayConvolve(kernel, tmp, subimage, sub_width);
                MAT_FreeArray(tmp);
            }
            //printf("done\n");
        }

        src = subimage->data.float_ptr;
    }

    //printf("Reach level: %u\n", level);

    /* adapt subimages array size */
    if (level < image->levels)
    {
        void *mem;

        for (i=level; i < image->levels; i++)
        {
            MAT_FreeArray(image->subimages[i]);
            image->subimages[i] = NULL;
        }

        image->levels = level;

        mem = realloc(image->subimages, level * sizeof(MAT_Array*));
        if (NULL == mem)
            return level; /* As the array is not touch we can live with NULL entries */

        image->subimages = mem;
    }
    else
        image->levels = level;

    return level;
}
