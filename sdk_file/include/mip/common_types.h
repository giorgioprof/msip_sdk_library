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
 * @brief A file Containing the common types used by the upe, file and protection modules.
 * 
 * @file common_types.h
 */

#ifndef API_MIP_COMMON_TYPES_H_
#define API_MIP_COMMON_TYPES_H_

#include <cstring>
#include <exception>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "mip/mip_namespace.h"

/** @cond DOXYGEN_HIDE */
#ifndef UNUSED
#define UNUSED(x) (void)x
#endif
/** @endcond */

MIP_NAMESPACE_BEGIN

/**
 * @brief Layout for watermarks.
 */
enum class WatermarkLayout : unsigned int {
  HORIZONTAL = 0, /**< Watermark layout is horizontal */
  DIAGONAL = 1, /**< Watermark layout is diagonal */
};

/**
 * @brief Alignment for content marks (content header or content footer).
 */
enum class ContentMarkAlignment : unsigned int {
  LEFT = 0, /**< Content marking is aligned to the left */
  RIGHT = 1, /**< Content marking is aligned to the right */
  CENTER = 2, /**< Content marking is centered */
};

/**
 * @brief The assignment method of the label on the document. Whether the Assignment of the label was done 
 * automatically, standard or as a privileged operation (The equivalent to an administrator operation). 
 */
enum class AssignmentMethod : unsigned int {
  STANDARD = 0, /**< Label assignment method is standard */
  PRIVILEGED = 1, /**< Label assignment method is privileged */
  AUTO = 2, /**< Label assignment method is automatic */
};

/**
* @brief defines what triggered the SetLabel event
*/
enum class ActionSource : unsigned int {
  MANUAL, /**< Selected manually by user */
  AUTOMATIC, /**< Set by policy conditions */
  RECOMMENDED, /**< Set by user after label was recommended by policy conditions */
  DEFAULT /**< Set by default in policy */
};

/**
* @brief Defines what state of the data is the application acting upon 
*/
enum class DataState : unsigned int {
  REST, /**< Inactive data stored physically in databases/file/warehouses */
  MOTION, /**< Data traversing a network or temporarily residing in computer memory to be read or updated */
  USE, /**< Active data under constant change stored physically in databases/file/warehouses etc */
};

inline const std::string& GetFileContentFormat() {
  static const std::string fileContentFormat = "file";
  return fileContentFormat;
}

inline const std::string& GetEmailContentFormat() {
  static const std::string emailContentFormat = "email";
  return emailContentFormat;
}

/**
 * @brief Label filter types, optional set of properties that can be used to filter labels or label behavior when
 * calling list sensitivity labels or autoclassifying data.
 */
enum LabelFilterType : unsigned int {
  None                                    = 0,       /**< Disable default labeling filtration */
  CustomProtection                        = 1 << 0,  /**< Filter labels that may result in custom protection*/
  TemplateProtection                      = 1 << 1,  /**< Filter labels that may result in template protection */
  DoNotForwardProtection                  = 1 << 2,  /**< Filter labels that may result in do not forward */
  AdhocProtection                         = 1 << 3,  /**< Filter labels that may result in adhoc protection */
  HyokProtection                          = 1 << 4,  /**< Filter labels that may result in hyok protection */
  PredefinedTemplateProtection            = 1 << 5,  /**< Filter labels that may result in predefined template protection */
  DoubleKeyProtection                     = 1 << 6,  /**< Filter labels that may result in protection that requires double key, can be template, adhoc, dnf.  This filter removes support for all double key labels*/
  DoubleKeyUserDefinedProtection          = 1 << 7,  /**< Filter labels that may result in protection that requires double key adhoc and dnf.  This filter removes support for double key adhoc labels.  It is more selective than DoubleKeyProtection filter which removes all support*/
  EncryptOnlyProtection                   = 1 << 8,  /**< Filter labels that may result in encrypt only */
  SensitiveInformationClassifier          = 1 << 9,  /**< Classifier based on Sensitive Information Detection (e.g. Credit card number or SSN) uses the Microsoft Classification Engine (MCE) */
  MachineLearningClassifier               = 1 << 10, /**< Classifier based on Machine Learning (e.g. Offensive Language) */
  ExtendedSensitiveInformationClassifier  = 1 << 11, /**< Classifier based on Sensitive Information Detection (e.g. Credit card number or SSN) does not use MCE */
};

