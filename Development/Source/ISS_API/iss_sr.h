/******************************************************************************
Copyright:      1999-2013, iFLYTEK Co., Ltd.
File name:      iss_sr.h
Description:    iFLYTEK Speech Suite - Speech Recognition.
                Brief Description:
                ISS: iFLYTEK Speech Suite;
                SR: Speech Recognition;
Author:         iFLYTEK
Version:        1.0
Date:           2013-10-10
History:        Created on Oct. 10, 2013
*******************************************************************************/
/** @defgroup group5 iFLYTEK Speech Suite - Speech Recognition
 * @brief iss_sr.h
 */
#ifndef _ISS_SR_H_
#define _ISS_SR_H_

#include "iss_types.h"
#include "iss_auth.h"
#include "iss_chipauth.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Speech recognition handle */
typedef void*   HISSSR;

typedef enum tagISS_SR_ACOUS_LANG{
	ISS_SR_ACOUS_LANG_VALUE_MANDARIN = 0,         ///< Mandarin
	ISS_SR_ACOUS_LANG_VALUE_ENGLISH,          ///< English
	ISS_SR_ACOUS_LANG_VALUE_CANTONESE         ///< Cantonese
} ISS_SR_ACOUS_LANG;

/** Message type enumeration */
enum tagISS_SR_MSG{
/** \brief  Whether the initialization is successful;\n
 *          After the recognition handle is created, the ISS_SR_MSG_InitStatus message will be sent to notify whether the recognition engine initialization is successfully or not.
 *  \param  wParam     ISSErrID type:\n
 *                      ISS_SUCCESS: The initialization is successful.\n
 *                      ISS_ERROR_FAIL: The initialization fails.\n
 *                      ISS_ERROR_OUT_OF_MEMORY: The memory is insufficient.\n
 *  \param  lParam   NULL 
 */
    ISS_SR_MSG_InitStatus              =   20000,

/** \brief   Whether the upload of the local personalized data is successful;
 *          After a user calls the ISSSRUpLoadDict to asynchronously upload the personalized data, if the value of the parameter bOnlyUploadToCloud is 0, \n
 *          the ISS_SR_MSG_UpLoadDictToLocalStatus message will be sent to notify whether the local data upload is successful or not.
 *  \param  wParam     ISSErrID type:\n
 *                      ISS_SUCCESS: The local personalized data is successfully uploaded.\n
 *                      ISS_ERROR_INVALID_JSON_FMT: The input JSON format is incorrect.\n
 *                      ISS_ERROR_INVALID_JSON_INFO: No required personalized data is extracted from the JSON format input.\n
 *  \param  lParam   NULL

 */
    ISS_SR_MSG_UpLoadDictToLocalStatus =   20001,

/** \brief   Whether the cloud personalized data is successfully uploaded; \n
 *          After a user calls the ISSSRUpLoadDict to asynchronously upload the personalized data,   
 *          the ISS_SR_MSG_UpLoadDictToCloudStatus message will be sent to notify whether the cloud data is successfully uploaded.
 *  \param  wParam     ISSErrID type:\n
                        ISS_SUCCESS: The cloud personalized data is successfully uploaded.\n
                        ISS_ERROR_INVALID_JSON_FMT: The input JSON format is incorrect.\n
                        ISS_ERROR_INVALID_JSON_INFO: No required personalized data is extracted from the JSON format input.\n
                        ISS_ERROR_NET_XXXX: Network error\n
                        ISS_ERROR_FAIL: The cloud upload of the personalized data fails.\n
 *  \param  lParam    NULL
 */
    ISS_SR_MSG_UpLoadDictToCloudStatus =   20002,

/** \brief   Sound energy, which is returned once when a 25 ms recording is calculated (40Hz); \n
 *          Used for displaying the microphone effect in the interface.
 *  \param  wParam   The volume value, which ranges from 0 to 931.
 *  \param  lParam   NULL

 */
    ISS_SR_MSG_VolumeLevel             =   20003,

/** \brief  Response timeout, no speech is detected at a predetermined time.
 *  \param  wParam   NULL
 *  \param  lParam   NULL
 *  \par Note
      The recognition engine has stopped, and no more data needs to be written.
 */
    ISS_SR_MSG_ResponseTimeout         =   20004,

/** \brief  Speech start point is detected.
 *  \param  wParam   NULL
 *  \param  lParam   NULL
 */
    ISS_SR_MSG_SpeechStart             =   20005,

/** \brief  Speech timeout.
 *  \param  wParam   NULL
 *  \param  lParam   NULL
 *  \par Note
    The recognition is in progress, with no more data need to be written.
 */
    ISS_SR_MSG_SpeechTimeOut           =   20006,

/** \brief  The speech end point is detected.
 *  \param  wParam   NULL
 *  \param  lParam   NULL
 *  \par Note
      The recognition is in progress, and no more data needs to be written.
 */
    ISS_SR_MSG_SpeechEnd               =   20007,

/** \brief  Other errors.
 *  \param  wParam    ISSErrID type:\n
 *                     ISS_ERROR_CHIP_LOAD_INFO: Fails to obtain the authorization chip information.\\n
 *                     ISS_ERROR_CHIP_CRYPT_FAIL: The authorization chip decryption fails.\n
 *  \param  lParam     const char* type, error information
 *  \par Note
      The recognition engine has stopped, and no more data needs to be written.
 */
    ISS_SR_MSG_Error                   =   20008,

/** \brief   The recognition results with semantics are returned. For the analysis of the recognition result, see the "Semantic Framework Agreement".
            When a local recognition result is returned, 
            an additional label "nlocalconfidencescore" representing the recognition confidence score will be attached. The confidence score of the transliteration result of the local recognition is 0. 
 *  \param  wParam   NULL
 *  \param  lParam   const char* type, recognition result
 *  \par Note
      Only one recognition result will be displayed after a recognition starts. The recognition engine has stopped, and no more data needs to be written.
 */
    ISS_SR_MSG_Result                  =   20009,

/** \brief   The status of the local recognition of the big transliteration resources being lazy loaded.
 *  \param   wParam   ISSErrID type:\n
 *                      ISS_ERROR_OUT_OF_MEMORY\n
 *                      ISS_ERROR_FAIL\n
 *                      ISS_SUCCESS\n
 *  \param   lParam   NULL
 */
    ISS_SR_MSG_LoadBigSrResStatus      =   20010,

/** \brief  If the suite is a hardware encrypted version, to avoid that the hardware encrypted recording may affect the cloud recognition results,
 *          the recording shall be decrypted when it is sent to the cloud and the recording decryption in the isssr.cfg shall be enabled.
 *          "DecodeAndioBeforeSendToCloud": 1.
 *          If it is found that the sent recording is not a hardware encrypted one in the process of decrypting, this message will be sent to warn   
 *          that the recording may be recorded by using an ordinary microphone.
 *  \param   wParam   NULL
 *  \param   lParam   NULL
 */
    ISS_SR_MSG_ErrorDecodingAudio      =   20011,

/** \brief   For the mixed recognition mode, the service name of the preliminary results will be provided based on the local recognition results. The allowed service name is 
 *          defined by configuring OutputFocusAsPreResultForHMITip. If the local gives the focus results of the information source which must rely on the server prior to the cloud, 
 *          then the focus is first given used for HMI prompt, for example: 
 *          There is no network link currently. Search for XX.
 *  \param  wParam   NULL
 *  \param  lParam   const char* type, focus name
 */
    ISS_SR_MSG_PreResult               =   20012,

/** \brief  For the mixed recognition mode, a message will be sent in advance to notify whether the cloud initialization is successful. The caller can use the pure cloud mode before the local initialization is completed; \n
  *        When a recognition handle is created, the ISS_SR_MSG_CloudInitStatus message will be sent to notify whether the cloud initialization is successful.
  * \param wParam:      ISS_SUCCESS:	    The initialization is successful.
			            ISS_ERROR_FAIL:			The initialization fails. 
			            ISS_ERROR_OUT_OF_MEMORY:	The memory is insufficient.
  * \param lParam:      NULL
*/
    ISS_SR_MSG_CloudInitStatus         =   20013,
	/*******************************************************************************
	MsgNum:      ISS_SR_MSG_RealTimeResult
	Description: 返回带语义的实时识别结果
	wParam:      NULL
	lParam:      const char*类型，识别结果
	Others:      一次识别开始后，可能会多次出现识别结果，识别引擎在工作.
	*******************************************************************************/
	ISS_SR_MSG_RealTimeResult               =   20014,

/** \brief For the mixed recognition mode, a message means having local result, but waiting cloud result.
 *  \param  wParam    NULL
 *  \param  lParam    const char* type, local result
 */
    ISS_SR_MSG_WaitingForCloudResult   =   20018,

/** \brief  Begin to copy Active/SRRes/Res* to user defined active path.
 *  \param  wParam   NULL
 *  \param  lParam   NULL
 */
	ISS_SR_MSG_Res_Update_Start = 20019,

/** \brief  Finish to copy Active/SRRes/Res* to user defined active path.
 *  \param  wParam   Is copy succeed, succeed on true, failed on false.
 *  \param  lParam   NULL
 */
	ISS_SR_MSG_Res_Update_End = 20020,

/** \brief For the mixed recognition mode, a message means having cloud result, but waiting local result.
 *  \param  wParam    NULL
 *  \param  lParam    const char* type, cloud result
 */
	ISS_SR_MSG_WaitingForLocalResult = 20021,

