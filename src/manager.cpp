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

#include "utilities.h"
// `file_info' module.
#include "file_info.h"

#include <libtocc/front_end/manager.h>
#include <libtocc/common/base_exception.h>


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

static PyObject* manager_initialize(ManagerObject* self)
{
  try
  {
    self->manager_instance->initialize();
    Py_RETURN_NONE;
  }
  catch (libtocc::BaseException& error)
  {
    // TODO
    return NULL;
  }
}

static PyObject* manager_get_file_info(ManagerObject* self, PyObject* args)
{
  char* file_id;

  if (!PyArg_ParseTuple(args, "s", &file_id))
  {
    return NULL;
  }

  try
  {
    return create_python_file_info(self->manager_instance->get_file_info(file_id));
  }
  catch (libtocc::BaseException& error)
  {
    // TODO
    return NULL;
  }
}

static PyObject* manager_get_file_by_traditional_path(ManagerObject* self, PyObject* args)
{
  char* traditional_path;

  if (!PyArg_ParseTuple(args, "s", &traditional_path))
  {
    return NULL;
  }

  try
  {
    return create_python_file_info(
        self->manager_instance->get_file_by_traditional_path(traditional_path));
  }
  catch (libtocc::BaseException& error)
  {
    // TODO
    return NULL;
  }
}

static PyObject* manager_import_file(ManagerObject* self, PyObject* args, PyObject* kwargs)
{
  char* source_path;
  char* title;
  char* traditional_path;
  PyObject* tags_list;

  static char* kwlist[] = { "source_path", "title", "traditional_path",
                             "tags", NULL };

  if (!PyArg_ParseTupleAndKeywords(args, kwargs,
                                   "s|ssO",
                                   kwlist,
                                   &source_path,
                                   &title,
                                   &traditional_path,
                                   &tags_list))
  {
    return NULL;
  }

  libtocc_python::PyObjectHolder(tags_list, true);

  libtocc::TagsCollection* tags_collection = NULL;

  try
  {
    if (tags_list == Py_None)
    {
      // Using the other overload without tags.
      libtocc::FileInfo result =
          self->manager_instance->import_file(source_path, title, traditional_path);
      return create_python_file_info(result);
    }
    else
    {
      if (!PyList_Check(tags_list))
      {
        // TODO: Raise exception.
      }

      // Creating a tags collection from the list of tags.
      tags_collection = libtocc_python::tags_list_to_collection(tags_list);

      // Importing file.
      libtocc::FileInfo result =
          self->manager_instance->import_file(source_path, title,
                                              traditional_path,
                                              tags_collection);

      delete tags_collection;
      tags_collection = NULL;

      return create_python_file_info(result);
    }
  }
  catch (libtocc::BaseException& error)
  {
    // TODO
    if (tags_collection != NULL)
    {
      delete tags_collection;
    }
  }
}

static PyObject* manager_remove_file(ManagerObject* self, PyObject* args)
{
  char* file_id;

  if (!PyArg_ParseTuple(args, "s", file_id))
  {
    return NULL;
  }

  try
  {
    self->manager_instance->remove_file(file_id);

    Py_RETURN_NONE;
  }
  catch(libtocc::BaseException& error)
  {
    // TODO
  }
}

static PyObject* manager_remove_files(ManagerObject* self, PyObject* args)
{
  PyObject* files_list;

  if (!PyArg_ParseTuple(args, "O", files_list))
  {
    return NULL;
  }

  if (!PyList_Check(files_list))
  {
    // TODO: set exception.
    return NULL;
  }

  libtocc::FileInfoCollection* file_infos = NULL;

  try
  {
    file_infos = libtocc_python::file_ids_to_info_collection(files_list);

    // Calling manager by the created collection.
    self->manager_instance->remove_files(*file_infos);

    delete file_infos;

    Py_RETURN_NONE;
  }
  catch (libtocc::BaseException& error)
  {
    // TODO
    if (file_infos != NULL)
    {
      delete file_infos;
    }

  }
}

static PyObject* manager_assign_tags(ManagerObject* self, PyObject* args)
{
  PyObject* files_list;
  PyObject* tags_list;

  if (!PyArg_ParseTuple(args, "OO", files_list, tags_list))
  {
    return NULL;
  }

  if (!PyList_Check(files_list))
  {
    // TODO: set exception.
    return NULL;
  }
  if (!PyList_Check(tags_list))
  {
    // TODO: set exception.
    return NULL;
  }

  libtocc::FileInfoCollection* file_infos;
  libtocc::TagsCollection* tags;

  try
  {
    file_infos = libtocc_python::file_ids_to_info_collection(files_list);
    tags = libtocc_python::tags_list_to_collection(tags_list);

    self->manager_instance->assign_tags(*file_infos, tags);

    delete file_infos;
    delete tags;

    Py_RETURN_NONE;
  }
  catch (libtocc::BaseException& error)
  {
    // TODO
    if (file_infos != NULL)
    {
      delete file_infos;
    }
    if (tags != NULL)
    {
      delete tags;
    }
  }

}

