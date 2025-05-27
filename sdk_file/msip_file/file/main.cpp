/**
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

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <codecvt>

#ifdef __linux__
#include <unistd.h>
#ifndef MAX_PATH
#define MAX_PATH 4096
#endif
#endif // __linux__

#include "cxxopts.hpp"

#include "auth_delegate_impl.h"
#include "consent_delegate_impl.h"
#include "file_execution_state_impl.h"
#include "file_handler_observer.h"
#include "stream_over_buffer.h"
#include "mip/common_types.h"
#include "mip/error.h"
#include "mip/file/file_handler.h"
#include "mip/file/file_inspector.h"
#include "mip/file/file_profile.h"
#include "mip/file/file_status.h"
#include "mip/file/labeling_options.h"
#include "mip/file/msg_inspector.h"
#include "mip/mip_context.h"
#include "mip/protection_descriptor.h"
#include "mip/protection_descriptor_builder.h"
#include "mip/protection/protection_handler.h"
#include "mip/protection/rights.h"
#include "mip/stream_utils.h"
#include "mip/stream_utils.h"
#include "mip/upe/policy_engine.h"
#include "mip/user_rights.h"
#include "mip/user_roles.h"
#include "mip/version.h"
#include "profile_observer.h"
#include "shutdown_manager.h"
#include "string_utils.h"
#include "utils.h"


using mip::ActionSource;
using mip::ApplicationInfo;
using mip::AssignmentMethod;
using mip::AuthDelegate;
using mip::CacheStorageType;
using mip::ConsentDelegate;
using mip::DataState;
using mip::FileEngine;
using mip::FileHandler;
using mip::FileInspector;
using mip::FileProfile;
using mip::FileStatus;
using mip::Identity;
using mip::InspectorType;
using mip::Label;
using mip::LabelingOptions;
using mip::MipContext;
using mip::MsgInspector;
using mip::NoPermissionsError;
using mip::PolicyEngine;
using mip::ProtectionDescriptor;
using mip::ProtectionDescriptorBuilder;
using mip::ProtectionHandler;
using mip::Stream;
using mip::UserRights;
using mip::UserRoles;
using sample::auth::AuthDelegateImpl;
using sample::consent::ConsentDelegateImpl;
using std::cin;
using std::codecvt_utf8_utf16;
using std::cout;
using std::endl;
using std::fstream;
using std::get_time;
using std::getline;
using std::ifstream;
using std::iostream;
using std::istream;
using std::make_shared;
using std::map;
using std::ostream_iterator;
using std::ostringstream;
using std::pair;
using std::put_time;
using std::shared_ptr;
using std::static_pointer_cast;
using std::string;
using std::stringstream;
using std::tm;
using std::vector;
using std::wstring_convert;
using std::wstring;

namespace {

static const char kPathSeparatorWindows = '\\';
static const char kPathSeparatorUnix = '/';
static const char kExtensionSeparator = '.';

// Explicit null character at the end is required since array initializer does NOT add it.
static const char kPathSeparatorCStringWindows[] = {kPathSeparatorWindows, '\0'};
static const char kPathSeparatorCStringUnix[] = {kPathSeparatorUnix, '\0'};
static const char kPathSeparatorsAll[] = {kPathSeparatorWindows, kPathSeparatorUnix, '\0'};

const char* whiteSpaceCharacters = " \t\r\n";
void Trim(string& a, const char* trim_char = whiteSpaceCharacters) {
  auto pos = a.find_last_not_of(trim_char);
  if (pos != string::npos) {
    a.erase(pos + 1);
    pos = a.find_first_not_of(trim_char);
    if (pos != string::npos) a.erase(0, pos);
  } else
    a.erase(a.begin(), a.end());
}

vector<string> SplitString(const string& str, char delim) {
  vector<string> output;
  stringstream ss(str);
  string substr;

  while (ss.good()) {
    getline(ss, substr, delim);
    output.emplace_back(std::move(substr));
  }

  return output;
}

map<string, string> SplitDict(const string& str) {
  map<string, string> dict;
  auto entries = SplitString(str, ',');
  for (const auto& entry : entries) {
    auto kv = SplitString(entry, ':');
    if (kv.size() == 2)
      dict[kv[0]] = kv[1];
  }
  return dict;
}

string GetFileName(const string& filePath) {
  auto index = filePath.find_last_of(kPathSeparatorsAll);
  if (index == string::npos) return filePath;
  return filePath.substr(index + 1);
}

string GetFileExtension(const string& filePath) {
  string fileName = GetFileName(filePath);
  auto index = fileName.rfind(kExtensionSeparator);
  if (index == string::npos) return "";
  return fileName.substr(index); // Include the dot in the file extension
}

shared_ptr<FileStatus> GetFileStatus(const string& filePath, const shared_ptr<Stream>& fileStream, const shared_ptr<MipContext>& mipContext) {
  if (fileStream) {
    return FileHandler::GetFileStatus(fileStream, filePath, mipContext);
  } else {
    return FileHandler::GetFileStatus(filePath, mipContext);
  }
}

bool EqualsIgnoreCase(const string& a, const string& b) {
  auto size = a.size();
  if (b.size() != size) {
    return false;
  }
    
  for (size_t i = 0; i < size; i++) {
    if (tolower(a[i]) != tolower(b[i])) {
      return false;
    }
  }

  return true;
}

map<mip::FlightingFeature, bool> SplitFeatures(const string& str) {
  map<mip::FlightingFeature, bool> dict;
  auto entries = SplitString(str, ',');
  for (const auto& entry : entries) {
    auto kv = SplitString(entry, ':');
    if (kv.size() == 2)
      dict[(mip::FlightingFeature)stoul(kv[0])] = (EqualsIgnoreCase(kv[1], "true") || (kv[1] == "1")) ? true : false;
    else
      throw std::runtime_error(string("FlightingFeature has wrong format: ") + entry);
  }
  return dict;
}

shared_ptr<mip::Stream> GetInputStreamFromFilePath(const string& filePath) {
  auto ifStream = make_shared<fstream>(filePath, std::ios_base::in | std::ios_base::binary);
  uint8_t buffer[4096] = {0};
  std::streamsize bytesRead = 0;
  vector<uint8_t> memoryFile;
  do {
    ifStream->clear();
    ifStream->read(reinterpret_cast<char*>(buffer), sizeof(buffer));
    bytesRead = ifStream->gcount();
    if (bytesRead > 0) {
      memoryFile.insert(memoryFile.end(), &buffer[0], &buffer[bytesRead]);
    }
  } while (bytesRead > 0);

  return make_shared<StreamOverBuffer>(std::move(memoryFile));
}

// Get the current label and protection on this file and print to console label and protection information
void GetLabel(
  const shared_ptr<FileHandler>& fileHandler) {
  auto protection = fileHandler->GetProtection(); // Get the current protection on the file
  auto label = fileHandler->GetLabel(); //Get the current label on the file

  if (!label && !protection) {
    cout << "File is neither labeled nor protected" << endl;
    return;
  }

  if (label) {
    bool isPrivileged = label->GetAssignmentMethod() == AssignmentMethod::PRIVILEGED;
    auto extendedProperties = label->GetExtendedProperties();
    cout << "File is labeled as: " << label->GetLabel()->GetName() << endl;
    cout << "Id: " << label->GetLabel()->GetId() << endl;

    if (const shared_ptr<mip::Label> parent = label->GetLabel()->GetParent().lock()) {
      cout << "Parent label: " << parent->GetName() << endl;
      cout << "Parent Id: " << parent->GetId() << endl;
    }
    auto time = std::chrono::system_clock::to_time_t(label->GetCreationTime());
    std::stringstream ss;
    ss << time;
    cout << "Set time: " << ss.str() << endl;
    cout << "Privileged: " << (isPrivileged ? "True" : "False") << endl;
    if (!extendedProperties.empty()) {
      cout << "Extended Properties: " << endl;
    }
    for (size_t j = 0; j < extendedProperties.size(); j++) {
      cout << "Key: " << extendedProperties[j].GetKey() <<
          ", Value: " << extendedProperties[j].GetValue() << endl;
    }

  } else
    cout << "File is not labeled by an official policy" << endl;

  if (protection) {
    cout << "File is protected with ";

    const shared_ptr<ProtectionDescriptor> protectionDescriptor = protection->GetProtectionDescriptor();
    if (protectionDescriptor->GetProtectionType() == mip::ProtectionType::TemplateBased)
      cout << "template." << endl;
    else
      cout << "custom permissions." << endl;

    cout << "Name: " << protectionDescriptor->GetName() << endl;
    cout << "Template Id: " << protectionDescriptor->GetTemplateId() << endl;

    for (const auto& usersRights : protectionDescriptor->GetUserRights()) {
      cout << "Rights: ";
      auto rights = usersRights.Rights();
      copy(rights.cbegin(), rights.cend() - 1, ostream_iterator<string>(cout, ", "));
      cout << *rights.crbegin() << endl;

      cout << "For Users: ";
      auto users = usersRights.Users();
      copy(users.cbegin(), users.cend() - 1, ostream_iterator<string>(cout, "; "));
      cout << *users.crbegin() << endl;
    }

    for (const auto& usersRoles : protectionDescriptor->GetUserRoles()) {
      cout << "Roles: ";
      auto roles = usersRoles.Roles();
      copy(roles.cbegin(), roles.cend() - 1, ostream_iterator<string>(cout, ", "));
      cout << *roles.crbegin() << endl;

      cout << "For Users: ";
      auto users = usersRoles.Users();
      copy(users.cbegin(), users.cend() - 1, ostream_iterator<string>(cout, "; "));
      cout << *users.crbegin() << endl;
    }

    if (protectionDescriptor->DoesContentExpire()) {
      time_t validUntilTime = std::chrono::system_clock::to_time_t(protectionDescriptor->GetContentValidUntil());
      tm validUntilUtc = {};
      tm validUntil = {};

      gmtime_r(&validUntilTime, &validUntilUtc);
      localtime_r(&validUntilTime, &validUntil);
      cout << "Content Expiration (UTC): " << put_time(&validUntilUtc, "%FT%TZ") << endl;
      cout << "Content Expiration: " << put_time(&validUntil, "%FT%T%z") << endl;
    }
  }
}

string CreateOutput(FileHandler* fileHandler) {
  auto outputFileName = fileHandler->GetOutputFileName();
  auto fileExtension = GetFileExtension(outputFileName);
  auto outputFileNameWithoutExtension = outputFileName.substr(0, outputFileName.length() - fileExtension.length());

  if (EqualsIgnoreCase(fileExtension, ".pfile")) {
    fileExtension = GetFileExtension(outputFileNameWithoutExtension) + fileExtension;
    outputFileNameWithoutExtension = outputFileName.substr(0, outputFileName.length() - fileExtension.length());
  }

  return outputFileNameWithoutExtension + "_modified" + fileExtension;
}

char PathSeparator() {
#ifdef _WIN32
  return kPathSeparatorWindows;
#else
  return kPathSeparatorUnix;
#endif
}

string GetDirFromPath(const string& fileSamplePath) {
  string result;
  if (!fileSamplePath.empty()) {
    auto position = fileSamplePath.find_last_of(PathSeparator());
    result = fileSamplePath.substr(0, position + 1);
  }
  return result;
}

string CombinePaths(const string& folder, const string& relativePath) {
  if (folder.empty()) return relativePath;
  string result = folder;
  if (result[result.length() - 1] != PathSeparator()) result += PathSeparator();
  return result + relativePath;
}


void SetLabel(
  const shared_ptr<FileHandler>& fileHandler,
  const shared_ptr<Label>& label,
  const string& filePath,
  AssignmentMethod method,
  const string& justificationMessage,
  const vector<pair<string, string>>& extendedProperties) {

  LabelingOptions labelingOptions(method);
  labelingOptions.SetDowngradeJustification(!justificationMessage.empty(), justificationMessage);
  labelingOptions.SetExtendedProperties(extendedProperties);

  if (label == nullptr) {
    fileHandler->DeleteLabel(labelingOptions); // Delete the current label from the file
  } else {
    fileHandler->SetLabel(label, labelingOptions, mip::ProtectionSettings());  // Set a label with label Id to the file
  }

  auto commitPromise = make_shared<std::promise<bool>>();
  auto commitFuture = commitPromise->get_future();
  auto modified = fileHandler->IsModified();
 
  if (modified) {
    auto outputFilePath = CreateOutput(fileHandler.get());
    try {
      fileHandler->CommitAsync(outputFilePath, commitPromise);
      auto committed = commitFuture.get();

      if (committed) {
        cout << "New file created: " << outputFilePath << endl;
        //Triggers audit event
        fileHandler->NotifyCommitSuccessful(filePath);
      } else {
        ifstream ifs(FILENAME_STRING(outputFilePath));
        if (!ifs.fail()) {
          throw std::runtime_error("commitAsync unable to delete outputfile");
        }
      }
    } catch (const std::exception& /*ex*/) {
      ifstream ifs(FILENAME_STRING(outputFilePath));
      if (!ifs.fail()) {
        cout << "commitAsync unable to delete outputfile: " << outputFilePath << endl;
      }
      throw;
    }
  } else {
      cout << "No changes to commit" << endl;
  }
}

