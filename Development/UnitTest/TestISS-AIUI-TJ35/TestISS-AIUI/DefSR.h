#ifndef DEFSR_H_
#define DEFSR_H_

#include "iss_auth.h"
#include "VAInc.h"
#include "Def.h"

#include "iss_sr_aiui.h"

#include <string>

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
	char * resDir; // ��Դ·��
#if VA_VC
	wchar_t * resDirW;
#endif

	std::string machineCode;

	//�ı�
	std::string srText_STKS;
	std::string srText_STKS_error;

	// ��Ƶ
	std::string srPcm_CallBaiYaWei;
	std::string srPcm_NavigateIflytek;
	std::string srPcm_AnyHotel;
	std::string srPcm_CallZhangSan;

	std::string srPcm_TodayWeather;
	std::string srPcm_NavigateWXD;
	std::string srPcm_OneShot;
	std::string srPcm_OSHW;

	std::string srPcm_SpeechTail;
	std::string srPcm_SpeechTimeOut;
	std::string srPcm_ResponseTimeOut;
	std::string srPcm_AppendAudioData100ms;
	std::string srPcm_AppendNoiseAudioData;
	std::string srPcm_SetResponseTimeOut;
	std::string srPcm_SetMvwTimeOut;

	std::string srPcm_SceneAll;
	std::string srPcm_ScenePoi;
	std::string srPcm_SceneContact;
	std::string srPcm_SceneSelect;
	std::string srPcm_SceneConfirm;
	std::string srPcm_SceneAnswerCall;

	std::string srPcm_STKS;
	
	// �ʵ�
	std::string dict_500;
	std::string dict_1000;
	std::string dict_5000;
	std::string dict_2000;

	//�쳣�ʵ�
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

	std::string OutsideDict1;
	std::string OutsideDict2;

	// �ص���Ϣ
	static SrMsgProcInfo msgProcInfo;
	static int msgInitStatus;
	static int msgUpLoadDictToLocalStatus;
	static int msgUpLoadDictToLocalStatusSuccess;
	static int msgUpLoadDictToCloudStatus;
	static int msgUpLoadDictToCloudStatusSuccess;
	static int msgVolumeLevel;
	static int msgResponseTimeout;
	static int msgSpeechStart;
	static int msgSpeechTimeOut;
	static int msgSpeechEnd;
	static int msgError;
	static int msgResult;
	static int msgSRResult;
	static int msgTPPResult;
	static int msgTransResult;
	static int msgLoadBigSrResStatus;
	static int msgErrorDecodingAudio;
	static int msgPreResult;
	static int msgCloudInitStatus;
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
	static int error_code;
	static int sr_response_time; // ��ʼ����Ƶ������ʶ������ʱ��
	static int speechEnd_time; // ��ʼ����Ƶ������SpeechEnd��Ϣ��ʱ��
	static int vad_SpeechStart; // vadǰ�˵�
	static int vad_SpeechEnd; // vad��˵�

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

