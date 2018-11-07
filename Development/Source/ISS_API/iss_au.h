/******************************************************************************
Copyright:      	1999-2016, iFLYTEK Co., Ltd.
File name:      	iss_sr.h
Description:   	iFLYTEK Speech Suite - Audio Utility.
						Brief Description:
						ISS: iFLYTEK Speech Suite;
						AU: Audio Utility;
Author:        	 	iFLYTEK
Version:        	1.0
Date:           		2016-07-18
History:       	 	Created on July. 18, 2016
*******************************************************************************/

/** @defgroup group5 iFLYTEK Speech Suite - Audio Utility
 * @brief iss_au.h
 */
 
 #ifndef __ISS_AU_H__
 #define __ISS_AU_H__
 
 #include "iss_types.h"
 
 
 #ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/* audio utility handle */
typedef void*   HISSAU;

/* audio echo cancel reference count */
typedef enum tagAecRefCount
{
	ISS_AU_AEC_SINGLE = 1,			///< mono reference signal
	ISS_AU_AEC_DOUBLE = 2			///< stereo reference signal
} AECREFCNT;

typedef enum tagAecType
{
	ISS_AU_AEC_NONALIGN = 0,		///< non-align BargeIn
	ISS_AU_AEC_ALIGN = 1			///< align BargeIn
} AECTYPE;

typedef enum tagAuModule
{
	ISS_AU_INVALID = -1,
	ISS_AU_AEC = 0,
	ISS_AU_COUNT
}AUMODULE;

#define ISSAU_PARAM_AEC_LEAK_SET						"2"
#define ISSAU_AEC_LEAK_VALUE							"10"	//"1"~"100"£¬default is "10"	

/* AEC out write mode parameters and parameter value */
#define ISSAU_PARAM_AEC_OUTWRITE_MODE					"0"		///< default mode is ISSAU_AEC_OUTWRITE_MODE_MONO
#define ISSAU_AEC_OUTWRITE_MODE_MONO					"0"		///< just get 128 samples of AEC output.( AECOut0 AECOut1   AECout127 )
#define ISSAU_AEC_OUTWRITE_MODE_STEREO_ECHO				"1"		///< get 128 samples of AEC output and 128 samples.( AECOut0 Mic0 AECOut1 Mic1   AECout127 Mic127 )
#define ISSAU_AEC_OUTWRITE_MODE_STEREO_REF				"2"		///< get 128 samples of AEC output and 128 samples.( AECOut0 Ref0 AECOut1 Ref1   AECout127 Ref127 )

/* AEC work mode parameters and parameter value*/
#define ISSAU_PARAM_AEC_WORK_MODEL						"1"		///< defaule mode is ISSAU_AEC_WORK_MODEL_MUSIC
#define ISSAU_AEC_WORK_MODEL_TELEPHONE					"0"		///< telephone model
#define ISSAU_AEC_WORK_MODEL_MUSIC						"1"		///< music model 

/** \brief Create an audio echo cancel handle; \n
 * \param phISSAU 					[Out] audio utility handle \n
 * \param eAecRef					[In] aec reference signal count \n
 * \param eType						[In] aec type, default align \n
 * \retval ISSErrID 					The error code returned by the speech recognition engine \n
 * \retval ISS_SUCCESS           The speech recognition handle is created successfully.\n
*/
ISSErrID ISSAPI ISSAUAecCreate(
	HISSAU* phISSAU,						/* [out] audio utility handle */
	AECREFCNT eAecRef,						/* [in] aec reference signal count */
	AECTYPE eType								/* [in] aec type */
);

/** \brief set AEC parameter; \n
 * \param hISSAU					[In] audio utility handle \n
 * \param szParam					[In] AEC Parameter name \n
 * \param szParamValue			[In] AEC Parameter value  \n
 * \retval ISSErrID 					The error code returned by the speech recognition engine \n
 * \retval ISS_SUCCESS			The speech recognition handle is created successfully.\n
*/
ISSErrID ISSAPI ISSAUSetParam(
	HISSAU hISSAU,							/* [In] audio utility handle */
	AUMODULE eMod,							/* [In] audio utility sub module */
	const char* const szParam,				/* [In] parameter name */
	const char* const szParamValue		/* [In] parameter value */
);

/** \brief process aec audio signal ; \n
 * \param hISSAU 					[In] audio utility handle \n
 * \param pAudio						[In] aec reference signal count \n
 * \param pRef							[In] aec type, default align \n
 * \param nSize						[In] size of signal buffer \n
 * \param pAec						[Out] reference signal buffer \n
 * \param pAecSize					[Out] size of pAec buffer		  \n
 * \retval ISSErrID 					The error code returned by the speech recognition engine \n
 * \retval ISS_SUCCESS           The speech recognition handle is created successfully.\n
*/
ISSErrID ISSAPI ISSAUAecProcessAudio(
	HISSAU hISSAU,					/* [In] audio utility handle */
	const void* const pAudio,		/* [In] audio signal buffer */
	const void* const pRef,			/* [In] reference signal buffer */
	int nSize,								/* [In] size of signal buffer */
	void** pAec,							/* [Out] aec processed signal buffer */
	int* pAecSize						/* [Out] size of pAec */
);

/** \brief reset au ; \n
 * \param hISSAU 					[In] audio utility handle \n
 * \param eMod						[In] audio utility sub module \n
 * \retval ISSErrID 					The error code returned by the speech recognition engine \n
 * \retval ISS_SUCCESS           The speech recognition handle is created successfully.\n
*/
ISSErrID ISSAPI ISSAUReset(
	HISSAU hISSAU,							/* [In] audio utility handle */
	AUMODULE eMod							/* [In] audio utility sub module */
);


/** \brief destory audio utility aec ; \n
 * \param hISSAU 					[In] audio utility handle \n
 * \retval ISSErrID 					The error code returned by the speech recognition engine \n
 * \retval ISS_SUCCESS           The speech recognition handle is created successfully.\n
*/
ISSErrID ISSAPI ISSAUAecDestory(
	HISSAU hISSAU					/* [In] audio utility handle */
);
	
#ifdef __cplusplus
}
#endif //__cplusplus
 
 
 #endif //__ISS_AU_H__