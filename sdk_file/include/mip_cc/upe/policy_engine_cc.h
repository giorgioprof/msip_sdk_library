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
 * @brief Contains C API definitions for policy engine
 * 
 * @file policy_engine_cc.h
 */

#ifndef API_MIP_UPE_POLICY_ENGINE_CC_H_
#define API_MIP_UPE_POLICY_ENGINE_CC_H_

#include "mip_cc/auth_callback_cc.h"
#include "mip_cc/common_types_cc.h"
#include "mip_cc/dictionary_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/identity_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/result_cc.h"
#include "mip_cc/upe/policy_handler_cc.h"
#include "mip_cc/upe/sensitivity_type_cc.h"

typedef mip_cc_handle* mip_cc_policy_engine;
typedef mip_cc_handle* mip_cc_policy_engine_settings;

/**
 * @brief Release resources associated with a policy engine
 * 
 * @param engine Policy engine to release
 */
MIP_CC_API(void) MIP_CC_ReleasePolicyEngine(mip_cc_policy_engine engine);

/**
 * @brief Gets size of buffer required to engine ID
 * 
 * @param engine Policy engine
 * @param idSize [Output] Size of buffer to hold engine ID (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetEngineIdSize(
    const mip_cc_policy_engine engine,
    int64_t* idSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets engine ID
 * 
 * @param engine Policy engine
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
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetEngineId(
    const mip_cc_policy_engine engine,
    char* idBuffer,
    const int64_t idBufferSize,
    int64_t* actualIdSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the size of client data associated with a policy engine
 * 
 * @param engine Policy engine
 * @param moreInfoUrlSize [Output] Size of client data (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetMoreInfoUrlSize(
    const mip_cc_policy_engine engine,
    int64_t* moreInfoUrlSize,
    mip_cc_error* errorInfo);

/**
 * @brief Get client data associated with a policy engine
 * 
 * @param engine Policy engine
 * @param moreInfoUrlBuffer [Output] Buffer the client data will be copied into
 * @param moreInfoUrlBufferSize Size (in number of chars) of moreInfoUrlBuffer.
 * @param actualMoreInfoUrlSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If moreInfoUrlBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualMoreInfoUrlSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetMoreInfoUrl(
    const mip_cc_policy_engine engine,
    char* moreInfoUrlBuffer,
    const int64_t moreInfoUrlBufferSize,
    int64_t* actualMoreInfoUrlSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets whether or not the policy dictates that a document must be labeled.
 * 
 * @param engine Policy engine
 * @param isLabelingRequired [Output] Whether or not policy dictates that a document must be labeled
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_IsLabelingRequired(
    const mip_cc_policy_engine engine,
    bool* isLabelingRequired,
    mip_cc_error* errorInfo);

/**
 * @brief Gets whether or not the policy dictates a justification for downgrade operations is required.
 * 
 * @param engine Policy engine
 * @param isDowngradeJustificationRequired [Output] Whether or a justification for downgrade operations is required.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_IsDowngradeJustificationRequired(
    const mip_cc_policy_engine engine,
    bool* isDowngradeJustificationRequired,
    mip_cc_error* errorInfo);
    
/**
 * @brief Gets the size of client data associated with a policy engine
 * 
 * @param engine Policy engine
 * @param policyFileIdSize [Output] Size of client data (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetPolicyFileIdSize(
    const mip_cc_policy_engine engine,
    int64_t* policyFileIdSize,
    mip_cc_error* errorInfo);

/**
 * @brief Get client data associated with a policy engine
 * 
 * @param engine Policy engine
 * @param policyFileIdBuffer [Output] Buffer the client data will be copied into
 * @param policyFileIdBufferSize Size (in number of chars) of policyFileIdBuffer.
 * @param actualPolicyFileIdSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If policyFileIdBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualPolicyFileIdSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetPolicyFileId(
    const mip_cc_policy_engine engine,
    char* policyFileIdBuffer,
    const int64_t policyFileIdBufferSize,
    int64_t* actualPolicyFileIdSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the size of client data associated with a policy engine
 * 
 * @param engine Policy engine
 * @param sensitivityFileIdSize [Output] Size of client data (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetSensitivityFileIdSize(
    const mip_cc_policy_engine engine,
    int64_t* sensitivityFileIdSize,
    mip_cc_error* errorInfo);

/**
 * @brief Get client data associated with a policy engine
 * 
 * @param engine Policy engine
 * @param sensitivityFileIdBuffer [Output] Buffer the client data will be copied into
 * @param sensitivityFileIdBufferSize Size (in number of chars) of sensitivityFileIdBuffer.
 * @param actualSensitivityFileIdSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If sensitivityFileIdBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualSensitivityFileIdSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetSensitivityFileId(
    const mip_cc_policy_engine engine,
    char* sensitivityFileIdBuffer,
    const int64_t sensitivityFileIdBufferSize,
    int64_t* actualSensitivityFileIdSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets whether or not the policy has automatic or recommendation rules
 * 
 * @param engine Policy engine
 * @param hasClassificationRules [Output] Whether or not policy has automatic or recommendation rules
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_HasClassificationRules(
    const mip_cc_policy_engine engine,
    bool* hasClassificationRules,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the time when the policy was last fetched
 * 
 * @param engine Policy engine
 * @param lastPolicyFetchTime [Output] Time when the policy was last fetched (in seconds since epoch)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetLastPolicyFetchTime(
    const mip_cc_policy_engine engine,
    int64_t* lastPolicyFetchTime,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the number of sensitivity labels associated with the policy engine
 * 
 * @param engine Policy engine
 * @param labelsSize [Output] Number of labels
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetSensitivityLabelsSize(
    const mip_cc_policy_engine engine,
    int64_t* labelsSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the sensitivity labels associated with the policy engine
 * 
 * @param engine Policy engine
 * @param labelBuffer [Output] Buffer the labels will be copied into. Labels are owned by the client
 *        application and must be released with MIP_CC_ReleaseLabel.
 * @param labelBufferSize Size (in number of labels) of the labelBuffer.
 * @param actualLabelsSize [Output] Number of labels written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
  * 
 * @return Result code indicating success or failure
 * 
 * @note If labelBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualLabelsSize will be set to the minimum required buffer size
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetSensitivityLabels(
    const mip_cc_policy_engine engine,
    mip_cc_label* labelBuffer,
    const int64_t labelBufferSize,
    int64_t* actualLabelsSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets sensitivity label by ID
 * 
 * @param engine Policy engine
 * @param labelId Label ID
 * @param label [Output] Sensitivity label. This value is owned by the caller and must be released with MIP_CC_ReleaseLabel.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetLabelById(
    const mip_cc_policy_engine engine,
    const char* labelId,
    mip_cc_label* label,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the number of sensitivity types associated with the policy engine
 * 
 * @param engine Policy engine
 * @param sensitivityTypesSize [Output] Number of sensitivity types
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetSensitivityTypesSize(
    const mip_cc_policy_engine engine,
    int64_t* sensitivityTypesSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the sensitivity types associated with the policy engine
 * 
 * @param engine Policy engine
 * @param sensitivityTypeBuffer [Output] Buffer the sensitivity types will be copied into. Sensitivity
 *        types are owned by the caller and must be released with MIP_CC_ReleaseSensitivityType.
 * @param sensitivityTypeBufferSize Size (in number of sensitivity types) of the sensitivityTypeBuffer.
 * @param actualSensitivityTypesSize [Output] Number of sensitivity types written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
  * 
 * @return Result code indicating success or failure
 * 
 * @note If sensitivityTypeBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualSensitivityTypesSize will be set to the minimum required buffer size
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetSensitivityTypes(
    const mip_cc_policy_engine engine,
    mip_cc_sensitivity_type* sensitivityTypeBuffer,
    const int64_t sensitivityTypeBufferSize,
    int64_t* actualSensitivityTypesSize,
    mip_cc_error* errorInfo);

/**
 * @brief Create a Policy Handler to execute policy-related functions
 * 
 * @param engine Policy engine
 * @param isAuditDiscoveryEnabled Whether or not audit discovery is enabled
 * @param handler [Output] Newly-created Policy Handler instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_CreatePolicyHandler(
    const mip_cc_policy_engine engine,
    const bool isAuditDiscoveryEnabled,
    mip_cc_policy_handler* handler,
    mip_cc_error* errorInfo);

/**
 * @brief Logs an application specific event to the audit pipeline
 * 
 * @param level Level of the event: Info/Error/Warning
 * @param eventType A description of the type of event
 * @param eventData The data associated with the event
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_SendApplicationAuditEvent(
    const mip_cc_policy_engine engine,
    const char* level,
    const char* eventType,
    const char* eventData,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of tenant ID
 * 
 * @param engine Policy engine
 * @param tenantIdSize [Output] Size of tenant ID (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetTenantIdSize(
    const mip_cc_policy_engine engine,
    int64_t* tenantIdSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets tenant ID
 * 
 * @param engine Policy engine
 * @param tenantIdBuffer [Output] Buffer the tenant ID will be copied into.
 * @param tenantIdBufferSize Size (in number of chars) of the tenantIdBuffer.
 * @param actualTenantIdSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If tenantIdBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualTenantIdSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetTenantId(
    const mip_cc_policy_engine engine,
    char* tenantIdBuffer,
    const int64_t tenantIdBufferSize,
    int64_t* actualTenantIdSize,
    mip_cc_error* errorInfo);
    

/**
 * @brief Gets size of policy data xml
 * 
 * @param engine Policy engine
 * @param xmlSize [Output] Size of policy data xml (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetPolicyDataXmlSize(
    const mip_cc_policy_engine engine,
    int64_t* xmlSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets policy data xml
 * 
 * @param engine Policy engine
 * @param xmlBuffer [Output] Buffer the xml will be copied into.
 * @param xmlBufferSize Size (in number of chars) of the xmlBuffer.
 * @param actualXmlSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If xmlBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualXmlSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetPolicyDataXml(
    const mip_cc_policy_engine engine,
    char* xmlBuffer,
    const int64_t xmlBufferSize,
    int64_t* actualXmlSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of sensitivity types data xml
 * 
 * @param engine Policy engine
 * @param xmlSize [Output] Size of policy data xml (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetSensitivityTypesDataXmlSize(
    const mip_cc_policy_engine engine,
    int64_t* xmlSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets sensitivity types data xml
 * 
 * @param engine Policy engine
 * @param xmlBuffer [Output] Buffer the xml will be copied into.
 * @param xmlBufferSize Size (in number of chars) of the xmlBuffer.
 * @param actualXmlSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If xmlBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualXmlSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetSensitivityTypesDataXml(
    const mip_cc_policy_engine engine,
    char* xmlBuffer,
    const int64_t xmlBufferSize,
    int64_t* actualXmlSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the size of client data associated with a policy engine
 * 
 * @param engine Policy engine
 * @param clientDataSize [Output] Size of client data (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetClientDataSize(
    const mip_cc_policy_engine engine,
    int64_t* clientDataSize,
    mip_cc_error* errorInfo);

/**
 * @brief Get client data associated with a policy engine
 * 
 * @param engine Policy engine
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
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetClientData(
    const mip_cc_policy_engine engine,
    char* clientDataBuffer,
    const int64_t clientDataBufferSize,
    int64_t* actualClientDataSize,
    mip_cc_error* errorInfo);

/**
 * @deprecated See the replacement MIP_CC_CreatePolicyEngineSettingsWithIdentityAndAuthCallback
 * @brief Create a settings object used to create a brand new policy engine
 * 
 * @param identity Identity that will be associated with PolicyEngine
 * @param clientData Customizable client data that is stored alongside the engine
 * @param locale Locale in which text results will output
 * @param loadSensitivityTypes Whether or not sensitivity types data (for classification) should also be loaded
 * @param settings [Output] Newly-created settings instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 *
 * @note 'loadSensitivityTypes' should be 'true' only if application expects to later call
 *        MIP_CC_PolicyEngine_GetSensitivityTypes. Otherwise, it should be false to avoid an unnecessary HTTP operation.
 */
