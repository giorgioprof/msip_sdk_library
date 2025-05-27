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

#ifndef API_MIP_FILE_FILE_PROFILE_H_
#define API_MIP_FILE_FILE_PROFILE_H_

#include <memory>
#include <string>
#include <vector>

#include "mip/common_types.h"
#include "mip/dns_redirection.h"
#include "mip/error.h"
#include "mip/file/file_engine.h"
#include "mip/file/file_export.h"
#include "mip/http_delegate.h"
#include "mip/logger_delegate.h"
#include "mip/mip_context.h"
#include "mip/mip_namespace.h"
#include "mip/task_dispatcher_delegate.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief FileProfile class is the root class for using the Microsoft Information Protection operations.
 * 
 * A typical application will only need one Profile.
 */
class FileProfile {
public:

  /**
   * @brief Observer interface for clients to get notifications for profile related events.
   * 
   * @note All errors inherit from mip::Error.
   * @note Client should not call the engine back on the thread that calls the observer.
   */
  class Observer {
  public:
    virtual ~Observer() {}

    /**
     * @brief Called when profile was loaded successfully.
     */
    virtual void OnLoadSuccess(
        const std::shared_ptr<mip::FileProfile>& profile, 
        const std::shared_ptr<void>& context) { UNUSED(profile); UNUSED(context); }

