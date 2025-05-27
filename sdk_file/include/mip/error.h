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
 * @brief A file containing the MIP SDK error types.
 * 
 * @file error.h
 */

#ifndef API_MIP_ERROR_H_
#define API_MIP_ERROR_H_

#include <algorithm>
#include <exception>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "mip/common_types.h"
#include "mip/mip_namespace.h"

namespace {

constexpr const char* GetStackTraceString() {
  return "StackTrace";
}

constexpr const char* GetHResultString() {
  return "HResult";
}

constexpr const char* GetBadInputErrorString() {
  return "BadInputError";
}

constexpr const char* GetNoPermissionsExtendedErrorName() { return "NoPermissionsExtendedError"; }

constexpr const char* GetErrorInfoCodesKey() { return "ExtendedErrorInfo_Codes"; }

constexpr const char* GetErrorInfoMessagesKey() { return "ExtendedErrorInfo_Messages"; }

constexpr const char* GetErrorInfoDetailsKey() { return "ExtendedErrorInfo_Details"; }

} // namespace

MIP_NAMESPACE_BEGIN

enum class ErrorType : unsigned int {
  BAD_INPUT_ERROR, /**< Caller passed bad input. */
  INSUFFICIENT_BUFFER_ERROR, /**< Caller passed a buffer that was too small. */
  FILE_IO_ERROR, /**< General File IO error. */
  NETWORK_ERROR, /**< General network issues; for example, unreachable service. */
  INTERNAL_ERROR, /**< Internal unexpected errors. */
  JUSTIFICATION_REQUIRED, /**< Justification should be provided to complete the action on the file. */
  NOT_SUPPORTED_OPERATION, /**< The requested operation is not yet supported. */
  PRIVILEGED_REQUIRED, /**< Can't override privileged label when new label method is standard. */
  ACCESS_DENIED, /**< The user could not get access to services. */
  CONSENT_DENIED, /**< An operation that required consent from user was not granted consent. */
  NO_PERMISSIONS, /**< The user could not get access to the content. For example, no permissions, content revoked */
  NO_AUTH_TOKEN, /**< The user could not get access to the content due to an empty auth token. */
  DISABLED_SERVICE, /**< The user could not get access to the content due to the service being disabled */
  PROXY_AUTH_ERROR, /**< Proxy authentication failed. */
  NO_POLICY, /**< No policy is configured for user/tenant */
  OPERATION_CANCELLED, /**< Operation cancelled */
  ADHOC_PROTECTION_REQUIRED, /**< Adhoc protection should be set to complete the action on the file*/
  DEPRECATED_API, /**< Caller invoked a deprecated API */
  TEMPLATE_NOT_FOUND, /**< Template ID is not recognized */
  LABEL_NOT_FOUND, /**< Label ID is not recognized */
  LABEL_DISABLED, /**< Label is disabled or inactive */
  DOUBLE_KEY_DISABLED, /**< The double key feature has not been enabled */
  LICENSE_NOT_REGISTERED,  /**< License not registered for tracking and revocation */
  CONTENT_FORMAT_NOT_SUPPORTED, /**< Content format is not supported */
  TEMPLATE_ARCHIVED, /**< Template has been archived and is unavailable for protection */
  CUSTOMER_KEY_UNAVAILABLE, /**< Customer key not available when attempting to fetch for Bring Your Own Key Protection */
  DELEGATE_RESPONSE, /**< Error generated from delegated response */
  COUNT, /**< Last element in this enum. Used to keep track of the number of mip::ErrorTypes */
};

struct ExtendedErrorInfo {
  std::string code;
  std::string message;
  std::map<std::string, std::string> details;
};

/**
 * @brief Base class for all errors that will be reported (thrown or returned) from MIP SDK.
 */
class Error : public std::exception {
public:
  /**
   * @brief Get the error message.
   * 
   * @return the error message
   */
  char const* what() const noexcept override {
    return mFormattedMessage.c_str();
  }

  /**
   * @brief Clone the error
   * 
   * @return a clone of the error.
   */
  virtual std::shared_ptr<Error> Clone() const = 0;

  /**
   * @brief Get the error type.
   * 
   * @return the error type.
   */
  virtual ErrorType GetErrorType() const { return mType; }

  /**
   * @brief Get the error name.
   * 
   * @return the error name. 
   */
  const std::string& GetErrorName() const { return mName; }

  /**
   * @brief Get the error message.
   * 
   * @return the error message.
   */
  const std::string& GetMessage(bool maskPII = false) const {
    return maskPII ? mMaskedMessage : mFormattedMessage;
  }

  /**
   * @brief Set the error message.
   * 
   * @param msg the error message.
   */
  void SetMessage(const std::string& msg) {
    std::string* targetStrings[] = { &mFormattedMessage, &mMaskedMessage };
    for (auto* targetString : targetStrings) {
      size_t pos = targetString->find(mMessage);
      if (pos != std::string::npos) {
        targetString->replace(pos, mMessage.length(), msg);
      } else {
        targetString->replace(0, 0, msg);
      }
    }
    mMessage = msg;
  }

