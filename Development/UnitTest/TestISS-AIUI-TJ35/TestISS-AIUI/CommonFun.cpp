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
     * FindFiles - 获得一个目录下的文件列表
     *
     * @param dir为绝对路径
     * @param filter为搜索通配符，如"*.*"
     * @param flist为返回的文件结果
     * @param recursive为是否递归搜索
     * @return: 成功返回0，否则返回-1
	 int FindFiles(const char * dir, const char * filter, std::vector<std::string> & flist, bool recursive = true);
  
  **/
    

//// 递归遍历一个目录，返回一个存储该目录及其子目录下所有文件路径的vector
//// param dir: 要遍历的目录路径
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

// 读取一个文件，返回一个包含文件各行内容的vector
// param file_path:			 读取的文件路径
// param ifEncodeByPlatform: 是否按平台对文件内容进行转码。
//							 若为true，会将文件内容编码从UTF8转为GBK（windows平台）。
//							 该参数只适用于UTF8编码的文件。
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

// 读取一个文件，返回文件内容字符串
// param file_path:			 读取的文件路径
// param ifEncodeByPlatform: 是否按平台对文件内容进行转码。
//							 若为true，会将文件内容编码从UTF8转为GBK（windows平台）。
//							 该参数只适用于UTF8编码的文件。
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

// 窄字符串转宽字符串
wstring widen(const string& str) {
	wostringstream wstm;
	const ctype<wchar_t>& ctfacet =
		use_facet< ctype<wchar_t> >(wstm.getloc());
	for (size_t i = 0; i<str.size(); ++i)
		wstm << ctfacet.widen(str[i]);
	return wstm.str();
}

// 宽字符串转窄字符串
string narrow(const wstring& str) {
	ostringstream stm;
	const ctype<char>& ctfacet =
		use_facet< ctype<char> >(stm.getloc());
	for (size_t i = 0; i<str.size(); ++i)
		stm << ctfacet.narrow(str[i], 0);
	return stm.str();
}

// 通用送音频接口，会将每个音频完整读入到内存中再送给套件中。
//
// 对于唤醒模块，会在送完音频后等待唤醒结果，最多等2.5s。
// 若等到唤醒结果，会将开始送音频到收到唤醒结果的时间差记录到DefMVW的vw_time变量中。
// 
// 对于识别模块，会在送完音频后等待识别结果，最多等10s。
// 若等到识别结果，会将开始送音频到收到识别结果的时间差记录到DefSR的sr_response_time变量中。
void appendAudio(
	   void * obj,			// 识别或唤醒句柄
	   string obj_type,		// 句柄类型，"sr"或"mvw"
	   string audio_path,	// 音频路径
	   Def & def			// 识别或唤醒的Def类
) {
	DEBUG_TRACE_INFOR(("appending audio file: " + audio_path).c_str());

	ISSErrID err = -1;
	int append_size_each_time = 320; // 每次送音频的字节数
	//int append_size_each_time = 3200; // 每次送音频的字节数
	long current_position = 0; // 当前送音频的位置
	unsigned int nNumberOfToWrite = (append_size_each_time - current_position) / 2; // 每次送音频的帧数
	unsigned int pNumberOfWritten = 0; // 实际送进去的音频帧数
	char * buffer; // 音频缓存

	long append_size_sum = 0; // 送入的音频总大小
	long append_time_sum = 0; // 送入的音频总时间
	double pass_time = 0;	  // 真实经过的时间

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

		// 判断是否需要去wav头
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
		// 判断是否需要去wav头
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

// 通用送大音频接口，使用文件流的方式边读取文件，边送给套件。
//
// 对于唤醒模块，会在送完音频后等待唤醒结果，最多等2.5s。
// 若等到唤醒结果，会将开始送音频到收到唤醒结果的时间差记录到DefMVW的vw_time变量中。
// 
// 对于识别模块，会在送完音频后等待识别结果，最多等10s。
// 若等到识别结果，会将开始送音频到收到识别结果的时间差记录到DefSR的sr_response_time变量中。
void appendBigAudio(
	void * obj,			// 识别或唤醒句柄
	string obj_type,	// 句柄类型，"sr"或"mvw"
	string pcm_path,	// 音频路径
	Def & def			// 识别或唤醒的Def类
) {
	ISSErrID err = -1;
	int append_size_each_time = 320; // 每次送音频的字节数
	unsigned int nNumberOfToWrite = append_size_each_time / 2; // 每次送音频的帧数
	unsigned int pNumberOfWritten = 0; // 实际送进去的音频帧数
	char * buffer; // 音频缓存

	long append_size_sum = 0; // 送入的音频总大小
	long append_time_sum = 0; // 送入的音频总时间
	double pass_time = 0;	  // 真实经过的时间

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
				// 判断是否需要去wav头
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
				// 判断是否需要去wav头
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

// AIUI套件识别模块送音频接口，使用文件流持续读文件并送给套件。
void appendAudioToAIUISR(
	void * obj,			// 识别或唤醒句柄
	string pcm_path,	// 音频路径
	Def & def,			// 识别或唤醒的Def类
	int blank_length	// 送完音频后，再送一段静音的长度，单位：ms
) {
	ISSErrID err = -1;
	int append_size_each_time = 320; // 每次送音频的字节数
	unsigned int nNumberOfToWrite = append_size_each_time / 2; // 每次送音频的帧数
	unsigned int pNumberOfWritten = 0; // 实际送进去的音频帧数
	char * buffer; // 音频缓存

	long append_size_sum = 0; // 送入的音频总大小
	long append_time_sum = 0; // 送入的音频总时间
	double pass_time = 0;	  // 真实经过的时间

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
			// 判断是否需要去wav头
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

	// 送静音
	blank_length = blank_length * 32; // 音频时常转换为字节
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


// AIUI套件识别模块送音频接口，使用文件流持续读文件并送给套件。
//送音频的时候不sleep
void appendAudioToAIUISRNoSleep(
	void * obj,			// 识别或唤醒句柄
	string pcm_path,	// 音频路径
	Def & def,			// 识别的Def类
	int blank_length	// 送完音频后，再送一段静音的长度，单位：ms
) {
	ISSErrID err = -1;
	int append_size_each_time = 320; // 每次送音频的字节数
	unsigned int nNumberOfToWrite = append_size_each_time / 2; // 每次送音频的帧数
	unsigned int pNumberOfWritten = 0; // 实际送进去的音频帧数
	char * buffer; // 音频缓存

	long append_size_sum = 0; // 送入的音频总大小
	long append_time_sum = 0; // 送入的音频总时间
	double pass_time = 0;	  // 真实经过的时间

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
			// 判断是否需要去wav头
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

	// 送静音
	blank_length = blank_length * 32; // 音频时常转换为字节
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

// 获取目录大小，统计包括目录下的所有子目录和文件
// param root_folder: 待统计的根目录
// param dir_size:	  目录大小
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

/*  按字符c分隔字符串str 
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