	ISS_SR_MSG_STKS_Result = 20022,

/** \brief For the oneshot mode, a message means having mvw result.
 *  \param  wParam    NULL
 *  \param  lParam    const char* type, mvw result
 */
	ISS_SR_MSG_ONESHOT_MVWResult = 20023,

	ISS_SR_MSG_LOCAL_RECG_END = 20024,
};

/** \brief The engine sends various messages via the Proc_OnMsgProc callback.\n
 *  \param   pUsrArg   [In] User defined parameter \n
 *  \param   uMsg      [In] The message identifier \n
 *  \param   wParam    [In] Message additional information\n
 *  \param   lParam    [In] Message additional information \n
 *  \return  None\n
 */
typedef void (ISSCALLBACK *Proc_OnMsgProc)(
    const void*         pUsrArg,     /* [In] User defined parameter */
    unsigned int        uMsg,        /* [In] The message identifier */
    unsigned int        wParam,      /* [In] Message additional information */
    const void*         lParam        /* [In] Message additional information */
);

#define ISS_SR_SCENE_ALL              		"all"                 	///< All scene
#define ISS_SR_SCENE_POI              		"poi"                 	///< POI navigation scene
#define ISS_SR_SCENE_CONTACTS         		"contacts"            	///< Contacts scene
#define ISS_SR_SCENE_SELECT           		"select"              	///< Select scene
#define ISS_SR_SCENE_CONFIRM          		"confirm"             	///< Confirm scene
#define ISS_SR_SCENE_ANSWER_CALL      		"answer_call"         	///< The scene of answering a call
#define ISS_SR_SCENE_CMDLIST_WITHALL  		"cmdlist_withall"     	///< Abandon

#define ISS_SR_SCENE_STKS					"stks"					///< short time keyword select
#define ISS_SR_SCENE_ONESHOT				"oneshot"				///< OneShot scene

#define ISS_SR_SCENE_SELECTLIST_POI   		"selectlist_poi"		///< Abandon
#define ISS_SR_SCENE_SELECTLIST_CONTACTS 	"selectlist_contacts" 	///< Abandon

#define ISS_SR_SCENE_MUSIC				"music"				///< Misic scene
#define ISS_SR_SCENE_HIMALAYAFM			"himalayaFM"		///< HiMaLaYa scene

///< For the select scene of the multiple awakening realizations, the utterances can be: the first, the second, the third, the fourth, the fifth, the sixth, the last, cancel.
#define ISS_SR_SCENE_SELECT_MVW         	"select_mvw"
///< For the confirm scene of the multiple awakening realizations, the utterances can be: confirm, cancel. 
#define ISS_SR_SCENE_CONFIRM_MVW        	"confirm_mvw"
///< For the answering calls scene of the multiple awakening realizations, the utterances can be: answer, hang up, cancel.
#define ISS_SR_SCENE_ANSWER_CALL_MVW    	"answer_call_mvw"
///< For the multi-scenario scene of the multiple awakening realizations,the utterances is the words of build.
#define ISS_SR_SCENE_BUILD_GRM_MVW      	"build_grm_mvw"    

#define ISS_SR_MODE_CLOUD_REC       (0)              ///< Pure network recognition
#define ISS_SR_MODE_LOCAL_REC       (1)              ///< Pure local recognition
#define ISS_SR_MODE_MIX_REC         (2)              ///< Cloud and terminal mixed recognition
#define ISS_SR_MODE_LOCAL_CMDLIST   (3)              ///< Pure local command word (abandoned)
#define ISS_SR_MODE_LOCAL_NLP       (4)              ///< Pure local semantics
#define ISS_SR_MODE_LOCAL_MVW       (5)              ///< Abandon

