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

#ifndef API_MIP_FILE_FILE_EXECUTION_STATE_H_
#define API_MIP_FILE_FILE_EXECUTION_STATE_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "mip/common_types.h"
#include "mip/file/file_handler.h"
#include "mip/mip_namespace.h"
#include "mip/upe/classification_request.h"
#include "mip/upe/classification_result.h"

MIP_NAMESPACE_BEGIN

class FileExecutionState {
public:
  /**
   * @brief Gets the state of the content while the application is interacting with it.
   *
   * @return State of the content data
   */
  virtual DataState GetDataState() const {
    return DataState::REST;
  }
  /**
   * @brief Return a map of classification results.
   *
   * @param fileHandler - the file handler of the used file
   * @param classificationIds a list of classification IDs.
   * @return a list of classification results.
   */
  virtual std::shared_ptr<ClassificationResults> GetClassificationResults(
      const std::shared_ptr<FileHandler>& /*fileHandler*/,
      const std::vector<std::shared_ptr<ClassificationRequest>>& /*classificationIds*/) const {
    return nullptr;
  }

  /**
   * @brief Return a map of application specific audit key-value pairs.
   *
   * @return a list of application specific audit metadata
   *
   * @note Registered Key:Value pairs
   * Sender: Email Id for the sender
   * Recipients: Represents a JSON array of recipients for an email
   * LastModifiedBy: Email Id for the user who last modified the content
   * LastModifiedDate: Date the content was last modified
   */
  virtual std::map<std::string, std::string> GetAuditMetadata() const { 
    return std::map<std::string, std::string>(); 
  }

  /**
   * @brief Return an identifier which correlates application events with the corresponding protection service REST requests.
   *
   * @return An identifier (usually specified as a GUID)
   */
  virtual const std::string GetApplicationScenarioId() const {
    return std::string();
  }

  /** @cond DOXYGEN_HIDE */
  virtual ~FileExecutionState() {}

protected:
  FileExecutionState() { }
  /** @endcond */
};

MIP_NAMESPACE_END

#endif  // API_MIP_FILE_FILE_EXECUTION_STATE_H_