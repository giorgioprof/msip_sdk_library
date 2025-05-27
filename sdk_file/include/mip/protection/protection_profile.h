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
 * @brief Defines ProtectionProfile interface
 * 
 * @file protection_profile.h
 */

#ifndef API_MIP_PROTECTION_PROTECTION_PROFILE_H_
#define API_MIP_PROTECTION_PROTECTION_PROFILE_H_

#include <memory>
#include <string>

#include "mip/common_types.h"
#include "mip/dns_redirection.h"
#include "mip/error.h"
#include "mip/http_delegate.h"
#include "mip/logger_delegate.h"
#include "mip/mip_context.h"
#include "mip/mip_export.h"
#include "mip/mip_namespace.h"
#include "mip/protection/protection_common_types.h"
#include "mip/protection/protection_engine.h"
#include "mip/task_dispatcher_delegate.h"
#include "mip/xml_delegate.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief ProtectionProfile is the root class for performing protection operations
 * 
 * @note An application needs to create a ProtectionProfile before performing any protection operations
 */
class ProtectionProfile {
public:
  /**
   * @brief Interface that receives notifications related to ProtectionProfile
   * 
   * @note This interface must be implemented by applications using the protection SDK
   */
  class Observer {
  public:
    /** @cond DOXYGEN_HIDE */
    virtual ~Observer() { }
    /** @endcond */

    /**
     * @brief Called when profile was loaded successfully
     * 
     * @param profile A reference to the newly created ProtectionProfile
     * @param context The same context that was passed to ProtectionProfile::LoadAsync
     * 
     * @note An application can pass any type of context (for example, std::promise, std::function) to
     * ProtectionProfile::LoadAsync and that same context will be forwarded as-is to
     * ProtectionProfile::Observer::OnLoadSuccess or ProtectionProfile::Observer::OnLoadFailure
     */
    virtual void OnLoadSuccess(
        const std::shared_ptr<ProtectionProfile>& profile,
        const std::shared_ptr<void>& context) { UNUSED(profile); UNUSED(context); }

    /**
     * @brief Called when loading a profile caused an error
     * 
     * @param error Error that occurred while loading
     * @param context The same context that was passed to ProtectionProfile::LoadAsync
     * 
     * @note An application can pass any type of context (for example, std::promise, std::function) to
     * ProtectionProfile::LoadAsync and that same context will be forwarded as-is to
     * ProtectionProfile::Observer::OnLoadSuccess or ProtectionProfile::Observer::OnLoadFailure
     */
    virtual void OnLoadFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when list of engines was generated successfully.
     * 
     * @param engineIds a list of engine IDs the are available.
     * @param context The same context that was passed to ProtectionProfile::ListEnginesAsync
     */
    virtual void OnListEnginesSuccess(
        const std::vector<std::string>& engineIds,
        const std::shared_ptr<void>& context) { UNUSED(engineIds); UNUSED(context); }

    /**
     * @brief Called when listing engines resulted in an error.
     * 
     * @param error the error that caused the list engines operation to fail.
     * @param context The same context that was passed to ProtectionProfile::ListEnginesAsync
     */
    virtual void OnListEnginesFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when a new engine was added successfully.
     * 
     * @param engine Newly created engine
     * @param context The same context that was passed to ProtectionProfile::AddEngineAsync
     */
    virtual void OnAddEngineSuccess(
        const std::shared_ptr<ProtectionEngine>& engine,
        const std::shared_ptr<void>& context) { UNUSED(engine); UNUSED(context); }

    /**
     * @brief Called when adding a new engine resulted in an error.
     * 
     * @param error the error that caused the add engine operation to fail.
     * @param context The same context that was passed to ProtectionProfile::AddEngineAsync
     */
    virtual void OnAddEngineFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when an engine was deleted successfully.
     * 
     * @param context The same context that was passed to ProtectionProfile::DeleteEngineAsync
     */
    virtual void OnDeleteEngineSuccess(
        const std::shared_ptr<void>& context) { UNUSED(context); }

