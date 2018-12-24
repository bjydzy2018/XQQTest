#include "CommonFun.h"
#include "DefMVW.h"
#include "DefSR.h"
#include "test_log.h"

#include <locale>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <boost/timer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

using namespace VA;
using namespace boost::algorithm;
using boost::timer;
using boost::lexical_cast;
using std::string;
using std::wstring;
using std::ifstream;

int sleep(int sleep_time) {
#if VA_VC
	Sleep(sleep_time);
#elif VA_GNUC
	usleep(sleep_time * 1000);
#endif

	return 0;
}

// �ݹ����һ��Ŀ¼������һ���洢��Ŀ¼������Ŀ¼�������ļ�·����vector
// param dir: Ҫ������Ŀ¼·��
vector<string> getFilesInDir(const std::string & dir) {
	using namespace boost::filesystem;

	vector<string> files;

	if (!exists(dir) || !is_directory(dir))
        return files;

    recursive_directory_iterator end;
    for (recursive_directory_iterator pos(dir); pos != end; pos++) {
		files.push_back(pos->path().string());
    }

	return files;
}

// ��ȡһ���ļ�������һ�������ļ��������ݵ�vector
// param file_path:			 ��ȡ���ļ�·��
// param ifEncodeByPlatform: �Ƿ�ƽ̨���ļ����ݽ���ת�롣
//							 ��Ϊtrue���Ὣ�ļ����ݱ����UTF8תΪGBK��windowsƽ̨����
//							 �ò���ֻ������UTF8������ļ���
vector<string> readFileByLine(string file_path, bool ifEncodeByPlatform) {
	using std::vector;

	ifstream fin(file_path.c_str(), ios::in);
	if (!fin.is_open()) {
		DEBUG_TRACE_ERROR(("file doesn't exist: " + file_path).c_str());
		throw string("file doesn't exist!");
	}

	string line;
	vector<string> lines;

	if (ifEncodeByPlatform)
		while (getline(fin, line)) {
			replace_all(line, "\r", "");
			replace_all(line, "\n", "");

			if (line != "")
				lines.push_back(string(PrtU8(line.c_str())));
		}
	else
		while (getline(fin, line)) {
			replace_all(line, "\r", "");
			replace_all(line, "\n", "");

			if (line != "")
				lines.push_back(line);
		}

	fin.close();
	return lines;
}

// ��ȡһ���ļ��������ļ������ַ���
// param file_path:			 ��ȡ���ļ�·��
// param ifEncodeByPlatform: �Ƿ�ƽ̨���ļ����ݽ���ת�롣
//							 ��Ϊtrue���Ὣ�ļ����ݱ����UTF8תΪGBK��windowsƽ̨����
//							 �ò���ֻ������UTF8������ļ���
string readFile(string file_path, bool ifEncodeByPlatform) {
	ifstream fin(file_path.c_str(), ios::in);
	if (!fin.is_open()) {
		DEBUG_TRACE_ERROR(("file doesn't exist: " + file_path).c_str());
		throw string("file doesn't exist!");
	}

	istreambuf_iterator<char> beg(fin), end;
	string strdata(beg, end);

	fin.close();
	if (ifEncodeByPlatform)
		return string(PrtU8(strdata.c_str()));
	else
		return strdata;
}

// խ�ַ���ת���ַ���
wstring widen(const string& str) {
	wostringstream wstm;
	const ctype<wchar_t>& ctfacet =
		use_facet< ctype<wchar_t> >(wstm.getloc());
	for (size_t i = 0; i<str.size(); ++i)
		wstm << ctfacet.widen(str[i]);
	return wstm.str();
}

// ���ַ���תխ�ַ���
string narrow(const wstring& str) {
	ostringstream stm;
	const ctype<char>& ctfacet =
		use_facet< ctype<char> >(stm.getloc());
	for (size_t i = 0; i<str.size(); ++i)
		stm << ctfacet.narrow(str[i], 0);
	return stm.str();
}

