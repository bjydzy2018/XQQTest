#include "DefSR.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"
#include "VAFileSystem.h"
#include <vector>


using namespace VA;
using std::string;
using std::vector;

SrMsgProcInfo DefSR::msgProcInfo;
int DefSR::msgInitStatus = 0;
int DefSR::msgUpLoadDictToLocalStatus = 0;
int DefSR::msgUpLoadDictToLocalStatusSuccess = 0;
int DefSR::msgUpLoadDictToCloudStatus = 0;
int DefSR::msgUpLoadDictToCloudStatusSuccess = 0;
int DefSR::msgVolumeLevel = 0;
int DefSR::msgResponseTimeout = 0;
int DefSR::msgSpeechStart = 0;
int DefSR::msgSpeechTimeOut = 0;
int DefSR::msgSpeechEnd = 0;
int DefSR::msgError = 0;
int DefSR::msgResult = 0;
int DefSR::msgSRResult = 0;
int DefSR::msgTPPResult = 0;
int DefSR::msgTransResult = 0;
int DefSR::msgLoadBigSrResStatus = 0;
int DefSR::msgErrorDecodingAudio = 0;
int DefSR::msgPreResult = 0;
int DefSR::msgCloudInitStatus = 0;
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
int DefSR::error_code = 0;
int DefSR::sr_response_time = 0;
int DefSR::speechEnd_time = 0;
int DefSR::vad_SpeechStart = 0;
int DefSR::vad_SpeechEnd = 0;