#define ISS_SR_PARAM_IAT_EXTEND_PARAMS  "iatextendparams"    ///< Used only for experiment or test
#define ISS_SR_PARAM_SPEECH_TIMEOUT     "speechtimeout"	     ///< Recognition scene: The speech timeout duration starting from the moment the SpeechStart is detected.
#define ISS_SR_PARAM_RESPONSE_TIMEOUT   "responsetimeout"    ///< Recognition scene: The timeout duration from the moment that the recording is imported to that the SpeechStart is not detected. 
#define ISS_SR_PARAM_SPEECH_GAP         "speechgrap"         ///< Recognition scene: The timeout duration from The last word to the word interval
#define ISS_SR_PARAM_SPEECH_TAIL        "speechtail"         ///< Recognition scene: The delay time from the moment that a user finished his/her speaking to that the engine detects the SpeechEnd. Usually the default time is 800 ms. 
#define ISS_SR_PARAM_MVW_TIMEOUT        "mvwtimeout"         ///< MVW multiple awakening scenes: The timeout duration from the moment the recording is imported. 0 means infinity. 

#define ISS_SR_MVW_PARAM_AEC  "mvw_enable_aec"
#define ISS_SR_MVW_PARAM_LSA  "mvw_enable_lsa"

#define ISS_SR_PARAM_LONGTITUDE         "longitude"
#define ISS_SR_PARAM_LATITUDE           "latitude"
#define ISS_SR_PARAM_CITY               "city"
#define ISS_SR_PARAM_WAP_PROXY          "wap_proxy"
#define ISS_SR_PARAM_NET_SUBTYPE        "net_subtype"

#define ISS_SR_PARAM_ACOUS_LANG                       "ent"                 ///< Recognition languages
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_MANDARIN        "automotiveknife16k"  ///< Mandarin by default
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_ENGLISH         "sms-en16k"           ///< English
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_CANTONESE       "cantonese16k"        ///< Cantonese
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_LMZ             "lmz16k"              ///< Sichuan dialect 
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_HENANESE        "henanese16k"         ///< Henan dialect
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_DONGBEIESE      "dongbeiese16k"       ///< Dongbei dialect
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_SHANDONGNESE    "shandongnese16k"     ///< Shandong dialect
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_SHANXINESE      "shanxinese16k"       ///< Shanxi dialect
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_HEFEINESE       "hefeinese16k"        ///< Hefei dialect
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_NANCHANGNESE    "nanchangnese16k"     ///< Nanchang dialect (JIangxi province)
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_CHANGSHANESE    "changshanese16k"     ///< Changsha dialect (Hunan province)
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_WUHANESE        "wuhanese16k"         ///< Wuhan dialect (Hubei province)
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_MINNANESE       "minnanese16k"        ///< Minnan dialect
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_SHANGHAINESE    "shanghainese16k"     ///< Shanghai dialect
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_TIANJINESE      "tianjinese16k"       ///< Tianjin dialect
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_NANJING         "nankinese16k"        ///< Nanjing dialect
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_TAIYUANESE      "taiyuanese16k"       ///< Taiyuan dialect (Shanxi province)
#define ISS_SR_PARAM_ACOUS_LANG_VALUE_UYGHUR          "uyghur16k"           ///< Uyghur

#define ISS_SR_PARAM_TRACE_LEVEL              "tracelevel"
#define ISS_SR_PARAM_TRACE_LEVEL_VALUE_NONE   "none"
#define ISS_SR_PARAM_TRACE_LEVEL_VALUE_ERROR  "error"
#define ISS_SR_PARAM_TRACE_LEVEL_VALUE_INFOR  "infor"
#define ISS_SR_PARAM_TRACE_LEVEL_VALUE_DEBUG  "debug"

#define ISS_SR_PARAM_DOUL_MIC_MAE_DENOISE     "DoulMicMaeDeNoise"       ///< The software dual microphone noise reduction switch: whether the software dual microphone noise reduction is enabled. If it is enabled, the imported audio shall be dual microphone stereo (16k).
                                                                        ///< The default value of this parameter can be set in the configuration file (DoulMicMaeDeNoiseAudioBeforeSendToVad),
                                                                        ///< It is not recommended to be called at run time.

#define ISS_SR_PARAM_SINGLE_MIC_DENOISE       "SingleMicDeNoise"        ///< The software single microphone noise reduction switch: whether the software single microphone is enabled. If it is enabled, the imported audio shall be single microphone stereo (16k).
                                                                        ///< The default value of this parameter can be set in the configuration file (DeNoiseAudioBeforeSendToVad),
                                                                        ///< It is not recommended to be called at run time.

#define ISS_SR_PARAM_BARGE_IN                 "BargeIn"                 ///< Software BargeIn switch: when the BargeIn is enabled, the TTS broadcasting sound is eliminated;
                                                                        ///< When the prompt tone of the TTS broadcasting is played and user interruption is permitted, enable the BargeIn. 
                                                                        ///< When the BargeIn is enabled, the software dual microphone noise reduction switch and software single microphone noise reduction switch are both invalid.

#define ISS_SR_PARAM_VALUE_ON                 "on"                      ///< On
#define ISS_SR_PARAM_VALUE_OFF                "off"                     ///< Off
#define ISS_SR_PARAM_TMP_LOG_DIR              "TmpLogDir"               ///< Set tmp log directory for debugging

// Log level.According to the requirement to combine mask.
#define ISS_SR_PARAM_LOG_LEVEL					(0X00000506)
#define ISS_SR_VOLUME_LOG_LEVEL_ALL				("-1")	// all info
#define ISS_SR_VOLUME_LOG_LEVEL_NONE			("0")	// none
#define ISS_SR_VOLUME_LOG_LEVEL_CRIT			("1")	// critical info
#define ISS_SR_VOLUME_LOG_LEVEL_ERROR			("2")	// error info
#define ISS_SR_VOLUME_LOG_LEVEL_WARNING			("4")	// warnint info

// Log output.According to the requirement to combine mask.
#define ISS_SR_PARAM_LOG_OUTPUT					(0X00000507)
#define ISS_SR_VOLUME_LOG_OUTPUT_NONE			("0")	// none
#define ISS_SR_VOLUME_LOG_OUTPUT_FILE			("1")	// file
#define ISS_SR_VOLUME_LOG_OUTPUT_CONSOLE		("2")	// console（except for android）
#define ISS_SR_VOLUME_LOG_OUTPUT_DEBUGGER		("4")	// debugger
#define ISS_SR_VOLUME_LOG_OUTPUT_MSGBOX			("8")	// message box

// Log FileName
#define ISS_SR_PARAM_LOG_FILE_NAME				(0X00000508)

