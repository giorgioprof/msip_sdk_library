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
 * @brief This file contains the PolicyProfile class which includes the PolicyProfile::Observer and the
 * PolicyProfile::Settings classes.
 * 
 * @file policy_profile.h
 */

#ifndef API_MIP_UPE_POLICY_PROFILE_H_
#define API_MIP_UPE_POLICY_PROFILE_H_

#include <memory>
#include <string>
#include <vector>

#include "mip/common_types.h"
#include "mip/error.h"
#include "mip/http_delegate.h"
#include "mip/logger_delegate.h"
#include "mip/mip_context.h"
#include "mip/mip_export.h"
#include "mip/mip_namespace.h"
#include "mip/task_dispatcher_delegate.h"
#include "mip/upe/policy_engine.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief PolicyProfile class is the root class for using the Microsoft Information Protection operations.
 * A typical application will only need one PolicyProfile but it can create multiple profiles if needed.
 */
class PolicyProfile {
public:
  /**
   * @brief Observer interface for clients to get notifications for profile related events.
   * 
   * @note All errors inherit from mip::Error.
   * @note Client should not call the engine back on the thread that calls the observer.
   */
  class Observer {
  public:

    /**
     * @brief Called when profile was loaded successfully.
     * 
     * @param profile the current profile used to start the operation.
     * @param context the context passed to the LoadAsync operation.
     */
    virtual void OnLoadSuccess(
        const std::shared_ptr<PolicyProfile>& profile,
        const std::shared_ptr<void>& context) { UNUSED(profile); UNUSED(context); }

    /**
     * @brief Called when loading a profile caused an error.
     * 
     * @param error the error that caused the load operation to fail.
     * @param context the context passed to the LoadAsync operation.
     */
    virtual void OnLoadFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when list of engines was generated successfully.
     * 
     * @param engineIds a list of engine IDs the are available.
     * @param context the context passed to the ListEnginesAsync operation.
     */
    virtual void OnListEnginesSuccess(
        const std::vector<std::string>& engineIds,
        const std::shared_ptr<void>& context) { UNUSED(engineIds); UNUSED(context); }

    /**
     * @brief Called when listing engines caused an error.
     * 
     * @param error the error that caused the list engine operation to fail.
     * @param context the context passed to the ListEnginesAsync operation.
     */
    virtual void OnListEnginesFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when an engine was unloaded successfully.
     * 
     * @param context the context passed to the UnloadEngineAsync operation.
     */
    virtual void OnUnloadEngineSuccess(const std::shared_ptr<void>& context) { UNUSED(context); }

    /**
     * @brief Called when unloading an engine caused an error.
     *  
     * @param error the error that caused the unload engine operation to fail.
     * @param context the context passed to the UnloadEngineAsync operation.
     */
    virtual void OnUnloadEngineFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when a new engine was added successfully.
     * 
     * @param engine the newly-added engine
     * @param context the context passed to the AddEngineAsync operation
     */
    virtual void OnAddEngineSuccess(
        const std::shared_ptr<PolicyEngine>& engine,
        const std::shared_ptr<void>& context) { UNUSED(engine); UNUSED(context); }

    /**
     * @brief Called prior to engine creation to describe whether or not the engine's policy data must be fetched from
     * the server or whether it can be created from locally cached data.
     * 
     * @param requiresPolicyFetch Describes whether engine data must be fetched via HTTP or if it will be loaded from cache
     * 
     * @note This optional callback may be used by an application to be informed whether or not an AddEngineAsync
     * operation will require an HTTP operation (with its associated delay) to complete.
     */
    virtual void OnAddEngineStarting(bool requiresPolicyFetch) { UNUSED(requiresPolicyFetch); }

    /**
     * @brief Called when adding a new engine caused an error.
     * 
     * @param error the error that caused the add engine operation to fail.
     * @param context the context passed to the AddEngineAsync operation.
     */
    virtual void OnAddEngineFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when an engine was deleted successfully.
     * 
     * @param context the context passed to the DeleteEngineAsync operation.
     */
    virtual void OnDeleteEngineSuccess(const std::shared_ptr<void>& context) { UNUSED(context); }

