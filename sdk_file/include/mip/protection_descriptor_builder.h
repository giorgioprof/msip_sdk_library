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
 * @brief Defines ProtectionDescriptor interface
 * 
 * @file protection_descriptor_builder.h
 */

#ifndef API_MIP_PROTECTION_DESCRIPTOR_BUILDER_H_
#define API_MIP_PROTECTION_DESCRIPTOR_BUILDER_H_

#include <map>

#include "mip/mip_export.h"
#include "mip/mip_namespace.h"
#include "mip/protection_descriptor.h"
#include "mip/user_rights.h"
#include "mip/user_roles.h"

MIP_NAMESPACE_BEGIN

/**
* @brief Constructs a ProtectionDescriptor that describes protection associated with a piece of content
*/
class ProtectionDescriptorBuilder {
public:
    /**
    * @brief License type to create a specialized license
    */
    enum class LicenseType {
      /**
      * @brief Defines a do not forward license type
      */
      DoNotForward = 0,
      /**
      * @brief Defines an encrypt only license type
      */
      EncryptOnly
    };
    
  /**
   * @brief Creates a ProtectionDescriptorBuilder whose access permissions are defined by users and rights
   * 
   * @param usersAndRights Collection of users-to-rights mappings
   * 
   * @return New ProtectionDescriptorBuilder instance
   */
  MIP_API static std::shared_ptr<ProtectionDescriptorBuilder> CreateFromUserRights(
      const std::vector<UserRights>& usersAndRights);

  /**
   * @brief Creates a ProtectionDescriptorBuilder whose access permissions are defined by users and roles
   * 
   * @param usersAndRoles Collection of users-to-roles mappings
   * 
   * @return New ProtectionDescriptorBuilder instance
   */
  MIP_API static std::shared_ptr<ProtectionDescriptorBuilder> CreateFromUserRoles(
      const std::vector<UserRoles>& usersAndRoles);

  /**
   * @brief Creates a ProtectionDescriptorBuilder whose access permissions are defined by the protection template
   * 
   * @param templateId protection template ID
   * 
   * @return New ProtectionDescriptorBuilder instance
   */
  MIP_API static std::shared_ptr<ProtectionDescriptorBuilder> CreateFromTemplate(const std::string& templateId);

  /**
   * @brief Creates a ProtectionDescriptorBuilder whose access permissions are defined by the protection template
   * 
   * @param serializedTemplate protection template
   * 
   * @return New ProtectionDescriptorBuilder instance
   */
  MIP_API static std::shared_ptr<ProtectionDescriptorBuilder> CreateFromSerializedTemplate(
    const std::vector<uint8_t>& serializedTemplate);

  /**
   * @brief Creates a ProtectionDescriptorBuilder whose access permissions are defined by a protection descriptor
   * 
   * @param protectionDescriptor protection descriptor
   * 
   * @return New ProtectionDescriptorBuilder instance
   */
  MIP_API static std::shared_ptr<ProtectionDescriptorBuilder> CreateFromProtectionDescriptor(const ProtectionDescriptor& protectionDescriptor);

  /**
   * @brief Creates a ProtectionDescriptorBuilder for a license type with the specified users
   * 
   * @param licenseType The license type to create this ProtectionDescriptorBuilder for
   * @param usersWithDefaultRights Collection of users to add to the license with default rights for the specific type
   * @param additionalUsersAndRights Optional additional collection of users-to-rights mappings
   * 
   * @return New ProtectionDescriptorBuilder instance
   */
  MIP_API static std::shared_ptr<ProtectionDescriptorBuilder> CreateFromLicenseType(
      LicenseType licenseType,
      const std::vector<std::string>& usersWithDefaultRights,
      const std::vector<UserRights>& additionalUsersAndRights);

  /**
   * @brief Creates a ProtectionDescriptor whose access permissions are defined by this ProtectionDescriptorBuilder instance
   * 
   * @return New ProtectionDescriptor instance
   */
  MIP_API virtual std::shared_ptr<ProtectionDescriptor> Build() = 0;

  /**
   * @brief Sets protection policy name
   * 
   * @param value Protection policy name
   */
  virtual void SetName(const std::string& value) = 0;

  /**
   * @brief Sets protection policy description
   * 
   * @param value Policy description
   */
  virtual void SetDescription(const std::string& value) = 0;

  /**
   * @brief Sets protection policy expiration time
   * 
   * @param value Policy expiration time
   */
  virtual void SetContentValidUntil(const std::chrono::time_point<std::chrono::system_clock>& value) = 0;

  /**
   * @brief Sets if protection policy allows offline content access or not
   * 
   * @param value If policy allows offline content access or not
   */
  virtual void SetAllowOfflineAccess(bool value) = 0;

  /**
   * @brief Sets protection policy referrer address
   * 
   * @param uri Policy referrer address
   * 
   * @note The referrer is a URI that can be displayed to the user upon failed protection policy acquisition that contains
   * information on how that user can gain permission to access the content.
   */
  virtual void SetReferrer(const std::string& uri) = 0;

  /**
   * @brief Sets app-specific data that should be encrypted
   * 
   * @param value App-specific data
   * 
   * @note An application can specify a dictionary of app-specific data that will be encrypted by the protection service. This
   * encrypted data is independent of the signed data set by SetSignedAppData.
   */
  virtual void SetEncryptedAppData(const std::map<std::string, std::string>& value) = 0;

  /**
   * @brief Sets app-specific data that should be signed
   * 
   * @param value App-specific data
   * 
   * @note An application can specify a dictionary of app-specific data that will be signed by the protection service. This
   * signed data is independent of the encrypted data set by SetEncryptedAppData.
   */
  virtual void SetSignedAppData(const std::map<std::string, std::string>& value) = 0;

  /**
   * @brief Sets the double key url to be used for custom protection
   * 
   * @param doubleKeyUrl Double key url
   */
  virtual void SetDoubleKeyUrl(const std::string& doubleKeyUrl) = 0;

   /**
   * @brief Sets the label id and tenant id for UDP protection. Only allowed for custom protection type.
   * 
   * @param LabelInfo label info containing labelId and tenantId.
   */
  virtual void SetLabelInfo(const LabelInfo& labelId) = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~ProtectionDescriptorBuilder() {}
protected:
  ProtectionDescriptorBuilder() {}
  /** @endcond */  
};

MIP_NAMESPACE_END

#endif //API_MIP_PROTECTION_DESCRIPTOR_BUILDER_H_