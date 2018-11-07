#ifndef DEFCATA_H_
#define DEFCATA_H_

#include "iss_cata.h"
#include "VAInc.h"
#include "Def.h"
#include "CommonFun.h"

#include <string>

class CataMsgProcInfo {
private:
	const void *    pUsrArg_;     /* [In] User defined parameter */
	unsigned int    uMsg_;        /* [In] The message identifier */
	unsigned int    wParam_;      /* [In] Message additional information */
	const void *    lParam_;      /* [In] Message additional information */

public:
	void setpUsrArg(const void * pUsrArg) { pUsrArg_ = pUsrArg; };
	void setuMsg(unsigned int uMsg) { uMsg_ = uMsg; };
	void setwParam(signed int wParam) { wParam_ = wParam; };
	void setlParam(const void * lParam) { lParam_ = lParam; };
	const void * getpUsrArg() { return pUsrArg_; };
	unsigned int getuMsg() { return uMsg_; };
	unsigned int getwParam() { return wParam_; };
	const void * getlParam() { return lParam_; };
	void init() { pUsrArg_ = NULL; uMsg_ = 0; wParam_ = 0; lParam_ = NULL; };
};

class DefCATA : public Def {
public:
	char * resDir; // 资源路径
#if VA_VC
	wchar_t * resDirW;
#endif

	// 回调消息
	static CataMsgProcInfo msgProcInfo;
	static int msgSearchResult;
	static int msgLoadResourceFail;
	static int msgProc;

	DefCATA();
	static void initMsg();
	static void onCataMsgProc(
		const void *        pUsrArg,      /* [In] User defined parameter */
		unsigned int        uMsg,         /* [In] Message identifier */
		unsigned int        wParam,       /* [In] Message additional information */
		const void *        lParam        /* [In] Message additional information */
		);

	std::string parseSegValues(std::string);
	std::string getAddBin(std::string);
	std::string getSrhBin(std::string);
	std::string getSrhText(std::string);
};

#endif // !DEFCATA_H_

