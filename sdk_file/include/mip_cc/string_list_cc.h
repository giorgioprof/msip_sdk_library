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
* @brief Contains C API definitions for common string list.
* 
* @file string_list_cc.h
*/

#ifndef API_MIP_STRING_LIST_CC_H
#define API_MIP_STRING_LIST_CC_H

#include <stdint.h>

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/result_cc.h"

typedef mip_cc_handle* mip_cc_string_list;

/**
 * @brief Create a string list
 * 
 * @param strings Array of strings
 * @param count Number of strings
 * @param stringList [Output] Newly-created string list
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note A mip_cc_string_list must be freed by calling MIP_CC_ReleaseStringList
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateStringList(
    const char** strings,
    const int64_t count,
    mip_cc_string_list* stringList,
    mip_cc_error* errorInfo);

/**
 * @brief Get strings that compose a string list
 * 
 * @param stringList Source string list
 * @param strings [Output] Array of strings, memory owned by mip_cc_string_list object
 * @param count [Output] Number of strings
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The memory for 'strings' is owned by the mip_cc_string_list object, so it should not be freed independently
 */
MIP_CC_API(mip_cc_result) MIP_CC_StringList_GetStrings(
    const mip_cc_string_list stringList,
    const char*** strings,
    int64_t* count,
    mip_cc_error* errorInfo);

/**
 * @brief Release resources associated with a string list
 * 
 * @param stringList String list to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseStringList(mip_cc_string_list stringList);

#endif  // API_MIP_STRING_LIST_CC_H
