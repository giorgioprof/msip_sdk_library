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
 * @brief Defines LicenseApplicationData interface
 * 
 * @file license_application_data.h
 */

#ifndef API_MIP_PROTECTION_LICENSE_APPLICATION_DATA_H_
#define API_MIP_PROTECTION_LICENSE_APPLICATION_DATA_H_

#include <string>

#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief The application data from a license
 */
class LicenseApplicationData {
public:
  /**
   * @brief Gets the application data name
   *
   * @return The name
   */
  virtual const std::string& GetName() const = 0;

  /**
   * @brief Gets the application data value
   *
   * @return The value
   */
  virtual const std::string& GetValue() const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~LicenseApplicationData() { }
  /** @endcond */

};

MIP_NAMESPACE_END
#endif // API_MIP_PROTECTION_LICENSE_APPLICATION_DATA_H_
