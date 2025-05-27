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
 * @brief Defines C-Style TemplateDescriptor functions
 * 
 * @file template_descriptor_cc.h
 */

#ifndef API_MIP_PROTECTION_TEMPLATE_DESCRIPTOR_CC_H_
#define API_MIP_PROTECTION_TEMPLATE_DESCRIPTOR_CC_H_

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/result_cc.h"

typedef mip_cc_handle* mip_cc_template_descriptor;

/**
 * @brief Gets template ID
 * 
 * @param protectionDescriptor Descriptor associated with protected content
 * @param templateId [Output] Template ID associated with protection
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_TemplateDescriptor_GetId(
    const mip_cc_template_descriptor protectionDescriptor,
    mip_cc_guid* templateId,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of buffer required to store name
 * 
 * @param templateDescriptor Descriptor associated with template
 * @param nameSize [Output] Size of buffer to hold name (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_TemplateDescriptor_GetNameSize(
    const mip_cc_template_descriptor templateDescriptor,
    int64_t* nameSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets Template name
 * 
 * @param templateDescriptor Descriptor associated with template
 * @param nameBuffer [Output] Buffer the name will be copied into.
 * @param nameBufferSize Size (in number of chars) of the nameBuffer.
 * @param actualNameSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If NameBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualNameSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_TemplateDescriptor_GetName(
    const mip_cc_template_descriptor templateDescriptor,
    char* nameBuffer,
    const int64_t nameBufferSize,
    int64_t* actualNameSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of buffer required to store description
 * 
 * @param templateDescriptor Descriptor associated with template
 * @param descriptionSize [Output] Size of buffer to hold description (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_TemplateDescriptor_GetDescriptionSize(
    const mip_cc_template_descriptor templateDescriptor,
    int64_t* descriptionSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets Template description
 * 
 * @param templateDescriptor Descriptor associated with template
 * @param descriptionBuffer [Output] Buffer the description will be copied into.
 * @param descriptionBufferSize Size (in number of chars) of the descriptionBuffer.
 * @param actualNameSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If descriptionBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualDescriptionSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_TemplateDescriptor_GetDescription(
    const mip_cc_template_descriptor templateDescriptor,
    char* nameBuffer,
    const int64_t nameBufferSize,
    int64_t* actualNameSize,
    mip_cc_error* errorInfo);

/**
 * @brief Release resources associated with a template descriptor
 * 
 * @param templateDescriptor Template descriptor to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseTemplateDescriptor(mip_cc_template_descriptor templateDescriptor);

#endif // API_MIP_PROTECTION_TEMPLATE_DESCRIPTOR_CC_H_