// Helper function to escape JSON string values
std::string escapeJsonString(const std::string& input) {
    std::string output = input;
    
    // Replace backslashes with double backslashes
    size_t pos = 0;
    while ((pos = output.find("\\", pos)) != std::string::npos) {
        output.replace(pos, 1, "\\\\");
        pos += 2;
    }
    
    // Replace unescaped double quotes with escaped double quotes
    pos = 0;
    while ((pos = output.find("\"", pos)) != std::string::npos) {
        // Check if this quote is already escaped
        if (pos == 0 || output[pos-1] != '\\') {
            output.replace(pos, 1, "\\\"");
            pos += 2;
        } else {
            pos += 1;
        }
    }
    
    return output;
}

std::string getUnprotectStatusJSON(bool status, const std::string& error, const std::string& outputPath) {
    std::ostringstream oss;
    oss << "{\"status\": " << (status ? "true" : "false") 
        << ", \"path\": \"" << outputPath << "\""
        << ", \"error\": \"" << escapeJsonString(error) << "\"}";
    return oss.str();
}


void Unprotect(
  const shared_ptr<MipContext>& mipContext,
  const shared_ptr<FileHandler>& fileHandler,
  shared_ptr<Stream> fileStream,
  const string& filePath,
  char * result) {
  cout << filePath << endl;
  auto fileStatus = GetFileStatus(filePath, fileStream, mipContext);
  auto isProtected = fileStatus->IsProtected();
  auto containsProtectedObjects = fileStatus->ContainsProtectedObjects();
  // Note that only checking if the file is protected does not require any network IO or auth
  string fileStatus_str;
  
  if (!isProtected && !containsProtectedObjects) {
    cout << "File is not protected and does not contain protected objects, no change made." << endl;
    strcpy(result, getUnprotectStatusJSON(false, "File is not protected and does not contain protected objects, no change made.", "").c_str());
    return;
  }

  fileHandler->RemoveProtection(); // Remove the protection from the file
  auto outputFilePath = CreateOutput(fileHandler.get());

  auto commitPromise = make_shared<std::promise<bool>>();
  auto commitFuture = commitPromise->get_future();
  auto modified = fileHandler->IsModified();
  if (modified) {
    fileHandler->CommitAsync(outputFilePath, commitPromise);
    auto committed = commitFuture.get();

    if (committed) {
      cout << "New file created: " << outputFilePath << endl;
        strcpy(result, getUnprotectStatusJSON(true, "", outputFilePath).c_str());
    } else {
      ifstream ifs(FILENAME_STRING(outputFilePath));
      if (!ifs.fail()) {
        throw std::runtime_error("commitAsync unable to delete outputfile");
      }
    }
  } else {
    cout << "No changes to commit" << endl;
    strcpy(result, getUnprotectStatusJSON(false, "No changes to commit", "").c_str());
  }
}

