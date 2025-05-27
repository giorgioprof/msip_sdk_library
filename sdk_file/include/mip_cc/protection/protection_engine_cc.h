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
 * @brief Defines C-Style ProtectionEngine functions
 * 
 * @file protection_engine_cc.h
 */

#ifndef API_MIP_PROTECTION_PROTECTION_ENGINE_CC_H_
#define API_MIP_PROTECTION_PROTECTION_ENGINE_CC_H_

#include <stdint.h>

#include "mip_cc/application_info_cc.h"
#include "mip_cc/auth_callback_cc.h"
#include "mip_cc/common_types_cc.h"
#include "mip_cc/consent_callback_cc.h"
#include "mip_cc/dictionary_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/protection/delegation_license_cc.h"
#include "mip_cc/protection/license_info_cc.h"
#include "mip_cc/protection/protection_handler_cc.h"
#include "mip_cc/protection/template_descriptor_cc.h"
#include "mip_cc/result_cc.h"
#include "mip_cc/string_list_cc.h"

typedef mip_cc_handle* mip_cc_protection_engine;
typedef mip_cc_handle* mip_cc_protection_engine_settings;

/**
 * @brief Release resources associated with a protection engine
 * 
 * @param engine Protection engine to release
 */
MIP_CC_API(void) MIP_CC_ReleaseProtectionEngine(mip_cc_protection_engine engine);

