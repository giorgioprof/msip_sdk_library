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
 * @brief Contains a class that allows creating parsed publishing license details
 * 
 * @file parsed_publishing_license_builder.h
 */

#ifndef API_MIP_PROTECTION_PARSED_PUBLISHING_LICENSE_BUILDER_H_
#define API_MIP_PROTECTION_PARSED_PUBLISHING_LICENSE_BUILDER_H_

#include <chrono>
#include <string>
#include <vector>

#include "mip/mip_export.h"
#include "mip/mip_namespace.h"
#include "mip/protection/license_application_data.h"
#include "mip/protection/license_connection_info.h"
#include "mip/protection/license_descriptor.h"
#include "mip/protection/parsed_publishing_license.h"

MIP_NAMESPACE_BEGIN


class ParsedPublishingLicenseBuilder {
public:

  /**
   * @brief Creates a ParsedPublishingLicenseBuilder
   * 
   * @return New ParsedPublishingLicenseBuilder instance
   */
  MIP_API static std::shared_ptr<ParsedPublishingLicenseBuilder> Create();

  /**
   * @brief Sets the license domains
   * 
   * @param domains
   */
  virtual void SetDomains(const std::vector<std::string>& domains) = 0;

  /**
   * @brief Sets the server public cert
   * 
   * @param serverPublicCert 
   */
  virtual void SetServerPublicCert(const std::string& serverPublicCert) = 0;

  /**
   * @brief Sets the content id
   * 
   * @param contentId
   */
  virtual void SetContentId(const std::string& contentId) = 0;

  /**
   * @brief Sets the issuer id
   * 
   * @param issuerId
   */
  virtual void SetIssuerId(const std::string& issuerId) = 0;

  /**
   * @brief Sets the sealed key type
   * 
   * @param sealedKeyType
   */
  virtual void SetSealedKeyType(const std::string& sealedKeyType) = 0;

  /**
   * @brief Sets the double key algorithm
   * 
   * @param doubleKeyAlgorithm
   */
  virtual void SetDoubleKeyAlgorithm(const std::string& doubleKeyAlgorithm) = 0;

  /**
   * @brief Sets the owner
   * 
   * @param owner
   */
  virtual void SetOwner(const std::string& owner) = 0;

  /**
   * @brief Sets the connection info
   * 
   * @param licenseConnectionInfo
   */
  virtual void SetConnectionInfo(std::shared_ptr<LicenseConnectionInfo> licenseConnectionInfo) = 0;

  /**
   * @brief Sets the descriptor
   * 
   * @param descriptor
   */
  virtual void SetDescriptor(std::shared_ptr<LicenseDescriptor> descriptor) = 0;

  /**
   * @brief Sets the referral info url
   * 
   * @param referralInfoUrl
   */
  virtual void SetReferralInfoUrl(const std::string& referralInfoUrl) = 0;

  /**
   * @brief Sets the signed application data
   * 
   * @param signedApplicationData
   */
  virtual void SetSignedApplicationData(const std::vector<std::shared_ptr<LicenseApplicationData>>& signedApplicationData) = 0;

  /**
   * @brief Sets the issued time
   * 
   * @param issuedTime
   */
  virtual void SetIssuedTime(const std::chrono::time_point<std::chrono::system_clock>& issuedTime) = 0;

  /**
   * @brief Sets the label info
   * 
   * @param labelInfo
   */
  virtual void SetLabelInfo(const LabelInfo& labelInfo) = 0;

  /**
   * @brief Sets the encrypted rights data
   *
   * @param encryptedRightsData
   */
   virtual void SetEncryptedRightsData(const std::vector<uint8_t>& encryptedRightsData) = 0;

  /**
   * @brief Creates a ParsedPublishingLicense with the properties set on this object instance
   * 
   * @return New ParsedPublishingLicense instance
   */
  virtual std::shared_ptr<ParsedPublishingLicense> CreateParsedPublishingLicense() = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~ParsedPublishingLicenseBuilder() { }
protected:
  ParsedPublishingLicenseBuilder() {}
  /** @endcond */
};

MIP_NAMESPACE_END

#endif // API_MIP_PROTECTION_PARSED_PUBLISHING_LICENSE_BUILDER_H_
