#include "klt/klt.h"
#include "math/matrix.h"

#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define CLAMP(a, v, b) (a<v?(v<b?v:b-1):a)

/* This is a normal LKT implementation.
** TODO: need to implement a joint-KLT as explained in paper
** "Joint Tracking of Features and Edges"
** by Stanley T. Birchfield and Shrinivas J. Pundlik.
*/
static float interpolate(const MAT_Matrix *matrix, int index, float x, float y)
{
    int left = (int)x;
    int top = (int)y;
    float ax = x-left;
    float ay = y-top;
    float *ptr = &matrix->array.data.float_ptr[top*matrix->ncols + 3*left + index];
    
    if (!((*ptr <= 1.0f) && (*(ptr+3) <= 1.0) && (*(ptr+(matrix->ncols)) <= 1.0) && (*(ptr+(matrix->ncols)+3) <= 1.0)))
    {
		printf("FATAL error in %s: invalid floating data from matrix @ %p\n", __FUNCTION__, matrix);
		printf("Point is (%f, %f), index=%u\n", x, y, index);
		if (matrix)
		{			
			printf("Matrix info: ncols=%u, nrows=%u, array width=%u [%s]\n",
				   matrix->ncols, matrix->nrows, matrix->array.width,
				   matrix->ncols*matrix->nrows==matrix->array.width?"OK":"NOK");
			if (matrix->array.width == matrix->ncols*matrix->nrows)
			{
				unsigned int off;
				
				float *start = matrix->array.data.float_ptr;
				float *end = matrix->array.data.float_ptr + matrix->array.width;
				
				off = top*matrix->ncols + 3*left + index;
				printf("Point offset: +%u => %p\n", off, start + off);
				printf("Matrix data: [%p, %p[\n", start, end);
			}
			else
				printf("Matrix data: from %p, ending is not not well known!\n", matrix->array.data.void_ptr);
		}
		abort();
    }
    
    return ((1-ax) * (1-ay) * *ptr +
			   ax  * (1-ay) * *(ptr+3) +
			(1-ax) *   ay   * *(ptr+(matrix->ncols)) +
			   ax  *   ay   * *(ptr+(matrix->ncols)+3));
}


/*  This function is the sum of following libklt functions,
** but done in the same convolution loop:
**
**  _computeGradientSum
**  _compute2by2GradientMatrix
**  _compute2by1ErrorVector
**
** This procedure computes following matrix:
**
**  Zi = [ Jxx Jxy ] and ei = [ Jxt ]
**       [ Jxy Jyy ]          [ Jyt ]
**
** Zi is the Lucas-Kanade gradiant covariance matrix.
** ei is the error vector.
** Both are computed for the feature i.
**
** See the paper previously cited on the top of this file.
** (Note Lambda terms in the paper are set to zero here)
*/
static void computeZieiCoeff(
    const KLT_Context *ctx,
    const MAT_Matrix *level1,       /* first image and gradients */
    const MAT_Matrix *level2,       /* second image and gradients */
    const MAT_Vec2f *pos1,          /* window center in first image */
    const MAT_Vec2f *pos2,          /* window center in second image */
    
    float *Jxx, /* results */
    float *Jxy, 
    float *Jyy,
    float *Jxt,
    float *Jyt)
{
    float cx1 = pos1->x;
    float cy1 = pos1->y;
    float cx2 = pos2->x;
    float cy2 = pos2->y;
    int hw = ctx->win_halfwidth, hh = ctx->win_halfheight;
    int i, j;

    *Jxx = 0.0f; *Jxy = 0.0f; *Jyy = 0.0f;
    *Jxt = 0.0f; *Jyt = 0.0f;

	//printf("Zi for (%g, %g) -> (%g, %g)\n", cx1, cy1, cx2, cy2);
    for (j=-hh ; j <= hh ; j++)
    {
        //printf("[");
        for (i=-hw ; i <= hw; i++)
        {
            float x1 = CLAMP(0.0f, cx1+i, level1->ncols/3-1);
            float y1 = CLAMP(0.0f, cy1+j, level1->nrows-1);
            float x2 = CLAMP(0.0f, cx2+i, level2->ncols/3-1);
            float y2 = CLAMP(0.0f, cy2+j, level2->nrows-1);
            float p1, p2, It, Ix, Iy;

			//dprintf("x1=%f, y1=%f, x2=%f, y2=%f\n", x1,y1,x2,y2);

            /* Get pixels value in both images (interpolate at subpixels) */
            p1 = interpolate(level1, 0, x1, y1);
            p2 = interpolate(level2, 0, x2, y2);
            It = p1 - p2;

            /* Do the same to sum gradients in both images */
            //Ix  = interpolate(level1, 1, x1, y1);
            Ix = interpolate(level2, 1, x2, y2);
            //Iy  = interpolate(level1, 2, x1, y1);
            Iy = interpolate(level2, 2, x2, y2);
            
            //printf(" %g", p1*255.);

            *Jxx += Ix * Ix;
            *Jxy += Ix * Iy;
            *Jyy += Iy * Iy;

            *Jxt += Ix * It;
            *Jyt += Iy * It;
        }
        //printf(" ]\n");
    }
}

