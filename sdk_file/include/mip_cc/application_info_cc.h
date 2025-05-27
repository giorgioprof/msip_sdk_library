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
 * @brief ApplicationInfo definition
 * 
 * @file application_info_cc.h
 */

#ifndef API_MIP_APPLICATION_INFO_CC_H_
#define API_MIP_APPLICATION_INFO_CC_H_

#include "mip_cc/mip_macros_cc.h"

/**
 * @brief A struct that includes application specific information 
 */
typedef struct {
  const char* applicationId;      /**< Application identifier as set in the AAD portal, (Should be a GUID without brackets). */
  const char* applicationName;    /**< Application name, (Should only contain valid ASCII character excluding ';') */
  const char* applicationVersion; /**< The version of the application being used, (Should only contain valid ASCII character excluding ';')  */
} mip_cc_application_info;

#endif  // API_MIP_APPLICATION_INFO_CC_H_