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
 * @brief Contains XmlDelegate interface definition used to parse xml recieved by mip
 *
 * @file xml_delegate.h
 */

#ifndef API_MIP_XML_DELEGATE_H_
#define API_MIP_XML_DELEGATE_H_

#include "mip/delegate_response.h"
#include "mip/xml_document.h"
#include "mip/xml_reader.h"

MIP_NAMESPACE_BEGIN

namespace xml {

typedef mip::DelegateResponse<mip::xml::XmlDocument> XmlDocumentResult;
typedef mip::DelegateResponse<mip::xml::XmlReader> XmlReaderResult;

/**
 * @brief Interface for overriding XML handling (must be capable of handling multiple threads)
*/
class XmlDelegate {
public:
  /**
  * @brief Create an xml reader that can transverse the input. This method will throw an XmlLoadException 
  * if it cannot initialize an XML reader, if the input is invalid, or if the input is too large to handle.
  *
  * @param xmlParserInput Input containing either xml in string format or a uri to xml
  *
  * @return A delegate response that contains either a shared pointer to a reader 
  * that can look at each node of the xml sequentially, or an XmlLoadException
  */
  virtual XmlReaderResult CreateXmlReader(const std::string& xmlParserInput) const = 0;
  
  /**
  * @brief Parse an xml formatted buffer into an XmlDocument
  *
  * @param data A string that should be in xml format
  * 
  * @warning The XmlDocument returned should memory manage itself. That is, when it goes out of scope, 
  * it should take care of any free functions needed to release memory. In our native implementation, this is achieved with
  * a unique_ptr with a specialized deleter function.
  *
  * @return A delegate response that contains either a shared_ptr to an XmlDocument or an exception
  * if parsing fails
  */
  virtual XmlDocumentResult ParseData(const std::string& data) const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~XmlDelegate() {}
  /** @endcond */
};

} // xml

MIP_NAMESPACE_END
#endif  // API_MIP_XML_DELEGATE_H_

