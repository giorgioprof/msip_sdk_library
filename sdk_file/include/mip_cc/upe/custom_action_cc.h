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
* @brief Contains C API definitions for custom label policy action
* 
* @file custom_action_cc.h
*/

#ifndef API_MIP_UPE_CUSTOM_ACTION_CC_H_
#define API_MIP_UPE_CUSTOM_ACTION_CC_H_

#include "mip_cc/dictionary_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/upe/action_cc.h"

/**
 * @brief Gets size of buffer required to store a "custom" action's name
 * 
 * @param action "custom" action
 * @param nameSize [Output] Size of buffer to hold name (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CustomAction_GetNameSize(
    const mip_cc_action action,
    int64_t* nameSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets a "custom" action's name
 * 
 * @param action "custom" action
 * @param nameBuffer [Output] Buffer the name will be copied into.
 * @param nameBufferSize Size (in number of chars) of the nameBuffer.
 * @param actualNameSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If nameBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualNameSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_CustomAction_GetName(
    const mip_cc_action action,
    char* nameBuffer,
    const int64_t nameBufferSize,
    int64_t* actualNameSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets a "custom" action's properties
 * 
 * @param action "custom" action
 * @param properties [Output] Dictionary of properties, memory owned by caller
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The 'properties' variable must be released by the caller by calling MIP_CC_ReleaseDictionary
 */
MIP_CC_API(mip_cc_result) MIP_CC_CustomAction_GetProperties(
    const mip_cc_action action,
    mip_cc_dictionary* properties,
    mip_cc_error* errorInfo);

#endif // API_MIP_UPE_CUSTOM_ACTION_CC_H_