  /**
   * @brief Add debug info entry
   * 
   * @param key Debug info key
   * @param value Debug info value
   */
  void AddDebugInfo(const std::string& key, const std::string& value, bool sensitive = false) {
    if (!key.empty() && !value.empty()) {
      mDebugInfo[key] = value;
      mFormattedMessage = mFormattedMessage + ", " + key + "=" + value;
      mMaskedMessage = mMaskedMessage + ", " + key + "=" + (sensitive ? "***" : value);
    }
  }

  /**
   * @brief Get debug info
   * 
   * @return Debug info (keys/values)
   */
  const std::map<std::string, std::string>& GetDebugInfo() const { return mDebugInfo; }

  /** @cond DOXYGEN_HIDE */
  virtual ~Error() {}

protected:
  explicit Error(
      const std::string& message,
      const std::string& name,
      ErrorType type)
      : mMessage(message), mName(name), mType(type) {
    mFormattedMessage = CreateFormattedMessage(message);
    mMaskedMessage = mFormattedMessage;
  }
  explicit Error(
      const std::string& message,
      const std::map<std::string, std::string> debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name,
      ErrorType type)
      : mMessage(message), mName(name), mType(type) {
    mFormattedMessage = CreateFormattedMessage(message);
    mMaskedMessage = mFormattedMessage;
    for (const auto& entry: sensitiveDebugInfo) {
      AddDebugInfo(entry.first, entry.second, true);
    }
    for (const auto& entry: debugInfo) {
      AddDebugInfo(entry.first, entry.second);
    }
  }
  /** @endcond */

  std::string mMessage;
  std::map<std::string, std::string> mDebugInfo;
  std::string mName;
  ErrorType mType;

private:
  std::string CreateFormattedMessage(const std::string& message) const {
    auto formattedMessage = message;

    // Remove stray newlines
    auto isNewlineFn = [](char c) { return c == '\n' || c == '\r'; };
    formattedMessage.erase(std::remove_if(formattedMessage.begin(), formattedMessage.end(), isNewlineFn), formattedMessage.end());

    return formattedMessage;
  }

  std::string mFormattedMessage;
  std::string mMaskedMessage;
};

/**
 * @brief Bad input error, thrown when the input to an SDK API is invalid.
 */
class BadInputError : public Error {
public:

   /**
   * @brief ErrorCode of bad input error
   */
  enum class ErrorCode {
    General = 0, /**< General bad input error */
    FileIsTooLargeForProtection = 1, /**< File is too large for protection */
    ParameterParsing = 2, /**< Parameter cannot be parsed correctly */
    LicenseNotTrusted = 3, /**< Publishing license not issued by trusted source */
    DoubleKey = 4, /**< A paremeter for double key encryption is needed and missing */
    FileFormatNotSupported = 5, /**<The input file's format is not supported */
  };

  /** @cond DOXYGEN_HIDE */
  explicit BadInputError(
      const std::string& message,
      const std::string& name = GetBadInputErrorString(),
      ErrorCode errorCode = ErrorCode::General)
      : BadInputError(message, {{}}, {{}}, name, errorCode) {}

  explicit BadInputError(
      const std::string& message,
      ErrorCode errorCode,
      const std::string& name = GetBadInputErrorString())
      : BadInputError(message, {{}}, {{}}, name, errorCode) {}

  explicit BadInputError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = GetBadInputErrorString(),
      ErrorCode errorCode = ErrorCode::General)
      : Error(message, debugInfo, sensitiveDebugInfo, name, ErrorType::BAD_INPUT_ERROR),
        mErrorCode(errorCode) {
    AddDebugInfo("BadInputError.Code", GetErrorCodeString(errorCode));
  }
  std::shared_ptr<Error> Clone() const override { return std::make_shared<BadInputError>(*this); }
  /** @endcond */

   /**
   * @brief Gets the errorCode of bad input
   *
   * @return ErrorCode of bad input error
   */
  ErrorCode GetErrorCode() const { return mErrorCode; }

private:
  ErrorCode mErrorCode;

  const std::string& GetErrorCodeString(ErrorCode code) const {
    static const std::string kUnrecognized = "UNRECOGNIZED";
    static const std::map<ErrorCode, std::string> kCodes = {
        { ErrorCode::General,                       "General" },
        { ErrorCode::FileIsTooLargeForProtection,   "FileIsTooLargeForProtection" },
        { ErrorCode::ParameterParsing,              "ParameterParsing" },
        { ErrorCode::LicenseNotTrusted,             "LicenseNotTrusted" },
        { ErrorCode::DoubleKey,                     "DoubleKey" },
        { ErrorCode::FileFormatNotSupported,        "FileFormatNotSupported"},
    };
    return kCodes.count(code) ? kCodes.at(code) : kUnrecognized;
  }
};

/**
 * @brief Delegate Response Error. Thrown or returned in response to encountering an error in a delegate method.
 */
