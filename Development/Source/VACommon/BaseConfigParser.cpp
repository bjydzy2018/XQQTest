#include <cstdio>
#include <string>
#include <cstdlib>

#include "VADebug.h"
#include "BaseConfigParser.h"

using namespace VA;

#if VA_DEBUG
    static VA::CTrace BaseConfigParserTrace("BaseConfigParser", VA_DEBUG_LEVEL_DEBUG);
#else
    static VA::CTrace BaseConfigParserTrace("BaseConfigParser", VA_DEBUG_LEVEL_ERROR);
#endif
#define DEBUG_TRACE(...) (BaseConfigParserTrace.TraceLevel(VA_DEBUG_LEVEL_INFOR, __VA_ARGS__))
#define DEBUG_TRACE_DEBUG(...) (BaseConfigParserTrace.TraceLevel(VA_DEBUG_LEVEL_DEBUG, __VA_ARGS__))
#define DEBUG_TRACE_INFOR(...) (BaseConfigParserTrace.TraceLevel(VA_DEBUG_LEVEL_INFOR, __VA_ARGS__))
#define DEBUG_TRACE_ERROR(...) (BaseConfigParserTrace.TraceLevel(VA_DEBUG_LEVEL_ERROR, __VA_ARGS__))

BaseConfigParser::BaseConfigParser()
{
	pCfgStr = NULL;
}

bool BaseConfigParser::loadConfigFile(const char *resPath, const char *cfgFileName)
{
#if 1
	strCfgPath = resPath;

    std::string szCfgFilename;
    szCfgFilename = resPath;
	szCfgFilename += "/";
    szCfgFilename += cfgFileName;
    FILE* configFile = fopen(szCfgFilename.c_str(), "rb");
#else
//bool BaseConfigParser::loadConfigFile(const wchar_t *resPath, const wchar_t *cfgFileName){
	std::wstring szCfgFilename;
	szCfgFilename = resPath;
	szCfgFilename += L"\\";
	szCfgFilename += cfgFileName;
	FILE* configFile = _wfopen(szCfgFilename.c_str(), L"rb");
#endif

	if(pCfgStr)
	{
		free(pCfgStr);
		pCfgStr = NULL;
	}

	if (configFile)
	{
		int nSize = 0;
		fseek(configFile, 0, SEEK_END);
		nSize = ftell(configFile);
		fseek(configFile, 0, SEEK_SET);
		pCfgStr = (char*) malloc(nSize + 1);
		memset(pCfgStr, 0, nSize + 1);
		fread(pCfgStr, 1, nSize, configFile);
		u8CfgStr = pCfgStr;
		u8CfgStr.TrimLeft("\xEF\xBB\xBF");
		memset(pCfgStr, 0, nSize + 1);
		strcpy(pCfgStr, u8CfgStr.GetBuffer());
		fclose(configFile);
        configFile=NULL;
	}
	else
	{
    	DEBUG_TRACE_ERROR("cfg file %s not found", szCfgFilename.c_str());
		return false;
	}

	return true;
}

bool BaseConfigParser::GetJsonObjMbr(Json::Value root, Json::Value *member, const char *key, JSON_VALUE_TYPE type)
{
	if (root.isObject() && root.isMember(key)) {
		*member = root[key];
		switch (type) {
		case JSON_VALUE_TYPE_OBJECT:
			return member->isObject();
		case JSON_VALUE_TYPE_ARRAY:
			return member->isArray();
		case JSON_VALUE_TYPE_INT:
			return member->isInt();
		case JSON_VALUE_TYPE_STRING:
			return member->isString();
		case JSON_VALUE_TYPE_BOOL:
			return member->isBool();
		default:
			return false;
		}
	}

	return false;
}

BaseConfigParser::~BaseConfigParser()
{
	if(pCfgStr)
	{
		free(pCfgStr);
		pCfgStr = NULL;
	}
}