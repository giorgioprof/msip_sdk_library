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
 * @brief Defines auth callback functions
 * 
 * @file auth_callback_cc.h
 */

#ifndef API_MIP_AUTH_CALLBACK_CC_H_
#define API_MIP_AUTH_CALLBACK_CC_H_

#include <stddef.h>
#include <stdint.h>

#include "mip_cc/identity_cc.h"
#include "mip_cc/mip_macros_cc.h"

/**
 * @brief Information provided by a server to generate an OAuth2 token
 */
typedef struct {
  const char* authority; /**< OAuth2 authority */
  const char* resource;  /**< OAuth2 resource */
  const char* scope;     /**< OAuth2 scope */
} mip_cc_oauth2_challenge;

/**
 * @brief callback function definition for acquiring OAuth2 token
 * 
 * @param identity The email address for whom token will be acquired
 * @param challenge OAuth2 challenge
 * @param context Opaque application context that was passed to the MIP API that resulted in this auth callback
 * @param tokenBuffer [Output] Buffer into which token will be copied. If null, 'actualTokenSize' will be populated, but
 *        no bytes will be copied
 * @param tokenBufferSize Size (in bytes) of output buffer
 * @param actualTokenSize [Output] Actual size (in bytes) of token
 * 
 * @return True is token was retrieved, else false
 */
MIP_CC_CALLBACK(mip_cc_auth_callback /*typename*/,
    bool /*return*/,
    const mip_cc_identity* /*identity*/,
    const mip_cc_oauth2_challenge* /*challenge*/,
    const void* /*context*/,
    uint8_t* /*tokenBuffer*/,
    const int64_t /*tokenBufferSize*/,
    int64_t* /*actualTokenSize*/);

#endif // API_MIP_AUTH_CALLBACK_CC_H_
