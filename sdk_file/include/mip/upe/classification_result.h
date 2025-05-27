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
 * @brief This file contains the ClassificationResult class.
 *
 * @file classification_result.h
 */

#ifndef API_MIP_UPE_CLASSIFICATION_RESULT_H_
#define API_MIP_UPE_CLASSIFICATION_RESULT_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "mip/mip_namespace.h"
#include "mip/upe/detailed_classification_result.h"

MIP_NAMESPACE_BEGIN
/**
 * @brief Class that contains the result of a classification call on the Execution State
 */
class ClassificationResult {
public:
  /**
   * @brief Get the ID of the classification policy.
   *
   * @return ID of the classification policy.
   */
  virtual std::string GetId() const = 0;
  /**
   * @brief Get the name of the classification policy.
   *
   * @return name of the classification policy.
   */
  virtual std::string GetName() const = 0;
  /**  
   * @brief Get the instance count.
   *
   * @return the instance count.
   */
  virtual int GetCount() const = 0;
  /**
   * @brief Get the confidence in the result.
   */
  virtual int GetConfidenceLevel() const = 0;
  /**
   * @brief Get the sensitive information detections.
   *
   * @return json string of all the sensitive information detections. if not empty must be a valid json format.
   */
  virtual std::string GetSensitiveInformationDetections() const = 0;
  /**
   * @brief Get the specific detection bands if the enchanced classification is enabled.
   *
   * @return a vector of instance counts at different confidence thresholds
   */
  virtual std::vector<std::shared_ptr<mip::DetailedClassificationResult>> GetDetailedClassificationAttributes() const {
    return std::vector<std::shared_ptr<mip::DetailedClassificationResult>>();
  }

  /** @cond DOXYGEN_HIDE */
  virtual ~ClassificationResult() {}

protected:
  ClassificationResult() {}

  /** @endcond */
};

/**
 * @brief A container that contains the results of a classification call on the Execution State
 */
using ClassificationResults = std::map<std::string, std::shared_ptr<ClassificationResult>>;

MIP_NAMESPACE_END

#endif  // API_MIP_UPE_CLASSIFICATION_RESULT_H_
