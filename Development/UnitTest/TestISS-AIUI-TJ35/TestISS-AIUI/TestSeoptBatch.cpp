
#include "TestSeoptBatch.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

using std::string;
using namespace VA;

//#define AUDIO_CHANNELS 4
#define AUDIO_CHANNEL_4 4
#define AUDIO_CHANNEL_2 2
TestSeoptBatch::TestSeoptBatch() {
	err = -1, 
	phISSSeopt = NULL;
}


std::string TestSeoptBatch::GetAudioOutName(std::string audio_in_path,std::string strLabel){
	std::size_t len = audio_in_path.length();
	std::size_t found_1 = audio_in_path.find_last_of("/\\");
	std::size_t found_2 = audio_in_path.find_last_of(".");
	std::string SubStr1 = audio_in_path.substr(found_1 + 1, found_2 - found_1 - 1);
	std::string SubStr2 = audio_in_path.substr(found_2, len - found_2);
	std::string RetStr = SubStr1 + strLabel + SubStr2;
	return RetStr;
}

void TestSeoptBatch::test(std::string audio_list_path,std::string audio_out_path, bool IfNeedSleep) {

	DEBUG_TRACE("TestSeoptBatch begin");
	DEBUG_TRACE("audio_list_path:%s", audio_list_path.c_str());
	DEBUG_TRACE("audio_out_path:%s", audio_out_path.c_str());
	std::vector<std::string> vecAudioFileName = readFileByLine(testRes_path + audio_list_path, true);
	if (SeoptMode_==0 && vecAudioFileName.size() == AUDIO_CHANNEL_2) {
		DEBUG_TRACE("audio input correct");
	}
	else if (SeoptMode_!=0 && vecAudioFileName.size() == AUDIO_CHANNEL_4) {
		DEBUG_TRACE("audio input correct");
	}
	else{
		DEBUG_TRACE("audio input error");
		return;
	}

	long append_size_sum = 0; // 送入的音频总大小
	long append_time_sum = 0; // 送入的音频总时间
	long pass_time = 0;	  // 真实经过的时间


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

	DEBUG_TRACE(" SeoptMode is %d",SeoptMode_);

	if(SeoptMode_==0){
		
		short*   arrBuf[AUDIO_CHANNEL_2] = { NULL };
		size_t nEnd = 0;
		bool bReadErr = false;
		for (unsigned int i = 0; i < vecAudioFileName.size(); ++i) {
			std::string AudioFile = testRes_path+vecAudioFileName[i];
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
			return ;
		}

		nEnd = nEnd / 2; // 采样点数

		// 创建音频文件
		/*std::ofstream fout1((testRes_path + audio_out_path + audio_out1).c_str(), ios::out | ios::binary);
		if (!fout1.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout1.close();
			return;
		}
		std::ofstream fout2((testRes_path + audio_out_path + audio_out2).c_str(), ios::out | ios::binary);
		if (!fout2.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout2.close();
			return;
		}
		std::ofstream fout3((testRes_path + audio_out_path + audio_out3).c_str(), ios::out | ios::binary);
		if (!fout3.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout3.close();
			return;
		}
		std::ofstream fout4((testRes_path + audio_out_path + audio_out4).c_str(), ios::out | ios::binary);
		if (!fout4.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout4.close();
			return;
		}
		std::ofstream fout34((testRes_path + audio_out_path + audio_out34).c_str(), ios::out | ios::binary);
		if (!fout34.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout34.close();
			return;
		}*/
		
		DEBUG_TRACE("sleep 10s before seopt");
		sleep(10000);
		
		ISSErrID iRet = ISSSeoptCreate(&phISSSeopt, def.resDir);
		DEBUG_TRACE("ISSSeoptCreate ret %d", iRet);

		DEBUG_TRACE(" SetParam is %s",ISS_SEOPT_PARAM_VALUE_MAB_VAD_ONLY_MODE);
		//MAB的onlyvad 模式
		ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAB_VAD_ONLY_MODE);
		ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_BEAM_INDEX, ISS_SEOPT_PARAM_VALUE_BEAM_LEFT);

		size_t nCur = 0;
		if (memcmp(arrBuf[0], "RIFF", AUDIO_CHANNEL_2) == 0) {
			nCur = 22;
		}

		static const int SAMPLES_NUM = ISS_SEOPT_FRAME_SHIFT * AUDIO_CHANNEL_2;
		TimerForTest t; 
		for (; nCur + ISS_SEOPT_FRAME_SHIFT < nEnd; nCur += ISS_SEOPT_FRAME_SHIFT) {

			short bufIn[SAMPLES_NUM] = { 0 };
			for (unsigned int j = 0; j < ISS_SEOPT_FRAME_SHIFT; ++j) {
				for (int k = 0; k < AUDIO_CHANNEL_2; ++k) {
					bufIn[j * AUDIO_CHANNEL_2 + k] = arrBuf[k][nCur + j];
				}
			}

			const void* pBufOut = NULL;
			int nSamplesOut = 0;
			int nChannelsOut = 0;
			iRet = ISSSeoptProcess(phISSSeopt, bufIn, ISS_SEOPT_FRAME_SHIFT, &pBufOut, &nSamplesOut, &nChannelsOut);
			isslog_error_noassert(iRet == 0, ("ISSSeoptProcess ret %d", iRet));
			isslog_error_noassert(nSamplesOut == ISS_SEOPT_FRAME_SHIFT, ("nSamplesOut != %d", ISS_SEOPT_FRAME_SHIFT));

			/*for(unsigned int nSampleIndex = 0; nSampleIndex < nSamplesOut; nSampleIndex++)
			{
				fout1.write(((const char*)pBufOut) + nSampleIndex * 8, 2);
				fout2.write(((const char*)pBufOut) + nSampleIndex * 8 + 2, 2);
				fout3.write(((const char*)pBufOut) + nSampleIndex * 8 + 4, 2);
				fout4.write(((const char*)pBufOut) + nSampleIndex * 8 + 6, 2);
			}*/

			//fout1.write(((const char*)pBufOut) + nSamplesOut * 0, nSamplesOut * 2);
			//fout2.write(((const char*)pBufOut) + nSamplesOut * 2, nSamplesOut * 2);
			//fout3.write(((const char*)pBufOut) + nSamplesOut * 4, nSamplesOut * 2);
			//fout4.write(((const char*)pBufOut) + nSamplesOut * 6, nSamplesOut * 2);
			//fout34.write(((const char*)pBufOut) + nSamplesOut * 4, nSamplesOut * 4);

			if (IfNeedSleep) {
				pass_time = (long)(t.elapsed());
				append_size_sum += ISS_SEOPT_FRAME_SHIFT * 2;
				append_time_sum = append_size_sum / 32;
				if (append_time_sum > pass_time)
					sleep((int)(append_time_sum - pass_time));
			}
		}

		iRet = ISSSeoptDestroy(phISSSeopt);
		DEBUG_TRACE("ISSSeoptDestroy ret %d", iRet);
		phISSSeopt = NULL;

		/*fout1.close();
		fout2.close();
		fout3.close();
		fout4.close();
		fout34.close();*/

		for (int i = 0; i < AUDIO_CHANNEL_2; ++i) {
			if (arrBuf[i] != NULL) {
				delete[] arrBuf[i];
				arrBuf[i] = NULL;
			}
		}

	}


	else if(SeoptMode_==1){
		std::string audio_mab_out1("");
		std::string audio_mab_out2("");

		audio_mab_out1 = "seopt_mab_out_1.pcm";
		audio_mab_out2 = "seopt_mab_out_2.pcm";

		short*   arrBuf[AUDIO_CHANNEL_4] = { NULL };
		size_t nEnd = 0;
		bool bReadErr = false;
		for (unsigned int i = 0; i < vecAudioFileName.size(); ++i) {
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
			return ;
		}

		nEnd = nEnd / 2; // 采样点数

		// 创建音频文件
		
		/*std::ofstream fout1((testRes_path + audio_out_path + audio_mab_out1).c_str(), ios::out | ios::binary);
		if (!fout1.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout1.close();
			return;
		}
		std::ofstream fout2((testRes_path + audio_out_path + audio_mab_out2).c_str(), ios::out | ios::binary);
		if (!fout2.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout2.close();
			return;
		}
		std::ofstream fout3((testRes_path + audio_out_path + audio_out3).c_str(), ios::out | ios::binary);
		if (!fout3.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout3.close();
			return;
		}
		std::ofstream fout4((testRes_path + audio_out_path + audio_out4).c_str(), ios::out | ios::binary);
		if (!fout4.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout4.close();
			return;
		}*/
	
		DEBUG_TRACE("sleep 10s before seopt");
		sleep(10000);

		ISSErrID iRet = ISSSeoptCreate(&phISSSeopt, def.resDir);
		DEBUG_TRACE("ISSSeoptCreate ret %d", iRet);

		DEBUG_TRACE(" SetParam is %s",ISS_SEOPT_PARAM_VALUE_MAB_MODE);
		//MAB模式
		ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAB_MODE);
		ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_BEAM_INDEX, ISS_SEOPT_PARAM_VALUE_BEAM_LEFT);

		size_t nCur = 0;
		if (memcmp(arrBuf[0], "RIFF", AUDIO_CHANNEL_4) == 0) {
			nCur = 22;
		}

		static const int SAMPLES_NUM = ISS_SEOPT_FRAME_SHIFT * AUDIO_CHANNEL_4;
		TimerForTest t; 
		for (; nCur + ISS_SEOPT_FRAME_SHIFT < nEnd; nCur += ISS_SEOPT_FRAME_SHIFT) {

			short bufIn[SAMPLES_NUM] = { 0 };
			for (unsigned int j = 0; j < ISS_SEOPT_FRAME_SHIFT; ++j) {
				for (int k = 0; k < AUDIO_CHANNEL_4; ++k) {
					bufIn[j * AUDIO_CHANNEL_4 + k] = arrBuf[k][nCur + j];
				}
			}

			const void* pBufOut = NULL;
			int nSamplesOut = 0;
			int nChannelsOut = 0;
			iRet = ISSSeoptProcess(phISSSeopt, bufIn, ISS_SEOPT_FRAME_SHIFT, &pBufOut, &nSamplesOut, &nChannelsOut);
			isslog_error_noassert(iRet == 0, ("ISSSeoptProcess ret %d", iRet));
			isslog_error_noassert(nSamplesOut == ISS_SEOPT_FRAME_SHIFT, ("nSamplesOut != %d", ISS_SEOPT_FRAME_SHIFT));

			/*for(unsigned int nSampleIndex = 0; nSampleIndex < nSamplesOut; nSampleIndex++)
			{
				fout1.write(((const char*)pBufOut) + nSampleIndex * 8, 2);
				fout2.write(((const char*)pBufOut) + nSampleIndex * 8 + 2, 2);
				fout3.write(((const char*)pBufOut) + nSampleIndex * 8 + 4, 2);
				fout4.write(((const char*)pBufOut) + nSampleIndex * 8 + 6, 2);
			}*/
			//fout1.write(((const char*)pBufOut) + nSamplesOut * 0, nSamplesOut * 2);
			//fout2.write(((const char*)pBufOut) + nSamplesOut * 2, nSamplesOut * 2);
			//fout3.write(((const char*)pBufOut) + nSamplesOut * 4, nSamplesOut * 2);
			//fout4.write(((const char*)pBufOut) + nSamplesOut * 6, nSamplesOut * 2);

			if (IfNeedSleep) {
				pass_time = (long)(t.elapsed());
				append_size_sum += ISS_SEOPT_FRAME_SHIFT * 2;
				append_time_sum = append_size_sum / 32;
				if (append_time_sum > pass_time)
					sleep((int)(append_time_sum - pass_time));
			}
		}


		/*fout1.close();
		fout2.close();
		fout3.close();
		fout4.close();*/

		iRet = ISSSeoptDestroy(phISSSeopt);
		DEBUG_TRACE("ISSSeoptDestroy ret %d", iRet);
		phISSSeopt = NULL;

		for (int i = 0; i < AUDIO_CHANNEL_4; ++i) {
			if (arrBuf[i] != NULL) {
				delete[] arrBuf[i];
				arrBuf[i] = NULL;
			}
		}

	}


	else{
		short*   arrBuf[AUDIO_CHANNEL_4] = { NULL };
		size_t nEnd = 0;
		bool bReadErr = false;
		for (unsigned int i = 0; i < vecAudioFileName.size(); ++i) {
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
			return ;
		}

		nEnd = nEnd / 2; // 采样点数

		// 创建音频文件
		/*std::ofstream fout1((testRes_path + audio_out_path + audio_out1).c_str(), ios::out | ios::binary);
		if (!fout1.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout1.close();
			return;
		}
		std::ofstream fout2((testRes_path + audio_out_path + audio_out2).c_str(), ios::out | ios::binary);
		if (!fout2.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout2.close();
			return;
		}
		std::ofstream fout3((testRes_path + audio_out_path + audio_out3).c_str(), ios::out | ios::binary);
		if (!fout3.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout3.close();
			return;
		}
		std::ofstream fout4((testRes_path + audio_out_path + audio_out4).c_str(), ios::out | ios::binary);
		if (!fout4.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed");
			fout4.close();
			return;
		}*/

		DEBUG_TRACE("sleep 10s before seopt");
		sleep(10000);
		ISSErrID iRet = ISSSeoptCreate(&phISSSeopt, def.resDir);
		DEBUG_TRACE("ISSSeoptCreate ret %d", iRet);

		if(SeoptMode_==2){
			DEBUG_TRACE(" SetParam is %s",ISS_SEOPT_PARAM_VALUE_MAE_MODE);
			//MAE模式
			ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAE_MODE);
			
		}

		if(SeoptMode_==3){
			DEBUG_TRACE(" SetParam is %s",ISS_SEOPT_PARAM_VALUE_MAB_AND_MAE_MODE);
			//MAB和MAE模式
			ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAB_AND_MAE_MODE);
		
		}

		size_t nCur = 0;
		if (memcmp(arrBuf[0], "RIFF", AUDIO_CHANNEL_4) == 0) {
			nCur = 22;
		}

		static const int SAMPLES_NUM = ISS_SEOPT_FRAME_SHIFT * AUDIO_CHANNEL_4;
		TimerForTest t; 
		for (; nCur + ISS_SEOPT_FRAME_SHIFT < nEnd; nCur += ISS_SEOPT_FRAME_SHIFT) {

			short bufIn[SAMPLES_NUM] = { 0 };
			for (unsigned int j = 0; j < ISS_SEOPT_FRAME_SHIFT; ++j) {
				for (int k = 0; k < AUDIO_CHANNEL_4; ++k) {
					bufIn[j * AUDIO_CHANNEL_4 + k] = arrBuf[k][nCur + j];
				}
			}

			const void* pBufOut = NULL;
			int nSamplesOut = 0;
			int nChannelsOut = 0;
			iRet = ISSSeoptProcess(phISSSeopt, bufIn, ISS_SEOPT_FRAME_SHIFT, &pBufOut, &nSamplesOut, &nChannelsOut);
			isslog_error_noassert(iRet == 0, ("ISSSeoptProcess ret %d", iRet));
			isslog_error_noassert(nSamplesOut == ISS_SEOPT_FRAME_SHIFT, ("nSamplesOut != %d", ISS_SEOPT_FRAME_SHIFT));


			/*for(unsigned int nSampleIndex = 0; nSampleIndex < nSamplesOut; nSampleIndex++)
			{
				fout1.write(((const char*)pBufOut) + nSampleIndex * 8, 2);
				fout2.write(((const char*)pBufOut) + nSampleIndex * 8 + 2, 2);
				fout3.write(((const char*)pBufOut) + nSampleIndex * 8 + 4, 2);
				fout4.write(((const char*)pBufOut) + nSampleIndex * 8 + 6, 2);
			}*/

			/*fout1.write(((const char*)pBufOut) + nSamplesOut * 0, nSamplesOut * 2);
			fout2.write(((const char*)pBufOut) + nSamplesOut * 2, nSamplesOut * 2);
			fout3.write(((const char*)pBufOut) + nSamplesOut * 4, nSamplesOut * 2);
			fout4.write(((const char*)pBufOut) + nSamplesOut * 6, nSamplesOut * 2);*/

			if (IfNeedSleep) {
				pass_time = (long)(t.elapsed());
				append_size_sum += ISS_SEOPT_FRAME_SHIFT * 2;
				append_time_sum = append_size_sum / 32;
				if (append_time_sum > pass_time)
					sleep((int)(append_time_sum - pass_time));
			}
		}

		iRet = ISSSeoptDestroy(phISSSeopt);
		DEBUG_TRACE("ISSSeoptDestroy ret %d", iRet);
		phISSSeopt = NULL;

		/*fout1.close();
		fout2.close();
		fout3.close();
		fout4.close();*/

		for (int i = 0; i < AUDIO_CHANNEL_4; ++i) {
			if (arrBuf[i] != NULL) {
				delete[] arrBuf[i];
				arrBuf[i] = NULL;
			}
		}

	}
	

	DEBUG_TRACE("TestSeoptBatch end");
	return ;
}
