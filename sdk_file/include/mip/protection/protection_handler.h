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
 * @brief Defines ProtectionHandler interface
 * 
 * @file protection_handler.h
 */

#ifndef API_MIP_PROTECTION_PROTECTION_HANDLER_H_
#define API_MIP_PROTECTION_PROTECTION_HANDLER_H_

#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "mip/common_types.h"
#include "mip/mip_export.h"
#include "mip/mip_namespace.h"
#include "mip/protection_descriptor.h"
#include "mip/protection/protection_common_settings.h"
#include "mip/protection/protection_common_types.h"
#include "mip/protection/publishing_use_license_request.h"
#include "mip/stream.h"
#include "mip/user_rights.h"
#include "mip/user_roles.h"

MIP_NAMESPACE_BEGIN

class CryptoProvider;

/**
 * @brief Manages protection-related actions for a specific protection configuration
 */
class ProtectionHandler {
public:
  /**
   * @brief Interface that receives notifications related to ProtectionHandler
   * 
   * @note This interface must be implemented by applications using the protection SDK
   */
  class Observer {
  public:
    /**
     * @brief Called when ProtectionHandler was created successfully
     *
     * @param protectionHandler The newly created ProtectionHandler
     * @param context The same context that was passed to ProtectionEngine::CreateProtectionHandlerFromDescriptorAsync or
     * ProtectionEngine::CreateProtectionHandlerFromPublishingLicenseAsync
     *
     * @note An application can pass any type of context (for example, std::promise, std::function) to
     * ProtectionEngine::CreateProtectionHandlerFromDescriptorAsync or
     * ProtectionEngine::CreateProtectionHandlerFromPublishingLicenseAsync, and that same context will be forwarded
     * as-is to ProtectionEngine::Observer::OnCreateProtectionHandlerSuccess or
     * ProtectionEngine::Observer::OnCreateProtectionHandlerFailure
     */
    virtual void OnCreateProtectionHandlerSuccess(
        const std::shared_ptr<ProtectionHandler>& protectionHandler,
        const std::shared_ptr<void>& context) { UNUSED(protectionHandler); UNUSED(context); };

    /**
     * @brief Called when ProtectionHandler creation failed
     *
     * @param error Failure that occurred during creation
     * @param context The same context that was passed to ProtectionEngine::CreateProtectionHandlerFromDescriptorAsync or
     * ProtectionEngine::CreateProtectionHandlerFromPublishingLicenseAsync
     *
     * @note An application can pass any type of context (for example, std::promise, std::function) to
     * ProtectionEngine::CreateProtectionHandlerFromDescriptorAsync or
     * ProtectionEngine::CreateProtectionHandlerFromPublishingLicenseAsync, and that same context will be forwarded
     * as-is to ProtectionEngine::Observer::OnCreateProtectionHandlerSuccess or
     * ProtectionEngine::Observer::OnCreateProtectionHandlerFailure
     */
    virtual void OnCreateProtectionHandlerFailure(
        const std::exception_ptr& error, 
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /** @cond DOXYGEN_HIDE */
    virtual ~Observer() { }
    /** @endcond */
  };

  /**
   * @brief Settings used to create a ProtectionHandler to consume existing content
   */
  class ConsumptionSettings : public ProtectionCommonSettings {
  public:
    /**
     * @brief ProtectionHandler::ConsumptionSettings constructor for creating a new handler
     * 
     * @param serializedPublishingLicense Serialized publishing license from protected content
     */
    ConsumptionSettings(const std::vector<uint8_t>& serializedPublishingLicense)
        : mLicenseInfo(std::make_shared<PublishingLicenseInfo>(serializedPublishingLicense)) {}

    /**
     * @brief ProtectionHandler::ConsumptionSettings constructor for creating a new handler
     * 
     * @param serializedPreLicense Serialized pre license from attached to the content.
     * @param serializedPublishingLicense Serialized publishing license from protected content
     */
    ConsumptionSettings(
        const std::vector<uint8_t>& serializedPreLicense,
        const std::vector<uint8_t>& serializedPublishingLicense)
        : mLicenseInfo(std::make_shared<PublishingLicenseInfo>(serializedPreLicense, serializedPublishingLicense)) {}

    /**
     * @brief ProtectionHandler::ConsumptionSettings constructor for creating a new handler
     * 
     * @param licenseInfo Publishing license info from protected content
     * 
     * @note Providing a PublishingLicenseInfo (as opposed to just a raw serialized publishing license) will remove
     *       the need for MIP SDK to parse the publishing license.
     */
    ConsumptionSettings(const std::shared_ptr<PublishingLicenseInfo>& licenseInfo)
        : mLicenseInfo(licenseInfo) {}

