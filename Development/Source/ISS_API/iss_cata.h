/******************************************************************************
Copyright:      1999-2015, iFLYTEK Co., Ltd.
File name:      iss_cata.h
Description:    iFLYTEK Speech Suite – Catalogue
                Brief Description:
                ISS : iFLYTEK Speech Suite;
				cata : Catalogue;
Author:         iFLYTEK
Version:        1.0
Date:           2015-05-22
History:        Created on May 22, 2015
*******************************************************************************/
/** @defgroup group3 iFLYTEK Speech Suite – General Search Module
 
 * @brief iss_cata.h
 
 */
#ifndef _ISS_CATA_H_
#define _ISS_CATA_H_

#include "iss_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* General search, the callback message */
enum tagISS_CATA_MSG{
/** \brief  When the search interface is asynchronous, there are search results.
  * \param wParam:      ISS_SUCCESS: The search is successful.
  *                     ISS_ERROR_FAIL: The search fails.
  * \param lParam:      const char*Type, search result 
*/
    ISS_CATA_MSG_OnSearchResult    =   30000,
	ISS_CATA_MSG_LoadResourceFail = 30001,
};
/* Language type */
typedef enum tagISS_CATA_LANG_TYPE{
	ISS_CATA_LANG_MANDARIN = 1,
	ISS_CATA_LANG_ENGLISH = 2,
	ISS_CATA_LANG_CANTONESE = 3,
	ISS_CATA_LANG_BUTT
}ISS_CATA_LANG_TYPE;
/* Param type */
typedef enum tagISS_CATA_PARAM_TYPE{
	ISS_CATA_NEED_PY_CONTAIN=1, /*表示SR模式下是否支持拼音包含匹配，设为1表示支持，默认不支持；*/
	ISS_CATA_OUTPUT_LIMIT=2   /*表示SR模式下搜索json结果的输出得分门限，如果某个匹配结果的得分低于这个门限，就不输出,取值范围0-10*/
}ISS_CATA_PARAM_TYPE;

// Log level.According to the requirement to combine mask.
#define ISS_CATA_PARAM_LOG_LEVEL				(0X00000506)
#define ISS_CATA_VOLUME_LOG_LEVEL_ALL			("-1")	// all info
#define ISS_CATA_VOLUME_LOG_LEVEL_NONE			("0")	// none
#define ISS_CATA_VOLUME_LOG_LEVEL_CRIT			("1")	// critical info
#define ISS_CATA_VOLUME_LOG_LEVEL_ERROR			("2")	// error info
#define ISS_CATA_VOLUME_LOG_LEVEL_WARNING		("4")	// warnint info

// Log output.According to the requirement to combine mask.
#define ISS_CATA_PARAM_LOG_OUTPUT				(0X00000507)
#define ISS_CATA_VOLUME_LOG_OUTPUT_NONE			("0")	// none
#define ISS_CATA_VOLUME_LOG_OUTPUT_FILE			("1")	// file
#define ISS_CATA_VOLUME_LOG_OUTPUT_CONSOLE		("2")	// console（except for android）
#define ISS_CATA_VOLUME_LOG_OUTPUT_DEBUGGER		("4")	// debugger
#define ISS_CATA_VOLUME_LOG_OUTPUT_MSGBOX		("8")	// message box

// Log FileName
#define ISS_CATA_PARAM_LOG_FILE_NAME			(0X00000508)

/** \brief   Message callback
  * \param  pUsrArg    [In] User defined parameter\n
  * \param  uMsg       [In] Message identifier \n
  * \param  pUsrArg    [In] Message additional information\n
  * \param  pUsrArg    [In] Message additional information \n
 * \return   None\n
*/
typedef void (ISSCALLBACK *Proc_OnCataMsgProc)(
	const void*         pUsrArg,      /* [In] User defined parameter */
	unsigned int        uMsg,         /* [In] Message identifier */
	unsigned int        wParam,       /* [In] Message additional information */
	const void*         lParam        /* [In] Message additional information */
);

typedef void* HISSCATAINDEX;      // General search, index building handle  
typedef void* HISSCATASEARCH;     // General search, search handle

