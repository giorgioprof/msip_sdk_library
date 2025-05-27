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
 * @brief Contains details of a parsed publishing license
 * 
 * @file parsed_publishing_license.h
 */

#ifndef API_MIP_PROTECTION_PARSED_PUBLISHING_LICENSE_H_
#define API_MIP_PROTECTION_PARSED_PUBLISHING_LICENSE_H_

#include <chrono>
#include <string>
#include <vector>
#include "mip/mip_namespace.h"
#include "mip/protection/license_application_data.h"
#include "mip/protection/license_connection_info.h"
#include "mip/protection/license_descriptor.h"
#include "mip/protection_descriptor.h"

MIP_NAMESPACE_BEGIN

class ParsedPublishingLicense {
public:

  /**
   * @brief Gets the license domains
   */
  virtual const std::vector<std::string>& GetDomains() = 0;

  /**
   * @brief Gets the server public cert
   */
  virtual const std::string& GetServerPublicCert() = 0;

  /**
   * @brief Gets the content id
   */
  virtual const std::string& GetContentId() = 0;

  /**
   * @brief Gets the issuer id
   */
  virtual const std::string& GetIssuerId() = 0;

  /**
   * @brief Gets the sealed key type
   */
  virtual const std::string& GetSealedKeyType() = 0;

  /**
   * @brief Gets the double key algorithm
   */
  virtual const std::string& GetDoubleKeyAlgorithm() = 0;

  /**
   * @brief Gets the owner
   */
  virtual const std::string& GetOwner() = 0;

  /**
   * @brief Gets the connection info
   */
  virtual std::shared_ptr<LicenseConnectionInfo> GetConnectionInfo() = 0;

  /**
   * @brief Gets the descriptor
   */
  virtual std::shared_ptr<LicenseDescriptor> GetDescriptor() = 0;

  /**
   * @brief Gets the referral info url
   */
  virtual const std::string& GetReferralInfoUrl() = 0;

  /**
   * @brief Gets the signed application data
   */
  virtual const std::vector<std::shared_ptr<LicenseApplicationData>>& GetSignedApplicationData() = 0;

  /**
   * @brief Gets the issued time
   */
  virtual const std::chrono::time_point<std::chrono::system_clock>& GetIssuedTime() = 0;

  /**
   * @brief Gets the label info of the license, if applicable
   */
  virtual const mip::LabelInfo& GetLabelInfo() = 0;

  /**
   * @brief Gets the encrypted rights data. Used internally by the SDK. Applications should not try to decipher or modify this data.
   */
   virtual const std::vector<uint8_t>& GetEncryptedRightsData() = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~ParsedPublishingLicense() { }
protected:
  ParsedPublishingLicense() {}
  /** @endcond */
};

MIP_NAMESPACE_END

#endif // API_MIP_PROTECTION_PARSED_PUBLISHING_LICENSE_H_
