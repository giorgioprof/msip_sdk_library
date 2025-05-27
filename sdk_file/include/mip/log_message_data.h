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
 * @brief Contains the LogMessageData class for logging
 * 
 * @file log_message_data.h
 */


#ifndef API_MIP_LOG_MESSAGE_DATA_H_
#define API_MIP_LOG_MESSAGE_DATA_H_

#include <chrono>
#include <memory>
#include <string>
#include <thread>

#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN
/**
 * @brief Different log levels used across the MIP SDK.
 */
enum class LogLevel : unsigned int {
  Trace   = 0, /* Log statement is of type Trace */
  Info    = 1, /* Log statement is of type Info */
  Warning = 2, /* Log statement is of type Warning */
  Error   = 3, /* Log statement is of type Error */
};

/**
 * @brief A class that stores log messages
 */
class LogMessageData {
public:
  LogMessageData(
      const LogLevel level,
      const std::string& message,
      const std::string& function,
      const std::string& file,
      int32_t line,
      const std::shared_ptr<void>& context,
      std::chrono::time_point<std::chrono::system_clock> messageTime,
      std::thread::id messageThreadId)
    : mLevel(level),
      mLogMessage(message),
      mFunction(function),
      mFile(file),
      mLine(line),
      mContext(context),
      mMessageTime(messageTime),
      mMessageThreadId(messageThreadId) {
  }

   /**
   * @brief The log level for the log statement
   */
  LogLevel GetLevel() const { return mLevel; }

   /**
   * @brief The message for the log statement
   */
  const std::string& GetLogMessage() const { return mLogMessage; }

   /**
   * @brief The function name for the log statement
   */
  const std::string& GetFunction() const { return mFunction; }

   /**
   * @brief The file name for the log statement
   */
  const std::string& GetFile() const { return mFile; }

   /**
   * @brief The line number for the log statement
   */
  int32_t GetLine() const { return mLine; }

   /**
   * @brief The logger context for the log statement
   */
  const std::shared_ptr<void>& GetContext() const { return mContext; }

   /**
   * @brief The message time of the log statement
   */
  std::chrono::time_point<std::chrono::system_clock> GetMessageTime() const { return mMessageTime; }

   /**
   * @brief The thread id of the log statement
   */
  std::thread::id GetMessageThreadId() const { return mMessageThreadId; }

private:
  LogLevel mLevel;
  std::string mLogMessage;
  std::string mFunction;
  std::string mFile;
  int32_t mLine;
  std::shared_ptr<void> mContext;
  std::chrono::time_point<std::chrono::system_clock> mMessageTime;
  std::thread::id mMessageThreadId;
};

MIP_NAMESPACE_END
#endif  // API_MIP_LOG_MESSAGE_DATA_H_

