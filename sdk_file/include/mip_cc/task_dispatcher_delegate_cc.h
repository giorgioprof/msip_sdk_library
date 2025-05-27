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
 * @brief Defines task dispatcher callback functions
 * 
 * @file task_dispatcher_delegate_cc.h
 */

#ifndef API_MIP_TASK_DISPATCHER_DELEGATE_CC_H_
#define API_MIP_TASK_DISPATCHER_DELEGATE_CC_H_

#include <stddef.h>
#include <stdint.h>

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/result_cc.h"

/**
 * @brief Defines a single async task dispatch request
 */
typedef struct {
  const char* id;                   /**< Task ID */
  int64_t delayMs;                  /**< Delay until task execution (in milliseconds) */
  bool executeOnIndependentThread;  /**< Whether this task should execute on a completely independent thread or can re-use a shared thread */
  void* loggerContext;              /**< logger context that should be used with the logger delegate before the task is run */
} mip_cc_async_task;

/**
 * @brief Callback function definition for dispatching an async task
 * 
 * @param taskId Unique task identifier
 */
MIP_CC_CALLBACK(mip_cc_dispatch_task_callback_fn /*typename*/,
    void /*return*/,
    const mip_cc_async_task* /*task*/);

/**
 * @brief Callback function for canceling a background tasks
 * 
 * @param taskId Unique task identifier
 * 
 * @return True if task was successfully cancelled, else false
 */
MIP_CC_CALLBACK(mip_cc_cancel_task_callback_fn /*typename*/,
    bool /*return*/,
    const char* /*taskId*/);

/**
 * @brief Callback function for canceling all background tasks
 */
MIP_CC_CALLBACK(mip_cc_cancel_all_tasks_callback_fn /*typename*/,
    void /*return*/);

typedef mip_cc_handle* mip_cc_task_dispatcher_delegate;

/**
 * @brief Creates a task dispatcher delegate which can be used to override MIP's default async task handling
 * 
 * @param dispatchTaskCallback Function pointer for dispatching async tasks
 * @param cancelTaskCallback Function pointer for cancelling background tasks
 * @param cancelAllTasksCallback Function pointer for cancelling all background tasks
 * @param taskDispatcher [Output] Handle to task dispatcher delegate object
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_CreateTaskDispatcherDelegate(
    const mip_cc_dispatch_task_callback_fn dispatchTaskCallback,
    const mip_cc_cancel_task_callback_fn cancelTaskCallback,
    const mip_cc_cancel_all_tasks_callback_fn cancelAllTasksCallback,
    mip_cc_task_dispatcher_delegate* taskDispatcher,
    mip_cc_error* errorInfo);

/**
 * @brief Notifies a TaskDispatcher delegate that a task is scheduled to execute now on the current thread
 * 
 * @param taskDispatcher Handle to task dispatcher delegate object
 * @param taskId ID of async task associated with this operation
 * 
 * @note This function must be called by the application when a task is scheduled to execute. It will result in
 *       immediate execution of the task on the current thread. The ID should match that of a previously-dispatched,
 *       non-cancelled task.
 */
MIP_CC_API(void) MIP_CC_ExecuteDispatchedTask(const mip_cc_task_dispatcher_delegate taskDispatcher, const char* taskId);

/**
 * @brief Release resources associated with an task dispatcher delegate handle
 * 
 * @param taskDispatcher Task dispatcher delegate to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseTaskDispatcherDelegate(mip_cc_task_dispatcher_delegate taskDispatcher);

#endif // API_MIP_TASK_DISPATCHER_DELEGATE_CC_H_
