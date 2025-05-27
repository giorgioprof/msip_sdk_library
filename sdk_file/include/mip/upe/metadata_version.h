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
 */
/**
 * @brief A file containing the MetadataVersion class and the MetadataVersionFormat enumerator.
 *
 * @file metadata_version.h
 */

#ifndef API_MIP_UPE_METADATA_VERSION_H_
#define API_MIP_UPE_METADATA_VERSION_H_

#include <cstdint>

#include "mip/mip_namespace.h"

using std::uint32_t;

MIP_NAMESPACE_BEGIN

/**
 * @brief Different algorithms to use when processing metadata.
 */
enum class MetadataVersionFormat : unsigned int {
  DEFAULT = 0,
  ONE_LABEL_PER_TENANT = 1 << 0          /**< When filtering label versions, only allow one label per tenant id*/
};

/**
 * @brief Or (|) operator for MetadataVersionFormat type enum.
 */
inline MetadataVersionFormat operator| (MetadataVersionFormat a, MetadataVersionFormat b) {
  return static_cast<MetadataVersionFormat>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}
/**
 * @brief And (&) operator for MetadataVersionFormat type enum.
 */
inline MetadataVersionFormat operator& (MetadataVersionFormat a, MetadataVersionFormat b) {
  return static_cast<MetadataVersionFormat>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}
/**
 * @brief Xor (^) operator for MetadataVersionFormat type enum.
 */
inline MetadataVersionFormat operator^ (MetadataVersionFormat a, MetadataVersionFormat b) {
  return static_cast<MetadataVersionFormat>(static_cast<unsigned int>(a) ^ static_cast<unsigned int>(b));
}

/**
 * @brief  Interface for a MetadataVersion. MetadataVersion determines which metadata is active and how it is processed.
 */
class MetadataVersion {
public:
  /**
   * @brief MetadataVersion constructor
   * @param version numerical version to use for metadata actions
   * @param flags flags to specify how the version is used to calculate metadata actions
   */
  MetadataVersion(uint32_t version, MetadataVersionFormat flags) : mVersion(version), mFlags(flags) {}
  /**
   * @brief Get the numerical version.
   *
   * @return The numerical version.
   */
  virtual uint32_t GetValue() const { return mVersion; }
  /**
   * @brief Get whether a specific flag is set.
   *
   * @return true if the flag is set.
   */
  virtual bool HasFlag(MetadataVersionFormat flag) const { return flag == (flag & mFlags); }
  /**
   * @brief Get the flags that define how metadata is processed for a given version.
   *
   * @return The flags that specify how the metadata is processed.
   */
  virtual MetadataVersionFormat GetFlags() const { return mFlags; }
  /** @cond DOXYGEN_HIDE */
  operator uint32_t() const { return mVersion; }
  virtual ~MetadataVersion() {}
private:
  uint32_t mVersion;
  MetadataVersionFormat mFlags;
  /** @endcond */
};

MIP_NAMESPACE_END

#endif  // API_MIP_UPE_METADATA_VERSION_H_
