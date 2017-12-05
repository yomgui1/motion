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
        case IMT_PIXFMT_ARGB32:
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
        case IMT_PIXFMT_ARGB32:
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
    image->grayscale = NULL;

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
	int err = IMT_ERR_NOERROR;
	IMT_Image *image;

	image = malloc(sizeof(*image));
	if (NULL != image)
    {
        err = IMT_InitImage(image, fmt, width, height, padding, data);
        if (err)
        {
            free(image);
            image = NULL;
        }
    }

    *p_image = image;
	return err;
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

    MAT_FreeMatrix(image->grayscale);
    image->grayscale = NULL;
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

void IMT_SwapAlpha(IMT_Image *image)
{
    unsigned int x,y;

    if (image->format == IMT_PIXFMT_GRAYA16)
    {
        for (y=0; y < image->height; y++)
        {
            unsigned char *dp = image->data + (y * image->stride);

            for (x=0; x < image->width; x++)
            {
                unsigned char tmp=dp[0];

                dp[0] = dp[1];
                dp[1] = tmp;

                dp += 2;
            }
        }
    }
    else if (image->format == IMT_PIXFMT_ARGB32)
    {
        for (y=0; y < image->height; y++)
        {
            unsigned char *dp = image->data + (y * image->stride);

            for (x=0; x < image->width; x++)
            {
                unsigned char tmp=dp[0];

                dp[0] = dp[3];
                dp[3] = tmp;

                dp += 4;
            }
        }
    }
}

int IMT_GenerateGrayscale(IMT_Image *src, int empty)
{
	unsigned int y, i;
	float *pix_dst;

	if (NULL == src->grayscale)
	{
		src->grayscale = MAT_AllocMatrix(src->height, src->width, MAT_MATRIXTYPE_FLOAT, 0, NULL);
		if (NULL == src->grayscale)
			return IMT_ERR_MEM;
	}

	if (empty)
		return IMT_ERR_NOERROR;

	pix_dst = src->grayscale->array.data.float_ptr;

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

					pix_dst[0] = sum / 8355840.;
					pix_dst++;
				}
			}
			break;

		case IMT_PIXFMT_ARGB32:
			for (y=0; y < src->height; y++)
			{
				for (i=0; i < src->stride; i += src->bpp)
				{
					unsigned char *pixel = src->data + (y*src->stride + i);
					unsigned int sum;

					sum  = pixel[1]*6969;
					sum += pixel[2]*23434;
					sum += pixel[3]*2365;

					pix_dst[0] = (sum * pixel[0]) / 8355840.;
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

					pix_dst[0] = ((int)pixel[0] * (int)pixel[1]) / 510.;
					pix_dst++;
				}
			}
			break;

		case IMT_PIXFMT_GRAY8:
			for (y=0; y < src->height; y++)
			{
				for (i=0; i < src->stride; i += src->bpp)
				{
					unsigned char *pixel = src->data + (y*src->stride + i);

					pix_dst[0] = pixel[0] / 255.;
					pix_dst++;
				}
			}
			break;

		default:
			fprintf(stderr, "%s: unsupported image source format (%u)\n", __FUNCTION__, src->format);
			MAT_FreeMatrix(src->grayscale);
			src->grayscale = NULL;
			return IMT_ERR_FORMAT;
	}

	return IMT_ERR_NOERROR;
}

MAT_Matrix *IMT_GetFloatImage(IMT_Image *image)
{
    unsigned int x, y;
    unsigned char *sp;
    MAT_Matrix *floatmat;
    float *dp;

    /* Float array allocation */
    floatmat = MAT_AllocMatrix(image->height, image->width, MAT_ARRAYTYPE_FLOAT, 1, NULL);
    if (NULL == floatmat)
		return NULL;

    /* Image data convertion */
    switch (image->bpp)
    {
        case 1:
            for (y=0; y < image->height; y++)
            {
                sp = image->data + (y * image->stride);
                dp = floatmat->array.data.float_ptr + (y * image->width);

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
                dp = floatmat->array.data.float_ptr + (y * image->width);

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
                dp = floatmat->array.data.float_ptr + (y * image->width);

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
                dp = floatmat->array.data.float_ptr + (y * image->width);

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
                dp = floatmat->array.data.float_ptr + (y * image->width);

                for (x=0; x < image->width; x++)
                {
                    int n;

                    for (n=0; n < image->bpp; n++)
                        *dp++ = UBYTE2FLOAT(*sp++);
                }
            }
            break;
    }

    return floatmat;
}

int IMT_FromFloatMatrix(IMT_Image *image, MAT_Matrix *floatmat)
{
    return merge_from_float(image, floatmat->array.data.float_ptr);
}

int IMT_FromFloatBuffer(IMT_Image *image, float *src)
{
    return merge_from_float(image, src);
}

int IMT_ImageConvolve(MAT_Array *kernel, IMT_Image *input, IMT_Image *output)
{
	int err;

	/* Make sure the images have their grascale float matrix */
	err = IMT_GenerateGrayscale(input, 0);
	if (err) return err;
	err = IMT_GenerateGrayscale(output, 1);
	if (err) return err;

    return MAT_MatrixConvolve(kernel, input->grayscale, output->grayscale);
}

int IMT_GeneratePyramidalSubImages(IMT_Image *image, unsigned int max_level, double sigma)
{
    MAT_Matrix *base, *subimage, *previous_subimage;
    MAT_Array *kernel=NULL, *derivatives=NULL;
    unsigned int x, y, i, sub_width, sub_height, level=0;
    float *src, *dst;

    /* Prepare grayscale data */
    if (IMT_GenerateGrayscale(image, 0))
        return -1;
	base = image->grayscale;

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

    /* Compute gaussian & derivatives kernels */
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
