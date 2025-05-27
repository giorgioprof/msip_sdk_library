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
* @brief Contains C API definitions for content label
* 
* @file content_label_cc.h
*/

#ifndef API_MIP_UPE_CONTENT_LABEL_CC_H_
#define API_MIP_UPE_CONTENT_LABEL_CC_H_

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/result_cc.h"
#include "mip_cc/upe/label_cc.h"
#include "mip_cc/upe/metadata_dictionary_cc.h"

typedef mip_cc_handle* mip_cc_content_label;

/**
 * @brief Release resources associated with a content label
 * 
 * @param contentLabel Label to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseContentLabel(mip_cc_content_label contentLabel);

/**
 * @brief Gets time when label was applied
 * 
 * @param contentLabel Label
 * @param creationTime [Output] Time when label was applied to document (in seconds since epoch)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ContentLabel_GetCreationTime(
    const mip_cc_content_label contentLabel,
    int64_t* creationTime,
    mip_cc_error* errorInfo);

/**
 * @brief Gets label assignment method
 * 
 * @param contentLabel Label
 * @param assignmentMethod [Output] Assignment method (e.g. 'standard' or 'privileged')
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ContentLabel_GetAssignmentMethod(
    const mip_cc_content_label contentLabel,
    mip_cc_label_assignment_method* assignmentMethod,
    mip_cc_error* errorInfo);

/**
 * @brief Gets extended properties
 * 
 * @param contentLabel Label
 * @param properties [Output] Dictionary of extended properties, memory owned by caller
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The 'properties' variable must be released by the caller by calling MIP_CC_ReleaseDictionary
 */
MIP_CC_API(mip_cc_result) MIP_CC_ContentLabel_GetExtendedProperties(
    const mip_cc_content_label contentLabel,
    mip_cc_metadata_dictionary* properties,
    mip_cc_error* errorInfo);

/**
 * @brief Gets whether or not a protection was applied by a label.
 * 
 * @param contentLabel Label
 * @param isProtectionAppliedByLabel [Output] If document is protected and the protection was explicitly applied by this label.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ContentLabel_IsProtectionAppliedFromLabel(
    const mip_cc_content_label contentLabel,
    bool* isProtectionAppliedByLabel,
    mip_cc_error* errorInfo);

/**
 * @brief Gets generic label properties from a content label instance
 * 
 * @param contentLabel Label
 * @param label [Output] Generic label, memory owned by caller
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The 'label' variable must be released by the caller by calling MIP_CC_ReleaseLabel
 */
MIP_CC_API(mip_cc_result) MIP_CC_ContentLabel_GetLabel(
    const mip_cc_content_label contentLabel,
    mip_cc_label* label,
    mip_cc_error* errorInfo);

#endif // API_MIP_UPE_CONTENT_LABEL_CC_H_
#include "mip_cc/dictionary_cc.h"