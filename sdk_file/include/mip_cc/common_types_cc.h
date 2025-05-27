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
* @brief A file Containing the common types used by the upe, file and protection modules.
* 
* @file common_types_cc.h
*/

#ifndef API_MIP_COMMON_TYPES_CC_H
#define API_MIP_COMMON_TYPES_CC_H

#include <stdint.h>

#include "mip_cc/mip_macros_cc.h"

/**
 * @brief Opaque handle to MIP object
 */
typedef struct {
  uint32_t typeId; /**< Magic number uniquely identifying s specific handle type */
  void* data;      /**< Raw handle data */
} mip_cc_handle;

/**
 * @brief GUID
 */
typedef struct {
  char guid[37];
} mip_cc_guid;

/**
 * @brief Storage type for caches
 */
typedef enum {
  MIP_CACHE_STORAGE_TYPE_IN_MEMORY = 0,        /**< In-memory storage */
  MIP_CACHE_STORAGE_TYPE_ON_DISK = 1,          /**< On-disk storage */
  MIP_CACHE_STORAGE_TYPE_ON_DISK_ENCRYPTED = 2 /**< On-disk storage with encryption (if supported by the platform) */
} mip_cc_cache_storage_type;

/**
 * @brief Content format
 */
typedef enum {
  MIP_CONTENT_FORMAT_DEFAULT = 0, /**< Standard file format */
  MIP_CONTENT_FORMAT_EMAIL = 1,   /**< Email */
} mip_cc_content_format;

/**
 * @brief Describes how a new label is being applied
 */
typedef enum {
  MIP_LABEL_ASSIGNMENT_METHOD_STANDARD = 0,   /**< Standard label assignments will not override a previous privileged assignment. */
  MIP_LABEL_ASSIGNMENT_METHOD_PRIVILEGED = 1, /**< A privileged label assignment will not be overridden by future standard assignments. */
  MIP_LABEL_ASSIGNMENT_METHOD_AUTO = 2,       /**< Reserved. Do not use. */
} mip_cc_label_assignment_method;

/**
 * @brief Alignment for content marks (content header or content footer)
 */
typedef enum {
  MIP_CONTENT_MARK_ALIGNMENT_LEFT = 0,   /**< Content marking is aligned to the left */
  MIP_CONTENT_MARK_ALIGNMENT_RIGHT = 1,  /**< Content marking is aligned to the right */
  MIP_CONTENT_MARK_ALIGNMENT_CENTER = 2, /**< Content marking is centered */
} mip_cc_content_mark_alignment;

/**
 * @brief Layout for watermarks
 */
typedef enum {
  MIP_WATERMARK_LAYOUT_HORIZONTAL = 0, /**< Watermark layout is horizontal */
  MIP_WATERMARK_LAYOUT_DIAGONAL = 1,   /**< Watermark layout is diagonal */
} mip_cc_watermark_layout;

typedef enum {
  MIP_LABEL_FILTER_NONE                           = 0,      /**< Disable default labeling filtration */
  MIP_LABEL_FILTER_CUSTOM_PROTECTION              = 1 << 0, /**< Filter labels that may result in custom protection*/
  MIP_LABEL_FILTER_TEMPLATE_PROTECTION            = 1 << 1, /**< Filter labels that may result in template protection */
  MIP_LABEL_FILTER_DO_NOT_FORWARD_PROTECTION      = 1 << 2, /**< Filter labels that may result in do not forward*/
  MIP_LABEL_FILTER_ADHOC_PROTECTION               = 1 << 3, /**< Filter labels that may result in adhoc protection */
  MIP_LABEL_FILTER_HYOK_PROTECTION                = 1 << 4, /**< Filter labels that may result in hyok protection */
  MIP_LABEL_FILTER_PREDEFINED_TEMPLATE_PROTECTION = 1 << 5, /**< Filter labels that may result in predefined template protection */
  MIP_LABEL_FILTER_DOUBLE_KEY_PROTECTION          = 1 << 6, /**< Filter labels that may result in protection that requires double key template*/
  MIP_LABEL_FILTER_DOUBLE_KEY_USER_DEFINED_PROTECTION       = 1 << 7, /**< Filter labels that may result in protection that requires double key adhoc and dnf*/
  MIP_LABEL_FILTER_ENCRYPT_ONLY_PROTECTION        = 1 << 8, /**< Filter labels that may result in encrypt only protection */
} mip_cc_label_filter;

typedef enum {
  MIP_CLOUD_UNKNOWN     = 0, /**< Cloud not specified or URL not recognized as an Azure cloud */
  MIP_CLOUD_CUSTOM      = 1, /**< Custom cloud: endpoint base URL manually specified by application */
  MIP_CLOUD_TEST        = 2, /**< Test cloud */
  MIP_CLOUD_COMMERCIAL  = 3, /**< Global commercial cloud */
  MIP_CLOUD_GERMANY     = 4, /**< Germany sovereign cloud */
  MIP_CLOUD_US_DOD      = 5, /**< US sovereign cloud (DoD) */
  MIP_CLOUD_US_GCC      = 6, /**< US sovereign cloud (GCC) */
  MIP_CLOUD_US_GCC_HIGH = 7, /**< US sovereign cloud (GCC-High) */
  MIP_CLOUD_US_SEC      = 8, /**< US sovereign cloud (Sec) */
  MIP_CLOUD_US_NAT      = 9, /**< US sovereign cloud (Nat) */
  MIP_CLOUD_CHINA_01    = 10, /**< China sovereign cloud */
} mip_cc_cloud;

typedef enum {
  MIP_METADATA_VERSION_FORMAT_DEFAULT = 0,                /**< Metadata is ordered based upon label id */
  MIP_METADATA_VERSION_FORMAT_ONE_LABEL_PER_TENANT = 1,   /**< Metadata is ordered based upon site-id */
} mip_cc_metadata_version_format;

typedef enum {
  MIP_WORKLOAD_OFFICE  = 0, /**< Disable default labeling filtration */
  MIP_WORKLOAD_PURVIEW = 1, /**< Filter labels that may result in custom protection*/
} mip_cc_workload;

#endif  // API_MIP_COMMON_TYPES_CC_H
