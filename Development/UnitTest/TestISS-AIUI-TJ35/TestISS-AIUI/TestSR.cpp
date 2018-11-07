#include "TestSR.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"
#include "Record.h"


using namespace VA;
using std::string;

TestSR::TestSR():err(-1), phISSSR(NULL) {}

#if VA_VC
class Recorder : public CRecord {
public:
	HISSSR	 phISSSR_;

	Recorder(HISSSR * phISSSR) {
		phISSSR_ = *phISSSR;
		RRESULT err = OpenDevice();
		UNREFERENCED_PARAMETER(err);
		_ASSERT(RECORD_ERR_OK == err);
	}
	~Recorder() {
		CloseDevice();
	}
	void OnRecordHandle(char* pData, DWORD dwSize) {
		unsigned int NumWritten = 0;
		ISSErrID err = ISSSRAppendAudioData(phISSSR_, (short *)pData, dwSize >> 1, &NumWritten);
		if (err) {
		}
		else {
			if (err == ISS_SUCCESS && NumWritten != dwSize >> 1) {
				DEBUG_TRACE("ISSSRAppendAudioData Buffer Full.\n");
			}
		}
	}
};

void TestSR::test_sr_online(int lang, string scene, int mode, string szCmd) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	DEBUG_TRACE("ISSSetMachineCode return %d" , err);
	if (err != ISS_SUCCESS)
		return;
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
#endif
	DEBUG_TRACE("ISSSRCreate return %d" , err);
	if (err != ISS_SUCCESS)
		return;

	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	if (!def.msgInitStatus)
		return;

	err = ISSSRStart(phISSSR, scene.c_str(), mode, szCmd.c_str());
	DEBUG_TRACE("ISSSRStart return %d" , err);
	if (err != ISS_SUCCESS)
		return;

	Recorder pRecorder(&phISSSR);
	pRecorder.StartRecord();
	
	while (1)
		sleep(10);
}

void TestSR::test_sr_online(int lang, string scene, int mode, string szCmd, string dict_path, int ifOnlyUploadToCloud) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	DEBUG_TRACE("ISSSetMachineCode return %d" , err);
	if (err != ISS_SUCCESS)
		return;
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
#endif
	DEBUG_TRACE("ISSSRCreate return %d" , err);
	if (err != ISS_SUCCESS)
		return;

	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	if (!def.msgInitStatus)
		return;

	err = ISSSRUpLoadDict(phISSSR, readFile(dict_path, false).c_str(), ifOnlyUploadToCloud);
	DEBUG_TRACE("ISSSRUpLoadDict return %d" , err);

	DEBUG_TRACE("waiting for uploading dict");
	if (!ifOnlyUploadToCloud)
		while (!def.msgUpLoadDictToLocalStatus || !def.msgUpLoadDictToCloudStatus)
			sleep(10);
	else
		while (!def.msgUpLoadDictToLocalStatus && !def.msgUpLoadDictToCloudStatus)
			sleep(10);
	DEBUG_TRACE("upload dict successfully");

	err = ISSSRStart(phISSSR, scene.c_str(), mode, szCmd.c_str());
	DEBUG_TRACE("ISSSRStart return %d" , err);
	if (err != ISS_SUCCESS)
		return;

	Recorder pRecorder(&phISSSR);
	pRecorder.StartRecord();

	while (1)
		sleep(10);
}
#endif

// 识别一条音频，不上传词典
void TestSR::test_sr(string audio_path, int lang, string scene, int mode, string szCmd) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	DEBUG_TRACE("ISSSetMachineCode return %d" , err);
	if (err != ISS_SUCCESS)
		return;
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
#endif
	DEBUG_TRACE("ISSSRCreate return %d" , err);
	if (err != ISS_SUCCESS)
		return;
	
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	if (!def.msgInitStatus)
		return;

	err = ISSSRStart(phISSSR, scene.c_str(), mode, szCmd.c_str());
	DEBUG_TRACE("ISSSRStart return %d" , err);
	if (err != ISS_SUCCESS)
		return;

#ifdef ISS_SR_3
	appendAudio(phISSSR, "sr", audio_path, def);
#else
	appendAudioToAIUISR(phISSSR, audio_path, def, 500);
#endif

#ifdef ISS_SR_3
	err = ISSSREndAudioData(phISSSR);
	DEBUG_TRACE("ISSSREndAudioData return %d" , err);
#endif

	DEBUG_TRACE("waiting for sr result");
	int i = 0;
	while (!def.msgResult && i++ < 850)
		sleep(10);

	if (def.msgResult)
		DEBUG_TRACE_INFOR("sr result: %s", PrtU8(def.sr_result.c_str()));
	else
		DEBUG_TRACE_ERROR("waited 8500ms and got no sr result");

	err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	DEBUG_TRACE("ISSSRDestroy return %d" , err);

	def.initMsg();
}

// 识别一条音频，上传词典
void TestSR::test_sr(string audio_path, int lang, string scene, int mode, string szCmd, string dict_path, int ifOnlyUploadToCloud) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	DEBUG_TRACE("ISSSetMachineCode return %d" , err);
	if (err != ISS_SUCCESS)
		return;
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
#endif
	DEBUG_TRACE("ISSSRCreate return %d" , err);
	if (err != ISS_SUCCESS)
		return;
	
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	if (!def.msgInitStatus)
		return;

	err = ISSSRUpLoadDict(phISSSR, readFile(dict_path, false).c_str(), ifOnlyUploadToCloud);
	DEBUG_TRACE("ISSSRUpLoadDict return %d" , err);
	DEBUG_TRACE("waiting for uploading dict");
	if (!ifOnlyUploadToCloud)
		while (!def.msgUpLoadDictToLocalStatus || !def.msgUpLoadDictToCloudStatus)
			sleep(10);
	else
		while (!def.msgUpLoadDictToLocalStatus && !def.msgUpLoadDictToCloudStatus)
			sleep(10);
	DEBUG_TRACE("upload dict successfully");

	err = ISSSRStart(phISSSR, scene.c_str(), mode, szCmd.c_str());
	DEBUG_TRACE("ISSSRStart return %d" , err);
	if (err != ISS_SUCCESS)
		return;

#ifdef ISS_SR_3
	appendAudio(phISSSR, "sr", audio_path, def);
#else
	appendAudioToAIUISR(phISSSR, audio_path, def, 500);
#endif

#ifdef ISS_SR_3
	err = ISSSREndAudioData(phISSSR);
	DEBUG_TRACE("ISSSREndAudioData return %d" , err);
#endif

	DEBUG_TRACE("waiting for sr result");
	int i = 0;
	while (!def.msgResult && i++ < 850)
		sleep(10);

	if (def.msgResult)
		DEBUG_TRACE_INFOR("sr result: %s", PrtU8(def.sr_result.c_str()));
	else
		DEBUG_TRACE_ERROR("waited 8500ms and got no sr result");

	err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	DEBUG_TRACE("ISSSRDestroy return %d" , err);

	def.initMsg();
}
