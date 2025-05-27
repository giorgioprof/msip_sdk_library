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
 * @brief Defines LicenseRightstData interface
 * 
 * @file license_rights_data.h
 */

#ifndef API_MIP_PROTECTION_LICENSE_RIGHTS_DATA_H_
#define API_MIP_PROTECTION_LICENSE_RIGHTS_DATA_H_

#include <chrono>
#include <string>
#include <vector>

#include "mip/mip_namespace.h"
#include "mip/protection_descriptor.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief The directory object information which includes- type, email, objectId and PUID
 */
class DirectoryObject {
public:
  /**
  * @brief Describes the type of directory object
  */
  enum class DirectoryObjectType {
    UNKNOWN = 0,
    INTERNAL = 1,  /**< Internal user - ANYONE */
    USER = 2, /**< individual user */
    GROUP = 3 /**< group object */
  };

  /**
   * @brief DirectoryObject constructor
   * 
   * @param type The type of directory object
   * @param email The email of directory object
   * @param objectId The object ID of directory object
   * @param puid The PUID of directory object
   */
  DirectoryObject(
      const DirectoryObjectType type, const std::string& email, const std::string& objectId, const std::string& puid)
      : mType(type), mEmail(email), mObjectId(objectId), mPuid(puid){};

  /**
   * @brief Gets type of directory object
   * 
   * @return Type of directory object
   */
  DirectoryObjectType GetDirectoryObjectType() const { return mType; };

  /**
   * @brief Gets email address of directory object
   * 
   * @return Email address of directory object
   */
  const std::string& GetEmail() const { return mEmail; };

  /**
   * @brief Gets object ID of directory object
   * 
   * @return object ID of directory object
   */
  const std::string& GetObjectId() const { return mObjectId; };

  /**
   * @brief Gets GUID of directory object
   * 
   * @return GUID of directory object
   */
  const std::string& GetPuid() const { return mPuid; };

private:
  DirectoryObjectType mType;
  std::string mEmail;
  std::string mObjectId;
  std::string mPuid;
};

/**
 * @brief The directory object and rights associated with it
 */
class DirectoryObjectRights {
public:
  /**
   * @brief DirectoryObjectRights constructor
   * 
   * @param directoryObject The directory object
   * @param rights Rights associated with the directory object
   */
  DirectoryObjectRights(const DirectoryObject& directoryObject, const std::vector<std::string>& rights)
      : mDirectoryObject(directoryObject), mRights(rights){};

  /**
   * @brief Gets directory object
   * 
   * @return The directory object
   */
  const DirectoryObject& GetDirectoryObject() const { return mDirectoryObject; };

  /**
   * @brief Gets rights associated with the directory object
   * 
   * @return Rights associated with the directory object
   */
  const std::vector<std::string>& GetRights() const { return mRights; };

private:
  DirectoryObject mDirectoryObject;
  std::vector<std::string> mRights;
};

/**
 * @brief The rights related information of the publishing license
 */
class LicenseRightsData {
public:
  /**
   * @brief LicenseRightsData constructor
   * 
   * @param type The type of protection, whether it originated from protection SDK template or not
   * @param directoryObjectsRights The set of directory objects and the rights associated with them
   * @param contentValidUtil Protection expiration time
   */
  LicenseRightsData(const ProtectionType type,
      const std::vector<DirectoryObjectRights>& directoryObjectsRights,
      const std::chrono::time_point<std::chrono::system_clock>& contentValidUntil)
      : mType(type), mDirectoryObjectsRights(directoryObjectsRights), mContentValidUntil(contentValidUntil){};

  /**
   * @brief Gets type of protection, whether it originated from protection SDK template or not
   * 
   * @return Type of protection
   */
  ProtectionType GetProtectionType() const { return mType; };

  /**
   * @brief Gets the set of directory objects and the rights associated with them
   * 
   * @return The set of directory objects and the rights associated with them
   */
  const std::vector<DirectoryObjectRights>& GetDirectoryObjectsRights() const { return mDirectoryObjectsRights; };

  /**
   * @brief Checks if content has an expiration time or not
   * 
   * @return True if content can expire, else false
   */
  bool DoesContentExpire() { return mContentValidUntil.time_since_epoch().count() != 0; };

  /**
   * @brief Gets protection expiration time
   * 
   * @return Protection expiration time
   */
  const std::chrono::time_point<std::chrono::system_clock> GetContentValidUntil() const { return mContentValidUntil; };

private:
  ProtectionType mType;
  std::vector<DirectoryObjectRights> mDirectoryObjectsRights;
  std::chrono::time_point<std::chrono::system_clock> mContentValidUntil;
};

MIP_NAMESPACE_END

#endif //API_MIP_PROTECTION_LICENSE_RIGHTS_DATA_H_
