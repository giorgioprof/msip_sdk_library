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
  * @brief Contains XmlNode interface definition used to interact with xml parsers
  *
  * @file xml_node.h
  */

#ifndef API_MIP_XML_NODE_H_
#define API_MIP_XML_NODE_H_

#include <memory>
#include <string>
#include <vector>

#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

namespace xml {

struct XmlNamespace {
  std::string prefix;
  std::string uri;
};

/**
 * @brief Constants for the different types of xml elements nodes.
 */
enum class XmlNodeType : int {
  UNKNOWN = -1,
  NONE = 0,
  ELEMENT = 1,
  ATTRIBUTE = 2,
  TEXT = 3,
  CDATA = 4,
  ENTITY_REFERENCE = 5,
  ENTITY = 6,
  PROCESSING_INSTRUCTION = 7,
  COMMENT = 8,
  DOCUMENT = 9,
  DOCUMENT_TYPE = 10,
  DOCUMENT_FRAGMENT = 11,
  NOTATION = 12,
  WHITESPACE = 13,
  SIGNIFICANT_WHITESPACE = 14,
  END_ELEMENT = 15,
  END_ENTITY = 16,
  XML_DECLARATION = 17
};

/**
* brief defines abstraction over XML node.
* @warning XmlNodes will be treated as memory managed by XmlDocument except for
* RemoveNodeFromDocument() which should handle its own memory clean up
*/
/* Example XML for methods
(1) <bk:book xmlns:bk='testbook' bk:ISBN='1-861001-57-5'>
(2)   <title>
(3)     Pride And Prejudice
(4)   </title>
(5)   <author>Jane Austen</author>
(6) </bk:book>
*/
class XmlNode {
public:
  /**
    * @brief Get the value of an attribute of this node
    *
    * @param attributeName The attribute name to search for 
    *
    * @return The value of that attribute or an empty string if not found
    * 
    * @example (1)->GetAttributeValue("ISBN") == "1-861001-57-5"
    */
  virtual std::string GetAttributeValue(const std::string& attributeName) const = 0;
  /**
    * @brief Get all attributes of this node
    *
    * @return A vector of pairs representing each attribute name and value
    * 
    * @example (1)->GetAttributes() == {("ISBN", "1-861001-57-5")}
    */
  virtual std::vector<std::pair<std::string, std::string>> GetAttributes() const = 0;
  /**
    * @brief Gets the next node on the same level of the heirarchy as the current node
    * 
    * @return The next XmlNode on the same level as this one or a null XmlNode if there are none
    * 
    * @example (2)->GetNextNode() == (5)
    */
  virtual std::shared_ptr<XmlNode> GetNextNode() const = 0;
  /**
    * @brief Get the first child of the node
    *
    * @return A shared pointer to the first child of the object node, or a null XmlNode if no children
    * 
    * @example (2)->GetFirstChild() == (3)
    */
  virtual std::shared_ptr<XmlNode> GetFirstChild() const = 0;
  /**
    * @brief Get the name of the current node
    * 
    * @return A string representing the name of the node
    * 
    * @example (2)->GetName == "title"
    */
  virtual std::string GetName() const = 0;
  /**
    * @brief Get the content of a text node
    *
    * @return A string matching the value of a text node. Empty if not a text node.
    * 
    * @example (3)->GetContent == "Pride And Prejudice"
    */
  virtual std::string GetContent() const = 0;
  /**
    * @brief Get the inner text of a element node with a text node as the first child
    * 
    * @return A string matching the internal text of the child of the node
    * 
    * @example (2)->GetInnerText == "Pride And Prejudice"
    */
  virtual std::string GetInnerText() const = 0;
  /**
    * @brief Get the uri and prefix of node
    * 
    * @return An XmlNamespace obect with the strings of uri and prefix filled to match any namespace
    * empty strings if no namespace
    * 
    * @example (1)->GetNamespace == XmlNamespace(uri="testbook", prefix="bk")
    */
  virtual XmlNamespace GetNamespace() const = 0;
  /**
    * @brief Get the type of node this xml is represented as
    * 
    * @return A member of the enum describing the node type
    */
  virtual XmlNodeType GetNodeType() const = 0;
  /**
    * @brief See if the object has been initialized with an underlying xml node
    * 
    * @return True if underlying node. False if not.
    */
  virtual bool IsNull() const = 0;

  /**
    * @brief Add a property to an existing node
    *
    * @param attributeName Name of the attribute to add 
    * @param attributeValue Value of the attribute to add
    */
  virtual void AddAttribute(const std::string& attributeName, const std::string& attributeValue) = 0;

  /**
    * @brief Removes a node property by name
    *
    * @param attributeName The property name to remove 
    *
    * @return 0 if the property was found and successfully removed and -1 otherwise
    */
  virtual int RemoveAttribute(const std::string& attributeName) = 0;
  /**
    * @brief Add a child to this XmlNode. New child is inserted after any existing children
    *
    * @param name The name of the new node 
    *
    * @return A pointer to the node that was added or a nullptr if the operation failed.
    */
  virtual std::shared_ptr<XmlNode> AddNewChild(const std::string& name) = 0;
  /**
    * @brief Add a child to this XmlNode. New child is inserted after any existing children
    *
    * @param name The name of the new node 
    * @param namespaceName The namespace prefix to insert the new node under
    *
    * @return A pointer to the node that was added or a nullptr if the operation failed.
    */
  virtual std::shared_ptr<XmlNode> AddNewChild(const std::string& name, const std::string& namespaceName) = 0;
  /**
    * @brief Add inner text to this xml node
    *
    * @param content The string to add as inner text
    *     
    * @return true if content was added or false if the operation failed.
    */
  virtual bool AddContent(const std::string& content) = 0;
  /**
    * @brief Remove this node and children from the xml document
    * 
    * @return true if node was successfully deleted or false if failed.
    */
  virtual bool RemoveNodeFromDocument() = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~XmlNode() {}
  /** @endcond */
};

} // xml

MIP_NAMESPACE_END

#endif // API_MIP_XML_NODE_H_
