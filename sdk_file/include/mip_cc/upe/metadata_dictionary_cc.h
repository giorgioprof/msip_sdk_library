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
* @brief Contains C API definitions for a metadata dictionary.
* 
* @file metadata_dictionary_cc.h
*/

#ifndef API_MIP_UPE_METADATA_DICTIONARY_CC_H
#define API_MIP_UPE_METADATA_DICTIONARY_CC_H

#include <stdint.h>

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/result_cc.h"

typedef mip_cc_handle* mip_cc_metadata_dictionary;

/**
 * @brief Metadata entry
 */
typedef struct {
  const char* key;        /**< Key entry*/
  const char* value;      /**< Value entry */
  uint32_t version;       /**< Version entry, should be initialized to 0 unless otherwise known*/
} mip_cc_metadata_entry;

/**
 * @brief Create a dictionary of string keys/values
 * 
 * @param entries Array of metadata entries
 * @param count Number of metadata entries
 * @param dictionary [Output] Newly-created dictionary
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note A mip_cc_dictionary must be freed by calling MIP_CC_ReleaseDictionary
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateMetadataDictionary(
    const mip_cc_metadata_entry* entries,
    const int64_t count,
    mip_cc_metadata_dictionary* dictionary,
    mip_cc_error* errorInfo);

/**
 * @brief Get metadata entries that compose a dictionary
 * 
 * @param dictionary Source dictionary
 * @param entries [Output] Array of metadata entries, memory owned by mip_cc_dictionary object
 * @param count [Output] Number of metadata entries
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 * 
 * @note The memory for 'entries' is owned by the mip_cc_dictionary object, so it should not be freed independently
 */
MIP_CC_API(mip_cc_result) MIP_CC_MetadataDictionary_GetEntries(
    const mip_cc_metadata_dictionary dictionary,
    mip_cc_metadata_entry** entries,
    int64_t* count,
    mip_cc_error* errorInfo);

/**
 * @brief Release resources associated with a dictionary
 * 
 * @param dictionary Dictionary to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseMetadataDictionary(mip_cc_metadata_dictionary dictionary);

#endif  // API_MIP_UPE_METADATA_DICTIONARY_CC_H
