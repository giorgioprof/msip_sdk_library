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
 * @brief Export/import and other macros for C API
 * 
 * @file mip_macros_cc.h
 */

#ifndef API_MIP_MIP_MACROS_CC_H_
#define API_MIP_MIP_MACROS_CC_H_

/** @cond DOXYGEN_HIDE */
#ifdef __cplusplus
#  define MIP_CC_EXTERN_C extern "C"
#else
#  define MIP_CC_EXTERN_C
#endif // __cplusplus

#ifdef _WIN32
#  define MIP_CC_CALLBACK(TName, TRet, ...) typedef TRet (__cdecl *TName)(__VA_ARGS__)
#  define MIP_CC_CALLBACK_IMPL(TRet) TRet __cdecl
#  ifdef MIP_IMPLEMENTATION
#    define MIP_CC_API(TRet) MIP_CC_EXTERN_C __declspec(dllexport) TRet __cdecl
#    define MIP_CC_DEPRECATED_API(TRet) MIP_CC_EXTERN_C __declspec(deprecated dllexport) TRet __cdecl
#  elif defined(NO_MIP_IMPLEMENTATION)
#    define MIP_CC_API(TRet) MIP_CC_EXTERN_C TRet
#    define MIP_CC_DEPRECATED_API(TRet) MIP_CC_EXTERN_C __declspec(deprecated) TRet
#  else
#    define MIP_CC_API(TRet) MIP_CC_EXTERN_C __declspec(dllimport) TRet __cdecl
#    define MIP_CC_DEPRECATED_API(TRet) MIP_CC_EXTERN_C __declspec(deprecated dllimport) TRet __cdecl
#  endif
#else
#  define MIP_CC_CALLBACK(TName, TRet, ...) typedef TRet (*TName)(__VA_ARGS__)
#  define MIP_CC_CALLBACK_IMPL(TRet) TRet
#  ifdef MIP_IMPLEMENTATION
#    define MIP_CC_API(TRet) MIP_CC_EXTERN_C __attribute__((visibility("default"))) TRet
#    define MIP_CC_DEPRECATED_API(TRet) MIP_CC_EXTERN_C __attribute__((__deprecated__, visibility("default"))) TRet
#  else
#    define MIP_CC_API(TRet) MIP_CC_EXTERN_C TRet
#    define MIP_CC_DEPRECATED_API(TRet) MIP_CC_EXTERN_C __attribute__((__deprecated__)) TRet
#  endif
#endif // _WIN32



/** @endcond */

#endif  // API_MIP_MIP_MACROS_CC_H_
