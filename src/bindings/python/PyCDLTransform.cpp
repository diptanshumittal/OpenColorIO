// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenColorIO Project.

#include <Python.h>
#include <OpenColorIO/OpenColorIO.h>

#include "PyUtil.h"
#include "PyDoc.h"

#define GetConstCDLTransform(pyobject) GetConstPyOCIO<PyOCIO_Transform, \
    ConstCDLTransformRcPtr, CDLTransform>(pyobject, PyOCIO_CDLTransformType)

#define GetEditableCDLTransform(pyobject) GetEditablePyOCIO<PyOCIO_Transform, \
    CDLTransformRcPtr, CDLTransform>(pyobject, PyOCIO_CDLTransformType)

namespace OCIO_NAMESPACE
{

namespace
{

///////////////////////////////////////////////////////////////////////
///

int PyOCIO_CDLTransform_init(PyOCIO_Transform * self, PyObject * args, PyObject * kwds); 
PyObject * PyOCIO_CDLTransform_equals(PyObject * self, PyObject * args);
PyObject * PyOCIO_CDLTransform_validate(PyObject * self);

PyObject * PyOCIO_CDLTransform_CreateFromFile(PyObject * self, PyObject * args);
PyObject * PyOCIO_CDLTransform_getXML(PyObject * self, PyObject *);
PyObject * PyOCIO_CDLTransform_setXML(PyObject * self, PyObject * args);
PyObject * PyOCIO_CDLTransform_getSlope(PyObject * self, PyObject *);
PyObject * PyOCIO_CDLTransform_getOffset(PyObject * self, PyObject *);
PyObject * PyOCIO_CDLTransform_getPower(PyObject * self, PyObject *);
PyObject * PyOCIO_CDLTransform_getSOP(PyObject * self, PyObject *);
PyObject * PyOCIO_CDLTransform_getSat(PyObject * self, PyObject *);
PyObject * PyOCIO_CDLTransform_setSlope(PyObject * self, PyObject * args);
PyObject * PyOCIO_CDLTransform_setOffset(PyObject * self, PyObject * args);
PyObject * PyOCIO_CDLTransform_setPower(PyObject * self, PyObject * args);
PyObject * PyOCIO_CDLTransform_setSOP(PyObject * self, PyObject * args);
PyObject * PyOCIO_CDLTransform_setSat(PyObject * self, PyObject * args);
PyObject * PyOCIO_CDLTransform_getSatLumaCoefs(PyObject * self, PyObject *);
PyObject * PyOCIO_CDLTransform_getID(PyObject * self, PyObject *);
PyObject * PyOCIO_CDLTransform_setID(PyObject * self,  PyObject * args);
PyObject * PyOCIO_CDLTransform_getDescription(PyObject * self, PyObject *);
PyObject * PyOCIO_CDLTransform_setDescription(PyObject * self, PyObject * args);

///////////////////////////////////////////////////////////////////////
///

PyMethodDef PyOCIO_CDLTransform_methods[] = {
    { "equals",
    PyOCIO_CDLTransform_equals, METH_VARARGS, CDLTRANSFORM_EQUALS__DOC__ },
    { "validate",
    (PyCFunction) PyOCIO_CDLTransform_validate, METH_NOARGS, CDLTRANSFORM_VALIDATE__DOC__ },
    { "CreateFromFile",
    PyOCIO_CDLTransform_CreateFromFile, METH_VARARGS, CDLTRANSFORM_CREATEFROMFILE__DOC__ },
    { "getXML",
    (PyCFunction) PyOCIO_CDLTransform_getXML, METH_NOARGS, CDLTRANSFORM_GETXML__DOC__ },
    { "setXML",
    PyOCIO_CDLTransform_setXML, METH_VARARGS, CDLTRANSFORM_SETXML__DOC__ },
    { "getSlope",
    (PyCFunction) PyOCIO_CDLTransform_getSlope, METH_NOARGS, CDLTRANSFORM_GETSLOPE__DOC__ },
    { "getOffset",
    (PyCFunction) PyOCIO_CDLTransform_getOffset, METH_NOARGS, CDLTRANSFORM_GETOFFSET__DOC__ },
    { "getPower",
    (PyCFunction) PyOCIO_CDLTransform_getPower, METH_NOARGS, CDLTRANSFORM_GETPOWER__DOC__ },
    { "getSOP",
    (PyCFunction) PyOCIO_CDLTransform_getSOP, METH_NOARGS, CDLTRANSFORM_GETSOP__DOC__ },
    { "getSat",
    (PyCFunction) PyOCIO_CDLTransform_getSat, METH_NOARGS, CDLTRANSFORM_GETSAT__DOC__ },
    { "setSlope",
    PyOCIO_CDLTransform_setSlope, METH_VARARGS, CDLTRANSFORM_SETSLOPE__DOC__ },
    { "setOffset",
    PyOCIO_CDLTransform_setOffset, METH_VARARGS, CDLTRANSFORM_SETOFFSET__DOC__ },
    { "setPower",
    PyOCIO_CDLTransform_setPower, METH_VARARGS, CDLTRANSFORM_SETPOWER__DOC__ },
    { "setSOP",
    PyOCIO_CDLTransform_setSOP, METH_VARARGS, CDLTRANSFORM_SETSOP__DOC__ },
    { "setSat",
    PyOCIO_CDLTransform_setSat, METH_VARARGS, CDLTRANSFORM_SETSAT__DOC__ },
    { "getSatLumaCoefs",
    (PyCFunction) PyOCIO_CDLTransform_getSatLumaCoefs, METH_NOARGS, CDLTRANSFORM_GETSATLUMACOEFS__DOC__ },
    { "getID",
    (PyCFunction) PyOCIO_CDLTransform_getID, METH_NOARGS, CDLTRANSFORM_GETID__DOC__ },
    { "setID",
    PyOCIO_CDLTransform_setID, METH_VARARGS, CDLTRANSFORM_SETID__DOC__ },
    { "getDescription",
    (PyCFunction) PyOCIO_CDLTransform_getDescription, METH_NOARGS, CDLTRANSFORM_GETDESCRIPTION__DOC__ },
    { "setDescription",
    PyOCIO_CDLTransform_setDescription, METH_VARARGS, CDLTRANSFORM_SETDESCRIPTION__DOC__ },
    { NULL, NULL, 0, NULL }
};

}

///////////////////////////////////////////////////////////////////////////
///

PyTypeObject PyOCIO_CDLTransformType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    OCIO_PYTHON_NAMESPACE(CDLTransform),        //tp_name
    sizeof(PyOCIO_Transform),                   //tp_basicsize
    0,                                          //tp_itemsize
    0,                                          //tp_dealloc
    0,                                          //tp_print
    0,                                          //tp_getattr
    0,                                          //tp_setattr
    0,                                          //tp_compare
    0,                                          //tp_repr
    0,                                          //tp_as_number
    0,                                          //tp_as_sequence
    0,                                          //tp_as_mapping
    0,                                          //tp_hash 
    0,                                          //tp_call
    0,                                          //tp_str
    0,                                          //tp_getattro
    0,                                          //tp_setattro
    0,                                          //tp_as_buffer
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   //tp_flags
    CDLTRANSFORM__DOC__,                        //tp_doc 
    0,                                          //tp_traverse 
    0,                                          //tp_clear 
    0,                                          //tp_richcompare 
    0,                                          //tp_weaklistoffset 
    0,                                          //tp_iter 
    0,                                          //tp_iternext 
    PyOCIO_CDLTransform_methods,                //tp_methods 
    0,                                          //tp_members 
    0,                                          //tp_getset 
    &PyOCIO_TransformType,                      //tp_base 
    0,                                          //tp_dict 
    0,                                          //tp_descr_get 
    0,                                          //tp_descr_set 
    0,                                          //tp_dictoffset 
    (initproc) PyOCIO_CDLTransform_init,        //tp_init 
    0,                                          //tp_alloc 
    0,                                          //tp_new 
    0,                                          //tp_free
    0,                                          //tp_is_gc
};

