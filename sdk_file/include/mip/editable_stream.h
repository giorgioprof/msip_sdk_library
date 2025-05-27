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
 * @brief A file containing the EditableStream interface/class definition.
 * 
 * @file editable_stream.h
 */

#ifndef API_MIP_EDITABLE_STREAM_H_
#define API_MIP_EDITABLE_STREAM_H_

#include "mip/mip_namespace.h"
#include "mip/stream.h"

MIP_NAMESPACE_BEGIN

class EditableStream : public Stream {
public:
  /**
   * @brief Update a number of bytes in the stream from a buffer.
   *
   * @param buffer pointer to a buffer
   * @param bufferLength buffer size.
   * @param replaceLength number of bytes to replace.
   * 
   * @return number of bytes written.
   * 
   * @note This will seek to the end of the updated section. 
   */
  virtual int64_t Update(const uint8_t* buffer, int64_t bufferLength, int64_t replaceLength) = 0;

  /**
   * @brief Delete a number of bytes from the stream.
   *
   * @param numBytes number of bytes to delete.
   * 
   * @return number of bytes deleted.
   * 
   * @note This will not move the stream position.
   */
  virtual int64_t Delete(int64_t numBytes) = 0;

  /**
   * @brief Insert a buffer into the stream.
   * 
   * @param buffer pointer to a buffer
   * @param bufferLength buffer size.
   * 
   * @return number of bytes written.
   * 
   * @note This will seek to the end of the inserted section. 
   */
  virtual int64_t Insert(const uint8_t* buffer, int64_t bufferLength) = 0;
};

MIP_NAMESPACE_END

#endif  // API_MIP_EDITABLE_STREAM_H_