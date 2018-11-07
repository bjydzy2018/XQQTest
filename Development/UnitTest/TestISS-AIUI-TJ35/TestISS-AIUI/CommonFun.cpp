#include "CommonFun.h"
#include "DefMVW.h"
#include "DefSR.h"
#include "test_log.h"
#include "TimerForTest.h"
#include "VAFileSystem.h"

#include <locale>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

//#include <boost/filesystem.hpp>
//#include <boost/filesystem/operations.hpp>

using namespace VA;
using std::string;
using std::wstring;
using std::ifstream;
using std::stringstream;
using std::vector;

string LongToString(long num){
	stringstream buffer;
	buffer<<num; 
	string str = buffer.str();
	return str;
}

string DoubleToString(double num){
	stringstream buffer;
	buffer<<num; 
	string str = buffer.str();
	return str;
}

bool startswith(const std::string& str, const std::string& start)
{
    int srclen = str.size();
    int startlen = start.size();
    if(srclen >= startlen)
    {
        string temp = str.substr(0, startlen);
        if(temp == start)
            return true;
    }

    return false;
}

bool endswith(const std::string& str, const std::string& end)
{
    int srclen = str.size();
    int endlen = end.size();
    if(srclen >= endlen)
    {
        string temp = str.substr(srclen - endlen, endlen);
        if(temp == end)
            return true;
    }

    return false;
}

// replace all occurance of t in s to w  
void replace_all(std::string & s, std::string const & t, std::string const & w)  
{  
  string::size_type pos = s.find(t), t_size = t.size(), r_size = w.size();
  while(pos != std::string::npos){ // found
    s.replace(pos, t_size, w);
    pos = s.find(t, pos + r_size );
  }
}

long GetFileSize(string filePath){
	ifstream fin(filePath.c_str(), ios::in | ios::binary);
	if (!fin.is_open()) {
		DEBUG_TRACE_ERROR("The file doesn't exist");
		throw string("File doesn't exist!");
	}

	fin.seekg(0, ios_base::end);
	long file_size = fin.tellg();
	//fin.seekg(0, ios_base::beg);
	fin.close();
	return file_size;
}

int sleep(int sleep_time) {
#if VA_VC
	Sleep(sleep_time);
#elif VA_GNUC
	usleep(sleep_time * 1000);
#endif

	return 0;
}


 /**
     * FindFiles - ���һ��Ŀ¼�µ��ļ��б�
     *
     * @param dirΪ����·��
     * @param filterΪ����ͨ�������"*.*"
     * @param flistΪ���ص��ļ����
     * @param recursiveΪ�Ƿ�ݹ�����
     * @return: �ɹ�����0�����򷵻�-1
	 int FindFiles(const char * dir, const char * filter, std::vector<std::string> & flist, bool recursive = true);
  
  **/
    

//// �ݹ����һ��Ŀ¼������һ���洢��Ŀ¼������Ŀ¼�������ļ�·����vector
//// param dir: Ҫ������Ŀ¼·��
//vector<string> getFilesInDir(const std::string & dir) {
//	using namespace boost::filesystem;
//
//	vector<string> files;
//
//	if (!exists(dir) || !is_directory(dir))
//        return files;
//
//    recursive_directory_iterator end;
//    for (recursive_directory_iterator pos(dir); pos != end; pos++) {
//		files.push_back(pos->path().string());
//    }
//
//	return files;
//}

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

