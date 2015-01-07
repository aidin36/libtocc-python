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

#ifndef LIBTOCC_PYTHON_FILE_INFO_H_INCLUDED
#define LIBTOCC_PYTHON_FILE_INFO_H_INCLUDED

/*
 * Header file for `file_info' module.
 */

extern "C"
{
#include "Python.h"
}

#include <libtocc/front_end/file_info.h>


#define FILE_INFO_IS_NUM 0
#define FILE_INFO_CREATE_NUM 1
#define FILE_INFO_CREATE_LIST_NUM 2
#define FILE_INFO_CREATE_FILE_IDS_NUM 3
#define FILE_INFO_GET_NUM 4
#define FILE_INFO_API_POINTERS 5


#ifdef FILE_INFO_MODULE

// This section is used when compiling file_info.cpp
bool is_python_file_info(PyObject* object);
PyObject* create_python_file_info(const libtocc::FileInfo& file_info);
PyObject* create_python_file_info_list(
      libtocc::FileInfoCollection& file_info_collection);
bool create_file_ids_array(PyObject* files_list, char** out_array);
libtocc::FileInfo* python_file_info_get(PyObject* file_info);

#else

// This section is used when someone else uses this module's API.
extern "C"
{
static void** FileInfoAPI;
}

/*
 * Checks if the specified Python Object is a FileInfo.
 */
#define is_python_file_info \
    (*(PyObject* (*)(PyObject* object)) FileInfoAPI[FILE_INFO_IS_NUM])

/*
 * Creates a Python Object from the specified FileInfo.
 */
#define create_python_file_info \
  (*(PyObject* (*)(const libtocc::FileInfo& file_info)) FileInfoAPI[FILE_INFO_CREATE_NUM])

/*
 * Creates a list of Python objects from the specified FileInfoCollection.
 */
#define create_python_file_info_list \
  (*(PyObject* (*)(libtocc::FileInfoCollection& file_info_collection)) FileInfoAPI[FILE_INFO_CREATE_LIST_NUM])

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
#define create_file_ids_array \
  (*(bool (*)(PyObject* files_list, char** out_array)) FileInfoAPI[FILE_INFO_CREATE_FILE_IDS_NUM])

/*
 * Returns the internal pointer for the specified FileInfoObject.
 * The pointer points to the libtocc::FileInfo kept inside the
 * PyObject.
 */
#define python_file_info_get \
  (*(libtocc::FileInfo* (*)(PyObject* file_info)) FileInfoAPI[FILE_INFO_GET_NUM])

extern "C"
{
/*
 * Returns 0 on success, -1 on error.
 * It will set an exception if there was error.
 */
static int import_file_info(void)
{
  FileInfoAPI = (void**)PyCapsule_Import("file_info._C_API", 0);
  if (FileInfoAPI == NULL)
  {
    return -1;
  }
  return 0;
}
}

#endif /* FILE_INFO_MODULE */

#endif /* LIBTOCC_PYTHON_FILE_INFO_H_INCLUDED */
