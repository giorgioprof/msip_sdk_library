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
 * @brief Defines TemplateDescriptor interface
 * 
 * @file template_descriptor.h
 */
#ifndef API_MIP_PROTECTION_TEMPLATE_DESCRIPTOR_H_
#define API_MIP_PROTECTION_TEMPLATE_DESCRIPTOR_H_

#include <string> 

#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

class TemplateDescriptor {
public:
  /**
   * @brief Get Template ID
   *
   * @return Template ID
   */
  virtual std::string GetId() const = 0;
  /**
   * @brief Get Template name
   *
   * @return Template name
   */
  virtual std::string GetName() const = 0;
  /**
   * @brief Get Template description
   *
   * @return Template description
   */
  virtual std::string GetDescription() const = 0;

#ifdef MIP_OFFLINE_PUBLISHING_ENABLED
  /**
   * @brief Gets the Serialized Template bytes, if available.
   *
   * @return Serialized Template data
   */
  virtual std::vector<uint8_t> GetSerializedTemplate() const = 0;

  /**
   * @brief Gets if the serialized template is available.
   *
   * @return True, if serialized template was fetched. False, if not.
   */
  virtual bool HasSerializedTemplate() const = 0;

  /**
   * @brief Gets whether or not the owner is in the rights list with full access
   *
   * @return bool
   */
  virtual bool GetIsOwnerGrantedFullAccess() const = 0;
#endif // MIP_OFFLINE_PUBLISHING_ENABLED
  /** @cond DOXYGEN_HIDE */
  virtual ~TemplateDescriptor() { }
  /** @endcond */

};

MIP_NAMESPACE_END
#endif // API_MIP_PROTECTION_TEMPLATE_DESCRIPTOR_H_

