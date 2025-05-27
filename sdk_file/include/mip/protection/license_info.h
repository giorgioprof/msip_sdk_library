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
 * @brief Defines LicenseInfo interface
 *
 * @file license_info.h
 */
#ifndef API_MIP_PROTECTION_LICENSE_INFO_H_
#define API_MIP_PROTECTION_LICENSE_INFO_H_

#include <memory>
#include <string>
#include <vector>

#include "mip/dynamic_watermark.h"
#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief Representation of the dynamic watermark which the admin has configured for a label
 */
class LicenseInfo {
  public:

    /**
     * @brief Gets the user rights.
     *
     * @return Vector of strings indicating the user's rights
     */
    virtual std::vector<std::string> GetUserRights() const = 0; 

    /**
     * @brief Gets the DynamicWatermark object that holds the dynamic watermark text to display on the
     *        end-user's document.
     *
     * @return A shared pointer to the DynamicWatermark object
     */
    virtual std::shared_ptr<const mip::DynamicWatermark> GetDynamicWatermark() const = 0; 
};

MIP_NAMESPACE_END

#endif  // API_MIP_LICENSE_INFO_H_