/**
 * @brief Creates a protection handler for publishing new content
 * 
 * @param engine Engine under which a handler will be created
 * @param settings Protection handler settings
 * @param context Client context that will be opaquely passed to HttpDelegate and AuthDelegate
 * @param handler [Output] Newly-created protection handler instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngine_CreateProtectionHandlerForPublishing(
    const mip_cc_protection_engine engine,
    const mip_cc_protection_handler_publishing_settings settings,
    const void* context,
    mip_cc_protection_handler* handler,
    mip_cc_error* errorInfo);

/**
 * @brief Creates a protection handler for consuming existing content
 * 
 * @param engine Engine under which a handler will be created
 * @param settings Protection handler settings
 * @param context Client context that will be opaquely passed to HttpDelegate and AuthDelegate
 * @param handler [Output] Newly-created protection handler instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngine_CreateProtectionHandlerForConsumption(
  const mip_cc_protection_engine engine,
  const mip_cc_protection_handler_consumption_settings settings,
  const void* context,
  mip_cc_protection_handler* handler,
  mip_cc_error* errorInfo);

/**
 * @brief Gets size of buffer required to engine ID
 * 
 * @param engine Protection engine
 * @param idSize [Output] Size of buffer to hold engine ID (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngine_GetEngineIdSize(
    const mip_cc_protection_engine engine,
    int64_t* idSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets engine ID
 * 
 * @param engine Protection engine
 * @param idBuffer [Output] Buffer the id will be copied into.
 * @param idBufferSize Size (in number of chars) of the idBuffer.
 * @param actualIdSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If idBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualIdSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngine_GetEngineId(
    const mip_cc_protection_engine engine,
    char* idBuffer,
    const int64_t idBufferSize,
    int64_t* actualIdSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the number of RMS templates associated with a protection engine
 * 
 * @param engine Protection engine
 * @param context Client context that will be opaquely passed to HttpDelegate and AuthDelegate
 * @param templatesSize [Output] Number of templates
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note This API may result in an independent HTTP operation. Consider using 'MIP_CC_ProtectionEngine_GetTemplates'
 *       directly with a pre-defined buffer to avoid unnecessary extra HTTP operations.
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngine_GetTemplatesSize(
    const mip_cc_protection_engine engine,
    const void* context,
    int64_t* templatesSize,
    mip_cc_error* errorInfo);

/**
 * @brief Get collection of templates available to a user
 * 
 * @param engine Protection engine
 * @param context Client context that will be opaquely passed to HttpDelegate and AuthDelegate
 * @param mip_cc_template_descriptor [Output] buffer to create template handlers.
 * @param templateBufferSize Size (in number of items) of the templateBuffer.
 * @param actualTemplatesSize [Output] Number of template IDs written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If templateBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualTemplateSize will be set to the minimum required buffer size. Each mip_cc_template_descriptor must
 *       be released by the caller by calling MIP_CC_ReleaseTemplateDescriptor().
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngine_GetTemplates(
    const mip_cc_protection_engine engine,
    const void* context,
    mip_cc_template_descriptor* templateDescriptors,
    const int64_t templateBufferSize,
    int64_t* actualTemplatesSize,
    mip_cc_error* errorInfo);

/**
 * @brief Get list of rights granted to a user for a label ID
 * 
 * @param engine Protection engine
 * @param context Client context that will be opaquely passed to HttpDelegate and AuthDelegate
 * @param documentId Document ID assigned to the document
 * @param labelId Label ID applied to the document
 * @param ownerEmail Owner of the document
 * @param delagedUserEmail Email of user if the authenticating user/application is acting on behalf of another user, empty if none
 * @param rights [Output] List of rights granted to a user, memory owned by caller
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The 'rights' variable must be released by the caller by calling MIP_CC_ReleaseStringList
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngine_GetRightsForLabelId(
    const mip_cc_protection_engine engine,
    const void* context,
    const char* documentId,
    const char* labelId,
    const char* ownerEmail,
    const char* delegatedUserEmail,
    mip_cc_string_list* rights,
    mip_cc_error* errorInfo);

/**
 * @brief Get list of rights granted to a user for a label ID
 * 
 * @param engine Protection engine
 * @param context Client context that will be opaquely passed to the HttpDelegate and AuthDelegate
 * @param documentId Document ID assigned to the document
 * @param labelId Label ID applied to the document
 * @param ownerEmail Owner of the document
 * @param delagedUserEmail Email of the user if the authenticating user/application is acting on behalf of
 *                         another user, empty if none
 * @param licenseInfo [Output] LicenseInfo pertaining to a given label id. Memory owned by caller.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The returned 'licenseInfo' (mip_cc_license_info*) must be freed by calling MIP_CC_ReleaseLicenseInfo
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngine_GetLicenseInfoForLabelId(
    const mip_cc_protection_engine engine,
    const void* context,
    const char* documentId,
    const char* labelId,
    const char* ownerEmail,
    const char* delegatedUserEmail,
    mip_cc_license_info* licenseInfo,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the size of client data associated with a protection engine
 * 
 * @param engine Protection engine
 * @param clientDataSize [Output] Size of client data (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngine_GetClientDataSize(
    const mip_cc_protection_engine engine,
    int64_t* clientDataSize,
    mip_cc_error* errorInfo);

/**
 * @brief Get client data associated with a protection engine
 * 
 * @param engine Protection engine
 * @param clientDataBuffer [Output] Buffer the client data will be copied into
 * @param clientDataBufferSize Size (in number of chars) of clientDataBuffer.
 * @param actualClientDataSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If clientDataBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualClientDataSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngine_GetClientData(
    const mip_cc_protection_engine engine,
    char* clientDataBuffer,
    const int64_t clientDataBufferSize,
    int64_t* actualClientDataSize,
    mip_cc_error* errorInfo);

/**
 * @brief Get the delegation and end-user licenses associated with a protection license
 * 
 * @param engine The \ref mip_cc_protection_engine and associated identity to use when querying the licenses
 * @param settings A collection of \ref mip_cc_delegation_license_settings that determine which licenses to retrieve
 * @param context (Optional) Optional context that is passed along to any listening HttpDelegate.
 * @param delegationLicenses [Output] Returned collection of \ref mip_cc_delegation_license objects.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure

 * @note To avoid memory leaks, \p delegationLicenses must be released using \ref MIP_CC_ReleaseDelegationLicenses.
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngine_GetDelegationLicenses(
    const mip_cc_protection_engine engine,
    const mip_cc_delegation_license_settings settings,
    const void* context,
    mip_cc_delegation_licenses* delegationLicenses,
    mip_cc_error* errorInfo);

/**
 * @deprecated See the replacement MIP_CC_CreateProtectionEngineSettingsWithIdentityAndAuthCallback
 * @brief Create a settings object used to create a brand new protection engine
 * 
 * @param identity Identity that will be associated with ProtectionEngine
 * @param clientData Customizable client data that is stored alongside the engine
 * @param locale Locale in which text results will output
 * @param engineSettings [Output] Newly-created settings instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_DEPRECATED_API(mip_cc_result) MIP_CC_CreateProtectionEngineSettingsWithIdentity(
    const mip_cc_identity* identity,
    const char* clientData,
    const char* locale,
    mip_cc_protection_engine_settings* engineSettings,
    mip_cc_error* errorInfo);

/**
 * @deprecated See the replacement MIP_CC_CreateProtectionEngineSettingsWithEngineIdAndAuthCallback
 * @brief Create a settings object used to load an existing protection engine by engine ID if it
         already exists, else create a new engine
 * 
 * @param engineId ID of existing cached engine
 * @param identity (Optional) Identity that will be associated with ProtectionEngine
 * @param clientData Customizable client data that is stored alongside the engine
 * @param locale Locale in which text results will output
 * @param engineSettings [Output] Newly-created settings instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note 'identity' will only be used for new engines. If an engine with the specified engine id already exists in
 *       the local cache, the identity from the cached engine will be used instead.
 */
