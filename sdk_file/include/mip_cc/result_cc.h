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
 * @brief Defines success/error result codes
 * 
 * @file result_cc.h
 */

#ifndef API_MIP_RESULT_CC_H_
#define API_MIP_RESULT_CC_H_

#include <stdint.h>

#include "mip_cc/mip_macros_cc.h"

/**
 * @brief API success/failure result
 */
typedef enum {
  MIP_RESULT_SUCCESS = 0,

  // MIP C API errors
  MIP_RESULT_ERROR_UNKNOWN = 1,                       /**< Unknown error */
  MIP_RESULT_ERROR_INSUFFICIENT_BUFFER = 2,           /**< Buffer provided by application is too small */

  // MIP C++ exceptions
  MIP_RESULT_ERROR_BAD_INPUT = 3,                     /**< Application passed bad input */
  MIP_RESULT_ERROR_FILE_IO_ERROR = 4,                 /**< General file i/o error */
  MIP_RESULT_ERROR_NETWORK = 5,                       /**< General network error (e.g. unreachable service) */
  MIP_RESULT_ERROR_INTERNAL = 6,                      /**< Unexpected internal error */
  MIP_RESULT_ERROR_JUSTIFICATION_REQUIRED = 7,        /**< Justification should be provided to complete the action on the file. */
  MIP_RESULT_ERROR_NOT_SUPPORTED_OPERATION = 8,       /**< Opeation is not supported */
  MIP_RESULT_ERROR_PRIVILEGED_REQUIRED = 9,           /**< Cannot override privileged label when with standard method */
  MIP_RESULT_ERROR_ACCESS_DENIED = 10,                /**< User does not have rights to access the service */
  MIP_RESULT_ERROR_CONSENT_DENIED = 11,               /**< An operation that required consent from the user was not granted consent */
  MIP_RESULT_ERROR_NO_PERMISSIONS = 12,               /**< The user could not get access to the content (e.g. no permissions, content revoked) */
  MIP_RESULT_ERROR_NO_AUTH_TOKEN = 13,                /**< The user could not get access to the content due to an empty auth token */
  MIP_RESULT_ERROR_SERVICE_DISABLED = 14,             /**< The user could not get access to the content due to the service being disabled */
  MIP_RESULT_ERROR_PROXY_AUTH = 15,                   /**< Proxy authentication failed */
  MIP_RESULT_ERROR_NO_POLICY = 16,                    /**< No policy is configured for user/tenant */
  MIP_RESULT_ERROR_OPERATION_CANCELLED = 17,          /**< Operation cancelled */
  MIP_RESULT_ERROR_ADHOC_PROTECTION_REQUIRED = 18,    /**< Ad-hoc protection should be set to complete the action on the file */
  MIP_RESULT_ERROR_DEPRECATED_API = 19,               /**< Caller invoked a deprecated API */
  MIP_RESULT_ERROR_TEMPLATE_NOT_FOUND = 20,           /**< Template ID is not recognized */
  MIP_RESULT_ERROR_LABEL_NOT_FOUND = 21,              /**< Label ID is not recognized */
  MIP_RESULT_ERROR_LABEL_DISABLED = 22,               /**< Label is disabled or inactive */
  MIP_RESULT_ERROR_DOUBLE_KEY_DISABLED = 23,          /**< The double key feature has not been enabled */
  MIP_RESULT_ERROR_LICENSE_NOT_REGISTERED = 24,       /**< License not registered for tracking and revocation */
  MIP_RESULT_ERROR_CONTENT_FORMAT_NOT_SUPPORTED = 25, /**< Content format is not supported */
  MIP_RESULT_ERROR_TEMPLATE_ARCHIVED = 26,            /**< Template has been archived and is unavailable for protection */
  MIP_RESULT_ERROR_CUSTOMER_KEY_UNAVAILABLE = 27,     /**< Bring your own encryption key needed and unavailable */
} mip_cc_result;

#endif // API_MIP_RESULT_CC_H_