    /**
     * @brief Called when deleting an engine resulted in an error.
     * 
     * @param error the error that caused the delete engine operation to fail.
     * @param context The same context that was passed to ProtectionProfile::DeleteEngineAsync
     */
    virtual void OnDeleteEngineFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /** @cond DOXYGEN_HIDE */
  protected:
    Observer() { }
    /** @endcond */
  };

  /**
   * @brief Settings used by ProtectionProfile during its creation and throughout its lifetime
   */
  class Settings {
  public:
    /**
     * @brief ProtectionProfile::Settings constructor that specifies an observer to be used for async operations
     * 
     * @param mipContext Global context settings
     * @param cacheStorageType Store any cached state in memory or on disk
     * @param consentDelegate Delegate used to obtain user permission to access external resources
     * @param observer Observer instance that will receive notifications of events related to ProtectionProfile
     * @param applicationInfo Info about application that is consuming the protection SDK
     */
    Settings(
        const std::shared_ptr<MipContext>& mipContext,
        CacheStorageType cacheStorageType,
        const std::shared_ptr<ConsentDelegate>& consentDelegate,
        const std::shared_ptr<ProtectionProfile::Observer>& observer)
        : mMipContext(mipContext),
          mCacheStorageType(cacheStorageType),
          mConsentDelegate(consentDelegate),
          mObserver(observer),
          mCanCacheLicenses(true),
#if defined(MIP_OFFLINE_PUBLISHING_ENABLED) || defined(SWIG) || defined(SWIG_DIRECTORS)
          mOfflinePublishing(false),
#endif // MIP_OFFLINE_PUBLISHING_ENABLED
          mDnsRedirection(DnsRedirection::MDEDiscovery) {
    }

    /**
     * @brief ProtectionProfile::Settings constructor, used for synchronous operations
     * 
     * @param mipContext Global context settings
     * @param cacheStorageType Store any cached state in memory or on disk
     * @param consentDelegate Delegate used to obtain user permission to access external resources
     * @param applicationInfo Info about application which is consuming the protection SDK
     */
    Settings(
        const std::shared_ptr<MipContext>& mipContext,
        CacheStorageType cacheStorageType,
        const std::shared_ptr<ConsentDelegate>& consentDelegate)
        : Settings(mipContext, cacheStorageType, consentDelegate, nullptr /*observer*/) {
    }

    /** @cond DOXYGEN_HIDE */
    ~Settings() { }
    /** @endcond */

    /**
     * @brief Get whether caches are stored in memory or on disk
     *
     * @return storage type used
     */
    CacheStorageType GetCacheStorageType() const { return mCacheStorageType; }

    /**
     * @brief Gets the consent delegate used for connecting to services
     * 
     * @return Consent delegate used for connecting to services
     */
    std::shared_ptr<ConsentDelegate> GetConsentDelegate() const { return mConsentDelegate; }

    /**
     * @brief Gets the observer that receives notifications of events related to ProtectionProfile
     * 
     * @return Observer that receives notifications of events related to ProtectionProfile
     */
    std::shared_ptr<ProtectionProfile::Observer> GetObserver() const { return mObserver; }

    /**
     * @brief Get MIP context which represents shared state across all profiles
     * 
     * @return MIP context
     */
    std::shared_ptr<MipContext> GetMipContext() const { return mMipContext; }

    /**
     * @brief Get the HTTP delegate (if any) provided by the application
     * 
     * @return HTTP delegate to be used for HTTP operations
     */
    std::shared_ptr<HttpDelegate> GetHttpDelegate() const { return mHttpDelegate; }

    /**
     * @brief Override default HTTP stack with client's own
     * 
     * @param httpDelegate HTTP callback interface implemented by client application
     */
    void SetHttpDelegate(const std::shared_ptr<HttpDelegate>& httpDelegate) { mHttpDelegate = httpDelegate; }

