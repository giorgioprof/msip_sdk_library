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
* @brief Contains C API definitions for "protect by template" label policy action
* 
* @file protect_by_template_action_cc.h
*/

#ifndef API_MIP_UPE_PROTECT_BY_TEMPLATE_ACTION_CC_H_
#define API_MIP_UPE_PROTECT_BY_TEMPLATE_ACTION_CC_H_

#include "mip_cc/error_cc.h"
#include "mip_cc/upe/action_cc.h"

/**
 * @brief Gets a "protect by template" action's template ID
 * 
 * @param action "protect by template" action
 * @param templateId [Output] ID of template that defines protections
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectByTemplateAction_GetTemplateId(
    const mip_cc_action action,
    mip_cc_guid* templateId,
    mip_cc_error* errorInfo);

/**
 * @brief Gets a "protect by template" action's whether or not it is double key
 * 
 * @param action "protect by template" action
 * @param isDoubleKey [Output] Whether or not this is a double key template
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectByTemplateAction_IsDoubleKey(
    const mip_cc_action action,
    bool* isDoubleKey,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of buffer required to store the double key encryption url.
 * 
 * @param action "protect by template" action
 * @param urlSize [Output] Size of buffer to hold the url (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectByTemplateAction_GetDoubleKeyEncryptionUrlSize(
    const mip_cc_action action,
    int64_t* urlSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets double key encryption url
 * 
 * @param action "protect by template" action
 * @param urlBuffer [Output] Buffer the url will be copied into.
 * @param urlBufferSize Size (in number of chars) of the urlBuffer.
 * @param actualUrlSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If urlBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualUrlSize will be set to the minimum required buffer size.
 * *     If the template action is not double key then MIP_RESULT_ERROR_NOT_SUPPORTED_OPERATION will be returned
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectByTemplateAction_GetDoubleKeyEncryptionUrl(
    const mip_cc_action action,
    char* urlBuffer,
    const int64_t urlBufferSize,
    int64_t* actualUrlSize,
    mip_cc_error* errorInfo);

#endif // API_MIP_UPE_PROTECT_BY_TEMPLATE_ACTION_CC_H_