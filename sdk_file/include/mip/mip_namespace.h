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
 * @brief MIP namespace macros.
 * 
 * @file mip_namespace.h
 */

#ifndef API_MIP_MIP_NAMESPACE_H_
#define API_MIP_MIP_NAMESPACE_H_

/** @cond DOXYGEN_HIDE */
// To build with a custom namespace, override these macros as compiler flags
//   MIP_DEPENDENCY_NAMESPACE_PREFIX - Example:  "foo"
//   MIP_NAMESPACE_PREFIX            - Examples: "foo" or "foo::bar"
//   MIP_NAMESPACE_BEGIN             - Examples: "namespace foo {" or "namespace foo { namespace bar {"
//   MIP_NAMESPACE_END               - Examples: "}" or "}}"

// Namespace of internal static dependencies (e.g. easylogging++, rapidjson)
#ifndef MIP_DEPENDENCY_NAMESPACE_PREFIX
#  define MIP_DEPENDENCY_NAMESPACE_PREFIX mipns
#endif

// Namespace of MIP components
#ifndef MIP_NAMESPACE_PREFIX
#  define MIP_NAMESPACE_PREFIX mipns
#endif
#ifndef MIP_NAMESPACE_BEGIN
#  define MIP_NAMESPACE_BEGIN namespace mipns {
#endif
#ifndef MIP_NAMESPACE_END
#  define MIP_NAMESPACE_END }
#endif

MIP_NAMESPACE_BEGIN
MIP_NAMESPACE_END

namespace mip = MIP_NAMESPACE_PREFIX;

#ifndef MIP_OFFLINE_PUBLISHING_CONFIGURED
#  if !((defined(TARGET_OS_IOS) && (TARGET_OS_IOS==1)) || defined(__ANDROID__))
#    define MIP_OFFLINE_PUBLISHING_ENABLED
#  endif // !((defined(TARGET_OS_IOS) && (TARGET_OS_IOS==1)) || defined(__ANDROID__))
#endif
/** @endcond */

#endif  // API_MIP_MIP_NAMESPACE_H_
