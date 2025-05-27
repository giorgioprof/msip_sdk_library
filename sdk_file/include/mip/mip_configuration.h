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
 * @brief Defines configuration for all external mip delegates
 * 
 * @file mip_configuration.h
 */

#ifndef API_MIP_MIP_CONFIGURATION_H_
#define API_MIP_MIP_CONFIGURATION_H_

#include <map>
#include <memory>

#include "mip/diagnostic_configuration.h"
#include "mip/flighting_feature.h"
#include "mip/json_delegate.h"
#include "mip/logger_delegate.h"
#include "mip/storage_delegate.h"
#include "mip/task_dispatcher_delegate.h"
#include "mip/xml_delegate.h"

MIP_NAMESPACE_BEGIN

  /**
   * @brief Configuration used by MIP sdk during its creation and throughout its lifetime
   */

class MipConfiguration {
public:
  MipConfiguration(
      const ApplicationInfo& appInfo,
      const std::string& path,
      LogLevel thresholdLogLevel,
      bool isOfflineOnly)
      : mAppInfo(appInfo),
        mPath(path),
        mThresholdLogLevel(thresholdLogLevel),
        mIsOfflineOnly(isOfflineOnly),
        mfeatureSettings(),
        mApiLogCacheMaxMessages(1024) {}

  /**
   * @brief Get the Description of host application
   * 
   * @return Description of host application
   */
  const ApplicationInfo&  GetApplicationInfo() const { return mAppInfo; }

  /**
   * @brief Get the File path for logs, caches, etc.
   * 
   * @return File path for logs, caches, etc.
   */
  const std::string& GetPath() const { return mPath; }

  /**
   * @brief Get the Minimum log level for .miplog
   * 
   * @return Minimum log level for .miplog.
   */
  const LogLevel& GetThresholdLogLevel() const { return mThresholdLogLevel; }

  /**
   * @brief whether Network operations is enabled or disabled(not all actions supported when offline)
   * 
   * @return Network operations state
   */
  const bool isOfflineOnly() const { return mIsOfflineOnly; }
  /**
   * @brief Get the Diagnostic (if any) provided by the application
   * 
   * @return Diagnostic Configuration to be used for configuring telemetry/audit.
   */
  std::shared_ptr<DiagnosticConfiguration> GetDiagnosticConfiguration() const { return mDiagnosticConfiguration; }

  /**
   * @brief Override default configuration of diagnostic
   * 
   * @param diagnosticConfiguration diagnostic configuration to be used for configuring telemetry/audit.
   */
  void SetDiagnosticConfiguration(const std::shared_ptr<DiagnosticConfiguration>& diagnosticConfiguration) { mDiagnosticConfiguration = diagnosticConfiguration; }

  /**
   * @brief Get the LoggerDelegate (if any) override implementation
   * 
   * @return LoggerDelegate (if any) override implementation.
   */
  std::shared_ptr<LoggerDelegate> GetLoggerDelegate() const { return mLoggerDelegate; }

  /**
   * @brief Set the LoggerDelegate (if any) override implementation
   * 
   * @param loggerDelegate LoggerDelegate override implementation
   */
  void SetLoggerDelegate(const std::shared_ptr<LoggerDelegate>& loggerDelegate) { mLoggerDelegate = loggerDelegate; }

  /**
   * @brief Get the override configuration (if any) for the default logger implementation
   * 
   * @return LoggerConfiguration override (if any)
   */
  std::shared_ptr<LoggerConfiguration>  GetLoggerConfiguration() const { return mLoggerConfiguration; }

  /**
   * @brief Override the default logger configuration
   * 
   * @param loggerConfiguration LoggerConfiguration override
   */
  void SetLoggerConfiguration(const std::shared_ptr<LoggerConfiguration>& loggerConfiguration) { mLoggerConfiguration = loggerConfiguration; }

  /**
   * @brief Get the StorageDelegate (if any) override implementation
   * 
   * @return StorageDelegate (if any) override implementation.
   */
  std::shared_ptr<StorageDelegate> GetStorageDelegate() const { return mStorageDelegate; }

  /**
   * @brief Set the StorageDelegate (if any) override implementation. It's a required delegate for MIP Core Context.
   * 
   * @param storageDelegate StorageDelegate override implementation
   */
  void SetStorageDelegate(const std::shared_ptr<StorageDelegate>& storageDelegate) { mStorageDelegate = storageDelegate; }
  
  /**
   * @brief Get the HttpDelegate (if any) override implementation.
   * 
   * @return HttpDelegate (if any) override implementation.
   */
  std::shared_ptr<HttpDelegate> GetHttpDelegate() const { return mHttpDelegate; }

  /**
   * @brief Set the HttpDelegate (if any) override implementation. It's a required delegate for MIP Core Context.
   * 
   * @param httpDelegate HttpDelegate override implementation
   */
  void SetHttpDelegate(const std::shared_ptr<HttpDelegate>& httpDelegate) { mHttpDelegate = httpDelegate; }
  
  /**
   * @brief Get the JsonDelegate (if any) override implementation
   *
   * @return JsonDelegate (if any) override implementation.
   */
  std::shared_ptr<JsonDelegate> GetJsonDelegate() const {
    return mJsonDelegate;
  }

  /**
   * @brief Get the XmlDelegate (if any) override implementation. 
   * MipConfiguration needs to be derived from to override the internal xmlDelegate with an alternative.
   * This delegate is only configurable when using the MIP Core Context and only for the protection and upe sdk.
   * 
   * @return XmlDelegate (if any) override implementation. nullptr by default.
   */
  std::shared_ptr<xml::XmlDelegate> GetXmlDelegate() const { return mXmlDelegate; }  

  /**
   * @brief Get the Flighting features which should be set to non-default values
   * 
   * @return Flighting features which should be set to non-default values
   */
  std::map<FlightingFeature, bool> GetFeatureSettings() const { return mfeatureSettings; }

  /**
   * @brief Set the Flighting features which should be set to non-default values
   * 
   * @param featureSettings Flighting features to be used.
   */
  void SetFeatureSettings(const std::map<FlightingFeature, bool>& featureSettings) { mfeatureSettings = featureSettings; }

  /**
   * @brief Get the max messages per Api call that will be logged in the api log cache
   * 
   * @return The max messages
   */
  size_t GetApiLogCacheMaxMessages() const { return mApiLogCacheMaxMessages; }

  /**
   * @brief Set the max messages per Api call that will be logged in the api log cache.  Setting this to 0 will disable api log caching
   * 
   * @param maxMessages
   */
  void SetApiLogCacheMaxMessages(size_t maxMessages) { mApiLogCacheMaxMessages = maxMessages; }

  ~MipConfiguration() { }

protected:
  std::shared_ptr<JsonDelegate> mJsonDelegate;
  std::shared_ptr<xml::XmlDelegate> mXmlDelegate;

private:
  ApplicationInfo mAppInfo;
  std::string mPath;
  LogLevel mThresholdLogLevel;
  bool mIsOfflineOnly;
  std::shared_ptr<LoggerDelegate> mLoggerDelegate;
  std::shared_ptr<DiagnosticConfiguration> mDiagnosticConfiguration;
  std::shared_ptr<StorageDelegate> mStorageDelegate;
  std::map<FlightingFeature, bool> mfeatureSettings;
  size_t mApiLogCacheMaxMessages;
  std::shared_ptr<HttpDelegate> mHttpDelegate;
  std::shared_ptr<LoggerConfiguration> mLoggerConfiguration;
/** @endcond */
  };

MIP_NAMESPACE_END
#endif  // API_MIP_MIP_CONFIGURATION_H_