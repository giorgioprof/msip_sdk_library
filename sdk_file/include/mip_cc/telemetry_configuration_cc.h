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
 * @brief Defines custom telemetry settings
 * 
 * @file telemetry_configuration_cc.h
 */

#ifndef API_MIP_TELEMETRY_CONFIGURATION_CC_H_
#define API_MIP_TELEMETRY_CONFIGURATION_CC_H_

#include <memory>

#include "mip_cc/common_types_cc.h"
#include "mip_cc/dictionary_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/http_delegate_cc.h"
#include "mip_cc/task_dispatcher_delegate_cc.h"

typedef mip_cc_handle* mip_cc_telemetry_configuration;

/**
 * @brief Create a settings object used to create a protection profile
 * 
 * @param telemetryConfig [Output] Newly-created telemetry configuration instance containing default settings
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateTelemetryConfiguration(
    mip_cc_telemetry_configuration* telemetryConfig,
    mip_cc_error* errorInfo);

/**
 * @brief Set a telemetry host name which will override internal telemetry settings
 * 
 * @param telemetryConfig Telemetry configuration
 * @param hostName Host name
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note This property is set when a client application uses the same Aria/1DS telemetry component and wishes for
 *       its internal telemetry settings (caching, logging, priority etc.) to be used instead of MIP's default settings
 */
MIP_CC_API(mip_cc_result) MIP_CC_TelemetryConfiguration_SetHostName(
    const mip_cc_telemetry_configuration telemetryConfig,
    const char* hostName,
    mip_cc_error* errorInfo);

/**
 * @brief Set a telemetry shared library override
 * 
 * @param telemetryConfig Telemetry configuration
 * @param libraryName Name of DLL that implements the Aria/1DS SDK's C API
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note This property is set when a client has an existing telemetry DLL that implements the Aria/1DS SDK's C API that
 *       should be used instead of mip_ClientTelemetry.dll
 */
MIP_CC_API(mip_cc_result) MIP_CC_TelemetryConfiguration_SetLibraryName(
    const mip_cc_telemetry_configuration telemetryConfig,
    const char* libraryName,
    mip_cc_error* errorInfo);

/**
 * @brief Override default telemetry HTTP stack with client's own
 * 
 * @param telemetryConfig Telemetry configuration
 * @param httpDelegate HTTP callback instance implemented by client application
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If this property is not set, the telemetry component will use MIP's default HTTP stack
 */
MIP_CC_API(mip_cc_result) MIP_CC_TelemetryConfiguration_SetHttpDelegate(
    const mip_cc_telemetry_configuration telemetryConfig,
    const mip_cc_http_delegate httpDelegate,
    mip_cc_error* errorInfo);

/**
 * @brief Override default async task dispatcher with client's own
 * 
 * @param telemetryConfig Telemetry configuration
 * @param taskDispatcherDelegate Task dispatcher callback instance implemented by client application
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_TelemetryConfiguration_SetTaskDispatcherDelegate(
    const mip_cc_telemetry_configuration telemetryConfig,
    const mip_cc_task_dispatcher_delegate taskDispatcherDelegate,
    mip_cc_error* errorInfo);

/**
 * @brief Sets whether or not the telemetry component is allowed ping network status on a background thread
 * 
 * @param telemetryConfig Telemetry configuration
 * @param isCachingEnabled Whether or not the telemetry component is allowed ping network status on a background thread
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note Default is 'true'
 */
MIP_CC_API(mip_cc_result) MIP_CC_TelemetryConfiguration_SetIsNetworkDetectionEnabled(
    const mip_cc_telemetry_configuration telemetryConfig,
    const bool isNetworkDetectionEnabled,
    mip_cc_error* errorInfo);

/**
 * @brief Sets whether or not the telemetry component is allowed to write caches to disk
 * 
 * @param telemetryConfig Telemetry configuration
 * @param isCachingEnabled Whether or not the telemetry component is allowed to write caches to disk
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note Default is 'true'
 */
MIP_CC_API(mip_cc_result) MIP_CC_TelemetryConfiguration_SetIsLocalCachingEnabled(
    const mip_cc_telemetry_configuration telemetryConfig,
    const bool isCachingEnabled,
    mip_cc_error* errorInfo);

/**
 * @brief Sets whether or not the telemetry component is allowed to write logs to disk
 * 
 * @param telemetryConfig Telemetry configuration
 * @param isTraceLoggingEnabled Whether or not the telemetry component is allowed to write logs to disk.
 * These files can grow to be very large, and may require manual cleanup from the host application.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note Default is 'true'
 */
MIP_CC_API(mip_cc_result) MIP_CC_TelemetryConfiguration_SetIsTraceLoggingEnabled(
    const mip_cc_telemetry_configuration telemetryConfig,
    const bool isTraceLoggingEnabled,
    mip_cc_error* errorInfo);

/**
 * @brief Sets whether or not an application/user has opted out of optional telemetry
 * 
 * @param telemetryConfig Telemetry configuration
 * @param isMinimalTelemetryEnabled Whether or not an application/user has opted out of optional telemetry
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note Default is 'false'
 */
MIP_CC_API(mip_cc_result) MIP_CC_TelemetryConfiguration_SetIsTelemetryOptedOut(
    const mip_cc_telemetry_configuration telemetryConfig,
    const bool isMinimalTelemetryEnabled,
    mip_cc_error* errorInfo);

/**
 * @brief Sets whether fast telemetry shutdown is enabled or not
 *
 * @param telemetryConfig Telemetry configuration
 * @param isFastShutdownEnabled Whether or not the telemetry fast shutdown is enabled
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 *
 * @return Result code indicating success or failure
 *
 * @note Default is 'false'
 */
MIP_CC_API(mip_cc_result) MIP_CC_TelemetryConfiguration_SetIsFastShutdownEnabled(
    const mip_cc_telemetry_configuration telemetryConfig, 
    const bool isFastShutdownEnabled,
    mip_cc_error* errorInfo);

/**
 * @brief Sets custom telemetry settings
 * 
 * @param telemetryConfig Telemetry configuration
 * @param customSettings Custom telemetry settings
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_TelemetryConfiguration_SetCustomSettings(
    const mip_cc_telemetry_configuration telemetryConfig,
    const mip_cc_dictionary customSettings,
    mip_cc_error* errorInfo);

/**
 * @brief Sets a telemetry property to mask
 * 
 * @param telemetryConfig Telemetry configuration
 * @param eventName Event name
 * @param propertyName Property name
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_TelemetryConfiguration_AddMaskedProperty(
    const mip_cc_telemetry_configuration telemetryConfig,
    const char* eventName,
    const char* propertyName,
    mip_cc_error* errorInfo);

/**
 * @brief Override default async task dispatcher with client's own
 * 
 * @param telemetryConfig Telemetry configuration
 * @param taskDispatcherDelegate Task dispatcher callback instance implemented by client application
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_TelemetryConfiguration_SetTaskDispatcherDelegate(
    const mip_cc_telemetry_configuration telemetryConfig,
    const mip_cc_task_dispatcher_delegate taskDispatcherDelegate,
    mip_cc_error* errorInfo);

/**
 * @brief Release resources associated with a protection profile settings
 * 
 * @param profileSettings Protection profile settings to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseTelemetryConfiguration(mip_cc_telemetry_configuration telemetryConfig);

#endif  // API_MIP_TELEMETRY_CONFIGURATION_CC_H_

