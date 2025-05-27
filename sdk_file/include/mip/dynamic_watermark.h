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
 * @brief Defines DynamicWatermark interface
 *
 * @file dynamic_watermark.h
 */

#ifndef API_MIP_DYNAMIC_WATERMARK_H_
#define API_MIP_DYNAMIC_WATERMARK_H_

#include <string>

#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief Representation of the dynamic watermark which the admin has configured for a label
 */
class DynamicWatermark {
  public:

    /** @cond DOXYGEN_HIDE */
    virtual ~DynamicWatermark() = default;
    /** @endcond */

    /**
     * @brief Gets the raw formatted string the admin has configured, which may include variables such as
     * ${Consumer.PrincipalName}.
     * This string can be populated from the label in the protection flow
     * or it can be populated from the use license in the consumption flow.
     *
     * @return The raw format string
     */
    virtual const std::string& GetFormatString() const = 0; 

    /**
     * @brief Gets the text which will be displayed to the end user by replacing the variables
     * in the format string.
     *
     * @return The display text an end user sees for the dynamic watermark, with the supported variables replaced
     */
    virtual std::string GetText() const = 0; 
};

MIP_NAMESPACE_END

#endif  // API_MIP_DYNAMIC_WATERMARK_H_