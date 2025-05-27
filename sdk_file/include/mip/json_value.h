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
 * @brief A file containing the JsonValue interface.
 *
 * @file json_value.h
 */
#ifndef API_MIP_JSON_VALUE_H_
#define API_MIP_JSON_VALUE_H_

#include <memory>
#include <string>
#include <vector>

#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

class JsonValue {
public:
  /**
   * @brief check if value is a string.
   *
   * @return true if json value is string.
   */
  virtual bool IsString() const = 0;
  /**
   * @brief check if value is a json array.
   *
   * @return true if json value is a json array.
   */
  virtual bool IsArray() const = 0;
  /**
   * @brief check if value is a json object.
   *
   * @return true if json value is a json object.
   */
  virtual bool IsObject() const = 0;
  /**
   * @brief return true if object has a child with the specified key.
   *
   * @return true if object has a child with the specified key.
   */
  virtual bool HasMember(const std::string& key) const = 0;
  /**
   * @brief Add a value member to an Array. The object should not be modified after being added.
   */
  virtual void PushBack(const std::shared_ptr<JsonValue>& jsonValue) = 0;
  /**
   * @brief Add a string member to an Array with the specified value.
   * @param member The value of the member to add.
   */
  virtual void PushBack(const std::string& member) = 0;
  /**
   * @brief Add a value member to an Object. The object should not be modified after being added.
   */
  virtual void AddMember(const std::string& key, const std::shared_ptr<JsonValue>& jsonValue) = 0;
  /*
  * @brief Add a string member to an Object with the specified key and value.
  * @param key The key of the member to add.
  * @param member The value of the member to add.
  */
  virtual void AddMember(const std::string& key, const std::string& member) = 0;
  /**
   * @brief Add a boolean member to an Object with the specified key and value.
   * @param key The key of the member to add.
   * @param member The value of the member to add.
   */
  virtual void AddMember(const std::string& key, bool member) = 0;
  /**
   * @brief Add a signed integer member to an Object with the specified key and value.
   * @param key The key of the member to add.
   * @param member The value of the member to add.
   */
  virtual void AddMember(const std::string& key, int member) = 0;
  /**
   * @brief Add an unsigned integer member to an Object with the specified key and value.
   * @param key The key of the member to add.
   * @param member The value of the member to add.
   */
  virtual void AddMember(const std::string& key, unsigned int member) = 0;
  /**
   * @brief return child value with the specified key if this is an Object.
   *
   * @return child value with the specified key if this is an Object.
   */
  virtual std::shared_ptr<JsonValue> GetMember(const std::string& key) const = 0;
  /**
   * @brief return child value at specified index if this is an Array.
   * @return child value at specified index if this is an Array.
   */
  virtual std::shared_ptr<JsonValue> GetMember(unsigned int index) const = 0;
  /**
   * @brief return number of child elements.
   * @return number of child elements.
   */
  virtual size_t Size() const = 0;
  /**
   * @brief return value array as strings, skip non string elements.
   *
   * @return a vector of the string in a json array.
   */
  virtual std::vector<std::string> GetStringArray() const = 0;
  /**
   * @brief Return all string members of this object and their keys, skip non string elements.
   * Example: for the json object {"a": "b", "c": "d", "e": 1, "f": {"g": "h"}}
   * this method would return a vector of two pairs: ("a", "b") and ("c", "d").
   * @return all string members of this object and their keys as a vector of pairs.
   */
  virtual std::vector<std::pair<std::string, std::string>> GetStringObjectMembers() const = 0;
  /**
   * @brief If the current value is a string, return it.
   * @return The current value if it is a string.
   */
  virtual std::string GetString() const = 0;
  /**
   * @brief Returns true if the current value is an int.
   * @return true if the current value is an int.
   */
  virtual bool IsInt() const = 0;
  /**
   * @brief If the current value is an int, return it.
   * @return The current value if it is an int.
   */
  virtual int GetInt() const = 0;
  /**
   * @brief Return true if the current value is a bool.
   * @return true if the current value is a bool.
   */
  virtual bool IsBool() const = 0;
  /**
   * @brief Return true if the current value is an unsigned int.
   * @return true if the current value is an unsigned int.
   */
  virtual bool IsUint() const = 0;
  /**
   * @brief If the current value is a uint, return it.
   * @return The current value if it is a uint.
   */
  virtual unsigned int GetUint() const = 0;
  /**
   * @brief Return true if the current value is a number of any type.
   * @return true if the current value is a number of any type.
   */
  virtual bool IsNumber() const = 0;
  /**
   * @brief If the current value is a double, return it.
   * @return The current value if it is a double.
   */
  virtual double GetDouble() const = 0;
  /**
   * @brief If the current value is a bool, return it.
   * @return The current value if it is a bool.
   */
  virtual bool GetBool() const = 0;
  /**
   * @brief Serialize the value and all children as a string.
   * @return This json value and all children serialized as a string.
   */
  virtual std::string SerializeToString() const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~JsonValue() {};
  /** @endcond */
};

MIP_NAMESPACE_END

#endif  // API_MIP_JSON_VALUE_H_
