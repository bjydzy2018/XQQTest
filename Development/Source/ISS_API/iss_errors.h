/*
Copyright:      1999-2013, iFLYTEK Co., Ltd.
File name:      iss_errors.h
Description:    The Error Type Definition of the iFLYTEK Speech Suite
                Brief Description:
                ISS : iFLYTEK Speech Suite;
Author:         iFLYTEK
Version:        1.0
Date:           2013-10-09
History:        Created on Oct. 9, 2013
 */
/** @defgroup group4 The Error Type Definition of the iFLYTEK Speech Suite
 
 * @brief iss_errors.h
 
 */

#ifndef __ISS_ERRORS_H__
#define __ISS_ERRORS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Types of the error codes returned by the iFLYTEK Speech Suite */
typedef int ISSErrID;

enum ISS_EXCEPTION
{
    ISS_SUCCESS                             = 0,        /**< The function is executed successfully. */
	ISS_ERROR_FAIL                          = -1,       /**< Failure */
    ISS_ERROR_EXCEPTION                     = -2,       /**< Abnormal */

    ISS_ERROR_INVALID_CALL                  = 10000,    /**< Illegal calls */
    ISS_ERROR_INVALID_JSON_FMT              = 10001,    /**< Illegal JSON format*/
    ISS_ERROR_INVALID_JSON_INFO             = 10002,    /**< Invalid JSON information */
    ISS_ERROR_TTS_STOPPED                   = 10003,    /**< Used */
    ISS_ERROR_TTS_COMPLETED                 = 10004,    /**< Used */
    ISS_ERROR_CREATE_THREAD_FAIL            = 10005,    /**< Fails to create a thread. */
    ISS_ERROR_NO_SPEECH                     = 10006,    /**< No speech is detected. */
    ISS_ERROR_GET_RESULT_TIMEOUT            = 10007,    /**< Obtaining results timeout */
    ISS_ERROR_BUILDGRM_INIT_ERROR           = 10008,    /**< Building dictionary initialization error */

    ISS_ERROR_INVALID_ACTIVE_KEY            = 10009,    /**< Illegal activation code */
    ISS_ERROR_INVALID_MACHINE_CODE          = 10010,    /**< Illegal machine code */
    ISS_ERROR_ACTIVATE_TO_MANY_TIMES        = 10011,    /**< The times of activation have reached the limit. */
    ISS_ERROR_ACCESS_TO_MANY_TIMES          = 10012,    /**< The visit times have reached the limit. */
    ISS_ERROR_MACHINE_CODE_NOT_SET          = 10013,    /**< The machine code is not set yet.*/
    ISS_ERROR_GET_ACTIVE_KEY_TIMEOUT        = 10014,    /**< Obtaining activation code timeout */
    ISS_ERROR_NO_RESULT                     = 10015,    /**< No recognition result  */
    ISS_ERROR_BUILDING_GRM                  = 10016,    /**< If the configuration file CouldStartLocalRecogniseWhenBuildLocalGrm is set 0, this error occurs when the local grammar is being built after the local recognition is started.*/

    ISS_ERROR_CHIP_LOAD_INFO                = 10017,    /**< Fails to get the authorized chip information. */
    ISS_ERROR_CHIP_CRYPT_FAIL               = 10018,    /**< Fails to decrypt the authorized chip. */
    ISS_ERROR_CHIP_DVC_NOT_AVAILABLE		= 10019,	/**< Fails to open the chip encryption device. */

    ISS_ERROR_MSC_COK_ERROR                 = 10020,    /**< Fails to create the MSC connection */

    ISS_ERROR_DISCARD_OLD_GRM               = 10021,    /**< When the local dictionary is re-uploaded, if there is an old unbuilt dictionary, the old one is abandoned. */

	ISS_ERROR_AIUI_CLIENT_NOT_READY			= 10022,		/**< AIUI client is not ready. */

