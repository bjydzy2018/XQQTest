#include "DefSR.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>

using namespace VA;
using std::string;
using std::vector;
using boost::lexical_cast;
using boost::regex;

SrMsgProcInfo DefSR::msgProcInfo;
int DefSR::msgInitStatus = 0;
int DefSR::msgCloudInitStatus = 0;
int DefSR::msgUpLoadDictToLocalStatus = 0;
int DefSR::msgUpLoadDictToCloudStatus = 0;
int DefSR::msgVolumeLevel = 0;
int DefSR::msgResponseTimeout = 0;
int DefSR::msgSpeechStart = 0;
int DefSR::msgSpeechTimeOut = 0;
int DefSR::msgSpeechEnd = 0;
int DefSR::msgError = 0;
int DefSR::msgResult = 0;
int DefSR::msgSRResult = 0;
int DefSR::msgTPPResult = 0;
int DefSR::msgLoadBigSrResStatus = 0;
int DefSR::msgErrorDecodingAudio = 0;
int DefSR::msgPreResult = 0;
int DefSR::msgRealTimeResult = 0;
int DefSR::msgWaitingForCloudResult = 0;
int DefSR::msgResUpdateStart = 0;
int DefSR::msgResUpdateEnd = 0;
int DefSR::msgWaitingForLocalResult = 0;
int DefSR::msgSTKSResult = 0;
int DefSR::msgONESHOTMVWResult = 0;
int DefSR::msgUpLoadDataToCloudStatus = 0;
int DefSR::msgCloudResult = 0;
int DefSR::msgLocalResult = 0;

std::string DefSR::sr_result("");
std::string DefSR::SRsr_result("");
std::string DefSR::TPP_result("");
std::string DefSR::msgSid("");
int DefSR::error_code = 0;
int DefSR::sr_response_time = 0;
int DefSR::speechEnd_time = 0;
int DefSR::vad_SpeechStart = 0;
int DefSR::vad_SpeechEnd = 0;

void DefSR::initMsg() {
	msgInitStatus = 0;
	msgCloudInitStatus = 0;
	msgUpLoadDictToLocalStatus = 0;
	msgUpLoadDictToCloudStatus = 0;
	msgVolumeLevel = 0;
	msgResponseTimeout = 0;
	msgSpeechStart = 0;
	msgSpeechTimeOut = 0;
	msgSpeechEnd = 0;
	msgError = 0;
	msgResult = 0;
	msgSRResult = 0;
	msgTPPResult = 0;
	msgLoadBigSrResStatus = 0;
	msgErrorDecodingAudio = 0;
	msgPreResult = 0;
	msgRealTimeResult = 0;
	msgWaitingForCloudResult = 0;
	msgResUpdateStart = 0;
	msgResUpdateEnd = 0;
	msgWaitingForLocalResult = 0;
	msgSTKSResult = 0;
	msgONESHOTMVWResult = 0;
	msgUpLoadDataToCloudStatus = 0;
	msgCloudResult = 0;
	msgLocalResult = 0;

	sr_result = "";
	SRsr_result = "";
	TPP_result = "";
	msgSid = "";
	error_code = 0;
	sr_response_time = 0;
	speechEnd_time = 0;
	vad_SpeechStart = 0;
	vad_SpeechEnd = 0;

	msgProcInfo.init();
}