#define ISS_SR_PARAM_IAT_LOGIN_EXT_PARAMS	"IatLoginExtendParams"
#define ISS_SR_PARAM_IAT_RELOGIN			"CloudReLogin"

 /** \brief  Create a recognition handle;\n
             Before the ISSSRCreate is called to create a recognition handle, the ::ISSSetMachineCode must have been called to set the machine code;\n
			This function is an asynchronous function. When the function is executed, the recognition can be made only when the ::ISS_SR_MSG_InitStatus message is received and the result is successful;\n
            The function is also a non-reentrant function which is not thread-safe. The caller should note that an existing recognition handle cannot be created unless it is destroyed. \n
 * \param   phISSSRObj         [Out] Recognition handle\n
 * \param   szResourceDir      [In] Resource directory, usually: adopts the GBK code format in the Windows Chinese environment, and the UTF-8 code format in the Linux environment.
 \n
 * \param	pfnOnMsgProc       [In] Message callback \n
 * \param   pUsrArg            [In] User identified parameter \n
 * \param   bLoadBigSrResDelay [In] Whether to lazy load the local recognition of the big transliteration resources. \n
 * \retval      ISSErrID                       The error code returned by the speech recognition engine \n
 * \retval      ISS_SUCCESS                    The speech recognition handle is created successfully.\n
 * \retval      ISS_ERROR_MACHINE_CODE_NOT_SET The machine code is not set. \n
 * \retval      ISS_ERROR_FILE_NOT_FOUND       No corresponding resource file is found. \n
 * \retval      ISS_ERROR_INVALID_PARA         Invalid parameter value \n
*/
ISSErrID ISSAPI ISSSRCreate(
    HISSSR*         phISSSRObj,        /* [Out] Recognition handle */
    const char*     szResourceDir,     /* [In] Resource directory, usually:
                                                adopts the GBK code format in the Windows Chinese environment, 
                                                and the UTF-8 code format in the Linux environment. */
    Proc_OnMsgProc  pfnOnMsgProc,      /* [In] Message callback */
    void*           pUsrArg,           /* [In] User defined parameter */
    int             bLoadBigSrResDelay  /* [In] Whether to lazy load the local recognition of the big transliteration resources */
);
 /** \brief  Create a recognition handle;\n
             Before the ISSSRCreateW is called to create a handle, the ::ISSSetMachineCode must have been called to set the machine code;\n
			This function is an asynchronous and non-reentrant function which is not thread-safe. When the function is executed, the recognition can be made only when the ::ISS_SR_MSG_InitStatus message is received and the result is successful;\n
             The caller should note that an existing recognition handle cannot be created unless it is destroyed. \n
 * \param   phISSSRObj         [Out] Recognition handle\n
 * \param   szResourceDir      [In] Resource directory, usually: adopts the UNICODE-16 format in the Windows vc environment, and the UNICODE-32 code format in the Linux gcc environment.
 * \param	pfnOnMsgProc       [In] Message callback \n
 * \param   pUsrArg            [In] User defined parameter \n
 * \param   bLoadBigSrResDelay [In] Whether to lazy load the local recognition of the big transliteration resources \n
 * \retval      ISSErrID                       The error code returned by the speech recognition engine \n
 * \retval      ISS_SUCCESS                    The speech recognition handle is created successfully.\n
 * \retval      ISS_ERROR_MACHINE_CODE_NOT_SET The machine code is not set.\n
 * \retval      ISS_ERROR_FILE_NOT_FOUND       No corresponding resource file is found.\n
 * \retval      ISS_ERROR_INVALID_PARA         Invalid parameter value \n
*/
ISSErrID ISSAPI ISSSRCreateW(
    HISSSR*         phISSSRObj,        /* [Out] Recognition handle */
    const wchar_t*  szResourceDir,     /* [In] Resource directory, usually:
                                                adopts the UNICODE-16 format in the Windows vc environment,
                                                and adopts the UNICODE-32 code format in the Linux gcc environment. */
    Proc_OnMsgProc  pfnOnMsgProc,      /* [In] Message callback */
    void*           pUsrArg,           /* [In] User defined parameter */
    int             bLoadBigSrResDelay  /* [In] Whether to lazy load the local recognition of the big transliteration resources */       
);


/** \brief  Create a recognition handle;\n
             Before the ISSSRCreateEx is called to create a recognition handle, the ::ISSSetMachineCode must have been called to set the machine code;\n
			This function is an asynchronous function. When the function is executed, the recognition can be made only when the ::ISS_SR_MSG_InitStatus message is received and the result is successful;\n
            The function is also a non-reentrant function which is not thread-safe. The caller should note that an existing recognition handle cannot be created unless it is destroyed. \n
 * \param   phISSSRObj         [Out] Recognition handle\n
 * \param	eAcousLang		   [In]	languages: Mandarin, English, Cantonese\n
 * \param   szResourceDir      [In] Resource directory, usually: adopts the GBK code format in the Windows Chinese environment, and the UTF-8 code format in the Linux environment.
 \n
 * \param	pfnOnMsgProc       [In] Message callback \n
 * \param   pUsrArg            [In] User identified parameter \n
 * \retval      ISSErrID                       The error code returned by the speech recognition engine \n
 * \retval      ISS_SUCCESS                    The speech recognition handle is created successfully.\n
 * \retval      ISS_ERROR_MACHINE_CODE_NOT_SET The machine code is not set. \n
 * \retval      ISS_ERROR_FILE_NOT_FOUND       No corresponding resource file is found. \n
 * \retval      ISS_ERROR_INVALID_PARA         Invalid parameter value \n
*/
ISSErrID ISSAPI ISSSRCreateEx(
    HISSSR*         phISSSRObj,        /* [Out] Recognition handle */
	ISS_SR_ACOUS_LANG eAcousLang,	   /* [In] languages: Mandarin, English, Cantonese*/
    const char*     szResourceDir,     /* [In] Resource directory, usually:
                                                adopts the GBK code format in the Windows Chinese environment, 
                                                and the UTF-8 code format in the Linux environment. */
    Proc_OnMsgProc  pfnOnMsgProc,      /* [In] Message callback */
    void*           pUsrArg          /* [In] User defined parameter */
    );

 /** \brief  Create a recognition handle;\n
             Before the ISSSRCreateWEx is called to create a handle, the ::ISSSetMachineCode must have been called to set the machine code;\n
			This function is an asynchronous and non-reentrant function which is not thread-safe. When the function is executed, the recognition can be made only when the ::ISS_SR_MSG_InitStatus message is received and the result is successful;\n
             The caller should note that an existing recognition handle cannot be created unless it is destroyed. \n
 * \param   phISSSRObj         [Out] Recognition handle\n
 * \param	eAcousLang		   [In]	languages: Mandarin, English, Cantonese\n
 * \param   szResourceDir      [In] Resource directory, usually: adopts the UNICODE-16 format in the Windows vc environment, and the UNICODE-32 code format in the Linux gcc environment.
 * \param	pfnOnMsgProc       [In] Message callback \n
 * \param   pUsrArg            [In] User defined parameter \n
 * \retval      ISSErrID                       The error code returned by the speech recognition engine \n
 * \retval      ISS_SUCCESS                    The speech recognition handle is created successfully.\n
 * \retval      ISS_ERROR_MACHINE_CODE_NOT_SET The machine code is not set.\n
 * \retval      ISS_ERROR_FILE_NOT_FOUND       No corresponding resource file is found.\n
 * \retval      ISS_ERROR_INVALID_PARA         Invalid parameter value \n
*/
ISSErrID ISSAPI ISSSRCreateWEx(
    HISSSR*         phISSSRObj,         /* [Out] Recognition handle */
	ISS_SR_ACOUS_LANG eAcousLang,		/* [In] languages: Mandarin, English, Cantonese*/
    const wchar_t*  szResourceDir,     /* [In] Resource directory, usually:
                                                adopts the UNICODE-16 format in the Windows vc environment,
                                                and adopts the UNICODE-32 code format in the Linux gcc environment. */
    Proc_OnMsgProc  pfnOnMsgProc,      /* [In] Message callback */
    void*           pUsrArg           /* [In] User defined parameter */
    );

