/******************************************************************************
Copyright:      1999-2015, iFLYTEK Co., Ltd.
File name:      iss_ttpy.h
Description:    iFLYTEK Speech Suite – Text to Pinyin Module.
                Brief Description:
                ISS : iFLYTEK Speech Suite;
				ttpy : Text to Pinyin;
Author:         iFLYTEK
Version:        1.0
Date:           2015-07-07
History:        Created on Jul. 7, 2015
*******************************************************************************/
/** @defgroup group9 iFLYTEK Speech Suite – Text to Pinyin Module
 
 * @brief iss_ttpy.h
 
 */

#ifndef _ISS_TTPY_
#define _ISS_TTPY_

#include "iss_types.h"
#ifdef _cplusplus
extern "C"{
#endif
typedef void* HISSTTPY; ///< The text to pinyin handle
typedef const char* ISSPINYIN; ///< The pinyin type
/** \brief   Create the text to pinyin handle; \n
             It is a synchronous, non-reentrant and non-thread-safe function.\n
  * \param   phISSCATAINDEXObj  [Out] The text to pinyin handle\n
  * \param   szResourceDir      [In] Resource directory, usually adopting the GBK code format in the Windows Chinese environment, and the UTF-8 code format in the Linux environment. \n
  * \retval     ISSErrID               The error code returned by the general search engine \n
  * \retval     ISS_SUCCESS            The text to pronunciation object handle is created successfully. \n
  * \retval     ISS_ERROR_RES_DATA     Fails to verify or load the resource files. \n
  * \retval     ISS_ERROR_INVALID_PARA Invalid parameter value\n
*/

ISSErrID ISSAPI ISSTTPYCreate(
    HISSTTPY*      phISSCATAINDEXObj,  /* [Out] The text to pinyin handle */
    const char*         szResourceDir     /* [In] Resource directory, usually:
                                                adopting the GBK code format in the Windows Chinese environment,
                                                and the UTF-8 code format in the Linux environment. */
);
/** \brief    Destroy the text to pinyin handle; \n
              It is a synchronous, non-reentrant and non-thread-safe function.\n
  * \param    hISSCATAINDEXObj    [In] The text to pinyin handle\n
  * \retval     ISSErrID                 The error code returned by the engine\n
  * \retval     ISS_SUCCESS              The handle is destroyed successfully with resources being released.\n
  * \retval     ISS_ERROR_INVALID_HANDLE Invalid handle\n
*/
ISSErrID ISSAPI ISSTTPYDestroy(
    HISSTTPY    hISSCATAINDEXObj    /* [In] The text to pinyin handle */
);
/** \brief    Implement text to pronunciation; \n
              It is a synchronous, non-reentrant and non-thread-safe function \n
  * \param    hISSCATAINDEXObj   [In] The text to pronunciation handle \n
  * \param    text               [In] Text \n
  * \retval     ISSErrID               The error code returned by the engine \n
  * \retval     ISS_SUCCESS            Successful \n
  * \retval     ISS_ERROR_INVALID_PARA Invalid parameter value \n
*/
ISSPINYIN ISSAPI ISSText2PY(
    HISSTTPY    hISSCATAINDEXObj,    /* [In] The text to pronunciation handle */
	const char* text                /*[in] Text*/
);





#ifdef _cplusplus
}
#endif

#endif