// Print the labels and sublabels to the console
void ListLabels(const vector<shared_ptr<mip::Label>>& labels, const string& delimiter = "") {
  static const size_t kMaxTooltipSize = 70;
  for (const auto& label : labels) {
    string labelTooltip = label->GetTooltip();
    if (labelTooltip.size() > kMaxTooltipSize)
      labelTooltip.substr(0, kMaxTooltipSize) + "...";
    
    string labelAutoTooltip = label->GetAutoTooltip();
    if (labelAutoTooltip.size() > kMaxTooltipSize)
      labelAutoTooltip.substr(0, kMaxTooltipSize) + "...";
    string isActive = (label->IsActive()) ? "true" : "false";
    cout << delimiter << "Label ID: " << label->GetId() << "\n" <<
        delimiter << "Label name: " << label->GetName() << "\n" <<
        delimiter << "Label sensitivity: " << label->GetSensitivity() << "\n" <<
        delimiter << "Label IsActive: " << isActive << "\n" <<
        delimiter << "Label tooltip: " << labelTooltip << "\n" <<
        delimiter << "Label autoTooltip: " << labelAutoTooltip << "\n" << endl;
    
    const vector<shared_ptr<mip::Label>>& childLabels = label->GetChildren();
    if (!childLabels.empty()) {
      cout << delimiter << "Child labels:" << endl;
      ListLabels(childLabels, delimiter + "  ");
    }
  }
}


