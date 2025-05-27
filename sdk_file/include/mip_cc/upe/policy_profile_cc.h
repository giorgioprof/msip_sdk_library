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
 * @brief Contains C API definitions for policy profile
 * 
 * @file policy_profile_cc.h
 */

#ifndef API_MIP_UPE_POLICY_PROFILE_CC_H_
#define API_MIP_UPE_POLICY_PROFILE_CC_H_

#include <stdint.h>

#include "mip_cc/application_info_cc.h"
#include "mip_cc/auth_callback_cc.h"
#include "mip_cc/common_types_cc.h"
#include "mip_cc/dictionary_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/http_delegate_cc.h"
#include "mip_cc/mip_context_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/result_cc.h"
#include "mip_cc/task_dispatcher_delegate_cc.h"
#include "mip_cc/upe/policy_engine_cc.h"

typedef mip_cc_handle* mip_cc_policy_profile;
typedef mip_cc_handle* mip_cc_policy_profile_settings;

/**
 * @brief Add a new policy engine to the profile.
 *
 * @param profile Parent profile under which an engine will be added
 * @param settings Policy engine settings
 * @param context Client context that will be opaquely passed to HttpDelegate and AuthDelegate
 * @param engine [Output] Newly-created policy engine instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 *
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyProfile_AddEngine(
    const mip_cc_policy_profile profile,
    const mip_cc_policy_engine_settings settings,
    const void* context,
    mip_cc_policy_engine* engine,
    mip_cc_error* errorInfo);

/**
 * @brief Trigger an authentication callback
 * 
 * @param profile Profile
 * @param cloud Azure cloud
 * @param authCallback Authentication callback that will be invoked
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note MIP will not cache or do anything else with the value returned by the auth delegate. This function is
 *       recommended for applications that aren't "logged in" until after MIP requests an auth token. It allows
 *       an application to fetch a token before MIP actually requires one.
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyProfile_AcquireAuthToken(
    const mip_cc_policy_profile profile,
    const mip_cc_cloud cloud,
    const mip_cc_auth_callback authCallback,
    mip_cc_error* errorInfo);

/**
 * @brief Load a profile
 * 
 * @param settings Profile settings
 * @param profile [Output] Newly-created policy profile instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_LoadPolicyProfile(
    const mip_cc_policy_profile_settings settings,
    mip_cc_policy_profile* profile,
    mip_cc_error* errorInfo);

/**
 * @brief Release resources associated with a policy profile
 * 
 * @param profile Policy profile to be released
 */
MIP_CC_API(void) MIP_CC_ReleasePolicyProfile(mip_cc_policy_profile profile);

/**
 * @brief Create a settings object used to create a policy profile
 * 
 * @param mipContext Global context shared across all profiles
 * @param cacheStorageType Storage cache configuration
 * @param reserved Reserved for backwards compatibility - set to nullptr
 * @param settings [Output] Newly-created settings instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreatePolicyProfileSettings(
    const mip_cc_mip_context mipContext,
    const mip_cc_cache_storage_type cacheStorageType,
    const void* reserved,
    mip_cc_policy_profile_settings* settings,
    mip_cc_error* errorInfo);

/**
 * @brief Sets the session ID that can be used to correlate logs and telemetry
 * 
 * @param settings Profile settings
 * @param sessionId Session ID that represents the lifetime of a policy profile
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyProfileSettings_SetSessionId(
    const mip_cc_policy_profile_settings settings,
    const char* sessionId,
    mip_cc_error* errorInfo);

/**
 * @brief Override default HTTP stack with client's own
 * 
 * @param settings Profile settings to which HTTP delegate will be assigned
 * @param httpDelegate HTTP callback instance implemented by client application
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyProfileSettings_SetHttpDelegate(
    const mip_cc_policy_profile_settings settings,
    const mip_cc_http_delegate httpDelegate,
    mip_cc_error* errorInfo);

/**
 * @brief Override default async task dispatcher with client's own
 * 
 * @param settings Profile settings to which task dispatcher delegate will be assigned
 * @param taskDispatcherDelegate Task dispatcher callback instance implemented by client application
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyProfileSettings_SetTaskDispatcherDelegate(
    const mip_cc_policy_profile_settings settings,
    const mip_cc_task_dispatcher_delegate taskDispatcherDelegate,
    mip_cc_error* errorInfo);

/**
 * @brief Configures custom settings, used for feature gating and testing.
 * 
 * @param settings Profile settings
 * @param customSettings Key/value pairs of custom settings
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyProfileSettings_SetCustomSettings(
    const mip_cc_policy_profile_settings settings,
    const mip_cc_dictionary customSettings,
    mip_cc_error* errorInfo);

/**
 * @brief Release resources associated with a policy profile settings
 * 
 * @param settings Policy profile settings to be released
 */
MIP_CC_API(void) MIP_CC_ReleasePolicyProfileSettings(mip_cc_policy_profile_settings profileSettings);

#endif // API_MIP_PROTECTION_PROTECTION_PROFILE_H_
