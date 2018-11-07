/**
Copyright:      1999-2013, iFLYTEK Co., Ltd.
File name:      iss_auth.h
Description:    iFLYTEK Speech Suite - Authorization.
                Brief Description:
                ISS  : iFLYTEK Speech Suite;
                auth : Authorization;
Author:         iFLYTEK
Version:        1.0
Date:           2013-12-04
History:        Created on Dec. 4, 2013
*/

/** @defgroup group11 iFLYTEK Speech Suite - Authorization
 
 * @brief iss_auth.h
 
 */

#ifndef __ISS_AUTH_H__
#define __ISS_AUTH_H__

#include "iss_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \brief  The machine code inputted; \n
            This interface shall be called at least once respectively, before the ::ISSGetActiveKey is called to get the activation code, the ::ISSActivate is called to make activation, 
            as well as the ::ISSSRCreate is called to create a recognition handle. \n
            The function is a synchronous, non-reentrant and thread-safe one.
            For each client, the machine code entered must be different, and the length of the machine code shall not exceed 1024. \n
			Up to 1000 times recognition can be made by each machine every day when it accesses to the cloud.\n
			The machine code must be unique, to identify the personalized resources.\n
 * \param   szMachineCode   [In] Machine code\n
 * \retval  ISSErrID                The error code of the speech suite license activation \n
 * \retval  ISS_SUCCESS             Enter the machine code successfully.\n
 * \retval  ISS_ERROR_INVALID_PARA  Invalid parameter value\n
 * \retval  ISS_ERROR_FAIL          Fails to enter the machine code.\n
 */
ISSErrID ISSAPI ISSSetMachineCode(
    const char* szMachineCode      /* [In] Machine code. */
);

/** \brief	输入序列号.
 *			在调用ISSGetActiveKey获取激活码前、在调用ISSActivate激活前、
 *          在调用ISSSRCreate创建识别句柄前, 都需要至少调用一次此接口. \n
 *	\param	szSerialCode				[In] 授权序列号.  \n
 *  \retval ISSErrID    				语音套件授权激活的错误码. \n
 *  \retval ISS_SUCCESS					输入序列号成功. \n
 *	\retval	ISS_ERROR_INVALID_PARA		无效的参数值. \n
 *	\retval	ISS_ERROR_FAIL				输入序列号失败. \n
 */
ISSErrID ISSAPI ISSSetSerialNumber(
    const char* szSerialCode      /* [In] 序列号 */
);

/** \brief  To get an activation code, the function only needs to be called online successfully once.
 *          Before the ISSGetActiveKey is called to get an activation code, the machine code must have been set by calling ::ISSSetMachineCode .\n
 *          The function is a synchronous, non-reentrant and non-thread-safe one. The function can be called only when the software is an encrypted version. \n
 *          Otherwise, it cannot be called. \n
 * \param   szResourceDir                   [In] Resource directory, usually: 
 *                                              adopts the GBK code format in the Windows Chinese environment, 
 *                                              and the UTF-8 code format in the Linux environment. \n
 *
 * \retval  ISSErrID                         The error code of the speech suite license activation \n
 * \retval  ISS_SUCCESS                      The activation code is obtained successfully.\n
 * \retval  ISS_ERROR_FAIL                   Fails to obtain the activation code. \n
 * \retval  ISS_ERROR_MACHINE_CODE_NOT_SET   The machine code is not set yet.\n
 * \retval  ISS_ERROR_INVALID_MACHINE_CODE   The machine code is unauthorized. \n
 * \retval  ISS_ERROR_ACTIVATE_TO_MANY_TIMES The authorization of the machine code reaches the maximum times. \n
 * \retval  ISS_ERROR_NET_XXXX               Network error\n
 */
ISSErrID ISSAPI ISSGetActiveKey(
    const char*     szResourceDir       /* [In] Resource directory, usually: 
                                                adopts the GBK code format in the Windows Chinese environment,
                                                and the UTF-8 code format in the Linux environment. */
);

/** \brief  Enter the activation directory and activate. \n
 *          The interface shall be called at least once respectively before the ::ISSSRCreate is called to create an recognition handle, 
 *          and the ::ISSTTSInitRes is called to create a text to speech resource handle. \n
 *          Before the ISSActivate is called to make activation, the machine code must have been set by calling the ::ISSSetMachineCode .\n
 *          The function is synchronous, non-reentrant and non-thread-safe one. The function can be called only when the software is an encrypted version. \n
            Otherwise, it cannot be called. \n
 * \param   szResourceDir   [In] Resource directory \n
 *
 * \retval  ISSErrID                       The error code of the speech suite license activation \n
 * \retval  ISS_SUCCESS                    The license activation is successful. \n
 * \retval  ISS_ERROR_INVALID_PARA         Invalid parameter\n
 * \retval  ISS_ERROR_MACHINE_CODE_NOT_SET The machine code is not set yet.\n
 * \retval  ISS_ERROR_INVALID_ACTIVE_KEY   Invalid activation code\n
 */
