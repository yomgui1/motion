/******************************************************************************
Copyright (c) 2009-2011 Guillaume Roguez

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

/* THIS IS A PYTHON2 AND PYTHON3 COMPATIBLE MODULE */

#include <Python.h>
#include <structmember.h>
#include <stdint.h>

#include <imtool/image.h>
#include <imtool/handler.h>
#include <imtool/error.h>

#include <klt/klt.h>
#include <fast/fast.h>


#if PY_MAJOR_VERSION < 3
#define INITFUNC initmotion
#if PY_MINOR_VERSION < 6
#define Py_TYPE(x) ((x)->ob_type)
#endif
#else
#define INITFUNC PyInit_motion
#endif

#ifndef MODNAME
#define MODNAME "motion"
#endif

#ifdef NDEBUG
#define D(x)
#else
#define D(x) x
#endif

#define INSI(m, s, v) if (PyModule_AddIntConstant(m, s, v)) return -1
#define INSL(m, s, v) if (PyModule_AddObject(m, s, PyLong_FromUnsignedLong(v))) return -1
#define INSS(m, s, v) if (PyModule_AddStringConstant(m, s, v)) return -1

#define ADD_TYPE(m, s, t) {Py_INCREF(t); PyModule_AddObject(m, s, (PyObject *)(t));}

#define PyArray_Check(op) PyObject_TypeCheck(op, &PyArrayType)
#define PyArray_CheckExact(op) ((op)->ob_type == &PyArrayType)

#define PyMatrix_Check(op) PyObject_TypeCheck(op, &PyMatrixType)
#define PyMatrix_CheckExact(op) ((op)->ob_type == &PyMatrixType)

#define OBJ_TNAME(o) (Py_TYPE(o)->tp_name)

#ifdef __ONLY_A_TEMPLATE__
#define PyXXX_Check(op) PyObject_TypeCheck(op, &PyXXX_Type)
#define PyXXX_CheckExact(op) ((op)->ob_type == &PyXXX_Type)

typedef struct PyXXX_STRUCT {
    PyObject_HEAD

    PyObject *dummy;
} PyXXX;

static PyTypeObject PyXXX_Type;

static PyObject *
xxx_new(PyTypeObject *type, PyObject *args)
{
    PyXXX *self;
    PyObject *obj;

    if (!PyArg_ParseTuple(args, "O:XXX", &obj)) /* BR */
        return NULL;

    self = (PyXXX*)type->tp_alloc(type, 0); /* NR */
    if (NULL != self)
    {
        self->dummy = obj;
    }

    Py_CLEAR(self);
    return NULL;
}

static int
xxx_traverse(PyXXX *self, visitproc visit, void *arg)
{
    Py_VISIT(self->dummy);
    return 0;
}

static int
xxx_clear(PyXXX *self)
{
    Py_CLEAR(self->dummy);
    return 0;
}

static void
xxx_dealloc(PyXXX *self)
{
    xxx_clear(self);
    self->ob_type->tp_free((PyObject *)self);
}

static PyObject *
xxx_get_yyy(PyXXX *self, PyObject *args)
{
    Py_RETURN_NONE;
}

static PyObject *
xxx_get_zzz(PyXXX *self, void* closure)
{
    Py_RETURN_NONE;
}

static int
xxx_set_zzz(PyXXX *self, void* closure)
{
    return 0;
}

static struct PyMethodDef xxx_methods[] = {
    {"get_yyy", (PyCFunction)xxx_get_yyy, METH_VARARGS, NULL},
    {NULL} /* sentinel */
};

static PyMemberDef xxx_members[] = {
    {"dummy", T_OBJECT, offsetof(PyXXX, dummy), 0, NULL},
    {NULL} /* sentinel */
};

static PyGetSetDef xxx_getseters[] = {
    {"zzz", (getter)xxx_get_zzz, (setter)xxx_set_zzz, "zzz", (void *)0},
    {NULL} /* sentinel */
};

static PyTypeObject PyXXX_Type = {
    PyObject_HEAD_INIT(NULL)

    tp_name         : MODNAME ".XXX",
    tp_basicsize    : sizeof(PyXXX),
    tp_flags        : Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC |  Py_TPFLAGS_BASETYPE,
    tp_doc          : "XXX Objects",

    tp_new          : (newfunc)xxx_new,
    tp_dealloc      : (destructor)xxx_dealloc,
    tp_traverse     : (traverseproc)xxx_traverse,
    tp_clear        : (inquiry)xxx_clear,
    tp_methods      : xxx_methods,
    tp_members      : xxx_members,
    tp_getset       : xxx_getseters,
};
#endif

