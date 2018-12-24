#ifndef DEFSR_H_
#define DEFSR_H_

#include "iss_auth.h"
#include "VAInc.h"
#include "Def.h"

#ifdef ISS_SR_3
#include "iss_sr_3.h"
#else
#include "iss_sr.h"
#endif
#include "ISSSRAdapter.h"
#include <string>

//这个是不是是多余的，可以去掉？？？
class SrMsgProcInfo {
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
	std::string getlParam() { return string((const char *)lParam_); };
	void init() { pUsrArg_ = NULL; uMsg_ = 0; wParam_ = 0; lParam_ = NULL; };
};

class DefSR : public Def {
public:
	char * resDir; // 资源路径
#if VA_VC
	wchar_t * resDirW;
#endif

	std::string machineCode;

	// 音频
	std::string srPcm_CallBaiYaWei;
	std::string srPcm_NavigateIflytek;
	std::string srPcm_AnyHotel;
	std::string srPcm_CallZhangSan;
	std::string srPcm_TodayWeather;
	std::string srPcm_NavigateWXD;
	std::string srPcm_SeoptSR;

	// 词典
	std::string dict_500;
	std::string dict_1000;
	std::string dict_5000;
	std::string dict_2000;

	//异常词典
	std::string wrong_format;
	std::string wrong_format1;
	std::string wrong_format2;
	std::string wrong_format3;
	std::string wrong_format4;
	std::string wrong_format5;
	std::string wrong_format6;
	std::string wrong_format7;
	std::string wrong_format8;
	std::string wrong_format9;

	std::string wrong_info;
	std::string wrong_info1;
	std::string wrong_info2;
	std::string wrong_info3;
	std::string wrong_info4;
	std::string wrong_info5;
	std::string wrong_info6;

	// 回调消息
	static SrMsgProcInfo msgProcInfo;
	static int msgInitStatus;
	static int msgCloudInitStatus;
	static int msgUpLoadDictToLocalStatus;
	static int msgUpLoadDictToCloudStatus;
	static int msgVolumeLevel;
	static int msgResponseTimeout;
	static int msgSpeechStart;
	static int msgSpeechTimeOut;
	static int msgSpeechEnd;
	static int msgError;
	static int msgResult;
	static int msgSRResult;
	static int msgTPPResult;
	static int msgLoadBigSrResStatus;
	static int msgErrorDecodingAudio;
	static int msgPreResult;
	static int msgRealTimeResult;
	static int msgWaitingForCloudResult;
	static int msgResUpdateStart;
	static int msgResUpdateEnd;
	static int msgWaitingForLocalResult;
	static int msgSTKSResult;
	static int msgONESHOTMVWResult;
	static int msgUpLoadDataToCloudStatus;
	static int msgCloudResult;
	static int msgLocalResult;

	static std::string sr_result;
	static std::string SRsr_result;
	static std::string TPP_result;
	static std::string msgSid;
	static int error_code;
	static int sr_response_time; // 开始送音频到给出识别结果的时间
	static int speechEnd_time; // 开始送音频到给出SpeechEnd消息的时间
	static int vad_SpeechStart; // vad前端点
	static int vad_SpeechEnd; // vad后端点

	DefSR();
	~DefSR();
	static void onMsgProc(
		const void*         pUsrArg,     /* [In] User defined parameter */
		unsigned int        uMsg,        /* [In] The message identifier */
		unsigned int        wParam,      /* [In] Message additional information */
		const void*         lParam        /* [In] Message additional information */
		);
	static void initMsg();
};

#endif // !DEFSR_H_