ISSErrID ISSAPI ISSActivate(
    const char*  szResourceDir          /* [In] Resource directory */
);

/** \brief  Enter the activation directory and activate. \n
 *          The interface shall be called at least once respectively before the ::ISSSRCreate is called to create an recognition handle, 
 *          and the ::ISSTTSInitRes is called to create a text to speech resource handle. \n
 *          Before the ISSActivate is called to make activation, the machine code must have been set by calling the ::ISSSetMachineCode .\n
 *          The function is synchronous, non-reentrant and non-thread-safe one. The function can be called only when the software is an encrypted version. \n
            Otherwise, it cannot be called. \n
 * \param   szResourceDir   [In] Resource directory \n
 *
 * \retval  ISSErrID                       The error code of the speech suite license activation \n
 * \retval  ISS_SUCCESS                    The license activation is successful. \n
 * \retval  ISS_ERROR_INVALID_PARA         Invalid parameter\n
 * \retval  ISS_ERROR_MACHINE_CODE_NOT_SET The machine code is not set yet.\n
 * \retval  ISS_ERROR_INVALID_ACTIVE_KEY   Invalid activation code\n
 */
ISSErrID ISSAPI ISSActivateW(
	const wchar_t*  szResourceDir          /* [In] Resource directory */
);


/** \brief  Enter the activation directory and activate. \n
 *          The interface shall be called at least once respectively before the ::ISSSRCreate is called to create an recognition handle, 
 *          and the ::ISSTTSInitRes is called to create a text to speech resource handle. \n
 *          Before the ISSActivate is called to make activation, the machine code must have been set by calling the ::ISSSetMachineCode .\n
 *          The function is synchronous, non-reentrant and non-thread-safe one. The function can be called only when the software is an encrypted version. \n
            Otherwise, it cannot be called. \n
 * \param   szResourceDir   [In] Resource directory \n
 *
 * \retval  ISSErrID                       The error code of the speech suite license activation \n
 * \retval  ISS_SUCCESS                    The license activation is successful. \n
 * \retval  ISS_ERROR_INVALID_PARA         Invalid parameter\n
 * \retval  ISS_ERROR_MACHINE_CODE_NOT_SET The machine code is not set yet.\n
 * \retval  ISS_ERROR_INVALID_ACTIVE_KEY   Invalid activation code\n
 */
ISSErrID ISSAPI ISSKeyActivate(
    const char*  szKey          /* [In] activate key */
);

/** \brief  Enter the activation directory and activate. \n
 *          The interface shall be called at least once respectively before the ::ISSSRCreate is called to create an recognition handle, 
 *          and the ::ISSTTSInitRes is called to create a text to speech resource handle. \n
 *          Before the ISSActivate is called to make activation, the machine code must have been set by calling the ::ISSSetMachineCode .\n
 *          The function is synchronous, non-reentrant and non-thread-safe one. The function can be called only when the software is an encrypted version. \n
            Otherwise, it cannot be called. \n
 * \param   szResourceDir   [In] Resource directory \n
 *
 * \retval  ISSErrID                       The error code of the speech suite license activation \n
 * \retval  ISS_SUCCESS                    The license activation is successful. \n
 * \retval  ISS_ERROR_INVALID_PARA         Invalid parameter\n
 * \retval  ISS_ERROR_MACHINE_CODE_NOT_SET The machine code is not set yet.\n
 * \retval  ISS_ERROR_INVALID_ACTIVE_KEY   Invalid activation code\n
 */
ISSErrID ISSAPI ISSKeyActivateW(
	const wchar_t*  szKey          /* [In] activate key */
);

/** \brief  Local tool to get mass activation code.
 * \param   szResourceDir                   [In] Resource directory, usually: 
 *                                              adopts the GBK code format in the Windows Chinese environment, 
 *                                              and the UTF-8 code format in the Linux environment. \n
 * \param   pcAppID							[In] AppID
 * \param   pcActiveKey						[Out]ActiveKey
 *
 * \retval  ISSErrID                         The error code of the speech suite license activation \n
 * \retval  ISS_SUCCESS                      The activation code is obtained successfully.\n
 * \retval  ISS_ERROR_FAIL                   Fails to obtain the activation code. \n
 * \retval  ISS_ERROR_MACHINE_CODE_NOT_SET   The machine code is not set yet.\n
 * \retval  ISS_ERROR_INVALID_MACHINE_CODE   The machine code is unauthorized. \n
 * \retval  ISS_ERROR_ACTIVATE_TO_MANY_TIMES The authorization of the machine code reaches the maximum times. \n
 * \retval  ISS_ERROR_NET_XXXX               Network error\n
 */
ISSErrID ISSAPI ISSGetActiveKeyLocal(
	const char*		pcAppID,
	char*			pcActiveKey,
	int				iBufSize,
	int*			piOutSize
);

#ifdef __cplusplus
}
#endif

#endif  /* __ISS_AUTH_H__ */