/*******************************************************************************************
** PyImageType
*/

typedef struct PyImage_STRUCT
{
    PyObject_HEAD

    IMT_Image *image;
} PyImage;

static PyTypeObject PyImageType;

static PyObject *
image_new(PyTypeObject *type, PyObject *args)
{
    PyImage *self;
    unsigned int fmt, width, height, padding=0;
    char *data=NULL;
    Py_ssize_t length;

    if (!PyArg_ParseTuple(args, "III|Is#:Image", &fmt, &width, &height, &padding, &data, &length)) /* BR */
        return NULL;

    self = (PyImage*)type->tp_alloc(type, 0); /* NR */
    if (NULL != self)
    {
        int err = IMT_AllocImage(&self->image, fmt, width, height, padding, data);
        if (!err)
            return (PyObject *)self;
        else
            PyErr_Format(PyExc_SystemError,
                         "Image allocation failed, reason: %s",
                         IMT_GetErrorString(err));
    }

    Py_CLEAR(self);
    return NULL;
}

static void
image_dealloc(PyImage *self)
{
    IMT_FreeImage(self->image);
    Py_TYPE(self)->tp_free((PyObject*)self);
}


static PyObject *
image_flush(PyImage *self)
{
    IMT_FlushImage(self->image);
    Py_RETURN_NONE;
}

static PyObject *
image_swap_alpha(PyImage *self)
{
    IMT_SwapAlpha(self->image);
    Py_RETURN_NONE;
}

static PyObject *
image_save(PyImage *self, PyObject *args)
{
    int err;
    char *filename;

    if (!PyArg_ParseTuple(args, "s", &filename)) /* BR */
        return NULL;

    err = IMT_Save(filename, self->image, NULL);
    if (err)
        return PyErr_Format(PyExc_SystemError,
                            "Image saving failed, reason: %s",
                            IMT_GetErrorString(err));

    Py_RETURN_NONE;
}

static PyObject *
image_get_grayscale(PyImage *self, void *closure)
{
    IMT_Image *tmp;
    PyImage *imobj;

    int err = IMT_GenerateGrayscale(self->image, 0);
    if (err)
        return PyErr_Format(PyExc_SystemError,
                            "Image grayscale convertion failed, reason: %s",
                            IMT_GetErrorString(err));

    err = IMT_AllocImageFromFloat(&tmp, IMT_PIXFMT_GRAY8,
                                  self->image->width,
                                  self->image->height,
                                  self->image->grayscale->array.data.float_ptr);
    if (err)
        return PyErr_Format(PyExc_SystemError,
                            "Image grayscale creation failed, reason: %s",
                            IMT_GetErrorString(err));

    imobj = PyObject_New(PyImage, &PyImageType);
    if (imobj)
    {
        imobj->image = tmp;
    }

    return (PyObject *)imobj;
}

static PyObject *
image_get_prop(PyImage *self, void *closure)
{
    unsigned int value;

    switch ((int)closure)
    {
        case 0: value = self->image->format; break;
        case 1: value = self->image->width; break;
        case 2: value = self->image->height; break;
        case 3: value = self->image->stride; break;
        default: Py_RETURN_NONE;
    }

    return PyLong_FromUnsignedLong(value);
}

static PyObject *
image_get_data(PyImage *self, void *closure)
{
#if PY_MAJOR_VERSION < 3
    return PyBuffer_FromReadWriteMemory(self->image->data, self->image->height * self->image->stride);
#else
    return PyBytes_FromStringAndSize(self->image->data, self->image->height * self->image->stride);
#endif
}

static int
image_set_data_float(PyImage *self, PyObject *obj, void *closure)
{
#if PY_MAJOR_VERSION < 3
#error not supported yet
#else
    Py_buffer buf;
    int err;

    if (NULL == obj) return 0;

    if (PyObject_GetBuffer(obj, &buf, PyBUF_SIMPLE))
    {
        PyErr_Format(PyExc_TypeError, "Accept only simple buffer object, not %s", OBJ_TNAME(obj));
        return 1;
    }

    if (buf.len < (sizeof(float)*self->image->bpp*self->image->height))
    {
        PyErr_SetString(PyExc_TypeError, "Too small buffer");
        return 1;
    }
    
    err = IMT_FromFloatBuffer(self->image, buf.buf);
    if (err)
    {
        PyErr_Format(PyExc_SystemError,
                     "IMT_FromFloatBuffer failed, reason: %s",
                     IMT_GetErrorString(err));
        return 1;
    }

    PyBuffer_Release(&buf);
    return 0;
#endif
}

