#ifndef API_MIP_LICENSE_INFO_CC_H_
#define API_MIP_LICENSE_INFO_CC_H_

#include "mip_cc/common_types_cc.h"
#include "mip_cc/dynamic_watermark_cc.h"
#include "mip_cc/error_cc.h"
#include "mip_cc/string_list_cc.h"

typedef mip_cc_handle* mip_cc_license_info;

/**
 * @brief Gets the user rights from the license info
 * 
 * @param licenseInfo The licenseInfo from which to retrieve the user rights
 * @param rights [Output] The user rights present in the license info as a list of strings
 * @param errorInfo [Output] (Optional) Failiure info if operation results is failure
 * 
 * @return Result code indicating success or failure
 * 
 * @note The returned 'rights' (mip_cc_string_list*) must be freed by
 *       calling MIP_CC_ReleaseStringList
*/
MIP_CC_API(mip_cc_result) MIP_CC_LicenseInfo_GetUserRights(
    const mip_cc_license_info licenseInfo,
    mip_cc_string_list* rights,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the dynamic watermark
 * 
 * @param licenseInfo The licenseInfo from which to retrieve the dynamic watermark
 * @param dynamicWatermark [Output] The dynamic watermark in the licenseInfo (null if not present)
 * @param errorInfo [Output] (Optional) Failiure info if operation results is failure
 * 
 * @return Result code indicating success or failure
 * 
 * @note The returned 'dynamicWatermark' (mip_cc_dynamic_watermark*) must be freed 
 *       calling MIP_CC_ReleaseDynamicWatermark
*/
MIP_CC_API(mip_cc_result) MIP_CC_LicenseInfo_GetDynamicWatermark( 
    const mip_cc_license_info licenseInfo, 
    mip_cc_dynamic_watermark* dynamicWatermark, 
    mip_cc_error* errorInfo); 

/**
 * @brief Function to release a license info handle
 * @param licenseInfo the license info being released
*/
MIP_CC_API(void) MIP_CC_ReleaseLicenseInfo(mip_cc_license_info licenseInfo);

#endif