void ProtectWithCustomPermissions(
  const shared_ptr<FileHandler>& fileHandler,
  const shared_ptr<FileHandler>& encfileHandler,
  char * result) {
  
  fileHandler->SetProtection(encfileHandler->GetProtection());
  auto outputFilePath = CreateOutput(fileHandler.get());

  auto commitPromise = make_shared<std::promise<bool>>();
  auto commitFuture = commitPromise->get_future();
  fileHandler->CommitAsync(outputFilePath, commitPromise);
  auto committed = commitFuture.get();

  if (committed) {
    cout << "New file created: " << outputFilePath << endl;
    strcpy(result, getUnprotectStatusJSON(true, "", outputFilePath).c_str());
  } else {
    ifstream ifs(FILENAME_STRING(outputFilePath));
    if (!ifs.fail()) {
      throw std::runtime_error("commitAsync unable to delete outputfile");
      strcpy(result, getUnprotectStatusJSON(false, "commitAsync unable to delete outputfile", "").c_str());
    } else {
      strcpy(result, getUnprotectStatusJSON(false, "No changes to commit", "").c_str());
     }
  }
 }

string ReadPolicyFile(const string& policyPath) {
  ifstream ifs(FILENAME_STRING(policyPath));
  if (ifs.fail())
    throw std::runtime_error("Failed to read path: " + policyPath);

  cout << "Using policy from file: " << policyPath << endl;

  ostringstream policyContent;
  policyContent << ifs.rdbuf();
  ifs.close();
  return policyContent.str();
}

