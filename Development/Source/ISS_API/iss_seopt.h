/*
Copyright:      1999-2017, iFLYTEK Co., Ltd.
File name:      iss_seopt.h
Description:    iFLYTEK Speech Suite ¨C seopt
Brief Description:
ISS : iFLYTEK Speech Suite;
Author:         iFLYTEK
Version:        1.0
Date:           2017-8-10
History:        Created on Aug. 10, 2017
*/
/** @defgroup group12 iFLYTEK Speech Suite ¨C seopt

* @brief iss_seopt.h

*/

#ifndef _ISS_SEOPT_H_
#define _ISS_SEOPT_H_

#include "iss_types.h"

#ifdef __cplusplus
extern "C" {
#endif

	/** \brief seopt handle
	 */
	typedef void* ISSSeoptHandle;

#define ISS_SEOPT_FRAME_SHIFT				(256)			///< samples of seopt processing once

#ifndef SE40
#define ISS_SEOPT_INPUT_CHANNELS			(4)				///< processing input audio channels
#else
#define ISS_SEOPT_INPUT_CHANNELS			(6)				///< processing input audio channels
#endif
#define ISS_SEOPT_INPUT_CHANNELS_HARDWARD	(2)				///< processing input audio channels, when use XF6010SYE

#define ISS_SEOPT_LEFT_CHANNELS				(0)				///< ISSSeoptGetGSCPower nIndex param value, get the left channel's audio info
#define ISS_SEOPT_RIGHT_CHANNELS			(1)				///< ISSSeoptGetGSCPower nIndex param value, get the right channel's audio info

#define ISS_SEOPT_PARAM_BEAM_INDEX			"beam_index"	///< param beamIndex id
#define ISS_SEOPT_PARAM_VALUE_BEAM_INIT		"-1"			///< param beamIndex value -1, means output the left channel audio
#define ISS_SEOPT_PARAM_VALUE_BEAM_LEFT		"0"				///< param beamIndex value 0, means output the left channel audio
#define ISS_SEOPT_PARAM_VALUE_BEAM_RIGTH	"2"				///< param beamIndex value 2, means output the left channel audio

#define ISS_SEOPT_PARAM_WORK_MODE					"work_mode"	///< param work_mode id
#define ISS_SEOPT_PARAM_VALUE_MAB_VAD_ONLY_MODE		"0"			///< param work_mode value 0, vad out from two beams, for XF6010SYE
#define ISS_SEOPT_PARAM_VALUE_MAB_MODE				"1"			///< param work_mode value 1, means mab mode on and mae mode off
#define ISS_SEOPT_PARAM_VALUE_MAE_MODE				"2"			///< param work_mode value 2, means mab mode off and mae mode on
#define ISS_SEOPT_PARAM_VALUE_MAB_AND_MAE_MODE		"3"			///< param work_mode value 3, means mab mode on and mae mode off
#define ISS_SEOPT_PARAM_VALUE_MAE_FAKE              "4"

#define ISS_SEOPT_PARAM_SAMPLING_DIGIT		"sampling_digit"	///< sampling digit of audio

#define ISS_SEOPT_PARAM_GAIN				"gain"			

#define ISS_SEOPT_PARAM_MODULATED						"modulated"
#define ISS_SEOPT_PARAM_MODULATED_TYPE_OFF				"0"
#define ISS_SEOPT_PARAM_MODULATED_TYPE_HARD				"1"
#define ISS_SEOPT_PARAM_MODULATED_TYPE_SOFT				"2"

#define ISS_SEOPT_PARAM_VALUE_ON						"on"                ///< On
#define ISS_SEOPT_PARAM_VALUE_OFF						"off"               ///< Off


/** \brief  Create a seopt handle; \n
 *          It is a synchronous, non-reentrant and non-thread-safe function; \n
 * \param	pHandle                [Out] seopt handle \n
 * \param	szResPath           [In] Resource directory, usually: adopts the GBK code format in the Windows Chinese environment, and the UTF-8 code format in the Linux environment.\n
 * \param   iMode             	[In] seopt working mode. see ISS_SEOPT_MODE_VW ISS_SEOPT_MODE_SR\n
 * \retval  ISSErrID                  The error code returned.
 * \retval  ISS_SUCCESS               The seopt handle is created successfully. \n
 * \retval  ISS_ERROR_FILE_NOT_FOUND  No corresponding resource file is found. \n
 * \retval  ISS_ERROR_INVALID_PARA    Invalid parameter value \n
 * \retval  ISS_ERROR_OUT_OF_MEMORY   Insufficient memory \n
 * \retval  ISS_ERROR_EXCEPTION   	  an exception happended \n
 */
ISSErrID ISSAPI ISSSeoptCreate(ISSSeoptHandle * pHandle, const char* szResPath);

/** \brief  Destory a seopt handle; \n
 *          It is a synchronous, non-reentrant and non-thread-safe function; \n
 * \param	handle             	[in] seopt handle \n
 * \retval  ISSErrID                  The error code returned.
 * \retval  ISS_SUCCESS               The seopt handle is created successfully. \n
 * \retval  ISS_ERROR_INVALID_HANDLE  Invalid seopt handle \n
 * \retval  ISS_ERROR_EXCEPTION   	  an exception happended \n
 */
ISSErrID ISSAPI ISSSeoptDestroy(ISSSeoptHandle handle);

/** \brief  Set some parameters; \n
 *          It is a synchronous, reentrant and thread-safe function. \n
 * \param	handle     			[In] seopt handle \n
 * \param	szParam   			[In] parameter \n
 * \param	szParamValue      	[In] parameter value\n
 * \retval  ISSErrID               		The error code returned.\n
 * \retval  ISS_SUCCESS             	param is set successfully. \n
 * \retval  ISS_ERROR_INVALID_HANDLE 	Invalid seopt handle \n
 * \retval  ISS_ERROR_EXCEPTION   	 	an exception happended \n
 */
ISSErrID ISSAPI ISSSeoptSetParam(ISSSeoptHandle handle, const char* szParam, const char* szParamValue);

/** \brief  process audio; \n
 *          It is a synchronous, reentrant and thread-safe function. \n
 * \param	handle     			[In] seopt handle \n
 * \param	pBufIn   			[In] The buffer address of the imported audio (16k 16bits), is made of four channels audio, mic00 mic10 ref00 ref10 ,mic01 mic11 ref01 ref11 ... \n
 * \param	nSamplesIn      	[In] The number of the sampling points to be written, not the number of the bytes \n
 * \param	ppBufOut      		[Out] The pointer of buffer address of the output audio (16k 16bits), if the channels out num is 2, the audio's first nSamplesOut is the left channel audio, the audio's second nSamplesOut is the right channel audio\n
 * \param	pnSamplesOut      	[Out] The pointer of samples to be output, not the number of the bytes \n
 * \param	pnChannelsOut      	[Out] The pointer of channels to be output\n
 * \retval  ISSErrID               		The error code returned.\n
 * \retval  ISS_SUCCESS             	param is set successfully. \n
 * \retval  ISS_ERROR_INVALID_HANDLE 	Invalid seopt handle \n
 * \retval  ISS_ERROR_EXCEPTION   	 	an exception happended \n
 */
ISSErrID ISSAPI ISSSeoptProcess(ISSSeoptHandle handle, const void* pBufIn, int nSamplesIn, const void** ppBufOut, int* pnSamplesOut, int* pnChannelsOut);

#ifndef SE40
/** \brief  get the power and angle of the channel's audio; \n
* \param	handle     			[In] seopt handle \n
* \param	nIndex   			[In] left(0) channel or right(1) channel, other value is invalid \n
* \param	pfPower      		[Out] The pointer of a channel's audio power to be output \n
* \param	pfAngle      		[Out] The pointer of a channel's audio angle to be output\n
* \retval  ISSErrID               		The error code returned.\n
* \retval  ISS_SUCCESS             	param is set successfully. \n
* \retval  ISS_ERROR_INVALID_HANDLE 	Invalid seopt handle \n
* \retval  ISS_ERROR_EXCEPTION   	 	an exception happended \n
*/
ISSErrID ISSAPI ISSSeoptGetGSCPower(ISSSeoptHandle handle, int nIndex, float* pfPower, float* pfAngle);

ISSErrID ISSAPI ISSSeoptDemodulate(ISSSeoptHandle handle, const void * pBufIn, int nSamplesIn, const void ** ppBufOut, int * pnSamplesOut);
ISSErrID ISSAPI ISSSeoptDemodulateEx(ISSSeoptHandle handle, const void * pBufIn, int nSamplesIn, const void ** ppBufOutL, const void ** ppBufOutH, int * pnSamplesOut);

#endif

#ifdef __cplusplus
}
#endif

#endif // !_ISS_SEOPT_H_