/** \brief  Destroy the speech recognition handle;\n
 *          This function is an asynchronous, non-reentrant and non-thread-safe function. After a recognition handle is created, the destroy function can be called only once. The input recognition handle should be valid, otherwise it may cause a crash. \n
 * \param  hISSSRObj        [In] Recognition handle\n
  
 * \retval   ISSErrID                 The error code returned by the speech recognition engine \n
 * \retval   ISS_SUCCESS              A recognition handle is destroyed successfully with resources being released. \n
 * \retval   ISS_ERROR_INVALID_HANDLE Invalid speech recognition handle\n
 *\par Note      
 When call ISSSRDestroy(), ensure that other thread no longer uses the recognition handle. \n
*/
ISSErrID ISSAPI ISSSRDestroy(
    HISSSR hISSSRObj   /* [In] Recognition handle */
);

/** \brief   Start a new speech recognition. If it is already at the recognition status, the newly started recognition will replace the previous recognition; \n
  *         When the function is executed, it means that the recording is available. The function is an asynchronous, non-reentrant and non-thread-safe function. \n
            When iMode = ::ISS_SR_MODE_LOCAL_CMDLIST , actions in the compile command word list are completed in the threads maintained by the suite, \n
            so the ISSSRStart interface will not be blocked; \n
		    After the Proc_OnInitStatus is returned successfully, the ISSSRStart can be called to initiate a recognition;\n
            After the ::ISS_SUCCESS is returned by the ISSSRStart, the ::ISSSRAppendAudioData can be called to send the recording.\n
  * \param  hISSSRObj   [In] Recognition handle\n
  * \param  szScene     [In] The specified scene and the defined scope of the recognition \n
                                   Support:\n
								         ::ISS_SR_SCENE_ALL \n
                                         ::ISS_SR_SCENE_POI \n
                                         ::ISS_SR_SCENE_CONTACTS \n
                                         ::ISS_SR_SCENE_SELECT \n
                                         ::ISS_SR_SCENE_CONFIRM \n
                                         ::ISS_SR_SCENE_SELECT_MVW (Ignore the parameter of iMode under this condition.)\n
                                         ::ISS_SR_SCENE_CONFIRM_MVW (Ignore the parameter of iMode under this condition.)\n
                                         ::ISS_SR_SCENE_ANSWER_CALL_MVW (Ignore the parameter of iMode under this condition.)\n
										 ::ISS_SR_SCENE_CMDLIST_WITHALL (Abandoned, but still supported)\n
										 (When iMode is ::ISS_SR_MODE_LOCAL_CMDLIST, the scene is “all scene”.) \n
  * \param  iMode       [In] iMode = ::ISS_SR_MODE_CLOUD_REC , pure network recognition\n
                             iMode = ::ISS_SR_MODE_LOCAL_REC , pure local recognition \n
                             iMode = ::ISS_SR_MODE_MIX_REC , could and terminal mixed recognition \n
                             iMode = ::ISS_SR_MODE_LOCAL_NLP , pure local semantics, under which condition, ignoring the szScene parameter \n
							 iMode = ::ISS_SR_MODE_LOCAL_CMDLIST (abandoned, but still supported), the local command word recognition, under which condition, ignoring the szScene parameter \n
  * \param  szCmd       [In] iMode = ::ISS_SR_MODE_LOCAL_CMDLIST (abandoned, but still supported),\n
                                   When iMode = ::ISS_SR_MODE_LOCAL_REC ,\n
                                   or iMode = ::ISS_SR_MODE_MIX_REC ,\n
                                   it is the command word list used by the local recognition, which is in the JSON format; \n
								   When iMode = ::ISS_SR_MODE_LOCAL_NLP ,\n
                                   It is the UTF8 code string in the JSON format for the semantic parsing purpose. \n
  
  * \retval             ISSErrID                    The error code returned by the speech recognition engine \n
  * \retval             ISS_SUCCESS                 Initiate a speech recognition successfully. \n
  * \retval             ISS_ERROR_INVALID_HANDLE    Invalid speech recognition handle\n
  * \retval             ISS_ERROR_INVALID_PARA      Invalid parameter. The scene or recognition mode input is incorrect.\n
  * \retval				ISS_ERROR_INVALID_JSON_FMT   The input JSON format is incorrect.\n
  * \retval			    ISS_ERROR_INVALID_JSON_INFO No required grammar information is extracted from the JSON input. \n
  * \retval				ISS_ERROR_BUILDING_GRM      After the configuration file CouldStartLocalRecogniseWhenBuildLocalGrm is set 0, if the local recognition is initiated when the local grammar is being built, this error code is returned. \n
*/
ISSErrID ISSAPI ISSSRStart(
    HISSSR      hISSSRObj,/* [In] Recognition handle */
    const char* szScene,  /* [In] The specified scene and the defined scope of the recognition
                                   Support: ISS_SR_SCENE_ALL,
                                         ISS_SR_SCENE_POI,
                                         ISS_SR_SCENE_CONTACTS,
                                         ISS_SR_SCENE_SELECT,
                                         ISS_SR_SCENE_CONFIRM,
										 ISS_SR_SCENE_CMDLIST_WITHALL (abandoned, but still supported)
										 (When the iMode is ISS_SR_MODE_LOCAL_CMDLIST, the scene is the “all scene”),
                                         ISS_SR_SCENE_SELECT_MVW (Ignore the parameter of iMode under this condition),
                                         ISS_SR_SCENE_CONFIRM_MVW (Ignore the parameter of iMode under this condition),
                                         ISS_SR_SCENE_ANSWER_CALL_MVW (Ignore the parameter of iMode under this condition)
                            */
    int         iMode,    /* [In] i_Mode = ISS_SR_MODE_CLOUD_REC, pure network recognition;
                                   i_Mode = ISS_SR_MODE_LOCAL_REC, pure local recognition;
                                   i_Mode = ISS_SR_MODE_MIX_REC, cloud and terminal mixed recognition;
                                   i_Mode = ISS_SR_MODE_LOCAL_CMDLIST (abandoned, but still supported), the local command word recognition
                                            , under which condition, ignoring the szScene parameter;
                                   i_Mode = ISS_SR_MODE_LOCAL_NLP, pure local semantics,
                                            , under which condition, ignoring the szScene parameter.
                            */
    const char* szCmd      /* [In] i_Mode = ISS_SR_MODE_LOCAL_CMDLIST (abandoned, but still supported)
                                   When i_Mode = ISS_SR_MODE_LOCAL_REC,
                                   or i_Mode = ISS_SR_MODE_MIX_REC,
                                   it is the command word list used by the local recognition, which is in the JSON format;
								   When i_Mode = ISS_SR_MODE_LOCAL_NLP,
                                   It is the UTF8 code string in the JSON format for the semantic parsing purpose. */
);

