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
 * @brief Defines stream callback functions
 * 
 * @file stream_cc.h
 */

#ifndef API_MIP_STREAM_CC_H_
#define API_MIP_STREAM_CC_H_

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/result_cc.h"

/**
 * @brief Callback function definition for reading into a buffer from the stream
 * 
 * @param context user provided context
 * @param buffer pointer to a buffer
 * @param bufferLength buffer size.
 * @return number of bytes read.
 */
MIP_CC_CALLBACK(mip_cc_stream_read_callback_fn /*typename*/,
    int64_t /*return*/,
    void* context,
    uint8_t* buffer,
    int64_t bufferLength);

/**
 * @brief Callback function definition for Writing into the stream from a buffer.
 *
 * @param context user provided context
 * @param buffer pointer to a buffer
 * @param bufferLength buffer size.
 * @return number of bytes written.
 */
MIP_CC_CALLBACK(mip_cc_stream_write_callback_fn /*typename*/,
    int64_t /*return*/,
    void* context,
    const uint8_t* buffer,
    int64_t bufferLength);

/**
 * @brief callback function definition for flushing the stream.
 *
 * @param context user provided context
 * @return true if successful else false.
 */
MIP_CC_CALLBACK(mip_cc_stream_flush_callback_fn /*typename*/,
    bool /*return*/,
    void* context);

/**
 * @brief callback function definition for Seeking specific position within the stream.
 *
 * @param context user provided context
 * @param position to seek into stream.
 */
MIP_CC_CALLBACK(mip_cc_stream_seek_callback_fn /*typename*/,
    void /*return*/,
    void* context,
    int64_t position );

/**
 * @brief call back function definition for a check if stream can be read from.
 *
 * @param context user provided context
 * @return true if readable else false.
 */
MIP_CC_CALLBACK(mip_cc_stream_canread_callback_fn /*typename*/,
    bool /*return*/,
    void* context);

/**
 * @brief call back function definition for a check if stream can be written to.
 *
 * @param context user provided context
 * @return true if writeable else false.
 */
MIP_CC_CALLBACK(mip_cc_stream_canwrite_callback_fn /*typename*/,
    bool /*return*/,
    void* context);

/**
 * @brief call back function definition for getting the current position within the stream.
 *
 * @param context user provided context
 * @return position within the stream.
 */
MIP_CC_CALLBACK(mip_cc_stream_position_callback_fn /*typename*/,
    int64_t /*return*/,
    void* context);

/**
 * @brief call back function definition for getting the size of the content within the stream.
 *
 * @param context user provided context
 * @return the stream size.
 */
MIP_CC_CALLBACK(mip_cc_stream_getsize_callback_fn /*typename*/,
    int64_t /*return*/,
    void* context);

/**
 * @brief call back function definition for setting the stream size.
 *
 * @param context user provided context
 * @param stream size.
 */
MIP_CC_CALLBACK(mip_cc_stream_setsize_callback_fn /*typename*/,
    void /*return*/,
    void* context,
    int64_t value);

typedef mip_cc_handle* mip_cc_stream;

/**
 * @brief Creates a stream 
 * 
 * @param readCallback Function pointer for reading into a buffer from the stream
 * @param writeCallback Function pointer for writeing into the stream from a buffer
 * @param flushCallback Function pointer for writing a log statement
 * @param seekCallback Function pointer for seeking specific position within the stream
 * @param canReadCallback Function pointer for a check if stream can be read from
 * @param canWriteCallback Function pointer for a check if stream can be written to
 * @param positionCallback Function pointer for getting the current position within the stream
 * @param getSizeCallback Function pointer for getting the size of the content within the stream
 * @param setSizeCallback Function pointer for setting the stream size
 * @param context Context that will be supplied to each callback function as the first arg. Should remain valid for the lifetime of the stream
 * @param stream [Output] Handle to stream object
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateStream(
    const mip_cc_stream_read_callback_fn readCallback,
    const mip_cc_stream_write_callback_fn writeCallback,
    const mip_cc_stream_flush_callback_fn flushCallback,
    const mip_cc_stream_seek_callback_fn seekCallback,
    const mip_cc_stream_canread_callback_fn canReadCallback,
    const mip_cc_stream_canwrite_callback_fn canWriteCallback,
    const mip_cc_stream_position_callback_fn positionCallback,
    const mip_cc_stream_getsize_callback_fn getSizeCallback,
    const mip_cc_stream_setsize_callback_fn setSizeCallback,
    void *context,
    mip_cc_stream* stream,
    mip_cc_error* errorInfo);


/**
 * @brief Release resources associated with a stream handle
 * 
 * @param stream stream to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseStream(mip_cc_stream stream);

#endif // API_MIP_STREAM_CC_H_
