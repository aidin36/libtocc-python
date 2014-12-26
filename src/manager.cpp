/*
 * This file is part of libtocc-python. A Python wrapper for libtocc.
 * (see <http://www.github.com/aidin36/libtocc-python>)
 * Copyright (C) 2014, Aidin Gharibnavaz <aidin@t-o-c-c.com>
 *
 * libtocc-python is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libtocc-python is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libtocc-python.  If not, see <http://www.gnu.org/licenses/>.
 */

extern "C"
{
#include <Python.h>
}

#include <libtocc/front_end/manager.h>


/*
 * Defines a Python class, for the Manager.
 */
typedef struct
{
  PyObject_HEAD
  libtocc::Manager* manager_instance;
} ManagerObject;


/*
 * __init__ method.
 */
static int manager_init(ManagerObject* self, PyObject* args, PyObject* kwargs)
{
  char* base_path;
  static char* kwlist[] = { "base_path", NULL };

  if (!PyArg_ParseTupleAndKeywords(args, kwargs,
                                   "s",
                                   kwlist,
                                   &base_path))
  {
    return -1;
  }

  self->manager_instance = new libtocc::Manager(base_path);

  return 0;
}

/*
 * Destructor.
 */
static void manager_object_dealloc(ManagerObject* self)
{
  if (self->manager_instance != NULL)
  {
    delete self->manager_instance;
    self->manager_instance = NULL;
  }
  PyObject_Del(self);
}

/*
 * Methods of Manager class.
 */
static PyMethodDef manager_methods[] =
{
    { NULL, NULL}
};

/*
 * Definition of Type.
 */
static PyTypeObject ManagerType =
{
  PyVarObject_HEAD_INIT(NULL, 0)
  "manager.Manager",
  sizeof(ManagerObject),
  0,
  /* Methods */
  (destructor)manager_object_dealloc,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  Py_TPFLAGS_DEFAULT,
  PyDoc_STR("The front end of the Tocc.\n\n"
            "To create an instance, call: Manager(base_path)\n"
            "@param base_path: Base path of where tocc files kept.\n"
            "  It should be an absolute path."),
  0,
  0,
  0,
  0,
  0,
  0,
  manager_methods,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  (initproc)manager_init,
};

/*
 * Definitions of Module.
 */
PyDoc_STRVAR(module_doc,
  "Defines Manager class.");

static struct PyModuleDef manager_module = {
    PyModuleDef_HEAD_INIT,
    "manager",
    module_doc,
    -1,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

/*
 * Module initialization func.
 */
extern "C"
PyMODINIT_FUNC PyInit_manager(void)
{
  PyObject* module = NULL;

  ManagerType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&ManagerType) < 0)
  {
    Py_XDECREF(module);
    return NULL;
  }

  module = PyModule_Create(&manager_module);
  if (module == NULL)
  {
    Py_XDECREF(module);
    return NULL;
  }

  PyModule_AddObject(module, "Manager", (PyObject*)&ManagerType);

  return module;
}
