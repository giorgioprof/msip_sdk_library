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
* @brief Contains C API definitions for label policy actions
* 
* @file action_cc.h
*/

#ifndef API_MIP_UPE_ACTION_CC_H_
#define API_MIP_UPE_ACTION_CC_H_

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/result_cc.h"

typedef mip_cc_handle* mip_cc_action;

/**
 * @brief Action type bit mask
 */
typedef enum {
  MIP_ACTION_TYPE_ADD_CONTENT_FOOTER = 1 << 0,        /**< Add a content footer to the document action type.*/
  MIP_ACTION_TYPE_ADD_CONTENT_HEADER = 1 << 1,        /**< Add a content header to the document action type.*/
  MIP_ACTION_TYPE_ADD_WATERMARK = 1 << 2,             /**< Add a water mark to the entire document action type.*/
  MIP_ACTION_TYPE_CUSTOM = 1 << 3,                    /**< A custom defined action type.*/
  MIP_ACTION_TYPE_JUSTIFY = 1 << 4,                   /**< A justify action type.*/
  MIP_ACTION_TYPE_METADATA = 1 << 5,                  /**< A Meta data change action type.*/
  MIP_ACTION_TYPE_PROTECT_ADHOC = 1 << 6,             /**< A protect by adhoc policy action type.*/
  MIP_ACTION_TYPE_PROTECT_BY_TEMPLATE = 1 << 7,       /**< A protect by template action type.*/
  MIP_ACTION_TYPE_PROTECT_DO_NOT_FORWARD = 1 << 8,    /**< A protect by do not forward action type.*/
  MIP_ACTION_TYPE_REMOVE_CONTENT_FOOTER = 1 << 9,     /**< Remove content footer action type.*/
  MIP_ACTION_TYPE_REMOVE_CONTENT_HEADER = 1 << 10,    /**< Remove content header action type.*/
  MIP_ACTION_TYPE_REMOVE_PROTECTION = 1 << 11,        /**< Remove protection action type.*/
  MIP_ACTION_TYPE_REMOVE_WATERMARK = 1 << 12,         /**< Remove watermarking action type.*/
  MIP_ACTION_TYPE_APPLY_LABEL = 1 << 13,              /**< Apply label action type.*/
  MIP_ACTION_TYPE_RECOMMEND_LABEL = 1 << 14,          /**< Recommend label action type.*/
  MIP_ACTION_TYPE_PROTECT_ADHOC_DK = 1 << 15,         /**< A protect by adhoc policy action type.*/
  // Reserved
  MIP_ACTION_TYPE_PROTECT_DO_NOT_FORWARD_DK = 1 << 17,/**< A protect by do not forward action type.*/
  MIP_ACTION_TYPE_PROTECT_BY_ENCRYPT_ONLY = 1 << 18,  /**< A protect by encryption action type.*/
} mip_cc_action_type;

/**
 * @brief Gets an action's type
 * 
 * @param action Action
 * @param actionType [Output] Type of action
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_Action_GetType(
    const mip_cc_action action,
    mip_cc_action_type* actionType,
    mip_cc_error* errorInfo);

/**
 * @brief Gets an action's ID
 * 
 * @param action Action
 * @param id [Output] Unique action ID
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_Action_GetId(
    const mip_cc_action action,
    mip_cc_guid* id,
    mip_cc_error* errorInfo);

#endif // API_MIP_UPE_ACTION_CC_H_