    /**
     * @brief Called when deleting an engine caused an error.
     * 
     * @param error the error that caused the delete engine operation to fail.
     * @param context the context passed to the DeleteEngineAsync operation.
     */
    virtual void OnDeleteEngineFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when the policy has changed for the engine with the given ID, Or when the loaded Custom Sensitivity Types
     * have changed.
     * 
     * @param engineId the engine
     * @note To load the new policy it is necessary to call AddEngineAsync again with the engine ID given.
     */
    virtual void OnPolicyChanged(const std::string& engineId) { UNUSED(engineId); }
    
    /** @cond DOXYGEN_HIDE */
    virtual ~Observer() { }
  protected:
    Observer() { }
    /** @endcond */
  };

  /**
   * @brief Settings used by PolicyProfile during its creation and throughout its lifetime
   */
  class Settings {
  public:
    /**
     * @brief Interface for configuring the profile.
     * 
     * @param mipContext Global context settings
     * @param cacheStorageType Store any cached state in memory or on disk
     * @param observer A class implementing the PolicyProfile::Observer interface. Can be nullptr.
     */
    Settings(
        const std::shared_ptr<MipContext>& mipContext,
        CacheStorageType cacheStorageType,
        const std::shared_ptr<PolicyProfile::Observer>& observer)
        : mMipContext(mipContext),
          mCacheStorageType(cacheStorageType),
          mObserver(observer) {}

    /**
     * @brief Get whether caches are stored in memory or on disk
     *
     * @return storage type used
     */
    CacheStorageType GetCacheStorageType() const { return mCacheStorageType; }

    /**
     * @brief Get the event observer.
     * 
     * @return the event observer.
     */
    const std::shared_ptr<PolicyProfile::Observer>& GetObserver() const { return mObserver; }

    /**
     * @brief Get MIP context which represents shared state across all profiles
     * 
     * @return MIP context
     */
    std::shared_ptr<MipContext> GetMipContext() const { return mMipContext; }

    /**
     * @brief Get the HTTP delegate (if any) provided by the application
     * 
     * @return Http delegate to be used for HTTP operations
     */
    std::shared_ptr<HttpDelegate> GetHttpDelegate() const { return mHttpDelegate; }

    /**
     * @brief Override default HTTP stack with client's own
     * 
     * @param httpDelegate Http callback interface implemented by client application
     */
    void SetHttpDelegate(const std::shared_ptr<HttpDelegate>& httpDelegate) { mHttpDelegate = httpDelegate; }

    /**
     * @brief Get the #StorageDelegate (if any) provided by the application
     * 
     * @return #StorageDelegate to be used for cache operations
     */
    std::shared_ptr<StorageDelegate> GetStorageDelegate() const { return mStorageDelegate; }

    /**
     * @brief Override default storage cache with client's own implementation.
     * 
     * @param storageDelegate #StorageDelegate implemented by client application
     */
    void SetStorageDelegate(const std::shared_ptr<StorageDelegate>& storageDelegate) { mStorageDelegate = storageDelegate; }

    /**
     * @brief Get the TaskDispatcher delegate (if any) provided by the application
     * 
     * @return TaskDispatcher delegate to be used for executing asynchronous tasks
     */
    std::shared_ptr<TaskDispatcherDelegate> GetTaskDispatcherDelegate() const { return mTaskDispatcherDelegate; }

    /**
     * @brief Override default asynchronous task dispatching handling with client's own
     * 
     * @param taskDispatcherDelegate Task dispatching callback interface implemented by client application
     * 
     * @note tasks can reference profile objects preventing its destruction as a result 
     * taskdispatcher queues should not be shared.
     */
    void SetTaskDispatcherDelegate(const std::shared_ptr<TaskDispatcherDelegate>& taskDispatcherDelegate) {
      mTaskDispatcherDelegate = taskDispatcherDelegate;
    }

    void SetSessionId(const std::string& sessionId) {
      mSessionId = sessionId;
    }

    const std::string& GetSessionId() const {
      return mSessionId;
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

    ~Settings() { }
  private:
    std::shared_ptr<MipContext> mMipContext;
    CacheStorageType mCacheStorageType;
    std::shared_ptr<PolicyProfile::Observer> mObserver;
    std::shared_ptr<HttpDelegate> mHttpDelegate;
    std::shared_ptr<TaskDispatcherDelegate> mTaskDispatcherDelegate;
    std::string mSessionId;
    std::vector<std::pair<std::string, std::string>> mCustomSettings;
    std::shared_ptr<void> mLoggerContext;
    std::shared_ptr<StorageDelegate> mStorageDelegate;
    /** @endcond */
  };