MIP_CC_DEPRECATED_API(mip_cc_result) MIP_CC_CreateProtectionEngineSettingsWithEngineId(
    const char* engineId,
    const mip_cc_identity* identity,
    const char* clientData,
    const char* locale,
    mip_cc_protection_engine_settings* engineSettings,
    mip_cc_error* errorInfo);

/**
 * @brief Create a settings object used to create a brand new protection engine
 * 
 * @param identity Identity that will be associated with ProtectionEngine
 * @param authCallback Callback object to be used for authentication, implemented by client application
 * @param clientData Customizable client data that is stored alongside the engine
 * @param locale Locale in which text results will output
 * @param engineSettings [Output] Newly-created settings instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateProtectionEngineSettingsWithIdentityAndAuthCallback(
    const mip_cc_identity* identity,
    const mip_cc_auth_callback authCallback,
    const char* clientData,
    const char* locale,
    mip_cc_protection_engine_settings* engineSettings,
    mip_cc_error* errorInfo);

/**
 * @brief Create a settings object used to load an existing protection engine by engine ID if it
         already exists, else create a new engine
 * 
 * @param engineId ID of existing cached engine
 * @param identity (Optional) Identity that will be associated with ProtectionEngine
 * @param authCallback Callback object to be used for authentication, implemented by client application
 * @param clientData Customizable client data that is stored alongside the engine
 * @param locale Locale in which text results will output
 * @param engineSettings [Output] Newly-created settings instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note 'identity' will only be used for new engines. If an engine with the specified engine id already exists in
 *       the local cache, the identity from the cached engine will be used instead.
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateProtectionEngineSettingsWithEngineIdAndAuthCallback(
    const char* engineId,
    const mip_cc_identity* identity,
    const mip_cc_auth_callback authCallback,
    const char* clientData,
    const char* locale,
    mip_cc_protection_engine_settings* engineSettings,
    mip_cc_error* errorInfo);

/**
 * @brief Sets the client data that will be stored opaquely alongside this engine and persist across sessions
 * 
 * @param settings Engine settings
 * @param clientData Client data
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngineSettings_SetClientData(
    const mip_cc_protection_engine_settings engineSettings,
    const char* clientData,
    mip_cc_error* errorInfo);

/**
 * @brief Configures custom settings, used for feature gating and testing.
 * 
 * @param engineSettings Engine settings
 * @param customSettings Key/value pairs of custom settings
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngineSettings_SetCustomSettings(
    const mip_cc_protection_engine_settings engineSettings,
    const mip_cc_dictionary customSettings,
    mip_cc_error* errorInfo);

/**
 * @brief Sets the session ID that can be used to correlate logs and telemetry
 * 
 * @param settings Engine settings
 * @param sessionId Session ID that represents the lifetime of a protection engine
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngineSettings_SetSessionId(
    const mip_cc_protection_engine_settings engineSettings,
    const char* sessionId,
    mip_cc_error* errorInfo);

/**
 * @brief Sets cloud which affects endpoint URLs for all service requests
 * 
 * @param settings Engine settings
 * @param cloud Cloud identifier (default = Unknown)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If cloud is not specified, then it will be determined by DNS lookup of the engine's
 *       identity domain if possible, else fall back to global cloud.
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngineSettings_SetCloud(
    const mip_cc_protection_engine_settings engineSettings,
    const mip_cc_cloud cloud,
    mip_cc_error* errorInfo);

/**
 * @brief Sets base URL for all service requests
 * 
 * @param settings Engine settings
 * @param cloudEndpointBaseUrl Base URL (e.g. 'https://api.aadrm.com')
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note This value will only be read and must be set for Cloud = MIP_CLOUD_CUSTOM
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngineSettings_SetCloudEndpointBaseUrl(
    const mip_cc_protection_engine_settings engineSettings,
    const char* cloudEndpointBaseUrl,
    mip_cc_error* errorInfo);

/**
 * @brief Sets underlying application ID
 *
 * @param settings Engine settings
 * @param underlyingApplicationId Application ID (could be a Guid)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 *
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngineSettings_SetUnderlyingApplicationId(
    const mip_cc_protection_engine_settings engineSettings,
    const char* underlyingApplicationId,
    mip_cc_error* errorInfo);

/**
 * @brief Sets whether or not only cloud service is allowed
 *
 * @param settings Engine settings
 * @param allowCloudServiceOnly A boolean value indicating whether or not only cloud service is allowed
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 *
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngineSettings_SetAllowCloudServiceOnly(
    const mip_cc_protection_engine_settings engineSettings,
    bool allowCloudServiceOnly,
    mip_cc_error* errorInfo);

/**
 * @brief Sets whether or not only cloud service is allowed
 *
 * @param settings Engine settings
 * @param templateRefreshIntervalHours How long, in hours, a cached template will be valid to use.
 * A value of 0 means templates should never be cached. This is the default.
 * @param templateBatchingSize If doing offline publishing, how many temlates will be fetched at a single time.
 * Max 25.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 *
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngineSettings_SetTemplateRefreshArgs(
    const mip_cc_protection_engine_settings engineSettings,
    int templateRefreshIntervalHours,
    int templateBatchingSize,
    mip_cc_error* errorInfo);

/**
 * @brief Add a capability to the set of capabilities supported when calling RMS
 * 
 * @param engineSettings Engine settings
 * @param capability The capability to add to indicate a supported feature to RMS
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
*/
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngineSettings_AddRmsCapability(
    const mip_cc_protection_engine_settings engineSettings,
    const char* capability,
    mip_cc_error* errorInfo);

/**
 * @brief Remove a capability from the set of capabilities supported when calling RMS
 * 
 * @param engineSettings Engine settings
 * @param capability The capability to remove from the supported features in RMS
 * @param removed [Output] A boolean indiciating if the supplied capability was removed.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
*/
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngineSettings_RemoveRmsCapability( 
    const mip_cc_protection_engine_settings engineSettings,
    const char* capability,
    bool* removed,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the set of supported capabilities being passed to RMS
 * 
 * @param engineSettings Engine settings
 * @param capabilities [Output] Gets the list of capabilities being passed to RMS
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The returned 'capabilities' (mip_cc_string_list*) must be freed by
 *       calling MIP_CC_ReleaseStringList.
*/
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionEngineSettings_GetRmsCapability(
    const mip_cc_protection_engine_settings engineSettings,
    mip_cc_string_list* capabilities,
    mip_cc_error* errorInfo);

/**
 * @brief Release resources associated with a protection engine settings
 * 
 * @param engineSettings Protection engine settings to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseProtectionEngineSettings(mip_cc_protection_engine_settings engineSettings);

#endif // API_MIP_PROTECTION_PROTECTION_ENGINE_CC_H_