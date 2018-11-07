#include "TestSeoptSRBatch.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"
#include "rapidjson/document.h"

#include <iostream>
#include <fstream>


#define AUDIO_CHANNEL_4 4
#define AUDIO_CHANNEL_2 2

using namespace VA;
using namespace rapidjson;

#define DEBUG_MODE	(0)

std::string TestSeoptSRBatch::GetAudioOutName(std::string audio_in_path,std::string strLabel){
	std::size_t len = audio_in_path.length();
	std::size_t found_1 = audio_in_path.find_last_of("/\\");
	std::size_t found_2 = audio_in_path.find_last_of(".");
	std::string SubStr1 = audio_in_path.substr(found_1 + 1, found_2 - found_1 - 1);
	std::string SubStr2 = audio_in_path.substr(found_2, len - found_2);
	std::string RetStr = SubStr1 + strLabel + SubStr2;
	return RetStr;
}

TestSeoptSRBatch::TestSeoptSRBatch() :err(-1), phISSSR(NULL) {
	DEBUG_TRACE("TestSeoptSRBatch::test enter");
}

TestSeoptSRBatch::~TestSeoptSRBatch() {
	err = ISSSeoptDestroy(phISSSeopt);
	DEBUG_TRACE("ISSSeoptDestroy ret %d", err);
	phISSSeopt = NULL;

    err = ISSSRDestroy(phISSSR);
    phISSSR = NULL;
    DEBUG_TRACE("ISSSRDestroy return %d",err);

}

//void TestSeoptSRBatch::test(int lang, int mode, string audio_list_path, string test_type,
//    string dict_path, int ifOnlyUploadToCloud) {
void TestSeoptSRBatch::test(string audio_list_path, int lang, int nSrMode, int nSeoptMode) {


    DEBUG_TRACE("TestSeoptBatch begin");
    DEBUG_TRACE("audio_list_path:%s", audio_list_path.c_str());

	short bufToSr[256*2];

	// 读取音频路径
	std::vector<std::string> vecAudioPaths = readFileByLine(testRes_path + audio_list_path, true);
	
    // set Machine Code
    if (machineCode_ != "") {
        DEBUG_TRACE("MachineCode:%s", machineCode_.c_str());
        err = ISSSetMachineCode(machineCode_.c_str());
        DEBUG_TRACE(("ISSSetMachineCode return " + LongToString(err)).c_str());
    }
    else {
        DEBUG_TRACE("MachineCode:%s", srDef.machineCode.c_str());
        err = ISSSetMachineCode(srDef.machineCode.c_str());
        DEBUG_TRACE(("ISSSetMachineCode return " + LongToString(err)).c_str());
    }


	// Seopt Create
	ISSErrID iRet = ISSSeoptCreate(&phISSSeopt, seoptDef.resDir);
	DEBUG_TRACE("ISSSeoptCreate ret %d", iRet);

	string lang_str("");
	string mode_str("");

	if (lang == 0)
		lang_str = "chinese";
	else if (lang == 1)
		lang_str = "english";
	else if (lang == 2)
		lang_str = "cantonese";

	if (nSrMode == 0)
		mode_str = "cloud";
	else if (nSrMode == 1)
		mode_str = "local";
	else if (nSrMode == 2)
		mode_str = "mix";

	ofstream fout((testRes_path + "seoptsr/batch/sr" +"_" + lang_str + "_" + mode_str + ".csv").c_str());
	if (!fout.is_open()) {
		DEBUG_TRACE_ERROR("create result file failed");
		return;
	}
	fout << "ISR_Result,SR_full_result\n";


	// ISSSRCreate
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(lang), srDef.resDir, DefSR::onMsgProc, NULL);
	DEBUG_TRACE("ISSSRCreate return %d",err);
	if (err != ISS_SUCCESS)
		return;

	int count = 0;
	while (!srDef.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	if (!srDef.msgInitStatus)
		return;
		
	int nAppendToSeoptChannel = 4;		
	int nAppendToSrChannel = 2;
	// 设置窄波束工作模式、唤醒方向
	switch(nSeoptMode) {
		case 0:		// ISS_SEOPT_PARAM_VALUE_MAB_VAD_ONLY_MODE
			ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAB_VAD_ONLY_MODE);
			ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_BEAM_INDEX, ISS_SEOPT_PARAM_VALUE_BEAM_LEFT);
			//ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_BEAM_INDEX, ISS_SEOPT_PARAM_VALUE_BEAM_RIGTH);
			ISSSRSetParam(phISSSR, ISS_SR_PARAM_SEOPT_MODE, ISS_SR_PARAM_VALUE_ON);
			nAppendToSeoptChannel = 2;	
			nAppendToSrChannel = 2;
			break;
		case 1:		// ISS_SEOPT_PARAM_VALUE_MAB_MODE
			ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAB_MODE);
			ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_BEAM_INDEX, ISS_SEOPT_PARAM_VALUE_BEAM_RIGTH);
			ISSSRSetParam(phISSSR, ISS_SR_PARAM_SEOPT_MODE, ISS_SR_PARAM_VALUE_ON);
			nAppendToSeoptChannel = 4;	
			nAppendToSrChannel = 2;
			break;
		case 2:		// ISS_SEOPT_PARAM_VALUE_MAE_MODE
			ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAE_MODE);
			ISSSRSetParam(phISSSR, ISS_SR_PARAM_SEOPT_MODE, ISS_SR_PARAM_VALUE_OFF);
			nAppendToSeoptChannel = 4;	
			nAppendToSrChannel = 1;
			break;
		case 3:		// ISS_SEOPT_PARAM_VALUE_MAB_AND_MAE_MODE
			ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAB_AND_MAE_MODE);
			ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_BEAM_INDEX, ISS_SEOPT_PARAM_VALUE_BEAM_LEFT);
			ISSSRSetParam(phISSSR, ISS_SR_PARAM_SEOPT_MODE, ISS_SR_PARAM_VALUE_OFF);
			nAppendToSeoptChannel = 4;	
			nAppendToSrChannel = 1;
			break;
		default:
			DEBUG_TRACE_ERROR("nSeoptMode [%d] error!", nSeoptMode);
			return;
	}

	for(size_t n = 0; n < vecAudioPaths.size(); ++n) {
		int append_size_sum = 0;
		int append_time_sum = 0;
	
		DEBUG_TRACE("before ISSSRStart");
		err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, nSrMode, "");
		DEBUG_TRACE("ISSSRStart return %d", err);

		short*   arrBuf[AUDIO_CHANNEL_4] = { NULL };
		size_t nEnd = 0;
		bool bReadErr = false;

		std::string strAudioPaths = vecAudioPaths[n];	
		std::vector<std::string> vecAudioFileName = splitString(strAudioPaths, ',');
		for(size_t i = 0; i < vecAudioFileName.size(); ++i) {
			std::string AudioFile = vecAudioFileName[i];
			VAFile fIn(AudioFile);
			if (!fIn.IsOpen()) {
				DEBUG_TRACE_ERROR("open %s failed", vecAudioFileName[i].c_str());
				bReadErr = true;
				break;
			}
			size_t fileLength = fIn.Length();
			char* bufAudio = new char[fileLength];
			arrBuf[i] = (short*)bufAudio;
			memset(bufAudio, 0, fileLength);
			fIn.ReadInto(bufAudio, fileLength);
			if (nEnd == 0) {
				nEnd = fileLength;
			}
			else {
				nEnd = (std::min)(nEnd, fileLength);
			}
		}

		if (bReadErr) {
			for (int i = 0; i < vecAudioFileName.size(); ++i) {
				if (arrBuf[i] != NULL) {
					delete[] arrBuf[i];
					arrBuf[i] = NULL;
				}
			}
			continue;	// 继续下一条
		}
		nEnd = nEnd / 2; // 采样点数

		size_t nCur = 0;
		if (memcmp(arrBuf[0], "RIFF", AUDIO_CHANNEL_4) == 0) {
			nCur = 22;
		}