vector<string> readFileByLine(string file_path, bool ifEncodeByPlatform,bool ifKeepBlankLine) {
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
			if(ifKeepBlankLine){
				lines.push_back(string(PrtU8(line.c_str())));
			}else{
				if (line != "")
					lines.push_back(string(PrtU8(line.c_str())));
			}
			
		}
	else
		while (getline(fin, line)) {
			replace_all(line, "\r", "");
			replace_all(line, "\n", "");
			if(ifKeepBlankLine){
				lines.push_back(line);
			}else{
				if (line != "")
					lines.push_back(line);
			}
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
	DEBUG_TRACE_INFOR(("appending audio file: " + audio_path).c_str());

	ISSErrID err = -1;
	int append_size_each_time = 320; // ÿ������Ƶ���ֽ���
	//int append_size_each_time = 3200; // ÿ������Ƶ���ֽ���
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

	long TimeBase = CurrentTime();
	//timer t;
	TimerForTest t;
	if (obj_type == "mvw") {
		DEBUG_TRACE_INFOR("append audio data to mvw");

		DefMVW & defMVW = dynamic_cast<DefMVW &>(def);

		// �ж��Ƿ���Ҫȥwavͷ
		if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
			DEBUG_TRACE_INFOR("delete wav head");
			current_position += 44;
		}

		while (current_position < audio_size) {
			if (audio_size - current_position < append_size_each_time) {
				nNumberOfToWrite = (audio_size - current_position) / 2;
			}else{
				nNumberOfToWrite = append_size_each_time/2;
			}

			err = ISSMVWAppendAudioData(obj, (short *)(buffer + current_position), nNumberOfToWrite, &pNumberOfWritten);
			if( err == 0){
				current_position += nNumberOfToWrite * 2;
			} else {
				DEBUG_TRACE("ISSMVWAppendAudioData Fail, return %d!",err);
				current_position += nNumberOfToWrite * 2;
			}
			//pass_time = t.elapsed() * 1000;
			pass_time = t.elapsed();
			append_size_sum += nNumberOfToWrite*2;
			append_time_sum = append_size_sum / 32;
			if (append_time_sum > pass_time)
				sleep((int)(append_time_sum - pass_time));

			/*if (err != ISS_SUCCESS || (err != ISS_SUCCESS && err != ISS_ERROR_NO_ENOUGH_BUFFER) || defMVW.msgWakeup)
				break;*/
		}

		int i = 0;
		while (!defMVW.msgWakeup && i++ < 250)
			sleep(10);

		if (defMVW.msgWakeup) {
			//defMVW.vw_time = t.elapsed() * 1000;
			defMVW.vw_time = defMVW.vw_current_time - TimeBase;
		}
	}
	else if (obj_type == "sr") {
		DEBUG_TRACE("append audio data to sr");

		DefSR & defSR = dynamic_cast<DefSR &>(def);
		// �ж��Ƿ���Ҫȥwavͷ
		if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
			DEBUG_TRACE_INFOR("delete wav head");
			current_position += 44;
		}

		while (current_position < audio_size) {
			if (audio_size - current_position < append_size_each_time) {
				nNumberOfToWrite = (audio_size - current_position) / 2;
			}else{
				nNumberOfToWrite = append_size_each_time/2;
			}

			//DEBUG_TRACE("Before ISSSRAppendAudioData");
			err = ISSSRAppendAudioData(obj, (short *)(buffer + current_position), nNumberOfToWrite, &pNumberOfWritten);
			//DEBUG_TRACE("After ISSSRAppendAudioData");
			if( err == 0){
				current_position += nNumberOfToWrite * 2;
			} else {
				current_position += nNumberOfToWrite * 2;
				DEBUG_TRACE("ISSSRAppendAudioData Fail, return %d!",err);
			}

			//pass_time = t.elapsed() * 1000;
			pass_time = t.elapsed();
			append_size_sum += nNumberOfToWrite*2;
			append_time_sum = append_size_sum / 32;
			if (append_time_sum > pass_time)
				sleep((int)(append_time_sum - pass_time));

			/*if ((err != ISS_SUCCESS && err != ISS_ERROR_NO_ENOUGH_BUFFER) || defSR.msgResult
				|| defSR.msgSpeechEnd || defSR.msgSpeechTimeOut || defSR.msgResponseTimeout || defSR.msgError)
				break;*/
			if (defSR.msgResult|| defSR.msgSpeechEnd || 
				defSR.msgSpeechTimeOut || defSR.msgResponseTimeout || defSR.msgError)
				break;
		}

		//if (defSR.msgSpeechEnd){
		//	//defSR.speechEnd_time = t.elapsed() * 1000;
		//	defSR.speechEnd_time = t.elapsed();
		//}

		//int i = 0;
		//while (!defSR.msgResult && i++ < 1000 && !defSR.msgError)
		//	sleep(10);

		//if (defSR.msgResult) {
		//	//defSR.sr_response_time = t.elapsed() * 1000;
		//	defSR.sr_response_time = t.elapsed();
		//}
		DEBUG_TRACE("append audio data end");
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

	//timer t;
	TimerForTest t;
	if (obj_type == "mvw") {
		DefMVW & defMVW = dynamic_cast<DefMVW &>(def);

		while (!fin.eof()) {
			buffer = new char[append_size_each_time];
			fin.read(buffer, sizeof(char) * append_size_each_time);

			if (first_append) {
				// �ж��Ƿ���Ҫȥwavͷ
				if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
					err = ISSMVWAppendAudioData(obj, (short *)(buffer + 44), nNumberOfToWrite, &pNumberOfWritten);
				}
				else {
					err = ISSMVWAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);
				}

				first_append = false;
			}
			else {
				err = ISSMVWAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);
			}
			if (err != ISS_SUCCESS)
				DEBUG_TRACE_ERROR("ISSMVWAppendAudioData return %d", err);

			//pass_time = t.elapsed() * 1000;
			pass_time = t.elapsed();
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
			//defMVW.vw_time = t.elapsed() * 1000;
			defMVW.vw_time = t.elapsed();
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
					err = ISSSRAppendAudioData(obj, (short *)(buffer + 44), nNumberOfToWrite, &pNumberOfWritten);
				}
				else {
					err = ISSSRAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);
				}

				first_append = false;
			}
			else {
				err = ISSSRAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);
			}

			//pass_time = t.elapsed() * 1000;
			pass_time = t.elapsed();
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

		if (defSR.msgSpeechEnd){
			//defSR.speechEnd_time = t.elapsed() * 1000;
			defSR.speechEnd_time = t.elapsed();
		}

		int i = 0;
		while (!defSR.msgResult && i++ < 1000 && !defSR.msgError)
			sleep(10);

		if (defSR.msgResult) {
			//defSR.sr_response_time = t.elapsed() * 1000;
			defSR.sr_response_time = t.elapsed();
		}
	}

	fin.close();
}

