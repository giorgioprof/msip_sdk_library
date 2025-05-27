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
 * @brief File containing definition of MipContext
 * 
 * @file mip_context.h
 */

#ifndef API_MIP_MIP_CONTEXT_H_
#define API_MIP_MIP_CONTEXT_H_

#include <map>
#include <memory>

#include "mip/common_types.h"
#include "mip/diagnostic_configuration.h"
#include "mip/flighting_feature.h"
#include "mip/json_delegate.h"
#include "mip/logger_delegate.h"
#include "mip/mip_configuration.h"
#include "mip/mip_export.h"
#include "mip/mip_namespace.h"
#include "mip/telemetry_delegate.h"
#include "mip/xml_delegate.h"

MIP_NAMESPACE_BEGIN

/** @cond DOXYGEN_HIDE */
class AuditManager;
class ProtectionCloudManager;
class PolicyCloudManager;
class OneDSCloudManager;
class TelemetryManager;
struct DiagnosticClientHandle {
  DiagnosticClientHandle() {}
  DiagnosticClientHandle(const char *diagnosticType, int64_t diagnosticHandle)
    : type(diagnosticType),
      handle(diagnosticHandle) {}

  const char *type = {};
  int64_t handle = {};
};
/** @endcond */

/**
 * @brief MipContext represents state that is shared across all profiles, engines, handlers
 */
class MipContext {
public:

#ifndef MIP_CORE_PACKAGE
  /**
   * @brief Create a new MipContext instance to be used when initializing profiles
   * 
   * @param appInfo Description of host application
   * @param path File path for logs, caches, etc.
   * @param thresholdLogLevel Minimum log level for .miplog
   * @param isOfflineOnly Enable/disable network operations (not all actions supported when offline)
   * @param loggerDelegateOverride (Optional) LoggerDelegate override implementation
   * @param diagnosticOverride (Optional) DiagnosticConfiguration override for audit/telemetry
  * 
   * @return MipContext instance
   */
#if !defined(SWIG) && !defined(SWIG_DIRECTORS)
    [[deprecated("MipContext::Create(...) is deprecated, use MipContext::Create(const std::shared_ptr<const MipConfiguration>&)")]]
#endif
   MIP_API static std::shared_ptr<MipContext> __CDECL Create(
      const ApplicationInfo& appInfo,
      const std::string& path,
      LogLevel thresholdLogLevel,
      bool isOfflineOnly,
      const std::shared_ptr<LoggerDelegate>& loggerDelegateOverride,
      const std::shared_ptr<DiagnosticConfiguration>& diagnosticOverride);

  /**
   * @brief Create a new MipContext instance with custom feature settings
   * 
   * @param appInfo Description of host application
   * @param path File path for logs, caches, etc.
   * @param thresholdLogLevel Minimum log level for .miplog
   * @param isOfflineOnly Enable/disable network operations (not all actions supported when offline)
   * @param loggerDelegateOverride (Optional) LoggerDelegate override implementation
   * @param diagnosticOverride (Optional) DiagnosticConfiguration override for audit/telemetry
   * @param featureSettingsOverride Flighting features which should be set to non-default values
   * 
   * @return MipContext instance
   */
#if !defined(SWIG) && !defined(SWIG_DIRECTORS)
    [[deprecated("MipContext::CreateWithCustomFeatureSettings is deprecated, use MipContext::Create(const std::shared_ptr<const MipConfiguration>&)")]]
#endif
  MIP_API static std::shared_ptr<MipContext> __CDECL CreateWithCustomFeatureSettings(
      const ApplicationInfo& appInfo,
      const std::string& path,
      LogLevel thresholdLogLevel,
      bool isOfflineOnly,
      const std::shared_ptr<LoggerDelegate>& loggerDelegateOverride,
      const std::shared_ptr<DiagnosticConfiguration>& diagnosticOverride,
      const std::map<FlightingFeature, bool>& featureSettingsOverride);
#endif // !MIP_CORE_PACKAGE