MIP_CC_DEPRECATED_API(mip_cc_result) MIP_CC_CreatePolicyEngineSettingsWithIdentity(
    const mip_cc_identity* identity,
    const char* clientData,
    const char* locale,
    bool loadSensitivityTypes,
    mip_cc_policy_engine_settings* settings,
    mip_cc_error* errorInfo);

/**
 * @deprecated See the replacement MIP_CC_CreatePolicyEngineSettingsWithEngineIdAndAuthCallback
 * @brief Create a settings object used to load an existing policy engine by engine ID if it
 *        already exists, else create a new engine
 * 
 * @param engineId ID of existing cached engine
 * @param identity (Optional) Identity that will be associated with PolicyEngine
 * @param clientData Customizable client data that is stored alongside the engine
 * @param locale Locale in which text results will output
 * @param loadSensitivityTypes Whether or not sensitivity types data (for classification) should also be loaded
 * @param settings [Output] Newly-created settings instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note 'loadSensitivityTypes' should be 'true' only if application expects to later call
 *        MIP_CC_PolicyEngine_GetSensitivityTypes. Otherwise, it should be false to avoid an unnecessary HTTP operation.
 * 
 * @note 'identity' will only be used for new engines. If an engine with the specified engine id already exists in
 *       the local cache, the identity from the cached engine will be used instead.
 */
