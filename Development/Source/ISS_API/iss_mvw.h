/*
Copyright:      1999-2014, iFLYTEK Co., Ltd.
File name:      iss_mvw.h
Description:    iFLYTEK Speech Suite – Multiple Voice Wakeup
                Brief Description:
                ISS : iFLYTEK Speech Suite;
                MVW : Multi Voice Wakeup;
Author:         iFLYTEK
Version:        1.0
Date:           2014-3-14
History:        Created on Mar. 14, 2014
*/
/** @defgroup group7 iFLYTEK Speech Suite – Multiple Voice Wakeup
 
 * @brief iss_mvw.h
 
 */

#ifndef _ISS_MVW_H_
#define _ISS_MVW_H_

#include "iss_types.h"
#include "iss_chipauth.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Please refer to the configuration files to check the scene ID information and wakeup word ID information.
#define ISS_MVW_SCENE_GLOBAL         (1)	///< Wakeup scene: global wakeup
#define ISS_MVW_SCENE_CONFIRM        (2)	///< Wakeup scene: confirm scene
#define ISS_MVW_SCENE_SELECT         (4)	///< Wakeup scene: select scene
#define ISS_MVW_SCENE_ANSWER_CALL    (8)	///< Wakeup scene: scene
#define ISS_MVW_SCENE_BUILD_GRM		 (16)    ///< Wakeup scene: scene
#define ISS_MVW_SCENE_ONESHOT		 (32)	///< Wakeup scence: oneshot
/** \brief Multiple voice wakeup handle 
*/

#define ISS_MVW_PARAM_AEC				"mvw_enable_aec"
#define ISS_MVW_PARAM_LSA				"mvw_enable_lsa"
#define ISS_MVW_PARAM_TMP_LOG_DIR		"TmpLogDir"                ///< Set tmp log directory for debugging
#define ISS_MVW_PARAM_THRESHOLD_LEVEL	"mvw_threshold_level"

#define ISS_MVW_PARAM_VALUE_ON                 "on"                      ///< On
#define ISS_MVW_PARAM_VALUE_OFF                "off"                     ///< Off

typedef void*   HISSMVW;

enum tagISS_MVW_LANG{
	ISS_MVW_LANG_CHN = 0,
	ISS_MVW_LANG_ENG = 1,
	ISS_MVW_LANG_BUTT
};

// Log level.According to the requirement to combine mask.
#define ISS_MVW_PARAM_LOG_LEVEL					(0X00000506)
#define ISS_MVW_VOLUME_LOG_LEVEL_ALL			("-1")	// all info
#define ISS_MVW_VOLUME_LOG_LEVEL_NONE			("0")	// none
#define ISS_MVW_VOLUME_LOG_LEVEL_CRIT			("1")	// critical info
#define ISS_MVW_VOLUME_LOG_LEVEL_ERROR			("2")	// error info
#define ISS_MVW_VOLUME_LOG_LEVEL_WARNING		("4")	// warnint info

// Log output.According to the requirement to combine mask.
#define ISS_MVW_PARAM_LOG_OUTPUT				(0X00000507)
#define ISS_MVW_VOLUME_LOG_OUTPUT_NONE			("0")	// none
#define ISS_MVW_VOLUME_LOG_OUTPUT_FILE			("1")	// file
#define ISS_MVW_VOLUME_LOG_OUTPUT_CONSOLE		("2")	// console（except for android）
#define ISS_MVW_VOLUME_LOG_OUTPUT_DEBUGGER		("4")	// debugger
#define ISS_MVW_VOLUME_LOG_OUTPUT_MSGBOX		("8")	// message box

// Log FileName
#define ISS_MVW_PARAM_LOG_FILE_NAME				(0X00000508)

