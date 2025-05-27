#ifndef API_MIP_DYNAMIC_WATERMARK_CC_H_
#define API_MIP_DYNAMIC_WATERMARK_CC_H_

#include "mip_cc/common_types_cc.h"
#include "mip_cc/error_cc.h"

typedef mip_cc_handle* mip_cc_dynamic_watermark;

/**
 * @brief Gets size of buffer required to store format string
 * 
 * @param dynamicWatermark Dynamic watermark associated with protected content
 * @param formatStringSize [Output] Size of buffer to hold format string (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_DynamicWatermark_GetFormatStringSize(
    const mip_cc_dynamic_watermark dynamicWatermark,
    int64_t* formatStringSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the raw formatted string the admin has configured, which may include variables such as
 *        ${Consumer.PrincipalName}.
 *        This string can be populated from the label in the protection flow
 *        or it can be populated from the use license in the consumption flow.
 * 
 * @param dynamicWatermark Dynamic watermark associated with protected content
 * @param formatStringBuffer [Output] The raw format string configured by admin
 * @param formatStringBufferSize Size (in number of chars) of the formatStringBuffer
 * @param actualFormatStringSize [Output] Number of chars written to the buffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 *
 * @note If formatStringBuffer is null or of insufficient size, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER
 *       will be returned and actualFormatStringSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_DynamicWatermark_GetFormatString(
    const mip_cc_dynamic_watermark dynamicWatermark,
    char* formatStringBuffer,
    const int64_t formatStringBufferSize,
    int64_t* actualFormatStringSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets size of buffer required to store the string returned by \ref MIP_CC_DynamicWatermark_GetText
 * 
 * @param dynamicWatermark Dynamic watermark associated with protected content
 * @param textSize [Output] Size of buffer to hold the dynamic watermark text (in number of chars)
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 */
MIP_CC_API(mip_cc_result) MIP_CC_DynamicWatermark_GetTextSize(
    const mip_cc_dynamic_watermark dynamicWatermark,
    int64_t* textSize,
    mip_cc_error* errorInfo);

/**
 * @brief Gets the text which will be displayed to the end user by replacing the variables
 *        in the format string.
 * 
 * @param dynamicWatermark Dynamic watermark associated with protected content
 * @param textBuffer [Output] the filled in text for the dynamic watermark
 * @param textBufferSize Size (in number of chars) of the textBuffer
 * @param actualTextSize [Output] Number of chars written to the textBuffer
 * @param errorInfo [Output] (Optional) Failure info if operation result is error
 * 
 * @return Result code indicating success or failure
 *
 * @note If textBuffer is null or of insufficient size, MIP_RESULT_ERROR_INSUFFICIENT_BUFFER
 *       will be returned and actualTextSize will be set to the minimum required buffer size.
 */
MIP_CC_API(mip_cc_result) MIP_CC_DynamicWatermark_GetText(
    const mip_cc_dynamic_watermark dynamicWatermark,
    char* textBuffer,
    const int64_t textBufferSize,
    int64_t* actualTextSize,
    mip_cc_error* errorInfo);

/**
 * @brief Function to release a dynamic watermark handle
 * 
 * @param dynamicWatermark The dynamic watermark handle to release
 */
MIP_CC_API(void) MIP_CC_ReleaseDynamicWatermark(mip_cc_dynamic_watermark dynamicWatermark);

#endif  // API_MIP_DYNAMIC_WATERMARK_H_