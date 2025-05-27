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
 * @brief Defines logger callback functions
 * 
 * @file logger_delegate_cc.h
 */

#ifndef API_MIP_LOGGER_DELEGATE_CC_H_
#define API_MIP_LOGGER_DELEGATE_CC_H_

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/result_cc.h"

/**
 * @brief Log level
 */
typedef enum {
  MIP_LOG_LEVEL_TRACE = 0,  /**< Trace */
  MIP_LOG_LEVEL_INFO,       /**< Info */
  MIP_LOG_LEVEL_WARNING,    /**< Warning */
  MIP_LOG_LEVEL_ERROR,      /**< Error */
} mip_cc_log_level;

typedef struct {
  int size;                         /*The size of this mip_cc_log_message_data.  Used for versioning*/
  mip_cc_log_level level;           /*the log level for the log statement*/
  const char* message;              /*the message for the log statement*/
  const char* function;             /*the function name for the log statement*/
  const char* file;                 /*the file name where log statement was generated.*/
  int32_t line;                     /*the line number where the log statement was generated.*/
  void* context;                    /*the logger context for the operation*/
  int64_t milliSecondsSinceEpoch;   /*the time that the log message was created in milliseconds since epoch (UTC)*/
  const char* messageThreadId;      /*The thread id that the message was created in*/
} mip_cc_log_message_data;

/**
 * @brief Callback function definition for initialization of logger
 * 
 * @param storagePath File path where logs may be stored
 */
MIP_CC_CALLBACK(mip_cc_logger_init_callback_fn /*typename*/,
    void /*return*/,
    const char* /*storagePath*/);

/**
 * @brief Callback function definition for flushing logs to disk
 */
MIP_CC_CALLBACK(mip_cc_logger_flush_callback_fn /*typename*/,
    void /*return*/);

/**
 * @brief Callback function definition for writing a log statement
 * 
 * @param level the log level for the log statement.
 * @param message the message for the log statement.
 * @param function the function name for the log statement.
 * @param file the file name where log statement was generated.
 * @param line the line number where the log statement was generated.
 */
MIP_CC_CALLBACK(mip_cc_logger_write_callback_fn /*typename*/,
    void /*return*/,
    const mip_cc_log_level /*level*/,
    const char* /*message*/,
    const char* /*function*/,
    const char* /*file*/,
    const int32_t /*line*/);

/**
 * @brief Callback function definition for writing a log statement
 * 
 * @param logMessageData The log data
 */
MIP_CC_CALLBACK(mip_cc_logger_write_with_message_data_callback_fn /*typename*/,
    void /*return*/,
    mip_cc_log_message_data logMessageData);

typedef mip_cc_handle* mip_cc_logger_delegate;

/**
 * @brief Creates a logger delegate which can be used to override MIP's default logger
 * 
 * @param initCallback Function pointer for initialization
 * @param flushCallback Function pointer for flushing logs
 * @param writeCallback Function pointer for writing a log statement
 * @param loggerDelegate [Output] Handle to logger delegate object
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateLoggerDelegate(
    const mip_cc_logger_init_callback_fn initCallback,
    const mip_cc_logger_flush_callback_fn flushCallback,
    const mip_cc_logger_write_callback_fn writeCallback,
    mip_cc_logger_delegate* loggerDelegate,
    mip_cc_error* errorInfo);

typedef struct {
    int size;                                       /*The size of this mip_cc_create_logger_delegate_message_data_args. Used for versioning*/
    mip_cc_logger_init_callback_fn initCallback;    /*Function pointer for initialization*/
    mip_cc_logger_flush_callback_fn flushCallback;  /*Function pointer for flushing logs*/
    mip_cc_logger_write_with_message_data_callback_fn writeCallback; /*Function pointer for writing a log statement*/
    mip_cc_logger_delegate* loggerDelegate;         /*[Output] Handle to logger delegate object*/
    mip_cc_error* errorInfo;                        /*[Output] (Optional) Failure info if operation result is error*/
} mip_cc_create_logger_delegate_message_data_args;

/**
 * @brief Creates a logger delegate which can be used to override MIP's default logger
 * 
 * @param methodargs Arguments to this function
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateLoggerDelegateMessageData(
    mip_cc_create_logger_delegate_message_data_args methodArgs);

/**
 * @brief Release resources associated with an logger delegate handle
 * 
 * @param loggerDelegate logger delegate to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseLoggerDelegate(mip_cc_logger_delegate loggerDelegate);

#endif // API_MIP_LOGGER_DELEGATE_CC_H_