/** \brief Stop the recognition; \n
  *        When the function is executed, it does not mean that all the corresponding functions can be realized. The function is an asynchronous, reentrant and thread-safe function. \n
  * \param  hISSSRObj        [In] Recognition handle\n
  * \retval     ISSErrID                 The error code returned by the speech recognition engine \n
  * \retval     ISS_SUCCESS              The recognition is stopped successfully. \n
  * \retval     ISS_ERROR_INVALID_HANDLE Invalid speech recognition handle.\n
  * \retval     ISS_ERROR_INVALID_CALL   Error calls (The ::ISSSRStart is not called yet and the recognition does not start.) \n
*/
ISSErrID ISSAPI ISSSRStop(
    HISSSR hISSSRObj        /* [In] Recognition handle */
);
/** \brief  Parameter settings;\n
  *         When the function is executed, it means that the parameter has been set. It is a synchronous, non-reentrant and non-thread-safe function. \n
            The szParam parameter can only take the following values:\n
	        ::ISS_SR_PARAM_SPEECH_TIMEOUT \n
	        ::ISS_SR_PARAM_RESPONSE_TIMEOUT \n
	        ::ISS_SR_PARAM_SPEECH_TAIL \n
	        ::ISS_SR_PARAM_MVW_TIMEOUT \n
	        ::ISS_SR_PARAM_CITY \n
	        ::ISS_SR_PARAM_LONGTITUDE \n
	        ::ISS_SR_PARAM_LATITUDE \n
	        ::ISS_SR_PARAM_IAT_EXTEND_PARAMS \n
	        ::ISS_SR_PARAM_ACOUS_LANG \n
	        ::ISS_SR_PARAM_TRACE_LEVEL \n
	        ::ISS_SR_PARAM_DOUL_MIC_MAE_DENOISE \n
	        ::ISS_SR_PARAM_SINGLE_MIC_DENOISE \n
	        ::ISS_SR_PARAM_BARGE_IN \n
  * \param  hISSSRObj         [In] Recognition handle\n
  * \param  szParam           [In] Supported parameter  ISS_SR_PARAM_XXXX\n
  * \param  szParamValue      [In] The parameter value\n
  
 * \retval   ISSErrID                 The error code returned by the speech recognition engine \n
 * \retval   ISS_SUCCESS              The parameter is set successfully.\n
 * \retval   ISS_ERROR_INVALID_HANDLE Invalid speech recognition handle\n
 * \retval   ISS_ERROR_INVALID_PARA   Invalid parameter\n

 */
ISSErrID ISSAPI ISSSRSetParam(
    HISSSR hISSSRObj,        /* [In] Recognition handle */
    const char* szParam,     /* [In] Supported parameter
                                      ISS_SR_PARAM_XXXX
                               */
    const char* szParamValue  /* [In] Parameter value */
);
#if 0
/** \brief   Actively load the local recognition of the big resources transliteration. The ::ISS_SR_MSG_LoadBigSrResStatus message will be returned to notify whether the loading is successful or not. \n
  *         When the function is executed, it does not mean the big resources transliteration loading is successful. The function is an asynchronous, non-reentrant and non-thread-safe function.  \n
  * \param  hISSSRObj         [In] Recognition handle\n
  * \retval ISSErrID               The error code returned by the speech recognition engine\n
  * \retval ISS_SUCCESS            The resource is successfully transliterated. \n
  * \retval ISS_ERROR_INVALID_CALL Error calls\n
*/
ISSErrID ISSAPI ISSSRLoadBigSrRes(
    HISSSR hISSSRObj         /* [In] Recognition handle */
);
#endif
/** \brief   The input recording data: the default input recording is S16-LE, single track PCM audio with 16k Hz sampling rate; \n
            If the ::ISS_SR_PARAM_DOUL_MIC_MAE_DENOISE software dual microphone noise reduction is enabled, the imported recording shall be the double track PCM audio;\n
		    If the ::ISS_SR_PARAM_BARGE_IN software Bargin is enabled, the recording channel depends on the configuration of the BargeInAudioRecordChannel configuration file;\n
            When the function is executed, it means that the writing is successful or not. It is a synchronous, non-reentrant and non-thread-safe function. If the recordings need to be written many times, the function shall be called after it is returned; \n
            Import the audio data only when the recognition is enabled; \n
            To avoid the speech recognition engine internal buffer overflow, the pNumberOfWritten parameter is added. If the user program encounters *pNumberOfWritten != nNumberOfToWrite, users can \n
            rewrite the data a moment later or discard the buffer data. (Usually this scenario does not happen to the smart phone or the vehicle system due to their high-performance processors. It happens maybe because the written data is too big at a time or a test program \n
            is written); \n
            Please note that during the process of writing data, the speech recognition engine may call back the recognition function.\n
  * \param  hISSSRObj           [In] Recognition handle\n
  * \param  pSamples            [In] The buffer address of the input audio\n
  * \param  nNumberOfToWrite    [In] The number of the sampling points to be written, not the number of the bytes \n
  * \param  pNumberOfWritten    [Out] The actual number of sampling points being written \n
  * \retval     ISSErrID                 The error code returned by the speech recognition engine \n
  * \retval     ISS_SUCCESS              The recording is written successfully, but the size of the written recording may be 0. A user can determine whether the recording is completely written in based on *pNumberOfWritten != nNumberOfToWrite. \n
  * \retval     ISS_ERROR_INVALID_HANDLE Invalid speech recognition handle\n
  * \retval     ISS_ERROR_INVALID_PARA   Invalid parameter\n
  * \retval     ISS_ERROR_INVALID_CALL   Error calls (The ::ISSSRStart is not called yet and the recognition does not start.) \n
*/
ISSErrID ISSAPI ISSSRAppendAudioData(
    HISSSR          hISSSRObj,         /* [In] Recognition handle */
    short*          pSamples,          /* [In] The buffer address of the input audio*/
    unsigned int    nNumberOfToWrite,  /* [In] The number of the sampling points to be written, not the number of the bytes */
    unsigned int*   pNumberOfWritten    /* [Out] The actual number of sampling points being written */
);

