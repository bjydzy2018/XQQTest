#ifndef DEF_SEARCH_FOR_NLI_H_
#define DEF_SEARCH_FOR_NLI_H_

#include "Def.h"

#ifndef ISS_SR_3
#include "libSearchForNli.h"
#endif
#include "VAInc.h"
#include "test_log.h"

#include <string>

class DefSearchForNLI : public Def {
public:
	const char * resDir; // ×ÊÔ´Â·¾¶

	DefSearchForNLI();
	std::string getSearchErrorCode(std::string);
};

#endif // !DEF_SEARCH_FOR_NLI_H_
