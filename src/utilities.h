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

#ifndef LIBTOCC_PYTHON_UTILITIES_H_INCLUDED
#define LIBTOCC_PYTHON_UTILITIES_H_INCLUDED

extern "C"
{
#include <Python.h>
}

#include <libtocc/front_end/file_info.h>


namespace libtocc_python
{

  /*
   * Holds a PyObject pointer, and decrease it's reference at
   * destruction time.
   * This class is used to automate the object releasing mechanism.
   */
  class PyObjectHolder
  {
  public:
    /*
     * Holds an object without increasing its reference count.
     */
    PyObjectHolder(PyObject* object);

    /*
     * Holds an object and increase its reference count at construct time.
     */
    PyObjectHolder(PyObject* object, bool inc_ref);

    ~PyObjectHolder();

  private:
    PyObject* object;
  };

  /*
   * Converts a PyUnicode object to a char*.
   *
   * If any error happens, it sets the PyErr and returns NULL.
   */
  char* python_unicode_to_char(PyObject* unicode_object);

  /*
   * Converts a Python's list of str (list of file IDs) to a collection
   * of File Infos.
   * Each element of the collection have only its ID set.
   *
   * Note that you should delete the return pointer when you finished with it.
   */
  libtocc::FileInfoCollection* file_ids_to_info_collection(PyObject* ids_list);

  /*
   * Converts a Python's list of str (list of Tags) to a collection of Tags.
   *
   * Note that you should delete the return pointer when you finished with it.
   */
  libtocc::TagsCollection* tags_list_to_collection(PyObject* tags_list);
}

#endif /* LIBTOCC_PYTHON_UTILITIES_H_INCLUDED */