void EnsureUserHasRights(const shared_ptr<FileHandler>& fileHandler) {
  if (!fileHandler->GetProtection() || fileHandler->GetProtection()->AccessCheck(mip::rights::Export())) {
    return;
  }

  throw NoPermissionsError(
      NoPermissionsError::Category::AccessDenied,
      "A minimum right of EXPORT is required to change label or protection",
      fileHandler->GetProtection()->GetProtectionDescriptor()->GetReferrer(),
      fileHandler->GetProtection()->GetOwner());
}

shared_ptr<FileProfile> CreateProfile(
    const shared_ptr<MipContext>& mipContext,
    const shared_ptr<ConsentDelegate>& consentDelegate) {
  const shared_ptr<ProfileObserver> sampleProfileObserver = make_shared<ProfileObserver>();

  FileProfile::Settings profileSettings(
      mipContext,
      CacheStorageType::InMemory,
      consentDelegate,
      sampleProfileObserver);

  auto loadPromise = make_shared<std::promise<shared_ptr<FileProfile>>>();
  auto loadFuture = loadPromise->get_future();
  FileProfile::LoadAsync(profileSettings, loadPromise); // Getting the profile
  return loadFuture.get();
}

vector<mip::LabelFilterType> CreateLabelFiltersFromString(const string& labelFilter) {
  vector<mip::LabelFilterType> retVal;
  auto entries = SplitString(labelFilter, ',');
  for (auto& filter : entries) {
    Trim(filter);
    if (filter == "None") {
      retVal.emplace_back(mip::LabelFilterType::None);
    } else if (filter == "CustomProtection") {
      retVal.emplace_back(mip::LabelFilterType::CustomProtection);
    } else if (filter == "TemplateProtection") {
      retVal.emplace_back(mip::LabelFilterType::TemplateProtection);
    } else if (filter == "DoNotForwardProtection") {
      retVal.emplace_back(mip::LabelFilterType::DoNotForwardProtection);
    } else if (filter == "AdhocProtection") {
      retVal.emplace_back(mip::LabelFilterType::AdhocProtection);
    } else if (filter == "HyokProtection") {
      retVal.emplace_back(mip::LabelFilterType::HyokProtection);
    } else if (filter == "PredefinedTemplateProtection") {
      retVal.emplace_back(mip::LabelFilterType::PredefinedTemplateProtection);
    } else if (filter == "DoubleKeyProtection") {
      retVal.emplace_back(mip::LabelFilterType::DoubleKeyProtection);
    } else if (filter == "DoubleKeyUserDefinedProtection") {
      retVal.emplace_back(mip::LabelFilterType::DoubleKeyUserDefinedProtection);
    } else if (filter == "SensitiveInformationClassifier") {
      retVal.emplace_back(mip::LabelFilterType::SensitiveInformationClassifier);
    } else if (filter == "MachineLearningClassifier") {
      retVal.emplace_back(mip::LabelFilterType::MachineLearningClassifier);
    } else if (filter == "ExtendedSensitiveInformationClassifier") {
      retVal.emplace_back(mip::LabelFilterType::ExtendedSensitiveInformationClassifier);
    } else if (filter.empty()) {
      //Do nothing
    } else {
      throw cxxopts::OptionException(string("Filter type not recognized: ") + filter);
    }
  }
  return retVal;
}

