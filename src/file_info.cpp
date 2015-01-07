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


#define FILE_INFO_MODULE
#include "file_info.h"
#include "utilities.h"

#include <stdlib.h>


/*
 * Defines a Python class, for the FileInfo.
 */
typedef struct
{
  PyObject_HEAD
  libtocc::FileInfo* file_info_instance;
} FileInfoObject;

/*
 * __init__ method.
 */
static int file_info_init(FileInfoObject* self, PyObject* args, PyObject* kwargs)
{
  char* file_id;
  static char* kwlist[] = { "file_id", NULL };

  if (!PyArg_ParseTupleAndKeywords(args, kwargs,
                                   "s",
                                   kwlist,
                                   &file_id))
  {
    return -1;
  }

  self->file_info_instance = new libtocc::FileInfo(file_id);

  return 0;
}

/*
 * Destructor.
 */
static void file_info_object_dealloc(FileInfoObject* self)
{
  if (self->file_info_instance != NULL)
  {
    delete self->file_info_instance;
    self->file_info_instance = NULL;
  }
  PyObject_Del(self);
}

static PyObject* file_info_get_id(FileInfoObject* self)
{
  const char* file_id = self->file_info_instance->get_id();

  return PyUnicode_FromString(file_id);
}

static PyObject* file_info_get_title(FileInfoObject* self)
{
  const char* file_title = self->file_info_instance->get_title();

  return PyUnicode_FromString(file_title);
}

static PyObject* file_info_get_traditional_path(FileInfoObject* self)
{
  const char* traditional_path = self->file_info_instance->get_traditional_path();

  return PyUnicode_FromString(traditional_path);
}

static PyObject* file_info_get_physical_path(FileInfoObject* self)
{
  const char* physical_path = self->file_info_instance->get_physical_path();

  return PyUnicode_FromString(physical_path);
}

static PyObject* file_info_get_tags(FileInfoObject* self)
{
  libtocc::TagsCollection tags_collection = self->file_info_instance->get_tags();

  if (tags_collection.size() == 0)
  {
    // Returning an empty list.
    return PyList_New(0);
  }

  // Converting tags collection into python list.
  PyObject* tags_list = PyList_New(tags_collection.size());
  if (tags_list == NULL)
  {
    return NULL;
  }

  libtocc_python::PyObjectHolder(tags_list, true);

  int list_index = 0;
  libtocc::TagsCollection::Iterator iterator(&tags_collection);
  for (; !iterator.is_finished(); iterator.next())
  {
    // I used SET_ITEM instead of SetItem because it's faster for newly
    // created lists.
    PyList_SET_ITEM(tags_list, list_index, PyUnicode_FromString(iterator.get()));
    list_index++;
  }

  return tags_list;
}

/*
 * Methods of FileInfo class.
 */
static PyMethodDef file_info_methods[] =
{
  {
    "get_id", (PyCFunction)file_info_get_id, METH_NOARGS,
    PyDoc_STR("Returns ID of the file.\n\n@return: str")
  },
  {
    "get_title", (PyCFunction)file_info_get_title, METH_NOARGS,
    PyDoc_STR("Returns Title of the file.\n\n@return: str")
  },
  {
    "get_traditional_path", (PyCFunction)file_info_get_traditional_path, METH_NOARGS,
    PyDoc_STR("Returns Traditional Path of the file.\n\n@return: str")
  },
  {
    "get_physical_path", (PyCFunction)file_info_get_physical_path, METH_NOARGS,
    PyDoc_STR("Returns Physical Path of the file.\n\n@return: str")
  },
  {
    "get_tags", (PyCFunction)file_info_get_tags, METH_NOARGS,
    PyDoc_STR("Returns list of tags assigned to this file.\n\n@return: list of str")
  },
  {NULL, NULL}
};

/*
 * Definition of Type.
 */
static PyTypeObject FileInfoType =
{
  PyVarObject_HEAD_INIT(NULL, 0)
  "file_info.FileInfo",
  sizeof(FileInfoObject),
  0,
  /* Methods */
  (destructor)file_info_object_dealloc,
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
  PyDoc_STR("Keeps information of a file.\n"
            "You shouldn't create an instance of this class directly."),
  0,
  0,
  0,
  0,
  0,
  0,
  file_info_methods,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  (initproc)file_info_init,
};

/*
 * Definitions of Module.
 */
PyDoc_STRVAR(module_doc,
  "Defines FileInfo class.");

