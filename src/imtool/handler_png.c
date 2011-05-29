#include "handler_intern.h"

#include <png.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct PNGUserData
{
	void *mem;
	unsigned int size;
	unsigned int seek;
};

static int imt_png_recognize_from_file(const char *filename)
{
	char *dot = strrchr(filename, '.');
	
	if (dot && !strcasecmp(dot, ".png"))
		return 1;
	return 0;
}

static int imt_png_load_from_file(const char *filename, IMT_Image **p_image, void *options)
{
	int err=IMT_ERR_NOERROR;
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	IMT_Image *image = *p_image;
	int color_type, bytesperpixel, depth, i;
	png_uint_32 width, height;
	IMT_Format fmt;
	png_bytepp row_pointers;
	
	fp = fopen(filename, "rb");
	if (NULL != fp)
	{
		png_byte head[8];
		
		/* Read PNG header */
		if (fread(head, 1, 8, fp) != 8)
		{
			err = IMT_ERR_IO;
			goto close_fp;
		}
			
		/* Is PNG file ? */
		if (png_sig_cmp(head, 0, 8))
		{
			err = IMT_ERR_INVALID_FILE;
			goto close_fp;
		}
		
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL)
		{
			fprintf(stderr, "Cannot png_create_read_struct\n");
			err = IMT_ERR_MEM;
			goto close_fp;
		}
		
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL)
		{
			png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
			fprintf(stderr, "Cannot png_create_info_struct\n");
			err = IMT_ERR_MEM;
			goto close_fp;
		}
		
		png_init_io(png_ptr, fp);
		png_set_sig_bytes(png_ptr, 8);

		err = IMT_ERR_SYS;
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			png_error(png_ptr, "PNG ERROR");
			png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
			if (image)
				IMT_FreeImage(image);
			*p_image = NULL;
			goto close_fp;
		}

		png_read_info(png_ptr, info_ptr);
		png_get_IHDR(png_ptr, info_ptr, &width, &height, &depth, &color_type, NULL, NULL, NULL);

		if (depth == 16)
		{
			png_set_strip_16(png_ptr);
			depth = 8;
		}

		bytesperpixel = png_get_channels(png_ptr, info_ptr);

		switch(color_type)
		{
			case PNG_COLOR_TYPE_RGB: fmt = IMT_PIXFMT_ARGB32; break;
			case PNG_COLOR_TYPE_RGB_ALPHA: fmt = IMT_PIXFMT_ARGB32; break;
			case PNG_COLOR_TYPE_GRAY: fmt = IMT_PIXFMT_GRAY8; break;
			case PNG_COLOR_TYPE_GRAY_ALPHA: fmt = IMT_PIXFMT_GRAYA16; break;
				
			default:
				fprintf(stderr, "PNG format not supported (%u)\n", color_type);
				err = IMT_ERR_FORMAT;
				longjmp(png_jmpbuf(png_ptr), 1);
		}

		if (((fmt == IMT_PIXFMT_GRAY8) || (fmt == IMT_PIXFMT_GRAYA16)) && (depth < 8))
		{
			png_set_expand(png_ptr);
			depth = 8;
		}
		
		if (color_type == PNG_COLOR_TYPE_RGB)
        {
#if	BYTE_ORDER == LITTLE_ENDIAN
			png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);
            png_set_bgr(png_ptr);
#else
            png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_BEFORE);
            png_set_swap(png_ptr);
