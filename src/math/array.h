#ifndef MAT_ARRAY_H
#define MAT_ARRAY_H

/* WARNING: unsafe macro */
#define MAT_SAMEARRAYTYPE(left, right) (((left)->type == (right)->type) && ((left)->width == (right)->width))

enum
{
    MAT_ARRAYTYPE_FLOAT=0,
    MAT_ARRAYTYPE_DOUBLE,
} MAT_ArrayType;

/* MAT_Array is a container object storing zero or more values of same type.
 */
typedef struct MAT_Array
{
    int type;
    unsigned int datatype_size;
    unsigned int width;
    union {
		void   *void_ptr;
		float  *float_ptr;
		double *double_ptr;
    } data;
} MAT_Array;

extern int MAT_SameArrayType(MAT_Array *left, MAT_Array *right);
extern MAT_Array *MAT_AllocArray(unsigned int width, int type, int clear);
extern void MAT_FreeArray(MAT_Array *array);
extern MAT_Array *MAT_DupArray(MAT_Array *src);
extern MAT_Array *MAT_ExtractArrayPlan(MAT_Array *src, unsigned int max, unsigned int index);
extern void MAT_ScalarAddArray(MAT_Array *array, double value);
extern void MAT_ScalarMulArray(MAT_Array *array, double value);
extern void MAT_ScalarDivArray(MAT_Array *array, double value);
extern double MAT_NormL1Array(MAT_Array *array);
extern double MAT_NormL2Array(MAT_Array *array);
extern double MAT_NormalizeL1Array(MAT_Array *array);
extern double MAT_NormalizeL2Array(MAT_Array *array);

#endif /* MAT_ARRAY_H */