    /**
     * @brief Get the publishing license associated with the protected content
     * 
     * @return Publishing license information
     */
    std::shared_ptr<PublishingLicenseInfo> GetPublishingLicenseInfo() const {
      return mLicenseInfo;
    }

    /**
     * @brief Sets the delegated user
     *
     * @param delegatedUserEmail the delegation email.
     * 
     * @note A delegated user is specified when the authenticating user/application is acting on behalf of another user
     */
    void SetDelegatedUserEmail(const std::string& delegatedUserEmail) { mDelegatedUserEmail = delegatedUserEmail; }

    /**
     * @brief Gets the delegated user
     * 
     * @return Delegated user
     * 
     * @note A delegated user is specified when the authenticating user/application is acting on behalf of another user
     */
    const std::string& GetDelegatedUserEmail() const { return mDelegatedUserEmail; }

    /**
     * @brief Defines the content name to register with document tracking.
     * 
     * @param contentName The identifier to register content under.
     * 
     * @note: If the content has already been registered, its name will not be changed.
     */
    void SetContentName(const std::string& contentName) { mContentName = contentName;}

    /**
     * @brief Gets the content name to register with document tracking.
     * 
     * @return The content name
     */
    const std::string& GetContentName() const { return mContentName; }

    /**
     * @brief Add a capability which will be passed along to RMS when fetching usage rights
     * 
     * @remarks Capabilities configured for this protection handler take precedence over any RMS capabilities
     *          configured on the protection engine.
     * 
     * @param capability An identifier that describes a capability that RMS supports
     */
    MIP_API void AddRmsCapability(const std::string& capability);

    /**
     * @brief Remove one of the added usage rights capabilities which get passed to RMS
     * 
     * @remarks Capabilities configured for this protection handler take precedence over any RMS capabilities
     *          configured on the protection engine.
     * 
     * @param capability The capability which should be removed.
     */
    MIP_API bool RemoveRmsCapability(const std::string& capability);

    /**
     * @brief The the full set of client capabilities supported when fetching usage rights from RMS.
     * 
     * @returns a pointer to a list of capabilities. 
     */
    const std::shared_ptr<std::vector<std::string>> GetRmsCapabilities() const { 
      if (mCapabilities == nullptr) { return mCapabilities; } 
      return std::make_shared<std::vector<std::string>>(*mCapabilities);
    }

  /** @cond DOXYGEN_HIDE */
  private:
    std::shared_ptr<PublishingLicenseInfo> mLicenseInfo;
    std::string mDelegatedUserEmail;
    std::string mContentName;
    std::shared_ptr<std::vector<std::string>> mCapabilities;
  /** @endcond */
  };

  /**
   * @brief Settings used to create a ProtectionHandler to protect new content
   */
  class PublishingSettings : public ProtectionCommonSettings {
  public:
    /**
     * @brief PublishingSettings constructor for enforcing protection on content
     * 
     * @param protectionDescriptor Protection details
     */
    PublishingSettings(const std::shared_ptr<ProtectionDescriptor>& protectionDescriptor)
        : mProtectionDescriptor(protectionDescriptor),
          mIsAuditedExtractionAllowed(false),
          mIsDeprecatedAlgorithmPreferred(false),
          mIsPublishingFormatJson(false),
          mRepairLabelIdAllowed(false),
          mRegenerateContentKey(false) {}
      
    /**
     * @brief PublishingSettings constructor for online republishing
     * 
     * @param protectionDescriptor New protection details
     * @param serializedPublishingLicense Original protection license
     */
    PublishingSettings(
        const std::shared_ptr<ProtectionDescriptor>& protectionDescriptor,
        const std::vector<uint8_t>& serializedPublishingLicense)
        : mProtectionDescriptor(protectionDescriptor),
          mRepublishingLicense(serializedPublishingLicense),
          mIsAuditedExtractionAllowed(false),
          mIsDeprecatedAlgorithmPreferred(false),
          mIsPublishingFormatJson(false),
          mRepairLabelIdAllowed(false),
          mRegenerateContentKey(false) {}

    /**
     * @brief PublishingSettings constructor for offline republishing
     * 
     * @param protectionDescriptor New protection details
     * @param protectionHandler Original protection handler
     */
    PublishingSettings(
        const std::shared_ptr<ProtectionDescriptor>& protectionDescriptor,
        const std::shared_ptr<ProtectionHandler>& protectionHandler)
        : mProtectionDescriptor(protectionDescriptor),
          mProtectionHandler(protectionHandler),
          mIsAuditedExtractionAllowed(false),
          mIsDeprecatedAlgorithmPreferred(false),
          mIsPublishingFormatJson(false),
          mRepairLabelIdAllowed(false),
          mRegenerateContentKey(false) {}