/** \brief  Create an index building handle;\n
            It is a synchronous, non-reentrant and non-thread-safe function. \n
  * \param  phISSCATAINDEXObj    [Out] The index building handle\n
  * \param  szResourceDir        [In] Resource directory, usually: adopts the GBK code format in the Windows Chinese environment, and the UTF-8 code format in the Linux environment. \n
  * \param  szIndexResName       [In] Index resource name: The index resource name shall be specified during the searching progress and is also used as the file name when the index resource is saved as a file. \n
  * \param  bDropOldResFlag      [In] Whether the previous index resources are abandoned or not: when the value is not 0, the previous index resources will be abandoned, which equals to having called the ::ISSCataIndexDropRes interface. \n
									  When the value is 0, the previous built indexes will be loaded. And if the resource validation fails, the ::ISS_ERROR_RES_DATA will be returned. Then a new handle needs to be created and the index record needs to be added again. \n
  * \param  pfnOnCataMsgProc     [In] Message callback \n
  * \param  pUsrArg              [In] User defined parameter \n
  * \retval     ISSErrID               The error code returned by the general search \n
  * \retval     ISS_SUCCESS            The index building handle is created successfully. \n
  * \retval     ISS_ERROR_RES_DATA     The resource file validation or load failure \n
  * \retval     ISS_ERROR_INVALID_PARA Invalid parameter value\n
*/
ISSErrID ISSAPI ISSCataIndexCreate(
    HISSCATAINDEX*      phISSCATAINDEXObj,  /* [Out] Index building handle */
    const char*         szResourceDir,      /* [In] Resource directory, usually: 
                                                adopts the GBK code format in the Windows Chinese environment,
                                                and the UTF-8 code format in the Linux environment */
    const char*         szIndexResName,     /* [In] Index resource name:
											   The index resource name shall be specified during the searching progress 
											   and is also used as the file name when the index source is saved as 
											   a file. */
    int                 bDropOldResFlag,    /* [In] Whether the previous index resources are abandoned or not:
											   when the value is not 0, the previous index resources will be abandoned,
											   which equals to having called the ::ISSCataIndexDropRes interface;
											   When the value is 0, the previous built indexes will be loaded. 
											   and if the resource validation fails, the ISS_ERROR_RES_DATA will be returned.
											   Then a new handle needs to be created and the index record needs to be added again. */
    Proc_OnCataMsgProc  pfnOnCataMsgProc,   /* [In] Message callback */
    void*               pUsrArg             /* [In] User defined parameter */
);
/** \brief  Create an index building handle;\n
            It is a synchronous, non-reentrant and non-thread-safe function. \n
  * \param  phISSCATAINDEXObj    [Out] The index building handle\n
  * \param  szResourceDir        [In] Resource directory, usually: adopts the GBK code format in the Windows Chinese environment, and the UTF-8 code format in the Linux environment. \n
  * \param  szIndexResName       [In] Index resource name: The index resource name shall be specified during the searching progress and is also used as the file name when the index resource is saved as a file. \n
  * \param  langType,            [In] The language type of Index resource
  * \param  bDropOldResFlag      [In] Whether the previous index resources are abandoned or not: when the value is not 0, the previous index resources will be abandoned, which equals to having called the ::ISSCataIndexDropRes interface. \n
									  When the value is 0, the previous built indexes will be loaded. And if the resource validation fails, the ::ISS_ERROR_RES_DATA will be returned. Then a new handle needs to be created and the index record needs to be added again. \n
  * \param  pfnOnCataMsgProc     [In] Message callback \n
  * \param  pUsrArg              [In] User defined parameter \n
  * \retval     ISSErrID               The error code returned by the general search \n
  * \retval     ISS_SUCCESS            The index building handle is created successfully. \n
  * \retval     ISS_ERROR_RES_DATA     The resource file validation or load failure \n
  * \retval     ISS_ERROR_INVALID_PARA Invalid parameter value\n
*/
ISSErrID ISSAPI ISSCataIndexCreateEx(
    HISSCATAINDEX*      phISSCATAINDEXObj,  /* [Out] Index building handle */
    const char*         szResourceDir,      /* [In] Resource directory, usually: 
                                                adopts the GBK code format in the Windows Chinese environment,
                                                and the UTF-8 code format in the Linux environment */
    const char*         szIndexResName,     /* [In] Index resource name:
											   The index resource name shall be specified during the searching progress 
											   and is also used as the file name when the index source is saved as 
											   a file. */
    int                 langType,           /* [In] The language type of Index resource.*/
    int                 bDropOldResFlag,    /* [In] Whether the previous index resources are abandoned or not:
											   when the value is not 0, the previous index resources will be abandoned,
											   which equals to having called the ::ISSCataIndexDropRes interface;
											   When the value is 0, the previous built indexes will be loaded. 
											   and if the resource validation fails, the ISS_ERROR_RES_DATA will be returned.
											   Then a new handle needs to be created and the index record needs to be added again. */
    Proc_OnCataMsgProc  pfnOnCataMsgProc,   /* [In] Message callback */
    void*               pUsrArg             /* [In] User defined parameter */
);
/** \brief  Destroy the index building handle; \n
            It is a synchronous, non-reentrant and non-thread-safe function.\n
  * \param  hISSCATAINDEXObj    [In] Index building handle \n
  * \retval     ISSErrID                 The error code returned by the general search \n
  * \retval     ISS_SUCCESS              The index building handle is destroyed successfully with resources being released. \n
  * \retval     ISS_ERROR_INVALID_HANDLE Invalid handle \n
*/
ISSErrID ISSAPI ISSCataIndexDestroy(
    HISSCATAINDEX    hISSCATAINDEXObj    /* [In] Index building handle */
);