static struct PyModuleDef file_info_module = {
    PyModuleDef_HEAD_INIT,
    "file_info",
    module_doc,
    -1,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

/*
 * Checks if the specified Python Object is a FileInfo.
 */
bool is_python_file_info(PyObject* object)
{
  return (Py_TYPE(object) == &FileInfoType);
}

/*
 * Creates a Python Object from the specified FileInfo.
 */
PyObject* create_python_file_info(const libtocc::FileInfo& file_info)
{
  FileInfoObject* self;
  self = PyObject_New(FileInfoObject, &FileInfoType);
  if (self == NULL)
  {
    return NULL;
  }

  self->file_info_instance = new libtocc::FileInfo(file_info);

  return (PyObject*)self;
}

/*
 * Creates a list of Python objects from the specified FileInfoCollection.
 */
PyObject* create_python_file_info_list(
    libtocc::FileInfoCollection& file_info_collection)
{
  if (file_info_collection.size() == 0)
  {
    // Returning an empty list.
    return PyList_New(0);
  }

  // Converting collection into list.
  PyObject* file_info_list = PyList_New(file_info_collection.size());

  Py_INCREF(file_info_list);

  int list_index = 0;
  libtocc::FileInfoCollection::Iterator iterator(&file_info_collection);
  for (; iterator.is_finished(); iterator.next())
  {
    PyList_SET_ITEM(file_info_list, list_index,
                    create_python_file_info(*iterator.get()));
    list_index++;
  }

  Py_DECREF(file_info_list);
  return file_info_list;
}

/*
 * Creates a list of file IDs.
 *
 * @param files_list: can be a list of Strings, or a list of FileInfo, or a list
 *   of both.
 * @param out_array: Array to fill. This method allocates memory for
 *   specified the pointer.
 *
 * @return: false if any errors happen.
 *   It sets the Python Error if error happen.
 */
bool create_file_ids_array(PyObject* files_list, char** out_array)
{
  //out_array = malloc(PyList_Size(files_list) * sizeof(char*));
  out_array = new char*[PyList_Size(files_list)];

  for (int i = 0; i < PyList_Size(files_list); i++)
  {
    PyObject* list_item = PyList_GetItem(files_list, i);

    if (is_python_file_info(list_item))
    {
      out_array[i] = (char*)
          ((FileInfoObject*)list_item)->file_info_instance->get_id();
    }
    else if (PyUnicode_Check(list_item))
    {
      out_array[i] = libtocc_python::python_unicode_to_char(list_item);
    }
    else
    {
      PyErr_Format(PyExc_TypeError,
                   "The argument should be a list of str or FileInfo."
                   "But something [%s] found!",
                   Py_TYPE(list_item)->tp_name);
      delete[] out_array;
      return false;
    }
  }

  return true;
}

/*
 * Returns the internal pointer for the specified FileInfoObject.
 * The pointer points to the libtocc::FileInfo kept inside the
 * PyObject.
 */
libtocc::FileInfo* python_file_info_get(PyObject* file_info)
{
  if (!is_python_file_info(file_info))
  {
    PyErr_SetString(PyExc_TypeError,
        "Specified object is not a FileInfoObject.");
  }

  return ((FileInfoObject*)file_info)->file_info_instance;
}

/*
 * Module initialization func.
 */
extern "C"
PyMODINIT_FUNC PyInit_file_info(void)
{
  PyObject* module = NULL;
  static void* FileInfoAPI[FILE_INFO_API_POINTERS];
  PyObject* c_api_object;

  // Creating module.
  FileInfoType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&FileInfoType) < 0)
  {
    Py_XDECREF(module);
    return NULL;
  }

  // Creating FileInfo type and adding it to module.
  module = PyModule_Create(&file_info_module);
  if (module == NULL)
  {
    Py_XDECREF(module);
    return NULL;
  }

  PyModule_AddObject(module, "FileInfo", (PyObject*)&FileInfoType);

  // Creating C API and adding it to module.

  // API funcs.
  FileInfoAPI[FILE_INFO_IS_NUM] = (void*)is_python_file_info;
  FileInfoAPI[FILE_INFO_CREATE_NUM] = (void*)create_python_file_info;
  FileInfoAPI[FILE_INFO_CREATE_LIST_NUM] = (void*)create_python_file_info_list;
  FileInfoAPI[FILE_INFO_CREATE_FILE_IDS_NUM] = (void*)create_file_ids_array;
  FileInfoAPI[FILE_INFO_GET_NUM] = (void*)python_file_info_get;

  // Capsule object.
  c_api_object = PyCapsule_New((void*)FileInfoAPI, "file_info._C_API", NULL);
  if (c_api_object != NULL)
  {
    PyModule_AddObject(module, "_C_API", c_api_object);
  }

  return module;
}
