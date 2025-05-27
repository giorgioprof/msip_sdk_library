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
* @brief Contains C API definitions for policy handler
* 
* @file policy_handler_cc.h
*/

#ifndef API_MIP_UPE_POLICY_HANDLER_CC_H_
#define API_MIP_UPE_POLICY_HANDLER_CC_H_

#include <stdint.h>

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/result_cc.h"
#include "mip_cc/upe/action_result_cc.h"
#include "mip_cc/upe/application_action_state_cc.h"
#include "mip_cc/upe/content_label_cc.h"
#include "mip_cc/upe/document_state_cc.h"

typedef mip_cc_handle* mip_cc_policy_handler;

/**
 * @brief Release resources associated with a policy handler
 * 
 * @param handler Policy handler to release
 */
MIP_CC_API(void) MIP_CC_ReleasePolicyHandler(mip_cc_policy_handler handler);

/**
 * @brief Gets a document's current label
 * 
 * @param handler Policy handler
 * @param documentState Document state
 * @param context Application context opaquely forwarded to any callbacks
 * @param contentLabel Label currently applied to a document
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyHandler_GetSensitivityLabel(
    const mip_cc_policy_handler handler,
    const mip_cc_document_state* documentState,
    const void* context,
    mip_cc_content_label* contentLabel,
    mip_cc_error* errorInfo);

/**
 * @brief Executes policy rules based on the provided state and determines corresponding actions
 * 
 * @param handler Policy handler
 * @param documentState Document state
 * @param applicationState Application action state
 * @param context Application context opaquely forwarded to any callbacks
 * @param actionResult [Output] Actions that should be taken by application, memory owned by caller
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The 'actionResult' variable must be released by the caller by calling MIP_CC_ReleaseActionResult
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyHandler_ComputeActions(
    const mip_cc_policy_handler handler,
    const mip_cc_document_state* documentState,
    const mip_cc_application_action_state* applicationState,
    const void* context,
    mip_cc_action_result* actionResult,
    mip_cc_error* errorInfo);

/**
 * @brief Called by application after computed actions have been applied and data committed to disk
 * 
 * @param handler Policy handler
 * @param documentState Document state
 * @param applicationState Application action state
 * @param context Application context opaquely forwarded to any callbacks
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note A call to this function is necessary to transmit complete label audit data.
 */
MIP_CC_API(mip_cc_result) MIP_CC_PolicyHandler_NotifyCommittedActions(
    const mip_cc_policy_handler handler,
    const mip_cc_document_state* documentState,
    const mip_cc_application_action_state* applicationState,
    const void* context,
    mip_cc_error* errorInfo);

#endif // API_MIP_UPE_POLICY_HANDLER_CC_H_