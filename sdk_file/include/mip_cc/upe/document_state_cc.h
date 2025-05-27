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
* @brief Describes document state when performing a label-related operation
* 
* @file document_state_cc.h
*/

#ifndef API_MIP_UPE_DOCUMENT_STATE_CC_H_
#define API_MIP_UPE_DOCUMENT_STATE_CC_H_

#include "mip_cc/common_types_cc.h"
#include "mip_cc/dictionary_cc.h"
#include "mip_cc/protection_descriptor_cc.h"
#include "mip_cc/upe/metadata_dictionary_cc.h"

/**
 * @brief Defines the state of the data as an application is acting upon it
 */
typedef enum {
  MIP_DATA_STATE_REST = 0,   /**< Inactive data stored physically in databases/file/warehouses */
  MIP_DATA_STATE_MOTION = 1, /**< Data traversing a network or temporarily residing in computer memory to be read or updated */
  MIP_DATA_STATE_USE = 2,    /**< Active data under constant change stored physically in databases/file/warehouses etc */
} mip_cc_data_state;

/**
 * @brief Callback function definition for retrieving document metatdata, filtered by name/prefix
 * 
 * @param names Array of metadata key names to include in result
 * @param namesSize Number of values in 'names' array
 * @param namePrefixes Array of metadata key name prefixes to include in result
 * @param namePrefixesSize Number of values in 'namesPrefixes' array
 * @param context Application context opaquely passed from API call to callback
 * @param metadata [Output] Dictionary of metadata key/values, created by client application. This dictionary will be released by MIP.
 */
MIP_CC_CALLBACK(mip_cc_metadata_callback /*typename*/,
    void /*return*/,
    const char** /*names*/,
    const int64_t /*namesSize*/,
    const char** /*namePrefixes*/,
    const int64_t /*namePrefixesSize*/,
    const void* /*context*/,
    mip_cc_metadata_dictionary* /*metadata*/);

/**
 * @brief Represents the current state of a label-aware document
 */
typedef struct {
  /**
   * Human-readable document description visible in tenant audit portal
   *     Example for a file: [path\filename]
   *     Example for an email: [Subject:Sender]
   */
  const char* contentId;

  /**
   * State of document data as application interacts with it
   */
  mip_cc_data_state dataState;

  /**
   * Document metadata callback
   */
  mip_cc_metadata_callback contentMetadataCallback;

  /**
   * Protection descriptor if document is currently protected, else null
   */
  mip_cc_protection_descriptor protectionDescriptor;

  /**
   * Format of document (file vs. email)
   */
  mip_cc_content_format contentFormat;

  /**
   * Optional application-specific metadata that is used when sending audit reports
   *     Recognized values:
   *       'Sender': Sender email address
   *       'Recipients': JSON array of email recipients
   *       'LastModifiedBy': Email address of the user who last modified a document
   *       'LastModifiedDate': Date a document was last modified
   */
  mip_cc_dictionary auditMetadata;
  
  /**
   * Document metadata version, default should be 0.
   */
  uint32_t contentMetadataVersion;

  /**
   * Describes how metadata versioning is processed.
   */
  mip_cc_metadata_version_format contentMetadataVersionFormat;

} mip_cc_document_state;

#endif // API_MIP_UPE_DOCUMENT_STATE_CC_H_