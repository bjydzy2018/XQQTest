/**
Copyright:      1999-2014, iFLYTEK Co., Ltd.
File name:      iss_poisearch.h
Description:    iFLYTEK Speech Suite - POI Search Module
                Brief Description:
                ISS: iFLYTEK Speech Suite;
				PS: POI Search;
Author:         iFLYTEK
Version:        1.0
Date:           2015-01-13
History:        Created on Jan. 13, 2015
*/
/** @defgroup group8 iFLYTEK Speech Suite - POI Search
 
 * @brief iss_poisearch.h
 
 */


#ifndef _ISS_POISEARCH_H_
#define _ISS_POISEARCH_H_

#include "iss_types.h"

#ifdef ISS_DLL_EXPORTS
#define ISSPSAPI  ISSAPI
#else
#define ISSPSAPI
#endif


#ifdef __cplusplus
extern "C" {
#endif


/** \brief  POI search initialization;\n
            It is a synchronous, non-reentrant and non-thread-safe function.\n
 * \param	szCfgPath     [In] The ipoisearch.cfg configuration file path: the ipoisearch.cfg is under the resource directory. \n
 * \retval  ISSErrID               Whether the POI search initialization is successful or not. \n
 * \retval  ISS_SUCCESS            The POI search initialization is successful. \n
 * \retval  ISS_ERROR_FAIL         The POI search initialization fails. \n
 * \retval  ISS_ERROR_INVALID_PARA Invalid parameter, for example, the cfg file path does not exist. \n

 */
ISSErrID ISSPSAPI ISSPSCreate(
    const char* szCfgPath			/* [in] The ipoisearch.cfg configuration file path: the ipoisearch.cfg is under the resource directory. */
);


/** \brief   Start searching POI; \n
            It is a synchronous, non-reentrant and non-thread-safe function.\n
			           The information szSearchParam contains: \n
                                query : Query \n
                                nearby : The nearby of a specific location. If 0 is entered, it means search the nearby XXX of the current location. If the POI is entered, it means search the nearby XXX of a specified location (the value of the POI). \n
                                lon : Current location - longitude, GCJ-02\n
                                lat : Current location - latitude, GCJ-02\n
                                area : The municipal administrative region \n
                                distance : Distance in km \n
                                outputkcode=1 : It means the K code is outputted. Otherwise, the longitude and latitude are outputted. \n
                                retn : The number of the results returned \n
            The szSearchParam example: query=4S shop;nearby=Guogou Plaza;area=;retn=3;lon=117.2559342720;lat=31.8543551494;distance=5;\n
            The szResult includes the following information:\n
                            address : Address\n
                            area: Administrative region\n
                            category: Category\n
                            distance: Distance\n
                            flag: Flag\n
                            latitude: Latitude \n
                            longitude: Longitude \n
                            kcode: K code\n
                            poiname: The name of POI\n
                            score: Score\n
            The szResult example:\n
                        {\n
                            "result" : [\n
                                {\n
                                    "address" : "No. 32 Longhe Road",\n
                                    "area" : "Shushan District, Hefei, Anhui Province",\n
                                    "category" : "Automobile service#Automobile decoration",\n
                                    "distance" : "638.319",\n
                                    "flag" : "npi",\n
                                    "latitude" : "31.8486506137092",\n
                                    "longitude" : "117.256689908521",\n
                                    "name" : "Jinqiao Car Decoration Center",\n
                                    "score" : "1.000"\n
                                },\n
                                {\n
                                    "address" : "No. 28, Feixi Road",\n
                                    "area" : "Shushan District, Hefei, Anhui Province",\n
                                    "category" : "Automobile service#Automobile decoration",\n
                                    "distance" : "889.052",\n
                                    "flag" : "npi",\n
                                    "latitude" : "31.8471359864845",\n
                                    "longitude" : "117.259980116679",\n
                                    "name" : "Heifei Jietai Automobile Service 3S Shop ",\n
                                    "score" : "1.000"\n
                                },\n
                                {\n
                                    "address" : "West First Ring Road",\n
                                    "area" : " Shushan District, Hefei, Anhui Province",\n
                                    "category" : " Automobile service#Automobile decoration",\n
                                    "distance" : "1076.698",\n
                                    "flag" : "npi",\n
                                    "latitude" : "31.8629652707024",\n
                                    "longitude" : "117.250718432282",\n
                                    "name" : "Tribal Automobile Decoration Center",\n
                                    "score" : "1.000"\n
                                }\n
                            ]\n
                        }\n
            The search results are strings in the JSON format. The assigned value of szResult must be null, and no space needs to be allocated or released. The space is allocated or released in the POI search module. \n
 * \param	szSearchParam     [In] The search parameter\n
 * \param	szResult          [Out] The search results, which are strings in the JSON format \n
 * \retval  ISSErrID               Whether the search is successfully initiated or not.\n
 * \retval  ISS_SUCCESS            The search is successfully initiated.\n
 * \retval  ISS_ERROR_INVALID_PARA Invalid parameter value, for example: The format of the search sentence is incorrect. The pointer value of the search result is not null. \n
 

 */
ISSErrID ISSPSAPI ISSPSStart(
    const char* szSearchParam,		/* [in] Search parameter */
	char** szResult				/* [out] The search results, which are strings in the JSON format */
);


/** \brief  Stop the POI search;\n
            It is a synchronous, non-reentrant and non-thread-safe function. \n
 * \retval  ISSErrID       Whether the search is stopped successfully or not. \n
 * \retval  ISS_SUCCESS    The POI search is stopped successfully. \n
 * \retval  ISS_ERROR_FAIL Fails to stop the POI search. \n
 */
ISSErrID ISSPSAPI ISSPSStop();

/** \brief  Destroy POI search;\n
            It is a synchronous, non-reentrant and non-thread-safe function. \n
 * \retval  ISSErrID       Whether the POI search is destroyed successfully or not. \n
 * \retval  ISS_SUCCESS    The POI search is successfully destroyed. \n
 * \retval  ISS_ERROR_FAIL Fails to destroy the POI search. \n
 */
ISSErrID ISSPSAPI ISSPSDestroy();


#ifdef __cplusplus
}
#endif

#endif /* _ISS_POISEARCH_H_ */