static PyGetSetDef image_getseters[] =
{
    {"grayscale", (getter)image_get_grayscale, NULL, "Return grayscale version of the image as a new Image object.", NULL},
    {"format", (getter)image_get_prop, NULL, "Return image format", (void*)0},
    {"width", (getter)image_get_prop, NULL, "Return image width", (void*)1},
    {"height", (getter)image_get_prop, NULL, "Return image height", (void*)2},
    {"stride", (getter)image_get_prop, NULL, "Return image stride", (void*)3},
    {"data", (getter)image_get_data, NULL, "Return data as byte buffer", NULL},
    {"floatdata", (getter)NULL, (setter)image_set_data_float, "Set data from a float buffer", NULL},
    {NULL} /* sentinel */
};

static struct PyMethodDef image_methods[] =
{
    {"flush", (PyCFunction)image_flush, METH_NOARGS, NULL},
    {"save", (PyCFunction)image_save, METH_VARARGS, NULL},
    {"swap_alpha", (PyCFunction)image_swap_alpha, METH_NOARGS, NULL},
    {NULL} /* sentinel */
};

static PyTypeObject PyImageType =
{
    PyObject_HEAD_INIT(NULL)

    tp_name         : MODNAME ".Image",
    tp_basicsize    : sizeof(PyImage),
    tp_flags        : Py_TPFLAGS_DEFAULT,
    tp_doc          : "Image Objects",

    tp_new          : (newfunc)image_new,
    tp_dealloc      : (destructor)image_dealloc,
    tp_methods      : image_methods,
    tp_getset       : image_getseters,
};

/*******************************************************************************************
** PyFeatureSetType
*/

typedef struct PyFeatureSet_STRUCT
{
    PyObject_HEAD

    KLT_FeatureSet ftset;
} PyFeatureSet;

static PyTypeObject PyFeatureSetType;

static PyObject *
ftset_new(PyTypeObject *type, PyObject *args)
{
    PyFeatureSet *self;

    self = (PyFeatureSet*)type->tp_alloc(type, 0); /* NR */
    if (NULL != self)
    {
        self->ftset.nfeatures = 0;
        self->ftset.features = NULL;
    }

    return (PyObject *)self;
}

