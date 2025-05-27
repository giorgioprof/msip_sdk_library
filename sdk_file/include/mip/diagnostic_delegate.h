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
 * @brief A file containing the DiagnosticDelegate class to be used to override MIP audit/telemetry
 * 
 * @file diagnostic_delegate.h
 */

#ifndef API_MIP_DIAGNOSTIC_DELEGATE_H_
#define API_MIP_DIAGNOSTIC_DELEGATE_H_

#include <memory>

#include "mip/event_context.h"
#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief A class that defines the interface to the MIP SDK audit/telemetry notifications.
 */
template <class T>
class DiagnosticDelegate {
public:

  /**
   * @brief Log a diagnostic event
   * 
   * @param event Event to be logged
   */
  virtual void WriteEvent(
    const std::shared_ptr<T>& event) = 0;

  /**
   * @brief Log a diagnostic event
   * 
   * @param event Event to be logged
   * @param eventContext EventContext associated with event
   */
  virtual void WriteEvent(
    const std::shared_ptr<T>& event, 
    const mip::EventContext& eventContext) = 0;

  /**
   * @brief Flush any queued events (e.g. due to shutdown)
   */
  virtual void Flush() = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~DiagnosticDelegate() {}
protected:
  DiagnosticDelegate() {}
   /** @endcond */
};

MIP_NAMESPACE_END

#endif // API_MIP_DIAGNOSTIC_DELEGATE_H_