// ͨ������Ƶ�ӿڣ��Ὣÿ����Ƶ�������뵽�ڴ������͸��׼��С�
//
// ���ڻ���ģ�飬����������Ƶ��ȴ����ѽ��������2.5s��
// ���ȵ����ѽ�����Ὣ��ʼ����Ƶ���յ����ѽ����ʱ����¼��DefMVW��vw_time�����С�
// 
// ����ʶ��ģ�飬����������Ƶ��ȴ�ʶ����������10s��
// ���ȵ�ʶ�������Ὣ��ʼ����Ƶ���յ�ʶ������ʱ����¼��DefSR��sr_response_time�����С�
void appendAudio(
	   void * obj,			// ʶ����Ѿ��
	   string obj_type,		// ������ͣ�"sr"��"mvw"
	   string audio_path,	// ��Ƶ·��
	   Def & def			// ʶ����ѵ�Def��
) {
	//DEBUG_TRACE_INFOR(("appending audio file: " + audio_path).c_str());

	ISSErrID err = -1;
	int append_size_each_time = 320; // ÿ������Ƶ���ֽ���
	long current_position = 0; // ��ǰ����Ƶ��λ��
	unsigned int nNumberOfToWrite = (append_size_each_time - current_position) / 2; // ÿ������Ƶ��֡��
	unsigned int pNumberOfWritten = 0; // ʵ���ͽ�ȥ����Ƶ֡��
	char * buffer; // ��Ƶ����

	long append_size_sum = 0; // �������Ƶ�ܴ�С
	long append_time_sum = 0; // �������Ƶ��ʱ��
	double pass_time = 0;	  // ��ʵ������ʱ��

	bool first_append = true;

	ifstream fin(audio_path.c_str(), ios::in | ios::binary);
	if (!fin.is_open()) {
		DEBUG_TRACE_ERROR("audio file doesn't exist");
		throw string("audio file doesn't exist!");
	}

	fin.seekg(0, ios_base::end);
	long audio_size = fin.tellg();

	fin.seekg(0, ios_base::beg);
	buffer = new char[audio_size];
	fin.read(buffer, sizeof(char) * audio_size);
	fin.close();

	timer t;
	if (obj_type == "mvw") {
		DEBUG_TRACE_INFOR("append audio data to mvw");

		DefMVW & defMVW = dynamic_cast<DefMVW &>(def);

		while (current_position < audio_size) {
			if (audio_size - current_position < append_size_each_time) {
				nNumberOfToWrite = (audio_size - current_position) / 2;
			}

			if (first_append) {
				// �ж��Ƿ���Ҫȥwavͷ
				if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
					DEBUG_TRACE_INFOR("delete wav head");
					err = ISSMVWAdapter::ISSMVWAppendAudioData(obj, (short *)(buffer + current_position + 44), nNumberOfToWrite, &pNumberOfWritten);
					current_position += nNumberOfToWrite * 2 + 44;
				}
				else {
					err = ISSMVWAdapter::ISSMVWAppendAudioData(obj, (short *)(buffer + current_position), nNumberOfToWrite, &pNumberOfWritten);
					current_position += nNumberOfToWrite * 2;
				}

				first_append = false;
			}
			else {
				err = ISSMVWAdapter::ISSMVWAppendAudioData(obj, (short *)(buffer + current_position), nNumberOfToWrite, &pNumberOfWritten);
				current_position += nNumberOfToWrite * 2;
			}
			if (err != ISS_SUCCESS)
				DEBUG_TRACE_ERROR("ISSMVWAppendAudioData return %d", err);

			pass_time = t.elapsed() * 1000;
			append_size_sum += append_size_each_time;
			append_time_sum = append_size_sum / 32;
			if (append_time_sum > pass_time)
				sleep((int)(append_time_sum - pass_time));

			if (err != ISS_SUCCESS || (err != ISS_SUCCESS && err != ISS_ERROR_NO_ENOUGH_BUFFER) || defMVW.msgWakeup)
				break;
		}

		int i = 0;
		while (!defMVW.msgWakeup && i++ < 250)
			sleep(10);

		if (defMVW.msgWakeup) {
			defMVW.vw_time = t.elapsed() * 1000;
		}
	}
	else if (obj_type == "sr") {
		DEBUG_TRACE_INFOR("append audio data to sr");

		DefSR & defSR = dynamic_cast<DefSR &>(def);

		while (current_position < audio_size) {
			if (audio_size - current_position < append_size_each_time) {
				nNumberOfToWrite = (audio_size - current_position) / 2;
			}

			if (first_append) {
				// �ж��Ƿ���Ҫȥwavͷ
				if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
					DEBUG_TRACE_INFOR("delete wav head");
					err = ISSSRAdapter::ISSSRAppendAudioData(obj, (short *)(buffer + current_position + 44), nNumberOfToWrite, &pNumberOfWritten);
					current_position += nNumberOfToWrite * 2 + 44;
				}
				else {
					err = ISSSRAdapter::ISSSRAppendAudioData(obj, (short *)(buffer + current_position), nNumberOfToWrite, &pNumberOfWritten);
					current_position += nNumberOfToWrite * 2;
				}

				first_append = false;
			}
			else {
				err = ISSSRAdapter::ISSSRAppendAudioData(obj, (short *)(buffer + current_position), nNumberOfToWrite, &pNumberOfWritten);
				current_position += nNumberOfToWrite * 2;
			}

			pass_time = t.elapsed() * 1000;
			append_size_sum += append_size_each_time;
			append_time_sum = append_size_sum / 32;
			if (append_time_sum > pass_time)
				sleep((int)(append_time_sum - pass_time));

			if ((err != ISS_SUCCESS && err != ISS_ERROR_NO_ENOUGH_BUFFER) || defSR.msgResult
				|| defSR.msgSpeechEnd || defSR.msgSpeechTimeOut || defSR.msgResponseTimeout || defSR.msgError)
				break;
		}

		if (defSR.msgSpeechEnd)
			defSR.speechEnd_time = t.elapsed() * 1000;

		int i = 0;
		while (!defSR.msgResult && i++ < 1000 && !defSR.msgError)
			sleep(10);

		if (defSR.msgResult) {
			defSR.sr_response_time = t.elapsed() * 1000;
		}
	}

	delete[] buffer;
	buffer = NULL;
}

