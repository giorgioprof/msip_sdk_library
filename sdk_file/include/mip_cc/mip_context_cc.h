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
 * @brief Defines C-Style MipContext functions
 * 
 * @file mip_context_cc.h
 */

#ifndef API_MIP_MIP_CONTEXT_CC_H_
#define API_MIP_MIP_CONTEXT_CC_H_

#include <stdint.h>

#include "mip_cc/application_info_cc.h"
#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/flighting_feature_cc.h"
#include "mip_cc/logger_delegate_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/result_cc.h"
#include "mip_cc/telemetry_configuration_cc.h"

typedef mip_cc_handle* mip_cc_mip_context;

/**
 * @brief Defines a single feature's enabled/disabled state
 */
typedef struct {
  mip_cc_flighting_feature feature; /**< Feature name */
  bool value;                       /**< Enabled/disabled state */
} mip_cc_feature_override;

/**
 * @brief Create a MIP context to manage state shared across all profile instances
 * 
 * @param applicationInfo Info about application that is consuming the protection SDK
 * @param path File path under which logging, telemetry, and other protection collateral is stored
 * @param logLevel Minimum log level for .miplog
 * @param isOfflineOnly Enable/disable network operations (not all actions supported when offline)
 * @param loggerDelegateOverride (Optional) Logger override implementation
 * @param diagnosticOverride (Optional) Overridden telemetry settings. If NULL, default settings will be used
 * @param mipContext [Output] Newly-created MIP context instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateMipContext(
    const mip_cc_application_info* applicationInfo,
    const char* path,
    const mip_cc_log_level logLevel,
    const bool isOfflineOnly,
    const mip_cc_logger_delegate loggerDelegateOverride,
    const mip_cc_telemetry_configuration diagnosticOverride,
    mip_cc_mip_context* mipContext,
    mip_cc_error* errorInfo);

/**
 * @brief Create a MIP context to manage state shared across all profile instances
 * 
 * @param applicationInfo Info about application that is consuming the protection SDK
 * @param path File path under which logging, telemetry, and other protection collateral is stored
 * @param logLevel Minimum log level for .miplog
 * @param isOfflineOnly Enable/disable network operations (not all actions supported when offline)
 * @param loggerDelegateOverride (Optional) Logger override implementation
 * @param diagnosticOverride (Optional) Overridden telemetry settings. If NULL, default settings will be used
 * @param featureSettings (Optional) Array of custom feature overrides
 * @param featureSettingsSize Size of custom feature overrides (in # of overrides)
 * @param mipContext [Output] Newly-created MIP context instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateMipContextWithCustomFeatureSettings(
    const mip_cc_application_info* applicationInfo,
    const char* path,
    const mip_cc_log_level logLevel,
    const bool isOfflineOnly,
    const mip_cc_logger_delegate loggerDelegateOverride,
    const mip_cc_telemetry_configuration diagnosticOverride,
    const mip_cc_feature_override* featureSettings,
    const int64_t featureSettingsSize,
    mip_cc_mip_context* mipContext,
    mip_cc_error* errorInfo);

/**
 * @brief Release resources associated with a MIP context
 * 
 * @param mipContext MIP context to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseMipContext(mip_cc_mip_context mipContext);

#endif // API_MIP_MIP_CONTEXT_CC_H_