void DefSR::initMsg() {
	msgInitStatus = 0;
	msgUpLoadDictToLocalStatus = 0;
	msgUpLoadDictToLocalStatusSuccess = 0;
	msgUpLoadDictToCloudStatus = 0;
	msgUpLoadDictToCloudStatusSuccess = 0;
	msgVolumeLevel = 0;
	msgResponseTimeout = 0;
	msgSpeechStart = 0;
	msgSpeechTimeOut = 0;
	msgSpeechEnd = 0;
	msgError = 0;
	msgResult = 0;
	msgSRResult = 0;
	msgTPPResult = 0;
	msgTransResult = 0;
	msgLoadBigSrResStatus = 0;
	msgErrorDecodingAudio = 0;
	msgPreResult = 0;
	msgCloudInitStatus = 0;
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
			DEBUG_TRACE_ERROR("init sr failed, wParam: %d",wParam);
		++msgInitStatus;
		break;
	case ISS_SR_MSG_CloudInitStatus:
		uMsg_str = "ISS_SR_MSG_CloudInitStatus";
		if (0 == wParam)
			DEBUG_TRACE_INFOR("cloud init sr success");
		else
			DEBUG_TRACE_ERROR("cloud init sr failed, wParam: %d",wParam);
		++msgCloudInitStatus;
		break;
	case ISS_SR_MSG_UpLoadDictToLocalStatus:
		uMsg_str = "ISS_SR_MSG_UpLoadDictToLocalStatus";
		if (0 == wParam){
			DEBUG_TRACE_INFOR("uploaddict to local success");
			++msgUpLoadDictToLocalStatusSuccess;
		}
		else{
			DEBUG_TRACE_ERROR("uploaddict to local fail");
		}
		++msgUpLoadDictToLocalStatus;
		break;
	case ISS_SR_MSG_UpLoadDictToCloudStatus:
		uMsg_str = "ISS_SR_MSG_UpLoadDictToCloudStatus";
		if (0 == wParam){
			DEBUG_TRACE_INFOR("uploaddict to cloud success");
			++msgUpLoadDictToCloudStatusSuccess;
		}
		else{
			DEBUG_TRACE_ERROR("uploaddict to cloud fail");
		}
		++msgUpLoadDictToCloudStatus;
		break;
	case ISS_SR_MSG_ResponseTimeout:
		uMsg_str = "ISS_SR_MSG_ResponseTimeout";
		++msgResponseTimeout;
		break;
	case ISS_SR_MSG_SpeechStart:
		uMsg_str = "ISS_SR_MSG_SpeechStart";
		vad_SpeechStart = wParam;
		++msgSpeechStart;
		break;
	case ISS_SR_MSG_SpeechTimeOut:
		uMsg_str = "ISS_SR_MSG_SpeechTimeOut";
		++msgSpeechTimeOut;
		break;
	case ISS_SR_MSG_SpeechEnd:
		uMsg_str = "ISS_SR_MSG_SpeechEnd";
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
		break;
	case ISS_SR_MSG_TPPResult:
		uMsg_str = "ISS_SR_MSG_TPPResult";
		++msgTPPResult;
		break;
	case ISS_SR_MSG_TransResult:
		uMsg_str = "ISS_SR_MSG_TransResult";
		++msgTransResult;
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
	case ISS_SR_MSG_WaitingForCloudResult:
		uMsg_str = "ISS_SR_MSG_WaitingForCloudResult";
		++msgWaitingForCloudResult;
		break;
	case ISS_SR_MSG_Res_Update_Start:
		uMsg_str = "ISS_SR_MSG_Res_Update_Start";
		++msgResUpdateStart;
		break;
	case ISS_SR_MSG_Res_Update_End:
		uMsg_str = "ISS_SR_MSG_Res_Update_End";
		++msgResUpdateEnd;
		break;
	case ISS_SR_MSG_WaitingForLocalResult:
		uMsg_str = "ISS_SR_MSG_WaitingForLocalResult";
		++msgWaitingForLocalResult;
		break;
	case ISS_SR_MSG_STKS_Result:
		uMsg_str = "ISS_SR_MSG_STKS_Result";
		++msgSTKSResult;
		break;
	case ISS_SR_MSG_ONESHOT_MVWResult:
		uMsg_str = "ISS_SR_MSG_ONESHOT_MVWResult";
		++msgONESHOTMVWResult;
		break;
	case ISS_SR_MSG_VolumeLevel:
		++msgVolumeLevel;
		break;
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

DefSR::DefSR():machineCode("ThisIsAFakeMachineCode") {
#if VA_VC

	string exePath = GetAppPath();
	string resPath = exePath + "\\..\\SRRes";
	DEBUG_TRACE("Res Path:%s", resPath.c_str());

	const char * resPath_narrow = resPath.c_str();
	resDir = new char[strlen(resPath_narrow) + 1];
	memcpy(resDir, resPath_narrow, strlen(resPath_narrow) + 1);

	const wchar_t * resPath_wide = widen(resPath).c_str();
	resDirW = new wchar_t[wcslen(resPath_wide) + 1];
	memcpy(resDirW, resPath_wide, wcslen(resPath_wide) + 1);
#elif VA_ANDROID
	resDir = "/sdcard/iflytek/res/sr";
#else
	char szExePath[512] = {0};
	if(!getcwd(szExePath, 512)){
		DEBUG_TRACE("getcwd fail!!!");
	}
	string resPath = szExePath;
	resPath += "/SRRes";
	std::cout << resPath << std::endl;
	
	const char * resPath_narrow = resPath.c_str();
	resDir = new char[strlen(resPath_narrow) + 1];
	memcpy(resDir, resPath_narrow, strlen(resPath_narrow) + 1);
	
#endif

	srPcm_CallBaiYaWei = testRes_path + "pcm_src/sr/PhoneBaiYawei.wav";
	srPcm_NavigateIflytek = testRes_path + "pcm_src/sr/NavigateIflytek.wav";
	srPcm_AnyHotel = testRes_path + "pcm_src/sr/AnyHotel.wav";
	srPcm_CallZhangSan = testRes_path + "pcm_src/sr/PhoneZhangsan.wav";

	srPcm_TodayWeather = testRes_path + "pcm_src/sr/TodayWeather.pcm";
	srPcm_NavigateWXD = testRes_path + "pcm_src/sr/NavigateWXD.pcm";

	srPcm_OneShot = testRes_path + "pcm_src/sr/oneshot.pcm";
	srPcm_OSHW = testRes_path + "pcm_src/sr/OSHW.pcm";
	srPcm_SpeechTail = testRes_path + "pcm_src/sr/speechtail.pcm";
	srPcm_SpeechTimeOut = testRes_path + "pcm_src/sr/SpeechTimeOut.pcm";
	srPcm_ResponseTimeOut = testRes_path + "pcm_src/sr/ResponseTimeOut.pcm";
	srPcm_AppendAudioData100ms = testRes_path + "pcm_src/sr/100ms.pcm";
	srPcm_AppendNoiseAudioData = testRes_path + "pcm_src/sr/NoisePcm_MemOverflow.pcm";
	srPcm_SetResponseTimeOut = testRes_path + "pcm_src/sr/SetResponse.pcm";
	srPcm_SetMvwTimeOut = testRes_path + "pcm_src/sr/SetMvwTimeOut.pcm";

	srPcm_SceneAll = testRes_path + "pcm_src/sr/SceneAll.pcm";
	srPcm_ScenePoi = testRes_path + "pcm_src/sr/KDXF.pcm";
	srPcm_SceneContact = testRes_path + "pcm_src/sr/contact.pcm";
	srPcm_SceneSelect = testRes_path + "pcm_src/sr/select.pcm";
	srPcm_SceneConfirm = testRes_path + "pcm_src/sr/cancle.pcm";
	srPcm_SceneAnswerCall = testRes_path + "pcm_src/sr/answer.pcm";

	srPcm_STKS = testRes_path + "pcm_src/sr/stks.pcm";
	srText_STKS = readFile(testRes_path + "pcm_src/sr/stks.txt", false);
	srText_STKS_error = readFile(testRes_path + "pcm_src/sr/stks_error.txt", false);

	dict_500 = readFile(testRes_path + "dict/dict_M-500.txt", false);
	dict_1000 = readFile(testRes_path + "dict/dict_M-1000.txt", false);
	dict_5000 = readFile(testRes_path + "dict/dict_M-1500.txt", false);
	dict_2000 = readFile(testRes_path + "dict/dict_list_2000.txt", false);
}

DefSR::~DefSR() {
	initMsg();
#if VA_VC
	delete[] resDir;
	delete[] resDirW;
#elif VA_ANDROID
	
#else
	delete[] resDir;
	
#endif
}