/**
 * @brief Name change within the cpp workspace to imply the filters has greater scope than 
 * just label behavior. Avoided changing name fully to prevent breaking on client operations.
 */
typedef LabelFilterType FunctionalityFilterType;

inline FunctionalityFilterType operator|(FunctionalityFilterType firstFilter, FunctionalityFilterType secondFilter) {
  return static_cast<FunctionalityFilterType>(static_cast<int>(firstFilter) | static_cast<int>(secondFilter));
}

inline FunctionalityFilterType operator&(FunctionalityFilterType firstFilter, FunctionalityFilterType secondFilter) {
  return static_cast<FunctionalityFilterType>(static_cast<int>(firstFilter) & static_cast<int>(secondFilter));
}

inline FunctionalityFilterType operator~(FunctionalityFilterType filter) {
  return static_cast<FunctionalityFilterType>(~(static_cast<int>(filter)));
}

/**
* @brief Defines new features by name
*/
enum class FeatureId : unsigned int {
  EncryptOnly, /**< Check if server supports EncryptOnly feature */
};

/**
* @brief various dynamic fields can be set into the text message of the application
* Some known:
* ${Item.Label}
* ${Item.Name}
* ${Item.Location}
* ${User.Name}
* ${User.PrincipalName}
* ${Event.DateTime}
* Others are still not defined:
* The sdk will replace them with correct values using these control flags.
*/
enum class VariableTextMarkingType : unsigned int {
  Default = 0, /**< Known markings are converted unknown marking are removed */
  PassThrough = 1, /**< Known markings are converted unknown marking are passed through */
  None = 2, /**< All markings are passed through */
};

/** @cond DOXYGEN_HIDE */

/**
* @brief behavior control on decrypting container types. (msg)
*/
enum class ContainerDecryptionOption : unsigned int {
  Default = 0, /** default same as msg */
  Msg = 1, /** decrypt top and secondary level, default for msg */
  Top = 2, /**< Known markings are converted unknown marking are passed through */
  All = 3, /**< Attempt to decrypt all, if failed keeps data encrypted. */
};

/**
 * @brief Converts AssignmentMethod enum to a string description
 * 
 * @param str an assignment method.
 * @return a string description of the assignment method.
 */
inline std::string ContainerDecryptionOptionString(ContainerDecryptionOption option) {
  static std::string kMethodArray[] = { "default", "msg", "top", "all" };
  return kMethodArray[static_cast<int>(option)];
}

/** @endcond */

/**
 * @brief The type of classification the application should be running on the data to determine autolabeling status
 */
enum class Classifier : unsigned int {
  SensitiveInformation          = 0,  // Classifier based on specific content (credit cards); Uses Microsoft Classification Engine
  MachineLearning               = 1,  // Classifier based on Machine Learning recognition (offensive language)
  ExtendedSensitiveInformation  = 2,  // Classifier based on specific content (credit cards); Does not use Microsoft Classification Engine
};

/**
 * @brief The workload the application is working on, used primary to check for consent.
 */
enum class Workload : unsigned int {
  Office                = 0,  // Office type workloads, which include File and Email content types.
  Purview               = 1,  // Purview type workloads, which includes File, Email and SchematizedData content types.
};

struct UniqueIdsAndContentFormats
{
  std::unordered_map<std::string, std::string> uniqueIds;
  std::vector<std::string> contentFormats;
};
typedef std::map<mip::Classifier, UniqueIdsAndContentFormats> ClassifierUniqueIdsAndContentFormats;

/**
 * @brief Converts AssignmentMethod enum to a string description
 * 
 * @param method an assignment method.
 * @return a string description of the assignment method.
 */
inline std::string GetAssignmentMethodString(AssignmentMethod method) {
  static std::string kMethodArray[] = { "Standard", "Privileged", "Auto" };
  return kMethodArray[static_cast<int>(method)];
}

/**
* @brief Get the action source name.
*
* @param actionSource The action source.
* @return a string representation of the action source.
*/
inline static std::string GetActionSourceString(ActionSource actionSource) {
  static std::string kMethodArray[] = { "Manual", "Automatic", "Recommended", "Default" };
  return kMethodArray[static_cast<int>(actionSource)];
}