    std::shared_ptr<ProtectionDescriptor> GetProtectionDescriptor() const { return mProtectionDescriptor; }

    /**
     * @brief Gets protection handler for republishing scenario
     */
    std::shared_ptr<ProtectionHandler> GetProtectionHandlerForRepublish() const { return mProtectionHandler; }

    /**
     * @brief Gets whether or not non-MIP-aware applications are allowed to open protected content
     * 
     * @return If non-MIP-aware applications are allowed to open protected content
     */
    bool GetIsAuditedExtractionAllowed() const { return mIsAuditedExtractionAllowed; }

    /**
     * @brief Sets whether or not non-MIP-aware applications are allowed to open protected content
     * 
     * @param isAuditedExtractionAllowed If non-MIP-aware applications are allowed to open protected content
     */
    void SetIsAuditedExtractionAllowed(bool isAuditedExtractionAllowed) {
      mIsAuditedExtractionAllowed = isAuditedExtractionAllowed;
    }

    /**
     * @brief Gets whether or not deprecated crypto algorithm (ECB) is preferred for backwards compatibility
     * 
     * @return If deprecated crypto algorithm is preferred
     */
    bool GetIsDeprecatedAlgorithmPreferred() const { return mIsDeprecatedAlgorithmPreferred; }

    /**
     * @brief Sets whether or not deprecated crypto algorithm (ECB) is preferred for backwards compatibility
     * 
     * @param isDeprecatedAlgorithmPreferred if deprectated crypto algorithm is preferred
     */
    void SetIsDeprecatedAlgorithmPreferred(bool isDeprecatedAlgorithmPreferred) {
      mIsDeprecatedAlgorithmPreferred = isDeprecatedAlgorithmPreferred;
    }

    /**
     * @brief Sets the delegated user
     *
     * @param delegatedUserEmail the delegation email.
     * 
     * @note A delegated user is specified when the authenticating user/application is acting on behalf of another user
     */
    void SetDelegatedUserEmail(const std::string& delegatedUserEmail) { mDelegatedUserEmail = delegatedUserEmail; }

    /**
     * @brief Gets the delegated user
     * 
     * @return Delegated user
     * 
     * @note A delegated user is specified when the authenticating user/application is acting on behalf of another user
     */
    const std::string& GetDelegatedUserEmail() const { return mDelegatedUserEmail; }

    /**
     * @brief Gets whether or not the returned pl is in json format (xml format is more widely accepted and is the 
     * default).
     * 
     * @return true if is set to json format output.
     */
    bool IsPublishingFormatJson() const { return mIsPublishingFormatJson; }

    /**
     * @brief Sets whether or not the returned pl is in json format (xml format is more widely accepted and is the default).
     * 
     * @param isPublishingFormatJson if json format is enabled.
     */
    void SetPublishingFormatJson(bool isPublishingFormatJson) {
      mIsPublishingFormatJson = isPublishingFormatJson;
    }

    /**
     * @brief Gets whether or not the content key and ID should be regenerated during republishing.
     * 
     * @return True if the content key and ID should be regenerated. False otherwise.
     */
    bool GetRegenerateContentKey() const { return mRegenerateContentKey; }

    /**
     * @brief Sets whether or not the content key and ID should be regenerated during republishing.
     * 
     * @param regenerateContentKey If the content key and ID should be regenerated.
     */
    void SetRegenerateContentKey(bool regenerateContentKey) {
      mRegenerateContentKey = regenerateContentKey;
    }

    /**
     * @brief Sets pre-license user
     * 
     * @param preLicenseUserEmail Pre-license user
     * 
     * @note If no pre-license user is specified, a pre-license will not be obtained
     */
#if !defined(SWIG) && !defined(SWIG_DIRECTORS)
    [[deprecated("SetPreLicenseUserEmail is deprecated, use SetRequestPreLicense")]]
#endif
    void SetPreLicenseUserEmail(const std::string& preLicenseUserEmail) {
      if(mPublishingUseLicenseRequest == nullptr) {
        mPublishingUseLicenseRequest = PublishingUseLicenseRequest::CreatePublishingUseLicenseRequest();
      }
      mPublishingUseLicenseRequest->SetDelegatedUserEmail(preLicenseUserEmail);
    }