class DelegateResponseError : public Error {
public:
  /**
   * @brief Creates an error/exception object. Call this method from a MIP delegate function to create a MIP or 
   * standard C++ exception object.
   * 
   * @param except The C++ exception that was encountered.
   */
  explicit DelegateResponseError(const std::exception_ptr& except) :
      Error(std::string(), std::string("DelegateResponseError"), ErrorType::DELEGATE_RESPONSE),
      mCurrentException(except) {
    if (except) {
      try {
        std::rethrow_exception(except);
      } catch(const Error& error) {
        *static_cast<Error*>(this) = error;
      } catch (const std::exception& thisException) {
        this->mMessage = std::string(thisException.what());
      }
    }
  }

  /**
   * @brief Creates an error/exception object. Call this method from a MIP delegate function to create a generic
   * MIP C++ exception object.
   * 
   * @param message Message associated with the exception.
   * @param stackTrace The stack trace at the time of the exception.
   * @param name Some string to uniquely identify the type of this exception.
   */
  DelegateResponseError(
    const std::string& message,
    const std::string& stackTrace,
    const std::string& name = "DelegateResponseError")
      : Error(message, name, ErrorType::DELEGATE_RESPONSE) {
    AddDebugInfo(GetStackTraceString(), stackTrace);
  }

  /**
   * @brief Creates an error/exception object. Call this method from a MIP delegate function to create a generic
   * MIP C++ exception object.
   * 
   * @param message Message associated with the exception.
   * @param HResult HResult that identifies the error that caused this exception.
   * @param stackTrace The stack trace at the time of the exception.
   * @param name Some string to uniquely identify the type of this exception.
   */
  DelegateResponseError(
    const std::string& message,
    long HResult,
    const std::string& stackTrace,
    const std::string& name = "DelegateResponseError")
      : Error(message, name, ErrorType::DELEGATE_RESPONSE) {
    std::stringstream hrStream;
    hrStream << std::hex << HResult;
    AddDebugInfo(GetHResultString(), hrStream.str());
    AddDebugInfo(GetStackTraceString(), stackTrace);
  }

  /**
   * @brief Creates an error/exception object. Call this method from a MIP delegate function to create a generic
   * MIP C++ exception object.
   * 
   * @param message Message associated with the exception.
   * @param HResult HResult that identifies the error that caused this exception.
   */
  DelegateResponseError(const std::string& message, long HResult)
      : Error(message, std::string("DelegateResponseError"), ErrorType::DELEGATE_RESPONSE) {
    std::stringstream hrStream;
    hrStream << std::hex << HResult;
    AddDebugInfo(GetHResultString(), hrStream.str());
  }

  /**
   * @brief Creates an error/exception object. Call this method from a MIP delegate function to create a generic
   * MIP C++ exception object.
   * 
   * @param message Message associated with the exception.
   */
  DelegateResponseError(const std::string& message)
      : Error(message, std::string("DelegateResponseError"), ErrorType::DELEGATE_RESPONSE) {
  }

  /** @cond DOXYGEN_HIDE */
  explicit DelegateResponseError(const Error& error) : Error(error) {
    mType = ErrorType::DELEGATE_RESPONSE;
  }

  std::shared_ptr<Error> Clone() const override {
    // There's nothing special about this class - a clone can simply call the copy constructor.
    auto result = std::make_shared<mip::DelegateResponseError>(*this);
    return std::static_pointer_cast<Error>(result);
  }
  
  ErrorType GetErrorType() const override { return ErrorType::DELEGATE_RESPONSE; }

  const std::exception_ptr& GetExceptionPtr() const { return mCurrentException; }

private:
  std::exception_ptr mCurrentException;
  /** @endcond */ 
};

/**
 * @brief Insufficient buffer error
 */
class InsufficientBufferError : public BadInputError {
public:
  /** @cond DOXYGEN_HIDE */
  explicit InsufficientBufferError(
      const std::string& message,
      const std::string& name = "InsufficientBufferError")
      : InsufficientBufferError(message, {{}}, {{}}, name) {}
  explicit InsufficientBufferError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "InsufficientBufferError")
      : BadInputError(message, debugInfo, sensitiveDebugInfo, name) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<InsufficientBufferError>(*this); }
  ErrorType GetErrorType() const override { return ErrorType::INSUFFICIENT_BUFFER_ERROR; }
  /** @endcond */
};

/**
 * @brief File IO error.
 */
class FileIOError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit FileIOError(
      const std::string& message,
      const std::string& name = "FileIOError")
      : FileIOError(message, {{}}, {{}}, name) {}
  explicit FileIOError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "FileIOError")
      : Error(message, debugInfo, sensitiveDebugInfo, name, ErrorType::FILE_IO_ERROR) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<FileIOError>(*this); }
  /** @endcond */ 
};

/**
 * @brief Networking error. Caused by unexpected behavior when making network calls to service endpoints.
 */