/*	此处若走到异常分支退出程序会有内存泄漏
 *	DEBUG_MODE 仅可在调试程序时打开
 */	
#if DEBUG_MODE	
		std::string audio_out1("");
		std::string audio_out2("");
		std::string audio_out3("");
		std::string audio_out4("");
		std::string audio_out34("");

		audio_out1 = "seopt_out_1.pcm";
		audio_out2 = "seopt_out_2.pcm";
		audio_out3 = "seopt_out_3.pcm";
		audio_out4 = "seopt_out_4.pcm";
		audio_out34 = "seopt_out_34.pcm";
		
		// 创建音频文件
		std::ofstream fout1((testRes_path + audio_out1).c_str(), ios::out | ios::binary);
		if (!fout1.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout1.close();
			return;
		}
		std::ofstream fout2((testRes_path + audio_out2).c_str(), ios::out | ios::binary);
		if (!fout2.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout2.close();
			return;
		}
		std::ofstream fout3((testRes_path + audio_out3).c_str(), ios::out | ios::binary);
		if (!fout3.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout3.close();
			return;
		}
		std::ofstream fout4((testRes_path + audio_out4).c_str(), ios::out | ios::binary);
		if (!fout4.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout4.close();
			return;
		}
		std::ofstream fout34((testRes_path + audio_out34).c_str(), ios::out | ios::binary);
		if (!fout34.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout34.close();
			return;
		}
