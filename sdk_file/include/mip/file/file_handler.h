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

#ifndef API_MIP_FILE_FILE_HANDLER_H_
#define API_MIP_FILE_FILE_HANDLER_H_

#include <memory>

#include "mip/file/file_export.h"
#include "mip/file/file_inspector.h"
#include "mip/file/file_status.h"
#include "mip/file/labeling_options.h"
#include "mip/file/protection_settings.h"
#include "mip/mip_context.h"
#include "mip/mip_namespace.h"
#include "mip/protection_descriptor.h"
#include "mip/protection/protection_handler.h"
#include "mip/stream.h"
#include "mip/upe/action.h"
#include "mip/upe/content_label.h"
#include "mip/protection/protection_engine.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief Interface for all file handling functions.
 */
class FileHandler {
public:

  /**
   * @brief Observer interface for clients to get notifications events related to file handler.
   * 
   * @note All errors inherit from mip::Error.
   * @note Client should not call the engine back on the thread that calls the observer.
   */
  class Observer {
  public:
  /** @cond DOXYGEN_HIDE */
    virtual ~Observer() {}
  /** @endcond */

    /**
     * @brief Called when the handler is created successfully.
     */
    virtual void OnCreateFileHandlerSuccess(
      const std::shared_ptr<FileHandler>& fileHandler,
      const std::shared_ptr<void>& context) { UNUSED(fileHandler); UNUSED(context); }