namespace
{

///////////////////////////////////////////////////////////////////////
///

int PyOCIO_CDLTransform_init(PyOCIO_Transform *self, PyObject* args, PyObject* kwds)
{
    OCIO_PYTRY_ENTER()
    CDLTransformRcPtr ptr = CDLTransform::Create();
    int ret = BuildPyTransformObject<CDLTransformRcPtr>(self, ptr);
    PyObject* pyslope = NULL;
    PyObject* pyoffset = NULL;
    PyObject* pypower = NULL;
    double sat = -1.0; // -1.0 is an illegal value for saturation
    char* direction = NULL;
    char* id = NULL;
    char* description = NULL;
    static const char *kwlist[] = { "slope", "offset",
        "power", "sat", "direction", "id", "description", NULL };
    if(!PyArg_ParseTupleAndKeywords(args, kwds, "|OOOdsss",
        const_cast<char **>(kwlist),
        &pyslope, &pyoffset, &pypower, &sat, &direction, &id,
        &description)) return -1;
    if (pyslope)
    {
        std::vector<double> slope;
        if(!FillDoubleVectorFromPySequence(pyslope, slope) ||
            (slope.size() != 3))
        {
            PyErr_SetString(PyExc_TypeError,
                "slope must be a float array, size 3");
            return 0;
        }
        ptr->setSlope(&slope[0]);
    }
    if (pyoffset)
    {
        std::vector<double> offset;
        if(!FillDoubleVectorFromPySequence(pyoffset, offset) ||
            (offset.size() != 3))
        {
            PyErr_SetString(PyExc_TypeError,
                "offset must be a float array, size 3");
            return 0;
        }
        ptr->setOffset(&offset[0]);
    }
    if (pypower)
    {
        std::vector<double> power;
        if(!FillDoubleVectorFromPySequence(pypower, power) ||
            (power.size() != 3))
        {
            PyErr_SetString(PyExc_TypeError,
                "power must be a float array, size 3");
            return 0;
        }
        ptr->setPower(&power[0]);
    }
    if(sat >= 0.0) ptr->setSat(sat);
    if(direction) ptr->setDirection(TransformDirectionFromString(direction));
    if(id) ptr->setID(id);
    if(description) ptr->setDescription(description);
    return ret;
    OCIO_PYTRY_EXIT(-1)
}

PyObject * PyOCIO_CDLTransform_CreateFromFile(PyObject * /*self*/, PyObject * args)
{
    OCIO_PYTRY_ENTER()
    const char * src = 0;
    const char * cccid = 0;
    if (!PyArg_ParseTuple(args, "ss:CreateFromFile", &src, &cccid)) return NULL;
    return BuildEditablePyTransform(CDLTransform::CreateFromFile(src, cccid));
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_equals(PyObject * self, PyObject * args)
{
    OCIO_PYTRY_ENTER()
    PyObject* pyother = 0;
    if (!PyArg_ParseTuple(args, "O:equals", &pyother)) return NULL;
    ConstCDLTransformRcPtr transform = GetConstCDLTransform(self);
    if(!IsPyOCIOType(pyother, PyOCIO_CDLTransformType))
        return PyBool_FromLong(false);
    ConstCDLTransformRcPtr other = GetConstCDLTransform(pyother);
    return PyBool_FromLong(transform->equals(*other));
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_validate(PyObject * self)
{
    OCIO_PYTRY_ENTER()
    ConstCDLTransformRcPtr transform = GetConstCDLTransform(self);
    transform->validate();
    Py_RETURN_NONE;
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_getXML(PyObject * self, PyObject *)
{
    OCIO_PYTRY_ENTER()
    ConstCDLTransformRcPtr transform = GetConstCDLTransform(self);
    return PyString_FromString(transform->getXML());
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_setXML(PyObject * self, PyObject * args)
{
    OCIO_PYTRY_ENTER()
    const char* str = 0;
    if (!PyArg_ParseTuple(args, "s:setXML", &str)) return NULL;
    CDLTransformRcPtr transform = GetEditableCDLTransform(self);
    transform->setXML(str);
    Py_RETURN_NONE;
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_getSlope(PyObject * self, PyObject *)
{
    OCIO_PYTRY_ENTER()
    ConstCDLTransformRcPtr transform = GetConstCDLTransform(self);
    std::vector<double> data(3);
    transform->getSlope(&data[0]);
    return CreatePyListFromDoubleVector(data);
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_getOffset(PyObject * self, PyObject *)
{
    OCIO_PYTRY_ENTER()
    ConstCDLTransformRcPtr transform = GetConstCDLTransform(self);
    std::vector<double> data(3);
    transform->getOffset(&data[0]);
    return CreatePyListFromDoubleVector(data);
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_getPower(PyObject * self, PyObject *)
{
    OCIO_PYTRY_ENTER()
    ConstCDLTransformRcPtr transform = GetConstCDLTransform(self);
    std::vector<double> data(3);
    transform->getPower(&data[0]);
    return CreatePyListFromDoubleVector(data);
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_getSOP(PyObject * self, PyObject *)
{
    OCIO_PYTRY_ENTER()
    ConstCDLTransformRcPtr transform = GetConstCDLTransform(self);
    std::vector<double> data(9);
    transform->getSOP(&data[0]);
    return CreatePyListFromDoubleVector(data);
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_getSat(PyObject * self, PyObject *)
{
    OCIO_PYTRY_ENTER()
    ConstCDLTransformRcPtr transform = GetConstCDLTransform(self);
    return PyFloat_FromDouble(transform->getSat());
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_setSlope(PyObject * self, PyObject * args)
{
    OCIO_PYTRY_ENTER()
    PyObject* pyData = 0;
    if (!PyArg_ParseTuple(args, "O:setSlope", &pyData)) return NULL;
    CDLTransformRcPtr transform = GetEditableCDLTransform(self);
    std::vector<double> data;
    if(!FillDoubleVectorFromPySequence(pyData, data) || (data.size() != 3))
    {
        PyErr_SetString(PyExc_TypeError, "First argument must be a float array, size 3");
        return 0;
    }
    transform->setSlope(&data[0]);
    Py_RETURN_NONE;
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_setOffset(PyObject * self, PyObject * args)
{
    OCIO_PYTRY_ENTER()
    PyObject* pyData = 0;
    if (!PyArg_ParseTuple(args, "O:setOffset", &pyData)) return NULL;
    CDLTransformRcPtr transform = GetEditableCDLTransform(self);
    std::vector<double> data;
    if(!FillDoubleVectorFromPySequence(pyData, data) || (data.size() != 3))
    {
        PyErr_SetString(PyExc_TypeError, "First argument must be a float array, size 3");
        return 0;
    }
    transform->setOffset(&data[0]);
    Py_RETURN_NONE;
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_setPower(PyObject * self, PyObject * args)
{
    OCIO_PYTRY_ENTER()
    PyObject* pyData = 0;
    if (!PyArg_ParseTuple(args, "O:setPower", &pyData)) return NULL;
    CDLTransformRcPtr transform = GetEditableCDLTransform(self);    
    std::vector<double> data;
    if(!FillDoubleVectorFromPySequence(pyData, data) || (data.size() != 3))
    {
        PyErr_SetString(PyExc_TypeError, "First argument must be a float array, size 3");
        return 0;
    }
    transform->setPower(&data[0]);
    Py_RETURN_NONE;
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_setSOP(PyObject * self, PyObject * args)
{
    OCIO_PYTRY_ENTER()
    PyObject* pyData = 0;
    if (!PyArg_ParseTuple(args, "O:setSOP", &pyData)) return NULL;
    CDLTransformRcPtr transform = GetEditableCDLTransform(self); 
    std::vector<double> data;
    if(!FillDoubleVectorFromPySequence(pyData, data) || (data.size() != 9))
    {
        PyErr_SetString(PyExc_TypeError, "First argument must be a float array, size 9");
        return 0;
    }
    transform->setSOP(&data[0]);    
    Py_RETURN_NONE;
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_setSat(PyObject * self, PyObject * args)
{
    OCIO_PYTRY_ENTER()
    double sat;
    if (!PyArg_ParseTuple(args, "d:setSat", &sat)) return NULL;
    CDLTransformRcPtr transform = GetEditableCDLTransform(self);    
    transform->setSat(sat);    
    Py_RETURN_NONE;
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_getSatLumaCoefs(PyObject * self, PyObject *)
{
    OCIO_PYTRY_ENTER()
    ConstCDLTransformRcPtr transform = GetConstCDLTransform(self);
    std::vector<double> data(3);
    transform->getSatLumaCoefs(&data[0]);
    return CreatePyListFromDoubleVector(data);
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_getID(PyObject * self, PyObject *)
{
    OCIO_PYTRY_ENTER()
    ConstCDLTransformRcPtr transform = GetConstCDLTransform(self);
    return PyString_FromString(transform->getID());
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_setID(PyObject * self, PyObject * args)
{
    OCIO_PYTRY_ENTER()
    const char* str = 0;
    if (!PyArg_ParseTuple(args, "s:setID", &str)) return NULL;
    CDLTransformRcPtr transform = GetEditableCDLTransform(self);
    transform->setID(str);
    Py_RETURN_NONE;
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_getDescription(PyObject * self, PyObject *)
{
    OCIO_PYTRY_ENTER()
    ConstCDLTransformRcPtr transform = GetConstCDLTransform(self);
    return PyString_FromString(transform->getDescription());
    OCIO_PYTRY_EXIT(NULL)
}

PyObject * PyOCIO_CDLTransform_setDescription(PyObject * self, PyObject * args)
{
    OCIO_PYTRY_ENTER()
    const char* str = 0;
    if (!PyArg_ParseTuple(args, "s:setDescription", &str)) return NULL;
    CDLTransformRcPtr transform = GetEditableCDLTransform(self);
    transform->setDescription(str);
    Py_RETURN_NONE;
    OCIO_PYTRY_EXIT(NULL)
}

}

} // namespace OCIO_NAMESPACE
