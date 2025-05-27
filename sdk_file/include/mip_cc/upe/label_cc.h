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
* @brief Contains C API definitions for label
* 
* @file label_cc.h
*/

#ifndef API_MIP_UPE_LABEL_CC_H_
#define API_MIP_UPE_LABEL_CC_H_

#include "mip_cc/common_types_cc.h"
#include "mip_cc/dictionary_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/result_cc.h"

typedef mip_cc_handle* mip_cc_label;

/**
 * @brief Release resources associated with a label
 * 
 * @param label Label to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseLabel(mip_cc_label label);

/**
 * @brief Gets label ID
 * 
 * @param label Label
 * @param labelId [Output] Label ID
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetId(
    const mip_cc_label label,
    mip_cc_guid* labelId,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of buffer required to store name
 * 
 * @param label Label
 * @param nameSize [Output] Size of buffer to hold name (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetNameSize(
    const mip_cc_label label,
    int64_t* nameSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets label name
 * 
 * @param label Label
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
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetName(
    const mip_cc_label label,
    char* nameBuffer,
    const int64_t nameBufferSize,
    int64_t* actualNameSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of buffer required to store description
 * 
 * @param label Label
 * @param descriptionSize [Output] Size of buffer to hold description (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetDescriptionSize(
    const mip_cc_label label,
    int64_t* descriptionSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets label description
 * 
 * @param label Label
 * @param descriptionBuffer [Output] Buffer the description will be copied into.
 * @param descriptionBufferSize Size (in number of chars) of the descriptionBuffer.
 * @param actualDescriptionSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If descriptionBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualDescriptionSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetDescription(
    const mip_cc_label label,
    char* descriptionBuffer,
    const int64_t descriptionBufferSize,
    int64_t* actualDescriptionSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of buffer required to store color
 * 
 * @param label Label
 * @param colorSize [Output] Size of buffer to hold color (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetColorSize(
    const mip_cc_label label,
    int64_t* colorSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets label color
 * 
 * @param label Label
 * @param colorBuffer [Output] Buffer the color will be copied into (in #RRGGBB format).
 * @param colorBufferSize Size (in number of chars) of the colorBuffer.
 * @param actualColorSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If colorBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualColorSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetColor(
    const mip_cc_label label,
    char* colorBuffer,
    const int64_t colorBufferSize,
    int64_t* actualColorSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets sensitivity level of label. Higher value means more sensitive.
 * 
 * @param label Label
 * @param sensitivity [Output] Sensitivity level
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetSensitivity(
    const mip_cc_label label,
    int32_t* sensitivity,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of buffer required to store tooltip
 * 
 * @param label Label
 * @param tooltipSize [Output] Size of buffer to hold tooltip (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetTooltipSize(
    const mip_cc_label label,
    int64_t* tooltipSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets label tooltip
 * 
 * @param label Label
 * @param tooltipBuffer [Output] Buffer the tooltip will be copied into.
 * @param tooltipBufferSize Size (in number of chars) of the tooltipBuffer.
 * @param actualTooltipSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If tooltipBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualTooltipSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetTooltip(
    const mip_cc_label label,
    char* tooltipBuffer,
    const int64_t tooltipBufferSize,
    int64_t* actualTooltipSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of buffer required to store auto-classification tooltip
 * 
 * @param label Label
 * @param tooltipSize [Output] Size of buffer to hold tooltip (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetAutoTooltipSize(
    const mip_cc_label label,
    int64_t* tooltipSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets label auto-classification tooltip
 * 
 * @param label Label
 * @param tooltipBuffer [Output] Buffer the tooltip will be copied into.
 * @param tooltipBufferSize Size (in number of chars) of the tooltipBuffer.
 * @param actualTooltipSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If tooltipBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualTooltipSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetAutoTooltip(
    const mip_cc_label label,
    char* tooltipBuffer,
    const int64_t tooltipBufferSize,
    int64_t* actualTooltipSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets whether or not a label is active
 * 
 * @param label Label
 * @param isActive [Output] Whether or not a label is considered active.
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note Only active labels can be applied. Inactivte labels cannot be applied and are used for display purposes only.
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_IsActive(
    const mip_cc_label label,
    bool* isActive,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the parent label, if any
 * 
 * @param label Label
 * @param parent [Output] Parent label, if any, else null
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetParent(
    const mip_cc_label label,
    mip_cc_label* parent,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the number of children labels
 * 
 * @param label Label
 * @param childrenSize [Output] Number of children
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetChildrenSize(
    const mip_cc_label label,
    int64_t* childrenSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the children labels
 * 
 * @param label Label
 * @param childrenBuffer [Output] Buffer the children labels will be copied into. Children labels
 *        are owned by the caller and must be released with MIP_CC_ReleaseLabel.
 * @param childrenBufferSize Size (in number of labels) of the childrenBuffer.
 * @param actualChildrenSize [Output] Number of children labels written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
  * 
 * @return Result code indicating success or failure
 * 
 * @note If childrenBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualChildrenSize will be set to the minimum required buffer size
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetChildren(
    const mip_cc_label label,
    mip_cc_label* childrenBuffer,
    const int64_t childrenBufferSize,
    int64_t* actualChildrenSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets policy-defined custom settings of a label
 * 
 * @param label Label
 * @param settings [Output] Dictionary of settings, owned by the caller
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The 'settings' variable must be released by the caller by calling MIP_CC_ReleaseDictionary
 */
MIP_CC_API(mip_cc_result) MIP_CC_Label_GetCustomSettings(
    const mip_cc_label label,
    mip_cc_dictionary* settings,
    mip_cc_error* errorInfo);

#endif // API_MIP_UPE_LABEL_CC_H_