/** Message type enumeration */
enum tagISS_MVW_MSG{
/** \brief  Whether the initialization is successful;\n
 *          After the multiple voice wakeup handle is created, the ISS_MVW_MSG_InitStatus message will be sent to notify whether the voice wakeup engine initialization is successfully or not.
 *  \param  wParam     ISSErrID type:\n
 *                      ISS_SUCCESS: The initialization is successful.\n
 *                      ISS_ERROR_FAIL: The initialization fails.\n
 *                      ISS_ERROR_OUT_OF_MEMORY: The memory is insufficient.\n
 *  \param  lParam   NULL 
 */
    ISS_MVW_MSG_InitStatus              =   40000,


/** \brief   Sound energy, which is returned once when a 25 ms recording is calculated (40Hz); \n
 *          Used for displaying the microphone effect in the interface.
 *  \param  wParam   The volume value, which ranges from 0 to 931.
 *  \param  lParam   NULL
 */
    ISS_MVW_MSG_VolumeLevel             =   40001,

/** \brief  Other errors.
 *  \param  wParam    ISSErrID type:\n
 *                     ISS_ERROR_CHIP_LOAD_INFO: Fails to obtain the authorization chip information.\\n
 *                     ISS_ERROR_CHIP_CRYPT_FAIL: The authorization chip decryption fails.\n
 *  \param  lParam     const char* type, error information
 */
    ISS_MVW_MSG_Error                   =   40002,

/** \brief   The multiple voice wakeup results with text are returned.
 *  \param  wParam   NULL
 *  \param  lParam   jason string, multiple voice wakeup result
 */
    ISS_MVW_MSG_Result                  =   40003,

/** \brief  timeout.
 *  \param  wParam   nScene
 *  \param  lParam   NULL
 */
    ISS_MVW_MSG_Timeout                 =   40004,

/** \brief  Ivw Optimize--Return value of Resource Update.
 *  \param  wParam   ISSErrID type
 *  \param  lParam   NULL
 */
    ISS_MVW_MSG_ResUpdate               =   40005,

/** \brief  Ivw Optimize--Return value of Resource Download.
 *  \param  wParam   ISSErrID type
 *  \param  lParam   NULL
 */
    ISS_MVW_MSG_ResDownload             =   40006
};

/** \brief  The callback interface of the multiple wakeup;\n
 *          If a user utters a multiple wakeup word, the multiple wakeup engine will notify the caller that there is a wakeup via this callback function;
 *          If a user utters the wakeup words many times, the callback function will be called repeatedly but not at the same time. 
 *          During this process, the user program can call the ::ISSMVWStop to stop the multiple wakeup.
 * \param	pUsrArg     [In] User defined parameter\n
 * \param	nMvwScene   [In] Multiple wakeup scene\n
 * \param   nMvwId      [In] Wakeup word ID\n
 * \param   nMvwScore   [In] Wakeup confidence score \n
 * \param   lParam      [In] The JSON format string, which provides additional information like the front endpoint value migration and the back endpoint value migration. \n
 * \return  None
 */
typedef void (ISSCALLBACK *Proc_ISSMVWOnWakeup)(
    void*         pUsrArg,     /* [In] User defined parameter */
    unsigned int  nMvwScene,   /* [In] Multiple wakeup scene */
    unsigned int  nMvwId,      /* [In] Wakeup word ID */
    signed int    nMvwScore,    /* [In] Wakeup confidence score */
	const char*	  lParam		/* [In] The JSON format string, which provides additional information like the front endpoint value migration and the back endpoint value migration.*/
);

/** \brief The speech suite sends various messages to user via the Proc_MVWOnMsgProc callback.\n
 *  \param   pUsrArg   [In] User defined parameter \n
 *  \param   uMsg      [In] The message identifier \n
 *  \param   wParam    [In] Message additional information\n
 *  \param   lParam    [In] Message additional information \n
 *  \return  None\n
 */
typedef void (ISSCALLBACK *Proc_MVWOnMsgProc)(
    const void*         pUsrArg,     /* [In] User defined parameter */
    unsigned int        uMsg,        /* [In] The message identifier */
    unsigned int        wParam,      /* [In] Message additional information */
    const void*         lParam        /* [In] Message additional information */
);

/** \brief  Create a multiple voice wakeup handle; \n
 *          It is a synchronous, non-reentrant and non-thread-safe function; \n
 *          Multiple instances cannot be created.\n
 * \param	phISSMVW                [Out] Multiple wakeup handle \n
 * \param	szResourceDir           [In] Resource directory, usually: adopts the GBK code format in the Windows Chinese environment, and the UTF-8 code format in the Linux environment.\n
 * \param   pfnOnWakeup             [In] Callback interface\n
 * \param   pcbOnISSChipCryptError  [In] The callback function pointer of the chip authorization failure: Only for the encrypted chip version, valid function pointer needs to be entered. Otherwise, enter NULL directly. \n
 * \param   pUsrArg                 [In] User defined parameter\n
 * \retval  ISSErrID                  The error code returned by the multiple voice wakeup engine.
 * \retval  ISS_SUCCESS               The multiple voice wakeup handle is created successfully. \n
 * \retval  ISS_ERROR_FILE_NOT_FOUND  No corresponding resource file is found. \n
 * \retval  ISS_ERROR_INVALID_PARA    Invalid parameter value \n
 * \retval  ISS_ERROR_OUT_OF_MEMORY   Insufficient memory \n
 * \retval  ISS_ERROR_INVALID_DATA    Resource data error n
 */
