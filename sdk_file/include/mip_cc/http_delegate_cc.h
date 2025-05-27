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
 * @brief Defines HTTP callback functions
 * 
 * @file http_delegate_cc.h
 */

#ifndef API_MIP_HTTP_DELEGATE_CC_H_
#define API_MIP_HTTP_DELEGATE_CC_H_

#include <stddef.h>
#include <stdint.h>

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/result_cc.h"

/**
 * @brief HTTP request type
 */
typedef enum {
  HTTP_REQUEST_TYPE_GET = 0,  /**< HTTP GET */
  HTTP_REQUEST_TYPE_POST = 1, /**< HTTP POST */
} mip_cc_http_request_type;

/**
 * @brief Success/failure state of the HTTP operation
 */
typedef enum {
  HTTP_RESULT_OK = 0,      /**< HTTP operation was completed successfull */
  HTTP_RESULT_FAILURE = 1, /**< HTTP operation failed (e.g. timeout, network failure, etc.) */
} mip_cc_http_result;

/**
 * @brief HTTP request/response header
 */
typedef struct {
  const char* name;  /**< Header name/key */
  const char* value; /**< Header value */
} mip_cc_http_header;

/**
 * @brief HTTP request
 */
typedef struct {
  const char* id;                    /**< Unique request ID--correlated with the same property in mip_cc_http_response */
  mip_cc_http_request_type type;     /**< HTTP request type (e.g. GET vs. POST) */
  const char* url;                   /**< HTTP request URL */
  int64_t bodySize;                  /**< Size of HTTP request body in bytes */
  const uint8_t* body;               /**< Buffer containg HTTP request body */
  int64_t headersCount;              /**< Number of HTTP request headers */
  const mip_cc_http_header* headers; /**< Buffer containing HTTP request headers */
} mip_cc_http_request;

/**
 * @brief HTTP response
 */
typedef struct {
  const char* id;                    /**< Unique request ID--correlated with the same property in mip_cc_http_request */
  int32_t statusCode;                /**< HTTP response status code */
  int64_t bodySize;                  /**< Size of HTTP response body in bytes */
  const uint8_t* body;               /**< Buffer containg HTTP response body */
  int64_t headersCount;              /**< Number of HTTP response headers */
  const mip_cc_http_header* headers; /**< Buffer containing HTTP response headers */
} mip_cc_http_response;

/**
 * @brief Callback function definition for issuing an HTTP request
 * 
 * @param request The HTTP request to be performed by the application
 * @param context The same opaque context passed to MIP API call that resulted in this HTTP request
 */
MIP_CC_CALLBACK(mip_cc_http_send_callback_fn /*typename*/,
    void /*return*/,
    const mip_cc_http_request* /*request*/,
    const void* /*context*/);

/**
 * @brief Callback function definition for cancelling an HTTP request
 * 
 * @param requestId The ID of the HTTP request to be cancelled
 */
MIP_CC_CALLBACK(mip_cc_http_cancel_callback_fn /*typename*/,
    void /*return*/,
    const char* /*requestId*/);

typedef mip_cc_handle* mip_cc_http_delegate;

/**
 * @brief Creates an HTTP delegate which can be used to override MIP's default HTTP stack
 * 
 * @param sendCallback Function pointer for issuing HTTP requests
 * @param cancelCallback Function pointer for cancelling HTTP requests
 * @param httpDelegate [Output] Handle to HTTP delegate object
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateHttpDelegate(
    const mip_cc_http_send_callback_fn sendCallback,
    const mip_cc_http_cancel_callback_fn cancelCallback,
    mip_cc_http_delegate* httpDelegate,
    mip_cc_error* errorInfo);

/**
 * @brief Notifies an HTTP delegate that an HTTP response is ready
 * 
 * @param httpDelegate Handle to HTTP delegate object
 * @param requestId ID of HTTP request associated with this operation
 * @param result Success/failure state of operation
 * @param response The HTTP response if operation succeeded, else nullptr
 * 
 * @note This function must be called by the application when an HTTP operation has completed. The ID of the HTTP
 *       response must match the ID of the HTTP request to allow MIP to correlate a response with its request
 */
MIP_CC_API(void) MIP_CC_NotifyHttpDelegateResponse(
    const mip_cc_http_delegate httpDelegate,
    const char* requestId,
    const mip_cc_http_result result,
    const mip_cc_http_response* response);

/**
 * @brief Release resources associated with an HTTP delegate handle
 * 
 * @param httpDelegate HTTP delegate to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseHttpDelegate(mip_cc_http_delegate httpDelegate);

#endif // API_MIP_HTTP_DELEGATE_CC_H_
