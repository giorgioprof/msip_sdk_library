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
 * @brief A file Containing the AddDynamicWatermarkAction class.
 *
 * @file add_watermark_action.h
 */

#ifndef API_MIP_UPE_ADD_DYNAMIC_WATERMARK_ACTION_H_
#define API_MIP_UPE_ADD_DYNAMIC_WATERMARK_ACTION_H_

#include <memory>

#include "mip/mip_namespace.h"
#include "mip/dynamic_watermark.h"
#include "mip/upe/action.h"

MIP_NAMESPACE_BEGIN
/**
 * @brief An action class that specifies adding watermark.
 */
class AddDynamicWatermarkAction : public Action {
public:
  /**
   * @brief Returns the dynamic watermark associated with this action request.
   *
   * @return The DynamicWatermark object
   */
  virtual const std::shared_ptr<const DynamicWatermark>& GetDynamicWatermark() const = 0;
};

MIP_NAMESPACE_END

#endif  // API_MIP_UPE_ADD_DYNAMIC_WATERMARK_ACTION_H_
