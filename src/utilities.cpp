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

#include "utilities.h"


namespace libtocc_python
{

  PyObjectHolder::PyObjectHolder(PyObject* object)
  {
    this->object = object;
  }

  PyObjectHolder::PyObjectHolder(PyObject* object, bool inc_ref)
  {
    this->object = object;
    Py_XINCREF(object);
  }

  PyObjectHolder::~PyObjectHolder()
  {
    Py_XDECREF(this->object);
  }

  char* python_unicode_to_char(PyObject* unicode_object)
  {
    PyObject* bytes_object =
        PyUnicode_AsEncodedString(unicode_object,
                                  "utf-8",
                                  "Could not encode the Unicode into UTF-8");
    if (bytes_object == NULL)
    {
      return NULL;
    }

    libtocc_python::PyObjectHolder bytes_holder(bytes_object);

    char* result = PyBytes_AsString(bytes_object);

    // `result' will be NULL, if any error happen.
    return result;
  }

  libtocc::FileInfoCollection* file_ids_to_info_collection(PyObject* ids_list)
  {
    // Copying the list to a FileInfoCollection.
    libtocc::FileInfoCollection* collection =
        new libtocc::FileInfoCollection(PyList_Size(ids_list));

    for (int i = 0; i < PyList_Size(ids_list); i++)
    {
      PyObject* item = PyList_GetItem(ids_list, i);
      PyObjectHolder item_holder(item);
      libtocc::FileInfo file_info(python_unicode_to_char(item));
      collection->add_file_info(file_info);
    }

    return collection;
  }

  libtocc::TagsCollection* tags_list_to_collection(PyObject* tags_list)
  {
    int tags_size = PyList_Size(tags_list);
    libtocc::TagsCollection* tags_collection = new
        libtocc::TagsCollection(tags_size);

    for (int i = 0; i < tags_size; i++)
    {
      PyObject* item = PyList_GetItem(tags_list, i);
      PyObjectHolder item_holder(item);
      tags_collection->add_tag(python_unicode_to_char(item));
    }

    return tags_collection;
  }
}
