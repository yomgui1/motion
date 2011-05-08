#ifndef KLT_H
#define KLT_H

#include <math/vector.h>
#include <imtool/image.h>

#define KLT_TRACKED           0
#define KLT_NOT_FOUND        -1
#define KLT_SMALL_DET        -2
#define KLT_MAX_ITERATIONS   -3
#define KLT_OOB              -4
#define KLT_LARGE_RESIDUE    -5

typedef struct KLT_Feature
{
    MAT_Vec2f position;
    MAT_Vec2f estimation;
    int status;
    float trackness;
} KLT_Feature;

typedef struct KLT_FeatureSet
{
    int nfeatures;
    KLT_Feature *features;
} KLT_FeatureSet;

typedef struct KLT_Context
{
    int max_iterations;
    int win_width, win_height;
    int win_halfwidth, win_halfheight;
    int max_pyramid_level;
    float pyramid_sigma;
    float min_determinant;
    float min_displacement;

    /* dynamic parameters, don't touch */
    int nfeatures;
    KLT_Feature *features;
    double min_trackness;
} KLT_Context;

extern void KLT_InitContextDefaults(KLT_Context *ctx);
extern int KLT_TrackFeatureAtLevel(
    const KLT_Context *ctx,
    const MAT_Matrix *level1,
    MAT_Vec2f *pos1,
    const MAT_Matrix *level2,
    MAT_Vec2f *pos2);
extern void KLT_TrackFeaturesAtLevel(
    const KLT_Context *ctx,
    int level,
    const MAT_Matrix *level1,
    const MAT_Matrix *level2);
extern int KLT_TrackFeatures(
    KLT_Context *ctx,
    IMT_Image *image1,
    IMT_Image *image2,
	KLT_FeatureSet *ftset);
extern int KLT_TrackFeature(
    KLT_Context * ctx,
    IMT_Image *   image1,
    KLT_Feature * feature1,
    IMT_Image *   image2,
    KLT_Feature * feature2);
extern int KLT_DetectGoodFeatures(KLT_Context *ctx, MAT_Matrix *image, KLT_FeatureSet *fs);

#endif /* KLT_H */