void ConfigureFunctionality(
    FileEngine::Settings& settings,
    const string& enableFunctionality,
    const string& disableFunctionality) {
  for(const auto& filter : CreateLabelFiltersFromString(enableFunctionality)) {
    settings.ConfigureFunctionality(filter, true);
  }

  for(const auto& filter : CreateLabelFiltersFromString(disableFunctionality)) {
    settings.ConfigureFunctionality(filter, false);
  }
}

shared_ptr<FileEngine> GetFileEngine(
    const shared_ptr<FileProfile>& fileProfile,
    const shared_ptr<AuthDelegate>& authDelegate,
    const string& username,
    const string& protectionBaseUrl,
    const string& policyBaseUrl,
    const string& policyPath,
    bool enableMsg,
    bool decryptTopOnly,
    bool decryptAll,
    bool enablePowerBI,
    bool protectionOnly,
    const string& locale,
    const string& enableFunctionality,
    const string& disableFunctionality,
    bool keepPdfLinearization) {
  FileEngine::Settings settings(Identity(username), authDelegate, "" /*clientData*/, locale, false /* loadSensitivityTypes*/);

  settings.SetCloud(mip::Cloud::Commercial);
  settings.SetProtectionOnlyEngine(protectionOnly);

  if (!protectionBaseUrl.empty() && !policyBaseUrl.empty()) {
    settings.SetProtectionCloudEndpointBaseUrl(protectionBaseUrl);
    settings.SetPolicyCloudEndpointBaseUrl(policyBaseUrl);
    settings.SetCloud(mip::Cloud::Custom);
  }

  ConfigureFunctionality(settings, enableFunctionality, disableFunctionality);
  
  vector<pair<string, string>> customSettings;
  if (!policyPath.empty()) { // If Policy path was given, saving the policy in custom setting
    customSettings.emplace_back(mip::GetCustomSettingPolicyDataName(), ReadPolicyFile(policyPath)); //Save the content of the policy in custom setting
  }
  if (enableMsg) {
    customSettings.emplace_back(mip::GetCustomSettingEnableMsgFileType(), "true"); // enable msg format for sample application testing.
  }
  if (enablePowerBI) {
    customSettings.emplace_back(mip::GetCustomSettingEnablePowerBIFileType(), "true"); // enable PowerBI format for sample application testing.
  }
  if (keepPdfLinearization) {
    customSettings.emplace_back(mip::GetCustomSettingKeepPdfLinearization(), "true"); // keep pdf linearization for sample application testing.
  }
  if (decryptTopOnly && decryptAll) {
    throw std::invalid_argument("Both decryptTopOnly and decryptAll cannot be true at the same time.");
  }
  if (decryptTopOnly) {
    // decrypt only the top of the message, keep attachment protected.
    customSettings.emplace_back(mip::GetCustomSettingContainerDecryptionOption(),
        mip::ContainerDecryptionOptionString(mip::ContainerDecryptionOption::Top)); 
  }
  if (decryptAll) {
    // decrypt the entire message, including attachments.
    customSettings.emplace_back(mip::GetCustomSettingContainerDecryptionOption(),
        mip::ContainerDecryptionOptionString(mip::ContainerDecryptionOption::All)); 
  }
  settings.SetCustomSettings(customSettings);

  auto addEnginePromise = make_shared<std::promise<shared_ptr<FileEngine>>>();
  auto addEngineFuture = addEnginePromise->get_future();
  fileProfile->AddEngineAsync(settings, addEnginePromise); // Getting the engine
  return addEngineFuture.get();
}

shared_ptr<FileHandler> GetFileHandler(
    const shared_ptr<FileEngine>& fileEngine,
    const shared_ptr<Stream>& stream,
    const string& filePath,
    DataState dataState,
    bool displayClassificationRequests,
    string applicationScenarioId) {
  auto fileExecutionState = make_shared<FileExecutionStateImpl>(dataState, nullptr, displayClassificationRequests, applicationScenarioId);
  auto createFileHandlerPromise = make_shared<std::promise<shared_ptr<FileHandler>>>();
  auto createFileHandlerFuture = createFileHandlerPromise->get_future();
  bool auditDiscoveryEnabled = !displayClassificationRequests;
  // Here content identifier is same as the filePath
  if (stream) {
    fileEngine->CreateFileHandlerAsync(stream, filePath, auditDiscoveryEnabled, make_shared<FileHandlerObserver>(), createFileHandlerPromise, fileExecutionState); // create the file handler
  } else {
    fileEngine->CreateFileHandlerAsync(filePath, filePath, auditDiscoveryEnabled, make_shared<FileHandlerObserver>(), createFileHandlerPromise, fileExecutionState); // create the file handler
  }
  return createFileHandlerFuture.get();
}

