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
 * @brief A file containing the LoggerDelegate class to be used to override MIP
 * logger
 * 
 * @file logger_delegate.h
 */


#ifndef API_MIP_LOGGER_DELEGATE_H_
#define API_MIP_LOGGER_DELEGATE_H_

#include <memory>
#include <string>

#include "mip/log_message_data.h"
#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief A class that defines the interface to the MIP SDK logger.
 */
class LoggerDelegate {
public:
   /**
   * @brief Initialize the logger.
   *
   * @param storagePath the path to the location where persistent state, including logs, may be stored.
   */
  virtual void Init(const std::string& storagePath) = 0;
   /**
   * @brief Flush the logger
   */
  virtual void Flush() = 0;
   /**
   * @brief Write a log statement to log file
   * 
   * @param level the log level for the log statement.
   * @param message the message for the log statement.
   * @param function the function name for the log statement.
   * @param file the file name where log statement was generated.
   * @param line the line number where the log statement was generated.
   */
  virtual void WriteToLog (
      const LogLevel level,
      const std::string& message,
      const std::string& function,
      const std::string& file,
      const int32_t line) = 0;

   /**
   * @brief Write a log statement to log file with a context.  Override this function to handle the context
   * 
   * @param level the log level for the log statement.
   * @param message the message for the log statement.
   * @param function the function name for the log statement.
   * @param file the file name where log statement was generated.
   * @param line the line number where the log statement was generated.
   * @param context Client context that was passed into the call that generated this log message
   */
  virtual void WriteToLogWithContext (
      const LogLevel level,
      const std::string& message,
      const std::string& function,
      const std::string& file,
      const int32_t line,
      const std::shared_ptr<void>& /*context*/) {
    WriteToLog(level, message, function, file, line);
  }

   /**
   * @brief Write a log statement to a log
   * 
   * @param logMessage The log message data containing the logging information
   */
  virtual void WriteToLogWithLogMessage(const LogMessageData& logMessage) {
    WriteToLogWithContext(
        logMessage.GetLevel(),
        logMessage.GetLogMessage(),
        logMessage.GetFunction(),
        logMessage.GetFile(),
        logMessage.GetLine(),
        logMessage.GetContext());
  }

  /** @cond DOXYGEN_HIDE */
  virtual ~LoggerDelegate() {}
protected:
  LoggerDelegate() {}
   /** @endcond */
};

MIP_NAMESPACE_END
#endif  // API_MIP_LOGGER_DELEGATE_H_