    /**
     * @brief Gets the pre-license user
     * 
     * @return Pre-license user
     */
#if !defined(SWIG) && !defined(SWIG_DIRECTORS)
    [[deprecated("GetPreLicenseUserEmail is deprecated, use GetPreLicenseRequest")]]
#endif
    const std::string& GetPreLicenseUserEmail() const {
      static std::string empty;
      return mPublishingUseLicenseRequest != nullptr ? mPublishingUseLicenseRequest->GetDelegatedUserEmail() : empty; }

    /**
     * @brief Gets the serialized republishing license
     * 
     * @return The serialized republishing license
     */
    const std::vector<uint8_t>& GetPublishingLicenseForRepublish() const {
      return mRepublishingLicense;
    }

    /**
     * @brief Setting this will cause the publishing request to include a pre-license
     * 
     * @param publishingUseLicenseRequest The pre-license request
     * 
     * @note If no pre-license request is specified, a pre-license will not be obtained
     */
    void SetRequestPreLicense(std::shared_ptr<PublishingUseLicenseRequest> publishingUseLicenseRequest) {
      mPublishingUseLicenseRequest = publishingUseLicenseRequest;
    }

    /**
     * @brief Gets the pre-license request
     * 
     * @return Pre-license request
     */
    std::shared_ptr<PublishingUseLicenseRequest> GetPreLicenseRequest() const {
      return mPublishingUseLicenseRequest;
    }

    /**
     * @brief Gets whether or not label id can be added to publishing license on republish
     * 
     * @return True if label info data in protection descriptor can be added to publishing license
     * 
     * @note This will only take effect if offline protection is used for republishing.
     */
    bool GetRepairLabelIdAllowed() const { return mRepairLabelIdAllowed; }

    /**
     * @brief Sets whether or not label id can be added to publishing license on republish
     * 
     * @param repairAllowed if label info data in protection descriptor can be added to publishing license
     * 
     * @note This will only take effect if offline protection is used for republishing. Should only be used to 
     * add a label to adhoc protection that is missing it.
     */
    void SetRepairLabelIdAllowed(bool repairAllowed) {
      mRepairLabelIdAllowed = repairAllowed;
    }

    /** @cond DOXYGEN_HIDE */
    private:
      std::shared_ptr<ProtectionDescriptor> mProtectionDescriptor;
      std::vector<uint8_t> mRepublishingLicense;
      std::shared_ptr<ProtectionHandler> mProtectionHandler;
      bool mIsAuditedExtractionAllowed;
      bool mIsDeprecatedAlgorithmPreferred;
      bool mIsPublishingFormatJson;
      bool mRepairLabelIdAllowed;
      bool mRegenerateContentKey;
      std::string mDelegatedUserEmail;
      std::shared_ptr<PublishingUseLicenseRequest> mPublishingUseLicenseRequest;
    /** @endcond */
  };

  /**
   * @brief Pre-license format
   */
  enum class PreLicenseFormat {
    Xml,  /**< Legacy XML/SOAP format used by MSIPC */
    Json, /**< JSON/REST format used by MIP SDK and RMS SDK */
  };

  /**
   * @brief Create a protected stream that will allow for encryption/decryption of content
   *
   * @param backingStream Backing stream from which to read/write
   * @param contentStartPosition Starting position (in bytes) within the backing stream where protected content begins
   * @param contentSize Size (in bytes) of protected content within backing stream
   *
   * @return Protected stream
   */
  virtual std::shared_ptr<Stream> CreateProtectedStream(
      const std::shared_ptr<Stream>& backingStream,
      int64_t contentStartPosition,
      int64_t contentSize) = 0;

  /**
   * @brief Encrypt a buffer
   *
   * @param offsetFromStart Relative position of inputBuffer from the very beginning of the cleartext content
   * @param inputBuffer Buffer of cleartext content that will be encrypted
   * @param inputBufferSize Size (in bytes) of input buffer
   * @param outputBuffer Buffer into which encrypted content will be copied
   * @param outputBufferSize Size (in bytes) of output buffer
   * @param isFinal If input buffer contains the final cleartext bytes or not
   * 
   * @return actual size (in bytes) of encrypted content
   */
  virtual int64_t EncryptBuffer(
      int64_t offsetFromStart,
      const uint8_t* inputBuffer,
      int64_t inputBufferSize,
      uint8_t* outputBuffer,
      int64_t outputBufferSize,
      bool isFinal) = 0;

