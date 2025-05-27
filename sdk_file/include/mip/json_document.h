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
 * @brief A file containing the JsonDocument interface.
 *
 * @file json_document.h
 */
#ifndef API_MIP_JSON_DOCUMENT_H_
#define API_MIP_JSON_DOCUMENT_H_

#include <memory>
#include <string>
#include <vector>

#include "mip/json_value.h"
#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN
/**
 * @brief JsonDocument abstraction class.
 * 
 */
class JsonDocument {
public:
  /**
   * @brief Gets a pointer to the root node of the document.
   * @return A pointer to the root node of the document.
   */
  virtual std::shared_ptr<JsonValue> Root() const = 0;
  /**
   * @brief Create an Object value to later be added as a child of this document.
   * @return An Object value to later be added as a child of this document.
   */
  virtual std::shared_ptr<JsonValue> CreateObjectValue() = 0;
  /**
   * @brief Create an Array value to later be added as a child of this document.
   * @return An Array value to later be added as a child of this document.
   */
  virtual std::shared_ptr<JsonValue> CreateArrayValue() = 0;
  /** @cond DOXYGEN_HIDE */
  virtual ~JsonDocument() {}

protected:
  JsonDocument() {}
  /** @endcond */
};

MIP_NAMESPACE_END

#endif // API_MIP_JSON_DOCUMENT_H_
