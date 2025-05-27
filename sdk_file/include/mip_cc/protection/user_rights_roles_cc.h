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
 * @brief Defines C-Style user rights & roles functions
 * 
 * @file user_rights_roles_cc.h
 */

#ifndef API_MIP_PROTECTION_USER_RIGHTS_ROLES_CC_H_
#define API_MIP_PROTECTION_USER_RIGHTS_ROLES_CC_H_

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/mip_macros_cc.h"
#include "mip_cc/protection_descriptor_cc.h"
#include "mip_cc/result_cc.h"
#include "mip_cc/string_list_cc.h"

/**
 * @brief Gets the list of users from a single users-to-rights pair.
 * This API is identical to calling protectionDescriptor->GetUserRights()[index].Users() on the C++ side.
 *
 * @param userRights The users-to-rights list to access
 * @param index Which users-to-rights pair to retrieve the list of users from
 * @param users [Output] The retrieved users list. It is the caller's responsibility to release this object with MIP_CC_ReleaseStringList
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 */
MIP_CC_API(mip_cc_result) MIP_CC_UserRights_GetUsers(
    const mip_cc_user_rights_result userRights,
    uint32_t index,
    mip_cc_string_list* users,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the list of rights from a single users-to-rights pair.
 * This API is identical to calling protectionDescriptor->GetUserRights()[index].Rights() on the C++ side.
 *
 * @param userRights The users-to-rights list to access
 * @param index Which users-to-rights pair to retrieve the list of rights from
 * @param rights [Output] The retrieved rights list. It is the caller's responsibility to release this object with MIP_CC_ReleaseStringList
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 */
MIP_CC_API(mip_cc_result) MIP_CC_UserRights_GetRights(
    const mip_cc_user_rights_result userRights,
    uint32_t index,
    mip_cc_string_list* rights,
    mip_cc_error* errorInfo);

/**
 * @brief Release resources associated with a user rights list
 * 
 * @param userRightsList User rights list to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseUserRightsResult(mip_cc_user_rights_result userRightsList);

/**
 * @brief Gets the list of users from a single users-to-roles pair.
 * This API is identical to calling protectionDescriptor->GetUserRoles()[index].Users() on the C++ side.
 *
 * @param userRoles The users-to-roles list to access
 * @param index Which users-to-roles pair to retrieve the list of users from
 * @param users [Output] The retrieved users list. It is the caller's responsibility to release this object with MIP_CC_ReleaseStringList
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 */
MIP_CC_API(mip_cc_result) MIP_CC_UserRoles_GetUsers(
    const mip_cc_user_roles_result userRoles,
    uint32_t index,
    mip_cc_string_list* users,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the list of roles from a single users-to-roles pair.
 * This API is identical to calling protectionDescriptor->GetUserRoles()[index].Roles() on the C++ side.
 *
 * @param userRoles The users-to-roles list to access
 * @param index Which users-to-roles pair to retrieve the list of roles from
 * @param roles [Output] The retrieved roles list. It is the caller's responsibility to release this object with MIP_CC_ReleaseStringList
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 */
MIP_CC_API(mip_cc_result) MIP_CC_UserRoles_GetRoles(
    const mip_cc_user_roles_result userRoles,
    uint32_t index,
    mip_cc_string_list* roles,
    mip_cc_error* errorInfo);

/**
 * @brief Release resources associated with a user roles list
 *
 * @param userRightsList User roles list to be released
 */
MIP_CC_API(void) MIP_CC_ReleaseUserRolesResult(mip_cc_user_roles_result userRoleList);

#endif // API_MIP_PROTECTION_USER_RIGHTS_ROLES_CC_H_