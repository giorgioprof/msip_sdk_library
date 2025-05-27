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

#ifndef API_MIP_FILE_MSG_INSPECTOR_H_
#define API_MIP_FILE_MSG_INSPECTOR_H_

#include <memory>
#include <vector>

#include "mip/file/file_inspector.h"
#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief Body type enumerator.
 */
enum class BodyType : unsigned int {
  UNKNOWN = 0, /**< Unkown body type */
  TXT = 1, /**< Text style body type, encoding is returned as utf8 */
  HTML = 2, /**< HTML style body type, encoding is returned as utf8 */
  RTF = 3 /**< RTF style body type, a binary format */
};

class MsgAttachmentData {
public:
  /**
   * @brief Get attachment as a binary byte vector.
   */
  virtual const std::vector<uint8_t>& GetBytes() = 0;
  /**
   * @brief Get attachment as a binary stream.
   */
  virtual std::shared_ptr<Stream> GetStream() const = 0;
  /**
   * @brief Get attachment name as a string.
   */
  virtual const std::string& GetName() const = 0;
  /**
   * @brief Get attachment long name as a string.
   */
  virtual const std::string& GetLongName() const = 0;
  /**
   * @brief Get attachment path name as a string. if path not empty then reference attachment.
   */
  virtual const std::string& GetPath() const = 0;
  /**
   * @brief Get attachment long path name as a string. if path not empty then reference attachment.
   */
  virtual const std::string& GetLongPath() const = 0;
  /** @cond DOXYGEN_HIDE */
  virtual ~MsgAttachmentData() {}
  /** @endcond */
};

class MsgInspector : public FileInspector {
public:
  /**
   * @brief Get the body of the msg. if TXT/HTML formatted as utf8.
   *
   * @return a vector of bytes.
   */
  virtual const std::vector<uint8_t>& GetBody() const = 0;
  /**
   * @brief Get body encoding code page, relevant for txt, html body formats.
   * 
   * @return an unsigned code page. @see https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers
   */
  virtual unsigned int GetCodePage() const = 0;
  /**
   * @brief Get body type.
   *
   * @return the message's body type.
   */
  virtual BodyType GetBodyType() const = 0;
  /**
   * @brief Get a list of attachment as msg attachment data objects.
   *
   * @return a vector of std::unique_ptr<MsgAttachmentData>
   */
  virtual const std::vector<std::shared_ptr<MsgAttachmentData>>& GetAttachments() const = 0;
  /** @cond DOXYGEN_HIDE */
  virtual ~MsgInspector() {}
  /** @endcond */
};

MIP_NAMESPACE_END

#endif  // API_MIP_FILE_MSG_INSPECTOR_H_