class NetworkError : public Error {
public:
  /**
   * @brief Category of network error
   */
  enum class Category {
    Unknown = 0, /**< Unknown network failure */
    FailureResponseCode = 1, /**< HTTP response code indicates failure */
    BadResponse = 2, /**< HTTP response could not be read */
    UnexpectedResponse = 3, /**< HTTP response completed but contained unexpected data */
    NoConnection = 4, /**< Failed to establish a connection */
    Proxy = 5, /**< Proxy failure */
    SSL = 6, /**< SSL failure */
    Timeout = 7, /**< Connection timed out */
    Offline = 8, /**< Operation requires network connectivity */
    Throttled = 9, /**< HTTP operation failed due to server traffic throttling */
    Cancelled = 10, /**< HTTP operation has been cancelled by the application */
    FunctionNotImplemented = 11, /**< HTTP response code indicates called function is not implemented */
    ServiceUnavailable = 12, /**< HTTP response code indicates service is unavailable */
  };

  /** @cond DOXYGEN_HIDE */
  explicit NetworkError(
      Category category,
      const std::string& sanitizedUrl,
      const std::string& requestId,
      int32_t statusCode,
      const std::string& message,
      const std::string& name = "NetworkError")
      : NetworkError(category, statusCode, message, {{}}, {{}}, name) {
    if (!sanitizedUrl.empty())
      AddDebugInfo("HttpRequest.SanitizedUrl", sanitizedUrl);
    if (!requestId.empty())
      AddDebugInfo("HttpRequest.Id", requestId);
  }

  explicit NetworkError(
      Category category,
      int32_t statusCode,
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "NetworkError")
      : Error(message, debugInfo, sensitiveDebugInfo, name, ErrorType::NETWORK_ERROR),
        mCategory(category),
        mResponseStatusCode(statusCode) {
    AddDebugInfo("NetworkError.Category", GetCategoryString(category));
    if (statusCode != 0)
      AddDebugInfo("HttpResponse.StatusCode", std::to_string(static_cast<int>(statusCode)));
  }

  std::shared_ptr<Error> Clone() const override {
    return std::make_shared<NetworkError>(*this);
  }
  /** @endcond */ 

  /**
   * @brief Gets the category of network failure
   * 
   * @return Category of network failure
   */
  Category GetCategory() const { return mCategory; }

  /**
   * @brief Gets the HTTP response status code
   * 
   * @return HTTP response status code, 0 if none
   */
  int32_t GetResponseStatusCode() const { return mResponseStatusCode; }

private:
  Category mCategory;
  int32_t mResponseStatusCode;

  const std::string& GetCategoryString(Category category) const {
    static const std::string kUnrecognized = "UNRECOGNIZED";
    static const std::map<Category, std::string> kCategories = {
        { Category::Unknown,             "Unknown" },
        { Category::FailureResponseCode, "FailureResponseCode" },
        { Category::BadResponse,         "BadResponse" },
        { Category::UnexpectedResponse,  "UnexpectedResponse" },
        { Category::NoConnection,        "NoConnection" },
        { Category::Proxy,               "Proxy" },
        { Category::SSL,                 "SSL" },
        { Category::Timeout,             "Timeout" },
        { Category::Offline,             "Offline" },
        { Category::Throttled,           "Throttled" },
        { Category::ServiceUnavailable,  "ServiceUnavailable" },
        { Category::Cancelled,           "Cancelled" },
    };
    return kCategories.count(category) ? kCategories.at(category) : kUnrecognized;
  }
};

/**
 * @brief Proxy authentication failure
 */
class ProxyAuthenticationError : public NetworkError {
public:
  /** @cond DOXYGEN_HIDE */
  explicit ProxyAuthenticationError(
      const std::string& sanitizedUrl,
      const std::string& requestId,
      int32_t statusCode,
      const std::string& message,
      const std::string& name = "ProxyAuthenticationError")
      : NetworkError(Category::Proxy, sanitizedUrl, requestId, statusCode, message, name) {}
  explicit ProxyAuthenticationError(
      int32_t statusCode,
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "ProxyAuthenticationError")
      : NetworkError(Category::Proxy, statusCode, message, debugInfo, sensitiveDebugInfo, name) {}
  std::shared_ptr<Error> Clone() const override {
    return std::make_shared<ProxyAuthenticationError>(*this);
  }
  ErrorType GetErrorType() const override { return ErrorType::PROXY_AUTH_ERROR; }
  /** @endcond */ 
};

/**
 * @brief Internal error. This error is thrown when something unexpected happens during execution.
 */
class InternalError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit InternalError(
      const std::string& message,
      const std::string& name = "InternalError")
      : InternalError(message, {{}}, {{}}, name) {}
  explicit InternalError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "InternalError")
      : Error(message, debugInfo, sensitiveDebugInfo, name, ErrorType::INTERNAL_ERROR) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<InternalError>(*this); }
  /** @endcond */ 
};

/**
 * @brief The operation requested by the application is not supported by the SDK.
 */
class NotSupportedError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit NotSupportedError(
      const std::string& message,
      const std::string& name = "NotSupportedError")
      : NotSupportedError(message, {{}}, {{}}, name) {}
  explicit NotSupportedError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "NotSupportedError")
      : Error(message, debugInfo, sensitiveDebugInfo, name, ErrorType::NOT_SUPPORTED_OPERATION) {}
  explicit NotSupportedError(
      const std::string& message,
      ErrorType errorType,
      const std::string& name = "NotSupportedError")
      : Error(message, name, errorType) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<NotSupportedError>(*this); }
  /** @endcond */ 
};

