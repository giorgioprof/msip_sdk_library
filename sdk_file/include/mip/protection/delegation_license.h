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
 * @brief Defines DelegationLicense interface
 * 
 * @file delegation_license.h
 */
#ifndef API_MIP_PROTECTION_DELEGATION_LICENSE_H_
#define API_MIP_PROTECTION_DELEGATION_LICENSE_H_

#include <string>
#include <vector>

#include "mip/mip_namespace.h"
#include "mip/protection/protection_handler.h"

MIP_NAMESPACE_BEGIN

class DelegationLicense {
public:

  /**
   * @brief Gets the delegation license in Json format
   * 
   * @return Serialized license
   * 
   * @note This license is bound to the identity of the user that made the request
   */
  virtual const std::vector<uint8_t>& GetSerializedDelegationJsonLicense() = 0;

  /**
   * @brief Gets the user license, if requested
   * 
   * @param format License format
   * 
   * @return Serialized user license if requested, otherwise empty vector
   * 
   * @note This license is bound to the delegated user in the request
   */
  virtual const std::vector<uint8_t>& GetSerializedUserLicense(ProtectionHandler::PreLicenseFormat format) = 0;

  /**
   * @brief Gets the user that this license was created for
   * 
   * @return The user
   */
  virtual const std::string& GetUser() = 0;

  /**
   * @brief Gets the delegation license in Xrml format
   * 
   * @return Serialized license
   * 
   * @note This license is bound to the identity of the user that made the request
   */
  virtual const std::vector<uint8_t>& GetSerializedDelegationXrmlLicense() = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~DelegationLicense() { }
  /** @endcond */

};

MIP_NAMESPACE_END
#endif // API_MIP_PROTECTION_DELEGATION_LICENSE_H_

