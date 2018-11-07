#ifndef BASECONFIGPARSER_YJREN_HEADER
#define BASECONFIGPARSER_YJREN_HEADER


#include "VAString.h"
#include "json/json.h"

namespace VA {

typedef enum {
	JSON_VALUE_TYPE_OBJECT,
	JSON_VALUE_TYPE_ARRAY,
	JSON_VALUE_TYPE_INT,
	JSON_VALUE_TYPE_STRING,
	JSON_VALUE_TYPE_BOOL,
	JSON_VALUE_TYPE_NUM
} JSON_VALUE_TYPE;

class BaseConfigParser {
protected:
	std::string strCfgPath;
	char* pCfgStr;
	U8String u8CfgStr;
	Json::Reader cfg_json;
	Json::Value root;

public:
	BaseConfigParser();
	virtual ~BaseConfigParser();
	bool loadConfigFile(const char *resPath, const char *cfgFileName);
	bool GetJsonObjMbr(Json::Value root, Json::Value *mbr_value, const char *key, JSON_VALUE_TYPE type);

	virtual bool parse() = 0;
};

}
#endif