/** \brief  The index resources are abandoned to create a new index; \n
  *         It is a synchronous, non-reentrant and non-thread-safe function.\n
  * \param  hISSCATAINDEXObj    [In] Index building handle \n
  * \retval     ISSErrID               The error code returned by the general search \n
  * \retval     ISS_SUCCESS            The index resources are destroyed successfully. \n
  * \retval     ISS_ERROR_INVALID_PARA Invalid parameter value \n
*/
ISSErrID ISSAPI ISSCataIndexDropRes(
    HISSCATAINDEX    hISSCATAINDEXObj    /* [In] Index building handle */
);

/** \brief  Insert the index record; \n
            It is a synchronous, non-reentrant and non-thread-safe function. \n
  * \param  hISSCATAINDEXObj    [In] Index building handle \n
  * \param  pData               [In] A piece of index data \n
  * \retval     ISSErrID               The error code returned by the general search \n
  * \retval     ISS_SUCCESS            An index record is inserted successfully. \n
  * \retval     ISS_ERROR_INVALID_PARA Invalid parameter value \n
*/
ISSErrID ISSAPI ISSCataIndexAddIdxEntity(
    HISSCATAINDEX                 hISSCATAINDEXObj,    /* [In] Index building handle */
    const char*    pData                /* [In] A piece of index data */
);

/** \brief  Delete an index record; \n
            It is a synchronous, non-reentrant and non-thread-safe function. \n
  * \param  hISSCATAINDEXObj     [In] Index building handle \n
  * \param  pData                [In] A piece of index data \n
  * \retval     ISSErrID               The error code returned by the general search \n
  * \retval     ISS_SUCCESS            The index record is deleted successfully. \n
  * \retval     ISS_ERROR_INVALID_PARA Invalid parameter value \n
*/
ISSErrID ISSAPI ISSCataIndexDelIdxEntity(
    HISSCATAINDEX                 hISSCATAINDEXObj,    /* [In] Index building handle */
    const char*    pData                /* [In] A piece of index data */
);

/** \brief  Build an index and write it to the file synchronously. In addition, update the index being used in the search; \n
  *         It is a synchronous, non-reentrant and non-thread-safe function. \n
  * \param  hISSCATAINDEXObj    [In] Index building handle \n
  * \retval     ISSErrID               The error code returned by the general search \n
  * \retval     ISS_SUCCESS            The index is built successfully. \n
  * \retval     ISS_ERROR_INVALID_PARA Invalid parameter value \n
*/
ISSErrID ISSAPI ISSCataIndexEndIdxEntity(
	HISSCATAINDEX  hISSCATAINDEXObj    /* [In] Index building handle */
);