    /* General errors 10100(0x2774) */
    ISS_ERROR_GENERAL                       = 10100,     /**< Base code  */
    ISS_ERROR_OUT_OF_MEMORY                 = 10101,     /**< Memory overflow*/
    ISS_ERROR_FILE_NOT_FOUND                = 10102,     /**< The file is not found. */
    ISS_ERROR_NOT_SUPPORT                   = 10103,     /**< Not supported  */
    ISS_ERROR_NOT_IMPLEMENT                 = 10104,     /**< Not implemented   */
    ISS_ERROR_ACCESS                        = 10105,     /**< Permission denied */
    ISS_ERROR_INVALID_PARA                  = 10106,     /**< Invalid parameter */
    ISS_ERROR_INVALID_PARA_VALUE            = 10107,     /**< Invalid parameter value */ 
    ISS_ERROR_INVALID_HANDLE                = 10108,     /**< Invalid handle  */
    ISS_ERROR_INVALID_DATA                  = 10109,     /**< Invalid data  */
    ISS_ERROR_NO_LICENSE                    = 10110,     /**< No license  */
    ISS_ERROR_NOT_INIT                      = 10111,     /**< Not initialized */
    ISS_ERROR_NULL_HANDLE                   = 10112,     /**< The value of the handle is null. */
    ISS_ERROR_OVERFLOW                      = 10113,     /**< Overflow  */
    ISS_ERROR_TIME_OUT                      = 10114,     /**< Timeout */
    ISS_ERROR_OPEN_FILE                     = 10115,     /**< An error occurs when the file is opened. */
    ISS_ERROR_NOT_FOUND                     = 10116,     /**< Not found  */
    ISS_ERROR_NO_ENOUGH_BUFFER              = 10117,     /**< No sufficient memory */
    ISS_ERROR_NO_DATA                       = 10118,     /**< No data */
    ISS_ERROR_NO_MORE_DATA                  = 10119,     /**< No more data */
    ISS_ERROR_NO_RESPONSE_DATA              = 10120,     /**< No feedback data */
    ISS_ERROR_ALREADY_EXIST                 = 10121,     /**< Already existed  */
    ISS_ERROR_LOAD_MODULE                   = 10122,     /**< Loading module error */
    ISS_ERROR_BUSY                          = 10123,     /**< Busy  */
    ISS_ERROR_INVALID_CONFIG                = 10124,     /**< Invalid configuration item */
    ISS_ERROR_VERSION_CHECK                 = 10125,     /**< Version error */
    ISS_ERROR_CANCELED                      = 10126,     /**< Cancel */
    ISS_ERROR_INVALID_MEDIA_TYPE            = 10127,     /**< Invalid media type */
    ISS_ERROR_CONFIG_INITIALIZE             = 10128,     /**< The configuration instance initialization error */ 
    ISS_ERROR_CREATE_HANDLE                 = 10129,     /**< Fails to create a handle */
    ISS_ERROR_CODING_LIB_NOT_LOAD           = 10130,     /**< The library is not loaded. */
	ISS_ERROR_INVALID_DIR                   = 10131,     /**< The dir is not valid. */
	ISS_ERROR_PARENT_PATH_NOT_SAME          = 10132,     /**< The parent path is not same. */
	ISS_ERROR_MVW_LANG_CONFLICT             = 10133,     /**< The Language of Mvw has conflicted. */
	ISS_ERROR_CREATE_DIRECTORY				= 10134,     /**< Fail to create a directory. */
	ISS_ERROR_MVW_START_ENGINE				= 10135,     /**< Fail to start ivw engine. */
	ISS_ERROR_MVW_CFG_FILE					= 10136,     /**< Fail to load or parse ivw cfg file and recover to default cfg file. */
	ISS_ERROR_MVW_USER_DEF_RES				= 10137,     /**< Fail to load or parse ivw user defined res and recover to default res. */
	ISS_ERROR_BUILD_REPEAT_KEYWORD			= 10138,     /**< Not supported to build repeated keywords. */
	ISS_ERROR_INVALID_CODING_FORMAT			= 10139,     /**< Wrong coding format. */
	ISS_ERROR_RES_NO_UPDATE					= 10140,     /**< Resource is already up-to-date. */
	ISS_ERROR_RES_CHECK						= 10141,     /**< Fail to check resource. */