/** \brief   If a user clicks the "I'm done" on the screen, it means that a user has stopped transferring the recording proactively. No more data will be written in; \n
  *        This function is used to stop writing data. When the function is executed, it means the data stops being written in. It is a synchronous, reentrant and thread-safe function. \n
  * \param  hISSSRObj        [In] Recognition handle\n
  * \retval     ISSErrID                 The error code returned by the speech recognition engine \n
  * \retval     ISS_SUCCESS              Successful\n
  * \retval     ISS_ERROR_INVALID_HANDLE Invalid speech recognition handle \n
  * \retval     ISS_ERROR_INVALID_CALL   Error calls (The ::ISSSRStart is not called yet and the recognition does not start.)\n
  * \retval		ISS_ERROR_NO_SPEECH      No user speech is detected. The recognition engine has stopped and no more data needs to be written in.\n
*/
ISSErrID ISSAPI ISSSREndAudioData(
    HISSSR hISSSRObj           /* [In] Recognition handle */
);

/** \brief      The personalized dictionary is uploaded asynchronously.\n
                The local personalized dictionary supports songs,singer,contact,apps,videos,albums,playlists,localcmds,\n
                The cloud personalized dictionary supports contact,hotwords.\n
                A message will be returned via callback to notify whether the personalized dictionary upload is successful or not. \n
                 ::ISS_SR_MSG_UpLoadDictToLocalStatus indicates whether the dictionary is successfully uploaded to local;\n
                ::ISS_SR_MSG_UpLoadDictToCloudStatus indicates whether the dictionary is successfully uploaded to cloud; \n
  *				When the function is executed, it only means that the corresponding string is written to the queen, but the grammar compiling does not start yet. It is an asynchronous, reentrant and thread-safe function. \n
                When a handle is created, \n
                the ISSSRUpLoadDict can be called to upload the dictionary without waiting to receive the ::ISS_SR_MSG_InitStatus message.\n
                If the value of the last parameter bOnlyUploadToCloud is 0: \n
                Every time when the ISSSRUpLoadDict interface is called, \n
                the ::ISS_SR_MSG_UpLoadDictToLocalStatus message is returned to notify whether the local personalized data upload is successful, \n
                and the ::ISS_SR_MSG_UpLoadDictToCloudStatus message is returned to notify whether the cloud personalized data upload is successful.\n
                If the value of the last parameter bOnlyUploadToCloud is not 0: Every time when the ISSSRUpLoadDict interface is called, \n
                the ::ISS_SR_MSG_UpLoadDictToCloudStatus message is returned to notify whether the cloud personalized data upload is successful. \n
  * \param  hISSSRObj            [In] Recognition handle\n
  * \param  szList               [In] Grammar information in the JSON format \n
  * \param  bOnlyUploadToCloud   [In] Whether the dictionary is only uploaded to the cloud. \n
  * \retval     ISSErrID                 The error code returned by the speech recognition engine \n
  * \retval     ISS_SUCCESS              An asynchronous grammar compiling task is added successfully. \n
  * \retval     ISS_ERROR_INVALID_HANDLE Invalid speech recognition handle \n
  * \retval     ISS_ERROR_INVALID_PARA   Invalid parameter value\n
* * \par Example grammar
*{\n
*    "grm": [\n
*        {\n
*            "dictname": "contact",\n
*            "dictcontant": [\n
*                { "name": "iFLYTEK","id": 0 },\n
*                { "name": "White snow","id": 1 },\n
*                { "name": "Junfeng liu",   "id": 2 },\n
*                { "name": "Yawei bai","id": 3 }\n
*            ]\n
*        },\n
*        {\n
*            "dictname": "singers",\n
*            "dictcontant": [\n
*                { "name": " Phoenix Legend ","id": 0 },\n
*                { "name": " Taylor Swift ","id": 1 },\n
*                { "name": " Aaron Carter ","id": 2 },\n
*                { "name": "S.H.E","id": 3 }\n
*            ]\n
*        },\n
*        {\n
*            "dictname": "songs",\n
*            "dictcontant": [\n
*                { "name": " Yesterday Once More ","id": 0 },\n
*                { "name": "Love Story","id": 1 },\n
*                { "name": " Young and Beautiful ","id": 2 },\n
*                { "name": " Let It Go ","id": 3 }\n
*            ]\n
*        },\n
*		{\n
*		"dictname": "albums",\n
*		"dictcontant": [\n
*		{ "name": " Fantasy ","id": 0 },\n
*		{ "name": "Still Fantasy","id": 1 },\n
*		{ "name": " Tranquil ","id": 2 }\n
*		]\n
*		},\n
*		{\n
*		"dictname": "playlists",\n
*		"dictcontant": [\n
*		{ "name": "My favorites","id": 0 },\n
*		{ "name": " Recently played ","id": 1 },\n
*		{ "name": " Classical music ","id": 2 }\n
*		]\n
*		},\n
*		{\n
*            "dictname": "apps",\n
*           "dictcontant": [\n
*                { "name": "iFLYTEK Yudian","id": 0 },\n
*               { "name": " Angry Birds ","id": 1 },\n
*                { "name": " Browser ","id": 2 }\n
*            ]\n
*        },\n
*        {\n
*            "dictname": "videos",\n
*            "dictcontant": [\n
*                { "name": " Tangled ","id": 0 },\n
*                { "name": "One Day","id": 1 },\n
*                { "name": "Life Is Beutiful","id": 2 },\n
*                { "name": " American Hustle","id": 3 },\n
*                { "name": " Urban Games","id": 4 },\n
*                { "name": "Deskmate","id": 5 }\n
*            ]\n
*        },\n
*        {\n
*            "dictname": "hotwords",\n
*            "dictcontant": [\n
*                { "name": " Navigate to the iFLYTEK","id": 0 },\n
*                { "name": "I would like to go to iFLYTEK ","id": 1 },\n
*                { "name": "Navigate to Anhui university","id": 2 },\n
*                { "name": "I would like to go to Anhui university","id": 3 },\n
*                { "name": "Navigate to Wanda Plaza","id": 4 }\n
*            ]\n
*        },\n
*        {\n
*            "dictname": "musercmd",\n
*            "dictcontant": [\n
*                { "name": "I love hefei","id": 0 },\n
*                { "name": "I love iFlytek","id": 1 }\n
*            ]\n
*        }\n
*    ]\n
*}\n
*/
ISSErrID ISSAPI ISSSRUpLoadDict(
    HISSSR hISSSRObj,          /* [In] Recognition handle */
    const char *szList,        /* [In] Grammar information in the JSON format */
    int bOnlyUploadToCloud      /* [In] Whether upload the dictionary only to the cloud. */
);