static void
ftset_dealloc(PyFeatureSet *self)
{
    PyMem_Free(self->ftset.features);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
ftset_flush(PyFeatureSet *self, PyObject *args)
{
    PyMem_Free(self->ftset.features);
    self->ftset.nfeatures = 0;
    self->ftset.features = NULL;
    
    Py_RETURN_NONE;
}

static PyObject *
ftset_get_positions(PyFeatureSet *self, void *closure)
{
    PyObject *result;
	int num;
	
	if (closure)
		num = self->ftset.num_tracked_features;
	else
		num = self->ftset.nfeatures;
		
	result = PyTuple_New(num);
    if (result)
    {
        unsigned int i, k;

        for (i=k=0; i < self->ftset.nfeatures; i++)
        {
			if (closure && (self->ftset.features[i].status != KLT_TRACKED)) continue;

            PyTuple_SET_ITEM(result, k++,
                             Py_BuildValue("Iiff", i,
                                           self->ftset.features[i].status,
                                           self->ftset.features[i].position.x,
                                           self->ftset.features[i].position.y));
		}
    }

    return result;
}

static int
ftset_set_positions(PyFeatureSet *self, PyObject *tuple, void *closure)
{
    unsigned int i;

    if (!PyTuple_CheckExact(tuple))
    {
        PyErr_SetString(PyExc_TypeError, "Only tuple object accepted for __set__.");
        return 1;
    }

    PyMem_Free(self->ftset.features);
    self->ftset.nfeatures = 0;
    self->ftset.features = NULL;

    if (!tuple || !PyTuple_GET_SIZE(tuple))
        return 0;

    self->ftset.nfeatures = PyTuple_GET_SIZE(tuple);
    self->ftset.features = PyMem_Malloc(sizeof(KLT_Feature) * self->ftset.nfeatures);
    if (!self->ftset.features)
    {
        PyErr_NoMemory();
        goto err;
    }

    for (i=0; i < PyTuple_GET_SIZE(tuple); i++)
    {
        PyObject *pos = PyTuple_GET_ITEM(tuple, i);
        float x, y;

        if (!PyTuple_CheckExact(pos) || (PyTuple_GET_SIZE(pos) != 2))
        {
            PyErr_SetString(PyExc_TypeError, "Value should be a tuple of 2-tuples of numeric values.");
            goto err;
        }

        if (!PyArg_ParseTuple(pos, "ff", &x, &y))
            goto err;

        self->ftset.features[i].position.x = x;
        self->ftset.features[i].position.y = y;
    }

    return 0;

err:
    PyMem_Free(self->ftset.features);
    self->ftset.nfeatures = 0;
    self->ftset.features = NULL;
    return 1;
}

static PyObject *
ftset_get_estimations(PyFeatureSet *self, void *closure)
{
    PyObject *result;

    result = PyTuple_New(self->ftset.num_tracked_features);
    if (result)
    {
        unsigned int i,k;

        for (i=k=0; i < self->ftset.nfeatures; i++)
        {
            if (self->ftset.features[i].status != KLT_TRACKED) continue;

            PyTuple_SET_ITEM(result, k++,
                             Py_BuildValue("Iff", i,
                                           self->ftset.features[i].estimation.x,
                                           self->ftset.features[i].estimation.y));
        }
    }

    return result;
}

static PyGetSetDef ftset_getsetters[] =
{
    {"trackers", (getter)ftset_get_positions, (setter)ftset_set_positions, "Tuple of all features positions.", (void *)0},
    {"tracked", (getter)ftset_get_positions, NULL, "Tuple of positions of only tracked features.", (void *)1},
    {"estimations", (getter)ftset_get_estimations, NULL, "Tuple of estimation of only tracked features.", (void *)0},
    {NULL} /* sentinel */
};

static struct PyMethodDef ftset_methods[] =
{
    {"flush", (PyCFunction)ftset_flush, METH_NOARGS, NULL},
    {NULL} /* sentinel */
};

static PyTypeObject PyFeatureSetType =
{
    PyObject_HEAD_INIT(NULL)

    tp_name         : MODNAME ".FeatureSet",
    tp_basicsize    : sizeof(PyFeatureSet),
    tp_flags        : Py_TPFLAGS_DEFAULT,
    tp_doc          : "FeatureSet Objects",

    tp_new          : (newfunc)ftset_new,
    tp_dealloc      : (destructor)ftset_dealloc,
    tp_methods      : ftset_methods,
    tp_getset       : ftset_getsetters,
};

/*******************************************************************************************
** PyKLTContextType
*/

typedef struct PyKLTContext_STRUCT
{
    PyObject_HEAD

    KLT_Context ctx;
} PyKLTContext;

static PyTypeObject PyKLTContextType;

static PyObject *
kltctx_new(PyTypeObject *type, PyObject *args)
{
    PyKLTContext *self;

    self = (PyKLTContext*)type->tp_alloc(type, 0); /* NR */
    if (NULL != self)
    {
        /* TODO */

        KLT_InitContextDefaults(&self->ctx);
    }

    return (PyObject *)self;
}

static PyObject *
kltctx_setdefault(PyKLTContext *self, PyObject *args)
{
    KLT_InitContextDefaults(&self->ctx);
    Py_RETURN_NONE;
}

static PyObject *
kltctx_track(PyKLTContext *self, PyObject *args)
{
    PyImage *im1, *im2;
    PyFeatureSet *pyftset;
    int err;

    if (!PyArg_ParseTuple(args, "O!O!O!",
                          &PyImageType, &im1,
                          &PyImageType, &im2,
                          &PyFeatureSetType, &pyftset))
        return NULL;
        
    err = KLT_TrackFeatures(&self->ctx, im1->image, im2->image, &pyftset->ftset);
    if (err)
    {
        PyErr_SetString(PyExc_RuntimeError, "KLT_TrackFeatures() failed");
        return NULL;
    }

    Py_RETURN_NONE;
}

static struct PyMethodDef kltctx_methods[] =
{
    {"set_default", (PyCFunction)kltctx_setdefault, METH_NOARGS, NULL},
    {"track", (PyCFunction)kltctx_track, METH_VARARGS, NULL},
    {NULL} /* sentinel */
};

static PyTypeObject PyKLTContextType =
{
    PyObject_HEAD_INIT(NULL)

    tp_name         : MODNAME ".KLTContext",
    tp_basicsize    : sizeof(PyKLTContext),
    tp_flags        : Py_TPFLAGS_DEFAULT,
    tp_doc          : "KLTContext Objects",

    tp_new          : (newfunc)kltctx_new,
    tp_methods      : kltctx_methods,
};

/*******************************************************************************************
** PyArrayType
*/

typedef struct PyArray_STRUCT
{
    PyObject_HEAD

    MAT_Array *array;
} PyArray;

static PyTypeObject PyArrayType;

static PyObject *_array_new(MAT_Array *array)
{
	PyArray *obj;
	
	obj = PyObject_New(PyArray, &PyArrayType);
	if (obj)
		obj->array = array;
	return (PyObject *)obj;
}

static MAT_Array *_array_from_sequence(int type, PyObject *seq)
{
	int n;
		
	n = PySequence_Fast_GET_SIZE(seq);
	if (n > 0)
	{
		PyObject **items = PySequence_Fast_ITEMS(seq);
		MAT_Vector *vec;
		
		vec = MAT_AllocArray(n, type, 0, NULL);
		if (NULL != vec)
		{
#define SET_ARRAY_VALUES(T, array) \
{ \
	T *ptr = array->data.T##_ptr; \
	while (n--) *ptr++ = PyFloat_AsDouble(*items++); \
}
			switch (type)
			{
				case MAT_TYPE_FLOAT:
					SET_ARRAY_VALUES(float, vec);
					break;

				case MAT_TYPE_DOUBLE:
					SET_ARRAY_VALUES(double, vec);
					break;
			}
			
			return vec;
		}
		else
			PyErr_SetString(PyExc_MemoryError, "Unable to alloc internal vector");
	}
	else
		PyErr_SetString(PyExc_TypeError, "Can't create array from empty sequence");

	return NULL;
}

static PyObject *
array_new(PyTypeObject *type, PyObject *args)
{
    PyArray *self;
    unsigned int width, arraytype;

	if (!PyArg_ParseTuple(args, "II", &width, &arraytype))
		return NULL;

    self = (PyArray*)type->tp_alloc(type, 0); /* NR */
    if (NULL != self)
    {
		self->array = MAT_AllocArray(width, arraytype, 0, NULL);
		if (NULL == self->array)
			Py_CLEAR(self);
	}

    return (PyObject *)self;
}

static void
array_dealloc(PyArray *self)
{
    MAT_FreeArray(self->array);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
array_zero(PyArray *self)
{
    MAT_ZeroArray(self->array);
    Py_RETURN_NONE;
}

static PyObject *
array_get_data(PyArray *self, void *closure)
{
	PyObject *data = PyTuple_New(self->array->width);
	int i;
	
	if (NULL != data)
	{
		for (i=0; i < self->array->width; i++)
		{
			PyObject *value;
			
			switch (self->array->type)
			{
				case MAT_TYPE_FLOAT:
					value = PyFloat_FromDouble(self->array->data.float_ptr[i]);
					break;

				case MAT_TYPE_DOUBLE:
					value = PyFloat_FromDouble(self->array->data.double_ptr[i]);
					break;
				
				default: value = NULL;
			}
			
			if (NULL == value)
			{
				Py_DECREF(data);
				return NULL;
			}
			
			PyTuple_SET_ITEM(data, i, value);
		}
	}
		
	return data;
}

static int
array_set_data(PyArray *self, PyObject *value, void *closure)
{
	PyObject *fast;
	int n, res;
	
	if (NULL == value)
	{
		MAT_ZeroArray(self->array);
		return 0;
	}
	
	fast = PySequence_Fast(value, "argument is not convertible into tuple or list"); /* NR */
	if (NULL == fast)
		return 1;
			
	n = PySequence_Fast_GET_SIZE(fast);
	if (n == self->array->width)
	{
		PyObject **items = PySequence_Fast_ITEMS(fast);
		
		switch (self->array->type)
		{
			case MAT_TYPE_FLOAT:
				SET_ARRAY_VALUES(float, self->array);
				break;

			case MAT_TYPE_DOUBLE:
				SET_ARRAY_VALUES(double, self->array);
				break;
		}
		
		res = 0;
	}
	else
		PyErr_SetString(PyExc_TypeError, "Size incompatible array for this matrix");
	
	Py_DECREF(fast);
	return res;
}

static struct PyMethodDef array_methods[] =
{
    {"zero", (PyCFunction)array_zero, METH_NOARGS, NULL},
    {NULL} /* sentinel */
};

static PyGetSetDef array_getsetters[] =
{
    {"data", (getter)array_get_data, (setter)array_set_data, "Array coefficients as tuple of float.", (void *)0},
    {NULL} /* sentinel */
};

static PyTypeObject PyArrayType =
{
    PyObject_HEAD_INIT(NULL)

    tp_name         : MODNAME ".Array",
    tp_basicsize    : sizeof(PyArray),
    tp_flags        : Py_TPFLAGS_DEFAULT,
    tp_doc          : "Array Objects",

    tp_new          : (newfunc)array_new,
    tp_dealloc      : (destructor)array_dealloc,
    tp_methods      : array_methods,
    tp_getset       : array_getsetters,
};

/*******************************************************************************************
** PyMatrixType
*/

typedef struct PyMatrix_STRUCT
{
    PyObject_HEAD

	unsigned int nrows, ncols;
    MAT_Matrix *matrix;
} PyMatrix;

static PyTypeObject PyMatrixType;

static PyObject *_matrix_new(MAT_Matrix *mat)
{
	PyMatrix *obj;
	
	obj = PyObject_New(PyMatrix, &PyMatrixType);
	if (obj)
		obj->matrix = mat;
	return (PyObject *)obj;
}

static PyObject *
matrix_new(PyTypeObject *type, PyObject *args)
{
    PyMatrix *self;
    unsigned int ncols, nrows, mattype;

	if (!PyArg_ParseTuple(args, "III", &nrows, &ncols, &mattype))
		return NULL;

    self = (PyMatrix*)type->tp_alloc(type, 0); /* NR */
    if (NULL != self)
    {
		self->matrix = MAT_AllocMatrix(nrows, ncols, mattype, 0, NULL);
		if (NULL != self->matrix)
		{
			self->nrows = nrows;
			self->ncols = ncols;
			MAT_SetIdentityMatrix(self->matrix);
		}
		else
			Py_CLEAR(self);
	}

    return (PyObject *)self;
}

static void
matrix_dealloc(PyMatrix *self)
{
    MAT_FreeMatrix(self->matrix);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
matrix_identity(PyMatrix *self)
{
    MAT_SetIdentityMatrix(self->matrix);
    Py_RETURN_NONE;
}

static PyObject *
matrix_zero(PyMatrix *self)
{
    MAT_ZeroMatrix(self->matrix);
    Py_RETURN_NONE;
}

static PyObject *
matrix_transpose(PyMatrix *self)
{
	MAT_InPlaceTransposeMatrix(self->matrix);
    Py_RETURN_NONE;
}

static PyObject *
matrix_multiply(PyMatrix *self, PyObject *args)
{
	PyObject *obj;
	
	if (!PyArg_ParseTuple(args, "O", &obj))
		return NULL;
		
	if (PyMatrix_CheckExact(obj))
	{
		MAT_Matrix *res = MAT_MultiplyMatrix(((PyMatrix *)obj)->matrix, self->matrix);
		if (NULL == res)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to multiply matrices");
			return NULL;
		}
		
		return _matrix_new(res);
	}
	else if (PyArray_CheckExact(obj))
	{
		MAT_Vector *res = MAT_MultiplyMatrixVector(self->matrix, ((PyArray *)obj)->array);
		if (NULL == res)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to multiply matrix with an array");
			return NULL;
		}
		
		return _array_new(res);
	}
	else if (PyNumber_Check(obj))
	{
		MAT_Matrix *res;
		
		obj = PyNumber_Float(obj); /* NR */
		if (NULL == obj)
			return NULL;

		res = MAT_ScalarMultiplyMatrix(PyFloat_AS_DOUBLE(obj), self->matrix);
		Py_DECREF(obj);
		return _matrix_new(res);
	}
	else
	{
		MAT_Vector *vec;
		PyObject *fast, *pyres=NULL;
		int n;
		
		fast = PySequence_Fast(obj, "argument is not convertible into tuple or list"); /* NR */
		if (NULL == fast)
			return NULL;
			
		n = PySequence_Fast_GET_SIZE(fast);
		if (n == self->ncols)
		{
			vec = _array_from_sequence(self->matrix->array.type, fast);
			if (NULL != vec)
			{
				MAT_Vector *res = MAT_MultiplyMatrixVector(self->matrix, vec);
				
				if (NULL != res)
					pyres = _array_new(res); /* NR */
				else
					PyErr_SetString(PyExc_RuntimeError, "Failed to multiply matrix with a vector");
					
				MAT_FreeArray(vec);
			}
		}
		else
			PyErr_Format(PyExc_TypeError, "Input sequence must contains %u items", self->matrix->ncols);
			
		Py_DECREF(fast);
		return pyres;
	}

	PyErr_SetString(PyExc_TypeError, "can multiply a matrix only with a matrix, a vector or a float");
	return NULL;
}

static PyObject *
matrix_get_array(PyMatrix *self, void *closure)
{
	MAT_Array *array = MAT_AllocArray(self->ncols*self->nrows, self->matrix->array.type, 0, self->matrix->array.data.void_ptr);
	
	if (NULL != array)
		return _array_new(array);
	
	PyErr_SetString(PyExc_MemoryError, "Failed to create array");
	return NULL;
}

static int
matrix_set_array(PyMatrix *self, PyObject *value, void *closure)
{
	PyObject *fast;
	int n, res;
	
	if (NULL == value)
	{
		MAT_ZeroMatrix(self->matrix);
		return 0;
	}
	
	fast = PySequence_Fast(value, "argument is not convertible into tuple or list"); /* NR */
	if (NULL == fast)
		return 1;
			
	n = PySequence_Fast_GET_SIZE(fast);
	if (n == self->matrix->array.width)
	{
		PyObject **items = PySequence_Fast_ITEMS(fast);
#define SET_MATRIX_VALUES(T) \
{ \
	T *ptr = self->matrix->array.data.T##_ptr; \
	while (n--) *ptr++ = PyFloat_AsDouble(*items++); \
}
		switch (self->matrix->array.type)
		{
			case MAT_TYPE_FLOAT:
				SET_MATRIX_VALUES(float);
				break;

			case MAT_TYPE_DOUBLE:
				SET_MATRIX_VALUES(double);
				break;
		}
		
		res = 0;
	}
	else
		PyErr_SetString(PyExc_TypeError, "Size incompatible array for this matrix");
	
	Py_DECREF(fast);
	return res;
}

static struct PyMethodDef matrix_methods[] =
{
    {"zero", (PyCFunction)matrix_zero, METH_NOARGS, NULL},
    {"identity", (PyCFunction)matrix_identity, METH_NOARGS, NULL},
    {"transpose", (PyCFunction)matrix_transpose, METH_NOARGS, NULL},
    {"multiply", (PyCFunction)matrix_multiply, METH_VARARGS, NULL},
    {NULL} /* sentinel */
};

static PyGetSetDef matrix_getsetters[] =
{
    {"array", (getter)matrix_get_array, (setter)matrix_set_array, "Matrix coefficients as flat array.", (void *)0},
    {NULL} /* sentinel */
};

static PyMemberDef matrix_members[] =
{
    {"nrows", T_ULONG, offsetof(PyMatrix, nrows), READONLY, NULL},
    {"ncols", T_ULONG, offsetof(PyMatrix, ncols), READONLY, NULL},
    {NULL} /* sentinel */
};

static PyTypeObject PyMatrixType =
{
    PyObject_HEAD_INIT(NULL)

    tp_name         : MODNAME ".Matrix",
    tp_basicsize    : sizeof(PyMatrix),
    tp_flags        : Py_TPFLAGS_DEFAULT,
    tp_doc          : "Matrix Objects",

    tp_new          : (newfunc)matrix_new,
    tp_dealloc      : (destructor)matrix_dealloc,
    tp_methods      : matrix_methods,
    tp_members      : matrix_members,
    tp_getset       : matrix_getsetters,
};

/*******************************************************************************************
** Module
*/

static PyObject *
motion_load_image(PyObject *self, PyObject *args)
{
    int err;
    PyImage *obj;
    IMT_Image *image;
    char *filename;

    if (!PyArg_ParseTuple(args, "s", &filename)) /* BR */
        return NULL;

    err = IMT_Load(filename, &image, NULL);
    if (err)
        return PyErr_Format(PyExc_SystemError,
                            "Image loading failed, reason: %s",
                            IMT_GetErrorString(err));

    obj = PyObject_New(PyImage, &PyImageType); /* NR */
    if (obj)
    {
        obj->image = image;
    }
    
    return (PyObject *)obj;
}

static PyObject *
motion_detect_corners(PyObject *self, PyObject *args)
{
    int num_corners=0;
    PyImage *imobj;
    float threshold;
    int nonmax=1;
    xy *corners;
    PyObject *result;

    if (!PyArg_ParseTuple(args, "O!f|ii",
                          &PyImageType, &imobj,
                          &threshold,
                          &num_corners,
                          &nonmax)) /* BR */
        return NULL;

    if (imobj->image->format != IMT_PIXFMT_GRAY8)
    {
        PyErr_SetString(PyExc_TypeError, "Invalide image format, must be IMT_PIXFMT_GRAY8");
        return NULL;
    }

    if (num_corners > 0)
        corners = fast9_detect_limited(imobj->image->data,
                                       imobj->image->width,
                                       imobj->image->height,
                                       imobj->image->stride,
                                       threshold,
                                       &num_corners, nonmax);
    else if (nonmax)
        corners = fast9_detect_nonmax(imobj->image->data,
                                      imobj->image->width,
                                      imobj->image->height,
                                      imobj->image->stride,
                                      threshold,
                                      &num_corners);
    else
        corners = fast9_detect(imobj->image->data,
                               imobj->image->width,
                               imobj->image->height,
                               imobj->image->stride,
                               threshold,
                               &num_corners);

    if (!corners)
        return PyErr_NoMemory();

    result = PyTuple_New(num_corners);
    if (result)
    {
        unsigned int i;

        for (i=0; i < num_corners; i++)
            PyTuple_SET_ITEM(result, i, Py_BuildValue("ff",
                                                      (float)corners[i].x,
                                                      (float)corners[i].y));
    }

    return result;
}

static PyMethodDef methods[] =
{
    {"load_image", (PyCFunction)motion_load_image, METH_VARARGS, NULL},
    {"detect_corners", (PyCFunction)motion_detect_corners, METH_VARARGS, NULL},
    {NULL} /* sentinel */
};

static int add_constants(PyObject *m)
{
    INSI(m, "IMT_PIXFMT_UNKNOWN", IMT_PIXFMT_UNKNOWN);
    INSI(m, "IMT_PIXFMT_GRAY8", IMT_PIXFMT_GRAY8);
    INSI(m, "IMT_PIXFMT_GRAYA16", IMT_PIXFMT_GRAYA16);
    INSI(m, "IMT_PIXFMT_RGB24", IMT_PIXFMT_RGB24);
    INSI(m, "IMT_PIXFMT_ARGB32", IMT_PIXFMT_ARGB32);
    INSI(m, "MAT_TYPE_FLOAT", MAT_TYPE_FLOAT);
    INSI(m, "MAT_TYPE_DOUBLE", MAT_TYPE_DOUBLE);
    INSI(m, "KLT_TRACKED", KLT_TRACKED);
    INSI(m, "KLT_NOT_FOUND", KLT_NOT_FOUND);
    INSI(m, "KLT_SMALL_DET", KLT_SMALL_DET);
    INSI(m, "KLT_MAX_ITERATIONS", KLT_MAX_ITERATIONS);
    INSI(m, "KLT_OOB", KLT_OOB);
    INSI(m, "KLT_LARGE_RESIDUE", KLT_LARGE_RESIDUE);

    return 0;
}

#if PY_MAJOR_VERSION >= 3
static PyModuleDef motionmodule =
{
    PyModuleDef_HEAD_INIT,
    MODNAME,
    "motion lib.",
    -1,
    methods, NULL, NULL, NULL, NULL
};
#endif

PyMODINIT_FUNC
INITFUNC(void)
{
    PyObject *m=NULL;

    IMT_Init();

    if (!PyType_Ready(&PyImageType)) {
        if (!PyType_Ready(&PyFeatureSetType)) {
            if (!PyType_Ready(&PyKLTContextType)) {
				if (!PyType_Ready(&PyMatrixType)) {
					if (!PyType_Ready(&PyArrayType)) {
#if PY_MAJOR_VERSION < 3
						m = Py_InitModule(MODNAME, methods);
#else
						m = PyModule_Create(&motionmodule);
#endif
						if (m) {
							add_constants(m);

							ADD_TYPE(m, "Image", &PyImageType);
							ADD_TYPE(m, "FeatureSet", &PyFeatureSetType);
							ADD_TYPE(m, "KLTContext", &PyKLTContextType);
							ADD_TYPE(m, "Array", &PyArrayType);
							ADD_TYPE(m, "Vector", &PyArrayType); /* Alias */
							ADD_TYPE(m, "Matrix", &PyMatrixType);
						}
					}
                }
            }
        }
    }

#if PY_MAJOR_VERSION >= 3
    return m;
#endif
}