/** \brief  Create a search handle;\n
            It is a synchronous, non-reentrant and non-thread-safe function. \n
  *         szIndexResNames: Index resource name. It is a list separated by commas. The suite resource directory contains the following reserved resource names: 
  *         1. The alias is another name of the resource name.
  *         2. The pinyin_fuzzy is the fuzzy pinyin resource.
  *         3. The yellow_page is the nationwide Yellow Pages index resources. The index domain used for search is: name,code. The name is the yellow page name.   
  *         The code is the yellow page number.
  *         For example: 
  *         1. The "alias,yellow_page,phone_contact" means that the alias fuzzy matching and key error correction are allowed, 
  *         and apart from loading the customized phone_contact index resources, the yellow_page is added to participate in the search.
  *         2. The "radio_info" means that only the customized radio_info index resource is loaded. A customized index resource can be built by calling the index interfaces. 
  * \param    phISSCATASEARCHObj   [Out] Search handle \n
  * \param    szResourceDir        [In] Resource directory, usually: adopts the GBK code format in the Windows Chinese environment, and the UTF-8 code format in the Linux environment.\n
  * \param    szIndexResNames      [In] Index resource name \n
  * \param    pfnOnCataMsgProc     [In] Message callback \n
  * \param    pUsrArg              [In] User defined parameter \n
  * \retval     ISSErrID                 The error code returned by the general search \n
  * \retval     ISS_SUCCESS              The search handle is created successfully. \n
  * \retval     ISS_ERROR_FILE_NOT_FOUND No corresponding resource file is found. \n
  * \retval     ISS_ERROR_INVALID_PARA   Invalid parameter value \n

*/
ISSErrID ISSAPI ISSCataSearchCreate(
    HISSCATASEARCH*     phISSCATASEARCHObj,     /* [Out] Search handle */
    const char*         szResourceDir,          /* [In] Resource directory, usually:
                                                   adopts the GBK code format in the Windows Chinese environment,
                                                   and the UTF-8 code format in the Linux environment. */
    const char*         szIndexResNames,        /* [In] Index resource name */
    Proc_OnCataMsgProc  pfnOnCataMsgProc,       /* [In] Message callback */
    void*               pUsrArg                 /* [In] User defined parameter */
);
/** \brief  Create a search handle;\n
            It is a synchronous, non-reentrant and non-thread-safe function. \n
  *         szIndexResNames: Index resource name. It is a list separated by commas. The suite resource directory contains the following reserved resource names: 
  *         1. The alias is another name of the resource name.
  *         2. The pinyin_fuzzy is the fuzzy pinyin resource.
  *         3. The yellow_page is the nationwide Yellow Pages index resources. The index domain used for search is: name,code. The name is the yellow page name.   
  *         The code is the yellow page number.
  *         For example: 
  *         1. The "alias,yellow_page,phone_contact" means that the alias fuzzy matching and key error correction are allowed, 
  *         and apart from loading the customized phone_contact index resources, the yellow_page is added to participate in the search.
  *         2. The "radio_info" means that only the customized radio_info index resource is loaded. A customized index resource can be built by calling the index interfaces. 
  * \param    phISSCATASEARCHObj   [Out] Search handle \n
  * \param    szResourceDir        [In] Resource directory, usually: adopts the GBK code format in the Windows Chinese environment, and the UTF-8 code format in the Linux environment.\n
  * \param    szIndexResNames      [In] Index resource name \n
  * \param    pfnOnCataMsgProc     [In] Message callback \n
  * \param    nLangType            [In] Language type \n
  * \param    pUsrArg              [In] User defined parameter \n
  * \retval     ISSErrID                 The error code returned by the general search \n
  * \retval     ISS_SUCCESS              The search handle is created successfully. \n
  * \retval     ISS_ERROR_FILE_NOT_FOUND No corresponding resource file is found. \n
  * \retval     ISS_ERROR_INVALID_PARA   Invalid parameter value \n

*/
ISSErrID ISSAPI ISSCataSearchCreateEx(
    HISSCATASEARCH*     phISSCATASEARCHObj,     /* [Out] Search handle */
    const char*         szResourceDir,          /* [In] Resource directory, usually:
                                                   adopts the GBK code format in the Windows Chinese environment,
                                                   and the UTF-8 code format in the Linux environment. */
    const char*         szIndexResNames,        /* [In] Index resource name */
    Proc_OnCataMsgProc  pfnOnCataMsgProc,       /* [In] Message callback */
	int                 nLangType,              /* [In] Language type*/
    void*               pUsrArg                 /* [In] User defined parameter */
);
/** \brief    Destroy the search handle
              It is a synchronous, non-reentrant and non-thread-safe function. \n
  * \param    hISSCATASEARCHObj     [In] Search handle\n
  * \retval     ISSErrID                 The error code returned by the general search \n
  * \retval     ISS_SUCCESS              The search handle is destroyed successfully with resources being released. \n
  * \retval     ISS_ERROR_INVALID_HANDLE Invalid handle \n
*/
ISSErrID ISSAPI ISSCataSearchDestroy(
    HISSCATASEARCH    hISSCATASEARCHObj    /* [In] Search handle */
);

