/******************************************************************************
Copyright:      1999-2013, iFLYTEK Co., Ltd.
File name:      iss_tts.h
Description:    iFLYTEK Speech Suite – Text to Speech.
                Brief Description:
                ISS: iFLYTEK Speech Suite;
                TTS: Text to Speech;
Author:         iFLYTEK
Version:        1.0
Date:           2013-10-10
History:        Created on Oct. 10, 2013
*******************************************************************************/
/** @defgroup group10 iFLYTEK Speech Suite – Text to Speech
 
 * @brief iss_tts.h
 
 */
#ifndef _ISS_TTS_H_
#define _ISS_TTS_H_

#include "iss_types.h"
#include "iss_chipauth.h"

#ifdef __cplusplus
extern "C" {
#endif

/**< Text to speech resource handle */
typedef void* HISSTTSRES;

/**< Text to speech handle */
typedef void* HISSTTS;

///< The code page of the input TTS text
typedef enum ISS_TTS_CODEPAGE {
    ISS_TTS_CODEPAGE_GBK = 936,
    ISS_TTS_CODEPAGE_UTF16LE = 1200,
    ISS_TTS_CODEPAGE_UTF8 = 65001
} ISSTTSCodePage;

#define ISS_TTS_RES_MODE_FILE_HANDLE 0	/*!< TTS Res load type. by file handle */
#define ISS_TTS_RES_MODE_MEMORY		 1	/*!< TTS Res load type. load to memory */

#define ISS_TTS_PARAM_SPEAKER (0x00000500)
typedef enum tagESpeaker {
#ifdef __QNX650__
	ivTTS_ROLE_CATHERINE = 69020,  /*!< Catherine (female, US English) */
	ivTTS_ROLE_XIAOJIE = 65060,       /*!< Xiaojie   (female, Chinese) */
#else
    ivTTS_ROLE_TIANCHANG = 1,   /**< Tianchang (female, Chinese) */
    ivTTS_ROLE_WENJING = 2,     /**< Wenjing (female, Chinese) */
    ivTTS_ROLE_XIAOYAN = 3,     /**< Xiaoyan (female, Chinese) */
    ivTTS_ROLE_YANPING = 3,     /**< Xiaoyan (female, Chinese) */
    ivTTS_ROLE_XIAOFENG = 4,    /**< Xiaofeng (male, Chinese) */
    ivTTS_ROLE_YUFENG = 4,      /**< Xiaofeng (male, Chinese) */
    ivTTS_ROLE_SHERRI = 5,      /**< Sherri (female, US English) */
    ivTTS_ROLE_XIAOJIN = 6,     /**< Xiaojin (female, Chinese) */
    ivTTS_ROLE_NANNAN = 7,      /**< Nannan (child, Chinese) */
    ivTTS_ROLE_XIAOJING = 8,    /**< Xiaojing (female, Chinese) */
    ivTTS_ROLE_JIAJIA = 9,      /**< Jiajia (girl, Chinese) */
    ivTTS_ROLE_YUER = 10,       /**< Yuer (female, Chinese) */
    ivTTS_ROLE_XIAOQIAN = 11,   /**< Xiaoqian (female, Chinese Northeast) */
    ivTTS_ROLE_LAOMA = 12,      /**< Laoma (male, Chinese) */
    ivTTS_ROLE_BUSH = 13,       /**< Bush (male, US English) */
    ivTTS_ROLE_XIAORONG = 14,   /**< Xiaorong (female, Chinese Szechwan) */
    ivTTS_ROLE_XIAOMEI = 15,    /**< Xiaomei (female, Cantonese) */
    ivTTS_ROLE_ANNI = 16,       /**< Anni (female, Chinese) */
    ivTTS_ROLE_JOHN = 17,       /**< John (male, US English) */
    ivTTS_ROLE_ANITA = 18,      /**< Anita (female, British English) */
    ivTTS_ROLE_TERRY = 19,      /**< Terry (female, US English) */
    ivTTS_ROLE_CATHERINE = 20,  /**< Catherine (female, US English) */
    ivTTS_ROLE_TERRYW = 21,     /**< Terry (female, US English Word) */
    ivTTS_ROLE_XIAOLIN = 22,    /**< Xiaolin (female, Chinese) */
    ivTTS_ROLE_XIAOMENG = 23,   /**< Xiaomeng (female, Chinese) */
    ivTTS_ROLE_XIAOQIANG = 24,  /**< Xiaoqiang (male, Chinese) */
    ivTTS_ROLE_XIAOKUN = 25,    /**< XiaoKun (male, Chinese) */
    ivTTS_ROLE_JIUXU = 51,      /**< Jiu Xu (male, Chinese) */
    ivTTS_ROLE_DUOXU = 52,      /**< Duo Xu (male, Chinese) */
    ivTTS_ROLE_XIAOPING = 53,   /**< Xiaoping (female, Chinese) */
    ivTTS_ROLE_DONALDDUCK = 54, /**< Donald Duck (male, Chinese) */
    ivTTS_ROLE_BABYXU = 55,     /**< Baby Xu (child, Chinese) */
    ivTTS_ROLE_DALONG = 56,     /**< Dalong (male, Cantonese) */
    ivTTS_ROLE_TOM = 57,        /**< Tom (male, US English) */
    ivTTS_ROLE_MINGMA = 58,		/**< MingMa (male, Chinese) */
	ivTTS_ROLE_BAOMA = 59,		/**< BaoMa (child, Chinese) */
	ivTTS_ROLE_BELLA = 60,		/**< Bella (child, US English) */
	ivTTS_ROLE_USER = 99,		/**< user defined */
	/* The customized pronunciation person VID, ranging from 50,000 to 100,000 */
	ivTTS_ROLE_XIAORUI = 50001,  	/**< Xiaorui   (female, Chinese) */
	ivTTS_ROLE_XIAOSHI = 51180,      /**< Xiaoshi   (female, Chinese) */
	ivTTS_ROLE_XIAOYAO = 50003,      /**< Xiaoyao   (female, Chinese) */
	ivTTS_ROLE_XIAOYANG = 50004,     /**< Xiaoyang  (male, Chinese) */
	ivTTS_ROLE_XIAOHONG = 50005,     /**< Xiaohong  (male, Chinese) */
	ivTTS_ROLE_XIAOHUI = 50006,      /**< Xiaohui   (male, Chinese) */
	ivTTS_ROLE_XIAOBO = 50007,       /**< Xiaobo    (male, Chinese) */
	ivTTS_ROLE_XIAOWANZI = 50008,    /**< Xiaowanzi (female, Chinese) */
	ivTTS_ROLE_XIAOXIN = 50009,      /**< Xiaoxin   (male, Chinese) */
	ivTTS_ROLE_XIAOMA = 50010,       /**< Xiaoma    (male, Chinese) */
	ivTTS_ROLE_XIAOLING = 50011,     /**< Xiaoling  (female, Chinese) */
	ivTTS_ROLE_XIAOXUE = 50110,      /**< Xiaoxue   (female, Chinese) */
	ivTTS_ROLE_XIAOJIE = 50130,      /**< Xiaojie   (female, Chinese) */
	ivTTS_ROLE_XIAONUO = 50160,      /**< Xiaonuo   (female, Chinese) */
	ivTTS_ROLE_YIFENG = 51090,       /**< Yifeng   (female, Chinese) */
	ivTTS_ROLE_XIAOFANG = 51020,       /**< Xiaofang   (child, Chinese) */
#endif
} ESpeaker;


#define ISS_TTS_PARAM_VOICE_SPEED (0x00000502) /*!< Used to set the TTS voice speed */
#define ISS_TTS_SPEED_MIN (-32768)             /*!< The slowest speed */
#define ISS_TTS_SPEED_NORMAL_DEFAULT (0)       /*!< The normal speed by default */
#define ISS_TTS_SPEED_MAX (+32767)             /*!< The fastest speed */

#define ISS_TTS_PARAM_VOICE_PITCH (0x00000503) /*!< Used to set the 
TTS voice pitch */
#define ISS_TTS_PITCH_MIN (-32768)             /*!< The lowest pitch */
#define ISS_TTS_PITCH_NORMAL_DEFAULT (0)       /*!< The normal pitch by default */
#define ISS_TTS_PITCH_MAX (+32767)             /*!< The highest pitch */

#define ISS_TTS_PARAM_VOLUME (0x00000504)   /*!< Used to set the TTS voice volume */
#define ISS_TTS_VOLUME_MIN (-32768)         /*!< The minimum volume */
#define ISS_TTS_VOLUME_NORMAL (0)           /*!< The normal volume */
#define ISS_TTS_VOLUME_MAX_DEFAULT (+32767) /*!< The maximum volume by default */

#define ISS_TTS_PARAM_USERMODE			(0x00000701)		/*!< user's mode */
/* constants for values of parameter ivTTS_PARAM_USERMODE(ivTTS_PARAM_NAVIGATION_MODE) */
#define ISS_TTS_VOLUME_USE_NORMAL				0			/*!< synthesize in the Mode of Normal */
#define ISS_TTS_VOLUME_USE_NAVIGATION			1			/*!< synthesize in the Mode of Navigation */
#define ISS_TTS_VOLUME_USE_MOBILE				2			/*!< synthesize in the Mode of Mobile */
#define ISS_TTS_VOLUME_USE_EDUCATION			3			/*!< synthesize in the Mode of Education */
#define ISS_TTS_VOLUME_USE_TV				    4			/*!< synthesize in the Mode of TV */

#define ISS_TTS_PARAM_VOLUME_INCREASE	(0X00000505)		/* volume value increase */
#define ISS_TTS_VOLUME_INCREASE_MIN				0		/* minimized volume (default) */
#define ISS_TTS_VOLUME_INCREASE_MAX				10		/* maximized volume */

#define ISS_TTS_PARAM_TMP_LOG_DIR       (0X00000606)               /*Set tmp log directory for debugging*/

// Log level.According to the requirement to combine mask.
#define ISS_TTS_PARAM_LOG_LEVEL					(0X00000506)
#define ISS_TTS_VOLUME_LOG_LEVEL_ALL			("-1")	// all info
#define ISS_TTS_VOLUME_LOG_LEVEL_NONE			("0")	// none
#define ISS_TTS_VOLUME_LOG_LEVEL_CRIT			("1")	// critical info
#define ISS_TTS_VOLUME_LOG_LEVEL_ERROR			("2")	// error info
#define ISS_TTS_VOLUME_LOG_LEVEL_WARNING		("4")	// warnint info

// Log output.According to the requirement to combine mask.
#define ISS_TTS_PARAM_LOG_OUTPUT				(0X00000507)
#define ISS_TTS_VOLUME_LOG_OUTPUT_NONE			("0")	// none
#define ISS_TTS_VOLUME_LOG_OUTPUT_FILE			("1")	// file
#define ISS_TTS_VOLUME_LOG_OUTPUT_CONSOLE		("2")	// console（except for android）
#define ISS_TTS_VOLUME_LOG_OUTPUT_DEBUGGER		("4")	// debugger
#define ISS_TTS_VOLUME_LOG_OUTPUT_MSGBOX		("8")	// message box

// Log FileName
#define ISS_TTS_PARAM_LOG_FILE_NAME				(0X00000508)

/** \brief Create the text to speech resource handle;
  *        When the function is executed, it means the handle creation is completed. Please note that the function is a synchronous, reentrant and thread-safe function.\n
  * \param phISSTTSRES     [Out] TTS resource handle\n
  * \param szResource      [In] Resource directory, usually adopting the GBK code format in the Windows Chinese environment, and the UTF-8 code format in the Linux environment. \n                                        
  * \param iResMode        [In] The way how the resource is loaded: when iResMode = 0, the file handle is opened. \n  
                                                When iResMode = 1, the memory is loaded directly. \n
  * \retval      ISSErrID                 The error code returned by the speech synthesis engine.\n
  * \retval      ISS_SUCCESS              The text to speech resource handle is created successfully; \n
  * \retval      ISS_ERROR_FILE_NOT_FOUND No corresponding resource file is found; \n
  * \retval      ISS_ERROR_INVALID_PARA   Invalid parameter value; \n
  * \retval      ISS_ERROR_OUT_OF_MEMORY  The memory is insufficient; \n
*/
ISSErrID ISSAPI ISSTTSInitRes(
    HISSTTSRES* phISSTTSRES,   /* [Out] TTS resource handle */
    const char* szResource, /* [In] Resource directory, usually:
                                       adopting the GBK code format in the Windows Chinese environment,
                                       and the UTF-8 code format in the Linux environment. */
    int iResMode, /* The way how the resource is loaded:
                    iResMode = 0   The file handle is opened.
                    iResMode = 1   The memory is loaded directly */
	const char*		pcSpeakerName
    );
/** \brief Create the text to speech resource handle;
  *        When the function is executed, it means the handle creation is completed. Please note that the function is a synchronous, reentrant and thread-safe function.\n
  * \param phISSTTSRES     [Out] TTS resource handle\n
  * \param szResource      [In] Resource directory, which usually adopts the UNICODE-16 format in the Windows vc environment, and the UNICODE-32 code format in the Linux gcc environment.\n                                      
  * \param iResMode        [In] The way how the resource is loaded: when iResMode = 0, the file handle is opened. \n  
                                                When iResMode = 1, the memory is loaded directly. \n
  * \retval      ISSErrID                 The error code returned by the speech synthesis engine.\n
  * \retval      ISS_SUCCESS              The text to speech resource handle is created successfully;\n
  * \retval      ISS_ERROR_FILE_NOT_FOUND No corresponding resource file is found; \n
  * \retval      ISS_ERROR_INVALID_PARA   Invalid parameter value\n
  * \retval      ISS_ERROR_OUT_OF_MEMORY  The memory is insufficient.\n
*/
ISSErrID ISSAPI ISSTTSInitResW(
    HISSTTSRES* phISSTTSRES,      /* [Out] TTS resource handle */
    const wchar_t* szResource, /* [In] Resource path, usually:
                                          adopting the UNICODE-16 format in the Windows vc environment,
                                          and the UNICODE-32 code format in the Linux gcc environment. */
    int iResMode /* The way how the resource is loaded:
                When iResMode = 0, the file handle is opened.
                When iResMode = 1, the memory is loaded directly. */
    );

/** \brief   Destroy the text to speech resource handle;
  *          When the function is executed, it means the TTS resource handle is destroyed successfully. It is a synchronous, reentrant and thread-safe function.\n
  * \param hISSTTSRES       [In] TTS resource handle\n
  * \retval     ISSErrID               The error code returned by the speech synthesis engine.\n
  * \retval     ISS_SUCCESS            The text to speech resource handle is destroyed successfully. \n
  * \retval     ISS_ERROR_INVALID_PARA Invalid parameter value\n

*/
ISSErrID ISSAPI ISSTTSUnInitRes(HISSTTSRES hISSTTSRES /* [In] TTS resource handle */
                                );

/** \brief This callback message is sent when the text to speech starts and the ::ISSTTSGetAudioData can be called to obtain the audio data. \n
 * \param  pUsrArg    [in] User defined parameter\n
 * \return   None\n
*/
typedef void(ISSCALLBACK* Proc_OnTTSDataReady)(
    void* pUsrArg /* [in] User defined parameter */
    );


/** \brief  Create the text to speech handle;\n
  *         When the function is executed, it means the TTS handle creation is completed. It is It is a synchronous, non-reentrant and non-thread-safe function; \n
  *         Multiple TTS examples can be created using the same TTS resource handle.\n
  * \param  phISSTTS                 [Out] TTS handle\n
  * \param  hISSTTSRES              [In] TTS resource handle \n
  * \param  pcbOnTTSDataReady        [In] The callback function pointer of the user OnTTSDataReady \n
  * \param  pcbOnISSChipCryptError   [In] The callback function pointer of the chip authorization failure: For the encrypted chip version, effective value of the function pointer shall be assigned. Otherwise, enter NULL directly.\n
  * \param  pUsrArg                  [In] The user defined parameter, which will be provided to the user in Callback.\n
  * \retval    ISSErrID                 The error code returned by the text to speech engine\n
  * \retval     ISS_SUCCESS             The text to speech handle is created successfully. \n
  * \retval     ISS_ERROR_INVALID_PARA  Invalid parameter value\n
  * \retval     ISS_ERROR_OUT_OF_MEMORY The memory is insufficient.\n
*/
ISSErrID ISSAPI ISSTTSCreate(
    HISSTTS* phISSTTS,                     /* [Out] TTS handle */
    HISSTTSRES hISSTTSRES,                 /* [In] TTS resource handle */
    Proc_OnTTSDataReady pcbOnTTSDataReady, /* [In]
                                              The callback function pointer of the user OnTTSDataReady */
    Proc_OnISSChipCryptError pcbOnISSChipCryptError, /* [In] The callback function pointer of the chip authorization failure */
    void* pUsrArg                          /* [In] The user defined parameter, which will be provided to the user in Callback. */
    );

/** \brief Destroy the text to speech handle;\n
  *        When the function is executed, the handle is destroyed. It is a synchronous, non-reentrant and non-thread-safe function.\n
  * \param  hISSTTS   [In] TTS handle\n
  * \retval   ISSErrID                The error code returned by the speech synthesis engine.\n
  * \retval  ISS_SUCCESS              The text to speech handle is destroyed successfully with resources being released.\n
  * \retval  ISS_ERROR_INVALID_HANDLE Invalid text to speech handle.\n
 */
ISSErrID ISSAPI ISSTTSDestroy(HISSTTS hISSTTS /* [In] TTS handle */
                              );


/** \brief Configuration parameter; \n
  *        The function is used to set the parameter. When the function is executed, it means the parameter setting is completed. It is a synchronous, non-reentrant and non-thread-safe function; \n
           The value of the iParamID is limited to the following scope:\n
	       ::ISS_TTS_PARAM_SPEAKER \n
	       ::ISS_TTS_PARAM_VOICE_SPEED \n
	       ::ISS_TTS_PARAM_VOICE_PITCH \n
	       ::ISS_TTS_PARAM_VOLUME \n
	       ::ISS_TTS_PARAM_USERMODE \n
  * \param  hISSTTS        [In] TTS handle  \n
  * \param  iParamID       [In] ParamID  \n
  * \param  iParamValue    [In] ParamValue \n
  
  * \retval   ISSErrID                     The error code returned by the speech synthesis engine.\n
  * \retval   ISS_SUCCESS                  The parameter is set successfully.\n
  * \retval   ISS_ERROR_INVALID_HANDLE     Invalid text to speech handle\n
  * \retval   ISS_ERROR_INVALID_PARA       Invalid parameter value\n
  * \retval   ISS_ERROR_INVALID_PARA_VALUE Invalid parameter value\n
*/
ISSErrID ISSAPI ISSTTSSetParam(HISSTTS hISSTTS, /* [In] TTS handle */
                               int iParamID,    /* [In] ParamID */
                               int iParamValue  /* [In] ParamValue */
                               );

/** \brief Configuration parameter; \n
  *        The function is used to set the parameter. When the function is executed, it means the parameter setting is completed. It is a synchronous, non-reentrant and non-thread-safe function; \n
           The value of the iParamID is limited to the following scope:\n
	       ::ISS_TTS_PARAM_SPEAKER \n
	       ::ISS_TTS_PARAM_VOICE_SPEED \n
	       ::ISS_TTS_PARAM_VOICE_PITCH \n
	       ::ISS_TTS_PARAM_VOLUME \n
	       ::ISS_TTS_PARAM_USERMODE \n
  * \param  hISSTTS        [In] TTS handle  \n
  * \param  iParamID       [In] ParamID  \n
  * \param  strParam    [In] ParamValue const char* type\n
  
  * \retval   ISSErrID                     The error code returned by the speech synthesis engine.\n
  * \retval   ISS_SUCCESS                  The parameter is set successfully.\n
  * \retval   ISS_ERROR_INVALID_HANDLE     Invalid text to speech handle\n
  * \retval   ISS_ERROR_INVALID_PARA       Invalid parameter value\n
  * \retval   ISS_ERROR_INVALID_PARA_VALUE Invalid parameter value\n
*/
ISSErrID ISSAPI ISSTTSSetParamEx(HISSTTS hISSTTS, /* [In] TTS handle */
								 int iParamID, /* [In] ParamID */
								 const char* strParam /* [In] ParamValue const char* type */
								 );

/** \brief Speech synthesis of a specified text: if the ISSTTSStart is called to synthesize a new text when the engine is synthesizing a text, the existing synthesis task will be discarded directly. \n
  *        The function is used to synthesize a specified text. When the function is executed, the text to speech is not completed. It is an asynchronous, non-reentrant and non-thread-safe function.\n
           The imported text can use the text annotation method to make the speech more close to user expectations. For more information, see Aisound5 Simple Text Annotation User Manual. Pdf. \n
  * \param  hISSTTS        [In] TTS handle\n
  * \param  pText          [In] TTS text \n
  * \param  iSize          [In] The Buffer size of the TTS text\n
  * \param  iTTSCodePage   [In] The code page of the TTS text\n
  
  * \retval   ISSErrID                 The error code returned by the speech synthesis engine.\n
  * \retval   ISS_SUCCESS              A text to speech is started successfully. \n
  * \retval   ISS_ERROR_INVALID_HANDLE Invalid text to speech handle\n
  * \retval   ISS_ERROR_INVALID_PARA   Invalid parameter value\n
*/
ISSErrID ISSAPI
    ISSTTSStart(HISSTTS hISSTTS,            /* [In] TTS handle */
                const void* pText,          /* [In] The Buffer of the TTS text */
                int iSize,                  /* [In] The Buffer size of the TTS text */
                ISSTTSCodePage iTTSCodePage /* [In] The code page of the TTS text */
                );

/** \brief  The obtained audio data. Support for 16000 sampling rate, S16-LE, single track audio. The size of a sample point is 2Bytes. \n
  *         When the function is executed, it means the TTS audio is obtained. It is a synchronous, non-reentrant and non-thread-safe function. \n
  * \param  hISSTTS            [In] TTS handle
  * \param  pOutBuffer         [In] The Buff address of the audio output\n
  * \param  iBufferFrames      [In] The expected number of the sampling points, not the number of bytes \n
  * \param  piBufferFramesGot  [Out] The actual number of the output sampling points\n
  * \param  piProgBegin        [out] The offset between the current progress and the first address of the imported text (in bytes). \n
  * \param  piProgLen          [out] The sentence length to which the current progress belongs (in bytes) \n
  
  * \retval   ISSErrID                 The error code returned by the speech synthesis engine.\n
  * \retval   ISS_SUCCESS              The text to speech audio is obtained successfully.\n
  * \retval   ISS_ERROR_INVALID_HANDLE Invalid text to speech handle\n
  * \retval   ISS_ERROR_INVALID_PARA   Invalid parameter value\n
  * \retval   ISS_ERROR_TTS_STOPPED    Error calls. The speech synthesis is completed, but the ISSTTSGetAudioData is called to obtain the recording. \n
  * \retval   ISS_ERROR_TTS_COMPLETED  All TTS audios are obtained, and this speech synthesis is completed. \n
*/
ISSErrID ISSAPI ISSTTSGetAudioData(
    HISSTTS hISSTTS,            /* [In] TTS handle */
    void* pOutBuffer,           /* [In] The Buff address of the audio output  */
    unsigned int iBufferFrames, /* [In] The expected number of the sampling points, not the number of bytes */
    unsigned int* piBufferFramesGot, /* [Out] The actual number of the output sampling points */
    unsigned int* piProgBegin, /* [out] The offset between the current progress and the first address of the imported text (in bytes).*/
    unsigned int* piProgLen /* [out] The sentence length to which the current progress belongs (in bytes) */
    );

/** \brief  Stop the text to speech;\n
  *        When the function is executed, it means the text to speech is successfully cancelled. It is a synchronous, reentrant and thread-safe function.\n
  * \param  hISSTTS  [In] TTS handle\n
  
  * \retval    ISSErrID                 The error code returned by the speech synthesis engine.\n
  * \retval    ISS_SUCCESS              This text to speech is successfully cancelled. \n
  * \retval    ISS_ERROR_INVALID_HANDLE Invalid text to speech handle\n
  * \retval    ISS_ERROR_TTS_STOPPED    Error calls. The text to speech is completed. \n
 */
ISSErrID ISSAPI ISSTTSStop(HISSTTS hISSTTS /* [In] TTS handle */
                           );
/** \brief  Obtain the user parameter; \n
  *        When the function is executed, the parameter is obtained. It is a synchronous, non-reentrant and non-thread-safe function..\n
  * \param  hISSTTS  [In] TTS handle\n
  * \param  ppUsrArg [Out] User parameter\n
  
  * \retval    ISSErrID                 The error code returned by the speech synthesis engine.\n
  * \retval    ISS_SUCCESS              The user parameter is obtained successfully.\n
  * \retval    ISS_ERROR_INVALID_HANDLE Invalid text to speech handle\n
 */
ISSErrID ISSAPI ISSTTSGetUsrArg(HISSTTS hISSTTS, void** ppUsrArg);

/** \brief  Set log config parameters \n
  * \param  iParamID     [In] ParamID\n
  * \param  szParamValue [In] ParamValue const char* type\n
  
  * \retval    ISSErrID						The error code returned by the speech synthesis engine.\n
  * \retval    ISS_SUCCESS					The user parameter is obtained successfully.\n
   * \retval   ISS_ERROR_INVALID_PARA       Invalid parameter.\n
 */
ISSErrID ISSAPI ISSTTSSetLogCfgParam(
	const int iParamID, 
	const char* szParamValue
);

#ifdef __cplusplus
}
#endif

#endif
