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
 * @brief Defines StorageTable interface
 * 
 * @file storage_table.h
 */

#ifndef API_MIP_STORAGE_TABLE_H_
#define API_MIP_STORAGE_TABLE_H_

#include <string>
#include <vector>

#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief A class that defines the interface to the MIP SDK storage table used for caching
 */
class StorageTable {
public:
  /**
   * @brief Adds a row to the table
   * 
   * @param allColumnValues All column values in sequence as represented in storage table.
   *
   * @deprecated This method will soon be deprecated in favor of InsertOrReplace. If the new version
   * has been implemented, there is no need to implement this version.
   */
#if !defined(SWIG) && !defined(SWIG_DIRECTORS)
  [[deprecated("Insert method is deprecated, instead use InsertOrReplace method")]]
#endif
  virtual void Insert(const std::vector<std::string>& allColumnValues)
  {
    InsertOrReplace(allColumnValues);
  }

  /**
   * @brief Adds a row to the table
   *
   * @param allColumnValues All column values in sequence as represented in storage table.
   */
  virtual void InsertOrReplace(const std::vector<std::string>& allColumnValues) = 0;

  /**
   * @brief List all the rows in the table
   * 
   * @return List of all rows where each row is a vector of column values
  */
  virtual std::vector<std::vector<std::string>> List() = 0;

  /**
   * @brief Update a set of rows within the table.
   * 
   * @param updateColumns List of column names that define the new row data.
   * @param updateValues List of column values corresponding to @p updateColumns.
   * @param queryColumns List of column names that identify the table rows to update.
   * @param queryValues List of column values corresponding to @p queryColumns.
   * 
   */
  virtual void Update(
      const std::vector<std::string>& updateColumns,
      const std::vector<std::string>& updateValues,
      const std::vector<std::string>& queryColumns,
      const std::vector<std::string>& queryValues) = 0;

  /**
   * @brief Delete a set of rows specified by queryColumns and queryValues
   *
   * @param queryColumns List of column names that identify the table rows to delete.
   * @param queryValues List of column values corresponding to @p queryColumns.
   *
   */
  virtual void Delete(
      const std::vector<std::string>& queryColumns,
      const std::vector<std::string>& queryValues) = 0;

  /**
   * @brief Find a set of rows specified by @p queryColumns and @p queryValues. This method can return multiple rows based on the filter.
   * 
   * @param queryColumns List of column names that identify the table rows to find.
   * @param queryValues List of column values corresponding to @p queryColumns.
   * 
   * @return List of all rows where each row is a list containing column values
   */
  virtual std::vector<std::vector<std::string>> Find(
      const std::vector<std::string>& queryColumns,
      const std::vector<std::string>& queryValues) = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~StorageTable() {}
protected:
  StorageTable() {}
   /** @endcond */
};

MIP_NAMESPACE_END
#endif // API_MIP_STORAGE_TABLE_H_