#endif
        }
        else if (fmt == IMT_PIXFMT_ARGB32)
			png_set_swap_alpha(png_ptr);
		
		/* IMT Image allocation */
		err = IMT_AllocImage(&image, fmt, width, height, 0, NULL);
		if (err)
			longjmp(png_jmpbuf(png_ptr), 1);
			
		*p_image = image;
		
		row_pointers = malloc(height*sizeof(png_bytep));
		if (NULL == row_pointers)
		{
			err = IMT_ERR_MEM;
			fprintf(stderr, "Cannot allocate row-pointers array\n");
			longjmp(png_jmpbuf(png_ptr), 1);
		}

		/* set the individual row-pointers to point at the correct offsets */
		for (i = 0; i < height; i++)
			row_pointers[i] = (png_bytep)((unsigned char *)image->data + i * image->stride);

		err = IMT_ERR_IO;
		png_read_image(png_ptr, row_pointers);
		png_read_end(png_ptr, info_ptr);
		
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		free(row_pointers);
		
		err = IMT_ERR_NOERROR;
		
	close_fp:
		fclose(fp);
	}
	else
		err = IMT_ERR_IO;
	
	return err;
}

static int imt_png_save_to_file(const char *filename, IMT_Image *image, void *options)
{
	int err=IMT_ERR_NOERROR;
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	int color_type, depth, i;
	png_bytepp row_pointers;
	
	fp = fopen(filename, "wb");
	if (NULL != fp)
	{
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL)
		{
			fprintf(stderr, "Cannot png_create_write_struct\n");
			err = IMT_ERR_MEM;
			goto close_fp;
		}
		
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL)
		{
			png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
			fprintf(stderr, "Cannot png_create_info_struct\n");
			err = IMT_ERR_MEM;
			goto close_fp;
		}

		err = IMT_ERR_SYS;
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			png_error(png_ptr, "PNG ERROR");
			png_destroy_write_struct(&png_ptr, &info_ptr);
			goto close_fp;
		}
		
		png_init_io(png_ptr, fp);
		png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
		
		depth = 8;
		switch (image->format)
		{
			case IMT_PIXFMT_GRAY8: color_type = PNG_COLOR_TYPE_GRAY; break;
			case IMT_PIXFMT_GRAYA16: color_type = PNG_COLOR_TYPE_GRAY_ALPHA; break;
			case IMT_PIXFMT_RGB24: color_type = PNG_COLOR_TYPE_RGB; break;
			case IMT_PIXFMT_ARGB32: color_type = PNG_COLOR_TYPE_RGB_ALPHA; break;
			default:
				fprintf(stderr, "Image format not supported (%u)\n", image->format);
				err = IMT_ERR_FORMAT;
				longjmp(png_jmpbuf(png_ptr), 1);
		}
		
		if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
			png_set_swap_alpha(png_ptr);
		
		//if (image->format == IMT_PIXFMT_RGB24)
		//	png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);
		
		png_set_IHDR(png_ptr, info_ptr,
					 image->width, image->height, depth, color_type,
					 PNG_INTERLACE_NONE,
					 PNG_COMPRESSION_TYPE_DEFAULT,
					 PNG_FILTER_TYPE_DEFAULT);
					
		row_pointers = png_malloc(png_ptr, image->height*png_sizeof(png_bytep));
		if (NULL == row_pointers)
		{
			err = IMT_ERR_MEM;
			fprintf(stderr, "Cannot allocate row-pointers array\n");
			longjmp(png_jmpbuf(png_ptr), 1);
		}

		/* set the individual row-pointers to point at the correct offsets */
		for (i = 0; i < image->height; i++)
			row_pointers[i] = (png_bytep)((unsigned char *)image->data + i * image->stride);
					
		err = IMT_ERR_IO;
		png_write_info(png_ptr, info_ptr);
		png_write_image(png_ptr, row_pointers);
		png_write_end(png_ptr, info_ptr);
		
		png_destroy_write_struct(&png_ptr, &info_ptr);
		png_free(png_ptr, row_pointers);
		err = IMT_ERR_NOERROR;
		
	close_fp:
		fclose(fp);
	}
	else
		err = IMT_ERR_IO;
	
	return err;
}

_IMT_Handler _imt_png_handler =
{
	recognize_from_name: imt_png_recognize_from_file,
	load : imt_png_load_from_file,
	save : imt_png_save_to_file,
};
