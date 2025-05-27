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
 * @brief A file Containing types used to control dns redirection
 * 
 * @file dns_redirection.h
 */

#ifndef API_MIP_DNS_REDIRECTION_H_
#define API_MIP_DNS_REDIRECTION_H_

#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief Storage type for the caches
 */
enum class DnsRedirection : unsigned int {
  Disabled = 0, /**< Dns redirect is disabled */
  MDEDiscovery = 1 << 0, /**< MDE DNS.  Uses DNS query _rmsdisco._http._tcp.  This mode is the default */
};

inline DnsRedirection operator|(DnsRedirection lhs, DnsRedirection rhs) {
  return static_cast<DnsRedirection>(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs));
}

MIP_NAMESPACE_END

#endif  // API_MIP_DNS_REDIRECTION_H_