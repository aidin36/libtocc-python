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

  libtocc::FileInfoCollection* file_ids_to_info_collection(PyObject* ids_list)
  {
    // Copying the list to a FileInfoCollection.
    libtocc::FileInfoCollection* collection =
        new libtocc::FileInfoCollection(PyList_Size(ids_list));

    for (int i = 0; i < PyList_Size(ids_list); i++)
    {
      PyObject* id_bytes = PyUnicode_AsEncodedString(
                              PyList_GetItem(ids_list, i),
                              "utf-8",
                              "Could not encode file ID.");
      if (id_bytes == NULL)
      {
        return NULL;
      }
      libtocc_python::PyObjectHolder id_bytes_holder(id_bytes);

      char* id_str = PyBytes_AsString(id_bytes);
      if (id_str == NULL)
      {
        return NULL;
      }

      libtocc::FileInfo file_info(id_str);
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
      PyObject* tag_bytes = PyUnicode_AsEncodedString(
                              PyList_GetItem(tags_list, i),
                              "utf-8",
                              "Could not encode tag.");
      if (tag_bytes == NULL)
      {
        return NULL;
      }
      libtocc_python::PyObjectHolder tag_bytes_holder(tag_bytes);

      char* tag_str = PyBytes_AsString(tag_bytes);
      if (tag_str == NULL)
      {
        return NULL;
      }
      tags_collection->add_tag(tag_str);
    }

    return tags_collection;
  }
}