ISSErrID ISSAPI ISSMVWCreate(
    HISSMVW*             phISSMVW,        /* [Out] Multiple wakeup handle */
    const char*          szResourceDir,   /* [In] Resource directory, usually:
                                                  adopts the GBK code format in the Windows Chinese environment,
                                                  and the UTF-8 code format in the Linux environment. */
    Proc_ISSMVWOnWakeup  pfnOnWakeup,     /* [In] Callback interface */
    Proc_OnISSChipCryptError pcbOnISSChipCryptError, /* [In] The callback function pointer of the chip authorization failure. */
    void*                pUsrArg          /* [in] User defined parameter */
);
/** \brief  Create a multiple voice wakeup handle; \n
 *          It is a synchronous, non-reentrant and non-thread-safe function; \n
 *          Multiple instances cannot be created. \n
 * \param	phISSMVW                [Out] Multiple wakeup handle \n
 * \param	szResourceDir           [In] Resource directory, usually: adopts the UNICODE-16 format in the Windows vc environment, and the UNICODE-32 code format in the Linux gcc environment. \n
 * \param   pfnOnWakeup             [In] Callback interface \n
 * \param   pcbOnISSChipCryptError  [In] The callback function pointer of the chip authorization failure: Only for the encrypted chip version, valid function pointer needs to be entered. Otherwise, enter NULL directly. \n
 * \param   pUsrArg                 [In] User defined parameter\n
 * \retval  ISSErrID                  The error code returned by the multiple voice wakeup engine
 * \retval  ISS_SUCCESS               The multiple voice wakeup handle is created successfully.\n
 * \retval  ISS_ERROR_FILE_NOT_FOUND  No corresponding resource file is found. \n
 * \retval  ISS_ERROR_INVALID_PARA    Invalid parameter value \n
 * \retval  ISS_ERROR_OUT_OF_MEMORY   Insufficient memory \n
 * \retval  ISS_ERROR_INVALID_DATA    Resource data error \n
 */
ISSErrID ISSAPI ISSMVWCreateW(
    HISSMVW*             phISSMVW,         /* [Out] Multiple wakeup handle */
    const wchar_t*       szResourceDir,    /* [In] Resource directory, usually:
                                                   adopts the UNICODE-16 format in the Windows vc environment,
                                                   and the UNICODE-32 code format in the Linux gcc environment. */
    Proc_ISSMVWOnWakeup  pfnOnWakeup,      /* [In] Callback interface */
    Proc_OnISSChipCryptError pcbOnISSChipCryptError, /* [In] The callback function pointer of the chip authorization failure. */
    void*                pUsrArg           /* [in] User defined parameter */
);

/** \brief  Create a multiple voice wakeup handle; \n
 *          It is a synchronous, non-reentrant and non-thread-safe function; \n
 *          Multiple instances cannot be created.\n
 * \param	phISSMVW                [Out] Multiple wakeup handle \n
 * \param	szResourceDir           [In] Resource directory, usually: adopts the GBK code format in the Windows Chinese environment, and the UTF-8 code format in the Linux environment.\n
 * \param   pfnMVWOnMsgProc         [In] Message callback interface\n
 * \param   pUsrArg                 [In] User defined parameter\n
 * \retval  ISSErrID                  The error code returned by the multiple voice wakeup engine.
 * \retval  ISS_SUCCESS               The multiple voice wakeup handle is created successfully. \n
 * \retval  ISS_ERROR_FILE_NOT_FOUND  No corresponding resource file is found. \n
 * \retval  ISS_ERROR_INVALID_PARA    Invalid parameter value \n
 * \retval  ISS_ERROR_OUT_OF_MEMORY   Insufficient memory \n
 * \retval  ISS_ERROR_INVALID_DATA    Resource data error n
 */
ISSErrID ISSAPI ISSMVWCreateEx(
    HISSMVW*             phISSMVW,        /* [Out] 唤醒句柄 */
    const char*          szResourceDir,  /* [In] 资源目录,一般情况下:
                                                在windows中文环境下为GBK编码,
                                                在linux环境下为UTF-8编码 */
    Proc_MVWOnMsgProc    pfnMVWOnMsgProc,    /* [In] 回调接口 */
    void*                pUsrArg         /* [in] 用户自定义参数 */
); 