/* This function is just the simple solving of the linear system:
**
** [Jxx Jxy] [ux] = [Jxt]
** [Jxy Jyy] [uy] = [Jyt]
** 
** where [ux uy]T is the possible feature displacement to test.
*/
static int solveTrackingEquation(
    float small_det,
	float Jxx, float Jxy, float Jyy,
    float Jxt, float Jyt,
    float *ux, float *uy)
{
	float det = Jxx*Jyy - Jxy*Jxy;
    
    //printf("J=[%g %g %g], det=%g\n", Jxx, Jxy, Jyy, det);
	if (det < small_det)
		return KLT_SMALL_DET;

	*ux = (Jyy*Jxt - Jxy*Jyt) / det;
	*uy = (Jxx*Jyt - Jxy*Jxt) / det;
	
	return KLT_TRACKED;
}

int KLT_TrackFeatureAtLevel(
    const KLT_Context *ctx,
    const MAT_Matrix *level1,
    MAT_Vec2f *pos1,
    const MAT_Matrix *level2,
    MAT_Vec2f *pos2)
{
    int i;
	
	/* Evaluate ui, the feature displacement estimate,
	 * using an iterative method updating ui by adding ûi at each iteration.
	 * ûi is the result of the linear system Zi.ûi = ei
	 */
    
    for (i=0; i < ctx->max_iterations; i++)
    {
		int res;
        float Jxx, Jxy, Jyy; /* Zi coeffs */
        float Jxt, Jyt;      /* ei coeffs */
        float uxi, uyi;

        //printf(">> iter[%u]\n", i);
        computeZieiCoeff(ctx, level1, level2,
						 pos1, pos2,
						 &Jxx, &Jxy, &Jyy, 
						 &Jxt, &Jyt);
						 
        //printf("[DBG] Zi=[%g, %g, %g], det=%f\n", Jxx, Jxy, Jyy, Jxx*Jyy-Jxy*Jxy);
        //printf("[DBG] ei=[%g, %g]\n", Jxt, Jyt);

		res = solveTrackingEquation(ctx->min_determinant, Jxx, Jxy, Jyy, Jxt, Jyt, &uxi, &uyi);
		if (res != KLT_TRACKED)
			return res;

		/* Update ui (feature position in the second image) */
		pos2->x += uxi;
		pos2->y += uyi;

        //printf("[DBG] ui=[%f, %f]\n", pos2->x, pos2->y);
		
		/* Feature goes out of image? */
		if ((pos2->x < 0.0f) || (pos2->x >= level1->ncols) ||
			(pos2->y < 0.0f) || (pos2->y >= level1->nrows))
			return KLT_OOB;
			
		/* Stop on convergence */
		if ((fabsf(uxi) < ctx->min_displacement) || (fabsf(uyi) < ctx->min_displacement))
        {
            printf("[DBG] Tracked in %u iteration(s)\n", i);
			return KLT_TRACKED;
        }
    }

    return KLT_MAX_ITERATIONS;
}

void KLT_TrackFeaturesAtLevel(
    const KLT_Context *ctx,
    int level,
    const MAT_Matrix *level1,
    const MAT_Matrix *level2)
{
    int i;
	float level_div = 1 << level;

	for (i=0; i < ctx->nfeatures; i++)
	{
		int res;
		MAT_Vec2f pos1;
		
		pos1.x = ctx->features[i].position.x / level_div;
		pos1.y = ctx->features[i].position.y / level_div;

		res = KLT_TrackFeatureAtLevel(ctx, level1, &pos1, level2, &ctx->estimated_features[i].position);
		//printf("[dbg] L%u, F%u: pos1=(%f, %f), pos2=(%f, %f)\n",
		//	   level, i, pos1.x, pos1.y, ctx->estimated_features[i].position.x, ctx->estimated_features[i].position.y);
        ctx->features[i].status = res;

        /* XXX: Original libklt verify if the intensity over the searching window doesn't change
		 * more than a constante value. Should I do the same?
		 */

        if (res == KLT_TRACKED)
        {
            /* get ready for the next level */
            if (level > 0)
            {
                ctx->estimated_features[i].position.x *= 2;
                ctx->estimated_features[i].position.y *= 2;
            }
        }
        else if (level > 0)
        {
            /* Reset to the image1 position for next level */
            ctx->estimated_features[i].position.x = pos1.x*2;
            ctx->estimated_features[i].position.y = pos1.y*2;
        }
	}
}