#endif

		static const int SAMPLES_NUM = ISS_SEOPT_FRAME_SHIFT * AUDIO_CHANNEL_4;
		TimerForTest t; 
		for (; nCur + ISS_SEOPT_FRAME_SHIFT < nEnd; nCur += ISS_SEOPT_FRAME_SHIFT) {

			short bufIn[SAMPLES_NUM] = { 0 };
			for (unsigned int j = 0; j < ISS_SEOPT_FRAME_SHIFT; ++j) {
				for (int k = 0; k < nAppendToSeoptChannel; ++k) {
					bufIn[j * nAppendToSeoptChannel + k] = arrBuf[k][nCur + j];
				}
			}

			const void* pBufOut = NULL;
			int nSamplesOut = 0;
			int nChannelsOut = 0;
			iRet = ISSSeoptProcess(phISSSeopt, bufIn, ISS_SEOPT_FRAME_SHIFT, &pBufOut, &nSamplesOut, &nChannelsOut);
			isslog_error_noassert(iRet == 0, ("ISSSeoptProcess ret %d", iRet));
			isslog_error_noassert(nSamplesOut == ISS_SEOPT_FRAME_SHIFT, ("nSamplesOut != %d", ISS_SEOPT_FRAME_SHIFT));
#if DEBUG_MODE			
			for(unsigned int nSampleIndex = 0; nSampleIndex < nSamplesOut; nSampleIndex++)
			{
				fout1.write(((const char*)pBufOut) + nSampleIndex * 8, 2);
				fout2.write(((const char*)pBufOut) + nSampleIndex * 8 + 2, 2);
				fout3.write(((const char*)pBufOut) + nSampleIndex * 8 + 4, 2);
				fout4.write(((const char*)pBufOut) + nSampleIndex * 8 + 6, 2);
				fout34.write(((const char*)pBufOut) + nSampleIndex * 8 + 4, 4);
			}
#endif

			unsigned int nNumOfWrite = 0;

			for (unsigned int nSampleIndex = 0; nSampleIndex < nSamplesOut; nSampleIndex++)
			{
				if (nAppendToSrChannel == 1)
				{
					bufToSr[nSampleIndex] = ((short *)pBufOut)[nSampleIndex * 4 + 2];
				}
				else if (nAppendToSrChannel == 2)
				{
					bufToSr[nSampleIndex * 2] = ((short *)pBufOut)[nSampleIndex * 4 + 2];
					bufToSr[nSampleIndex * 2 + 1] = ((short *)pBufOut)[nSampleIndex * 4 + 3];
				}
				else
				{
					DEBUG_TRACE_ERROR("nAppendToSrChannel [%d] Error!", nAppendToSrChannel);
					return;
				}
			}

			ISSSRAppendAudioData(phISSSR, bufToSr, nSamplesOut * nAppendToSrChannel, &nNumOfWrite);

			long pass_time = (long)(t.elapsed());
			append_size_sum += ISS_SEOPT_FRAME_SHIFT;
			append_time_sum = append_size_sum / 16;
			if (append_time_sum > pass_time)
				sleep((int)(append_time_sum - pass_time));
		}
		
		// 送静音
		int blank_length = 800 * 32 * nAppendToSrChannel; // 音频时常转换为字节
		int append_size_each_time = 256 * 2 * nAppendToSrChannel;
		unsigned int pNumberOfWritten = 0;
		while (blank_length > 0) {
			if (blank_length / append_size_each_time > 0) {
				char * buffer = new char[append_size_each_time];
				memset(buffer, 0, sizeof(char) * append_size_each_time);
				err = ISSSRAppendAudioData(phISSSR, (short *)buffer, 256 * nAppendToSrChannel, &pNumberOfWritten);
				delete[] buffer;
			}
			else {
				char * buffer = new char[blank_length];
				memset(buffer, 0, sizeof(char) * blank_length);
				err = ISSSRAppendAudioData(phISSSR, (short *)buffer, blank_length / 2, &pNumberOfWritten);
				delete[] buffer;
			}
			blank_length -= append_size_each_time;
		}
		//sleep(3000);
		
		count = 0;
		while (!srDef.msgResult &&!srDef.msgError && 
			!srDef.msgSpeechTimeOut && !srDef.msgResponseTimeout && count++ < 1000)
			sleep(10);

		string SR_result("");
		string SR_full_result("");

		if (srDef.msgResult) {
			SR_full_result = string(VA::U8StrToGBKStr(srDef.sr_result.c_str()));

			Document doc;
			doc.Parse(SR_full_result.c_str());
			if (doc.HasMember("intent")) {
				Value & intent = doc["intent"];

				if (intent.HasMember("text")) {
					SR_result= intent["text"].GetString();
				}
			}

			replace_all(SR_full_result, "\n", "");
			replace_all(SR_full_result, "\"", "\"\"");

			replace_all(SR_result, "\n", "");
			replace_all(SR_result, "\"", "\"\"");
		}
		fout << "\"" << SR_result << "\",";
		fout << "\"" << SR_full_result << "\"\n";
		fout.flush();

		srDef.msgResult = 0;
		srDef.initMsg();

		for (int i = 0; i < vecAudioFileName.size(); ++i) {
			if (arrBuf[i] != NULL) {
				delete[] arrBuf[i];
				arrBuf[i] = NULL;
			}
		}
		
	}
}