    /**
     * @brief Get the #StorageDelegate (if any) provided by the application
     * 
     * @return #StorageDelegate to be used for caching
     */
    std::shared_ptr<StorageDelegate> GetStorageDelegate() const { return mStorageDelegate; }

    /**
     * @brief Override default storage cache with client's own implementation.
     * 
     * @param storageDelegate #StorageDelegate interface implemented by client application
     */
    void SetStorageDelegate(const std::shared_ptr<StorageDelegate>& storageDelegate) { mStorageDelegate = storageDelegate; }

    /**
     * @brief Get the TaskDispatcher delegate (if any) provided by the application
     * 
     * @return TaskDispatcher delegate to be used for executing asynchronous tasks
     */
    std::shared_ptr<TaskDispatcherDelegate> GetTaskDispatcherDelegate() const { return mTaskDispatcherDelegate; }

    /**
     * @brief Override default asynchonous task dispatching handling with client's own
     * 
     * @param taskDispatcherDelegate Task dispatching callback interface implemented by client application
     * 
     * @note tasks can reference profile objects preventing its destruction as a result 
     * taskdispatcher queues should not be shared.
     */
    void SetTaskDispatcherDelegate(const std::shared_ptr<TaskDispatcherDelegate>& taskDispatcherDelegate) {
      mTaskDispatcherDelegate = taskDispatcherDelegate;
    }

    /**
     * @brief Sets the session ID
     * 
     * @param sessionId Session ID that will be used to correlate logs/telemetry
     */
    void SetSessionId(const std::string& sessionId) { mSessionId = sessionId; }

    /**
     * @brief Gets the session ID
     * 
     * @return Session ID that will be used to correlate logs/telemetry
     */
    const std::string& GetSessionId() const { return mSessionId; }

    /**
    * @brief Configures whether or not end user licenses (EULs) will be cached locally
    * 
    * @param canCacheLicenses Whether or not engine should cache a license when opening protected content
    * 
    * @note If true, opening protected content will cache the associated license locally. If false, opening protected
    * content will always perform HTTP operation to acquire the license from the RMS service.
    */
    void SetCanCacheLicenses(bool canCacheLicenses) {
      mCanCacheLicenses = canCacheLicenses;
    }

    /**
    * @brief Gets whether or not end user licenses (EULs) are cached locally
    * 
    * @return License caching configuration
    */
    bool CanCacheLicenses() const {
      return mCanCacheLicenses;
    }

    /**
     * @brief Set the custom settings, used for feature gating and testing.
     * 
     * @param customSettings List of name/value pairs.
     */
    void SetCustomSettings(const std::vector<std::pair<std::string, std::string>>& customSettings) {
      mCustomSettings = customSettings;
    }
    
    /**
     * @brief Get the custom settings, used for feature gating and testing.
     * 
     * @return List of name/value pairs.
     */
    const std::vector<std::pair<std::string, std::string>>& GetCustomSettings() const {
      return mCustomSettings;
    }

#if !defined(SWIG) && !defined(SWIG_DIRECTORS)
    /**
     * @brief Get logger context that will be opaquely passed to the logger delegate for logs associated with the created profile
     * 
     * @return The logger context
     */
    const std::shared_ptr<void>& GetLoggerContext() const { return mLoggerContext; }
#endif

    /**
     * @brief Sets the logger context that will be opaquely passed to the logger delegate for logs associated with the created profile
     * 
     * @param loggerContext The logger context
     * 
     */
    void SetLoggerContext(const std::shared_ptr<void>& loggerContext) {
      mLoggerContext = loggerContext;
    }
    
