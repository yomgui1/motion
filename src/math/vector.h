#ifndef MAT_VECTOR_H
#define MAT_VECTOR_H

#include <math/array.h>

/* Vectors are implemented as Arrays */
typedef MAT_Array MAT_Vector;

typedef struct MAT_Vec2f { float x, y; } MAT_Vec2f;
typedef struct MAT_Vec3f { float x, y, z; } MAT_Vec3f;
typedef struct MAT_Vec4f { float x, y, z, w; } MAT_Vec4f;

#endif /* MAT_VECTOR_H */
