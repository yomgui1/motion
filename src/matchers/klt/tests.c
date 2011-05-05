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
    ctx.pyramid_sigma = 0.9;

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

#if 0
    int i, j;
    MAT_Matrix *mat = image1->subimages[0];
    for (i = 0; i < image1->height; ++i)  {
        printf("%03u [ ", i);
        for (j = 0; j < image1->width; ++j)  {
            printf("%02x", (int)(mat->array.data.float_ptr[i*mat->ncols+j*3+2]*128.)+128);
        }
        printf(" ]\n");
    }
#endif

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
    if ((ex < 0.01) && (ey < 0.01))
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
    IMT_Image *image1, *image2;
    IMT_Image *gray1, *gray2;
    xy *corners=NULL;
    KLT_FeatureSet ft;

    KLT_InitContextDefaults(&ctx);
    ctx.win_halfwidth = 3;
    ctx.win_halfheight = 3;
    ctx.max_pyramid_level = 4;

	err = IMT_Load(filename1, &image1, NULL);
    err |= IMT_Load(filename2, &image2, NULL);
    if (err) goto bye;

    err = IMT_Grayscale(image1, &gray1);
    err |= IMT_Grayscale(image2, &gray2);
    if (err) goto bye;

    if (NULL == filename3)
    {
        num_corners = 1000;
        corners = fast9_detect_limited(gray1->data, gray1->width, gray1->height, gray1->stride, 10, &num_corners, 1);
        printf("%u corners found\n", num_corners);

        if ((NULL == corners) || !corners)
            goto bye;

        ft.nfeatures = num_corners;
        ft.features = malloc(sizeof(KLT_Feature)*num_corners);
        for (i=0; i < num_corners; i++)
        {
            ft.features[i].status = 0;
            ft.features[i].position.x = corners[i].x;
            ft.features[i].position.y = corners[i].y;
            
            ((unsigned char *)gray1->data)[corners[i].y*gray1->width + corners[i].x] = 255;
        }
    }
    else
    {
        err = readKLTFeaturesList(filename3, &ft);
        if (err)
            goto bye;

        for (i=0; i < ft.nfeatures; i++)
        {
            MAT_Vec2f *pos = &ft.features[i].position;

            ((unsigned char *)gray1->data)[(int)pos->y * gray1->width + (int)pos->x] = 255;
        }
    }
        
    IMT_Save("gray1.png", gray1, NULL);

    res = KLT_TrackFeatures(&ctx, &ft, gray1, gray2);
    printf("[DBG] KLT_TrackFeatures() resulted with %d\n", res);

	count = 0;
    for (i=0; i < ft.nfeatures; i++)
    {
		if (ft.features[i].status == KLT_TRACKED)
		{
			((unsigned char *)gray2->data)[(int)ft.features[i].position.y*gray2->width + (int)ft.features[i].position.x] = 255;
			count++;
		}
    }
    
	printf("%u remains tracked\n" ,count);

    IMT_Save("gray2.png", gray2, NULL);

    res = 0;

bye:
    IMT_FreeImage(image1);
    IMT_FreeImage(image2);
    IMT_FreeImage(gray1);
    IMT_FreeImage(gray2);
    free(corners);
    free(ft.features);

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

#if 0
    if (argc < 3)
	{
		fprintf(stderr, "No enough parameters\nUSAGE: %s <input_image1> <input_image2> [featureslist.fl]\n", argv[0]);
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
#endif

    return 0;
}