  /**
   * @brief Create a new MipContext instance with mip configuration
   * 
   * @param mipConfiguration mipConfiguration to be used across MIP
   * 
   * @return MipContext instance
   */
  MIP_API static std::shared_ptr<MipContext> __CDECL Create(
      const std::shared_ptr<const MipConfiguration>& configuration);

  /**
   * @brief Terminate MIP
   * 
   * @note This method must be called prior to process/DLL shutdown
   */
  virtual void ShutDown() = 0;

  /**
   * @brief Gets whether or not a feature is enabled
   * 
   * @param feature Feature to enable/disable
   * 
   * @return Whether or not a feature is enabled
   * 
   * @note If a FeatureFlightingDelegate has not been provided by an application, this will always return true
   */
  virtual bool IsFeatureEnabled(FlightingFeature feature) const = 0;

  /**
   * @brief Get application description
   * 
   * @return Application description
   */
  virtual const ApplicationInfo& GetApplicationInfo() const = 0;

  /**
   * @brief Get file path for logs, caches, etc.
   * 
   * @return File path (with "mip" leaf directory)
   */
  virtual const std::string& GetMipPath() const = 0;

  /**
   * @brief Get offline-only setting
   * 
   * @return Whether or not application is running in offline-only mode
   */
  virtual bool IsOfflineOnly() const = 0;
  
  /**
   * @brief Get threshold log level
   * 
   * @return threshold log level
   */
  virtual LogLevel GetThresholdLogLevel() const = 0;

  /**
   * @brief Get logger implementation
   * 
   * @return Logger
   */
  virtual std::shared_ptr<LoggerDelegate> GetLoggerDelegate() const = 0;

  /**
   * @brief Get Http implementation
   * 
   * @return HttpDelegate
   */
  virtual std::shared_ptr<HttpDelegate> GetHttpDelegate() const = 0;

  /**
   * @brief Get raw logger implementation
   * 
   * @return Logger
   */
  virtual LoggerDelegate* GetRawLoggerDelegate() const = 0;

   /**
   * @brief Get Storage Delegate implementation if provided
   * 
   * @return StorageDelegate
   */
  virtual std::shared_ptr<StorageDelegate> GetStorageDelegate() const = 0;

  /**
   * @brief Get flighting feature set.
   * 
   * @return Flighting feature map
   */
  virtual const std::map<FlightingFeature, bool>& GetFlightingFeatures() const = 0;

  /**
   * @brief Gets the mip configuration that was used to create the MipContext
   * 
   * @return The mip configuration
   */
  virtual const std::shared_ptr<const MipConfiguration>& GetMipConfiguration() const = 0;

    /**
   * @brief Gets the logger configuration that will be used by the default logger
   * 
   * @return Logger configuration
   */
  virtual const LoggerConfiguration& GetLoggerConfiguration() const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual const ProtectionCloudManager& GetProtectionCloudManager() const = 0;
  virtual const PolicyCloudManager& GetPolicyCloudManager() const = 0;
  virtual const OneDSCloudManager& GetOneDSCloudManager() const = 0;
  virtual std::shared_ptr<TelemetryManager> GetTelemetryManager() const = 0;
  virtual std::shared_ptr<AuditManager> GetAuditManager() const = 0;
  virtual TelemetryManager* GetRawTelemetryManager() const = 0;
  virtual std::shared_ptr<JsonDelegate> GetJsonDelegate() const = 0;
  virtual DiagnosticClientHandle AddDiagnosticClient(Cloud cloud, DataBoundary dataBoundary) const = 0;
  virtual const std::shared_ptr<EventContext> GetDefaultEventContext() const = 0;

  /**
   * @brief Get the Xml Delegate that should be used when parsing. 
   * If the application provides a MipConfiguration with an xml delegate and is using MipCore 
   * this method returns that alternative delegate.
   * Otherwise the default implementation will be used.
   * 
   * @return A shared_ptr to the xml delegate
   */
  virtual std::shared_ptr<xml::XmlDelegate> GetXmlDelegate() const = 0;
  virtual ~MipContext() {}
  /** @endcond */
};

MIP_NAMESPACE_END
#endif  // API_MIP_MIP_CONTEXT_H_

