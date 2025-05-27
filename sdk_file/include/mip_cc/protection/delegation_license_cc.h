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
 * @brief Defines C-Style DelegationLicense and DelegationLicenseSettings functions
 * 
 * @file delegation_license_settings_cc.h
 */

#ifndef API_MIP_PROTECTION_DELEGATION_LICENSE_CC_H_
#define API_MIP_PROTECTION_DELEGATION_LICENSE_CC_H_

#include "mip_cc/common_types_cc.h"
#include "mip_cc/dynamic_watermark_cc.h"
#include "mip_cc/mip_context_cc.h"
#include "mip_cc/protection/protection_handler_cc.h"
#include "mip_cc/string_list_cc.h"

/**
 * @brief Handle to a \ref DelegationLicense object
 * 
 */
typedef mip_cc_handle* mip_cc_delegation_license;
/**
 * @brief Handle to a DelegationLicenseSettings object
 * 
 */
typedef mip_cc_handle* mip_cc_delegation_license_settings;
/**
 * @brief Handle to a collection of \ref mip_cc_delegation_license objects
 * 
 */
typedef mip_cc_handle* mip_cc_delegation_licenses;

/**
 * @brief Creates the settings required for generating delegation licenses and end-user licenses.
 * 
 * @param mipContext Global context shared across all profiles
 * @param serializedPublishingLicense Buffer containing raw publishing license
 * @param publishingLicenseBufferSize Size of publishing license buffer
 * @param users List of users to obtain licenses on behalf of
 * @param acquireEndUserLicenses Set to 'true' to obtain end-user licenses that are encrypted using end-user keys
 * @param delegationLicenseSettings [Output] Handle to the new delegation license settings object
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note To avoid memory leaks, \p delegationLicenseSettings must be released using \ref MIP_CC_ReleaseDelegationLicenseSettings.
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateDelegationLicenseSettings(
    const mip_cc_mip_context mipContext,
    const uint8_t* serializedPublishingLicense,
    int32_t publishingLicenseBufferSize,
    mip_cc_string_list users,
    bool acquireEndUserLicenses,
    mip_cc_delegation_license_settings* delegationLicenseSettings,
    mip_cc_error* errorInfo);

/**
 * @brief Releases a \ref mip_cc_delegation_license_settings object
 * 
 * @param licenses The \ref mip_cc_delegation_license_settings object to release
 */
MIP_CC_API(void) MIP_CC_ReleaseDelegationLicenseSettings(
    mip_cc_delegation_license_settings licenses);

/**
 * @brief Gets an array of delegation license objects from a mip_cc_delegation_licenses object
 * 
 * @param licenses Handle to a collection of \ref mip_cc_delegation_license objects
 * @param delegationLicenseArray [Input/Output] Caller allocated array of \ref mip_cc_delegation_license objects.
 * Set this value to \c nullptr to determine the number of items in \p licenses.
 * @param count [Input/Output] Upon input, the size of \p delegationLicenseArray <em>in items</em>.
 * Upon output, the number of items written to \p delegationLicenseArray.
 * Each item is \c sizeof(mip_cc_delegation_license).
 * If \p delegationLicenseArray is \c nullptr, \c count returns the size required (in items) to store
 * the array of mip_cc_delegation_license objects.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 *
 * @note If the input array is too small, the input is truncated and
 * \ref MIP_RESULT_ERROR_INSUFFICIENT_BUFFER is returned. The caller is responsible for releasing
 * each returned \ref mip_cc_delegation_license by calling \ref MIP_CC_ReleaseDelegationLicense.
 */
