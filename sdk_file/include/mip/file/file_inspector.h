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

#include <memory>
#include <vector> 

#include "mip/mip_namespace.h"
#include "mip/stream.h"

#ifndef API_MIP_FILE_FILE_INSPECTOR_H_
#define API_MIP_FILE_FILE_INSPECTOR_H_

MIP_NAMESPACE_BEGIN

/**
 * @brief Inspector type correlating to supported file types.
 */
enum class InspectorType : unsigned int {
  Unknown,  /**<  Unkown file inspector. */
  Msg       /**<  Msg style file inspector, rpmsg / msg based. */
};

class FileInspector {
public:
  /**
   * @brief Get file types,.
   *
   * @return InspectorType.
   */
  virtual InspectorType GetInspectorType() const = 0;
  /**
   * @brief Get the file stream.
   *
   * @return a shared ptr to the file stream.
   */
  virtual std::shared_ptr<Stream> GetFileStream() const = 0;
  /** @cond DOXYGEN_HIDE */
  virtual ~FileInspector() {}
  /** @endcond */
};

MIP_NAMESPACE_END

#endif  // API_MIP_FILE_FILE_INSPECTOR_H_