/* Only used for debug */
int KLT_TrackFeature(
    KLT_Context * ctx,
    IMT_Image *   image1,
    KLT_Feature * feature1,
    IMT_Image *   image2,
    KLT_Feature * feature2)
{
	int level, max_level;
	float max_level_div, sigma;

    sigma = ctx->pyramid_sigma;

    /* Prepare images */
    if(IMT_GeneratePyramidalSubImages(image1, ctx->max_pyramid_level, sigma) < 0)
        return 1;
    if(IMT_GeneratePyramidalSubImages(image2, ctx->max_pyramid_level, sigma) < 0)
        return 1;

    if (image1->levels <= image2->levels)
        max_level = image1->levels;
    else
        max_level = image1->levels;
    max_level_div = 1 << max_level;
	
	/* Prepare estimated features position into second image */
    feature2->position.x = feature1->position.x / max_level_div;
    feature2->position.y = feature1->position.y / max_level_div;
	
    //printf("[DBG] Tracking feature at (%f, %f)\n",
    //       feature1->position.x, feature1->position.y);
	for (level=max_level; level >= 0; level--)
    {
        int res;
        MAT_Vec2f pos1;
        float level_div = 1 << level;

        pos1.x = feature1->position.x / level_div;
		pos1.y = feature1->position.y / level_div;

        //printf("[DBG] Level %u: tracking at (%f, %f)...\n",
        //       level, feature2->position.x*level_div, feature2->position.y*level_div);
		res = KLT_TrackFeatureAtLevel(ctx,
                                      image1->subimages[level],
                                      &pos1,
                                      image2->subimages[level],
                                      &feature2->position);
        feature1->status = res;
        printf("[DBG] Level %u: (%f, %f) -> (%f, %f), status %d\n",
               level,
               pos1.x * level_div,
               pos1.y * level_div,
               feature2->position.x * level_div,
               feature2->position.y * level_div,
               res);

        if (res == KLT_TRACKED)
        {
            /* get ready for the next level */
            if (level > 0)
            {
                feature2->position.x *= 2;
                feature2->position.y *= 2;
            }
        }
        else if (level > 0)
        {
            /* Reset to the image1 position for next level */
            feature2->position.x = pos1.x*2;
            feature2->position.y = pos1.y*2;
        }
    }

    return 0;
}

int KLT_TrackFeatures(
    KLT_Context *ctx,
    KLT_FeatureSet *features_set,
    IMT_Image *image1,
    IMT_Image *image2)
{
	int i, level, max_level;
	float max_level_div;
	KLT_Feature *estimated_features;
	
	ctx->win_width = 2*ctx->win_halfwidth + 1;
	ctx->win_height = 2*ctx->win_halfheight + 1;
	
    /* Prepare images */
    if(IMT_GeneratePyramidalSubImages(image1, ctx->max_pyramid_level, ctx->pyramid_sigma) < 0)
        return 1;
    if(IMT_GeneratePyramidalSubImages(image2, ctx->max_pyramid_level, ctx->pyramid_sigma) < 0)
        return 1;

    if (image1->levels <= image2->levels)
        max_level = image1->levels;
    else
        max_level = image1->levels;
    max_level_div = 1 << max_level;

	estimated_features = malloc(features_set->nfeatures * sizeof(KLT_Feature));
    if (NULL == estimated_features) return 1;
	
	/* Prepare estimated features position into second image */
	for (i=0; i < features_set->nfeatures; i++)
	{
        estimated_features[i].status = KLT_TRACKED;
		estimated_features[i].position.x = features_set->features[i].position.x / max_level_div;
		estimated_features[i].position.y = features_set->features[i].position.y / max_level_div;
	}
	
    ctx->nfeatures = features_set->nfeatures;
	ctx->features = features_set->features;
	ctx->estimated_features = estimated_features;
	
	for (level=max_level; level >= 0; level--)
		KLT_TrackFeaturesAtLevel(ctx, level, image1->subimages[level], image2->subimages[level]);
		
	/* TODO: and now? what to do of features estimated positions? */
	for (i=0; i < features_set->nfeatures; i++)
	{
		features_set->features[i].position.x = estimated_features[i].position.x;
		features_set->features[i].position.y = estimated_features[i].position.y;
	}

    return 0;
}

void KLT_InitContextDefaults(KLT_Context *ctx)
{
    ctx->max_iterations = 10;
    ctx->win_halfwidth = 3;
    ctx->win_halfheight = 3;
    ctx->pyramid_sigma = .9f;
    ctx->max_pyramid_level = 4;
    ctx->min_determinant = 1e-6;
    ctx->min_displacement = 1e-4;
}