/** \brief   Synchronous search interface
  *          It is a synchronous, non-reentrant and non-thread-safe function. \n
  *          szQuery example:\n
  *          field=xxx,query=xxx,fuzzy=0,searchType=T9\n
  *          or field=xxx,query=xxx,fuzzy=0,searchType=KEYBOARD\n
  *	         field denotes the domain name, query denotes the query string, fuzzy denotes whether fuzzy search is needed, \n
  *        	 and searchType denotes the search type, whose scope is {T9,KEYBOARD,SR}. The search type is all capitalized. \n
  * \param    hISSCATASEARCHObj     [In] Search handle \n
  * \param    szQuery               [In] Search parameter\n
  * \param    pErr                  [Out] Error code\n
  * \retval     ISSErrID                 The error code returned by the general search engine \n
  * \retval     ISS_SUCCESS              Successful \n
  * \retval     ISS_ERROR_INVALID_HANDLE Invalid handle \n

*/
ISSAPI_PORT const char* ISSCALL ISSCataSearchSync(
	HISSCATASEARCH    hISSCATASEARCHObj,    /* [In]  Search handle */
	const char*       szQuery,              /* [In] Search parameter */
	ISSErrID*         pErr                  /* [Out] Error code   */
);

/** \brief   Asynchronous search interface; \n
  *          It is an asynchronous, non-reentrant and non-thread-safe function.\n
  *          szQuery example:\n
  *          field=xxx,query=xxx,fuzzy=0,searchType=T9\n
  *          or field=xxx,query=xxx,fuzzy=0,searchType=KEYBOARD\n
  *	         field denotes the domain name, query denotes the query string, fuzzy denotes whether fuzzy search is needed, n
  *        	 and searchType denotes the search type, whose scope is {T9,KEYBOARD,SR}. The search type is all capitalized. \n
  * \param    hISSCATASEARCHObj     [In] Search handle \n
  * \param    szQuery               [In] Search parameter\n
  * \retval     ISSErrID                 The error code returned by the general search engine \n
  * \retval     ISS_SUCCESS              Successful\n
  * \retval     ISS_ERROR_INVALID_HANDLE Invalid handle\n

*/
ISSErrID ISSAPI ISSCataSearchAsync(
	HISSCATASEARCH    hISSCATASEARCHObj,    /* [In] Search handle */
	const char*       szQuery               /* [In] Search parameter */
);
ISSErrID ISSAPI ISSCataSetParam(
	HISSCATASEARCH    hISSCATASEARCHObj,    /* [In] Search handle */
	int pmType, 
	int val
);

/** \brief  Set log config parameters \n
  * \param  iParamID     [In] ParamID\n
  * \param  szParamValue [In] ParamValue const char* type\n
  
  * \retval    ISSErrID						The error code returned by the speech synthesis engine.\n
  * \retval    ISS_SUCCESS					The user parameter is obtained successfully.\n
   * \retval   ISS_ERROR_INVALID_PARA       Invalid parameter.\n
 */
ISSErrID ISSAPI ISSCataSetLogCfgParam(
	const int iParamID, 
	const char* szParamValue
);

#ifdef __cplusplus
}
#endif

#endif /* _ISS_POISEARCH_H_ */