/**
* @brief Get the content state name.
*
* @param actionSource The state of the content being worked upon.
* @return a string representation of the content state.
*/
inline static std::string GetDataStateString(DataState state) {
  static std::string kMethodArray[] = { "Rest", "Motion", "Use" };
  return kMethodArray[static_cast<int>(state)];
}

// Extension to use when creating sqlite table on disk
inline const std::string& GetSqliteExtension() {
  static const std::string sqliteExtension = ".sqlite3";
  return sqliteExtension;
}

// Path to use when creating a table in memory
inline const std::string& GetMemoryDatabasePath() {
  static const std::string memoryPath = ":memory:";
  return memoryPath;
}

/**
 * @brief Abstraction for identity.
 */
class Identity {
public:
  /**
   * @brief Default Identity constructor used when a user email address is not known.
   */
  Identity() { }
  /**
   * @brief Identity copy constructor.
   * 
   * @param Identity used to create the copy.
   */
  Identity(const Identity& other)
    : mEmail(other.mEmail),
      mName(other.mName) { }

  /**
   * @brief Identity constructor used when a user email address is known.
   * 
   * @param email must be valid email address.
   */
  explicit Identity(const std::string& email) : mEmail(email) { }

  /**
   * @brief Identity constructor used when a user email address and user name is known.
   * 
   * @param email must be valid email address.
   * @param name user name.
   */
  explicit Identity(const std::string& email, const std::string& name) : mEmail(email), mName(name) { }

  /**
   * @brief Get the email.
   * 
   * @return the email.
   */
  const std::string& GetEmail() const { return mEmail; }

  /**
   * @brief Get the user's friendly name. used for text marking.
   * 
   * @return the friendly name.
   */
  const std::string& GetName() const { return mName; }

private:
  std::string mEmail;
  std::string mName;
};

/**
 * @brief Delegate for auth related operations.
 */
class AuthDelegate {
public:
  /**
   * @brief a class that contains all the information required from the calling application in order to 
   * generate an oauth2 token.
   */
  class OAuth2Challenge {
  public:
    /**
     * @brief Construct a new OAuth2Challenge object
     *
     * @param authority the authority the token needs to be generated against.
     * @param resource  the resource the token is set to.
     * @param scope  the scope the token is set to.
     */
    OAuth2Challenge(
        const std::string& authority,
        const std::string& resource,
        const std::string& scope = std::string(),
        const std::string& claims = std::string())
        : mAuthority(authority),
          mResource(resource),
          mScope(scope),
          mClaims(claims) {
    }
    /**
     * @brief Get the authority string.
     *
     * @return the authority string.
     */
    const std::string& GetAuthority() const { return mAuthority; }
    /**
     * @brief Get the resource string
     *
     * @return the resource string.
     */
    const std::string& GetResource() const { return mResource; }
    /**
     * @brief Get the scope string
     *
     * @return the scope string.
     */
    const std::string& GetScope() const { return mScope; }
    /**
     * @brief Get the claims string
     *
     * @return the claims string.
     */
    const std::string& GetClaims() const { return mClaims; }
  private:
    std::string mAuthority;
    std::string mResource;
    std::string mScope;
    std::string mClaims;
  };

  /**
   * @brief A class containing access token info provided by an application
   */
  class OAuth2Token {
  public:
    /**
     * @brief Construct a new OAuth2Token object
     */
    OAuth2Token() {}

    /**
     * @brief Construct a new OAuth2Token object from JWT access token.
     *
     * @param accessToken JWT access token.
     */
    OAuth2Token(const std::string& accessToken) : mAccessToken(accessToken) {}

    /**
     * @brief Get the access token string.
     *
     * @return Access token string.
     */
    const std::string& GetAccessToken() const { return mAccessToken; }

    /**
     * @brief Set the access token string.
     *
     * @param accessToken Access token string.
     */
    void SetAccessToken(const std::string& accessToken) { mAccessToken = accessToken; }

    /**
     * @brief Get the error message, if any
     *
     * @return Error message.
     */
    const std::string& GetErrorMessage() const { return mErrorMessage; }

    /**
     * @brief Set the error message.
     *
     * @param errorMessage Error message.
     */
    void SetErrorMessage(const std::string& errorMessage) { mErrorMessage = errorMessage; }

