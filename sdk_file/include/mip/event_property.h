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
 * @brief A file containing the EventProperty class which describes a single audit/telemetry property
 * 
 * @file event_property.h
 */

#ifndef API_MIP_EVENT_PROPERTY_H_
#define API_MIP_EVENT_PROPERTY_H_

#include <memory>
#include <string>

#include "mip/diagnostic_types.h"
#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief A single audit/telemetry property
 */
class EventProperty {
public:
  /**
   * @brief Get the underlying data type for this property
   * 
   * @return Underlying data type
   */
  virtual EventPropertyType GetPropertyType() const = 0;

  /**
   * @brief Get name of property
   * 
   * @return Name of property
   */
  virtual const std::string& GetName() const = 0;

  /**
   * @brief Get personally-identifiable information (PII) classification, if any
   * 
   * @return PII classification
   */
  virtual Pii GetPii() const = 0;

  /**
   * @brief Get whether or not this property is restricted to the audit pipeline
   * 
   * @return Whether or not this properties is restricted to the audit pipeline
   * 
   * @note If this is true, the property contains sensitive information and must not be written to file logs or to
   *       any pipeline except for audit until it is manually scrubbed.
   */
  virtual bool IsAuditOnly() const = 0;

  /**
   * @brief Get property value (double)
   * 
   * @return Property value
   */
  virtual double GetDouble() const = 0;

  /**
   * @brief Get property value (int64)
   * 
   * @return Property value
   */
  virtual int64_t GetInt64() const = 0;

  /**
   * @brief Get property value (string)
   * 
   * @return Property value
   */
  virtual const std::string& GetString() const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~EventProperty() {}
protected:
  EventProperty() {}
   /** @endcond */
};

MIP_NAMESPACE_END
#endif  // API_MIP_EVENT_PROPERTY_H_

