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
 * @brief Defines TenantInformation interface
 * 
 * @file tenant_information.h
 */

#ifndef API_MIP_PROTECTION_TENANT_INFORMATION_H_
#define API_MIP_PROTECTION_TENANT_INFORMATION_H_

#include <memory>
#include <string>

#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief Tenant information from RMS
 */
class TenantInformation {
public:
  /**
   * @brief Get the issuer name
   *
   * @return The display name for the organization associated with this tenant
   */
  virtual const std::string& GetIssuerName() const = 0;
  /**
   * @brief Get the extranet Url of tenant
   *
   * @return The URL used to obtain licenses for content outside an enterprise's intranet
   */
  virtual const std::string& GetExtranetUrl() const = 0;
  /**
   * @brief Get intranet Url of tenant (Expected to match extranet url outside of ADRMS scenarios)
   *
   * @return The URL used to obtain licenses for content within an enterprise's intranet.
   * Same as extranet url for tenants that use Azure RMS.
   */
  virtual const std::string& GetIntranetUrl() const = 0;

  /**
   * @brief Get Id of Tenant if applicable
   *
   * @return The unique id used to refer to this tenant
   */
  virtual const std::string& GetTenantId() const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~TenantInformation() {}
protected:
  TenantInformation() {}
  /** @endcond */
};

MIP_NAMESPACE_END

#endif // API_MIP_PROTECTION_TENANT_INFORMATION_H_