MIP_CC_DEPRECATED_API(mip_cc_result) MIP_CC_CreatePolicyEngineSettingsWithEngineId(
    const char* engineId,
    const mip_cc_identity* identity,
    const char* clientData,
    const char* locale,
    bool loadSensitivityTypes,
    mip_cc_policy_engine_settings* settings,
    mip_cc_error* errorInfo);

/**
 * @brief Create a settings object used to create a brand new policy engine
 * 
 * @param identity Identity that will be associated with PolicyEngine
 * @param authCallback Callback object to be used for authentication, implemented by client application
 * @param clientData Customizable client data that is stored alongside the engine
 * @param locale Locale in which text results will output
 * @param loadSensitivityTypes Whether or not sensitivity types data (for classification) should also be loaded
 * @param settings [Output] Newly-created settings instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 *
 * @note 'loadSensitivityTypes' should be 'true' only if application expects to later call
 *        MIP_CC_PolicyEngine_GetSensitivityTypes. Otherwise, it should be false to avoid an unnecessary HTTP operation.
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreatePolicyEngineSettingsWithIdentityAndAuthCallback(
    const mip_cc_identity* identity,
    const mip_cc_auth_callback authCallback,
    const char* clientData,
    const char* locale,
    bool loadSensitivityTypes,
    mip_cc_policy_engine_settings* settings,
    mip_cc_error* errorInfo);

/**
 * @brief Create a settings object used to load an existing policy engine by engine ID if it
 *        already exists, else create a new engine
 * 
 * @param engineId ID of existing cached engine
 * @param identity (Optional) Identity that will be associated with PolicyEngine
 * @param authCallback Callback object to be used for authentication, implemented by client application
 * @param clientData Customizable client data that is stored alongside the engine
 * @param locale Locale in which text results will output
 * @param loadSensitivityTypes Whether or not sensitivity types data (for classification) should also be loaded
 * @param settings [Output] Newly-created settings instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note 'loadSensitivityTypes' should be 'true' only if application expects to later call
 *        MIP_CC_PolicyEngine_GetSensitivityTypes. Otherwise, it should be false to avoid an unnecessary HTTP operation.
 * 
 * @note 'identity' will only be used for new engines. If an engine with the specified engine id already exists in
 *       the local cache, the identity from the cached engine will be used instead.
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreatePolicyEngineSettingsWithEngineIdAndAuthCallback(
    const char* engineId,
    const mip_cc_identity* identity,
    const mip_cc_auth_callback authCallback,
    const char* clientData,
    const char* locale,
    bool loadSensitivityTypes,
    mip_cc_policy_engine_settings* settings,
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
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngineSettings_SetClientData(
    const mip_cc_policy_engine_settings settings,
    const char* clientData,
    mip_cc_error* errorInfo);

/**
 * @brief Configures custom settings, used for feature gating and testing.
 * 
 * @param settings Engine settings
 * @param customSettings Key/value pairs of custom settings
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngineSettings_SetCustomSettings(
    const mip_cc_policy_engine_settings settings,
    const mip_cc_dictionary customSettings,
    mip_cc_error* errorInfo);

/**
 * @brief Sets the session ID that can be used to correlate logs and telemetry
 * 
 * @param settings Engine settings
 * @param sessionId Session ID that represents the lifetime of a policy engine
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngineSettings_SetSessionId(
    const mip_cc_policy_engine_settings settings,
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
 * @note If cloud is not specified, then it will default to global cloud.
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngineSettings_SetCloud(
    const mip_cc_policy_engine_settings settings,
    const mip_cc_cloud cloud,
    mip_cc_error* errorInfo);

/**
 * @brief Sets base URL for all service requests
 * 
 * @param settings Engine settings
 * @param cloudEndpointBaseUrl Base URL (e.g. 'https://dataservice.protection.outlook.com')
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note This value will only be read and must be set for Cloud = MIP_CLOUD_CUSTOM
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngineSettings_SetCloudEndpointBaseUrl(
    const mip_cc_policy_engine_settings settings,
    const char* cloudEndpointBaseUrl,
    mip_cc_error* errorInfo);

/**
 * @brief Sets delegated user
 * 
 * @param settings Engine settings
 * @param delegatedUserEmail Email address of delegated user
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note A delegated user is specified when the authenticating user/application is acting on behalf of another user
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngineSettings_SetDelegatedUserEmail(
    const mip_cc_policy_engine_settings settings,
    const char* delegatedUserEmail,
    mip_cc_error* errorInfo);

/**
 * @deprecated See the replacement MIP_CC_PolicyEngineSettings_ConfigureFunctionality
 * @brief Sets label filter
 * 
 * @param settings Engine settings
 * @param labelFilter enum representing label filter, if not set default is hyok, doublekeyencryption
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 */
MIP_CC_DEPRECATED_API(mip_cc_result) MIP_CC_PolicyEngineSettings_SetLabelFilter(
    const mip_cc_policy_engine_settings settings,
    const mip_cc_label_filter labelFilter,
    mip_cc_error* errorInfo);