    /* Error codes of network 10200(0x27D8)*/
    ISS_ERROR_NET_GENERAL                   = 10200,     /**< Network general error */
    ISS_ERROR_NET_OPENSOCK                  = 10201,     /**< An error occurs when open the socket. */
    ISS_ERROR_NET_CONNECTSOCK               = 10202,     /**< The socket connection error */
    ISS_ERROR_NET_ACCEPTSOCK                = 10203,     /**< An error occurs when receive the socket. */
    ISS_ERROR_NET_SENDSOCK                  = 10204,     /**< Send error */
    ISS_ERROR_NET_RECVSOCK                  = 10205,     /**< Receive error  */
    ISS_ERROR_NET_INVALIDSOCK               = 10206,     /**< Invalid socket */
    ISS_ERROR_NET_BADADDRESS                = 10207,     /**< Invalid address */
    ISS_ERROR_NET_BINDSEQUENCE              = 10208,     /**< The binding sequence error */
    ISS_ERROR_NET_NOTOPENSOCK               = 10209,     /**< Fails to open the socket. */ 
    ISS_ERROR_NET_NOTBIND                   = 10210,     /**< There is no binding */
    ISS_ERROR_NET_NOTLISTEN                 = 10211,     /**< There is no listening  */
    ISS_ERROR_NET_CONNECTCLOSE              = 10212,     /**< The connection is closed. */
    ISS_ERROR_NET_NOTDGRAMSOCK              = 10213,     /**< Not the datagram socket */
    ISS_ERROR_NET_DNS                       = 10214,     /**< DNS resolution error */

    /* Error codes of mssp message 10300(0x283C) */
    ISS_ERROR_MSG_GENERAL                   = 10300,     /**< Message general error */
    ISS_ERROR_MSG_PARSE_ERROR               = 10301,     /**< Parse error  */
    ISS_ERROR_MSG_BUILD_ERROR               = 10302,     /**< Building error  */
    ISS_ERROR_MSG_PARAM_ERROR               = 10303,     /**< Parameter error  */
    ISS_ERROR_MSG_CONTENT_EMPTY             = 10304,     /**< The content is empty. */
    ISS_ERROR_MSG_INVALID_CONTENT_TYPE      = 10305,     /**< Invalid content type */
    ISS_ERROR_MSG_INVALID_CONTENT_LENGTH    = 10306,     /**< The length of the content is invalid. */
    ISS_ERROR_MSG_INVALID_CONTENT_ENCODE    = 10307,     /**< The code of the content is invalid */ 
    ISS_ERROR_MSG_INVALID_KEY               = 10308,     /**< Invalid key */
    ISS_ERROR_MSG_KEY_EMPTY                 = 10309,     /**< The key is empty.*/
    ISS_ERROR_MSG_SESSION_ID_EMPTY          = 10310,     /**< The session ID is empty. */
    ISS_ERROR_MSG_LOGIN_ID_EMPTY            = 10311,     /**< The login ID is empty. */
    ISS_ERROR_MSG_SYNC_ID_EMPTY             = 10312,     /**< The sync ID is empty. */
    ISS_ERROR_MSG_APP_ID_EMPTY              = 10313,     /**< The application ID is empty. */
    ISS_ERROR_MSG_EXTERN_ID_EMPTY           = 10314,     /**< The extend ID is empty. */
    ISS_ERROR_MSG_INVALID_CMD               = 10315,     /**< Invalid command */
    ISS_ERROR_MSG_INVALID_SUBJECT           = 10316,     /**< Invalid subject  */
    ISS_ERROR_MSG_INVALID_VERSION           = 10317,     /**< Invalid version  */
    ISS_ERROR_MSG_NO_CMD                    = 10318,     /**< There is no command. */
    ISS_ERROR_MSG_NO_SUBJECT                = 10319,     /**< There is no subject.  */
    ISS_ERROR_MSG_NO_VERSION                = 10320,     /**< There is no version number. */
    ISS_ERROR_MSG_MSSP_EMPTY                = 10321,     /**< The message is empty. */
    ISS_ERROR_MSG_NEW_RESPONSE              = 10322,     /**< Fails to create a response message. */
    ISS_ERROR_MSG_NEW_CONTENT               = 10323,     /**< Fails to create a new content. */
    ISS_ERROR_MSG_INVALID_SESSION_ID        = 10324,     /**< Invalid session ID. */

