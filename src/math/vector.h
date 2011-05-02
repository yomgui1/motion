#ifndef MAT_VECTOR_H
#define MAT_VECTOR_H

enum
{
    MAT_VECTYPE_FLOAT=0,
    MAT_VECTYPE_DOUBLE,
} MAT_VecType;

typedef struct MAT_Vec
{
    int type;
    int dims_num;
    int *dims;
    unsigned data_size;
    void *data;
} MAT_Vec;

extern MAT_Vec *MAT_VecNew(int type, int dims_num, int *dims);
extern void MAT_VecFree(MAT_Vec *vec);
extern MAT_Vec *MAT_VecDup(MAT_Vec *src);
extern float MAT_VecNormL1(MAT_Vec *vec);
extern float MAT_VecNormL2(MAT_Vec *vec);
extern float MAT_VecNormalizeL1(MAT_Vec *vec);
extern float MAT_VecNormalizeL2(MAT_Vec *vec);

#endif /* MAT_VECTOR_H */