// AIUI�׼�ʶ��ģ������Ƶ�ӿڣ�ʹ���ļ����������ļ����͸��׼���
void appendAudioToAIUISR(
	void * obj,			// ʶ����Ѿ��
	string pcm_path,	// ��Ƶ·��
	Def & def,			// ʶ����ѵ�Def��
	int blank_length	// ������Ƶ������һ�ξ����ĳ��ȣ���λ��ms
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

	DefSR & defSR = dynamic_cast<DefSR &>(def);

	//timer t;
	TimerForTest t;

	DEBUG_TRACE_INFOR("append audio data to sr");

	while (!fin.eof()) {
		buffer = new char[append_size_each_time];
		fin.read(buffer, sizeof(char) * append_size_each_time);

		if (first_append) {
			// �ж��Ƿ���Ҫȥwavͷ
			if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
				err = ISSSRAppendAudioData(obj, (short *)(buffer + 44), nNumberOfToWrite-22, &pNumberOfWritten);
			}
			else {
				err = ISSSRAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);
			}

			first_append = false;
		}
		else {
			err = ISSSRAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);
		}

		//pass_time = t.elapsed() * 1000;
		pass_time = t.elapsed();
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

			err = ISSSRAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);

			delete[] buffer;
		}
		else {
			char * buffer = new char[blank_length];
			memset(buffer, 0, sizeof(char) * blank_length);

			err = ISSSRAppendAudioData(obj, (short *)buffer, blank_length / 2, &pNumberOfWritten);

			delete[] buffer;
		}

		blank_length -= append_size_each_time;
	}

	fin.close();
}


