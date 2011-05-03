#include "klt/context.h"
#include "math/array.h"
#include "math/vector.h"
#include "math/matrix.h"

#include "klt/context.h"

#include <math.h>

#define CLAMP(a, v, b) (a<v?(v<b?v:b-1):a)

#define KLT_TRACKED           0
#define KLT_NOT_FOUND        -1
#define KLT_SMALL_DET        -2
#define KLT_MAX_ITERATIONS   -3
#define KLT_OOB              -4
#define KLT_LARGE_RESIDUE    -5

#define SMALL_DET (1e-6)
#define DISP_CONV_THRESHOLD (1e-6)

/* This is a Join-LKT implementation as explained in paper
** "Joint Tracking of Features and Edges"
** by Stanley T. Birchfield and Shrinivas J. Pundlik.
*/
static float interpolate(const MAT_Matrix *matrix, int index, float x, float y)
{
    int left = (int)x;
    int top = (int)y;
    float ax = x-left;
    float ay = y-top;
    float *ptr = &matrix->array.data.float_ptr[top*matrix->ncols + left + index];
    
    return ((1-ax) * (1-ay) * *ptr +
			   ax  * (1-ay) * *(ptr+1) +
			(1-ax) *   ay   * *(ptr+(matrix->ncols)) +
			   ax  *   ay   * *(ptr+(matrix->ncols)+1));
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
    const float cx1 = pos1->x;
    const float cy1 = pos1->y;
    const float cx2 = pos2->x;
    const float cy2 = pos2->y;
    const int hw = ctx->win_halfwidth, hh = ctx->win_halfheight;
    int i, j;

    *Jxx = 0.0; *Jxy = 0.0; *Jyy = 0.0;
    *Jxt = 0.0; *Jyt = 0.0;

    for (j=-hh ; j <= hh ; j++)
    {
        for (i =-hw ; i <= hw; i++)
        {
            float x1 = CLAMP(0.0, cx1 + i, ctx->win_width);
            float y1 = CLAMP(0.0, cy1 + j, ctx->win_height);
            float x2 = CLAMP(0.0, cx2 + i, ctx->win_width);
            float y2 = CLAMP(0.0, cy2 + j, ctx->win_height);
            float p1, p2, It, Ix, Iy;
            
            /* Get pixels value in both images (interpolate at subpixels) */
            p1 = interpolate(level1, 0, x1, y1);
            p2 = interpolate(level2, 0, x2, y2);
            It = p1 - p2;

            /* Do the same for gradients in the second image */
            Ix = interpolate(level2, 1, x2, y2);
            Iy = interpolate(level2, 2, x2, y2);

            *Jxx += Ix * Ix;
            *Jxy += Ix * Iy;
            *Jyy += Iy * Iy;

            *Jxt += Ix * It;
            *Jyt += Iy * It;
        }
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
	float Jxx, float Jxy, float Jyy,
    float Jxt, float Jyt,
    float *ux, float *uy)
{
	float det = Jxx*Jyy - Jxy*Jxy;
    
	if (det < SMALL_DET)
		return KLT_SMALL_DET;

	*ux = (Jyy*Jxt - Jxy*Jyt) / det;
	*uy = (Jxx*Jyt - Jxy*Jxt) / det;
	
	return KLT_TRACKED;
}


static int trackFeatureAtLevel(
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

        computeZieiCoeff(ctx, level1, level2,
						 pos1, pos2,
						 &Jxx, &Jxy, &Jyy, 
						 &Jxt, &Jyt);
						 
		res = solveTrackingEquation(Jxx, Jxy, Jyy, Jxt, Jyt, &uxi, &uyi);
		if (res != KLT_TRACKED)
			return res;
			
		/* Update ui (feature position in the second image) */
		pos2->x += uxi;
		pos2->y += uyi;
		
		/* Feature goes out of image? */
		if ((pos2->x < 0.0) || (pos2->x >= level1->ncols) ||
			(pos2->y < 0.0) || (pos2->y >= level1->nrows))
			return KLT_OOB;
			
		/* Stop on convergence */
		/* TODO: what's fastest? 2 abs + 3 conditionals or 2 mul + 1 add + 1 conditional ? */
		if ((fabsf(uxi) < DISP_CONV_THRESHOLD) || (fabsf(uyi) < DISP_CONV_THRESHOLD))
			break;
    }
    
    if (i == ctx->max_iterations)
		return KLT_MAX_ITERATIONS;

    return KLT_TRACKED;
}

static void trackFeaturesAtLevel(
    const KLT_Context *ctx,
    int level;
    const MAT_Matrix *level1,
    const MAT_Matrix *level2)
{
	float level_div = 1 << level;
	
	for (i=0; i < ctx->nfeatures; i++)
	{
		int res;
		MAT_Vec2f pos1;
		
		pos1.x = ctx->features[i].position.x / level_div;
		pos1.y = ctx->features[i].position.y / level_div;
		
		res = trackFeatureAtLevel(ctx, level, level1, &pos1, level2, &ctx->estimated_features[i].position);
		ctx->features[i].state = res;
		
		/* XXX: Original libklt verify if the intensity over the searching window doesn't change
		 * more than a constante value. Should I do the same?
		 */
		 
		/* ready for the next level */
		if (level > 0)
		{
			ctx->estimated_features[i].position.x *= 2;
			ctx->estimated_features[i].position.y *= 2;
		}
	}
}

int KLT_TrackFeatures(const KLT_Context *ctx, KLT_FeatureSet *features_set, IMT_Image *image1, IMT_Image *image2)
{
	int level;
	float max_level_div = 1 << ctx->pyramid_levels;
	KLT_Feature *estimated_features;
	
	/* TODO: alloc estimated features array */
	
	/* Prepare estimated features position into second image */
	for (i=0; i < features_set->count; i++)
	{
		features_set->features[i].position.x = estimated_features[i].position.x / max_level_div;
		features_set->features[i].position.y = estimated_features[i].position.y / max_level_div;
	}
	
	ctx->features = features_set->features;
	ctx->estimated_features = estimated_features;
	ctx->nfeatures = features_set->count;
	
	for (levels=ctx->pyramid_levels; level >= 0; level++)
		trackFeaturesAtLevel(ctx, level, image1->subimages[level], image2->subimages[level]);
		
	/* TODO: and now? what to do of features estimated positions? */
}