    /**
     * @brief Adds a redirect uri
     * 
     * @param originalUri The uri that should be redirected from
     * @param redirectUri The uri that will replace the originalUri
     * 
     * @note Use this function to provide a domain that should be redirected to another Uri
     *        The originalUri will be normalized before comparison with the domain for the current request.
     *        If the domain of the current request matches that of the originalUri then the redirectUri will be used instead
     *        This redirection will occur before DNS lookup redirection
     */
    void AddRedirectionUri(const std::string& originalUri, const std::string& redirectUri) {
      mRedirectionUris[originalUri] = redirectUri;
    }

    /**
     * @brief Gets the redirection uris
     * 
     * @return Map of redirection uris
     */
    const std::map<std::string, std::string>& GetRedirectionUris() const {
      return mRedirectionUris;
    }
#if defined(MIP_OFFLINE_PUBLISHING_ENABLED) || defined(SWIG) || defined(SWIG_DIRECTORS)
    /**
     * @brief Gets offline publishing status
     * 
     * @return A boolean value indicating whether or not offline publishing is enabled
     */
    bool GetOfflinePublishing() const {
      return mOfflinePublishing;
    }

    /**
     * @brief Enables or disables offline publishing
     * 
     * @param offlinePublishing A boolean value indicating whether or not offline publishing is enabled
     * 
     */
    void SetOfflinePublishing(bool offlinePublishing) {
      mOfflinePublishing = offlinePublishing;
    }
#endif // MIP_OFFLINE_PUBLISHING_ENABLED

    /**
     * @brief Gets the dns redirect mode
     * 
     * @return The redirect mode used
     */
    DnsRedirection GetDnsRedirection() const {
      return mDnsRedirection;
    }

    /**
     * @brief Sets the dns redirection mode that controls how redirection is handled during online operations
     * 
     * @param dnsRedirection The redirection mode to use
     */
    void SetDnsRedirection(DnsRedirection dnsRedirection) {
      mDnsRedirection = dnsRedirection;
    }

  private:
    std::shared_ptr<MipContext> mMipContext;
    CacheStorageType mCacheStorageType;
    std::shared_ptr<ConsentDelegate> mConsentDelegate;
    std::shared_ptr<ProtectionProfile::Observer> mObserver;
    std::shared_ptr<HttpDelegate> mHttpDelegate;
    std::shared_ptr<TaskDispatcherDelegate> mTaskDispatcherDelegate;
    std::string mSessionId;
    bool mCanCacheLicenses = true;
    std::vector<std::pair<std::string, std::string>> mCustomSettings;
    std::shared_ptr<void> mLoggerContext;
    std::map<std::string, std::string> mRedirectionUris;
    std::shared_ptr<StorageDelegate> mStorageDelegate;
#if defined(MIP_OFFLINE_PUBLISHING_ENABLED) || defined(SWIG) || defined(SWIG_DIRECTORS)
    bool mOfflinePublishing;
#endif // MIP_OFFLINE_PUBLISHING_ENABLED
    DnsRedirection mDnsRedirection;
  };

  /**
   * @brief Starts loading a profile based on the provided settings.
   * 
   * @param settings Settings used by ProtectionProfile during its initialization and throughout its lifetime
   * @param context This same context will be forwarded to ProtectionProfile::Observer::OnLoadSuccess or
   * ProtectionProfile::Observer::OnLoadFailure as-is.
   * 
   * @return Async control object.
   * 
   * @note ProtectionProfile::Observer will be called upon success or failure.
   */
  MIP_API static std::shared_ptr<AsyncControl> __CDECL LoadAsync(
      const Settings& settings,
      const std::shared_ptr<void>& context);

  /**
   * @brief Loading a profile based on the provided settings.
   * 
   * @param settings Settings used by ProtectionProfile during its initialization and throughout its lifetime
   * 
   * @return Newly created profile
   */
  MIP_API static std::shared_ptr<ProtectionProfile> __CDECL Load(const Settings& settings);

  /**
   * @brief Gets library version
   *
   * @return Library version
   */
  MIP_API static const char* __CDECL GetVersion();