// AIUI�׼�ʶ��ģ������Ƶ�ӿڣ�ʹ���ļ����������ļ����͸��׼���
//����Ƶ��ʱ��sleep
void appendAudioToAIUISRNoSleep(
	void * obj,			// ʶ����Ѿ��
	string pcm_path,	// ��Ƶ·��
	Def & def,			// ʶ���Def��
	int blank_length	// ������Ƶ������һ�ξ����ĳ��ȣ���λ��ms
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

	DefSR & defSR = dynamic_cast<DefSR &>(def);

	//timer t;
	TimerForTest t;
	while (!fin.eof()) {
		buffer = new char[append_size_each_time];
		fin.read(buffer, sizeof(char) * append_size_each_time);

		if (first_append) {
			// �ж��Ƿ���Ҫȥwavͷ
			if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
				err = ISSSRAppendAudioData(obj, (short *)(buffer + 44), nNumberOfToWrite-22, &pNumberOfWritten);
			}
			else {
				err = ISSSRAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);
			}

			first_append = false;
		}
		else {
			err = ISSSRAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);
		}

		//pass_time = t.elapsed() * 1000;
		pass_time = t.elapsed();
		append_size_sum += append_size_each_time;
		append_time_sum = append_size_sum / 32;
		/*if (append_time_sum > pass_time)
			sleep((int)(append_time_sum - pass_time));*/

		delete[] buffer;
		buffer = NULL;
	}

	// �;���
	blank_length = blank_length * 32; // ��Ƶʱ��ת��Ϊ�ֽ�
	while (blank_length > 0) {
		if (blank_length / append_size_each_time > 0) {
			char * buffer = new char[append_size_each_time];
			memset(buffer, 0, sizeof(char) * append_size_each_time);

			err = ISSSRAppendAudioData(obj, (short *)buffer, nNumberOfToWrite, &pNumberOfWritten);

			delete[] buffer;
		}
		else {
			char * buffer = new char[blank_length];
			memset(buffer, 0, sizeof(char) * blank_length);

			err = ISSSRAppendAudioData(obj, (short *)buffer, blank_length / 2, &pNumberOfWritten);

			delete[] buffer;
		}

		blank_length -= append_size_each_time;
	}

	fin.close();
}

// ��ȡĿ¼��С��ͳ�ư���Ŀ¼�µ�������Ŀ¼���ļ�
// param root_folder: ��ͳ�Ƶĸ�Ŀ¼
// param dir_size:	  Ŀ¼��С
//void getFolderSize(string root_folder, long & dir_size) {
//	long file_size = 0;
//	boost::filesystem::path folderPath(root_folder);
//
//	if (boost::filesystem::exists(folderPath)) {
//		boost::filesystem::directory_iterator end_itr;
//
//		for (boost::filesystem::directory_iterator dirIte(root_folder); dirIte != end_itr; ++dirIte)
//		{
//			boost::filesystem::path filePath(dirIte->path());
//			if (!boost::filesystem::is_directory(dirIte->status()))
//				dir_size = dir_size + boost::filesystem::file_size(filePath);
//			else
//				getFolderSize(filePath.string(), dir_size);
//		}
//	}
//
//}

void getFolderSize(string root_folder, long & dir_size) {
	dir_size = 0;
	vector<string> folder_files;
	FindFiles(root_folder.c_str(),"*.*",folder_files,true);
	for(size_t i = 0; i < folder_files.size(); ++i){
		dir_size += GetFileSize(folder_files[i]);
	}
}

/*  ���ַ�c�ָ��ַ���str 
 */
vector<std::string> splitString(string& str, char c) {
	std::stringstream sstr(str);
	vector<string> strs;
	string tmp;

	while(getline(sstr, tmp, c)) {
		strs.push_back(tmp);
	}
	return strs;
}