#ifdef ISS_AIUI

#ifndef DEF_SEOPT_H_
#define DEF_SEOPT_H_

#include "Def.h"
#include "iss_seopt.h"
#include "VAInc.h"
#include "ISSSeoptAdapter.h"
#include <string>

class DefSeopt : public Def {
public:
	const char * resDir; // ×ÊÔ´Â·¾¶

	DefSeopt();
};

#endif // !DEF_SEOPT_H_

#endif