/**
 * @brief Current label was assigned as a privileged operation (The equivalent to an administrator operation),
 * therefore it can't be overriden.
 */
class PrivilegedRequiredError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit PrivilegedRequiredError(
      const std::string& message,
      const std::string& name = "PrivilegedRequiredError")
      : PrivilegedRequiredError(message, {{}}, {{}}, name) {}
  explicit PrivilegedRequiredError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "PrivilegedRequiredError")
      : Error(message, debugInfo, sensitiveDebugInfo, name, ErrorType::PRIVILEGED_REQUIRED) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<PrivilegedRequiredError>(*this); }
  /** @endcond */ 
};

/**
 * @brief The user could not get access to the content. For example, no permissions, content revoked.
 */
class AccessDeniedError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit AccessDeniedError(
      const std::string& message,
      const std::string& name = "AccessDeniedError")
      : AccessDeniedError(message, {{}}, {{}}, name) {}
  explicit AccessDeniedError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "AccessDeniedError")
      : Error(message, debugInfo, sensitiveDebugInfo, name, ErrorType::ACCESS_DENIED) {}
  virtual std::shared_ptr<Error> Clone() const override { return std::make_shared<AccessDeniedError>(*this); }
  /** @endcond */ 
};

/**
 * @brief The user could not get access to the content. For example, no permissions, content revoked.
 */
class NoPermissionsError : public AccessDeniedError {
public:
  /**
   * @brief Category of no permissions error
   */
  enum class Category {
    Unknown = 0, /**< Unknown no permissions failure */
    UserNotFound = 1,           /**< Requested user was not found failure */
    AccessDenied = 2,           /**< Access to content or action was not permitted */
    AccessExpired = 3,          /**< Access to content or action has expired */
    InvalidEmail = 4,
    UnknownTenant = 5,
    NotOwner = 6,               /**< User needs to be owner to perform action */
    NotPremiumLicenseUser = 7,  /**< User needs to be a premium license holder to perform action. Tracking and Revocation for example */
    ClientVersionNotSupported = 8, /**< User needs to update their client in order to support features used within this document >*/
  };

  /** @cond DOXYGEN_HIDE */
  explicit NoPermissionsError(
      Category category,
      const std::string& message,
      const std::string& referrer = "",
      const std::string& owner = "",
      const std::string& name = "NoPermissionsError")
      : NoPermissionsError(category, message, referrer, owner, {{}}, {{}}, name) {}
  explicit NoPermissionsError(
      Category category,
      const std::string& message,
      const std::string& referrer,
      const std::string& owner,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "NoPermissionsError")
      : AccessDeniedError(message, debugInfo, sensitiveDebugInfo, name), mCategory(category), mReferrer(referrer), mOwner(owner) {
        AddDebugInfo("NoPermissionsError.Category", GetCategoryString(mCategory));
        if (!referrer.empty())
          AddDebugInfo("NoPermissionsError.Referrer", referrer, true);
        if (!owner.empty())
          AddDebugInfo("NoPermissionsError.Owner", owner, true);
      }
#if !defined(SWIG) && !defined(SWIG_DIRECTORS)
    [[deprecated]]
#endif
  explicit NoPermissionsError(
      const std::string& message,
      const std::string& referrer,
      const std::string& owner,
      const std::string& name = "NoPermissionsError")
      : NoPermissionsError(Category::Unknown, message, referrer, owner, {{}}, {{}}, name) {}
#if !defined(SWIG) && !defined(SWIG_DIRECTORS)
    [[deprecated]]
#endif
  explicit NoPermissionsError(
      const std::string& message,
      const std::string& referrer,
      const std::string& owner,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "NoPermissionsError")
      : NoPermissionsError(Category::Unknown, message, referrer, owner, debugInfo, sensitiveDebugInfo, name) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<NoPermissionsError>(*this); }
  ErrorType GetErrorType() const override { return ErrorType::NO_PERMISSIONS; }
  /** @endcond */ 

  /**
   * @brief Gets the contact in case of missing rights to the document.
   * 
   * @return The contact in case of missing rights to the document.
   */
  std::string GetReferrer() const { return mReferrer; }

  /**
   * @brief Gets the owner of the document
   * 
   * @return Document owner
   */
  std::string GetOwner() const { return mOwner; }

  /**
   * @brief Gets the category of no permissions failure
   * 
   * @return Category of no permissions failure
   */
  Category GetCategory() const { return mCategory; }

private:
  Category mCategory;
  std::string mReferrer;
  std::string mOwner;

  const std::string& GetCategoryString(Category category) const {
    static const std::string kUnrecognized = "UNRECOGNIZED";
    static const std::map<Category, std::string> kCategories = {
        { Category::Unknown,                    "Unknown" },
        { Category::UserNotFound,               "UserNotFound" },
        { Category::AccessDenied,               "AccessDenied" },
        { Category::AccessExpired,              "AccessExpired" },
        { Category::InvalidEmail,               "InvalidEmail" },
        { Category::UnknownTenant,              "UnknownTenant" },
        { Category::NotOwner,                   "NotOwner" },
        { Category::NotPremiumLicenseUser,      "NotPremiumLicenseUser" },
        { Category::ClientVersionNotSupported,  "ClientVersionNotSupported" }
    };
    return kCategories.count(category) ? kCategories.at(category) : kUnrecognized;
  }
};

