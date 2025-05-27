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
 * @brief Defines DelegationLicenseSettings class
 * 
 * @file delegation_license_settings.h
 */
#ifndef API_MIP_PROTECTION_DELEGATION_LICENSE_SETTINGS_H_
#define API_MIP_PROTECTION_DELEGATION_LICENSE_SETTINGS_H_

#include <memory>
#include <string>
#include <vector>

#include "mip/mip_context.h"
#include "mip/mip_export.h"
#include "mip/mip_namespace.h"
#include "mip/protection/protection_common_types.h"
#include "mip/protection/protection_common_settings.h"

MIP_NAMESPACE_BEGIN

class DelegationLicenseSettings : public ProtectionCommonSettings {
public:
  /**
   * @brief Gets the PublishingLicenseInfo, the publishing license
   * 
   * @return The PublishingLicenseInfo
   */
  virtual std::shared_ptr<const PublishingLicenseInfo> GetLicenseInfo() const = 0;

  /**
   * @brief Gets the list of users for the request
   * 
   * @return The users
   */
  virtual const std::vector<std::string>& GetUsers() const  = 0;

  /**
   * @brief Gets the boolean value that indicates whether or not to obtain end user license in addition to the delegate license
   * 
   * @return Whether to aquire end user licenses
   */
  virtual bool GetAquireEndUserLicenses() const = 0;

#ifndef MIP_CORE_PACKAGE
  /**
   * @brief Creates a DelegationLicenseSettings object
   *
   * @param licenseInfo The publishing license to request licenses for
   * @param users A list of user email addresses to create licenses for
   * @param acquireEndUserLicenses Whether to include end user licenses (license bound to the users) or to only include a delegated license (bound to the identity on the engine)
   *
   * @return A DelegationLicenseSettings object
   */
#if !defined(SWIG) && !defined(SWIG_DIRECTORS)
  [[deprecated("CreateDelegationLicenseSettings(const PublishingLicenseInfo&,const std::vector<std::string>&,bool) is deprecated, use CreateDelegationLicenseSettings(const std::shared_ptr<MipContext>&,const PublishingLicenseInfo&,const std::vector<std::string>&,bool)")]]
#endif
  MIP_API static std::shared_ptr<DelegationLicenseSettings> __CDECL CreateDelegationLicenseSettings(
      const PublishingLicenseInfo& licenseInfo,
      const std::vector<std::string>& users,
      bool acquireEndUserLicenses);
#endif

  /**
   * @brief Creates a DelegationLicenseSettings object
   *
   * @param licenseInfo The publishing license to request licenses for
   * @param users A list of user email addresses to create licenses for
   * @param acquireEndUserLicenses Whether to include end user licenses (license bound to the users) or to only include a delegated license (bound to the identity on the engine)
   *
   * @return A DelegationLicenseSettings object
   */
  MIP_API static std::shared_ptr<DelegationLicenseSettings> __CDECL CreateDelegationLicenseSettings(
      const std::shared_ptr<MipContext>& mipContext,
      const PublishingLicenseInfo& licenseInfo,
      const std::vector<std::string>& users,
      bool acquireEndUserLicenses);

  /** @cond DOXYGEN_HIDE */
  virtual ~DelegationLicenseSettings() {}
protected:
  DelegationLicenseSettings() {}
  /** @endcond */
};

MIP_NAMESPACE_END
#endif // API_MIP_PROTECTION_DELEGATION_LICENSE_SETTINGS_H_
