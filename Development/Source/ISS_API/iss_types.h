/******************************************************************************
Copyright:      1999-2013, iFLYTEK Co., Ltd.
File name:      iss_types.h
Description:    The Types Description of the iFLYTEK Speech Suite
                Brief Description:
                ISS : iFLYTEK Speech Suite;
Author:         iFLYTEK
Version:        1.0
Date:           2013-10-01
History:        Created on Oct. 1, 2013*/
/** @mainpage Header File Description
 
  * @brief iss_auth.h iFLYTEK Speech Suite - Authorization \n
  * @brief iss_chipauth.h iFLYTEK Speech Suite - Authorization to Access the Hardware Chip Module \n
  * @brief iss_mvw.h iFLYTEK Speech Suite –Voice Wakeup\n
  * @brief iss_sr.h iFLYTEK Speech Suite - Speech Recognition \n
  * @brief iss_tts.h iFLYTEK Speech Suite - Text to Speech \n
  * @brief iss_types.h The Types Description of the iFLYTEK Speech Suite \n
  * @brief iss_errors.h The Error Type Definition of the iFLYTEK Speech Suite\n
  * \version 1.0\n
  * \author iFLYTEK\n

 
 */

/** @defgroup group2 The Types Description of the iFLYTEK Speech Suite
 
 * @brief iss_types.h
 
 */


#ifndef __ISS_TYPES_H__
#define __ISS_TYPES_H__


// #define ISS_SOFTWARE_ENCRYPTION

// The way how the function is called

#if !defined(ISSCALL)
#if defined(_WIN32)
    #define ISSCALL __stdcall
#else
    #define ISSCALL
#endif /* _WIN32 */
#endif /* ISSCALL */

// The function exported symbols
#if defined _WIN32 || defined __CYGWIN__
#define ISS_DLL_IMPORT __declspec(dllimport)
#define ISS_DLL_EXPORT __declspec(dllexport)
#define ISS_DLL_LOCAL
#define ISS_WCHAR_SUPPORT
#else
#if __GNUC__ >= 4
#define ISS_DLL_IMPORT __attribute__ ((visibility ("default")))
#define ISS_DLL_EXPORT __attribute__ ((visibility ("default")))
#define ISS_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define ISS_DLL_IMPORT
#define ISS_DLL_EXPORT
#define ISS_DLL_LOCAL
#endif
#endif

#ifdef ISS_DLL_EXPORTS // defined if we are building the DLL (instead of using it)
#define ISSAPI_PORT ISS_DLL_EXPORT
#else
#define ISSAPI_PORT ISS_DLL_IMPORT
#endif // ISS_DLL_EXPORTS

#define ISSAPI ISSAPI_PORT ISSCALL

#define ISSLOCAL ISS_DLL_LOCAL ISSCALL

#define ISSCALLBACK

#define ISS_RES_DIR_MAX_PATH    256     ///< The maximum length of the path

#include "iss_errors.h"

#endif /* __ISS_TYPES_H__ */