    /* Error codes of DataBase 10400(0x28A0)*/
    ISS_ERROR_DB_GENERAL                    = 10400,     /**< General database error */
    ISS_ERROR_DB_EXCEPTION                  = 10401,     /**< Abnormal */
    ISS_ERROR_DB_NO_RESULT                  = 10402,     /**< No result */
    ISS_ERROR_DB_INVALID_USER               = 10403,     /**< Invalid user */
    ISS_ERROR_DB_INVALID_PWD                = 10404,     /**< Invalid password  */
    ISS_ERROR_DB_CONNECT                    = 10405,     /**< The connection error.  */
    ISS_ERROR_DB_INVALID_SQL                = 10406,     /**< Invalid SQL  */
    ISS_ERROR_DB_INVALID_APPID              = 10407,     /**< Invalid application ID */

    /* Error codes of Resource 10500(0x2904)*/
    ISS_ERROR_RES_GENERAL                   = 10500,     /**< General resource error */
    ISS_ERROR_RES_LOAD                      = 10501,     /**< Not loaded */
    ISS_ERROR_RES_FREE                      = 10502,     /**< Idle */
    ISS_ERROR_RES_MISSING                   = 10503,     /**< Missing  */
    ISS_ERROR_RES_INVALID_NAME              = 10504,     /**< Invalid name */
    ISS_ERROR_RES_INVALID_ID                = 10505,     /**< Invalid ID  */
    ISS_ERROR_RES_INVALID_IMG               = 10506,     /**< Invalid image  */
    ISS_ERROR_RES_WRITE                     = 10507,     /**< Writing operation error */
    ISS_ERROR_RES_LEAK                      = 10508,     /**<Leakage  */
    ISS_ERROR_RES_HEAD                      = 10509,     /**< Resource head error */
    ISS_ERROR_RES_DATA                      = 10510,     /**< An error occurs to the data. */
    ISS_ERROR_RES_SKIP                      = 10511,     /**< Skip */
    ISS_ERROR_RES_MISMATCH                  = 10512,     /**< Resource mismatch. */

    /* Error codes of TTS 10600(0x2968)*/
    ISS_ERROR_TTS_GENERAL                   = 10600,     /**< General text to speech error */
    ISS_ERROR_TTS_TEXTEND                   = 10601,     /**< End of the text  */
    ISS_ERROR_TTS_TEXT_EMPTY                = 10602,     /**< The text is empty. */
	ISS_ERROR_TTS_RESOURCE                  = 10603,     /**< Resource is error. */
	ISS_ERROR_TTS_RESOURCE_COUNT            = 10604,     /**< Resource count is error */
	ISS_ERROR_TTS_COMMON_RESOURCE_MISSING   = 10605,     /**< Common resource is missing */

    /* Error codes of Recognizer 10700(0x29CC) */
    ISS_ERROR_REC_GENERAL                   = 10700,     /**< General error */
    ISS_ERROR_REC_INACTIVE                  = 10701,     /**< Under the inactive state */
    ISS_ERROR_REC_GRAMMAR_ERROR             = 10702,     /**< Grammar error */
    ISS_ERROR_REC_NO_ACTIVE_GRAMMARS        = 10703,     /**< No active grammar */
    ISS_ERROR_REC_DUPLICATE_GRAMMAR         = 10704,     /**< Repeated grammar */
    ISS_ERROR_REC_INVALID_MEDIA_TYPE        = 10705,     /**< Invalid media type */
    ISS_ERROR_REC_INVALID_LANGUAGE          = 10706,     /**< Invalid language */
    ISS_ERROR_REC_URI_NOT_FOUND             = 10707,     /**< No corresponding URI */
    ISS_ERROR_REC_URI_TIMEOUT               = 10708,     /**< Time is out when get the URI content. */
    ISS_ERROR_REC_URI_FETCH_ERROR           = 10709,     /**< An error occurred when get the URI content. */

    /* Error codes of Speech Detector 10800(0x2A30) */
    ISS_ERROR_EP_GENERAL                    = 10800,     /**< (EP) Common error */
    ISS_ERROR_EP_NO_SESSION_NAME            = 10801,     /**< (EP) No link name*/
    ISS_ERROR_EP_INACTIVE                   = 10802,     /**< (EP) Inactive */
    ISS_ERROR_EP_INITIALIZED                = 10803,     /**< (EP) Initialization error */ 