/** \brief  Create a multiple voice wakeup handle; \n
 *          It is a synchronous, non-reentrant and non-thread-safe function; \n
 *          Multiple instances cannot be created. \n
 * \param	phISSMVW                [Out] Multiple wakeup handle \n
 * \param	szResourceDir           [In] Resource directory, usually: adopts the UNICODE-16 format in the Windows vc environment, and the UNICODE-32 code format in the Linux gcc environment. \n
 * \param   pfnMVWOnMsgProc         [In] Message callback interface \n
 * \param   pUsrArg                 [In] User defined parameter\n
 * \retval  ISSErrID                  The error code returned by the multiple voice wakeup engine
 * \retval  ISS_SUCCESS               The multiple voice wakeup handle is created successfully.\n
 * \retval  ISS_ERROR_FILE_NOT_FOUND  No corresponding resource file is found. \n
 * \retval  ISS_ERROR_INVALID_PARA    Invalid parameter value \n
 * \retval  ISS_ERROR_OUT_OF_MEMORY   Insufficient memory \n
 * \retval  ISS_ERROR_INVALID_DATA    Resource data error \n
 */
ISSErrID ISSAPI ISSMVWCreateWEx(
    HISSMVW*             phISSMVW,        /* [Out] 唤醒句柄 */
    const wchar_t*       szResourceDir,  /* [In] 资源目录,一般情况下:
                                                在windows vc环境下为UNICODE-16编码,
                                                在linux gcc环境下为UNICODE-32编码 */
    Proc_MVWOnMsgProc    pfnMVWOnMsgProc,    /* [In] 回调接口 */
    void*                pUsrArg         /* [in] 用户自定义参数 */
);

/** \brief  Destroy multiple voice wakeup handle.
 *          It is a synchronous, non-reentrant and non-thread-safe function. \n
 * \param	hISSMVW     [In] Multiple wakeup handle \n
 * \retval  ISSErrID                 The error code returned by the multiple voice wakeup engine \n
 * \retval  ISS_SUCCESS              Destroy the multiple voice wakeup handle successfully with resources being released.\n
 * \retval  ISS_ERROR_INVALID_HANDLE Invalid voice wakeup handle \n
 */
ISSErrID ISSAPI ISSMVWDestroy(
    HISSMVW  hISSMVW    /* [In] Wakeup handle */
);

/** \brief  Set some parameters; \n
 *          It is a synchronous, reentrant and thread-safe function. \n
 * \param	hISSMVW     [In] Multiple wakeup handle \n
 * \param	szParam   [In] parameter \n
 * \param	szParamValue      [In] parameter value\n
 * \retval  ISSErrID                 The error code returned by the multiple voice wakeup engine \n
 * \retval  ISS_SUCCESS              The threshold is set successfully. \n
 * \retval  ISS_ERROR_INVALID_HANDLE Invalid voice wakeup handle \n 
 */
ISSErrID ISSAPI ISSMVWSetParam(
	HISSMVW hISSMVW,			/* [In] Wakeup handle */
	const char* szParam,		/* [In] parameter */
	const char* szParamValue	/* [In] parameter value */
);

/** \brief  Set the multiple voice wakeup threshold; \n
 *          It is a synchronous, reentrant and thread-safe function. \n
 * \param	hISSMVW     [In] Multiple wakeup handle \n
 * \param	nMvwScene   [In] Multiple wakeup scene \n
 * \param	nMvwId      [In] Wakeup word ID\n
 * \param	nThreshold  [In] The threshold value \n
 * \retval  ISSErrID                 The error code returned by the multiple voice wakeup engine \n
 * \retval  ISS_SUCCESS              The threshold is set successfully. \n
 * \retval  ISS_ERROR_INVALID_HANDLE Invalid voice wakeup handle \n 
 */
ISSErrID ISSAPI ISSMVWSetThreshold(
    HISSMVW       hISSMVW,         /* [In] Wakeup handle */
    unsigned int  nMvwScene,       /* [In] Multiple wakeup scene */
    unsigned int  nMvwId,          /* [In] Wakeup word ID */
    int           nThreshold       /* [In] The threshold value */
);

/** \brief  Start multiple voice wakeup;\n
 *          It is a synchronous, non-reentrant and non-thread-safe function. \n
 * \param	hISSMVW     [In] Multiple wakeup handle \n
 * \param	nMvwScene   [In] Multiple wakeup scene \n
 * \retval  ISSErrID                 The error code returned by the multiple voice wakeup engine \n
 * \retval  ISS_SUCCESS              Start the multiple voice wakeup successfully. \n
 * \retval  ISS_ERROR_INVALID_HANDLE Invalid voice wakeup handle \n
 */