  /**
   * @brief Starts loading a profile based on the provided settings.
   * 
   * @param settings the profile settings used to load the profile object.
   * @param context a context parameter that will be passed into the observer functions.
   * @note PolicyProfile::Observer will be called upon success or failure.
   */
  MIP_API static std::shared_ptr<AsyncControl> __CDECL LoadAsync(
      const Settings& settings,
      const std::shared_ptr<void>& context);

  /**
   * @brief Loading a profile based on the provided settings.
   * 
   * @param settings Settings used by PolicyProfile during its initialization and throughout its lifetime
   * 
   * @return Newly created profile
   */
  MIP_API static std::shared_ptr<PolicyProfile> __CDECL Load(const Settings& settings);

  /**
   * @brief Get the library version.
   *
   * @return a version string,
   */
  MIP_API static const char* __CDECL GetVersion();

  /**
   * @brief Get the settings set on the profile.
   * 
   * @return settings set on the profile.
   */
  virtual const Settings& GetSettings() const = 0;

  /**
   * @brief Starts list engines operation.
   *
   * @param context a parameter that will be passed into the observer functions.
   * @note PolicyProfile::Observer will be called upon success or failure.
   */
  virtual std::shared_ptr<AsyncControl> ListEnginesAsync(const std::shared_ptr<void>& context) = 0;
  
  /**
   * @brief List of engines.
   *
   * @return Cached engine IDs
   */
  virtual std::vector<std::string> ListEngines() = 0;

  /**
   * @brief Starts unloading the policy engine with the given ID.
   * 
   * @param id the unique engine ID.
   * @param context a parameter that will be forwarded opaquely to the observer functions.
   * @note PolicyProfile::Observer will be called upon success or failure.
   */
  virtual std::shared_ptr<AsyncControl> UnloadEngineAsync(
      const std::string& id,
      const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Starts unloading the policy engine with the given ID.
   * 
   * @param id the unique engine ID.
   */
  virtual void UnloadEngine(const std::string& id) = 0;

  /**
   * @brief Starts adding a new policy engine to the profile.
   * 
   * @param settings the mip::PolicyEngine::Settings object that specifies the engine's settings.
   * @param context a parameter that will be forwarded opaquely to the observer functions and optional HttpDelegate.
   * @note PolicyProfile::Observer will be called upon success or failure.
   */
  virtual std::shared_ptr<AsyncControl> AddEngineAsync(
        const PolicyEngine::Settings& settings,
        const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Add a new policy engine to the profile.
   *
   * @param settings the mip::PolicyEngine::Settings object that specifies the engine's settings.
   * @param context a parameter that will be forwarded opaquely to the optional HttpDelegate
   *
   * @return Newly created PolicyEngine
   */
  virtual std::shared_ptr<PolicyEngine> AddEngine(
      const PolicyEngine::Settings& settings,
      const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Starts deleting the policy engine with the given ID. All data for the given profile will be deleted.
   * 
   * @param id the unique engine ID.
   * @param context a parameter that will be passed into the observer functions.
   * @note PolicyProfile::Observer will be called upon success or failure.
   */
  virtual std::shared_ptr<AsyncControl> DeleteEngineAsync(
      const std::string& id,
      const std::shared_ptr<void>& context) = 0;
  
  /**
   * @brief Delete the policy engine with the given ID. All data for the given engine will be deleted.
   *
   * @param id the unique engine ID.
   */
  virtual void DeleteEngine(const std::string& engineId) = 0;

  /**
   * @brief Trigger an authentication callback
   * 
   * @param cloud Azure cloud
   * @param authDelegate Authentication callback that will be invoked
   * 
   * @note MIP will not cache or do anything else with the value returned by the auth delegate. This function is
   *       recommended for applications that aren't "logged in" until after MIP requests an auth token. It allows
   *       an application to fetch a token before MIP actually requires one.
   */
  virtual void AcquireAuthToken(Cloud cloud, const std::shared_ptr<AuthDelegate>& authDelegate) const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~PolicyProfile() { }
protected:
  PolicyProfile() { }
  /** @endcond */
};

MIP_NAMESPACE_END

#endif  // API_MIP_UPE_POLICY_PROFILE_H_