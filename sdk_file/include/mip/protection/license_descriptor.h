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
 * @brief Defines LicenseDescriptor interface
 * 
 * @file license_descriptor.h
 */
#ifndef API_MIP_PROTECTION_LICENSE_DESCRIPTOR_H_
#define API_MIP_PROTECTION_LICENSE_DESCRIPTOR_H_

#include <memory>
#include <string>
#include <vector>

#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief The descriptor line item from a license
 */
class LicenseNameAndDescriptionItem {
public:
  /**
   * @brief Gets the lcid
   *
   * @return The LCID
   */
  virtual uint32_t GetLCID() const = 0;

  /**
   * @brief Gets the name
   *
   * @return The name
   */
  virtual const std::string& GetName() const = 0;

  /**
   * @brief Gets the description
   *
   * @return The description
   */
  virtual const std::string& GetDescription() const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~LicenseNameAndDescriptionItem() { }
  /** @endcond */

};

/**
 * @brief The descriptor from a license
 */
class LicenseDescriptor {
public:
  /**
   * @brief Gets the Id
   *
   * @return The Id
   */
  virtual const std::string& GetId() const = 0;

  /**
   * @brief Gets the descriptor items
   *
   * @return The descriptor items
   */
  virtual const std::vector<std::shared_ptr<LicenseNameAndDescriptionItem>>& GetDescriptorItems() const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~LicenseDescriptor() { }
  /** @endcond */
};

MIP_NAMESPACE_END
#endif // API_MIP_PROTECTION_LICENSE_DESCRIPTOR_H_
