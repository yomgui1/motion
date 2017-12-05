#include "imtool/handler.h"
#include "imtool/error.h"
#include "math/convolution.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int i, err, width, level;
	IMT_Image *input, *output;
	MAT_Array *kernel;
	MAT_Array *derivatives;
	double sigma;
    char *name;

	if (argc < 4)
	{
		fprintf(stderr, "No enough parameters\nUSAGE: %s <input_image> <output_image> sigma\n", argv[0]);
		return 1;
	}

	sigma = strtod(argv[3], NULL);

	IMT_Init();

	printf("Loading image '%s' ", argv[1]);
	err = IMT_Load(argv[1], &input, NULL);
	if (err || !input)
	{
		printf("[FAILED], %s\n", IMT_GetErrorString(err));
		goto bye;
	}
	else
		printf("[OK]\n");

	printf("Generating grayscale version of input image ");
	err = IMT_GenerateGrayscale(input, 0);
	if (err)
	{
		printf("[FAILED], %s\n", IMT_GetErrorString(err));
		goto bye;
	}
	else
		printf("[OK]\n");

	printf("Allocating output image ");
	err = IMT_AllocImage(&output, input->format, input->width, input->height, 0, NULL);
	if (err || !output)
	{
		printf("[FAILED], %s\n", IMT_GetErrorString(err));
		goto bye;
	}
	else
		printf("[OK]\n");

	printf("Computing Gaussian kernel ");
	width = MAT_ZMGaussianKernel(sigma, &kernel, &derivatives);
	if (!width || !kernel || !derivatives)
	{
		printf("[FAILED]\n");
		goto bye;
	}
	else
		printf("[OK], width=%u\n", width);

	printf("Convolve ");
	if (IMT_ImageConvolve(kernel, input, output))
	{
		printf("[FAILED]\n");
		goto bye;
	}
	else
		printf("[OK]\n");

    printf("Create pyramidal data of input image ");
    level = IMT_GeneratePyramidalSubImages(input, 4, sigma);
	if (level < 0)
	{
		printf("[FAILED]\n");
		goto bye;
	}
	else
		printf("[OK]\n");

    for (i=0; i <= input->levels; i++)
    {
        IMT_Image *tmp;
        MAT_Array *pixel_plan;

        /* Subimages are arrays of 3d vectors (pixel, dpx, dpy) */
        pixel_plan = MAT_ExtractArrayPlan(&input->subimages[i]->array, 3, 0);

        printf("Converting subimage %u  (%ux%u)", i, input->width >> i, input->height >> i);
        err = IMT_AllocImageFromFloat(&tmp,
									  IMT_PIXFMT_GRAY8,
									  input->width >> i,
									  input->height >> i,
									  pixel_plan->data.float_ptr);
        MAT_FreeArray(pixel_plan);

        if (err)
        {
            printf("[FAILED], %s\n", IMT_GetErrorString(err));
            continue;
        }
        else
            printf("[OK]\n");


        if (!err)
        {
            char name[80];

            sprintf(name, "subimage_%02u.png", i);
            printf("Saving subimage '%s' ", name);
            err = IMT_Save(name, tmp, NULL);
            if (err)
                printf("[FAILED], %s\n", IMT_GetErrorString(err));
            else
                printf("[OK]\n");

            IMT_FreeImage(tmp);
        }
    }

bye:
	IMT_FreeImage(input);
	IMT_FreeImage(output);
	MAT_FreeArray(kernel);
	MAT_FreeArray(derivatives);
	return 0;
}