  /**
   * @brief Decrypt a buffer
   *
   * @param offsetFromStart Relative position of inputBuffer from the very beginning of the encrypted content
   * @param inputBuffer Buffer of encrypted content that will be decrypted
   * @param inputBufferSize Size (in bytes) of input buffer
   * @param outputBuffer Buffer into which decrypted content will be copied
   * @param outputBufferSize Size (in bytes) of output buffer
   * @param isFinal If input buffer contains the final encrypted bytes or not
   * 
   * @return actual size (in bytes) of decrypted content
   */
  virtual int64_t DecryptBuffer(
      int64_t offsetFromStart,
      const uint8_t* inputBuffer,
      int64_t inputBufferSize,
      uint8_t* outputBuffer,
      int64_t outputBufferSize,
      bool isFinal) = 0;

  /**
   * @brief Calculates size (in bytes) of content if it were to be encrypted with this ProtectionHandler
   * 
   * @param unprotectedLength Size (in bytes) of unprotected content
   * @param includesFinalBlock Describes if the unprotected content in question includes the final block or not.
   *  For example, in CBC4k encryption mode, non-final protected blocks are the same size as unprotected blocks, but
   *  final protected blocks are larger than their unprotected counterparts.
   * 
   * @return Size (in bytes) of protected content
   */
  virtual int64_t GetProtectedContentLength(int64_t unprotectedLength, bool includesFinalBlock) = 0;

  /**
   * @brief Gets the block size (in bytes) for the cipher mode used by this ProtectionHandler
   * 
   * @return Block size (in bytes)
   */
  virtual int64_t GetBlockSize() = 0;

  /**
   * @brief Gets the rights granted to the user/identity associated with this ProtectionHandler
   * 
   * @return Rights granted to the user
   */
  virtual std::vector<std::string> GetRights() const = 0;

  /**
   * @brief Checks if protection handler grants user access to the specified right
   * 
   * @param right Right to check
   * 
   * @return If protection handler grants user access to the specified right or not
   */
  virtual bool AccessCheck(const std::string& right) const = 0;

  /**
   * @brief Gets user associated with the protection handler
   *
   * @return User associated with protection handler
   */
  virtual const std::string GetIssuedTo() = 0;

  /**
   * @brief Gets email address of content owner
   * 
   * @return Email address of content owner
   */
  virtual const std::string GetOwner() = 0;

  /**
   * @brief Gets if the current user is the content owner or not
   * 
   * @return If the current user is the content owner or not
   */
  virtual bool IsIssuedToOwner() = 0;

  /**
   * @brief Gets protection details
   * 
   * @return Protection details
   */
  virtual std::shared_ptr<ProtectionDescriptor> GetProtectionDescriptor() = 0;

  /**
   * @brief Gets unique identifier for the document/content
   * 
   * @return Unique content identifier
   * 
   * @note Publishing licenses will have this identifier surrounded by curly braces "{}".
   * Those braces are removed from the value returned here
   */
  virtual const std::string GetContentId() = 0;

  /**
   * @brief Gets if protection handler uses deprecated crypto algorithms (ECB) for backward compatibility or not
   * 
   * @return If protection handler uses deprecated crypto algorithms or not
   */
  virtual bool DoesUseDeprecatedAlgorithms() = 0;

  /**
   * @brief Gets if protected content requires application-defined padding or if it is handled internally.
   *
   * @return True, if application-defined padding is required, false if not.
   */
  virtual bool UsesApplicationDefinedPadding() = 0;

  /**
   * @brief Gets if protection handler grants user 'audited extract' right or not
   * 
   * @return If protection handler grants user 'audited extract' right or not
   */
  virtual bool IsAuditedExtractAllowed() = 0;

  /**
   * @brief Serialize ProtectionHandler into a publishing license (PL)
   * 
   * @return Serialized publishing license
   */
  virtual const std::vector<uint8_t>& GetSerializedPublishingLicense() const = 0;

  /**
   * @brief Get pre-license
   * 
   * @param format Pre-license format
   * 
   * @return Serialized pre-license
   * 
   * @note A pre-license allows a user to immediately consume content without making an additional HTTP call. The
   *       ProtectionHandler must have been created with a ProtectionHandler::PublishingSettings::SetPreLicenseUserEmail
   *       value or else this will return an empty vector.
   */
  virtual const std::vector<uint8_t>& GetSerializedPreLicense(PreLicenseFormat format) const = 0;

  /**
   * @brief Gets the cipher mode of the protection handler
   * 
   * @return The cipher mode
   */
  virtual CipherMode GetCipherMode() const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual const std::shared_ptr<CryptoProvider> GetCryptoProvider() = 0;
  virtual const std::shared_ptr<CryptoProvider>& GetPolicyCryptoProvider() const = 0;
  virtual ~ProtectionHandler() {}

  /** @endcond */
};

MIP_NAMESPACE_END
#endif // API_MIP_PROTECTION_PROTECTION_HANDLER_H_