  private:
    std::string mAccessToken;
    std::string mErrorMessage;
  };

  /**
   * @brief This method is called when an auth token is required for the policy engine with the given identity and the given challenge.
   * The client should return whether acquiring token was successful. If successful, it should initialize the given token object.
   *
   * @param identity User for whom a token is requested
   * @param challenge OAuth2 challenge
   * @param token [Output] Base64-encoded OAuth2 token
   * 
   * @return True if token successfully acquired, else false
   * 
   * @note On failure, if token output parameter contains an error message, it will be included in the NoAuthTokenError
   *       exception that will later be raised to the application.
   * 
   * @deprecated This method will soon be deprecated in favor of the one accepting a context parameter. If the new version
   * has been implemented, there is no need to implement this version.
   */
  virtual bool AcquireOAuth2Token(
      const Identity& identity,
      const OAuth2Challenge& challenge,
      OAuth2Token& token) { UNUSED(identity); UNUSED(challenge); UNUSED(token); return false; }

  /**
   * @brief This method is called when an auth token is required for the policy engine with the given identity and the given challenge.
   * The client should return whether acquiring token was successful. If successful, it should initialize the given token object.
   *
   * @param identity User for whom a token is requested
   * @param challenge OAuth2 challenge
   * @param context Opaque context that was passed to the MIP API by the host application
   * @param token [Output] Base64-encoded OAuth2 token
   * 
   * @return True if token successfully acquired, else false
   * 
   * @note On failure, if token output parameter contains an error message, it will be included in the NoAuthTokenError
   *       exception that will later be raised to the application.
   */
  virtual bool AcquireOAuth2Token(
      const Identity& identity,
      const OAuth2Challenge& challenge,
      const std::shared_ptr<void>& context,
      OAuth2Token& token) {
    // Temporarily call into old 'AcquireOAuth2Token' method until it is fully deprecated (in case application
    // hasn't yet implemented this new method)
    UNUSED(context);
    return AcquireOAuth2Token(identity, challenge, token);
  }

  /** @cond DOXYGEN_HIDE */
  virtual ~AuthDelegate() { }
protected:
  AuthDelegate() { }
  /** @endcond */
};

/**
 * @brief A user's response when consent is requested to connect to a service endpoint.
 */ 
enum class Consent : unsigned int {
  AcceptAlways, /**< Consent, and remember this decision */
  Accept,       /**< Consent, just one time */
  Reject        /**< Do not consent */
};

/**
 * @brief Delegate for consent related operations.
 * 
 * @note This delegate is implemented by a client application to know when a
 * consent request notification should be displayed to the user.
 */
class ConsentDelegate {
public:
  /**
   * @brief Called when SDK requires user consent to connect to a service
   * endpoint.
   * 
   * @param url The URL for which the SDK requires user consent
   * 
   * @return a Consent enum with the user's decision.
   * 
   * @note When the SDK requests user consent with this method, the client
   * application should present the URL to the user. Client applications should
   * provide some means of obtaining user consent and return the appropriate
   * Consent enum that corresponds to the user's decision.
   */
  virtual Consent GetUserConsent(const std::string& url) = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~ConsentDelegate() { }
protected:
  ConsentDelegate() { }
  /** @endcond */
};

/**
 * @brief A struct that includes application specific information 
 */
struct ApplicationInfo {
  std::string applicationId;     /**< Application identifier as set in the AAD portal, (Should be a GUID without brackets). */
  std::string applicationName;   /**< Application name, (Should only contain valid ASCII character excluding ';') */
  std::string applicationVersion;/**< The version of the application being used, (Should only contain valid ASCII character excluding ';')  */
};

/**
 * @brief Name of the setting to explicitly specify policy data.
 * 
 * @return the custom settings key.
 */
inline constexpr const char* GetCustomSettingPolicyDataName() {
  return "policy_data";
}
/**
 * @brief Name of the setting to explicitly specify file path to export SCC policy data to.
 * 
 * @return the custom settings key.
 */
inline constexpr const char* GetCustomSettingExportPolicyFileName() {
  return "export_policy_file";
}
/**
 * @brief Name of the setting to explicitly specify sensitivity data.
 * 
 * @return the custom settings key.
 */
