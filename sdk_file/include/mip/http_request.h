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
 * @brief Contains HttpRequest interface used by HttpDelegate
 * 
 * @file http_request.h
 */

#ifndef API_MIP_HTTP_REQUEST_H_
#define API_MIP_HTTP_REQUEST_H_

#include <vector>

#include "mip/common_types.h"
#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief HTTP request type
 */
enum class HttpRequestType {
  Get, /**< GET */
  Post, /**< POST */
};

/**
 * @brief Minimum TLS version
 */
enum class TransportLayerSecurityMinimumVersion {
  None = 0,
  TLS1_2 = 1, /**< TLS version 1.2 **/
  TLS1_3 = 2, /**< TLS version 1.3 **/
};


/**
 * @brief Interface that describes a single HTTP request
 */
class HttpRequest {
public:
  /**
   * @brief Gets request ID
   * 
   * @return Request ID
   * 
   * @note The corresponding HttpResponse will have the same ID
   */
  virtual const std::string& GetId() const = 0;

  /**
   * @brief Get request type
   *
   * @return Request type
   */
  virtual HttpRequestType GetRequestType() const = 0;

   /**
   * @brief Get request url
   *
   * @return Request url
   */
  virtual const std::string& GetUrl() const = 0;

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

  /**
   * @brief Get the minimum required TLS version for this request.
   * 
   * @return The minimum required TLS version the Http transport provider should be secured with.
   */
  virtual TransportLayerSecurityMinimumVersion GetTransportLayerSecurityMinimumVersion() const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~HttpRequest() {}
   /** @endcond */
};

MIP_NAMESPACE_END
#endif  // API_MIP_HTTP_REQUEST_H_