ISSErrID ISSAPI ISSMVWStart(
    HISSMVW  hISSMVW,              /* [In] Multiple wakeup handle */
    unsigned int  nMvwScene        /* [In] Multiple wakeup scene */
);
/** \brief  Add scene to Start multiple voice wakeup;\n
 *          It is a synchronous, non-reentrant and non-thread-safe function. \n
 * \param	hISSMVW     [In] Multiple wakeup handle \n
 * \param	nMvwScene   [In] Multiple wakeup scene \n
 * \retval  ISSErrID                 The error code returned by the multiple voice wakeup engine \n
 * \retval  ISS_SUCCESS              Start the multiple voice wakeup successfully. \n
 * \retval  ISS_ERROR_INVALID_HANDLE Invalid voice wakeup handle \n
 */
ISSErrID ISSAPI ISSMVWAddStartScene(
    HISSMVW  hISSMVW,              /* [In] Multiple wakeup handle */
    unsigned int  nMvwScene        /* [In] Multiple wakeup scene */
);

/** \brief  The imported audio data. Support for 16k sampling rate, S16-LE, single track audio.\n
 *          It is a synchronous, non-reentrant and non-thread-safe function; \n
 *          The audio data can be inputted only when the wakeup is enabled; \n
 *          To avoid the speech recognition engine internal buffer overflow,
 *          the pNumberOfWritten parameter is added. If the user program encounters *pNumberOfWritten != nNumberOfToWrite, users can
 *          rewrite the data a moment later or discard the buffer data. (Usually this scenario does not happen to the smart phone or the vehicle system due to their high-performance processors.
 *          It happens maybe because the written data is too big at a time or a test program
 *          is written).\n
 *          Please note that the wakeup function may be called back by the multiple voice wakeup engine when the data is being written in.\n
 *          The interface can be called only in a thread.\n
 * \param	hISSMVW             [In] Wakeup handle\n
 * \param	pSamples            [In] The buffer address of the imported audio \n
 * \param	nNumberOfToWrite    [In] The number of the sampling points to be written, not the number of the bytes \n
 * \param	pNumberOfWritten    [Out] The actual number of sampling points being written \n
 * \retval  ISSErrID                 The error code returned by the multiple voice wakeup engine \n
 * \retval  ISS_SUCCESS              The recording is written successfully, but the size of the written recording may be 0. A user can determine whether the recording is completely written in based on *pNumberOfWritten != nNumberOfToWrite.\n
 * \retval  ISS_ERROR_INVALID_HANDLE Invalid voice wakeup handle \n
 * \retval  ISS_ERROR_INVALID_PARA   Invalid parameter value\n
 * \retval  ISS_ERROR_INVALID_CALL   Error calls(The ::ISSMVWStart is not called yet and the wakeup does not start.) \n
 */
ISSErrID ISSAPI ISSMVWAppendAudioData(
    HISSMVW          hISSMVW,             /* [In] Multiple wakeup handle */
    short*           pSamples,            /* [In] The buffer address of the imported audio */
    unsigned int     nNumberOfToWrite,    /* [In] The number of the sampling points to be written, not the number of the bytes */
    unsigned int*    pNumberOfWritten     /* [Out] The actual number of sampling points being written */
);

/** \brief  Stop multiple voice wakeup; \n
 *          It is a synchronous, non-reentrant and non-thread-safe function.
 *          The user program can call the ISSMVWStop to stop the wakeup engine during the Proc_ISSMVWOnWakeup callback progress. 
 * \param	hISSMVW    [In] Wakeup handle \n
 * \retval  ISSErrID                 The error code returned by the multiple voice wakeup engine. \n
 * \retval  ISS_SUCCESS              The voice wakeup is stopped successfully. \n
 * \retval  ISS_ERROR_INVALID_HANDLE Invalid voice wakeup handle \n
 */
ISSErrID ISSAPI ISSMVWStop(
    HISSMVW  hISSMVW    /* [In] Wakeup handle */
);

