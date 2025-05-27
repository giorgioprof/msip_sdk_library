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
/**
 * @brief Contains the JsonDelegate interface used to parse and produce JSON
 * data
 *
 * @file json_delegate.h
 */


#ifndef API_MIP_JSON_DELEGATE_H_
#define API_MIP_JSON_DELEGATE_H_

#include <memory>

#include "mip/delegate_response.h"
#include "mip/json_document.h"
#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

typedef mip::DelegateResponse<mip::JsonDocument> JsonResult;

class JsonDelegate {
public:
  /**
   * @brief Creates a blank json document with an Object as the root.
   * 
   * @return A delegate response which either contains a pointer to a blank json document with an Object as the root,
   * or an exception.
   */
  virtual mip::JsonResult CreateJsonObjectDocument() const = 0;

  /**
   * @brief parse value as json document.
   *
   * @return A delegate response which either contains a pointer to a json document of the parsed data,
   * or an exception.
   */
  virtual mip::JsonResult Parse(const std::string& value) const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~JsonDelegate() {};
  /** @endcond */
};

MIP_NAMESPACE_END

#endif // API_MIP_JSON_DELEGATE_H_
