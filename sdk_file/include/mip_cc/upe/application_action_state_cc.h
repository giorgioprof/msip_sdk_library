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
* @brief Describes application state when performing a label-related operation
* 
* @file application_action_state_cc.h
*/

#ifndef API_MIP_UPE_APPLICATION_ACTION_STATE_CC_H_
#define API_MIP_UPE_APPLICATION_ACTION_STATE_CC_H_

#include "mip_cc/common_types_cc.h"
#include "mip_cc/dictionary_cc.h"
#include "mip_cc/upe/label_cc.h"

/**
 * @brief Describes what the application is trying to do regarding the current label
 */
typedef enum {
  MIP_LABEL_ACTION_STATE_NO_CHANGE = 0, /**< The current label should not change. */
  MIP_LABEL_ACTION_STATE_REMOVE = 1,    /**< The current label should be removed. */
  MIP_LABEL_ACTION_STATE_UPDATE = 2,    /**< The current label should be changed. */
} mip_cc_label_action_state;

/**
 * @brief Label-related actions an application understands and supports
 */
typedef enum {
  MIP_LABEL_ACTION_TYPE_ADD_CONTENT_FOOTER = 1 << 0,     /**< Add a content footer to the document action type. */
  MIP_LABEL_ACTION_TYPE_ADD_CONTENT_HEADER = 1 << 1,     /**< Add a content header to the document action type. */
  MIP_LABEL_ACTION_TYPE_ADD_WATERMARK = 1 << 2,          /**< Add a water mark to the entire document action type. */
  MIP_LABEL_ACTION_TYPE_CUSTOM = 1 << 3,                 /**< A custom defined action type. */
  MIP_LABEL_ACTION_TYPE_JUSTIFY = 1 << 4,                /**< A justify action type. */
  MIP_LABEL_ACTION_TYPE_METADATA = 1 << 5,               /**< A Meta data change action type. */
  MIP_LABEL_ACTION_TYPE_PROTECT_ADHOC = 1 << 6,          /**< A protect by adhoc policy action type. */
  MIP_LABEL_ACTION_TYPE_PROTECT_BY_TEMPLATE = 1 << 7,    /**< A protect by template action type. */
  MIP_LABEL_ACTION_TYPE_PROTECT_DO_NOT_FORWARD = 1 << 8, /**< A protect by do not forward action type. */
  MIP_LABEL_ACTION_TYPE_REMOVE_CONTENT_FOOTER = 1 << 9,  /**< Remove content footer action type. */
  MIP_LABEL_ACTION_TYPE_REMOVE_CONTENT_HEADER = 1 << 10, /**< Remove content header action type. */
  MIP_LABEL_ACTION_TYPE_REMOVE_PROTECTION = 1 << 11,     /**< Remove protection action type. */
  MIP_LABEL_ACTION_TYPE_REMOVE_WATERMARK = 1 << 12,      /**< Remove watermarking action type. */
  MIP_LABEL_ACTION_TYPE_APPLY_LABEL = 1 << 13,           /**< Apply label action type. */
  MIP_LABEL_ACTION_TYPE_RECOMMEND_LABEL = 1 << 14,       /**< Recommend label action type. */
  MIP_LABEL_ACTION_TYPE_PROTECT_ADHOC_DK = 1 << 15,      /**< A protect by adhoc policy action type.*/
  // Reserved
  MIP_LABEL_ACTION_TYPE_PROTECT_DO_NOT_FORWARD_DK = 1 << 17, /**< A protect by do not forward action type.*/
  MIP_LABEL_ACTION_TYPE_PROTECT_BY_ENCRYPT_ONLY = 1 << 18,   /**< A protect by encryption action type.*/
} mip_cc_label_action_type;

/**
 * @brief Represents the current state of the application as it performs a label-related operation
 */
typedef struct {
  mip_cc_label_action_state actionState;                    /**< Describes if/how an application is attempting to change label state. */
  mip_cc_label newLabel;                                    /**< If 'actionType' is 'UPDATE': New label. */
  mip_cc_dictionary newLabelExtendedProperties;             /**< If 'actionType' is 'UPDATE': Additional properties to be written to metadata. */
  mip_cc_label_assignment_method newLabelAssignmentMethod;  /**< If 'actionType' is 'UPDATE': The method of assignment of the new label. */
  bool isDowngradeJustified;                                /**< If 'actionType' is 'UPDATE': Whether or not a label downgrade has been justified by user. */
  const char* downgradeJustification;                       /**< If 'actionType' is 'UPDATE': Label downgrade justification text provided by user. */
  mip_cc_label_action_type supportedActions;                /**< Enum mask describing the label-related actions an application is able to perform. */
} mip_cc_application_action_state;

#endif // API_MIP_UPE_APPLICATION_ACTION_STATE_CC_H_