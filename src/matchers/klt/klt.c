#include "klt/context.h"
#include "math/array.h"
#include "math/vector.h"

#include "klt/context.h"

/* Code adapted from original KLT lib sources */

float interpolate(const MAT_Matrix *matrix, int index, float x, float y)
{
    int left = (int)x;
    int top = (int)y;
    float dx = x-left;
    float dy = y-top;
    float *pixel = matrix->array.data.float_ptr[top*matrix->width + left + index];
    
    return (0);
}

/* 
** This function is the sum of following libklt functions,
** but done in the same window loop:
**
**  _computeGradientSum
**  _compute2by2GradientMatrix
**  _compute2by1ErrorVector
*/

static void computeGradientAndErrorMatrix(
    const KLT_Context *ctx,
    const MAT_Matrix *level1,       /* first image and gradients */
    const MAT_Matrix *level2,       /* second image and gradients */
    const MAT_Vec2f *pos1,          /* window center in first image */
    const MAT_Vec2f *pos2,          /* window center in second image */
    float *gxx,
    float *gxy, 
    float *gyy,
    float *ex,
    float *ey)
{
    const float cx1 = pos1->x;
    const float cy1 = pos1->y;
    const float cx2 = pos2->x;
    const float cy2 = pos2->y;
    const int hw = ctx->win_width/2, hh = ctx->win_height/2;
    int i, j;

    /* Compute values */
    *gxx = 0.0;  *gxy = 0.0;  *gyy = 0.0;

    for (j = -hh ; j <= hh ; j++)
    {
        for (i = -hw ; i <= hw; i++)
        {
            float x1 = cx1 + i;
            float y1 = cy1 + j;
            float x2 = cx2 + i;
            float y2 = cy2 + j;
            float p1, p2, diff, gx, gy;
            
            /* Get pixels value in both images (interpolate at subpixels) */
            p1 = interpolate(level1, 0, x1, y1);
            p2 = interpolate(level2, 0, x2, y2);
            diff = p1 - p2;

            /* Do the same for gradients in the second image */
            gx = interpolate(level2, 1, x2, y2);
            gy = interpolate(level2, 2, x2, y2);

            *gxx += gx * gx;
            *gxy += gx * gy;
            *gyy += gy * gy;

            *ex += diff * gx;
            *ey += diff * gy;
        }
    }

#if 0
    /* XXX: by default KLT fix that to 1.0, so why bother... */
    *ex *= step_factor;
    *ey *= step_factor;
#endif
}

int KLT_TrackFeatureInLevel(
    const KLT_Context *ctx,
    const MAT_Matrix *level1,
    MAT_Vec2f *pos1,
    const MAT_Matrix *level2,
    MAT_Vec2f *pos2)
{
    int i;
    
    for (i=0; i < ctx->max_iterations; i++)
    {
        float gxsx, gxy, gyy, ex, ey;

        computeGradientAndErrorMatrix(ctx, level1, level2,
                                      pos1, pos2,
                                      &gxsx, &gxy, &gyy, &ex, &ey);
    }

    return 0;
}