/**
 * @brief The user could not get access to the content due to extended Access checks like ABAC.
 */
class NoPermissionsExtendedError : public NoPermissionsError {
public:
  /** @cond DOXYGEN_HIDE */
  explicit NoPermissionsExtendedError(Category category,
      const std::string& message,
      const std::string& referrer,
      const std::string& owner,
      const std::vector<ExtendedErrorInfo>& extendedErrorInfo,
      const std::string& name = GetNoPermissionsExtendedErrorName())
      : NoPermissionsError(category, message, referrer, owner, name),
        mExtendedErrorInfo(extendedErrorInfo) { AddExtendedErrorInfoToDebugInfo(extendedErrorInfo); }

  explicit NoPermissionsExtendedError(Category category,
      const std::string& message,
      const std::string& referrer,
      const std::string& owner,
      const std::map<std::string, std::string> debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::vector<ExtendedErrorInfo>& extendedErrorInfo,
      const std::string& name = GetNoPermissionsExtendedErrorName())
      : NoPermissionsError(category, message, referrer, owner, debugInfo, sensitiveDebugInfo, name),
        mExtendedErrorInfo(extendedErrorInfo) { AddExtendedErrorInfoToDebugInfo(extendedErrorInfo); }
  
  std::shared_ptr<Error> Clone() const override { return std::make_shared<NoPermissionsExtendedError>(*this); }

  std::vector<ExtendedErrorInfo> GetExtendedErrorInfo() const { 
    return mExtendedErrorInfo;
  }
  /** @endcond */

private:
  void AddExtendedErrorInfoToDebugInfo(const std::vector<ExtendedErrorInfo>& extendedErrorInfo) {
    std::stringstream codesStream;
    std::stringstream messagesStream;
    std::stringstream detailsStream;

    static const std::string seperator = ",";
    static const std::string objSeperator = ";";
    static const std::string mapElementSeperator = "|";
    for (size_t i = 0; i < extendedErrorInfo.size(); i++) {
      codesStream << extendedErrorInfo[i].code;
      messagesStream << extendedErrorInfo[i].message;
      std::map<std::string, std::string>::const_iterator detailsIter = extendedErrorInfo[i].details.begin();
      while (detailsIter != extendedErrorInfo[i].details.end()) {
        detailsStream << detailsIter->first << seperator << detailsIter->second;
        detailsIter++;
        if (detailsIter != extendedErrorInfo[i].details.end()) {
          detailsStream << mapElementSeperator;
        }
      }

      if (i + 1 < extendedErrorInfo.size()) {
        codesStream << objSeperator;
        messagesStream << objSeperator;
        detailsStream << objSeperator;
      }
    }

    if (!extendedErrorInfo.empty()) {
      AddDebugInfo(GetErrorInfoCodesKey(), codesStream.str());
      AddDebugInfo(GetErrorInfoMessagesKey(), messagesStream.str());
      AddDebugInfo(GetErrorInfoDetailsKey(), detailsStream.str());
    }
  }

  std::vector<ExtendedErrorInfo> mExtendedErrorInfo;
};

/**
 * @brief The user could not get access to the content due to missing authentication token
 */
class NoAuthTokenError : public AccessDeniedError {
public:
  /** @cond DOXYGEN_HIDE */
  explicit NoAuthTokenError(
      const std::string& message,
      const std::string& name = "NoAuthTokenError")
      : NoAuthTokenError(message, {{}}, {{}}, name) {}
  explicit NoAuthTokenError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "NoAuthTokenError")
      : AccessDeniedError(message, debugInfo, sensitiveDebugInfo, name) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<NoAuthTokenError>(*this); }
  ErrorType GetErrorType() const override { return ErrorType::NO_AUTH_TOKEN; }
  /** @endcond */
};

/**
 * @brief The user could not get access to the content due to a service being disabled
 */
class ServiceDisabledError : public AccessDeniedError {
public:
  /**
   * @brief Describes the extent for which the service is disabled
   */
  enum class Extent {
    User, /**< Service is disabled for the user. */
    Device, /**< Service is disabled for the device. */
    Platform, /**< Service is disabled for the platform. */
    Tenant, /**< Service is disabled for the tenant. */
  };

  /** @cond DOXYGEN_HIDE */
  explicit ServiceDisabledError(
      Extent extent,
      const std::string& requestId,
      const std::string& message,
      const std::string& name = "ServiceDisabledError")
      : ServiceDisabledError(extent, message, {{}}, {{}}, name) {
    if (!requestId.empty())
      AddDebugInfo("HttpRequest.Id", requestId);
  }
  explicit ServiceDisabledError(
      Extent extent,
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "ServiceDisabledError")
      : AccessDeniedError(message, debugInfo, sensitiveDebugInfo, name),
        mExtent(extent) {
    AddDebugInfo("ServiceDisabledError.Extent", GetExtentString(extent));
  }

