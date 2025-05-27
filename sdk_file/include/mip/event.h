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
 * @brief A file containing the Event class which describes a single audit/telemetry event
 * 
 * @file event.h
 */

#ifndef API_MIP_EVENT_H_
#define API_MIP_EVENT_H_

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "mip/diagnostic_types.h"
#include "mip/event_property.h"
#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief A single audit/telemetry event
 */
class Event {
public:
  /**
   * @brief Get event name
   * 
   * @return Event name
   */
  virtual const std::string& GetName() const = 0;

  /**
   * @brief Get level of event, indicating whether it is considered necessary service data (NSD) or not
   * 
   * @return Level of event
   */
  virtual EventLevel GetLevel() const = 0;

  /**
   * @brief Get event start time
   * 
   * @return Event start time
   */
  virtual const std::chrono::steady_clock::time_point& GetStartTime() const = 0;

  /**
   * @brief Add a property to the event
   * 
   * @param prop Property to add
   */
  virtual void AddProperty(const std::shared_ptr<EventProperty>& prop) = 0;

  /**
   * @brief Add a bool property to the event
   * 
   * @param name Property name
   * @param value Property value
   */
  virtual void AddProperty(const std::string& name, bool value) = 0;

  /**
   * @brief Add a double property to the event
   * 
   * @param name Property name
   * @param value Property value
   * @param pii PII classification
   */
  virtual void AddProperty(const std::string& name, double value, Pii pii) = 0;

  /**
   * @brief Add an int64 property to the event
   * 
   * @param name Property name
   * @param value Property value
   * @param pii PII classification
   */
  virtual void AddProperty(const std::string& name, int64_t value, Pii pii) = 0;

  /**
   * @brief Add a string property to the event
   * 
   * @param name Property name
   * @param value Property value
   * @param pii PII classification
   */
  virtual void AddProperty(const std::string& name, const std::string& value, Pii pii) = 0;

  /**
   * @brief Add an audit-only string property to the event
   * 
   * @param name Property name
   * @param value Property value
   * 
   * @note An audit-only property contains sensitive information and must not be written to file logs or to any pipeline
   *       except for audit until it is manually scrubbed.
   */
  virtual void AddAuditOnlyProperty(const std::string& name, const std::string& value) = 0;

  /**
   * @brief Get all event properties
   * 
   * @return Event properties
   */
  virtual std::vector<std::shared_ptr<EventProperty>> GetProperties() const = 0;

  /**
   * @brief Get property with the given name, if any
   * 
   * @param name Name of the property to get
   * 
   * @return Property with the given name, or nullptr if none
   */
  virtual std::shared_ptr<EventProperty> GetProperty(const std::string& name) const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~Event() {}
protected:
  Event() {}
   /** @endcond */
};

MIP_NAMESPACE_END
#endif // API_MIP_EVENT_H_