MIP_CC_API(mip_cc_result) MIP_CC_DelegationLicenses_GetLicenses(
    const mip_cc_delegation_licenses licenses,
    mip_cc_delegation_license* delegationLicenseArray,
    int32_t* count,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the dynamic watermark from the delegation license object
 * 
 * @param license Handle to a \ref mip_cc_delegation_license object
 * @param dynamicWatermark [Output] Handle to the dynamic watermark (null if not present)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The returned \p dynamicWatermark (mip_cc_dynamic_watermark*) must be freed by
 * calling \ref MIP_CC_ReleaseDynamicWatermark
*/ 
MIP_CC_API(mip_cc_result) MIP_CC_DelegationLicense_GetDynamicWatermark ( 
    const mip_cc_delegation_license license, 
    mip_cc_dynamic_watermark* dynamicWatermark, 
    mip_cc_error* errorInfo); 

/**
 * @brief Releases a \ref mip_cc_delegation_licenses object
 * 
 * @param licenses The \ref mip_cc_delegation_licenses object to release
 */
MIP_CC_API(void) MIP_CC_ReleaseDelegationLicenses(
    mip_cc_delegation_licenses licenses);

/**
 * @brief Releases a \ref mip_cc_delegation_license object
 * 
 * @param license The \ref mip_cc_delegation_license object to release
 */
MIP_CC_API(void) MIP_CC_ReleaseDelegationLicense(
    mip_cc_delegation_license license);

/**
 * @brief Gets the delegation license in JSON format. For encryption operations, the delegation license
 * uses the key that is associated with the identity that retrieved the delegation license.
 * 
 * @param license Handle to the \ref mip_cc_delegation_license object
 * @param serializedLicense [Input/Output] Caller allocated buffer <em>in bytes</em> for the
 * JSON delegation license. Set this value to \c nullptr to determine the number of bytes in
 * \p serializedLicense.
 * @param serializedLicenseSize [Input/Output] Upon input, the size of \p serializedLicense
 * <em>in bytes</em>. Upon output, the number of bytes written to \p serializedLicense.
 * If \p serializedLicense is \c nullptr, \c serializedLicenseSize returns the size required
 * (in bytes) to store the \p serializedLicense.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 *
 * @note If the input buffer is too small, the input is truncated and
 * \ref MIP_RESULT_ERROR_INSUFFICIENT_BUFFER is returned. The input buffer is \e NOT guaranteed
 * to be null terminated.
 */
MIP_CC_API(mip_cc_result) MIP_CC_DelegationLicense_GetSerializedDelegationJsonLicense(
    const mip_cc_delegation_license license,
    uint8_t* serializedLicense,
    int32_t* serializedLicenseSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the delegation license in XRML format. For encryption operations, the delegation license
 * uses the key that is associated with the identity that retrieved the delegation license.
 * 
 * @param license Handle to the \ref mip_cc_delegation_license object
 * @param serializedLicense [Input/Output] Caller allocated buffer <em>in bytes</em> for the
 * XRML delegation license. Set this value to \c nullptr to determine the number of bytes in
 * \p serializedLicense.
 * @param serializedLicenseSize [Input/Output] Upon input, the size of \p serializedLicense
 * <em>in bytes</em>. Upon output, the number of bytes written to \p serializedLicense.
 * If \p serializedLicense is \c nullptr, \c serializedLicenseSize returns the size required
 * (in bytes) to store the \p serializedLicense.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 *
 * @note If the input buffer is too small, the input is truncated and
 * \ref MIP_RESULT_ERROR_INSUFFICIENT_BUFFER is returned. The input buffer is \e NOT guaranteed
 * to be null terminated.
 */
MIP_CC_API(mip_cc_result) MIP_CC_DelegationLicense_GetSerializedDelegationXrmlLicense(
    const mip_cc_delegation_license license,
    uint8_t* serializedLicense,
    int32_t* serializedLicenseSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the end-user license that can be accessed with the key associated with the end-user's identity.
 * 
 * @param license Handle to the \ref mip_cc_delegation_license object
 * @param format The \ref mip_cc_pre_license_format to use when retrieving the end-user license
 * @param serializedLicense [Input/Output] Caller allocated buffer <em>in bytes</em> for the
 * XRML delegation license. Set this value to \c nullptr to determine the number of bytes in
 * \p serializedLicense.
 * @param serializedLicenseSize [Input/Output] Upon input, the size of \p serializedLicense
 * <em>in bytes</em>. Upon output, the number of bytes written to \p serializedLicense.
 * If \p serializedLicense is \c nullptr, \c serializedLicenseSize returns the size required
 * (in bytes) to store the \p serializedLicense.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 *
 * @note If the input buffer is too small, the input is truncated and
 * \ref MIP_RESULT_ERROR_INSUFFICIENT_BUFFER is returned. The input buffer is \e NOT guaranteed
 * to be null terminated.
 */
MIP_CC_API(mip_cc_result) MIP_CC_DelegationLicense_GetSerializedUserLicense(
    const mip_cc_delegation_license license,
    const mip_cc_pre_license_format format,
    uint8_t* serializedLicense,
    int32_t* serializedLicenseSize,
    mip_cc_error* errorInfo);

/**
 * @brief Get the user associated with this collection of licenses
 * 
 * @param license Handle to the \ref mip_cc_delegation_license object
 * @param user [Output] Address of a null terminated character array that defines the user that is associated
 * with this license collection.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 *
 * @return Result code indicating success or failure
 * 
 * @note The memory associated with \p user is temporary and may not be accessible once other
 * \c MIP_CC_DelegationLicense calls are executed.
 */
MIP_CC_API(mip_cc_result) MIP_CC_DelegationLicense_GetUser(
    const mip_cc_delegation_license license,
    const char** user,
    mip_cc_error* errorInfo);

#endif // API_MIP_PROTECTION_DELEGATION_LICENSE_CC_H_