    /* Error codes of TUV */  
    ISS_ERROR_TUV_GENERAL                   = 10900,     /**< General error*/
    ISS_ERROR_TUV_GETHIDPARAM               = 10901,     /**< Get Busin Param huanid */
    ISS_ERROR_TUV_TOKEN                     = 10902,     /**< Get Token  */
    ISS_ERROR_TUV_CFGFILE                   = 10903,     /**< Open cfg file  */
    ISS_ERROR_TUV_RECV_CONTENT              = 10904,     /**< Received content is error  */
    ISS_ERROR_TUV_VERFAIL                   = 10905,     /**< Verify failure */

    /* Error codes of IMTV */
    ISS_ERROR_LOGIN_SUCCESS                 = 11000,     /**< Log in successfully. */
    ISS_ERROR_LOGIN_NO_LICENSE              = 11001,     /**< No license */
    ISS_ERROR_LOGIN_SESSIONID_INVALID       = 11002,     /**< Invalid SessionID */
    ISS_ERROR_LOGIN_SESSIONID_ERROR         = 11003,     /**< Error SessionID */
    ISS_ERROR_LOGIN_UNLOGIN                 = 11004,     /**< Not logged in */
    ISS_ERROR_LOGIN_INVALID_USER            = 11005,     /**< Invalid user */
    ISS_ERROR_LOGIN_INVALID_PWD             = 11006,     /**< Invalid password */
    ISS_ERROR_LOGIN_SYSTEM_ERROR            = 11099,     /**< System error */

    /* Error codes of HCR */
    ISS_ERROR_HCR_GENERAL                   = 11100,     /**< Unknown exception or error is found (abandoned but temporarily reserved) */
    ISS_ERROR_HCR_RESOURCE_NOT_EXIST        = 11101,     /**< The resource does not exist. (abandoned but temporarily reserved) */
    ISS_ERROR_HCR_CREATE                    = 11102,     /**< Fails to create the handwriting engine. (abandoned but temporarily reserved)*/
    ISS_ERROR_HCR_DESTROY                   = 11103,     /**< Fails to destroy the handwriting engine. (abandoned but temporarily reserved) */
    ISS_ERROR_HCR_START                     = 11104,     /**< Fails to start the handwriting engine. (abandoned but temporarily reserved) */
    ISS_ERROR_HCR_APPEND_STROKES            = 11105,     /**< Fails to get the result. (abandoned but temporarily reserved)*/
    ISS_ERROR_HCR_GET_RESULT                = 11106,     /**< The ice timeout exception may occur when the client executes the following request operations: start, setid, appendData, and getresult. (abandoned but temporarily reserved)*/
    ISS_ERROR_HCR_SET_PREDICT_DATA          = 11107,     /**< Setting failure (abandoned but temporarily reserved) */
    ISS_ERROR_HCR_GET_PREDICT_RESULT        = 11108,     /**< Obtaining error (abandoned but temporarily reserved) */


    /* Error codes of http 12000(0x2EE0) */
    ISS_ERROR_HTTP_BASE                     = 12000,    /**< HTTP Error base code */

    /*Error codes of ISV */
    ISS_ERROR_ISV_NO_USER                   = 13000,    /**< User not exist */

    /* Error codes of Lua scripts */
    ISS_ERROR_LUA_BASE                      = 14000,    /**< Error code reserved */
    ISS_ERROR_LUA_YIELD                     = 14001,    /**< Error code reserved */
    ISS_ERROR_LUA_ERRRUN                    = 14002,    /**< Error code reserved */
    ISS_ERROR_LUA_ERRSYNTAX                 = 14003,    /**< Error code reserved */
    ISS_ERROR_LUA_ERRMEM                    = 14004,    /**< Error code reserved */
    ISS_ERROR_LUA_ERRERR                    = 14005,    /**< Error code reserved*/