/** \brief   Obtain the version number; \n
  *          This function directly returns the version number. It is a synchronous, reentrant and thread-safe function \n
  * \return  const char* The version number string \n
*/
ISSAPI_PORT const char* ISSCALL ISSGetRevisionNum(void);

/** \brief     Input a text and obtain the processing results from the cloud; \n
  *            When the function is executed, the parsing result of the semantics can be directly obtained. It is a synchronous, non-reentrant and non-thread-safe function. (only used for the internal test) 
  * \param  hISSSRObj       [In] Recognition handle \n
  * \param  szText          [In] The input text equals to the recognition results when access to the cloud service script.\n
  * \param  szParam         [In] The additional parameter inputted when access to the cloud service script.\n
                                        If information like the latitude and longitude of the city is imported via ::ISSSRSetParam, \n
                                        there no need to import this information again.\n 
  * \param  pErr            [Out] Error code\n
  * \retval     ISSErrID                 The error code returned by the speech recognition engine \n
  * \retval     ISS_SUCCESS              Stop this recognition successfully.\n
  * \retval     ISS_ERROR_INVALID_HANDLE Invalid speech recognition handle \n
*/
ISSAPI_PORT const char* ISSCALL ISSSRMspSearch(
    HISSSR hISSSRObj,          /* [In] Recognition handle */
    const char* szText,        /* [In] The input text equals to the recognition results when access to the cloud service script. */
    const char* szParam,       /* [In] The additional parameter inputted when access to the cloud service script.
                                        If information like the latitude and longitude of the city is imported via ::ISSSRSetParam, 
                                        there no need to import this information again. */
    ISSErrID*   pErr            /* [Out] Error code */
);

/** \brief Input the text and obtain the local processing results;
  *        When the function is executed, the local parsing result can be obtained immediately. Please pay attention to the value range of the szScene that both the recognition handle and szText are valid. \n
           It is a synchronous, non-reentrant and non-thread-safe function. If the local semantics need to be parsed many times, a new parsing can be made only after the previous parsing is completed.\n
  * \param   hISSSRObj      [In] Recognition handle\n
  * \param   szText         [In] The input text equals to the recognition results when the local semantics are visited. \n
  * \param   szScene        [In] The specified scene and the defined scope of the recognition \n
                                              Support: \n
											        ::ISS_SR_SCENE_ALL \n
                                                    ::ISS_SR_SCENE_POI \n
                                                    ::ISS_SR_SCENE_CONTACTS \n
                                                    ::ISS_SR_SCENE_SELECT \n
                                                    ::ISS_SR_SCENE_CONFIRM \n
  * \param   pErr      i     [Out] Error code\n
  * \retval     ISSErrID                 The error code returned by the speech recognition engine \n
  * \retval     ISS_SUCCESS              Stop this recognition successfully.\n
  * \retval     ISS_ERROR_INVALID_HANDLE Invalid speech recognition handle \n
*/
ISSAPI_PORT const char* ISSCALL ISSSRLocalNli(
    HISSSR hISSSRObj,          /* [In] Recognition handle */
    const char* szText,        /* [In] The input text equals to the recognition results when access to the local semantics. */
    const char* szScene,       /* [In] The specified scene and the defined scope of the recognition
                                   Support: ISS_SR_SCENE_ALL
                                        ISS_SR_SCENE_POI
                                        ISS_SR_SCENE_CONTACTS
                                        ISS_SR_SCENE_SELECT
                                        ISS_SR_SCENE_CONFIRM */
    ISSErrID*   pErr           /* [Out] Error code   */
);

ISSAPI_PORT ISSErrID ISSCALL  ISSSRCheckGrmBuilding(
	HISSSR hISSSRObj,           /* [In] Recognition handle */
	const char* szDictName,     /* [In] dict name checked */
	const char* szGrmId,		/* [In] grm id */
	int* pStatus				/* [Out] status */
);

/** \brief  Set log config parameters \n
  * \param  iParamID     [In] ParamID\n
  * \param  szParamValue [In] ParamValue const char* type\n
  
  * \retval    ISSErrID						The error code returned by the speech synthesis engine.\n
  * \retval    ISS_SUCCESS					The user parameter is obtained successfully.\n
   * \retval   ISS_ERROR_INVALID_PARA       Invalid parameter.\n
 */
ISSErrID ISSAPI ISSSRSetLogCfgParam(
	const int iParamID, 
	const char* szParamValue
);

/** \brief  Set Keywords of the specified Mvw Scene \n
  * \param  hISSSRObj	[In] Recognition handle\n
  * \param  nMvwScene	[In] Mvw scene\n
  * \param  szWords		[In] New Keywords\n
  
  * \retval     ISSErrID                 The error code returned by the speech recognition engine \n
  * \retval     ISS_SUCCESS              Set keywords successfully. \n
  * \retval     ISS_ERROR_INVALID_HANDLE Invalid speech recognition handle \n
  * \retval     ISS_ERROR_INVALID_PARA   Invalid parameter value\n
 */
ISSErrID ISSAPI ISSSRSetMvwKeyWords(
	 HISSSR hISSSRObj,			/* [In] Recognition handle */
	 unsigned int nMvwScene,	/* [In] Mvw scene */
	 const char* szWords		/* [In] New Keywords */
);


/** \brief  Get all version information \n 
 *  \param  hISSSRObj	[In] Recognition handle\n

 *  \retval 	the kernel version infomation
 */
ISSAPI_PORT const char* ISSCALL ISSSRGetAllVersionInfo(HISSSR hISSSRObj);

#ifdef __cplusplus
}
#endif

#endif
