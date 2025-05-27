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
 * @brief Defines C-Style crypto types
 * 
 * @file protection_crypto_types_cc.h
 */

#ifndef API_MIP_PROTECTION_CRYPTO_TYPES_CC_H_
#define API_MIP_PROTECTION_CRYPTO_TYPES_CC_H_

/**
 * @brief Cipher mode identifier
 */
typedef enum {
  MIP_CIPHER_MODE_CBC4K = 0,              /**< CBC 4K mode with internal padding */
  MIP_CIPHER_MODE_ECB = 1,                /**< ECB mode */
  MIP_CIPHER_MODE_CBC512NOPADDING = 2,    /**< CBC 512 mode with external (client) padding */
  MIP_CIPHER_MODE_CBC4KNOPADDING = 3      /**< CBC 4K mode with external (client) padding */
} mip_cc_cipher_mode;


#endif // API_MIP_PROTECTION_CRYPTO_TYPES_CC_H_