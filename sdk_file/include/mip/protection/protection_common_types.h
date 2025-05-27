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
 * @brief A file containing the common types used by the protection module.
 * 
 * @file protection_common_types.h
 */

#ifndef API_MIP_PROTECTION_PROTECTION_COMMON_TYPES_H_
#define API_MIP_PROTECTION_PROTECTION_COMMON_TYPES_H_

#define DYNAMIC_WATERMARK_CAPABILITY "dynamicwatermark"

#include <chrono>
#include <map>
#include <string>
#include <vector>

#include "mip/mip_namespace.h"
#include "mip/protection/license_connection_info.h"
#include "mip/protection/license_descriptor.h"
#include "mip/protection/parsed_publishing_license.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief Holds the details of a Publishing License used to create a protection handler
 */
class PublishingLicenseInfo {
public:
  PublishingLicenseInfo(
      const std::vector<uint8_t>& serializedPublishingLicense)
      : mSerializedPublishingLicense(serializedPublishingLicense),
        mIsParsed(false),
        mIssuedTime(std::chrono::system_clock::from_time_t(0)),
        mLabelInfo("","") {}
  
  PublishingLicenseInfo(
      const std::vector<uint8_t>& serializedPreLicense,
      const std::vector<uint8_t>& serializedPublishingLicense)
      : mPreLicense(serializedPreLicense),
        mSerializedPublishingLicense(serializedPublishingLicense),
        mIsParsed(false),
        mIssuedTime(std::chrono::system_clock::from_time_t(0)),
        mLabelInfo("","") {}

  void SetParsedData(std::shared_ptr<ParsedPublishingLicense> parsedPublishingLicense) {
    mIsParsed = true;
    mDomains = parsedPublishingLicense->GetDomains();
    mServerPublicCert = parsedPublishingLicense->GetServerPublicCert();
    mContentId = parsedPublishingLicense->GetContentId();
    mIssuerId = parsedPublishingLicense->GetIssuerId();
    mSealedKeyType = parsedPublishingLicense->GetSealedKeyType();
    mDoubleKeyAlgorithm = parsedPublishingLicense->GetDoubleKeyAlgorithm();
    mOwner = parsedPublishingLicense->GetOwner();
    mConnectionInfo = parsedPublishingLicense->GetConnectionInfo();
    mDescriptor = parsedPublishingLicense->GetDescriptor();
    mReferralInfoUrl = parsedPublishingLicense->GetReferralInfoUrl();
    mSignedApplicationData = parsedPublishingLicense->GetSignedApplicationData();
    mIssuedTime = parsedPublishingLicense->GetIssuedTime();
    mLabelInfo = parsedPublishingLicense->GetLabelInfo();
    mEncryptedRightsData = parsedPublishingLicense->GetEncryptedRightsData();
  }

#if !defined(SWIG) && !defined(SWIG_DIRECTORS)
    [[deprecated("This version of SetParsedData is deprecated, use the version that accepts a ParsedPublishingLicense")]]
#endif
 void SetParsedData(
      const std::vector<std::string>& domains,
      const std::string& serverPublicCert,
      const std::string& contentId,
      const std::string& issuerId) {
    mDomains = domains;
    mServerPublicCert = serverPublicCert;
    mContentId = contentId;
    mIssuerId = issuerId;
    mIsParsed = true;
  }
  
  const std::vector<uint8_t>& GetSerializedPublishingLicense() const { return mSerializedPublishingLicense; }
  const std::vector<uint8_t>& GetPreLicense() const { return mPreLicense; }

  const std::vector<std::string>& GetDomains() const { return mDomains; }
  const std::string& GetServerPublicCertificate() const { return mServerPublicCert; }
  const std::string& GetIssuerId() const { return mIssuerId; }
  const std::string& GetSealedKeyType() const { return mSealedKeyType; }
  const std::string& GetContentId() const { return mContentId; }
  bool IsLicenseParsed() const { return mIsParsed; }
  bool HasPreLicense() const { return !mPreLicense.empty(); }
  bool GetIsDoubleKeyLicense() const { return !mDoubleKeyAlgorithm.empty(); }
  const std::string& GetDoubleKeyAlgorithm() const { return mDoubleKeyAlgorithm; }
  const std::string& GetOwner() const { return mOwner; }
  std::shared_ptr<LicenseConnectionInfo> GetConnectionInfo() const { return mConnectionInfo; }
  std::shared_ptr<LicenseDescriptor> GetDescriptor() const { return mDescriptor; }
  const std::string& GetReferralInfoUrl() const { return mReferralInfoUrl; }
  const std::vector<std::shared_ptr<LicenseApplicationData>>& GetSignedApplicationData() const { return mSignedApplicationData; }
  const std::chrono::time_point<std::chrono::system_clock>& GetIssuedTime() const { return mIssuedTime; }
  const mip::LabelInfo GetLabelInfo() const { return mLabelInfo; }
  const std::vector<uint8_t>& GetEncryptedRightsData() const { return mEncryptedRightsData; }

private:
  std::vector<uint8_t> mPreLicense;
  std::vector<uint8_t> mSerializedPublishingLicense;

  std::vector<std::string> mDomains;
  std::string mServerPublicCert;
  std::string mContentId;
  std::string mIssuerId;
  std::string mSealedKeyType;
  bool mIsParsed;
  std::string mDoubleKeyAlgorithm;
  std::string mOwner;
  std::shared_ptr<LicenseConnectionInfo> mConnectionInfo;
  std::shared_ptr<LicenseDescriptor> mDescriptor;
  std::string mReferralInfoUrl;
  std::vector<std::shared_ptr<LicenseApplicationData>> mSignedApplicationData;
  std::chrono::time_point<std::chrono::system_clock> mIssuedTime;
  LabelInfo mLabelInfo;
  std::vector<uint8_t> mEncryptedRightsData;
};

/**
 * @brief Cipher mode identifier
 */
enum class CipherMode {
  CIPHER_MODE_CBC4K = 0,              /**< CBC 4K mode with internal padding */
  CIPHER_MODE_ECB = 1,                /**< ECB mode */
  CIPHER_MODE_CBC512NOPADDING = 2,    /**< CBC 512 mode with external (client) padding */
  CIPHER_MODE_CBC4KNOPADDING = 3      /**< CBC 4K mode with external (client) padding */
};

MIP_NAMESPACE_END

#endif // API_MIP_PROTECTION_PROTECTION_COMMON_TYPES_H_