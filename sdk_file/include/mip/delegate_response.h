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
 * @brief A simple framework to create a response to delegate calls that can result in error
 *
 * @file delegate_response.h
 */

#ifndef API_MIP_DELEGATE_RESPONSE
#define API_MIP_DELEGATE_RESPONSE

#include <memory>

#include "mip/error.h"
#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief Base class for all DelegateResponse classes
 */
class DelegateResponseBase {
  /** @cond DOXYGEN_HIDE */
protected:
  DelegateResponseBase() : mReserved(0) {}
  // For internal purpose only
  long mReserved;
  static long GetReserved(const DelegateResponseBase& response) {
    return response.mReserved;
  }
  /** DOXYGEN_HIDE*/
};

/**
 * @brief Template class to return delegate operation results
 */
template <class T> class DelegateResponse : public DelegateResponseBase {
public:
  /**
   * @brief Create a DelegateResponse to wrap returned objects or exceptions
   *
   * @param data The type of data that the delegate has created to return wrapped in a shared pointer
   */
  DelegateResponse(std::shared_ptr<T> data) : mData(data), mDelegateResponseError(nullptr) {
    if (!mData) {
      throw std::invalid_argument("Created an empty DelegateResponse");
    }
  }

  /**
   * @brief Create a DelegateResponse to wrap returned objects or exceptions
   *
   * @param error The error that was encountered while processing the delegate
   */
  DelegateResponse(mip::Error& error) : mData(nullptr), mDelegateResponseError(new DelegateResponseError(error)) {
    if (mDelegateResponseError->GetMessage().empty()) {
      throw std::invalid_argument("Created an empty DelegateResponse");
    }
  }

  /**
   * @brief Create a DelegateResponse to wrap returned objects or exceptions
   *
   * @param exception If the delegate encountered an exception it will populate this field with a exception in a shared pointer
   */
  explicit DelegateResponse(const std::exception_ptr& exception)
      : mData(nullptr),
        mDelegateResponseError(exception ? new DelegateResponseError(exception) : nullptr) {
    if (!mDelegateResponseError) {
      throw std::invalid_argument("Created an empty DelegateResponse");
    }
  }
  
  /**
   * @brief Returns the requested data, nullptr if call triggered an exception.
   * 
   * @return The data in a shared pointer of the templated type or nullptr.
   */
  std::shared_ptr<T> GetData() const { return mData; }

  /**
   * @brief Return any error generated during the call, nullptr if call completed successfully.
   * 
   * @return Exception pointer due to any failure in the delegate call or nullptr.
   */
  std::shared_ptr<mip::DelegateResponseError> GetError() const {
    return mDelegateResponseError;
  }

  /** @cond DOXYGEN_HIDE */
  virtual ~DelegateResponse() {}
protected:
  DelegateResponse() {}
  /** @endcond */

private:
  std::shared_ptr<T> mData;
  std::shared_ptr<mip::DelegateResponseError> mDelegateResponseError;
};

MIP_NAMESPACE_END

#endif // API_MIP_DELEGATE_RESPONSE