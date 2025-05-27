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
 * @brief Contains HttpResponse interface used by HttpDelegate
 * 
 * @file http_response.h
 */

#ifndef API_MIP_HTTP_RESPONSE_H_
#define API_MIP_HTTP_RESPONSE_H_

#include <vector>

#include "mip/common_types.h"
#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief Interface that describes a single HTTP response, implemented by client app when overriding HttpDelegate
 */
class HttpResponse {
public:
  /**
   * @brief Gets response ID
   * 
   * @return Response ID
   * 
   * @note The corresponding HttpRequest will have had the same ID
   */
  virtual const std::string& GetId() const = 0;

  /**
   * @brief Get response status code
   *
   * @return Status code
   */
  virtual int32_t GetStatusCode() const = 0;

  /**
   * @brief Get request body
   *
   * @return Request body
   */
  virtual const std::vector<uint8_t>& GetBody() const = 0;

  /**
   * @brief Get request headers
   *
   * @return Request headers
   */
  virtual const std::map<std::string, std::string, CaseInsensitiveComparator>& GetHeaders() const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~HttpResponse() {}
   /** @endcond */
};

MIP_NAMESPACE_END
#endif  // API_MIP_HTTP_RESPONSE_H_

