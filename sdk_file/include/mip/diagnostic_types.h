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
 * @brief File containing diagnostic-related types
 * 
 * @file diagnostic_types.h
 */

#ifndef API_MIP_DIAGNOSTIC_TYPES_H_
#define API_MIP_DIAGNOSTIC_TYPES_H_

#include <stdint.h>

#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief Description of PII data, if any.
 * 
 * @note If an event is sent through audit pipeline, this value should be forced to 'None'.
 */
enum class Pii {
  None = 0,             /**< Data is not PII */
  SmtpAddress = 1,      /**< Data contains an SMTP address */
  Identity = 2,         /**< Data contains an identity */
  Uri = 3,              /**< Data contains a URI */
  Fqdn = 4,             /**< Data contains a FQDN */
  EUPI = 5,             /**< Data contains a End User Pseudonymous Information */
  OII = 6,              /**< Data contains a Organization Identifiable Information */
  EUII = 7,             /**< Data contains End User Identifiable Information */
  CustomerContent = 8,  /**< Data contains Customer Content */
};

/**
 * @brief Complete description of PII data types, if any.
 */
enum class CombinationPii : uint32_t {
  None =            0,          /**< Data is not PII */
  SmtpAddress =     1 << 0,     /**< Data contains an SMTP address */
  Identity =        1 << 1,     /**< Data contains an identity */
  Uri =             1 << 2,     /**< Data contains a URI */
  Fqdn =            1 << 3,     /**< Data contains a FQDN */
  EUPI =            1 << 4,     /**< Data contains a End User Pseudonymous Information */
  OII =             1 << 5,     /**< Data contains a Organization Identifiable Information */
  EUII =            1 << 6,     /**< Data contains End User Identifiable Information */
  CustomerContent = 1 << 7,     /**< Data contains Customer Content */
  All =             0xFFFFFFFF, /**< Any and all Pii is represented */
};

inline constexpr enum CombinationPii operator|(const enum CombinationPii selfValue, const enum CombinationPii inValue) {
  return static_cast<enum CombinationPii>(uint32_t(selfValue) | uint32_t(inValue));
}

inline constexpr enum CombinationPii operator&(const enum CombinationPii selfValue, const enum CombinationPii inValue) {
  return static_cast<enum CombinationPii>(uint32_t(selfValue) & uint32_t(inValue));
}

/**
 * @brief Description of event importance
 */
enum class EventLevel {
  Basic = 0,                /**< Data is used only for telemetry/logging purposes */
  ImportantServiceData = 1, /**< Data is necessary to track the usage of critical features */
  NecessaryServiceData = 2, /**< Data is necessary as a critical feature (e.g. audit) */
};

/**
 * @brief Underlying event property data type
 */
enum class EventPropertyType {
  Double, /**< double */
  Int64,  /**< int64_t */
  String, /**< string */
};

MIP_NAMESPACE_END

#endif // API_MIP_DIAGNOSTIC_TYPES_H_
