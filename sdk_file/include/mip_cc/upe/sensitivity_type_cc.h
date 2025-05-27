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
* @brief Contains C API definitions for sensitivity type
* 
* @file sensitivity_type_cc.h
*/

#ifndef API_MIP_UPE_SENSITIVITY_TYPE_CC_H_
#define API_MIP_UPE_SENSITIVITY_TYPE_CC_H_

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/result_cc.h"

typedef mip_cc_handle* mip_cc_sensitivity_type;

/**
 * @brief Release resources associated with a sensitivity type
 * 
 * @param sensitivityType Sensitivity type to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseSensitivityType(mip_cc_sensitivity_type sensitivityType);

/**
 * @brief Gets size of buffer required to store a sensitivity type's rule package ID
 * 
 * @param sensitivityType Sensitivity type
 * @param idSize [Output] Size of buffer to hold rule package ID (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_SensitivityType_GetRulePackageIdSize(
    const mip_cc_sensitivity_type sensitivityType,
    int64_t* idSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets a sensitivity type's rule package ID
 * 
 * @param sensitivityType Sensitivity type
 * @param idBuffer [Output] Buffer the ID will be copied into.
 * @param idBufferSize Size (in number of chars) of the idBuffer.
 * @param actualIdSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If idBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualIdSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_SensitivityType_GetRulePackageId(
    const mip_cc_sensitivity_type sensitivityType,
    char* idBuffer,
    const int64_t idBufferSize,
    int64_t* actualIdSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of buffer required to store a sensitivity type's rule package
 * 
 * @param sensitivityType Sensitivity type
 * @param rulePackageSize [Output] Size of buffer to hold rule package (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_SensitivityType_GetRulePackageSize(
    const mip_cc_sensitivity_type sensitivityType,
    int64_t* rulePackageSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets a sensitivity type's rule package
 * 
 * @param sensitivityType Sensitivity type
 * @param rulePackageBuffer [Output] Buffer the rule package will be copied into.
 * @param rulePackageBufferSize Size (in number of chars) of the rulePackageBuffer.
 * @param actualRulePackageSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note If rulePackageBuffer is null or insufficient, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER will be returned and
 *       actualRulePackageSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_SensitivityType_GetRulePackage(
    const mip_cc_sensitivity_type sensitivityType,
    char* rulePackageBuffer,
    const int64_t rulePackageBufferSize,
    int64_t* actualRulePackageSize,
    mip_cc_error* errorInfo);

#endif // API_MIP_UPE_SENSITIVITY_TYPE_CC_H_