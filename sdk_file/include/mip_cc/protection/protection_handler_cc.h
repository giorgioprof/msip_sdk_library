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
* @brief Defines C-Style ProtectionHandler interface
* 
* @file protection_handler_cc.h
*/

#ifndef API_MIP_PROTECTION_PROTECTION_HANDLER_CC_H_
#define API_MIP_PROTECTION_PROTECTION_HANDLER_CC_H_

#include <stdint.h>

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/protection_descriptor_cc.h"
#include "mip_cc/protection/protection_crypto_types_cc.h"
#include "mip_cc/result_cc.h"
#include "mip_cc/string_list_cc.h"

typedef mip_cc_handle* mip_cc_protection_handler;
typedef mip_cc_handle* mip_cc_protection_handler_publishing_settings;
typedef mip_cc_handle* mip_cc_protection_handler_consumption_settings;

/**
* @brief Defines pre-license format
*/
typedef enum {
  MIP_PRE_LICENSE_FORMAT_XML = 0,  /**< Legacy XML/SOAP format used by MSIPC */
  MIP_PRE_LICENSE_FORMAT_JSON = 1, /**< JSON/REST format used by MIP SDK and RMS SDK */
} mip_cc_pre_license_format;

/**
 * @brief Create a settings object used to create a protection handler for publishing new content
 * 
 * @param descriptor Protection details
 * @param settings [Output] Newly-created settings instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateProtectionHandlerPublishingSettings(
    const mip_cc_protection_descriptor descriptor,
    mip_cc_protection_handler_publishing_settings* settings,
    mip_cc_error* errorInfo);

/**
 * @brief Sets whether or not deprecated crypto algorithm (ECB) is preferred for backwards compatibility
 * 
 * @param settings Protection handler settings
 * @param isDeprecatedAlgorithmPreferred Whether or not deprecated algorithm is preferred
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandlerPublishingSettings_SetIsDeprecatedAlgorithmPreferred(
    const mip_cc_protection_handler_publishing_settings settings,
    const bool isDeprecatedAlgorithmPreferred,
    mip_cc_error* errorInfo);

/**
 * @brief Sets whether or not non-MIP-aware applications are allowed to open protected content
 * 
 * @param settings Protection handler settings
 * @param isAuditedExtractionAllowed Whether or not non-MIP-aware applications are allowed to open protected content
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandlerPublishingSettings_SetIsAuditedExtractionAllowed(
    const mip_cc_protection_handler_publishing_settings settings,
    const bool isAuditedExtractionAllowed,
    mip_cc_error* errorInfo);

/**
 * @brief Sets whether or not PL is in JSON format (default is XML)
 * 
 * @param settings Protection handler settings
 * @param isPublishingFormatJson Whether or not resulting PL should be in JSON format
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandlerPublishingSettings_SetIsPublishingFormatJson(
    const mip_cc_protection_handler_publishing_settings settings,
    const bool isPublishingFormatJson,
    mip_cc_error* errorInfo);

/**
 * @brief Sets delegated user
 * 
 * @param settings Protection handler settings
 * @param delegatedUserEmail Email address of delegated user
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note A delegated user is specified when the authenticating user/application is acting on behalf of another user
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandlerPublishingSettings_SetDelegatedUserEmail(
    const mip_cc_protection_handler_publishing_settings settings,
    const char* delegatedUserEmail,
    mip_cc_error* errorInfo);

/**
 * @brief Sets pre-license user
 * 
 * @param settings Protection handler settings
 * @param preLicenseUserEmail Email address of pre-license user
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note A pre-license user is specified when a pre-license should be fetched when publishing
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandlerPublishingSettings_SetPreLicenseUserEmail(
    const mip_cc_protection_handler_publishing_settings settings,
    const char* preLicenseUserEmail,
    mip_cc_error* errorInfo);


/**
 * @brief Sets application scenario Id
 * 
 * @param settings Protection handler settings
 * @param applicationScenarioId application scenario Id
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandlerPublishingSettings_SetApplicationScenarioId(
    const mip_cc_protection_handler_publishing_settings settings,
    const char* applicationScenarioId,
    mip_cc_error* errorInfo);

/**
 * @brief Configures custom settings, used for feature gating and testing.
 * 
 * @param settings Protection handler settings
 * @param customSettings Key/value pairs of custom settings
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandlerPublishingSettings_SetCustomSettings(
    const mip_cc_protection_handler_publishing_settings settings,
    const mip_cc_dictionary customSettings,
    mip_cc_error* errorInfo);

/**
 * @brief Create a settings object used to create a protection handler for consuming existing content
 * 
 * @param publishingLicenseBuffer Buffer containing raw publishing license
 * @param publishingLicenseBufferSize Size of publishing license buffer
 * @param settings [Output] Newly-created settings instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateProtectionHandlerConsumptionSettings(
    const uint8_t* publishingLicenseBuffer,
    const int64_t publishingLicenseBufferSize,
    mip_cc_protection_handler_consumption_settings* settings,
    mip_cc_error* errorInfo);

/**
 * @brief Create a settings object used to create a protection handler for consuming existing content
 * 
 * @param preLicenseBuffer Buffer containing raw pre license buffer
 * @param preLicenseBufferSize Size of pre license buffer
 * @param publishingLicenseBuffer Buffer containing raw publishing license
 * @param publishingLicenseBufferSize Size of publishing license buffer
 * @param settings [Output] Newly-created settings instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateProtectionHandlerConsumptionSettingsWithPreLicense(
    const uint8_t* preLicenseBuffer,
    const int64_t preLicenseBufferSize,
    const uint8_t* publishingLicenseBuffer,
    const int64_t publishingLicenseBufferSize,
    mip_cc_protection_handler_consumption_settings* settings,
    mip_cc_error* errorInfo);

/**
 * @brief Sets whether or not protection handler creation permits online HTTP operations
 * 
 * @param settings Protection handler settings
 * @param isOfflineOnly True if HTTP operations are forbidden, else false
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If this is set to true, protection handler creation will only succeed if content has already been previously
 *       decrypted and its unexpired license is cached. A MIP_RESULT_ERROR_NETWORK result will be returned if cached
 *       content is not found.
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandlerConsumptionSettings_SetIsOfflineOnly(
    const mip_cc_protection_handler_consumption_settings settings,
    const bool isOfflineOnly,
    mip_cc_error* errorInfo);

/**
 * @brief Sets delegated user
 * 
 * @param settings Protection handler settings
 * @param delegatedUserEmail Email address of delegated user
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note A delegated user is specified when the authenticating user/application is acting on behalf of another user
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandlerConsumptionSettings_SetDelegatedUserEmail(
    const mip_cc_protection_handler_consumption_settings settings,
    const char* delegatedUserEmail,
    mip_cc_error* errorInfo);

/**
 * @brief Sets application scenario Id
 * 
 * @param settings Protection handler settings
 * @param applicationScenarioId application scenario Id
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandlerConsumptionSettings_SetApplicationScenarioId(
    const mip_cc_protection_handler_consumption_settings settings,
    const char* applicationScenarioId,
    mip_cc_error* errorInfo);

/**
 * @brief Configures custom settings, used for feature gating and testing.
 * 
 * @param settings Protection handler settings
 * @param customSettings Key/value pairs of custom settings
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandlerConsumptionSettings_SetCustomSettings(
    const mip_cc_protection_handler_consumption_settings settings,
    const mip_cc_dictionary customSettings,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of publishing license (in bytes)
 * 
 * @param handler Handler representing protected content
 * @param publishingLicenseBufferSize [Output] Size of publishing license (in bytes)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_GetSerializedPublishingLicenseSize(
    const mip_cc_protection_handler handler,
    int64_t* publishingLicenseBufferSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets publishing license
 * 
 * @param handler Handler representing protected content
 * @param publishingLicenseBuffer [Output] Buffer to which publishing license will be written
 * @param publishingLicenseBufferSize Size of publishing license buffer
 * @param actualPublishingLicenseSize [Output] Actual size of publishing license (in bytes)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If publishingLicenseBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualPublishingLicenseSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_GetSerializedPublishingLicense(
    const mip_cc_protection_handler handler,
    uint8_t* publishingLicenseBuffer,
    const int64_t publishingLicenseBufferSize,
    int64_t* actualPublishingLicenseSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of pre-license (in bytes)
 * 
 * @param handler Handler representing protected content
 * @param format Pre-license format
 * @param preLicenseBufferSize [Output] Size of pre-license (in bytes)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_GetSerializedPreLicenseSize(
    const mip_cc_protection_handler handler,
    mip_cc_pre_license_format format,
    int64_t* preLicenseBufferSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets pre-license
 * 
 * @param handler Handler representing protected content
 * @param format Pre-license format
 * @param preLicenseBuffer [Output] Buffer to which pre-license will be written
 * @param preLicenseBufferSize Size of pre-license buffer
 * @param actualPreLicenseSize [Output] Actual size of pre-license (in bytes)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If preLicenseBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualPreLicenseSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_GetSerializedPreLicense(
    const mip_cc_protection_handler handler,
    mip_cc_pre_license_format format,
    uint8_t* preLicenseBuffer,
    const int64_t preLicenseBufferSize,
    int64_t* actualPreLicenseSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets protection descriptor
 * 
 * @param handler Handler representing protected content
 * @param descriptor [Output] Protection descriptor
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_GetProtectionDescriptor(
    const mip_cc_protection_handler handler,
    mip_cc_protection_descriptor* descriptor,
    mip_cc_error* errorInfo);

/**
 * @brief Gets list of rights granted to a user
 * 
 * @param handler Handler representing protected content
 * @param rights [Output] List of rights granted to a user, memory owned by caller
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The 'rights' variable must be released by the caller by calling MIP_CC_ReleaseStringList
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_GetRights(
    const mip_cc_protection_handler handler,
    mip_cc_string_list* rights,
    mip_cc_error* errorInfo);

/**
 * @brief Calculates size of protected content, factoring in padding, etc.
 * 
 * @param handler Handler representing protected content
 * @param unprotectedSize Size of unprotected/cleartext content (in bytes)
 * @param includesFinalBlock Describes if the unprotected content in question includes the final block or not.
 *        For example, in CBC4k encryption mode, non-final protected blocks are the same size as unprotected blocks, but
 *        final protected blocks are larger than their unprotected counterparts.
 * @param protectedSize [Output] Size of protected content
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_GetProtectedContentSize(
    const mip_cc_protection_handler handler,
    const int64_t unprotectedSize,
    const bool includesFinalBlock,
    int64_t* protectedSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the block size (in bytes) for the cipher mode used by a protection handler
 * 
 * @param handler Handler representing protected content
 * @param blockSize [Output] Block size (in bytes)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_GetBlockSize(
    const mip_cc_protection_handler handler,
    int64_t* blockSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of buffer required to store user that has been granted access to protected content
 * 
 * @param handler Handler representing protected content
 * @param issuedUserSize [Output] Size of buffer to hold issued user (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_GetIssuedUserSize(
    const mip_cc_protection_handler handler,
    int64_t* issuedUserSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the user that has been granted access to protected content
 * 
 * @param handler Handler representing protected content
 * @param issuedUserBuffer [Output] Buffer the issued user will be copied into.
 * @param issuedUserBufferSize Size (in number of chars) of the issuedUserBuffer.
 * @param actualIssuedUserSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If issuedUserBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualIssuedUserSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_GetIssuedUser(
    const mip_cc_protection_handler handler,
    char* issuedUserBuffer,
    const int64_t issuedUserBufferSize,
    int64_t* actualIssuedUserSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of buffer required to store the owner of protected content
 * 
 * @param handler Handler representing protected content
 * @param ownerSize [Output] Size of buffer to hold issued user (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_GetOwnerSize(
    const mip_cc_protection_handler handler,
    int64_t* ownerSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the owner of protected content
 * 
 * @param handler Handler representing protected content
 * @param ownerBuffer [Output] Buffer the issued user will be copied into.
 * @param ownerBufferSize Size (in number of chars) of the ownerBuffer.
 * @param actualOwnerSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If ownerBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualOwnerSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_GetOwner(
    const mip_cc_protection_handler handler,
    char* ownerBuffer,
    const int64_t ownerBufferSize,
    int64_t* actualOwnerSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the content IE of protected content
 * 
 * @param handler Handler representing protected content
 * @param contentId [Output] Content ID
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @note Publishing licenses will have this identifier surrounded by curly braces "{}".
 * Those braces are removed from the value stored in contentId
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_GetContentId(
    const mip_cc_protection_handler handler,
    mip_cc_guid* contentId,
    mip_cc_error* errorInfo);

/**
 * @brief Gets whether or not protection handler uses deprecated crypto algorithm (ECB) for backwards compatibility
 * 
 * @param handler Handler representing protected content
 * @param doesUseDeprecatedAlgorithm [Output] Whether or not protection handler uses deprecated crypto algorithm
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_DoesUseDeprecatedAlgorithm(
    const mip_cc_protection_handler handler,
    bool* doesUseDeprecatedAlgorithm,
    mip_cc_error* errorInfo);

/**
 * @brief Encrypt a buffer
 *
 * @param offsetFromStart Relative position of inputBuffer from the very beginning of the cleartext content
 * @param inputBuffer Buffer of cleartext content that will be encrypted
 * @param inputBufferSize Size (in bytes) of input buffer
 * @param outputBuffer [Output] Buffer into which encrypted content will be copied
 * @param outputBufferSize Size (in bytes) of output buffer
 * @param isFinal If input buffer contains the final cleartext bytes or not
 * @param actualEncryptedSize [Output] Actual size of encrypted content (in bytes)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_EncryptBuffer(
    const mip_cc_protection_handler handler,
    const int64_t offsetFromStart,
    const uint8_t* inputBuffer,
    const int64_t inputBufferSize,
    uint8_t* outputBuffer,
    const int64_t outputBufferSize,
    const bool isFinal,
    int64_t* actualEncryptedSize,
    mip_cc_error* errorInfo);

/**
 * @brief Decrypt a buffer
 * 
 * @param offsetFromStart Relative position of inputBuffer from the very beginning of the encrypted content
 * @param inputBuffer Buffer of encrypted content that will be decrypted
 * @param inputBufferSize Size (in bytes) of input buffer
 * @param outputBuffer [Output] Buffer into which decrypted content will be copied
 * @param outputBufferSize Size (in bytes) of output buffer
 * @param isFinal If input buffer contains the final encrypted bytes or not
 * @param actualDecryptedSize [Output] Actual size of encrypted content (in bytes)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_DecryptBuffer(
    const mip_cc_protection_handler handler,
    const int64_t offsetFromStart,
    const uint8_t* inputBuffer,
    const int64_t inputBufferSize,
    uint8_t* outputBuffer,
    const int64_t outputBufferSize,
    const bool isFinal,
    int64_t *actualDecryptedSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the cipher mode of the protection handler
 * 
 * @param handler Handler representing protected content
 * @param cipherMode [Output] The cipher mode
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandler_GetCipherMode(
    const mip_cc_protection_handler handler,
    mip_cc_cipher_mode* cipherMode,
    mip_cc_error* errorInfo);

/**
 * @brief Release resources associated with a protection handler settings
 * 
 * @param settings Protection handler settings to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseProtectionHandlerPublishingSettings(mip_cc_protection_handler_publishing_settings settings);

/**
 * @brief Release resources associated with a protection handler settings
 * 
 * @param settings Protection handler settings to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseProtectionHandlerConsumptionSettings(mip_cc_protection_handler_consumption_settings settings);

/**
 * @brief Add a capability to the set of capabilities supported when calling RMS
 * 
 * @param consumptionSettings Consumption handler settings
 * @param capability The capability to add to indicate a supported feature to RMS
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandlerConsumptionSettings_AddRmsCapability(
    const mip_cc_protection_handler_consumption_settings consumptionSettings,
    const char* capability,
    mip_cc_error* errorInfo);

/**
 * @brief Remove a capability from the set of capabilities supported when calling RMS
 * 
 * @param consumptionSettings Consumption handler settings
 * @param capability The capability to remove from the supported features in RMS
 * @param removed [Output] A boolean indiciating if the supplied capability was removed.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandlerConsumptionSettings_RemoveRmsCapability(
    const mip_cc_protection_handler_consumption_settings consumptionSettings,
    const char* capability,
    bool* removed,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the set of supported capabilities being passed to RMS
 * 
 * @param consumptionSettings Consumption handler settings
 * @param capabilities [Output] Gets the list of capabilities being passed to RMS
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The returned 'capabilities' (mip_cc_string_list*) must be freed by 
 *       calling MIP_CC_ReleaseStringList. 
*/
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionHandlerConsumptionSettings_GetRmsCapability ( 
    const mip_cc_protection_handler_consumption_settings consumptionSettings, 
    mip_cc_string_list* capabilities, 
    mip_cc_error* errorInfo); 

/**
 * @brief Release resources associated with a protection handler
 * 
 * @param handler Protection handler to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseProtectionHandler(mip_cc_protection_handler handler);
#endif // API_MIP_PROTECTION_PROTECTION_HANDLER_CC_H_