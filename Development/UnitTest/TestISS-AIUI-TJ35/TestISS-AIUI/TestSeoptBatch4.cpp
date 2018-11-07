#include "TestSeoptBatch4.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

using std::string;
using namespace VA;

// 目前只支持输入6路音频
#define AUDIO_CHANNEL_6 6

TestSeoptBatch4::TestSeoptBatch4() {
    err = -1, 
    phISSSeopt = NULL;
}

std::string TestSeoptBatch4::GetAudioOutName(std::string audio_in_path,std::string strLabel){
    std::size_t len = audio_in_path.length();
    std::size_t found_1 = audio_in_path.find_last_of("/\\");
    std::size_t found_2 = audio_in_path.find_last_of(".");
    std::string SubStr1 = audio_in_path.substr(found_1 + 1, found_2 - found_1 - 1);
    std::string SubStr2 = audio_in_path.substr(found_2, len - found_2);
    std::string RetStr = SubStr1 + strLabel + SubStr2;
    return RetStr;
}

void TestSeoptBatch4::test(std::string audio_list_path,std::string audio_out_path, bool IfNeedSleep, bool bIsSaveAudioOut) {
    DEBUG_TRACE("TestSeoptBatch4 begin");
    DEBUG_TRACE("audio_list_path:%s", audio_list_path.c_str());
    DEBUG_TRACE("audio_out_path:%s", audio_out_path.c_str());
    std::vector<std::string> vecAudioFileName = readFileByLine(testRes_path + audio_list_path, true);
    if (vecAudioFileName.size() == AUDIO_CHANNEL_6) {   // 校验输入音频是否为4路
        DEBUG_TRACE("audio input correct");
    }
    else{
        DEBUG_TRACE("audio input error");
        return;
    }

    bool is_save_out_audio = false;

    long append_size_sum = 0; // 送入的音频总大小
    long append_time_sum = 0; // 送入的音频总时间
    long pass_time = 0;	  // 真实经过的时间


    std::string audio_out1("");
    std::string audio_out2("");
    std::string audio_out3("");
    std::string audio_out4("");

    audio_out1 = "seopt_out_1.pcm";
    audio_out2 = "seopt_out_2.pcm";
    audio_out3 = "seopt_out_3.pcm";
    audio_out4 = "seopt_out_4.pcm";

	short*   arrBuf[AUDIO_CHANNEL_6] = { NULL };
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

    if (audio_out_path.empty())
    {
        is_save_out_audio = false;
    }

    // 创建音频文件
    std::ofstream fout1;
    std::ofstream fout2;
    std::ofstream fout3;
    std::ofstream fout4;

    if (bIsSaveAudioOut) {
        fout1.open((testRes_path + audio_out_path + audio_out1).c_str(), ios::out | ios::binary);
        if (!fout1.is_open()) {
            DEBUG_TRACE_ERROR("audio file open failed");
            fout1.close();
            return;
        }
        fout2.open((testRes_path + audio_out_path + audio_out2).c_str(), ios::out | ios::binary);
        if (!fout2.is_open()) {
            DEBUG_TRACE_ERROR("audio file open failed");
            fout2.close();
            return;
        }
        fout3.open((testRes_path + audio_out_path + audio_out3).c_str(), ios::out | ios::binary);
        if (!fout3.is_open()) {
            DEBUG_TRACE_ERROR("audio file open failed");
            fout3.close();
            return;
        }
        fout4.open((testRes_path + audio_out_path + audio_out4).c_str(), ios::out | ios::binary);
        if (!fout4.is_open()) {
            DEBUG_TRACE_ERROR("audio file open failed");
            fout4.close();
            return;
        }
    }

    

	DEBUG_TRACE("sleep 10s before seopt");
	sleep(10000);

	ISSErrID iRet = ISSSeoptCreate(&phISSSeopt, def.resDir);
	DEBUG_TRACE("ISSSeoptCreate ret %d", iRet);

	size_t nCur = 0;
	if (memcmp(arrBuf[0], "RIFF", 4) == 0) {
		nCur = 22;
	}

	static const int SAMPLES_NUM = ISS_SEOPT_FRAME_SHIFT * AUDIO_CHANNEL_6;
	TimerForTest t; 
	for (; nCur + ISS_SEOPT_FRAME_SHIFT < nEnd; nCur += ISS_SEOPT_FRAME_SHIFT) {

		short bufIn[SAMPLES_NUM] = { 0 };
		for (unsigned int j = 0; j < ISS_SEOPT_FRAME_SHIFT; ++j) {
			for (int k = 0; k < AUDIO_CHANNEL_6; ++k) {
				bufIn[j * AUDIO_CHANNEL_6 + k] = arrBuf[k][nCur + j];
			}
		}

		const void* pBufOut = NULL;
		int nSamplesOut = 0;
		int nChannelsOut = 0;
		iRet = ISSSeoptProcess(phISSSeopt, bufIn, ISS_SEOPT_FRAME_SHIFT, &pBufOut, &nSamplesOut, &nChannelsOut);
		isslog_error_noassert(iRet == 0, ("ISSSeoptProcess ret %d", iRet));
		isslog_error_noassert(nSamplesOut == ISS_SEOPT_FRAME_SHIFT, ("nSamplesOut != %d", ISS_SEOPT_FRAME_SHIFT));

		for(unsigned int nSampleIndex = 0; bIsSaveAudioOut && (nSampleIndex < nSamplesOut); nSampleIndex++)
		{
			fout1.write(((const char*)pBufOut) + nSampleIndex * 8, 2);
			fout2.write(((const char*)pBufOut) + nSampleIndex * 8 + 2, 2);
			fout3.write(((const char*)pBufOut) + nSampleIndex * 8 + 4, 2);
			fout4.write(((const char*)pBufOut) + nSampleIndex * 8 + 6, 2);
		}

		if (IfNeedSleep) {
			pass_time = (long)(t.elapsed());
			append_size_sum += ISS_SEOPT_FRAME_SHIFT * 2;
			append_time_sum = append_size_sum / 32;
			if (append_time_sum > pass_time)
				sleep((int)(append_time_sum - pass_time));
		}
	}

    if (bIsSaveAudioOut) {
        fout1.close();
        fout2.close();
        fout3.close();
        fout4.close();
    }

	iRet = ISSSeoptDestroy(phISSSeopt);
	DEBUG_TRACE("ISSSeoptDestroy ret %d", iRet);
	phISSSeopt = NULL;

	for (int i = 0; i < AUDIO_CHANNEL_6; ++i) {
		if (arrBuf[i] != NULL) {
			delete[] arrBuf[i];
			arrBuf[i] = NULL;
		}
	}




    DEBUG_TRACE("TestSeoptBatch4 end");
    return ;
}