// ͨ���ʹ���Ƶ�ӿڣ�ʹ���ļ����ķ�ʽ�߶�ȡ�ļ������͸��׼���
//
// ���ڻ���ģ�飬����������Ƶ��ȴ����ѽ��������2.5s��
// ���ȵ����ѽ�����Ὣ��ʼ����Ƶ���յ����ѽ����ʱ����¼��DefMVW��vw_time�����С�
// 
// ����ʶ��ģ�飬����������Ƶ��ȴ�ʶ����������10s��
// ���ȵ�ʶ�������Ὣ��ʼ����Ƶ���յ�ʶ������ʱ����¼��DefSR��sr_response_time�����С�
void appendBigAudio(
	void * obj,			// ʶ����Ѿ��
	string obj_type,	// ������ͣ�"sr"��"mvw"
	string pcm_path,	// ��Ƶ·��
	Def & def			// ʶ����ѵ�Def��
) {
	ISSErrID err = -1;
	int append_size_each_time = 320; // ÿ������Ƶ���ֽ���
	unsigned int nNumberOfToWrite = append_size_each_time / 2; // ÿ������Ƶ��֡��
	unsigned int pNumberOfWritten = 0; // ʵ���ͽ�ȥ����Ƶ֡��
	char * buffer; // ��Ƶ����

	long append_size_sum = 0; // �������Ƶ�ܴ�С
	long append_time_sum = 0; // �������Ƶ��ʱ��
	double pass_time = 0;	  // ��ʵ������ʱ��

	bool first_append = true;

	ifstream fin(pcm_path.c_str(), ios::in | ios::binary);
	if (!fin.is_open()) {
		DEBUG_TRACE_ERROR("audio file doesn't exist");
		throw string("audio file doesn't exist!");
	}

	timer t;
	if (obj_type == "mvw") {
		DefMVW & defMVW = dynamic_cast<DefMVW &>(def);

		while (!fin.eof()) {
			buffer = new char[append_size_each_time];
			fin.read(buffer, sizeof(char) * append_size_each_time);

			if (first_append) {
				// �ж��Ƿ���Ҫȥwavͷ
				if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
					err = ISSMVWAdapter::ISSMVWAppendAudioData(obj, (short *)(buffer + 44), nNumberOfToWrite, &pNumberOfWritten);
				}
				else {
					err = ISSMVWAdapter::ISSMVWAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);
				}

				first_append = false;
			}
			else {
				err = ISSMVWAdapter::ISSMVWAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);
			}
			if (err != ISS_SUCCESS)
				DEBUG_TRACE_ERROR("ISSMVWAppendAudioData return %d", err);

			pass_time = t.elapsed() * 1000;
			append_size_sum += append_size_each_time;
			append_time_sum = append_size_sum / 32;
			if (append_time_sum > pass_time)
				sleep((int)(append_time_sum - pass_time));

			delete[] buffer;
			buffer = NULL;

			if ((err != ISS_SUCCESS && err != ISS_ERROR_NO_ENOUGH_BUFFER) || defMVW.msgWakeup)
				break;
		}

		int i = 0;
		while (!defMVW.msgWakeup && i++ < 250)
			sleep(10);

		if (defMVW.msgWakeup) {
			defMVW.vw_time = t.elapsed() * 1000;
		}
	}
	else if (obj_type == "sr") {
		DefSR & defSR = dynamic_cast<DefSR &>(def);

		while (!fin.eof()) {
			buffer = new char[append_size_each_time];
			fin.read(buffer, sizeof(char) * append_size_each_time);

			if (first_append) {
				// �ж��Ƿ���Ҫȥwavͷ
				if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
					err = ISSSRAdapter::ISSSRAppendAudioData(obj, (short *)(buffer + 44), nNumberOfToWrite, &pNumberOfWritten);
				}
				else {
					err = ISSSRAdapter::ISSSRAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);
				}

				first_append = false;
			}
			else {
				err = ISSSRAdapter::ISSSRAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);
			}

			pass_time = t.elapsed() * 1000;
			append_size_sum += append_size_each_time;
			append_time_sum = append_size_sum / 32;
			if (append_time_sum > pass_time)
				sleep((int)(append_time_sum - pass_time));

			delete[] buffer;
			buffer = NULL;

			if ((err != ISS_SUCCESS && err != ISS_ERROR_NO_ENOUGH_BUFFER) || defSR.msgResult
				|| defSR.msgSpeechEnd || defSR.msgSpeechTimeOut || defSR.msgResponseTimeout || defSR.msgError)
				break;
		}

		if (defSR.msgSpeechEnd)
			defSR.speechEnd_time = t.elapsed() * 1000;

		int i = 0;
		while (!defSR.msgResult && i++ < 1000 && !defSR.msgError)
			sleep(10);

		if (defSR.msgResult) {
			defSR.sr_response_time = t.elapsed() * 1000;
		}
	}

	fin.close();
}

