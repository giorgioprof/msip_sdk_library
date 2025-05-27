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
  * @brief Contains XmlDocument interface definition used to interact with xml parsers
  *
  * @file xml_document.h
  */
#ifndef API_MIP_XML_DOCUMENT_H_
#define API_MIP_XML_DOCUMENT_H_

#include <memory>
#include <string>
#include <vector>

#include "mip/mip_namespace.h"
#include "mip/xml_node.h"

MIP_NAMESPACE_BEGIN

namespace xml {

/**
 * Interface to interact with XML document.
 */
class XmlDocument {
public:
  /**
    * @brief Get matching xml nodes from a specified xpath. Will throw exception of type mip::BadInputError 
    * if xml nodes cannot be parsed, there is an invalid encoding, or the xpath cannot be interpreted
    *
    * @param xpath The path to search the xml document for the node 
    * @param node Specify children of a specific node
    * 
    * @return A vector of matching XmlNode shared pointers or an empty vector if none match
    */
  virtual std::vector<std::shared_ptr<XmlNode>> SelectNodes(
      const std::string& xpath,
      const std::shared_ptr<XmlNode>& node = nullptr) const = 0;
  /**
    * @brief Parse an xml formatted buffer into an xmldocument
    *
    * @return A utf-8 xml formatted string of the content of this XmlDocument
    */
  virtual std::string GetXmlContent() const = 0;
  /**
    * @brief Get the node that is an ancestor of all other nodes
    *
    * @return A shared_ptr to an XmlNode
    */
  virtual std::shared_ptr<XmlNode> GetRootNode() const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~XmlDocument() {}
  /** @endcond */
};

} // xml

MIP_NAMESPACE_END

#endif // API_MIP_XML_DOCUMENT_H_
