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

#define PyImage_Check(op) PyObject_TypeCheck(op, &PyImageType)
#define PyImage_CheckExact(op) ((op)->ob_type == &PyImageType)

#define PyFeatureSet_Check(op) PyObject_TypeCheck(op, &PyFeatureSetType)
#define PyFeatureSet_CheckExact(op) ((op)->ob_type == &PyFeatureSetType)

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

typedef struct PyImage_STRUCT {
    PyObject_HEAD

    IMT_Image *image;
} PyImage;

static PyTypeObject PyImageType;

static PyObject *
image_new(PyTypeObject *type, PyObject *args)
{
    PyImage *self;
    unsigned int fmt, width, height, stride=0;
    char *data=NULL;
    Py_ssize_t length;

    if (!PyArg_ParseTuple(args, "III|Is#:Image", &fmt, &width, &height, &stride, &data, &length)) /* BR */
        return NULL;

    if (stride == 0)
        stride = width * IMT_GetBytesPerPixel(fmt);

    self = (PyImage*)type->tp_alloc(type, 0); /* NR */
    if (NULL != self)
    {
        int err = IMT_AllocImage(&self->image, fmt, width, height, stride, data);
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

static PyGetSetDef image_getseters[] = {
    {"grayscale", (getter)image_get_grayscale, NULL, "Return grayscale version of the image as a new Image object.", NULL},
    {"format", (getter)image_get_prop, NULL, "Return image format", (void*)0},
    {"width", (getter)image_get_prop, NULL, "Return image width", (void*)1},
    {"height", (getter)image_get_prop, NULL, "Return image height", (void*)2},
    {"stride", (getter)image_get_prop, NULL, "Return image stride", (void*)3},
    {"data", (getter)image_get_data, NULL, "Return data as byte buffer", NULL},
    {NULL} /* sentinel */
};

static struct PyMethodDef image_methods[] =
{
    {"flush", (PyCFunction)image_flush, METH_NOARGS, NULL},
    {"save", (PyCFunction)image_save, METH_VARARGS, NULL},
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

typedef struct PyFeatureSet_STRUCT {
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
                             Py_BuildValue("ff",
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
                             Py_BuildValue("ff",
                                           self->ftset.features[i].estimation.x,
                                           self->ftset.features[i].estimation.y));
        }
    }

    return result;
}

static PyGetSetDef ftset_getseters[] = {
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
    tp_getset       : ftset_getseters,
};

/*******************************************************************************************
** PyKLTContextType
*/

typedef struct PyKLTContext_STRUCT {
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

static PyMethodDef methods[] = {
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
                }
            }
        }
    }

#if PY_MAJOR_VERSION >= 3
    return m;
#endif
}
