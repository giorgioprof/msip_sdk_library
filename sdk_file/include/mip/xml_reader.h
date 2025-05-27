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
 * @brief A file containing the XmlReader interface.
 *
 * @file xml_reader.h
 */
#ifndef API_MIP_XML_READER_H_
#define API_MIP_XML_READER_H_

#include <string>

#include "mip/mip_namespace.h"
#include "mip/xml_node.h"

MIP_NAMESPACE_BEGIN

namespace xml {

/**
 * @brief XmlReader abstraction class.
 * 
 */
class XmlReader {
public:
  /**
   * @brief Reads the next node of the element.
   *
   * @warning throws XmlParserException on failure
   * 
   * @return true if read node successfully.
   */
  virtual bool Read() = 0;
  /**
   * @brief Get current node type.
   *  
   * @return Xml Reader Node type.
   */
  virtual XmlNodeType GetNodeType() const = 0;
  /**
   * @brief Returns the name if found, exception otherwise. empty name is possible.
   * 
   * @warning throws XmlParserException on failure
   * 
   * @return node name.
   */
  virtual std::string GetName() const = 0;
  /**
   * @brief Returns if name was found. Sets name only when true. empty name is possible.
   * 
   * @return node name.
   */
  virtual bool GetName(std::string& name) const = 0;
  /**
   * @brief Skips current node until it finds a non empty node.
   * 
   * @warning throws XmlParserException on failure
   *  
   * @return true if name successful.
   */
  virtual bool Skip() = 0;
  /**
   * @brief Returns if value was found. Sets value only when true. empty value is possible..
   *
   * @param value output value
   * 
   * @return true if value exists else false.
   */
  virtual bool GetValue(std::string& value) const = 0;
  /**
   * @brief Returns current node ancestors (for debug text), in the following purpose <node grand parent attributes=...><node parent><node>.
   *
   * @note used for debugging purposes, and error logging.
   */
  virtual std::string GetAncestors() const = 0;
  /**
   * @brief Returns whether the current node as an empty element.
   *   
   * @warning throws XmlParserException on failure 
   * 
   * @return true if element is empty.
   */
  virtual bool IsEmptyElement() const = 0;
  /**
   * @brief Returns true if attributes was found. Sets attribute only when true. empty attribute is possible..
   *
   * @param attributeName attribute name.
   * @param attribute attribute value.
   * @return true if successful
   */
  virtual bool GetAttribute(const std::string& attributeName, std::string& attribute) const = 0;
  /**
   * @brief Returns whether the current node has attributes.
   * 
   * @warning throws XmlParserException on failure
   *
   * @return true if the current node has attributes.
   */
  virtual bool HasAttributes() const = 0;
  /**
   * @brief Move to first attribute.
   * 
   * @warning throws XmlParserException on failure
   *
   * @return true if successful
   */
  virtual bool MoveToFirstAttribute() = 0;
  /**
   * @brief Move to Next attribute.
   * 
   * @warning throws XmlParserException on failure
   *
   * @return true if successful
   */
  virtual bool MoveToNextAttribute() = 0;
  /**
   * @brief Move from attribute to element.
   * 
   * @warning throws XmlParserException on failure
   *
   * @return true if successful
   */
  virtual bool MoveToElement() = 0;
  /**
   * @brief Method returns all node elements in an unmodified way from the original xml.
   * 
   * @warning throws XmlParserException on failure
   *
   * @return entire serialized element.
   * 
   * @note moved to end of node.
   */
  virtual std::string DumpNode() = 0;
  /** @cond DOXYGEN_HIDE */
  virtual ~XmlReader() {}
protected:
  XmlReader() {}
  /** @endcond */
};

} // xml

MIP_NAMESPACE_END

#endif // API_MIP_XML_READER_H_