string GetWorkingDirectory() {
  string fileSamplePath;

  char result[MAX_PATH];
  auto count = readlink("/proc/self/exe", result, MAX_PATH);
  fileSamplePath = string(result, (count > 0) ? count : 0);

  fileSamplePath = GetDirFromPath(fileSamplePath);
  return fileSamplePath;
}

} // namespace


extern "C" int getFileStatus(const char *filePath_str, const char *applicationId_str, char *result)
{
  try {
    shared_ptr<mip::Stream> fileStream = nullptr;
    map<string, string> telemetrySettings;
    const string filePath(filePath_str);
    const string applicationId(applicationId_str);
    ApplicationInfo appInfo;
    appInfo.applicationId = applicationId;
    appInfo.applicationName = "MsipFileApp";
    appInfo.applicationVersion = "1.0.0.0";
    auto fileSampleWorkingDirectory = GetWorkingDirectory();

    auto diagnosticOverride = make_shared<mip::DiagnosticConfiguration>();
    diagnosticOverride->customSettings = telemetrySettings;
    diagnosticOverride->isAuditPriorityEnhanced = true;
    diagnosticOverride->maxTeardownTimeSec = 2;
    diagnosticOverride->isMaxTeardownTimeEnabled = true;
    auto mipConfiguration = make_shared<mip::MipConfiguration>(appInfo, "file_sample_storage", mip::LogLevel::Trace, 0);
    mipConfiguration->SetDiagnosticConfiguration(diagnosticOverride);
    map<mip::FlightingFeature, bool> featureSettingsOverride;
    mipConfiguration->SetFeatureSettings(featureSettingsOverride);

    auto mipContext = MipContext::Create(mipConfiguration);
    sample::ShutdownManager lifetimeManager(mipContext);
    string fileStatus_str;
    auto fileStatus = GetFileStatus(filePath, fileStream, mipContext);
    fileStatus_str += "{\"protected\": ";
    fileStatus_str += fileStatus->IsProtected() ? "true" : "false";
    fileStatus_str += ", \"labeled\": ";
    fileStatus_str += fileStatus->IsLabeled() ? "true" : "false";
    fileStatus_str += ", \"protected_objects\": ";
    fileStatus_str += fileStatus->ContainsProtectedObjects() ? "true" : "false";
    fileStatus_str += ", \"path\": \"";
    fileStatus_str += filePath_str;
    fileStatus_str += "\", \"status\": true";
    fileStatus_str += "}";
    strcpy(result, fileStatus_str.c_str());
    return EXIT_SUCCESS;   
  }
  catch (const std::exception& ex) {
    string fileStatus_str;
    fileStatus_str += "{\"status\": false, \"error\": \"";
    fileStatus_str += escapeJsonString(ex.what());
    fileStatus_str += "\", \"path\": \"";
    fileStatus_str += filePath_str;
    fileStatus_str += "\"}";
    strcpy(result, fileStatus_str.c_str());
    return EXIT_FAILURE;
  }
}    


extern "C" int unprotectFile(const char* protectionToken_str, const char *filePath_str, const char *applicationId_str, char *result)
{
  try {
    shared_ptr<mip::Stream> fileStream = nullptr;
    map<string, string> telemetrySettings;
    const string filePath(filePath_str);
    const string protectionToken(protectionToken_str);
    const string applicationId(applicationId_str);
    ApplicationInfo appInfo;
    appInfo.applicationId = applicationId;
    appInfo.applicationName = "MsipFileApp";
    appInfo.applicationVersion = "1.0.0.0";
    auto fileSampleWorkingDirectory = GetWorkingDirectory();

    const string username = "";
    const string password = "";
    const string sccToken = "";

    auto authDelegate = make_shared<AuthDelegateImpl>(false /*isVerbose*/, username, password, applicationId, sccToken, protectionToken, fileSampleWorkingDirectory);

    auto consentDelegate = make_shared<ConsentDelegateImpl>(false /*isVerbose*/);

    auto diagnosticOverride = make_shared<mip::DiagnosticConfiguration>();
    diagnosticOverride->customSettings = telemetrySettings;
    diagnosticOverride->isAuditPriorityEnhanced = true;
    diagnosticOverride->maxTeardownTimeSec = 2;
    diagnosticOverride->isMaxTeardownTimeEnabled = true;
    auto mipConfiguration = make_shared<mip::MipConfiguration>(appInfo, "file_sample_storage", mip::LogLevel::Trace, 0);
    mipConfiguration->SetDiagnosticConfiguration(diagnosticOverride);
    map<mip::FlightingFeature, bool> featureSettingsOverride;
    mipConfiguration->SetFeatureSettings(featureSettingsOverride);

    auto mipContext = MipContext::Create(mipConfiguration);
    sample::ShutdownManager lifetimeManager(mipContext);

    const string protectionBaseUrl = "";
    const string policyBaseUrl = "";
    const string policyPath = "";
    const string enableFunctionality = "";
    const string disableFunctionality = "";

    auto profile = CreateProfile(mipContext, consentDelegate);   
    auto fileEngine = GetFileEngine(
        profile,
        authDelegate,
        username, // set for protect
        protectionBaseUrl,
        policyBaseUrl,
        policyPath, 
        false,
        false,
        false,
        false,
        true, // true for protect
        "en-US",
        enableFunctionality,
        disableFunctionality,
        false
    );
    
    DataState dataState = DataState::REST;
    string applicationScenarioId = "";
    auto fileHandler = GetFileHandler(fileEngine, fileStream, filePath, dataState, false, applicationScenarioId);

    EnsureUserHasRights(fileHandler);
    Unprotect(mipContext, fileHandler, fileStream, filePath, result);
    return EXIT_SUCCESS;
        
  }
  catch (const std::exception& ex) {    
    strcpy(result, getUnprotectStatusJSON(false, ex.what(), "").c_str());
    return EXIT_FAILURE;
  }
}


