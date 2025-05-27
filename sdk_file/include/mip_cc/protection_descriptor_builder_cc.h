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
 * @brief Defines C-Style ProtectionDescriptorBuilder interface
 * 
 * @file protection_descriptor_builder_cc.h
 */

#ifndef API_MIP_PROTECTION_DESCRIPTOR_BUILDER_CC_H_
#define API_MIP_PROTECTION_DESCRIPTOR_BUILDER_CC_H_

#include "mip_cc/protection_descriptor_cc.h"

typedef mip_cc_handle* mip_cc_protection_descriptor_builder;

typedef enum {
  MIP_PROTECTION_LICENSE_TYPE_DO_NOT_FORWARD = 0, /**< Defines a Do Not Forward License */
  MIP_PROTECTION_LICENSE_TYPE_ENCRYPT_ONLY = 1,   /**< Defines an Encrypt Only License */
} mip_cc_protection_license_type;

/**
 * @brief Creates a protection descriptor whose access permissions are defined by users and rights
 *
 * @param userRights Groups of users and their rights
 * @param userRightsCount Number of user/rights groups
 * @param protectionDescriptorBuilder [Output] Newly-created protection descriptor builder instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 *
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateProtectionDescriptorBuilderFromUserRights(
    const mip_cc_user_rights* userRights,
    const int64_t userRightsCount,
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    mip_cc_error* errorInfo);

/**
 * @brief Creates a protection descriptor builder whose access permissions are defined by users and roles
 *
 * @param userRoles Groups of users and their roles
 * @param userRolesCount Number of user/roles groups
 * @param protectionDescriptorBuilder [Output] Newly-created protection descriptor builder instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 *
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateProtectionDescriptorBuilderFromUserRoles(
    const mip_cc_user_roles* userRoles,
    const int64_t userRolesCount,
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    mip_cc_error* errorInfo);

/**
 * @brief Creates a protection descriptor builder whose access permissions are defined by an RMS template
 *
 * @param templateId Template ID in GUID format
 * @param protectionDescriptorBuilder [Output] Newly-created protection descriptor builder instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 *
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateProtectionDescriptorBuilderFromTemplate(
    const char* templateId,
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    mip_cc_error* errorInfo);

/**
 * @brief Creates a protection descriptor builder based on an existing protection descriptor
 *
 * @param protectionDescriptor The protection descriptor to base this builder on
 * @param protectionDescriptorBuilder [Output] Newly-created protection descriptor builder instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 *
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateProtectionDescriptorBuilderFromProtectionDescriptor(
    mip_cc_protection_descriptor* protectionDescriptor,
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    mip_cc_error* errorInfo);

/**
 * @brief Creates a ProtectionDescriptorBuilder for a license type with the specified users
 * 
 * @param licenseType The license type to create this Protection Descriptor Builder for
 * @param usersWithDefaultRights Collection of users to add to the license with default rights for the specific type
 * @param usersWithDefaultRightsCount Number of users in the usersWithDefaultRights array
 * @param additionalUsersAndRights Optional additional collection of users-to-rights mappings
 * @param additionalUsersAndRightsCount Number of users in the additionalUsersAndRights array
 * @param protectionDescriptorBuilder [Output] Newly-created protection descriptor builder instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateProtectionDescriptorBuilderFromLicenseType(
    mip_cc_protection_license_type licenseType,
    const char** usersWithDefaultRights,
    const int64_t usersWithDefaultRightsCount,
    const mip_cc_user_rights* additionalUsersAndRights,
    const int64_t additionalUsersAndRightsCount,
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    mip_cc_error* errorInfo);

/**
 * @brief Creates a Protection Descriptor Builder whose access permissions are defined by the protection template
 * 
 * @param serializedTemplate Protection template
 * @param serializedTemplateSize Size of the serialized template
 * @param protectionDescriptorBuilder [Output] Newly-created protection descriptor builder instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateProtectionDescriptorBuilderFromSerializedTemplate(
    const uint8_t* serializedTemplate,
    const int64_t serializedTemplateSize,
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    mip_cc_error* errorInfo);

/**
 * @brief Build a protection descriptor from the protection descriptor builder
 * 
 * @param protectionDescriptorBuilder Protection descriptor builder
 * @param protectionDescriptor [Output] Newly-created protection descriptor instance
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_BuildProtectionDescriptor(
    mip_cc_protection_descriptor_builder protectionDescriptorBuilder,
    mip_cc_protection_descriptor* protectionDescriptor,
    mip_cc_error* errorInfo);

/**
 * @brief Set the name of the protection
 * 
 * @param protectionDescriptorBuilder Protection descriptor builder
 * @param name Protection name
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionDescriptorBuilder_SetName(
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    const char* name,
    mip_cc_error* errorInfo);

/**
 * @brief Set the description of the protection
 * 
 * @param protectionDescriptorBuilder Protection descriptor builder
 * @param description Protection description
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionDescriptorBuilder_SetDescription(
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    const char* description,
    mip_cc_error* errorInfo);

/**
 * @brief Sets protection expiration time (in seconds since epoch)
 * 
 * @param protectionDescriptor Descriptor associated with protected content
 * @param contentValidUntil Content expiration time (in seconds since epoch)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionDescriptorBuilder_SetContentValidUntil(
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    int64_t contentValidUntil,
    mip_cc_error* errorInfo);


/**
 * @brief Sets if protection policy allows offline content access or not
 * 
 * @param protectionDescriptorBuilder Protection descriptor builder
 * @param allowOfflineAccess If policy allows offline content access or not
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionDescriptorBuilder_SetAllowOfflineAccess(
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    bool allowOfflineAccess,
    mip_cc_error* errorInfo);

/**
 * @brief Sets the protection referrer email address
 * 
 * @param protectionDescriptorBuilder Protection descriptor builder
 * @param uri Address to referrer unauthenticated users to
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionDescriptorBuilder_SetReferrer(
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    const char* uri,
    mip_cc_error* errorInfo);

/**
 * @brief Sets the encrypted application data for the protection
 * 
 * @param protectionDescriptorBuilder Protection descriptor builder
 * @param appData Application-specific data to encrypt
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionDescriptorBuilder_SetEncryptedAppData(
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    const mip_cc_dictionary appData,
    mip_cc_error* errorInfo);

/**
 * @brief Sets the signed application data for the protection
 * 
 * @param protectionDescriptorBuilder Protection descriptor builder
 * @param appData Application-specific data for unencrypted section
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionDescriptorBuilder_SetSignedAppData(
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    const mip_cc_dictionary appData,
    mip_cc_error* errorInfo);

/**
 * @brief Sets a double key url for custom protection
 * 
 * @param protectionDescriptorBuilder Protection descriptor builder
 * @param doubleKeyUrl Double key url
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionDescriptorBuilder_SetDoubleKeyUrl(
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    const char* doubleKeyUrl,
    mip_cc_error* errorInfo);

/**
 * @brief Sets the label information for UDP protection
 * 
 * @param protectionDescriptorBuilder Protection descriptor builder
 * @param labelId Label ID
 * @param tenantId Tenant ID
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_ProtectionDescriptorBuilder_SetLabelInfo(
    mip_cc_protection_descriptor_builder* protectionDescriptorBuilder,
    const char* labelId,
    const char* tenantId,
    mip_cc_error* errorInfo);

/**
 * @brief Release resources associated with a protection descriptor builder
 * 
 * @param protectionDescriptorBuilder Protection descriptor builder to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseProtectionDescriptorBuilder(mip_cc_protection_descriptor_builder protectionDescriptorBuilder);

#endif // API_MIP_PROTECTION_DESCRIPTOR_BUILDER_CC_H_
