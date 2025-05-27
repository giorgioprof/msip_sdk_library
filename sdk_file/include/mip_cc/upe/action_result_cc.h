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
* @brief A file containing definition of action result type
* 
* @file action_result_cc.h
*/

#ifndef API_MIP_UPE_ACTION_RESULT_CC_H
#define API_MIP_UPE_ACTION_RESULT_CC_H

#include <stdint.h>

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/result_cc.h"
#include "mip_cc/upe/action_cc.h"

typedef mip_cc_handle* mip_cc_action_result;

/**
 * @brief Get actions that compose an action result
 * 
 * @param actionResult Source action result
 * @param actions [Output] Array of actions, memory owned by mip_cc_action_result object
 * @param count [Output] Number of key/value pairs
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The memory for 'actions' is owned by the mip_cc_action_result object, so it should not be freed independently
 */
MIP_CC_API(mip_cc_result) MIP_CC_ActionResult_GetActions(
    const mip_cc_action_result actionResult,
    mip_cc_action** actions,
    int64_t* count,
    mip_cc_error* errorInfo);

/**
 * @brief Release resources associated with an action result
 * 
 * @param actionResult Action result to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseActionResult(mip_cc_action_result actionResult);

#endif  // API_MIP_UPE_ACTION_RESULT_CC_H
