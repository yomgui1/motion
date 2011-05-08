#include "imtool/handler.h"
#include "imtool/error.h"
#include "klt/klt.h"
#include "fast/fast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int readKLTFeaturesList(const char *filename, KLT_FeatureSet *set)
{
#define LINELENGTH 100
    static const char warning_line[] = \
        "!!! Warning:  This is a KLT data file.  "
        "Do not modify below this line !!!\n";

    FILE *fp = fopen(filename, "r");
    char line[LINELENGTH], *s;
    int i, rc=1, n;

    if (NULL == fp)
        return 1;

    while (strcmp(line, warning_line) != 0)
    {
        s = fgets(line, LINELENGTH, fp);
        if (feof(fp))
        {
            fprintf(stderr, "(%s) File is corrupted -- Couldn't find line:\n"
                    "\t%s\n", __FUNCTION__, warning_line);
            goto bye;
        }
    }

    /* Read 'Feature List' */
    while (fgetc(fp) != '-');
    while (fgetc(fp) != '\n');
    s = fgets(line, LINELENGTH, fp);
    if (strcmp(line, "KLT Feature List\n"))
        fprintf(stderr, "(%s) File corrupted or incorrect type -- (No 'KLT Feature List')",
                __FUNCTION__);

    /* Read nFeatures */
    while (fgetc(fp) != '-');
    while (fgetc(fp) != '\n');
    n = fscanf(fp, "%s", line);
    if (strcmp(line, "nFeatures") != 0)
        fprintf(stderr, "(%s) File is corrupted -- "
                "(Expected 'nFeatures', found '%s' instead)", __FUNCTION__, line);
    n = fscanf(fp, "%s", line);
    if (strcmp(line, "=") != 0)
        fprintf(stderr, "(%s) File is corrupted -- "
                "(Expected '=', found '%s' instead)", __FUNCTION__, line);
    n = fscanf(fp, "%d", &set->nfeatures);

    /* Skip junk before data */
    while (fgetc(fp) != '-');
    while (fgetc(fp) != '\n');

    set->features = malloc(sizeof(KLT_Feature) * set->nfeatures);
    if (NULL == set->features)
        goto bye;

    for (i=0; i < set->nfeatures; i++)
    {
        int indx, dummy;
        MAT_Vec2f *pos = &set->features[i].position;

        n = fscanf(fp, "%d |", &indx);
        if (indx != i) fprintf(stderr, "(%s) Bad index at i = %d -- %d", __FUNCTION__, i, indx);
        while (fgetc(fp) != '(');
        n = fscanf(fp, "%f,%f)=%d", &(pos->x), &(pos->y), &dummy);
        set->features[i].status = 0;
    }

    rc = 0;

bye:
    fclose(fp);
    return rc;
}

static int test_TrackFeatureAtLevel(void)
{
    KLT_Context ctx;
    int err, res=-1;
    IMT_Image *image1, *image2;
    int x0=25, y0=25;
    int dx=2, dy=1;
    MAT_Vec2f pos1, pos2;
    float ex, ey;

    KLT_InitContextDefaults(&ctx);

    err = IMT_AllocImage(&image1, IMT_PIXFMT_GRAY8, 51, 51, 0, NULL);
    err |= IMT_AllocImage(&image2, IMT_PIXFMT_GRAY8, 51, 51, 0, NULL);
    if (err) goto bye;

    bzero(image1->data, image1->height * image1->stride);
    bzero(image2->data, image2->height * image2->stride);

    ((unsigned char *)image1->data)[y0*image1->stride + x0] = 255;
    ((unsigned char *)image2->data)[(y0+dy)*image2->stride + x0 + dx] = 255;

    err = IMT_GeneratePyramidalSubImages(image1, 0, ctx.pyramid_sigma);
    err |= IMT_GeneratePyramidalSubImages(image2, 0, ctx.pyramid_sigma);
    if (err) goto bye;

    if ((NULL == image1->subimages[0]) || (NULL == image2->subimages[0]))
        goto bye;

    pos1.x = x0;
    pos1.y = y0;
    pos2.x = x0;
    pos2.y = y0;

    res = KLT_TrackFeatureAtLevel(&ctx,
                                  image1->subimages[0], &pos1,
                                  image2->subimages[0], &pos2);
    printf("[dbg] KLT_TrackFeatureAtLevel() resulted with %d\n", res);
    printf("[dbg] pos1=(%f, %f), pos2=(%f, %f)\n", pos1.x, pos1.y, pos2.x, pos2.y);

    ex = fabsf(pos2.x-(x0+dx));
    ey = fabsf(pos2.y-(y0+dy));
    printf("[dbg] error: (%f, %f)\n", ex, ey);
    if ((ex < 0.001) && (ey < 0.001))
        res = 0;
    else
        res = 1;

bye:
    IMT_FreeImage(image1);
    IMT_FreeImage(image2);

    return res;
}