inline constexpr const char* GetCustomSettingSensitivityTypesDataName() {
  return "sensitivity_types_data";
}
/**
 * @brief Name of the setting to explicitly specify policy data file path.
 * 
 * @return the custom settings key.
 */
inline constexpr const char* GetCustomSettingPolicyDataFile() {
  return "policy_file";
}
/**
 * @brief Name of the setting to explicitly specify sensitivity types data file path.
 * 
 * @return the custom settings key.
 */
inline constexpr const char* GetCustomSettingSensitivityTypesDataFile() {
  return "sensitivity_types_file";
}
/**
 * @brief Name of the setting that allows to enable label by custom properties and custom properties by label features.
 *
 * @return the custom settings key.
 */
inline constexpr const char* GetCustomSettingLabelCustomPropertiesSyncEnabled() {
  return "label_custom_properties_sync_enabled";
}
/**
 * @brief Name of the setting that enables overriding policy ttl in days be default it will be 30 days. 
 * Values should be set as string integers i < 0 means infinite time to live.
 * 
 * @return the custom settings key.
 */
inline constexpr const char* GetCustomSettingPolicyTtlDays() {
  return "PolicyTtlDays";
}
/**
 * @brief Name of the setting that enables overriding sensitivity policy ttl in days be default it will be 30 days. 
 * Values should be set as string integers i < 0 means infinite time to live.
 * 
 * @return the custom settings key.
 */
inline constexpr const char* GetCustomSettingSensitivityPolicyTtlDays() {
  return "SensitivityPolicyTtlDays";
}
/** @cond DOXYGEN_HIDE */
/**
 * @brief enforce pfile protection on msg file type when using file api.
 * EnforceMsgPfileProtection will overide enable_msg_file_type 
 * (enable_msg_file_type &= !EnforceMsgPfileProtection)
 * @return the custom settings key.
 */
inline constexpr const char* GetEnforceMsgPfileProtection() {
  return "EnforceMsgPfileProtection";
}
/**
 * @brief enable msg file type when using file api, experimental.
 * currently support remove protection, known gaps ole attachment, voice attachments.
 * @return the custom settings key.
 */
inline constexpr const char* GetCustomSettingEnableMsgFileType() {
  return "enable_msg_file_type";
}
/**
 * @brief Container (msg) decryption option, "default" (decrypt top level and secondary), "top" (decrypt top only), 
 * "all" not yet supported, experimental feature.
 * @return the custom container decryption option.
 */
inline constexpr const char* GetCustomSettingContainerDecryptionOption() {
  return "container_decryption_option";
}
/**
 * @brief enable PowerBI file type when using file api, experimental.
 * @return the custom settings key.
 */
inline constexpr const char* GetCustomSettingEnablePowerBIFileType() {
  return "enable_power_bi_file_type";
}
/**
 * @brief keep pdf linearization during protection and unprotection.
 * @return the custom settings key.
 */
inline constexpr const char* GetCustomSettingKeepPdfLinearization() {
  return "keep_pdf_linearization";
}
inline constexpr const char* GetCustomSettingEnableDeprecatedRemoveActions() {
  return "enable_deprecated_remove_actions";
}
inline constexpr const char* GetCustomSettingPrintDiscoveredInfoTypesToTrace() {
  return "LogMatchedContentToTrace";
}
inline constexpr const char* GetCustomSettingTaskTimeoutMs() {
  return "TaskTimeoutMs";
}

inline constexpr const char* GetCustomSettingLabelWithoutPolicy() {
  return "label_without_policy";
}

inline constexpr const char* GetCustomSettingMaxFileSizeForProtection() {
  return "max_file_size_for_protection";
}

inline constexpr const char* GetCustomSettingMaxNestedProtectedMsgs() {
  return "max_nested_protected_msgs";
}

inline constexpr const char* GetCustomSettingMaxPolicyXmlVersion() {
  return "max_policy_xml_version";
}

/**
 * @brief enable mipservice to specify delegated application information, for application using their service.
 * Current supported format for delegation {"id":"<valid-guid>", "name":"<ascii chars>", "version":"1.0.0.0"}
 * @return the custom settings key.
 */
inline constexpr const char* GetCustomSettingDelegateApplicationInfo() {
  return "delegate_application_info";
}

