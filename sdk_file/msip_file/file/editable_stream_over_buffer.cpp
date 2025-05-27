/**
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
#include "editable_stream_over_buffer.h"

#include <cstring>
#include <stdexcept>
#include <vector>

using std::copy;
using std::runtime_error;
using std::vector;

#ifdef _WIN32
#define MEMCPY(dest, destSize, src, count) memcpy_s(dest, destSize, src, count)
#else // _WIN32
#define MEMCPY(dest, destSize, src, count) memcpy(dest, src, count)
#endif // _WIN32

EditableStreamOverBuffer::EditableStreamOverBuffer(vector<uint8_t>&& buffer)
    : mBuffer(buffer),
      mSize(static_cast<uint64_t>(buffer.size())),
      mPosition(0) {
}

EditableStreamOverBuffer::~EditableStreamOverBuffer() { }

int64_t EditableStreamOverBuffer::Read(uint8_t* buffer, int64_t bufferLength) {
  if (bufferLength <= 0)
    return 0;
  auto bytesRead = (bufferLength <= mSize - mPosition) ? bufferLength : mSize - mPosition;
  if (bytesRead) {
    MEMCPY(buffer, static_cast<size_t>(bufferLength), &mBuffer[static_cast<size_t>(mPosition)], static_cast<size_t>(bytesRead));
    mPosition += bytesRead;
  }
  return bytesRead;
}

int64_t EditableStreamOverBuffer::Write(const uint8_t* buffer, int64_t bufferLength) {
  return Update(buffer, bufferLength, bufferLength);
}

int64_t EditableStreamOverBuffer::Insert(const uint8_t* buffer, int64_t bufferLength) {
  if (bufferLength <= 0)
    return 0;
  if (std::numeric_limits<int64_t>::max() - mSize < bufferLength)
    throw runtime_error("Inserting buffer would exceed maximum stream length.");
  mBuffer.insert(mBuffer.begin() + static_cast<size_t>(mPosition), buffer, buffer + static_cast<size_t>(bufferLength));
  mSize += bufferLength;
  mPosition += bufferLength;
  return bufferLength;
};

int64_t EditableStreamOverBuffer::Update(const uint8_t* buffer, int64_t bufferLength, int64_t replaceLength) {
  Delete(replaceLength);
  return Insert(buffer, bufferLength);
};

int64_t EditableStreamOverBuffer::Delete(int64_t numBytes) {
  if (numBytes <= 0)
    return 0;
  auto bytesDeleted = (numBytes <= mSize - mPosition) ? numBytes : mSize - mPosition;
  if (bytesDeleted) {
    mBuffer.erase(mBuffer.begin() + static_cast<size_t>(mPosition), mBuffer.begin() + static_cast<size_t>(mPosition + bytesDeleted));
    mSize -= bytesDeleted;
  }
  return bytesDeleted;
};

bool EditableStreamOverBuffer::Flush() { return true; }

void EditableStreamOverBuffer::Seek(int64_t position) {
  if (position < 0)
    throw runtime_error("Position must not be less than zero.");
  if (position > mSize)
    throw runtime_error("Position must not be larger than size.");
  mPosition = position;
}

bool EditableStreamOverBuffer::CanRead() const { return true; }

bool EditableStreamOverBuffer::CanWrite() const { return true; }

int64_t EditableStreamOverBuffer::Position() { return mPosition; }

int64_t EditableStreamOverBuffer::Size() {return mSize; }

void EditableStreamOverBuffer::Size(int64_t /* value */) {
  throw runtime_error("Not Implemented");
}