#include "imtool/image.h"
#include "imtool/error.h"
#include "math/convolution.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

static int merge_from_float(IMT_Image *image, float *src)
{
    unsigned int x, y;

    switch (image->bpp)
    {
        case 1:
            for (y=0; y < image->height; y++)
            {
                float *sp = src + (y * image->width);
                unsigned char *dp = image->data + (y * image->stride);
                
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
                float *sp = src + (y * image->width);
                unsigned char *dp = image->data + (y * image->stride);
                
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
                float *sp = src + (y * image->width);
                unsigned char *dp = image->data + (y * image->stride);
                
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
                float *sp = src + (y * image->width);
                unsigned char *dp = image->data + (y * image->stride);
                
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
                float *sp = src + (y * image->width);
                unsigned char *dp = image->data + (y * image->stride);
                
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

int IMT_InitImage(
    IMT_Image *  image,
    IMT_Format   fmt,
    unsigned int width,
    unsigned int height,
    unsigned int padding,
    void *       data)
{
    int bpp;
    unsigned int stride;

    bpp = IMT_GetBytesPerPixel(fmt);
    if (bpp <= 0)
        return IMT_ERR_FORMAT;

    stride = bpp * width + padding;

    //printf("\n[DBG] init image: %ux%u, fmt=%u, bpp=%u, stride=%u\n", width, height, fmt, bpp, stride);

    if (NULL == data)
    {
        image->_alloc_data = malloc(height * stride);
        if (NULL == image->_alloc_data) return IMT_ERR_MEM;
        image->data = image->_alloc_data;
    }
    else
    {
        image->_alloc_data = NULL;
        image->data = data;
    }

    image->format = fmt;
    image->width = width;
    image->height = height;
    image->bpp = bpp;
    image->stride = stride;
    image->floatimage = NULL;

    /* for pyramidal representation */
    image->levels = 0;
    image->subimages = NULL;

    return IMT_ERR_NOERROR;
}

int IMT_AllocImage(
    IMT_Image ** p_image,
    IMT_Format   fmt,
    unsigned int width,
    unsigned int height,
    unsigned int padding,
    void *       data)
{
	IMT_Image *image;
    
	image = malloc(sizeof(*image));
	if (NULL != image)
    {
        int err = IMT_InitImage(image, fmt, width, height, padding, data);
        if (err)
        {
            free(image);
            return err;
        }
    }

    *p_image = image;

	return IMT_ERR_NOERROR;
}

void IMT_FlushImage(IMT_Image *image)
{
    int i;

    if (NULL != image->subimages)
    {
        for (i=0; i <= image->levels; i++)
            MAT_FreeMatrix(image->subimages[i]);
        free(image->subimages);
        image->subimages = NULL;
        image->levels = 0;
    }

    MAT_FreeMatrix(image->floatimage);
}

void IMT_FreeImage(IMT_Image *image)
{
	if (image)
    {
        IMT_FlushImage(image);
        free(image->_alloc_data);
		free(image);
    }
}

int IMT_AllocImageFromFloat(
    IMT_Image **p_image,
    IMT_Format format,
    unsigned int width,
    unsigned int height,
    float *data)
{
    int err;

    err = IMT_AllocImage(p_image, format, width, height, 0, NULL);
    if (err)
        return err;

    return merge_from_float(*p_image, data);
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
					unsigned char *pixel = src->data + (y*src->stride + i);
					unsigned int sum;
					
					sum  = pixel[0]*6969ul;
					sum += pixel[1]*23434ul;
					sum += pixel[2]*2365ul;
					
					pix_dst[0] = sum / 32768ul;
					pix_dst++;
				}
			}
			break;
			
		case IMT_PIXFMT_RGBA32:
			for (y=0; y < src->height; y++)
			{
				for (i=0; i < src->stride; i += src->bpp)
				{
					unsigned char *pixel = src->data + (y*src->stride + i);
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
					unsigned char *pixel = src->data + (y*src->stride + i);
					
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

MAT_Matrix *IMT_GetFloatImage(IMT_Image *image, int empty)
{
    unsigned int x, y;
    unsigned char *sp;
    float *dp;

    /* Float array allocation */
    if (NULL == image->floatimage)
    {
        image->floatimage = MAT_AllocMatrix(image->height, image->width, MAT_ARRAYTYPE_FLOAT, empty, NULL);
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
                dp = image->floatimage->array.data.float_ptr + (y * image->width);
                
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
                dp = image->floatimage->array.data.float_ptr + (y * image->width);

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
                dp = image->floatimage->array.data.float_ptr + (y * image->width);

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
                dp = image->floatimage->array.data.float_ptr + (y * image->width);

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
                dp = image->floatimage->array.data.float_ptr + (y * image->width);

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
	
    return merge_from_float(image, image->floatimage->array.data.float_ptr);
}

int IMT_ImageConvolve(MAT_Array *kernel, IMT_Image *input, IMT_Image *output)
{
	int res;
	MAT_Matrix *tmp;

    /* Works only on gray 8bits images */
    if (input->format != IMT_PIXFMT_GRAY8)
        return -1;

	/* Make sure the images have float data arrays */
	if (NULL == IMT_GetFloatImage(input, 0)) return 1;
	if (NULL == IMT_GetFloatImage(output, 1)) return 1;
	
	tmp = MAT_AllocMatrix(input->height, input->width, MAT_ARRAYTYPE_FLOAT, 0, NULL);
	if (NULL == tmp) return 1;
	
    res = MAT_MatrixConvolve(kernel, input->floatimage, tmp);

	MAT_FreeMatrix(tmp);
	return res;
}

int IMT_GeneratePyramidalSubImages(IMT_Image *image, unsigned int max_level, double sigma)
{
    MAT_Matrix *base, *subimage, *previous_subimage;
    MAT_Array *kernel=NULL, *derivatives=NULL;
    unsigned int x, y, i, sub_width, sub_height, level=0;
    float *src, *dst;

    /* Works only on gray 8bits images */
    if (image->format != IMT_PIXFMT_GRAY8)
        return -1;

    /* Make sure a float version exists */
    base = IMT_GetFloatImage(image, 0);
    if (NULL == base)
        return -1;
  
#if 0      
    {
		int j;
		MAT_Matrix *mat = base;
		printf("Dumping base:\n");
		for (i = 0; i < mat->nrows; ++i)  {
			printf("%p [", &mat->array.data.float_ptr[i*mat->ncols]);
			for (j = 0; j < mat->ncols; j++)  {
				printf(" %f", mat->array.data.float_ptr[i*mat->ncols+j]);
			}
			printf(" ]\n");
		}
	}
#endif

    /* compute gaussian & derivatives kernels */
    MAT_ZMGaussianKernel(sigma, &kernel, &derivatives);
    if (NULL == kernel)
        return -1;

    /* Check if level 0 exists, create it if not */
    if (NULL == image->subimages)
    {
        image->levels = 0;
        image->subimages = malloc((max_level+1) * sizeof(MAT_Matrix*));
        if (NULL == image->subimages) return -1;

        subimage = MAT_AllocMatrix(image->height, 3*image->width, MAT_ARRAYTYPE_FLOAT, 0, NULL);
        if (NULL == subimage)
        {
            free(image->subimages);
            return -1;
        }
        image->subimages[0] = subimage;

        MAT_MatrixConvolveAndDerivative(kernel, derivatives, base, subimage);
    }
    else
    {
        /* Check if enough levels exist or realloc more rooms if needed */
        if (image->levels < max_level)
        {
            void *mem = realloc(image->subimages, (max_level+1) * sizeof(MAT_Matrix*));
            if (NULL == mem) return image->levels;
            image->subimages = mem;
        }
    }

    /* Start to compute level after the highest one stored */
    level = image->levels;
    sub_width = image->width >> level;
    sub_height = image->height >> level;
    previous_subimage = NULL;

    /* Downsample by 2 successive subimages while subimage's dimensions are greater than 1
     * or until we reach the maximal requested level.
     */
    //printf("\nStarting at level %u, size=%ux%u, max_level=%u\n", level, sub_width, sub_height, max_level);
    while ((level < max_level) && (sub_width > 1) && (sub_height > 1))
    {
        unsigned int previous_width=sub_width;

		level++;
        sub_width >>= 1;
        sub_height >>= 1;

		//printf("Level %u, downsampling @ %ux%u...", level, sub_width, sub_height);
        subimage = MAT_AllocMatrix(sub_height, sub_width, MAT_ARRAYTYPE_FLOAT, 0, NULL);
        image->subimages[level] = MAT_AllocMatrix(sub_height, 3*sub_width, MAT_ARRAYTYPE_FLOAT, 0, NULL);
        if ((NULL == subimage) || (NULL == image->subimages[level]))
        {
            MAT_FreeMatrix(subimage);
            MAT_FreeMatrix(image->subimages[level]);
            if (previous_subimage)
                MAT_FreeMatrix(previous_subimage);
            level--;
            break;
        }

		//printf("data @ %p\n", level, image->subimages[level]->array.data.void_ptr);

        if (previous_subimage)
            src = previous_subimage->array.data.float_ptr;
        else
            src = base->array.data.float_ptr;

        dst = subimage->array.data.float_ptr;
        for (y=0; y < sub_height; y++, src+=2*previous_width, dst+=sub_width)
        {
            float *src_pixel = src;
            float *dst_pixel = dst;

            //printf("%u [", level);
            for (x=0; x < sub_width; x++, src_pixel+=2, dst_pixel++)
            {
                float sum;

                /* average source pixels to obtain sub-sampled pixel */
                sum  = src_pixel[0];
                sum += src_pixel[1];
                sum += src_pixel[0+previous_width];
                sum += src_pixel[1+previous_width];
                
                //printf("%02x", (int)((sum/4)*255));
        
                *dst_pixel = sum / 4.f;
            }
            //printf(" ]\n");
        }
        //printf("done\n");

        if (previous_subimage)
             MAT_FreeMatrix(previous_subimage);

        /* Apply Gaussian and derivatives kernels */
        MAT_MatrixConvolveAndDerivative(kernel, derivatives, subimage, image->subimages[level]);

        previous_subimage = subimage;
    }

    //printf("Reached level: %u\n", level);

    /* reduce the memory usage */
    if (level < image->levels)
    {
        void *mem;

        for (i=level+1; i <= image->levels; i++)
        {
            MAT_FreeMatrix(image->subimages[i]);
            image->subimages[i] = NULL;
        }

        image->levels = level;

        mem = realloc(image->subimages, level * sizeof(MAT_Matrix*));
        if (NULL == mem)
            return level; /* As the array is not touch we can live with NULL entries */

        image->subimages = mem;
    }
    else
        image->levels = level;

    return level;
}