void DefSR::onMsgProc(
	const void*         pUsrArg,     /* [In] User defined parameter */
	unsigned int        uMsg,        /* [In] The message identifier */
	unsigned int        wParam,      /* [In] Message additional information */
	const void*         lParam       /* [In] Message additional information */
) {
	msgProcInfo.setpUsrArg(pUsrArg);
	msgProcInfo.setuMsg(uMsg);
	msgProcInfo.setwParam(wParam);
	msgProcInfo.setlParam(lParam);

	string uMsg_str("");

	switch (uMsg) {
	case ISS_SR_MSG_InitStatus:
		uMsg_str = "ISS_SR_MSG_InitStatus";
		if (0 == wParam)
			DEBUG_TRACE_INFOR("init sr success");
		else
			DEBUG_TRACE_ERROR(("init sr failed, wParam: " + lexical_cast<string>(wParam)).c_str());
		++msgInitStatus;
		break;
	case ISS_SR_MSG_CloudInitStatus:
		uMsg_str = "ISS_SR_MSG_CloudInitStatus";
		if (0 == wParam)
			DEBUG_TRACE_INFOR("cloud init sr success");
		else
			DEBUG_TRACE_ERROR(("cloud init sr failed, wParam: " + lexical_cast<string>(wParam)).c_str());
		++msgCloudInitStatus;
		break;
	case ISS_SR_MSG_UpLoadDictToLocalStatus:
		uMsg_str = "ISS_SR_MSG_UpLoadDictToLocalStatus";
		++msgUpLoadDictToLocalStatus;
		break;
	case ISS_SR_MSG_UpLoadDictToCloudStatus:
		uMsg_str = "ISS_SR_MSG_UpLoadDictToCloudStatus";
		++msgUpLoadDictToCloudStatus;
		msgSid = (const char *)lParam;
		//DEBUG_TRACE("msgSid:%s", PrtU8(msgSid.c_str()));
		break;
#ifdef ISS_SR_3
	case ISS_SR_MSG_ResponseTimeout:
		uMsg_str = "ISS_SR_MSG_ResponseTimeout";
		++msgResponseTimeout;
		break;
	case ISS_SR_MSG_SpeechTimeOut:
		uMsg_str = "ISS_SR_MSG_SpeechTimeOut";
		++msgSpeechTimeOut;
		break;
	case ISS_SR_MSG_ErrorDecodingAudio:
		uMsg_str = "ISS_SR_MSG_ErrorDecodingAudio";
		++msgErrorDecodingAudio;
		break;
	case ISS_SR_MSG_PreResult:
		uMsg_str = "ISS_SR_MSG_PreResult";
		++msgPreResult;
		break;
	case ISS_SR_MSG_RealTimeResult:
		uMsg_str = "ISS_SR_MSG_RealTimeResult";
		++msgRealTimeResult;
		break;
	case ISS_SR_MSG_Res_Update_Start:
		uMsg_str = "ISS_SR_MSG_Res_Update_Start";
		++msgResUpdateStart;
		break;
	case ISS_SR_MSG_Res_Update_End:
		uMsg_str = "ISS_SR_MSG_Res_Update_End";
		++msgResUpdateEnd;
		break;
	case ISS_SR_MSG_STKS_Result:
		uMsg_str = "ISS_SR_MSG_STKS_Result";
		++msgSTKSResult;
		break;
	case ISS_SR_MSG_ONESHOT_MVWResult:
		uMsg_str = "ISS_SR_MSG_ONESHOT_MVWResult";
		++msgONESHOTMVWResult;
		break;
#endif
	case ISS_SR_MSG_SpeechStart:
		uMsg_str = "ISS_SR_MSG_SpeechStart";
		DEBUG_TRACE("ISS_SR_MSG_SpeechStart");
		vad_SpeechStart = wParam;
		++msgSpeechStart;
		break;
	case ISS_SR_MSG_SpeechEnd:
		uMsg_str = "ISS_SR_MSG_SpeechEnd";
		DEBUG_TRACE("ISS_SR_MSG_SpeechEnd");
		vad_SpeechEnd = wParam;
		++msgSpeechEnd;
		break;
	case ISS_SR_MSG_Error:
		uMsg_str = "ISS_SR_MSG_Error";
		error_code = wParam;
		++msgError;
		break;
	case ISS_SR_MSG_Result:
		uMsg_str = "ISS_SR_MSG_Result";
		++msgResult;
		sr_result = (const char *)lParam;
		break;
	case ISS_SR_MSG_SRResult:
		uMsg_str = "ISS_SR_MSG_SRResult";
		++msgSRResult;
		SRsr_result = (const char *)lParam;
		break;
	case ISS_SR_MSG_TPPResult:
		uMsg_str = "ISS_SR_MSG_TPPResult";
		++msgTPPResult;
		TPP_result = (const char *)lParam;
		break;
	case ISS_SR_MSG_WaitingForCloudResult:
		uMsg_str = "ISS_SR_MSG_WaitingForCloudResult";
		++msgWaitingForCloudResult;
		break;
	case ISS_SR_MSG_WaitingForLocalResult:
		uMsg_str = "ISS_SR_MSG_WaitingForLocalResult";
		++msgWaitingForLocalResult;
		break;
	case ISS_SR_MSG_VolumeLevel:
		++msgVolumeLevel;
		break;
#ifndef ISS_SR_3
	case ISS_SR_MSG_UpLoadDataToCloudStatus:
		uMsg_str = "ISS_SR_MSG_UpLoadDataToCloudStatus";
		++msgUpLoadDataToCloudStatus;
		break;
	case ISS_SR_MSG_CloudResult:
		uMsg_str = "ISS_SR_MSG_CloudResult";
		++msgCloudResult;
		break;
	case ISS_SR_MSG_LocalResult:
		uMsg_str = "ISS_SR_MSG_LocalResult";
		++msgLocalResult;
		break;
#endif
	default:
		break;
	}

	if (uMsg != ISS_SR_MSG_VolumeLevel) {
		if (lParam != NULL) {
			DEBUG_TRACE("get callback msg: %d_%s, wParam: %d, lParam: %s", uMsg, uMsg_str.c_str(), wParam, PrtU8((const char *)lParam));
		}
		else {
			DEBUG_TRACE("get callback msg: %d_%s, wParam: %d", uMsg, uMsg_str.c_str(), wParam);
		}
	}
}