  std::shared_ptr<Error> Clone() const override { return std::make_shared<ServiceDisabledError>(*this); }
  ErrorType GetErrorType() const override { return ErrorType::DISABLED_SERVICE; }
  /** @endcond */

  /**
   * @brief Gets the extent for which the service is disabled
   * 
   * @return Extent for which the service is disabled
   */
  Extent GetExtent() const { return mExtent; }

private:
  const std::string& GetExtentString(Extent extent) const {
    static const std::string kUnrecognized = "UNRECOGNIZED";
    static const std::map<Extent, std::string> kExtents = {
        { Extent::User,     "User" },
        { Extent::Device,   "Device" },
        { Extent::Platform, "Platform" },
        { Extent::Tenant,   "Tenant" },
    };
    return kExtents.count(extent) ? kExtents.at(extent) : kUnrecognized;
  }

  Extent mExtent;
};

/**
 * @brief An operation that required consent from user was not granted consent.
 */
class ConsentDeniedError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit ConsentDeniedError(
      const std::string& message,
      const std::string& name = "ConsentDeniedError")
      : ConsentDeniedError(message, {{}}, {{}}, name) {}
  explicit ConsentDeniedError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "ConsentDeniedError")
      : Error(message, debugInfo, sensitiveDebugInfo, name, ErrorType::CONSENT_DENIED) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<ConsentDeniedError>(*this); }
  /** @endcond */ 
};

/**
 * @brief Tenant policy is not configured for classification/labels.
 */
class NoPolicyError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  
  /**
   * @brief Describes what is the failure point while parsing.
   */
  enum class Category {
    SyncFile, /**< Policy is missing syncfile node, override label node  */
    Labels,   /**< Policy is missing labels node, override rules node */
    Rules     /**< Policy is missing rules node. */
  };
  
  explicit NoPolicyError(
      const std::string& message,
      const Category category,
      const std::string& name = "NoPolicyError")
    : NoPolicyError(message, category, {{}}, {{}}, name) {}
  explicit NoPolicyError(
      const std::string& message,
      const Category category,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "NoPolicyError")
    : Error(message, debugInfo, sensitiveDebugInfo, name, ErrorType::NO_POLICY),
      mCategory(category) {
    AddDebugInfo("NoPolicyError.Category", GetCategoryString(category));
  }
  
  std::shared_ptr<Error> Clone() const override { return std::make_shared<NoPolicyError>(*this); }
  Category GetCategory() const { return mCategory; }
private:
  const std::string& GetCategoryString(Category category) const {
    static const std::string kUnrecognized = "UNRECOGNIZED";
    static const std::map<Category, std::string> kCategories = {
        { Category::SyncFile,             "SyncFile" },
        { Category::Labels,               "Labels" },
        { Category::Rules,                "Rules" },
    };
    return kCategories.count(category) ? kCategories.at(category) : kUnrecognized;
  }

  Category mCategory;
  /** @endcond */
};

/**
 * @brief Operation was cancelled.
 */
class OperationCancelledError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit OperationCancelledError(
      const std::string& message,
      const std::string& name = "OperationCancelledError")
      : OperationCancelledError(message, {{}}, {{}}, name) {}
  explicit OperationCancelledError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "OperationCancelledError")
      : Error(message, debugInfo, sensitiveDebugInfo, name, ErrorType::OPERATION_CANCELLED) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<OperationCancelledError>(*this); }
  /** @endcond */ 
};

/**
 * @brief Adhoc protection should be set to complete the action on the file.
*/
class AdhocProtectionRequiredError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit AdhocProtectionRequiredError(
      const std::string& message,
      const std::string& name = "AdhocProtectionRequiredError")
      : AdhocProtectionRequiredError(message, {{}}, {{}}, name) {}
  explicit AdhocProtectionRequiredError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "AdhocProtectionRequiredError")
      : Error(message, debugInfo, sensitiveDebugInfo, name, ErrorType::ADHOC_PROTECTION_REQUIRED) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<AdhocProtectionRequiredError>(*this); }
  /** @endcond */
};

/**
 * @brief Caller invoked a deprecated API
 */
class DeprecatedApiError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit DeprecatedApiError(
      const std::string& message,
      const std::string& name = "DeprecatedApiError")
      : DeprecatedApiError(message, {{}}, {{}}, name) {}
  explicit DeprecatedApiError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "DeprecatedApiError")
      : Error(message, debugInfo, sensitiveDebugInfo, name, ErrorType::DEPRECATED_API) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<DeprecatedApiError>(*this); }
  /** @endcond */
};

/**
 * @brief Template ID is not recognized by RMS service
 */
class TemplateNotFoundError : public BadInputError {
public:
  /** @cond DOXYGEN_HIDE */
  explicit TemplateNotFoundError(
      const std::string& message,
      const std::string& name = "TemplateNotFoundError")
      : TemplateNotFoundError(message, {{}}, {{}}, name) {}
  explicit TemplateNotFoundError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "TemplateNotFoundError")
      : BadInputError(message, debugInfo, sensitiveDebugInfo, name) {}

  std::shared_ptr<Error> Clone() const override { return std::make_shared<TemplateNotFoundError>(*this); }
  ErrorType GetErrorType() const override { return ErrorType::TEMPLATE_NOT_FOUND; }
  /** @endcond */
};