static PyObject* manager_unassign_tags(ManagerObject* self, PyObject* args)
{
  PyObject* files_list;
  PyObject* tags_list;

  if (!PyArg_ParseTuple(args, "OO", files_list, tags_list))
  {
    return NULL;
  }

  if (!PyList_Check(files_list))
  {
    // TODO: set exception.
    return NULL;
  }
  if (!PyList_Check(tags_list))
  {
    // TODO: set exception.
    return NULL;
  }

  libtocc::FileInfoCollection* file_infos;
  libtocc::TagsCollection* tags;

  try
  {
    file_infos = libtocc_python::file_ids_to_info_collection(files_list);
    tags = libtocc_python::tags_list_to_collection(tags_list);

    self->manager_instance->unassign_tags(*file_infos, tags);

    delete file_infos;
    delete tags;

    Py_RETURN_NONE;
  }
  catch (libtocc::BaseException& error)
  {
    // TODO
    if (file_infos != NULL)
    {
      delete file_infos;
    }
    if (tags != NULL)
    {
      delete tags;
    }
  }

}

/*
 * Methods of Manager class.
 */
static PyMethodDef manager_methods[] =
{
    {
      "initialize", (PyCFunction)manager_initialize, METH_NOARGS,
      PyDoc_STR("Initializes the specified base path.\n"
                "This method should be called once in a new base path. Or else, you\n"
                "can't work with that path.\n"
                "Note that this method should be called once for every path.\n"
                "\n"
                "@raise DatabaseInitializationError: If path was already initialized,\n"
                 "  or there was something wrong with the path.")
    },
    {
      "get_file_info", (PyCFunction)manager_get_file_info, METH_VARARGS,
      PyDoc_STR("Gets information of a file.\n"
                "\n"
                "@param file_id: (str) ID of the file to get.\n"
                "\n"
                "@return: FileInfo\n"
                "\n"
                "@throw DatabaseScriptLogicalError: if file not found.\n")
    },
    {
      "get_file_by_traditional_path", (PyCFunction)manager_get_file_by_traditional_path, METH_VARARGS,
      PyDoc_STR("Gets information of the file, that its traditional_path matches\n"
                "with the specified one.\n"
                "\n"
                "@param traditional_path: Path of the file to get.\n"
                "\n"
                "@return: Information of the file.\n"
                "\n"
                "@throw DatabaseScriptLogicalError: if file not found.")
    },
    {
      "import_file", (PyCFunction)manager_import_file, METH_VARARGS | METH_KEYWORDS,
      PyDoc_STR("Imports a file from the path to the Tocc managed file system.\n"
                "\n"
                "@param source_path: (str) Path to the source file.\n"
                "@keyword title: (str) title of the file.\n"
                "@keyword traditional_path: (str) traditional path of the file.\n"
                "  (Can be empty string.)\n"
                "@keyword tags: (list of str) Tags to assign to the file.\n"
                "\n"
                "@note: If you don't want to set title or traditional path,\n"
                "  pass empty string (\"\"), not None.\n"
                "\n"
                "@return: Information of the newly created file.")
    },
    {
      "remove_file", (PyCFunction)manager_remove_file, METH_VARARGS,
      PyDoc_STR("Deletes the specified file, both from database and\n"
                "file system.\n"
                "\n"
                "@param file_id: (str) ID of the file to delete.")
    },
    {
      "remove_files", (PyCFunction)manager_remove_files, METH_VARARGS,
      PyDoc_STR("Deletes a list of files, both from database and\n"
                "file system.\n"
                "\n"
                "@param file_ids: (list of str) IDs of files to delete.")
    },
    {
      "assign_tags", (PyCFunction)manager_assign_tags, METH_VARARGS,
      PyDoc_STR("Assign list of tags to a list of files.\n"
                "It assigns all tags to each file.\n"
                "\n"
                "@param file_ids: (list of str) IDs of files to assign\n"
                "  tags to.\n"
                "@param tags: (list of str) Tags to assign.")
    },
    {
      "unassign_tags", (PyCFunction)manager_unassign_tags, METH_VARARGS,
      PyDoc_STR("Unassign list of tags from a list of files.\n"
                "It unassign each tags from all of the files.\n"
                "Raises exception if specified files not found.\n"
                "\n"
                "@param file_ids: (list of str) IDs of files to unassign\n"
                "  their tags.\n"
                "@param tags: (list of str) Tags to unassign.")
    },
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

  // Importing C API of `file_info' module.
  if (import_file_info() < 0)
  {
    Py_XDECREF(module);
    return NULL;
  }

  return module;
}
