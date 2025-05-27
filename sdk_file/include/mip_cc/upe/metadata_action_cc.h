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
* @brief Contains C API definitions for "metadata" label policy action
* 
* @file metadata_action_cc.h
*/

#ifndef API_MIP_UPE_METADATA_ACTION_CC_H_
#define API_MIP_UPE_METADATA_ACTION_CC_H_

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/string_list_cc.h"
#include "mip_cc/upe/action_cc.h"
#include "mip_cc/upe/metadata_dictionary_cc.h"

/**
 * @brief Gets a "metadata" action's metadata to remove
 * 
 * @param action "metadata" action
 * @param metadataNames [Output] Key names of metadata to remove, memory owned by caller
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The 'metadataNames' variable must be released by the caller by calling MIP_CC_ReleaseStringList
 * @note Removing metadata should be done before adding metadata
 */
MIP_CC_API(mip_cc_result) MIP_CC_MetadataAction_GetMetadataToRemove(
    const mip_cc_action action,
    mip_cc_string_list* metadataNames,
    mip_cc_error* errorInfo);

/**
 * @brief Gets a "metadata" action's metadata to add
 * 
 * @param action "metadata" action
 * @param metadata [Output] list of metadata entries to add, memory owned by caller
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The 'metadata' variable must be released by the caller by calling MIP_CC_ReleaseDictionary
 * @note Removing metadata should be done before adding metadata
 */
MIP_CC_API(mip_cc_result) MIP_CC_MetadataAction_GetMetadataToAdd(
    const mip_cc_action action,
    mip_cc_metadata_dictionary* metadata,
    mip_cc_error* errorInfo);

#endif // API_MIP_UPE_METADATA_ACTION_CC_H_