// AIUI�׼�ʶ��ģ������Ƶ�ӿڣ�ʹ���ļ����������ļ����͸��׼���
void appendAudioToAIUISR(
	void * obj,			// ʶ����Ѿ��
	string pcm_path,	// ��Ƶ·��
	Def & def,			// ʶ����ѵ�Def��
	int appendSizeEachTime,
	int blank_length	// ������Ƶ������һ�ξ����ĳ��ȣ���λ��ms
) {
	ISSErrID err = -1;
	int append_size_each_time = appendSizeEachTime; // ÿ������Ƶ���ֽ���
	DEBUG_TRACE("each time append audio size is :%d", append_size_each_time);
	unsigned int nNumberOfToWrite = append_size_each_time / 2; // ÿ������Ƶ��֡��
	unsigned int pNumberOfWritten = 0; // ʵ���ͽ�ȥ����Ƶ֡��
	char * buffer; // ��Ƶ����

	long append_size_sum = 0; // �������Ƶ�ܴ�С
	long append_time_sum = 0; // �������Ƶ��ʱ��
	double pass_time = 0;	  // ��ʵ������ʱ��

	bool first_append = true;

	ifstream fin(pcm_path.c_str(), ios::in | ios::binary);
	if (!fin.is_open()) {
		DEBUG_TRACE_ERROR("audio file doesn't exist");
		throw string("audio file doesn't exist!");
	}

	fin.seekg(0, ios_base::end);
	long audio_size = fin.tellg();
	fin.seekg(0, ios_base::beg);

	DefSR & defSR = dynamic_cast<DefSR &>(def);
	DEBUG_TRACE_INFOR("append audio data to sr");
	timer t;
	while (!fin.eof()) {
		if ((audio_size - append_size_sum) < append_size_each_time) {
			nNumberOfToWrite = (audio_size - append_size_sum + 1) / 2;
			buffer = new char[append_size_each_time];
			fin.read(buffer, sizeof(char) * append_size_each_time);
		}
		else {
			buffer = new char[append_size_each_time];
			fin.read(buffer, sizeof(char) * append_size_each_time);
		}

		if (first_append) {
			// �ж��Ƿ���Ҫȥwavͷ
			if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
				err = ISSSRAdapter::ISSSRAppendAudioData(obj, (short *)(buffer + 44), nNumberOfToWrite-22, &pNumberOfWritten);
			}
			else {
				err = ISSSRAdapter::ISSSRAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);
			}

			first_append = false;
		}
		else {
			err = ISSSRAdapter::ISSSRAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);
		}
		
		pass_time = t.elapsed() * 1000;
		append_size_sum += append_size_each_time;
		append_time_sum = append_size_sum / 32;
		if (append_time_sum > pass_time)
			sleep((int)(append_time_sum - pass_time));

		delete[] buffer;
		buffer = NULL;

	}

	// �;���
	blank_length = blank_length * 32; // ��Ƶʱ��ת��Ϊ�ֽ�
	while (blank_length > 0) {
		if (blank_length / append_size_each_time > 0) {
			char * buffer = new char[append_size_each_time];
			memset(buffer, 0, sizeof(char) * append_size_each_time);

			err = ISSSRAdapter::ISSSRAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);

			delete[] buffer;
		}
		else {
			char * buffer = new char[blank_length];
			memset(buffer, 0, sizeof(char) * blank_length);

			err = ISSSRAdapter::ISSSRAppendAudioData(obj, (short *)buffer, blank_length / 2, &pNumberOfWritten);

			delete[] buffer;
		}

		blank_length -= append_size_each_time;
	}

	fin.close();
}

// ��ȡĿ¼��С��ͳ�ư���Ŀ¼�µ�������Ŀ¼���ļ�
// param root_folder: ��ͳ�Ƶĸ�Ŀ¼
// param dir_size:	  Ŀ¼��С
void getFolderSize(string root_folder, long & dir_size) {
	long file_size = 0;
	boost::filesystem::path folderPath(root_folder);

	if (boost::filesystem::exists(folderPath)) {
		boost::filesystem::directory_iterator end_itr;

		for (boost::filesystem::directory_iterator dirIte(root_folder); dirIte != end_itr; ++dirIte)
		{
			boost::filesystem::path filePath(dirIte->path());
			if (!boost::filesystem::is_directory(dirIte->status()))
				dir_size = dir_size + boost::filesystem::file_size(filePath);
			else
				getFolderSize(filePath.string(), dir_size);
		}
	}

}
