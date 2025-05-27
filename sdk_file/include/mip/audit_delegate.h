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
 * @brief A file containing the AuditDelegate class to be used to override MIP audit
 * 
 * @file audit_delegate.h
 */

#ifndef API_MIP_AUDIT_DELEGATE_H_
#define API_MIP_AUDIT_DELEGATE_H_


#include "mip/audit_event.h"
#include "mip/diagnostic_delegate.h"
#include "mip/mip_namespace.h"

MIP_NAMESPACE_BEGIN

/**
 * @brief Audit Settings in policy.
 */
enum class EnableAuditSetting : unsigned int {
  Undefined    = 0, /* Audit has not been enabled or disabled */
  Enabled      = 1, /* Audit has been enabled */
  Disabled     = 2, /* Audit has been disabled */
};

/**
 * @brief A class that defines the interface to the MIP SDK audit notifications.
 */
class AuditDelegate : public DiagnosticDelegate<AuditEvent>{
public:

  /** @cond DOXYGEN_HIDE */
  
   /**
   * @brief Sets the audit settings from the policy.
   * 
   * @param auditSetting audit setting present in the policy.
   */
  virtual void SetEnableAuditSetting(const EnableAuditSetting /*auditSetting*/) {}

  virtual ~AuditDelegate() {}
protected:
  AuditDelegate() {}
   /** @endcond */
};

MIP_NAMESPACE_END

#endif // API_MIP_AUDIT_DELEGATE_H_