extern "C" int protectFile(const char* protectionToken_str, const char *filePath_str, const char* encryptedFilePath_str, const char* username_str, const char *applicationId_str, char *result)
{
  try {
    shared_ptr<mip::Stream> fileStream = nullptr;
    map<string, string> telemetrySettings;
    const string filePath(filePath_str);
    const string protectionToken(protectionToken_str);
    const string applicationId(applicationId_str);
    const string encryptedFilePath(encryptedFilePath_str);
    const string username(username_str);
    ApplicationInfo appInfo;
    appInfo.applicationId = applicationId;
    appInfo.applicationName = "MsipFileApp";
    appInfo.applicationVersion = "1.0.0.0";
    auto fileSampleWorkingDirectory = GetWorkingDirectory();

    const string password = "";
    const string sccToken = "";

    auto authDelegate = make_shared<AuthDelegateImpl>(false /*isVerbose*/, username, password, applicationId, sccToken, protectionToken, fileSampleWorkingDirectory);

    auto consentDelegate = make_shared<ConsentDelegateImpl>(false /*isVerbose*/);

    auto diagnosticOverride = make_shared<mip::DiagnosticConfiguration>();
    diagnosticOverride->customSettings = telemetrySettings;
    diagnosticOverride->isAuditPriorityEnhanced = true;
    diagnosticOverride->maxTeardownTimeSec = 2;
    diagnosticOverride->isMaxTeardownTimeEnabled = true;
    auto mipConfiguration = make_shared<mip::MipConfiguration>(appInfo, "file_sample_storage", mip::LogLevel::Trace, 0);
    mipConfiguration->SetDiagnosticConfiguration(diagnosticOverride);
    map<mip::FlightingFeature, bool> featureSettingsOverride;
    mipConfiguration->SetFeatureSettings(featureSettingsOverride);

    auto mipContext = MipContext::Create(mipConfiguration);
    sample::ShutdownManager lifetimeManager(mipContext);

    const string protectionBaseUrl = "";
    const string policyBaseUrl = "";
    const string policyPath = "";
    const string enableFunctionality = "";
    const string disableFunctionality = "";

    auto profile = CreateProfile(mipContext, consentDelegate);   
    auto fileEngine = GetFileEngine(
        profile,
        authDelegate,
        username, // set for protect
        protectionBaseUrl,
        policyBaseUrl,
        policyPath, 
        false,
        false,
        false,
        false,
        true, // true for protect
        "en-US",
        enableFunctionality,
        disableFunctionality,
        false
    );
    
    DataState dataState = DataState::REST;
    string applicationScenarioId = "";
    auto fileHandler = GetFileHandler(fileEngine, fileStream, filePath, dataState, false, applicationScenarioId);
    auto encFileHandler = GetFileHandler(fileEngine, fileStream, encryptedFilePath, dataState, false, applicationScenarioId);

    EnsureUserHasRights(fileHandler);
    ProtectWithCustomPermissions(
      fileHandler,
      encFileHandler, 
      result
    );
    return EXIT_SUCCESS;       
  }
  catch (const std::exception& ex) {    
    strcpy(result, getUnprotectStatusJSON(false, ex.what(), "").c_str());
    return EXIT_FAILURE;
  }
}  