	/* Error codes for Cata */
	ISS_ERROR_CATA_CREATE_INST_FAIL         = 15001,   /**<Create Cata Inst fail*/
	ISS_ERROR_CATA_IDX_LOAD_NO_DATA         = 15002,   /**<No data when importing index*/
	ISS_ERROR_CATA_IDX_LOAD_WRONG_DATA_SIZE = 15003,   /**<Error in data size when importing index*/
	ISS_ERROR_CATA_IDX_ERR_CRC_CODE         = 15004,   /**<Check code error, generally indicates that the file is corrupt */
	ISS_ERROR_CATA_IDX_REPEAT_SAME_FIELD    = 15005,   /**<Data contains the same domain name information */
	ISS_ERROR_CATA_IDX_FILE_EMPTY	        = 15006,   /**<Index file is empty */
	ISS_ERROR_CATA_IDX_ILLEGAL_RAMDIR	    = 15007,   /**<Illegal memory index */
	ISS_ERROR_CATA_IDX_NO_RAMDIR	        = 15008,   /**<No memory index*/
	ISS_ERROR_CATA_IDX_LACK_RAWDATA         = 15009,   /**<No source data input */
	ISS_ERROR_CATA_IDX_MUCH_FIELD	        = 15010,   /**<Domain number exceeded the limit */
	ISS_ERROR_CATA_IDX_ERR_INST_SET_FULL    = 15011,   /**<Instance set is full */
	ISS_ERROR_CATA_IDX_ERR_INVALID_HANDLE   = 15012,   /**<Invalid instance handle */
	ISS_ERROR_CATA_IDX_CLUCENE_ERROR	    = 15013,   /**<CLucene internal error */
	ISS_ERROR_CATA_IDX_WRONG_TYPE	        = 15014,   /**<Index resource type error */
	ISS_ERROR_CATA_IDX_DATA_EXCEED_LENGH    = 15015,   /**<Resource string over length limit */
	ISS_ERROR_CATA_IDX_FIELD_EMPTY          = 15016,   /**<Domain name is empty */
	ISS_ERROR_CATA_IDX_MUCH_ELEMENT         = 15017,   /**<Extended data too many units */
	ISS_ERROR_CATA_IDX_DATA_EMPTY	        = 15018,   /**<Data is empty */
	ISS_ERROR_CATA_IDX_WRONG_PARAM          = 15019,   /**<Parameter error */
	ISS_ERROR_CATA_IDX_FIELD_EXCEED_LENGH	= 15020,   /**<Domain length beyond limit */
	ISS_ERROR_CATA_IDX_FIELD_DATA_EMPTY     = 15021,   /**<Fields and values are empty */
	ISS_ERROR_CATA_IDX_IDX_EXIST            = 15022,   /**<Index already exists */
	ISS_ERROR_CATA_IDX_ALREADY_INIT         = 15023,   /**<Already initialized */
	ISS_ERROR_CATA_IDX_NOT_INIT             = 15024,   /**<Uninitialized */
	ISS_ERROR_CATA_SRH_INST_SET_FULL        = 15025,   /**<Srh inst set is full */
    ISS_ERROR_CATA_SRH_INST_CREATE_FAIL     = 15026,   /**<Srh inst create fail */
	ISS_ERROR_CATA_SRH_INST_INVALID         = 15027,   /**<Srh inst is invalid */
	ISS_ERROR_CATA_SRH_STACK_FULL           = 15028,   /**<Stack full */
	ISS_ERROR_CATA_SRH_PARA_VALID           = 15029,   /**<Para is invalid*/
	ISS_ERROR_CATA_SRH_SAME_PARAM           = 15030,   /**<Para is same*/
	ISS_ERROR_CATA_SRH_ALIAS_RES_VALID      = 15031,
	ISS_ERROR_CATA_SRH_PYFZY_RES_VALID      = 15032,
	ISS_ERROR_CATA_SRH_NO_INDEX             = 15033,
	ISS_ERROR_CATA_SRH_INDEX_VALID          = 15034,
	ISS_ERROR_CATA_SRH_QUERYEXTEND_FAILD    = 15035,
	ISS_ERROR_CATA_SRH_FUZZYEXTEND_FAILD    = 15036,
	ISS_ERROR_CATA_SRH_RES_EXIST            = 15037,
	ISS_ERROR_CATA_SRH_WAR_STACK_EMPTY      = 15038,
	ISS_ERROR_CATA_SRH_WAR_NO_RESULT        = 15039,
	ISS_ERROR_CATA_SRH_WAR_NO_ALIAS         = 15040,
	ISS_ERROR_CATA_SRH_WAR_NO_FUZZY_RES     = 15041,
	ISS_ERROR_CATA_SRH_ALREADY_INIT         = 15042,
	ISS_ERROR_CATA_SRH_NOT_INIT             = 15043,
    ISS_ERROR_CATA_IDX_JSON_PARAM_WRONG     = 15060

};

#ifdef __cplusplus
}
#endif

#endif /* __ISS_ERRORS_H__ */
