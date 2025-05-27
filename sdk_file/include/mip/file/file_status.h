/*
 *
 * Copyright (c) Microsoft Corporation.
 * All rights reserved.
 *
 * This code is licensed under the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files(the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions :
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef API_MIP_FILE_FILE_STATUS_H_
#define API_MIP_FILE_FILE_STATUS_H_

#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief Interface for all file status functions.
 */
class FileStatus {
public:
  /**		
   * @brief Checks if a file is protected or not.				
   */	
  virtual bool IsProtected() = 0;

  /**
   * @brief Checks if a file is labeled or not.
   */
  virtual bool IsLabeled() = 0;

  /**		
   * @brief Checks if the unprotected container file contains protected objects. 
   *        Returns false if this file is protected or not a container file			
   */	
  virtual bool ContainsProtectedObjects() = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~FileStatus() {}
  /** @endcond */

protected:
  /** @cond DOXYGEN_HIDE */
  FileStatus() {}
  /** @endcond */
};

MIP_NAMESPACE_END

#endif // API_MIP_FILE_FILE_Status_H_