  /**
   * @brief Gets settings used by ProtectionProfile during its initialization and throughout its lifetime
   * 
   * @return Settings used by ProtectionProfile during its initialization and throughout its lifetime
   */
  virtual const Settings& GetSettings() const = 0;

  /**
   * @brief Starts list engines operation.
   *
   * @param context Client context that will be opaquely passed back to observers
   * 
   * @return Async control object.
   * 
   * @note ProtectionProfile::Observer will be called upon success or failure.
   */
  virtual std::shared_ptr<AsyncControl> ListEnginesAsync(const std::shared_ptr<void>& context) = 0;

  /**
   * @brief List engines.
   *
   * @return Cached engine IDs
   */
  virtual std::vector<std::string> ListEngines() = 0;

  /**
   * @brief Starts adding a new protection engine to the profile.
   * 
   * @param settings the mip::ProtectionEngine::Settings object that specifies the engine's settings.
   * @param context Client context that will be opaquely passed back to observers
   * 
   * @return Async control object.
   * 
   * @note ProtectionProfile::Observer will be called upon success or failure.
   */
  virtual std::shared_ptr<AsyncControl> AddEngineAsync(
      const ProtectionEngine::Settings& settings,
      const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Add a new protection engine to the profile.
   *
   * @param settings the mip::ProtectionEngine::Settings object that specifies the engine's settings.
   *
   * @return Newly created ProtectionEngine
   */
  virtual std::shared_ptr<ProtectionEngine> AddEngine(const ProtectionEngine::Settings& settings) = 0;

  /**
   * @brief Starts deleting the protection engine with the given ID. All data for the given engine will be deleted.
   * 
   * @param id the unique engine ID.
   * @param context Client context that will be opaquely passed back to observers
   * 
   * @return Async control object.
   * 
   * @note ProtectionProfile::Observer will be called upon success or failure.
   */
  virtual std::shared_ptr<AsyncControl> DeleteEngineAsync(
      const std::string& engineId,
      const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Delete the protection engine with the given ID. All data for the given engine will be deleted.
   *
   * @param id the unique engine ID.
   */
  virtual void DeleteEngine(const std::string& engineId) = 0;

#ifndef MIP_CORE_PACKAGE
  /**
   * @brief Creates a holder for details of a Publishing License and can be used to create a Protection Handler
   *
   * @param serializedPublishingLicense the serialized publishing license.
   *
   * @return a holder for details of a Publishing License
   *
   * @note The returned PublishingLicenseInfo is parsed
   */
#if !defined(SWIG) && !defined(SWIG_DIRECTORS)
  [[deprecated("ProtectionProfile::GetPublishingLicenseInfo(const std::vector<uint8_t>&) is deprecated, use ProtectionProfile::GetPublishingLicenseInfo(const std::vector<uint8_t>&, const std::shared_ptr<MipContext>&)")]]
#endif
  MIP_API static std::shared_ptr<PublishingLicenseInfo> __CDECL GetPublishingLicenseInfo(
      const std::vector<uint8_t>& serializedPublishingLicense);
#endif // !MIP_CORE_PACKAGE

  /**
   * @brief Creates a holder for details of a Publishing License and can be used to create a Protection Handler. Allows delegate overrides.
   *
   * @param serializedPublishingLicense the serialized publishing license.
   * @param mipContext a MIP context containing delegate overrides.
   *
   * @return a holder for details of a Publishing License
   * 
   * @note The returned PublishingLicenseInfo is parsed
   */
  MIP_API static std::shared_ptr<PublishingLicenseInfo> __CDECL GetPublishingLicenseInfo(
      const std::vector<uint8_t>& serializedPublishingLicense,
      const std::shared_ptr<MipContext>& mipContext);

  /** @cond DOXYGEN_HIDE */
  virtual ~ProtectionProfile() { }
  /** @endcond */
  };

MIP_NAMESPACE_END

#endif // API_MIP_PROTECTION_PROTECTION_PROFILE_H_