static int test_TrackFeature(void)
{
    KLT_Context ctx;
    int err, res=-1;
    IMT_Image *image1, *image2;
    int x0=32, y0=64;
    int dx=3, dy=5;
    KLT_Feature f1, f2;
    float ex, ey;

    KLT_InitContextDefaults(&ctx);
    ctx.max_pyramid_level = 2;

    err = IMT_AllocImage(&image1, IMT_PIXFMT_GRAY8, 64, 128, 0, NULL);
    err |= IMT_AllocImage(&image2, IMT_PIXFMT_GRAY8, 64, 128, 0, NULL);
    if (err) goto bye;

    bzero(image1->data, image1->height * image1->stride);
    bzero(image2->data, image2->height * image2->stride);

    ((unsigned char *)image1->data)[y0*image1->stride + x0] = 255;
    ((unsigned char *)image2->data)[(y0+dy)*image2->stride + x0 + dx] = 255;

    f1.position.x = x0;
    f1.position.y = y0;

    res = KLT_TrackFeature(&ctx, image1, &f1, image2, &f2);

    printf("[dbg] KLT_TrackFeature() resulted with %d\n", res);
    printf("[dbg] Feature status: %d\n", f1.status);
    printf("[dbg] pos1=(%f, %f), pos2=(%f, %f)\n",
           f1.position.x, f1.position.y, f2.position.x, f2.position.y);

    ex = fabsf(f2.position.x-(x0+dx));
    ey = fabsf(f2.position.y-(y0+dy));
    printf("[dbg] error: (%f, %f)\n", ex, ey);
    if ((ex < 0.001) && (ey < 0.001))
        res = 0;
    else
        res = 1;

bye:
    IMT_FreeImage(image1);
    IMT_FreeImage(image2);

    return res;
}

