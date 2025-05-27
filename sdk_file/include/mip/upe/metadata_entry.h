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
 * @brief This file contains the MetadataEntry class.
 * 
 * @file metadata_entry.h
 */

#ifndef API_MIP_UPE_METADATA_ENTRY_H_
#define API_MIP_UPE_METADATA_ENTRY_H_

#include <string>

#include "mip/mip_namespace.h"
#include "mip/upe/metadata_version.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief An abstraction class for metadata entry.
 * 
 */
class MetadataEntry {
public:
  /**
   * @brief C'tor for a MetadataEntry abstraction
   * 
   * @param key metadata key entry.
   * @param value metadata value entry
   * @param version metadata version value
   */  
  MetadataEntry(const std::string& key, const std::string& value, uint32_t version) 
    : MetadataEntry(key, value, MetadataVersion(version, MetadataVersionFormat::DEFAULT)){
  }
  /**
   * @brief C'tor for a MetadataEntry abstraction
   * 
   * @param key metadata key entry.
   * @param value metadata value entry
   * @param version metadata version value
   */  
  MetadataEntry(const std::string& key, const std::string& value, const MetadataVersion& version) 
    : mKey(key),
      mValue(value), 
      mVersion(version){
  }
  /**
   * @brief C'tor for a MetadataEntry abstraction, version is set to a default of 0.
   * 
   * @param key metadata key entry.
   * @param value metadata value entry
   */
  MetadataEntry(const std::string& key, const std::string& value) 
    : MetadataEntry(key, value, 0) {
  }
  /**
   * @brief Get the metadata entry key.
   * 
   * @return metadata entry key.
   */
  const std::string& GetKey() const { return mKey; }
  /**
   * @brief Get the metadata entry value.
   * 
   * @return metadata entry value.
   */
  const std::string& GetValue() const { return mValue; }
  /**
   * @brief Get the metadata entry version.
   * 
   * @return metadata entry version.
   */
  MetadataVersion GetVersion() const { return mVersion; }

  /** @cond DOXYGEN_HIDE */
  bool operator==(const MetadataEntry& other) const {
    return mKey == other.mKey && mValue == other.mValue && mVersion == other.mVersion;
  }

  virtual ~MetadataEntry() { }
private: 
  std::string mKey;
  std::string mValue;
  MetadataVersion mVersion;
  /** @endcond */
};

MIP_NAMESPACE_END

#endif  // API_MIP_UPE_METADATA_ENTRY_H_