/**
 * @brief Enables or disables functionality
 * 
 * @param settings Engine settings
 * @param labelFilter enum representing type to configure
 * @param enabled True to enable, False to disable
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngineSettings_ConfigureFunctionality(
    const mip_cc_policy_engine_settings settings,
    const mip_cc_label_filter labelFilter,
    bool enabled,
    mip_cc_error* errorInfo);

/**
 * @brief Release resources associated with a policy engine settings
 * 
 * @param settings Policy engine settings to be released
 */
MIP_CC_API(void) MIP_CC_ReleasePolicyEngineSettings(mip_cc_policy_engine_settings settings);

/**
 * @brief  Gets the recommended WXP (Windows, Excel, Powerpoint) metadata version, currently 0 for old verion
 * 1 for co-authoring enabled version. 
 * 
 * @param engine Policy engine
 * @param metadataVersion [Output] WXP metadata version.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_GetWxpMetadataVersion(
    const mip_cc_policy_engine engine,
    int64_t* metadataVersion,
    mip_cc_error* errorInfo);

/**
 * @brief  Checks consent for a specific workload
 * 
 * @param engine Policy engine
 * @param metadataVersion [Output] WXP metadata version.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyEngine_HasWorkloadConsent(
    const mip_cc_policy_engine engine,
    mip_cc_workload workload,
    mip_cc_error* errorInfo);
#endif // API_MIP_PROTECTION_PROTECTION_ENGINE_CC_H_