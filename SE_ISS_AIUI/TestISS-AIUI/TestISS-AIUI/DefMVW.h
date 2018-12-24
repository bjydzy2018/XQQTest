#ifndef DEFMVW_H_
#define DEFMVW_H_

#include "Def.h"
#include "iss_mvw.h"
#include "VAInc.h"
#include "ISSMVWAdapter.h"
#include <string>

class WakeupInfo {
private:
	void *			pUsrArg_;
	unsigned int	nMvwScene_;
	unsigned int	nMvwId_;
	signed int		nMvwScore_;
	std::string			lParam_;

public:
	void setpUsrArg(void * pUsrArg) { pUsrArg_ = pUsrArg; };
	void setnMvwScene(unsigned int nMvwScene) { nMvwScene_ = nMvwScene; };
	void setnMvwId(unsigned int nMvwId) { nMvwId_ = nMvwId; };
	void setnMvwScore(signed int nMvwScore) { nMvwScore_ = nMvwScore; };
	void setlParam(std::string lParam) { lParam_ = lParam; };
	void * getpUsrArg() { return pUsrArg_; };
	unsigned int getnMvwScene() { return nMvwScene_; };
	unsigned int getnMvwId() { return nMvwId_; };
	signed int getnMvwScore() { return nMvwScore_; };
	std::string getlParam() { return lParam_; };
	void init() { nMvwScene_ = 0; nMvwId_ = 0; nMvwScore_ = 0; lParam_ = ""; };
};

class MvwMsgProcInfo {
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

class DefMVW : public Def {
public:
	// 资源路径
	const char * resDir;
#if VA_VC
	const wchar_t* resDirW;
#endif

	static double vw_time; // 从开始送音频到出唤醒结果的时间
	
	// 唤醒音频路径
	std::string mvwPcm_global;
	std::string mvwPcm_confirm;
	std::string mvwPcm_cancel;
	std::string mvwPcm_select;
	std::string mvwPcm_answer;
	std::string mvwPcm_hangup;
	std::string mvwPcm_first;
	std::string mvwPcm_second;
	std::string mvwPcm_third;
	std::string mvwPcm_fourth;
	std::string mvwPcm_fifth;
	std::string mvwPcm_sixth;
	std::string mvwPcm_last;

	std::string mvwPcm_first_en;
	std::string mvwPcm_confirm_en;

	std::string mvwPcm_KaiYi;
	std::string mvwPcm_KaiYi_long;
	std::string mvwPcm_ZhiDou;
	std::string mvwPcm_ChenSheng;
	std::string mvwPcm_DaLianJiPai;
	std::string mvwPcm_HuShiCiShen;
	std::string mvwPcm_ChongQingXiaoMian;
	std::string mvwPcm_NavigateIflytek;
	std::string mvwPcm_iflytek;
	std::string mvwPcm_ShenTong;
	std::string mvwPcm_ZhengKai;
	std::string mvwPcm_PhoneZhangsan;
	std::string mvwPcm_PhoneBaiYawei;
	std::string mvwPcm_ZhiDou_long;
	std::string mvwPcm_LingXi;
	std::string mvwPcm_LingXi_long;
	std::string mvwPcm_123;
	std::string mvwPcm_HelloHello;

	std::string mvwKeyWord_KaiYiNiHao;
	std::string mvwKeyWord_KaiYi_LingXi;
	std::string mvwKeyWord_123;

	// 回调消息
	static WakeupInfo wakeupInfo;
	static MvwMsgProcInfo msgProcInfo;
	static int msgInitStatus;
	static int msgVolumeLevel;
	static int msgError;
	static int msgResult;
	static int msgTimeout;
	static int msgWakeup;
	static int msgProc;

	DefMVW();
	static void initMsg();
	static void onWakeup(
		void*			pUsrArg,	/* [In] 用户自定义参数 */
		unsigned int	nMvwScene,	/* [In] 多唤醒场景 */
		unsigned int	nMvwId,		/* [In] 唤醒词ID */
		signed int		nMvwScore,  /* [In] 唤醒的置信度得分 */
		const char*		lParam
	);
	static void onMsgProc(
		const void*     pUsrArg,    /* [In] User defined parameter */
		unsigned int    uMsg,       /* [In] The message identifier */
		unsigned int    wParam,     /* [In] Message additional information */
		const void*     lParam      /* [In] Message additional information */
	);
};

#endif