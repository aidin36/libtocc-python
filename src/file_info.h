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


#define FILE_INFO_CREATE_NUM 0
#define FILE_INFO_COLLECTION_CREATE_NUM 1
#define FILE_INFO_API_POINTERS 2

#ifdef FILE_INFO_MODULE

// This section is used when compiling file_info.cpp
static PyObject* create_python_file_info(const libtocc::FileInfo& file_info);
PyObject* create_python_file_info_list(
      libtocc::FileInfoCollection& file_info_collection);

#else

// This section is used when someone else uses this module's API.
extern "C"
{
static void** FileInfoAPI;
}

#define create_python_file_info \
  (*(PyObject*) (*)(const libtocc::FileInfo& file_info) FileInfoAPI[FILE_INFO_CREATE_NUM])
#define create_python_file_info_list \
  (*(PyObject*) (*)(libtocc::FileInfoCollection& file_info_collection) FileInfoAPI[FILE_INFO_COLLECTION_CREATE_NUM])

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