static int test_TrackFeatures(
    const char *filename1,
    const char *filename2,
    const char *filename3)
{
    KLT_Context ctx;
    int err, res=-1, num_corners, i, count;
    IMT_Image *image1=NULL, *image2=NULL, *tmp=NULL;
    xy *corners=NULL;
    KLT_FeatureSet ftset = {0};

    KLT_InitContextDefaults(&ctx);

	err = IMT_Load(filename1, &image1, NULL);
	if (err)
	{
		fprintf(stderr, "Failed to open image #1: '%s'\n", IMT_GetErrorString(err));
		goto bye;
	}
	
    err = IMT_Load(filename2, &image2, NULL);
	if (err)
	{
		fprintf(stderr, "Failed to open image #2: '%s'\n", IMT_GetErrorString(err));
		goto bye;
	}

    err = IMT_GenerateGrayscale(image1, 0);
    if (err) goto bye;
    
    err = IMT_GenerateGrayscale(image2, 0);
    if (err) goto bye;

    if (NULL == filename3)
    {
		err = IMT_AllocImageFromFloat(&tmp, IMT_PIXFMT_GRAY8, image1->width, image1->height, image1->grayscale->array.data.float_ptr);
		if (err)
			goto bye;
			
        num_corners = 500;
        corners = fast9_detect_limited(tmp->data, tmp->width, tmp->height, tmp->stride, 0.3, &num_corners, 1);
        IMT_FreeImage(tmp);
        
        if (NULL == corners)
            goto bye;

		printf("%u corners found\n", num_corners);
        ftset.nfeatures = num_corners;
        ftset.features = malloc(sizeof(KLT_Feature)*num_corners);
        
        if (!ftset.features)
			goto bye;
        
        for (i=0; i < num_corners; i++)
        {
            ftset.features[i].status = KLT_NOT_FOUND;
            ftset.features[i].position.x = corners[i].x;
            ftset.features[i].position.y = corners[i].y;
        }
    }
    else
    {
        err = readKLTFeaturesList(filename3, &ftset);
        if (err)
            goto bye;
    }
        
    res = KLT_TrackFeatures(&ctx, image1, image2, &ftset);
    printf("[DBG] KLT_TrackFeatures() resulted with %d\n", res);

	count = 0;
    for (i=0; i < ftset.nfeatures; i++)
    {
		if (ftset.features[i].status == KLT_TRACKED)
			count++;
    }
    
	printf("%u remains tracked\n", count);
	
	err = IMT_AllocImageFromFloat(&tmp, IMT_PIXFMT_GRAY8, image1->width, image1->height, image1->grayscale->array.data.float_ptr);
	if (!err)
	{
		for (i=0; i < ftset.nfeatures; i++)
		{
			if (ftset.features[i].status == KLT_TRACKED)
			{
				int x = ftset.features[i].position.x;
				int y = ftset.features[i].position.y;
				
				((unsigned char *)(tmp->data))[y*tmp->stride + x] = 255;
			}
		}
			
		IMT_Save("gray1.png", tmp, NULL);
		IMT_FreeImage(tmp);
	}
	
	err = IMT_AllocImageFromFloat(&tmp, IMT_PIXFMT_GRAY8, image2->width, image2->height, image2->grayscale->array.data.float_ptr);
	if (!err)
	{
		for (i=0; i < ftset.nfeatures; i++)
		{
			if (ftset.features[i].status == KLT_TRACKED)
			{
				int x = ftset.features[i].estimation.x;
				int y = ftset.features[i].estimation.y;
				
				((unsigned char *)(tmp->data))[y*tmp->stride + x] = 255;
			}
		}
		
		IMT_Save("gray2.png", tmp, NULL);
		IMT_FreeImage(tmp);
	}

    res = 0;

bye:
	free(corners);
    free(ftset.features);
    IMT_FreeImage(image1);
    IMT_FreeImage(image2);

    return res;
}

int main(int argc, char **argv)
{
    int res;
    const char *ftfile;

    IMT_Init();

    fprintf(stderr, "\n>>>> Testing TrackFeatureAtLevel()...\n");
    res = test_TrackFeatureAtLevel();
    fprintf(stderr, "\n>>>> TrackFeatureAtLevel() ");
    if (res < 0)
        fprintf(stderr, "ERROR: internal failures\n");
    else if (res > 0)
        fprintf(stderr, "FAILED\n");
    else
        fprintf(stderr, "OK\n");

    fprintf(stderr, "\n>>>> Testing TrackFeature()...\n");
    res = test_TrackFeature();
    fprintf(stderr, "\n>>>> TrackFeature() ");
    if (res < 0)
        fprintf(stderr, "ERROR: internal failures\n");
    else if (res > 0)
        fprintf(stderr, "FAILED\n");
    else
        fprintf(stderr, "OK\n");

    if (argc < 3)
	{
		fprintf(stderr, "\nNo enough parameters to continue.\nUSAGE: %s <input_image1> <input_image2> [featureslist.fl]\n", argv[0]);
		return 0;
	}

    if (argc == 4)
        ftfile = argv[3];
    else
        ftfile = NULL;

    fprintf(stderr, "\n>>>> Testing TrackFeatures()...\n");
    res = test_TrackFeatures(argv[1], argv[2], ftfile);
    fprintf(stderr, "\n>>>> TrackFeatures() ");
    if (res < 0)
        fprintf(stderr, "ERROR: internal failures\n");
    else if (res > 0)
        fprintf(stderr, "FAILED\n");
    else
        fprintf(stderr, "OK\n");

    return 0;
}