DefSR::DefSR():machineCode("ThisIsALongFakeMachineCodeForAIUI") {
#if VA_VC
	string exePath = boost::filesystem::initial_path<boost::filesystem::path>().string();
	vector <string> exePaths;
	split_regex(exePaths, exePath, regex("bin\\\\Release"));
	string resPath = exePaths[0] + "bin\\SRRes";
	std::cout << resPath << std::endl;

	const char * resPath_narrow = resPath.c_str();
	resDir = new char[strlen(resPath_narrow) + 1];
	memcpy(resDir, resPath_narrow, strlen(resPath_narrow) + 1);

	const wchar_t * resPath_wide = widen(resPath).c_str();
	resDirW = new wchar_t[wcslen(resPath_wide) + 1];
	memcpy(resDirW, resPath_wide, wcslen(resPath_wide) + 1);
#elif VA_ANDROID
	resDir = "/sdcard/iflytek/res/sr";
#else
	resDir = "SRRes";
#endif

	srPcm_CallBaiYaWei = testRes_path + "pcm_src/sr/PhoneBaiYawei.wav";
	srPcm_NavigateIflytek = testRes_path + "pcm_src/sr/NavigateIflytek.wav";
	srPcm_AnyHotel = testRes_path + "pcm_src/sr/AnyHotel.wav";
	srPcm_CallZhangSan = testRes_path + "pcm_src/sr/PhoneZhangsan.wav";
	srPcm_TodayWeather = testRes_path + "pcm_src/sr/TodayWeather.pcm";
	srPcm_NavigateWXD = testRes_path + "pcm_src/sr/NavigateWXD.pcm";
	srPcm_SeoptSR = testRes_path + "pcm_src/sr/SeoptSR.pcm";

	dict_500 = readFile(testRes_path + "dict/contactDict-500.txt", false);
	dict_1000 = readFile(testRes_path + "dict/contactDict-1000.txt", false);
	dict_5000 = readFile(testRes_path + "dict/contactDict-1500.txt", false);
	dict_2000 = readFile(testRes_path + "dict/contactDict-2000.txt", false);

	wrong_format = readFile(testRes_path + "dict/wrong_format.txt", false);
	wrong_format1 = readFile(testRes_path + "dict/wrong_format1.txt", false);
	wrong_format2 = readFile(testRes_path + "dict/wrong_format2.txt", false);
	wrong_format3 = readFile(testRes_path + "dict/wrong_format3.txt", false);
	wrong_format4 = readFile(testRes_path + "dict/wrong_format4.txt", false);
	wrong_format5 = readFile(testRes_path + "dict/wrong_format5.txt", false);
	wrong_format6 = readFile(testRes_path + "dict/wrong_format6.txt", false);
	wrong_format7 = readFile(testRes_path + "dict/wrong_format7.txt", false);
	wrong_format8 = readFile(testRes_path + "dict/wrong_format8.txt", false);
	wrong_format9 = readFile(testRes_path + "dict/wrong_format9.txt", false);

	wrong_info = readFile(testRes_path + "dict/wrong_info.txt", false);
	wrong_info1 = readFile(testRes_path + "dict/wrong_info1.txt", false);
	wrong_info2 = readFile(testRes_path + "dict/wrong_info2.txt", false);
	wrong_info3 = readFile(testRes_path + "dict/wrong_info3.txt", false);
	wrong_info4 = readFile(testRes_path + "dict/wrong_info4.txt", false);
	wrong_info5 = readFile(testRes_path + "dict/wrong_info5.txt", false);
	wrong_info6 = readFile(testRes_path + "dict/wrong_info6.txt", false);

}

DefSR::~DefSR() {
	initMsg();
#if VA_VC
	delete[] resDir;
	delete[] resDirW;
#endif
}
