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
 * @brief Error definition and functions
 * 
 * @file error_cc.h
 */

#ifndef API_MIP_ERROR_CC_H_
#define API_MIP_ERROR_CC_H_

#include "mip_cc/result_cc.h"

#define ERROR_STRING_BUFFER_SIZE 1024

typedef enum {
  MIP_NETWORK_ERROR_CATEGORY_UNKNOWN,                   /**< Unknown network failure */
  MIP_NETWORK_ERROR_CATEGORY_FAILURE_RESPONSE_CODE,     /**< HTTP response code indicates failure */
  MIP_NETWORK_ERROR_CATEGORY_BAD_RESPONSE,              /**< HTTP response could not be read */
  MIP_NETWORK_ERROR_CATEGORY_UNEXPECTED_RESPONSE,       /**< HTTP response completed but contained unexpected data */
  MIP_NETWORK_ERROR_CATEGORY_NO_CONNECTION,             /**< Failed to establish a connection */
  MIP_NETWORK_ERROR_CATEGORY_PROXY,                     /**< Proxy failure */
  MIP_NETWORK_ERROR_CATEGORY_SSL,                       /**< SSL failure */
  MIP_NETWORK_ERROR_CATEGORY_TIMEOUT,                   /**< Connection timed out */
  MIP_NETWORK_ERROR_CATEGORY_OFFLINE,                   /**< Operation requires network connectivity */
  MIP_NETWORK_ERROR_CATEGORY_THROTTLED,                 /**< HTTP operation failed due to server traffic throttling */
  MIP_NETWORK_ERROR_CATEGORY_CANCELLED,                 /**< HTTP operation has been cancelled by the application */
  MIP_NETWORK_ERROR_CATEGORY_FUNCTION_NOT_IMPLEMENTED,  /**< HTTP operation has been cancelled by the application */
  MIP_NETWORK_ERROR_CATEGORY_SERVICE_UNAVAILABLE,       /**< HTTP response code indicates service is unavailable */
} mip_cc_network_error_category;

typedef enum {
  MIP_SERVICE_DISABLED_ERROR_EXTENT_USER,     /**< Service is disabled for the user. */
  MIP_SERVICE_DISABLED_ERROR_EXTENT_DEVICE,   /**< Service is disabled for the device. */
  MIP_SERVICE_DISABLED_ERROR_EXTENT_PLATFORM, /**< Service is disabled for the platform. */
  MIP_SERVICE_DISABLED_ERROR_EXTENT_TENANT,   /**< Service is disabled for the tenant. */
} mip_cc_service_disabled_error_extent;

typedef enum {
  MIP_NO_PERMISSIONS_ERROR_UNKNOWN,     /**< Unknown no permissions failure */
  MIP_NO_PERMISSIONS_ERROR_USER_NOT_FOUND,   /**< User was not found */
  MIP_NO_PERMISSIONS_ERROR_ACCESS_DENIED,
  MIP_NO_PERMISSIONS_ERROR_ACCESS_EXPIRED,
  MIP_NO_PERMISSIONS_ERROR_INVALID_EMAIL,
  MIP_NO_PERMISSIONS_ERROR_UNKNOWN_TENANT,
  MIP_NO_PERMISSIONS_ERROR_NOT_OWNER,
  MIP_NO_PERMISSIONS_ERROR_NOT_PREMIUM_LICENSE_USER,
  MIP_NO_PERMISSIONS_ERROR_CLIENT_NOT_UPDATED,  /**< The SDK used by the client application does not support the requested operation. Update the SDK or check to make sure the correct arguments were provided */
} mip_cc_no_permissions_error_category;

/**
 * @brief Error information
 */
typedef struct {
  mip_cc_result result;
  char description[ERROR_STRING_BUFFER_SIZE];

  union {
    mip_cc_network_error_category networkError_Category;
    mip_cc_no_permissions_error_category noPermissionsError_Category;
  };
  
  // MIP_RESULT_ERROR_NETWORK details
  int32_t networkError_ResponseCode;

  // MIP_RESULT_ERROR_NO_PERMISSIONS details
  char noPermissionsError_Owner[ERROR_STRING_BUFFER_SIZE];
  char noPermissionsError_Referrer[ERROR_STRING_BUFFER_SIZE];

  // MIP_RESULT_ERROR_SERVICE_DISABLED details
  mip_cc_service_disabled_error_extent serviceDisabledError_Extent;
} mip_cc_error;

#endif  // API_MIP_ERROR_CC_H_