/** \brief  Stop multiple voice wakeup; \n
 *          It is a synchronous, non-reentrant and non-thread-safe function.
 *          The user program can call the ISSMVWStop to stop the wakeup engine during the Proc_ISSMVWOnWakeup callback progress. 
 * \param	hISSMVW    [In] Wakeup handle \n
 * \param	nMvwScene   [In] Multiple wakeup scene \n
 * \retval  ISSErrID                 The error code returned by the multiple voice wakeup engine. \n
 * \retval  ISS_SUCCESS              The voice wakeup is stopped successfully. \n
 * \retval  ISS_ERROR_INVALID_HANDLE Invalid voice wakeup handle \n
 */
ISSErrID ISSAPI ISSMVWStopScene(
    HISSMVW  hISSMVW,    /* [In] Wakeup handle */
	unsigned int  nMvwScene   /* [In] Multiple wakeup scene */
);

/**\brief 设置唤醒词
 *\param hISSMVW    [In] 唤醒句柄
 *\param nMvwScene  [In] 唤醒场景.目前只支持值0 Global全局唤醒。取值见issmvw.cfg文件
 *\param szWords     [In] 新唤醒词列表. utf-8编码的Json格式的字符串.如果保留原唤醒词,需要将原唤醒词列出.\n
                                            { "Keywords" : [   {"KeyWordId" : 0, "KeyWord" : "唤醒词一", "DefaultThreshold40": 0},\n
                                                                      {"KeyWordId" : 1, "KeyWord" : "唤醒词二", "DefaultThreshold40": 0},\n
                                                                      {"KeyWordId" : 2, "KeyWord" : "唤醒词三", "DefaultThreshold40": 0} ]\n
                                            }\n
                                   字段解释:\n
                                   KeyWordId,唤醒词ID 唤醒后由回调函数返回给应用\n
                                   KeyWord, 唤醒词值 \n
                                   DefaultThreshold, 唤醒门槛 默认0 如果某个词唤醒效果不好设小此值\n
 *\retval  函数返回值
*/
ISSErrID ISSAPI ISSMVWSetKeyWords(
				  HISSMVW  hISSMVW,    /* [In] 唤醒句柄 */
                  unsigned int nMvwScene,/*[In] 唤醒场景.目前只支持值0 Global全局唤醒。取值见issmvw.cfg文件*/
                  const char* szWords); /*[In]新唤醒词*/

/**\brief  清空已设置唤醒词, 使用issmvw.cfg文件中配置的默认唤醒词.
 *\param hISSMVW    [In] 唤醒句柄
 *\param  nMvwScene    [In] 唤醒场景.目前只支持值0 Global全局唤醒。取值见issmvw.cfg文件
 *\retval 函数返回值
*/               
ISSErrID ISSAPI ISSMVWSetDefaultKeyWords(
	HISSMVW  hISSMVW,    /* [In] 唤醒句柄 */
	unsigned int nMvwScene);
/*
\brief  设置唤醒的语种,此函数必须在创建实例前调用
\param nLangType    [In] 语种类型
\retval 函数返回值
*/               
ISSErrID ISSAPI ISSMVWSetMvwLanguage(unsigned int nLangType);

/*
\brief  判断是否可以送录音，返回true代表可以送录音,返回false代表不可以送录音
\retval 
*/               
bool ISSAPI ISSMVWIsCouldAppendAudioData();

/** \brief  Set log config parameters \n
  * \param  iParamID     [In] ParamID\n
  * \param  szParamValue [In] ParamValue const char* type\n
  
  * \retval    ISSErrID						The error code returned by the speech synthesis engine.\n
  * \retval    ISS_SUCCESS					The user parameter is obtained successfully.\n
   * \retval   ISS_ERROR_INVALID_PARA       Invalid parameter.\n
 */
ISSErrID ISSAPI ISSMVWSetLogCfgParam(
	const int iParamID, 
	const char* szParamValue
);

/*
\brief  获取唤醒引擎版本号
\param hISSMVW 唤醒句柄    

\retval 唤醒引擎版本信息
*/
ISSAPI_PORT const char* ISSCALL ISSMVWGetVersion(HISSMVW  hISSMVW);

ISSErrID ISSAPI ISSMVWSetIvwOptimizeUpload(
	HISSMVW         hISSMVW,     /* [In] 唤醒句柄 */
	bool			bEnableFlag, /* [In] 关闭或打开唤醒闭环优化音频上传 */
	unsigned int    uMaxAudioCnt /* [In] 上传音频最大条数 */
);

ISSErrID ISSAPI ISSMVWSetIvwOptimizeDownload(
	HISSMVW         hISSMVW     /* [In] 唤醒句柄 */
);

#ifdef __cplusplus
}
#endif

#endif  /* _ISS_MVW_H_ */
