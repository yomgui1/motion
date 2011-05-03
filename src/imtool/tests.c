#include "handler.h"
#include "error.h"
#include "math/convolution.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int err, width;
	IMT_Image *input, *output, *gray;
	MAT_Array *kernel;
	MAT_Array *derivatives;
	double sigma;
	
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
	err = IMT_Grayscale(input, &gray);
	if (err || !gray)
	{
		printf("[FAILED], %s\n", IMT_GetErrorString(err));
		goto bye;
	}
	else
		printf("[OK]\n");
		
	printf("Generating float data array of the gray image ");
	err = IMT_GenerateFloatData(gray, 0);
	if (err)
	{
		printf("[FAILED], %s\n", IMT_GetErrorString(err));
		goto bye;
	}
	else
		printf("[OK]\n");
		
	printf("Allocating output image ");
	err = IMT_AllocImage(&output, gray->format, gray->width, gray->height, 0, NULL);
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
	if (IMT_ImageConvolve(kernel, gray, output))
	{
		printf("[FAILED]\n");
		goto bye;
	}
	else
		printf("[OK]\n");
			
	printf("Flushing float data into image data ");
	err = IMT_FromFloatData(output);
	if (err)
	{
		printf("[FAILED], %s\n", IMT_GetErrorString(err));
		goto bye;
	}
	else
		printf("[OK]\n");
		
	printf("Saving output image ");
	err = IMT_Save(argv[2], output, NULL);
	if (err)
	{
		printf("[FAILED], %s\n", IMT_GetErrorString(err));
		goto bye;
	}
	else
		printf("[OK]\n");
		
bye:
	IMT_FreeImage(input);
	IMT_FreeImage(gray);
	IMT_FreeImage(output);
	MAT_FreeArray(kernel);
	MAT_FreeArray(derivatives);
	return 0;
}