/**
 * @brief Configures whether co-auth metadata versioning is respected for Office file types.
 * By default, co-auth support is enabled via the Security & Compliance Center configuration.
 * Use this setting to disable co-auth metadata versioning even if it is enabled via policy.
 * To disable co-auth metadata, set this custom setting to false.
 */
inline constexpr const char* GetCustomSettingEnableCoAuthMetadata() {
  return "co_auth_metadata";
}

/**
 * @brief if set enable list senstivity labels to return all labels regardless of content formats, primarily useed for 
 * mip service where per call filtration is controlled by graph api.
 * @return the custom settings key.
 */
inline constexpr const char* GetCustomSettingLabelsForAllContentFormats() {
  return "labels_for_all_content_formats";
}

/** @endcond */

/** @cond DOXYGEN_HIDE */
class CaseInsensitiveComparator {
public:
  bool operator() (const std::string& lhs, const std::string& rhs) const {
    if (lhs.size() != rhs.size())
      return lhs.size() < rhs.size();

#ifdef _WIN32
    return _strnicmp(lhs.c_str(), rhs.c_str(), lhs.size()) < 0;
#else
    return strncasecmp(lhs.c_str(), rhs.c_str(), lhs.size()) < 0;
#endif // WIN32
  }
};
/** @endcond */

/**
 * @brief Storage type for the caches
 */
enum class CacheStorageType : unsigned int {
  InMemory = 0, /**< In Memory storage */
  OnDisk = 1, /**< On Disk storage */
  OnDiskEncrypted = 2 /**< On Disk storage with internal encryption (if supported by the platform) */
};

/**
 * @brief Describes PFile extensions behavior
 */
enum class PFileExtensionBehavior {
  Default = 0,     /**< Extensions will become as SDK default behavior  */
  PFileSuffix = 1, /**< Extensions will become <EXT>.PFILE */
  PPrefix = 2,     /**< Extensions will become P<EXT> */
};

/**
 * @brief Azure cloud identifier
 */
enum class Cloud {
  Unknown = 0,                     /**< Cloud not specified or URL not recognized as an Azure cloud */
  Custom = 1,                      /**< Custom cloud: endpoint base URL manually specified by application */
  Test = 2,                        /**< Test cloud */
  Commercial = 3,                  /**< Global commercial cloud */
  Germany = 4,                     /**< Germany sovereign cloud */
  US_DoD = 5,                      /**< US sovereign cloud (DoD) */
  US_GCC = 6,                      /**< US sovereign cloud (GCC) */
  US_GCC_High = 7,                 /**< US sovereign cloud (GCC-High) */
  US_Sec = 8,                      /**< US sovereign cloud (Sec) */
  US_Nat = 9,                      /**< US sovereign cloud (Nat) */
  China_01 = 10,                   /**< China sovereign cloud */
};

/**
 * @brief Diagnostic region identifier
 */
enum class DataBoundary {
  Default = 0,                  /**< Region is not specified */
  United_States = 1,            /**< United States region. Previously North America region */
  North_America = 1,            /**< DEPRECATED North America region */
  European_Union = 2,           /**< European Union region */
  Germany = 3,                  /**< Germany region. Maps to European Union. Previously used for Europe, MiddleEast, and Africa */
  Europe_MiddleEast_Africa = 3, /**< DEPRECATED Europe, Middle East & Africa region */
  Japan = 4,                    /**< Japan region. Previously Asia region */
  Asia = 4,                     /**< DEPRECATED Asia region */
  Australia = 5,                /**< Australia region */
};

/**
 * @brief Class used to cancel async operation.
 */
class AsyncControl {
public:
  /**
    * @brief Calling cancel will result in a attempt to cancel the task, in case of success the
    * appropriate onFailure callback will be called with mip::OperationCancelledError.
    * This feature is dependent on the task dispatcher delegate (@see mip::TaskDispatcherDelegate),
    * 
    * @return false if cancel signal cannot be dispatched, true otherwise.
    * 
    * @note Do not hold a strong reference to an AsyncControl object in a task completion block.
    */
  virtual bool Cancel() = 0;
  /** @cond DOXYGEN_HIDE */
  virtual ~AsyncControl() = default;
   /** @endcond */
};

MIP_NAMESPACE_END

#endif  // API_MIP_COMMON_TYPES_H_