    /**
     * @brief Called when creating the handler failed.
     */
    virtual void OnCreateFileHandlerFailure(
      const std::exception_ptr& error,
      const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when classify success.
     */
    virtual void OnClassifySuccess(
        const std::vector<std::shared_ptr<Action>>& actions, const std::shared_ptr<void>& context) {
      UNUSED(actions);
      UNUSED(context);
    }

    /**
     * @brief Called when classify failed.
     */
    virtual void OnClassifyFailure(const std::exception_ptr& error, const std::shared_ptr<void>& context) {
      UNUSED(error);
      UNUSED(context);
    }

    /**
     * @brief Called when getting the decrypted temporary file success.
     */
    virtual void OnGetDecryptedTemporaryFileSuccess(
        const std::string& decryptedFilePath, const std::shared_ptr<void>& context) {
      UNUSED(decryptedFilePath);
      UNUSED(context);
    }

    /**
     * @brief Called when getting the decrypted temporary file failed.
     */
    virtual void OnGetDecryptedTemporaryFileFailure(const std::exception_ptr& error, const std::shared_ptr<void>& context) {
      UNUSED(error);
      UNUSED(context);
    }

    /**
     * @brief Called when getting the decrypted temporary stream success.
     */
    virtual void OnGetDecryptedTemporaryStreamSuccess(
        const std::shared_ptr<Stream>& decryptedStream, const std::shared_ptr<void>& context) {
      UNUSED(decryptedStream);
      UNUSED(context);
    }

    /**
     * @brief Called when getting the decrypted temporary stream failed.
     */
    virtual void OnGetDecryptedTemporaryStreamFailure(
        const std::exception_ptr& error, const std::shared_ptr<void>& context) {
      UNUSED(error);
      UNUSED(context);
    }

    /**
     * @brief Called when committing the changes to the file were successful.
     */
    virtual void OnCommitSuccess(
      bool committed, 
      const std::shared_ptr<void>& context) { UNUSED(committed); UNUSED(context); }

    /**
     * @brief Called when committing the changes to the file failed.
     */
    virtual void OnCommitFailure(
      const std::exception_ptr& error, 
      const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when inspect success.
     */
    virtual void OnInspectSuccess(
        const std::shared_ptr<FileInspector>& fileInspector, const std::shared_ptr<void>& context) {
      UNUSED(fileInspector);
      UNUSED(context);
    }

    /**
     * @brief Called when inspect failed.
     */
    virtual void OnInspectFailure(const std::exception_ptr& error, const std::shared_ptr<void>& context) {
      UNUSED(error);
      UNUSED(context);
    }

  protected:
  /** @cond DOXYGEN_HIDE */
    Observer() {}
    /** @endcond */    
  };

  /**
   * @brief Starts retrieving the sensitivity label from the file.
   * @throw NotSupportedError when a protection only engine is in use and no policy handler is found.
   */
  virtual std::shared_ptr<ContentLabel> GetLabel() = 0;

  /**
   * @brief Retrieves the file properties according to version.
   */
  virtual std::vector<std::pair<std::string, std::string>> GetProperties(uint32_t version) = 0;

  /**
   * @brief Starts retrieving the protection policy from the file.
   */
  virtual std::shared_ptr<ProtectionHandler> GetProtection() = 0;

 /**
   * @brief Register the content for tracking and revocation
   *
   * @param isOwnerNotificationEnabled Set to true to notify the owner via email whenever the document is decrypted, or
   * false to not send the notification.
   * @param observer A class implementing the ProtectionHandler::Observer interface
   * @param context Client context that will be opaquely forwarded to observers and optional HttpDelegate
   *
   * @return Async control object.
   */
  virtual void RegisterContentForTrackingAndRevocationAsync(
      bool isOwnerNotificationEnabled,
      const std::shared_ptr<ProtectionEngine::Observer>& observer,
      const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Perform revocation for the content
   *
   * @param observer A class implementing the ProtectionHandler::Observer interface
   * @param context Client context that will be opaquely forwarded to observers and optional HttpDelegate
   *
   * @return Async control object.
   */
  virtual void RevokeContentAsync(
      const std::shared_ptr<ProtectionEngine::Observer>& observer, const std::shared_ptr<void>& context) = 0;
  
  /**
   * @brief Checks whether a file is protected or not.
   * 
   * @param filePath Path of file to check, which must include the file name and, 
   *        if one exists, the file extension. The file extension is used to determine the file format.
   * @param mipContext Global MIP context
   * 
   * @return True if file is protected, else false
   */
  FILE_API static bool __CDECL IsProtected(
      const std::string& filePath,
      const std::shared_ptr<MipContext>& mipContext);

  /**
   * @brief Checks whether a file is protected or not.
   * 
   * @param filePath Path of file to check, which must include the file name and, 
   *        if one exists, the file extension. The file extension is used to determine the file format.
   * @param mipContext Global MIP context
   * @param loggerContext Logger context that will be opaquely passed to the logger delegate
   * 
   * @return True if file is protected, else false
   */
  FILE_API static bool __CDECL IsProtected(
      const std::string& filePath,
      const std::shared_ptr<MipContext>& mipContext,
      const std::shared_ptr<void>& loggerContext);
    
  /**
   * @brief Checks whether a file is protected or not.
   * 
   * @param stream Stream containing file data to check
   * @param filePath File path associated with data in 'stream', which must include the file name and,
   *        if one exists, the file extension. The file extension is used to determine the file format.
   *        Passing in the wrong file extension will cause unexpected behavior.      
   * @param mipContext Global MIP context
   * 
   * @return True if file is protected, else false
   */
  FILE_API static bool __CDECL IsProtected(
      const std::shared_ptr<Stream>& stream,
      const std::string& filePath,
      const std::shared_ptr<MipContext>& mipContext);

  /**
   * @brief Checks whether a file is protected or not.
   * 
   * @param stream Stream containing file data to check
   * @param filePath File path associated with data in 'stream', which must include the file name and,
   *        if one exists, the file extension. The file extension is used to determine the file format.
   *        Passing in the wrong file extension will cause unexpected behavior.    
   * @param mipContext Global MIP context
   * @param loggerContext Logger context that will be opaquely passed to the logger delegate
   * 
   * @return True if file is protected, else false
   */
  FILE_API static bool __CDECL IsProtected(
      const std::shared_ptr<Stream>& stream,
      const std::string& filePath,
      const std::shared_ptr<MipContext>& mipContext,
      const std::shared_ptr<void>& loggerContext);

  /**
   * @brief Checks whether a file is labeled/protected or not.
   * 
   * @param filePath Path of file to check, which must include the file name and, 
   *        if one exists, the file extension. The file extension is used to determine the file format.
   * @param mipContext Global MIP context
   * 
   * @return True if file is protected or contains label metadata, else false
   * 
   * @note This will only detect Microsoft labels. It will not detect 3rd-party labels, even if a tenant is configured
   *       to translate 3rd-party label metadata to Microsoft labels. The primary purpose of this API is to allow an
   *       application to quickly detect labeled/protected content without any HTTP calls. An HTTP call is required to 
   *       retrieve the tenant-specific label mapping that is necessary for detecting 3rd-part labels.
   */
  FILE_API static bool __CDECL IsLabeledOrProtected(
      const std::string& filePath,
      const std::shared_ptr<MipContext>& mipContext);

  /**
   * @brief Checks whether a file is labeled/protected or not.
   * 
   * @param filePath Path of file to check, which must include the file name and, 
   *        if one exists, the file extension. The file extension is used to determine the file format.
   * @param mipContext Global MIP context
   * @param loggerContext Logger context that will be opaquely passed to the logger delegate
   * 
   * @return True if file is protected or contains label metadata, else false
   * 
   * @note This will only detect Microsoft labels. It will not detect 3rd-party labels, even if a tenant is configured
   *       to translate 3rd-party label metadata to Microsoft labels. The primary purpose of this API is to allow an
   *       application to quickly detect labeled/protected content without any HTTP calls. An HTTP call is required to 
   *       retrieve the tenant-specific label mapping that is necessary for detecting 3rd-part labels.
   */
  FILE_API static bool __CDECL IsLabeledOrProtected(
      const std::string& filePath,
      const std::shared_ptr<MipContext>& mipContext,
      const std::shared_ptr<void>& loggerContext);
    
  /**
   * @brief Checks whether a file is labeled/protected or not.
   * 
   * @param stream Stream containing file data to check
   * @param filePath File path associated with data in 'stream', which must include the file name and,
   *        if one exists, the file extension. The file extension is used to determine the file format.
   *        Passing in the wrong file extension will cause unexpected behavior.   
   * @param mipContext Global MIP context
   * 
   * @return True if file is protected or contains label metadata, else false
   * 
   * @note This will only detect Microsoft labels. It will not detect 3rd-party labels, even if a tenant is configured
   *       to translate 3rd-party label metadata to Microsoft labels. The primary purpose of this API is to allow an
   *       application to quickly detect labeled/protected content without any HTTP calls. An HTTP call is required to 
   *       retrieve the tenant-specific label mapping that is necessary for detecting 3rd-part labels.
   */
  FILE_API static bool __CDECL IsLabeledOrProtected(
      const std::shared_ptr<Stream>& stream,
      const std::string& filePath,
      const std::shared_ptr<MipContext>& mipContext);

  /**
   * @brief Checks whether a file is labeled/protected or not.
   * 
   * @param stream Stream containing file data to check
   * @param filePath File path associated with data in 'stream', which must include the file name and,
   *        if one exists, the file extension. The file extension is used to determine the file format.
   *        Passing in the wrong file extension will cause unexpected behavior.   
   * @param mipContext Global MIP context
   * @param loggerContext Logger context that will be opaquely passed to the logger delegate
   * 
   * @return True if file is protected or contains label metadata, else false
   * 
   * @note This will only detect Microsoft labels. It will not detect 3rd-party labels, even if a tenant is configured
   *       to translate 3rd-party label metadata to Microsoft labels. The primary purpose of this API is to allow an
   *       application to quickly detect labeled/protected content without any HTTP calls. An HTTP call is required to 
   *       retrieve the tenant-specific label mapping that is necessary for detecting 3rd-part labels.
   */
  FILE_API static bool __CDECL IsLabeledOrProtected(
      const std::shared_ptr<Stream>& stream,
      const std::string& filePath,
      const std::shared_ptr<MipContext>& mipContext,
      const std::shared_ptr<void>& loggerContext);

  /**
   * @brief Checks whether a file is labeled, protected, or contains protected objects.
   * 
   * @param filePath Path of file to check, which must include the file name and, 
   *        if one exists, the file extension. The file extension is used to determine the file format.
   * @param mipContext Global MIP context
   * @param loggerContext Logger context that will be opaquely passed to the logger delegate
   * 
   * @return The shared pointer of a FileStatus object. 
   * 
   * @note This will only detect Microsoft labels. It will not detect 3rd-party labels, even if a tenant is configured
   *       to translate 3rd-party label metadata to Microsoft labels. The primary purpose of this API is to allow an
   *       application to quickly detect labeled/protected content without any HTTP calls. An HTTP call is required to 
   *       retrieve the tenant-specific label mapping that is necessary for detecting 3rd-part labels.
   *       For container files like msg, this method will only detect if an unprotected container file contains protected 
   *       non-container objects.
   */
  FILE_API static std::shared_ptr<FileStatus> __CDECL GetFileStatus(
      const std::string& filePath,
      const std::shared_ptr<MipContext>& mipContext,
      const std::shared_ptr<void>& loggerContext = nullptr);

  /**
   * @brief Checks whether a file is labeled, protected, or contains protected objects.
   * 
   * @param stream Stream containing file data to check
   * @param filePath File path associated with data in 'stream', which must include the file name and,
   *        if one exists, the file extension. The file extension is used to determine the file format.
   *        Passing in the wrong file extension will cause unexpected behavior.
   * @param mipContext Global MIP context
   * @param loggerContext Logger context that will be opaquely passed to the logger delegate
   * 
   * @return The shared pointer of a FileStatus object. 
   * 
   * @note This will only detect Microsoft labels. It will not detect 3rd-party labels, even if a tenant is configured
   *       to translate 3rd-party label metadata to Microsoft labels. The primary purpose of this API is to allow an
   *       application to quickly detect labeled/protected content without any HTTP calls. An HTTP call is required to 
   *       retrieve the tenant-specific label mapping that is necessary for detecting 3rd-part labels.
   *       For container files like msg, this method will only detect if an unprotected container file contains protected 
   *       non-container objects.
   */
  FILE_API static std::shared_ptr<FileStatus> __CDECL GetFileStatus(
      const std::shared_ptr<Stream>& stream,
      const std::string& filePath,
      const std::shared_ptr<MipContext>& mipContext,
      const std::shared_ptr<void>& loggerContext = nullptr);

  /**
   * @brief Get publishing license from the file
   * @param filePath Path of file to open, which must include the file name and, 
   *        if one exists, the file extension. The file extension is used to determine the file format.
   * @param mipContext Global MIP context
   * @return Publishing License if file has it.
   */
  FILE_API static std::vector<uint8_t> __CDECL GetSerializedPublishingLicense(
      const std::string& filePath,
      const std::shared_ptr<MipContext>& mipContext);

  /**
   * @brief Get publishing license from the file
   * @param filePath Path of file to open, which must include the file name and, 
   *        if one exists, the file extension. The file extension is used to determine the file format.
   * @param mipContext Global MIP context
   * @param loggerContext Logger context that will be opaquely passed to the logger delegate
   * @return Publishing License if file has it.
   */
  FILE_API static std::vector<uint8_t> __CDECL GetSerializedPublishingLicense(
      const std::string& filePath,
      const std::shared_ptr<MipContext>& mipContext,
      const std::shared_ptr<void>& loggerContext);

  /**
   * @brief Get publishing license from the file
   * @param stream Stream containing file data to check
   * @param filePath File path associated with data in 'stream', which must include the file name and,
   *        if one exists, the file extension. The file extension is used to determine the file format.
   *        Passing in the wrong file extension will cause unexpected behavior.
   * @param mipContext Global MIP context
   * @return Publishing License if file has it.
   */
  FILE_API static std::vector<uint8_t> __CDECL GetSerializedPublishingLicense(
      const std::shared_ptr<Stream>& stream,
      const std::string& filePath,
      const std::shared_ptr<MipContext>& mipContext);

  /**
   * @brief Get publishing license from the file
   * @param stream Stream containing file data to check
   * @param filePath File path associated with data in 'stream', which must include the file name and,
   *        if one exists, the file extension. The file extension is used to determine the file format.
   *        Passing in the wrong file extension will cause unexpected behavior.
   * @param mipContext Global MIP context
   * @param loggerContext Logger context that will be opaquely passed to the logger delegate
   * @return Publishing License if file has it.
   */
  FILE_API static std::vector<uint8_t> __CDECL GetSerializedPublishingLicense(
      const std::shared_ptr<Stream>& stream,
      const std::string& filePath,
      const std::shared_ptr<MipContext>& mipContext,
      const std::shared_ptr<void>& loggerContext);

   /**
   * @brief Executes the rules in the handler and returns the list of actions
   * to be executed.
   *
   * @return list of actions that should be applied on the content.
   * @throw NotSupportedError when a protection only engine is in use and no policy handler is found.
   */
  virtual void ClassifyAsync(const std::shared_ptr<void>& context) = 0;

   /**
   * @brief Create a file inspector object, used to retrieve file contents from compatible file formats.
   *
   * @return a file inspector.
   */
  virtual void InspectAsync(const std::shared_ptr<void>& context) = 0;
  
  /**
   * @brief Sets the sensitivity label to the file.
   * 
   * @note Changes won't be written to the file until CommitAsync is called.
   *       Privileged and Auto method allows the API to override any existing label
   * @warning Throws JustificationRequiredError when setting the label requires 
   *          the operation to be justified (via the labelingOptions parameter).
   * @throw NotSupportedError when a protection only engine is in use and no policy handler is found.
   */
  virtual void SetLabel(
      const std::shared_ptr<Label>& label,
      const LabelingOptions& labelingOptions,
      const ProtectionSettings& protectionSettings) = 0;

  /**
  * @brief Deletes the sensitivity label from the file.
  * 
  * @note Changes won't be written to the file until CommitAsync is called.
  *       Privileged and Auto method allows the API to override any existing label
  * @warning Throws JustificationRequiredError when setting the label requires the 
  *          operation to be justified (via the labelingOptions parameter).
  * @throw NotSupportedError when a protection only engine is in use and no policy handler is found.
  */
  virtual void DeleteLabel(const LabelingOptions& labelingOptions) = 0;

  /**
  * @brief Sets either custom or template-based permissions 
  *        (according to protectionDescriptor->GetProtectionType) to the file.
  * 
  * @note Changes won't be written to the file until CommitAsync is called.
  */
  virtual void SetProtection(
      const std::shared_ptr<ProtectionDescriptor>& protectionDescriptor,
      const ProtectionSettings& protectionSettings) = 0;

  /**
  * @brief Sets protection on a document using an existing protection handler.
  * 
  * @note Changes won't be written to the file until CommitAsync is called.
  */
  virtual void SetProtection(
      const std::shared_ptr<ProtectionHandler>& protectionHandler) = 0;

  /**
   * @brief Removes protection from the file. 
   * If the original file format does not support labeling, 
   *   the label will be lost when protection is removed. 
   * When the native format supports labeling, the label metadata is maintained.
   * 
   * @note Changes won't be written to the file until CommitAsync is called.
   */
  virtual void RemoveProtection() = 0;

  /**
   * @brief Writes the changes to the file specified by the |outputFilePath| parameter.
   * 
   * @note FileHandler::Observer will be called upon success or failure.
   * 
   * @warning If the output file's disk space is insufficient, the FileHandler::CommitAsync function may generate corrupted files. 
   * In this case, delete the corrupted files and either adjust the disk size or choose another disk before retrying the operation.
   */
  virtual void CommitAsync(const std::string& outputFilePath, const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Writes the changes to the stream specified by the |outputStream| parameter.
   * 
   * @note |outputStream| must not be the same as inputStream used for creating the handler.
   * 
   * @note FileHandler::Observer will be called upon success or failure.
   * 
   * @warning If the output file's disk space is insufficient, the FileHandler::CommitAsync function may generate corrupted files. 
   * In this case, delete the corrupted files and either adjust the disk size or choose another disk before retrying the operation.
   */
  virtual void CommitAsync(const std::shared_ptr<Stream>& outputStream, const std::shared_ptr<void>& context) = 0;

  /**		
   * @brief Checks if there are changes to commit to the file.		
   * @note Changes won't be written to the file until CommitAsync is called.		
   */		
  virtual bool IsModified() = 0;		
  
  /**
   * @brief Returns a path to a temporary file (that will be deleted if possible) - representing the decrypted content.
   *
   * @note FileHandler::Observer will be called upon success or failure.
   */
  virtual void GetDecryptedTemporaryFileAsync(const std::shared_ptr<void>& context) = 0;

  /**
  * @brief Returns a stream - representing the decrypted content.
  *
  * @note FileHandler::Observer will be called upon success or failure.
  */
  virtual void GetDecryptedTemporaryStreamAsync(const std::shared_ptr<void>& context) = 0;

 /**
   * @brief To be called when the changes have been committed to disk
   * @param actualFilePath The actual file path for the output file
   * @note Fires an Audit event
   * @throw NotSupportedError when a protection only engine is in use and no policy handler is found.
   */
  virtual void NotifyCommitSuccessful(const std::string& actualFilePath) = 0;

  /**
   * @brief Calculates the output file name and extension based on the original file name and the accumulated changes.
   */
  virtual std::string GetOutputFileName() = 0;

/** @cond DOXYGEN_HIDE */
  virtual ~FileHandler() {}
  /** @endcond */

protected:
/** @cond DOXYGEN_HIDE */
  FileHandler() {}
  /** @endcond */
};

MIP_NAMESPACE_END

#endif // API_MIP_FILE_FILE_HANDLER_H_
