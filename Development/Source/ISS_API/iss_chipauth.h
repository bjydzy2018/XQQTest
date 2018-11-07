/**
Copyright:      1999-2014, iFLYTEK Co., Ltd.
File name:      iss_chipauth.h
Description:    iFLYTEK Speech Suite - Authorization to Access the Hardware Chip Module 
                Brief Description:
                ISS : iFLYTEK Speech Suite;
Author:         iFLYTEK
Version:        1.0
Date:           2014-8-27
History:        Created on Aug. 27, 2014
*/

/** @defgroup group1 iFLYTEK Speech Suite - Authorization to Access the Hardware Chip Module
 
 * @brief iss_chipauth.h

 */

#ifndef _ISS_CHIPAUTH_H_
#define _ISS_CHIPAUTH_H_

#include "iss_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \brief The callback of getting the authorized chip information; \n
           In different client platforms, authorized access to the hardware chip interface is not completely consistent, 
           so users need to get chip authorization information in the callback.\n
 *  \param   pHint       [In]  The 8-byte decryption hint \n
 *  \param   pChipinfo   [Out] The 32-byte decryption hint of the authorized chip \n
 * \return   ISSErrID    Whether the chip information is obtained successfully or not. \n
             ISS_SUCCESS: The chip information is obtained successfully; \n   
*/
typedef ISSErrID (ISSCALLBACK *Proc_ISSChipLoadinfo)(
    const unsigned char* pHint,    /* [In]  The 8-byte decryption hint */
    unsigned char* pChipinfo       /* [Out] The 32-byte decryption hint of the authorized chip */
);


/** \brief  The callback of the failed chip authorization.\n
  * \param   pUsrArg   \n
  * \param   err [In] The error code of the chip authorization failure \n
                     ISS_ERROR_INVALID_PARA: The callback function of obtaining the authorized chip information is not set; \n
                     ISS_ERROR_CHIP_LOAD_INFO: Fails to get the authorized chip information; \n
                     ISS_ERROR_CHIP_CRYPT_FAIL: Fails to decrypt the authorized chip.\n
*/

typedef void (ISSCALLBACK *Proc_OnISSChipCryptError)(
    void* pUsrArg,
    ISSErrID err    /* [In] The error code of the chip authorization failure
	                    ISS_ERROR_INVALID_PARA: The callback function of obtaining the authorized chip information is not set;
                        ISS_ERROR_CHIP_LOAD_INFO: Fails to get the authorized chip information;
                        ISS_ERROR_CHIP_CRYPT_FAIL: Fails to decrypt the authorized chip.
                      */
);

/** \brief      Set the callback function of obtaining the authorized chip information; \n
                It is a synchronous, non-reentrant and non-thread-safe function. \n
 * \param       pfnChipLoadinfo   [In] The callback of getting the authorized chip information \n
 * \retval      ISSErrID               Whether the callback function is set successfully or not. \n
 * \retval      ISS_SUCCESS            The callback function is set successfully. \n
 * \retval      ISS_ERROR_INVALID_PARA Invalid parameter value, for example, the value of the callback function pointer is null. \n
 * \par Note
If the suite for the encrypted chip is used, this interface shall be called prior to other interfaces. \n

 */
ISSErrID ISSAPI ISSSetChipLoadinfoCb(
    Proc_ISSChipLoadinfo pfnChipLoadinfo  /* [In] The callback of getting the authorized chip information */
);

#ifdef __cplusplus
}
#endif

#endif /* _ISS_CHIPAUTH_H_ */
