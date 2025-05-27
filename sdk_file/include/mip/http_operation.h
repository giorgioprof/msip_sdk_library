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
 * @brief Contains HttpOperation interface used by HttpDelegate
 * 
 * @file http_response.h
 */

#ifndef API_MIP_HTTP_OPERATION_H_
#define API_MIP_HTTP_OPERATION_H_

#include "mip/http_response.h"
#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief Interface that describes a single HTTP operation, implemented by client app when overriding HttpDelegate
 */
class HttpOperation {
public:
  /**
   * @brief Gets operation ID
   * 
   * @return Operation ID
   * 
   * @note The corresponding HttpRequest and HttpResponse will have the same ID
   */
  virtual const std::string& GetId() const = 0;

  /**
   * @brief Get response, if any
   *
   * @return Response
   */
  virtual std::shared_ptr<HttpResponse> GetResponse() = 0;

  /**
   * @brief Get cancellation status of operation
   *
   * @return Cancellation status
   */
  virtual bool IsCancelled() = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~HttpOperation() {}
   /** @endcond */
};

MIP_NAMESPACE_END
#endif  // API_MIP_HTTP_OPERATION_H_