/**
 * @brief Template ID is archived and unavailable for protection
 */
class TemplateArchivedError : public BadInputError {
public:
  /** @cond DOXYGEN_HIDE */
  explicit TemplateArchivedError(
      const std::string& message,
      const std::string& name = "TemplateArchivedError")
      : TemplateArchivedError(message, {{}}, {{}}, name) {}
  explicit TemplateArchivedError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "TemplateArchivedError")
      : BadInputError(message, debugInfo, sensitiveDebugInfo, name) {}

  std::shared_ptr<Error> Clone() const override { return std::make_shared<TemplateArchivedError>(*this); }
  ErrorType GetErrorType() const override { return ErrorType::TEMPLATE_ARCHIVED; }
  /** @endcond */
};

/**
 * @brief Content Format is not supported
 */
class ContentFormatNotSupportedError : public BadInputError {
public:
  /** @cond DOXYGEN_HIDE */
  explicit ContentFormatNotSupportedError(
      const std::string& message,
      const std::string& name = "ContentFormatNotSupportedError")
      : ContentFormatNotSupportedError(message, {{}}, {{}}, name) {}
  explicit ContentFormatNotSupportedError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "ContentFormatNotSupportedError")
      : BadInputError(message, debugInfo, sensitiveDebugInfo, name) {}

  std::shared_ptr<Error> Clone() const override { return std::make_shared<ContentFormatNotSupportedError>(*this); }
  ErrorType GetErrorType() const override { return ErrorType::CONTENT_FORMAT_NOT_SUPPORTED; }
  /** @endcond */
};

/**
 * @brief Label ID is not recognized
 */
class LabelNotFoundError : public BadInputError {
public:
  /** @cond DOXYGEN_HIDE */
  explicit LabelNotFoundError(
      const std::string& message,
      const std::string& name = "LabelNotFoundError")
      : LabelNotFoundError(message, {{}}, {{}}, name) {}
  explicit LabelNotFoundError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "LabelNotFoundError")
      : BadInputError(message, debugInfo, sensitiveDebugInfo, name) {}

  std::shared_ptr<Error> Clone() const override { return std::make_shared<LabelNotFoundError>(*this); }
  ErrorType GetErrorType() const override { return ErrorType::LABEL_NOT_FOUND; }
  /** @endcond */
};

/**
 * @brief License is not registered
 */
class LicenseNotRegisteredError : public BadInputError {
public:
  /** @cond DOXYGEN_HIDE */
  explicit LicenseNotRegisteredError(
      const std::string& message,
      const std::string& name = "LicenseNotRegisteredError")
      : LicenseNotRegisteredError(message, {{}}, {{}}, name) {}
  explicit LicenseNotRegisteredError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "LicenseNotRegisteredError")
      : BadInputError(message, debugInfo, sensitiveDebugInfo, name) {}

  std::shared_ptr<Error> Clone() const override { return std::make_shared<LicenseNotRegisteredError>(*this); }
  ErrorType GetErrorType() const override { return ErrorType::LICENSE_NOT_REGISTERED; }
  /** @endcond */
};

/**
 * @brief Label is disabled or inactive
 */
class LabelDisabledError : public BadInputError {
public:
  /** @cond DOXYGEN_HIDE */
  explicit LabelDisabledError(
      const std::string& message,
      const std::string& name = "LabelDisabledError")
      : LabelDisabledError(message, {{}}, {{}}, name) {}
  explicit LabelDisabledError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string> sensitiveDebugInfo,
      const std::string& name = "LabelDisabledError")
      : BadInputError(message, debugInfo, sensitiveDebugInfo, name) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<LabelDisabledError>(*this); }
  ErrorType GetErrorType() const override { return ErrorType::LABEL_DISABLED; }
  /** @endcond */
};

/**
 * @brief Bring your own encryption key needed and unavailable
 */
class CustomerKeyUnavailableError : public AccessDeniedError {
public:
  /** @cond DOXYGEN_HIDE */
  explicit CustomerKeyUnavailableError(
      const std::string& message,
      const std::string& name = "CustomerKeyUnavailableError")
      : CustomerKeyUnavailableError(message, {{}}, {{}}, name) {}
  explicit CustomerKeyUnavailableError(
      const std::string& message,
      const std::map<std::string, std::string>& debugInfo,
      const std::map<std::string, std::string>& sensitiveDebugInfo,
      const std::string& name = "CustomerKeyUnavailableError")
      : AccessDeniedError(message, debugInfo, sensitiveDebugInfo, name) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<CustomerKeyUnavailableError>(*this); }
  ErrorType GetErrorType() const override { return ErrorType::CUSTOMER_KEY_UNAVAILABLE; }
  /** @endcond */
};

MIP_NAMESPACE_END

#endif  // API_MIP_ERROR_H_