    /**
     * @brief Called when loading a profile caused an error.
     */
    virtual void OnLoadFailure(const std::exception_ptr& error, const std::shared_ptr<void>& context) {
        UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when list of engines was generated successfully.
     */
    virtual void OnListEnginesSuccess(
        const std::vector<std::string>& engineIds, 
        const std::shared_ptr<void>& context) { UNUSED(engineIds); UNUSED(context); }

    /**
     * @brief Called when listing engines caused an error.
     */
    virtual void OnListEnginesFailure(
        const std::exception_ptr& error, 
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when an engine was unloaded successfully.
     */
    virtual void OnUnloadEngineSuccess(const std::shared_ptr<void>& context) { UNUSED(context); }

    /**
     * @brief Called when unloading an engine caused an error.
     */
    virtual void OnUnloadEngineFailure(
        const std::exception_ptr& error, 
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when a new engine was added successfully.
     */
    virtual void OnAddEngineSuccess(
        const std::shared_ptr<mip::FileEngine>& engine, 
        const std::shared_ptr<void>& context) { UNUSED(engine); UNUSED(context); }

    /**
     * @brief Called when adding a new engine caused an error.
     */
    virtual void OnAddEngineFailure(const std::exception_ptr& error, const std::shared_ptr<void>& context) { 
        UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when an engine was deleted successfully.
     */
    virtual void OnDeleteEngineSuccess(const std::shared_ptr<void>& context) { UNUSED(context); }

    /**
     * @brief Called when deleting an engine caused an error.
     */
    virtual void OnDeleteEngineFailure(
        const std::exception_ptr& error, 
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when the policy has changed for the engine with the given ID.
     */
    virtual void OnPolicyChanged(const std::string& engineId) { UNUSED(engineId); }

    /**
     * @brief Called prior to engine creation to describe whether or not the policy engine's policy data must be fetched from
     * the server or whether it can be created from locally cached data.
     * 
     * @param requiresPolicyFetch Describes whether engine data must be fetched via HTTP or if it will be loaded from cache
     * 
     * @note This optional callback may be used by an application to be informed whether or not an AddEngineAsync
     * operation will require an HTTP operation (with its associated delay) to complete.
     */
    virtual void OnAddPolicyEngineStarting(bool requiresPolicyFetch) { UNUSED(requiresPolicyFetch); }

  protected:
    Observer() {}
  };

  /**
   * @brief Settings used by FileProfile during its creation and throughout its lifetime
   */
  class Settings {
  public:
    /**
     * @brief FileProfile::Settings constructor
     * 
     * @param mipContext Global context settings
     * @param cacheStorageType Store any cached state in memory or on disk
     * @param consentDelegate Delegate used to obtain user permission to access external resources
     * @param observer Observer instance that will receive notifications of events related to FileProfile 
     */
    Settings(
        const std::shared_ptr<MipContext>& mipContext,
        CacheStorageType cacheStorageType,
        std::shared_ptr<ConsentDelegate> consentDelegate,
        std::shared_ptr<Observer> observer)
        : mMipContext(mipContext),
          mCacheStorageType(cacheStorageType),
          mConsentDelegate(consentDelegate),
          mObserver(observer),
          mCanCacheLicenses(true),
          mDnsRedirection(DnsRedirection::MDEDiscovery) {}

    /** @cond DOXYGEN_HIDE */
    ~Settings() {}
    /** @endcond */

    /**
     * @brief Get whether caches are stored in memory or on disk
     *
     * @return storage type used
     */
    CacheStorageType GetCacheStorageType() const { return mCacheStorageType; }

    /**
     * @brief Gets the consent delegate used to request user consent connecting to services
     * 
     * @return Consent delegate used for requesting user consent
     */
    std::shared_ptr<ConsentDelegate> GetConsentDelegate() const { return mConsentDelegate; }

    /**
     * @brief Gets the observer that receives notifications of events related to FileProfile
     * 
     * @return Observer that receives notifications of events related to FileProfile
     */
    std::shared_ptr<Observer> GetObserver() const { return mObserver; }

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
     * @brief Get the TaskDispatcher delegate (if any) provided by the application
     * 
     * @return TaskDispatcher delegate to be used for executing asynchronous tasks
     */
    std::shared_ptr<TaskDispatcherDelegate> GetTaskDispatcherDelegate() const { return mTaskDispatcherDelegate; }

    /**
     * @brief Override default asynchonous task dispatching handling  with client's own
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
    void SetSessionId(const std::string& sessionId) {
      mSessionId = sessionId;
    }

    /**
     * @brief Gets the session ID
     * 
     * @return Session ID that will be used to correlate logs/telemetry
     */
    const std::string& GetSessionId() const {
      return mSessionId;
    }

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
    std::string mPath;
    CacheStorageType mCacheStorageType;
    std::shared_ptr<ConsentDelegate> mConsentDelegate;
    std::shared_ptr<Observer> mObserver;
    std::string mSessionId;
    std::shared_ptr<HttpDelegate> mHttpDelegate;
    std::shared_ptr<TaskDispatcherDelegate> mTaskDispatcherDelegate;
    bool mCanCacheLicenses;
    std::shared_ptr<void> mLoggerContext;
    DnsRedirection mDnsRedirection;
  };

  /**
   * @brief Starts loading a profile based on the provided |settings|.
   * @return Async control object.
   * 
   * @note FileProfile::Observer will be called upon success or failure.
   */
  FILE_API static std::shared_ptr<AsyncControl> __CDECL LoadAsync(
      const Settings& settings,
      const std::shared_ptr<void>& context);

  /**
   * @brief Gets library version.
   *
   * @return Version string
   */
  FILE_API static const char* __CDECL GetVersion();

  /** @cond DOXYGEN_HIDE */
  virtual ~FileProfile() {}
  /** @endcond */

  /**
   * @brief Returns the profile settings.
   */
  virtual const Settings& GetSettings() const = 0;

  /**
   * @brief Starts list engines operation.
   * @return Async control object.
   * 
   * @note FileProfile::Observer will be called upon success or failure.
   */
  virtual std::shared_ptr<AsyncControl> ListEnginesAsync(const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Starts unloading the file engine with the given ID.
   * @return Async control object.
   * 
   * @note FileProfile::Observer will be called upon success or failure.
   */
  virtual std::shared_ptr<AsyncControl> UnloadEngineAsync(
      const std::string& id,
      const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Starts adding a new file engine to the profile.
   * @return Async control object.
   * 
   * @note FileProfile::Observer will be called upon success or failure.
   */
  virtual std::shared_ptr<AsyncControl> AddEngineAsync(
      const FileEngine::Settings& settings,
      const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Starts deleting the file engine with the given ID. All data for the given profile will be deleted.
   * @return Async control object.
   * 
   * @note FileProfile::Observer will be called upon success or failure.
   */
  virtual std::shared_ptr<AsyncControl> DeleteEngineAsync(
      const std::string& id,
      const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Trigger an authentication callback for policy
   * 
   * @param cloud Azure cloud
   * @param authDelegate Authentication callback that will be invoked
   * 
   * @note MIP will not cache or do anything else with the value returned by the auth delegate. This function is
   *       recommended for applications that aren't "logged in" until after MIP requests an auth token. It allows
   *       an application to fetch a token before MIP actually requires one.
   */
  virtual void AcquirePolicyAuthToken(Cloud cloud, const std::shared_ptr<AuthDelegate>& authDelegate) const = 0;

protected:
/** @cond DOXYGEN_HIDE */
  FileProfile() {}
  /** @endcond */
};

MIP_NAMESPACE_END

#endif  // API_MIP_FILE_FILE_PROFILE_H_