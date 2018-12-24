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

// 递归遍历一个目录，返回一个存储该目录及其子目录下所有文件路径的vector
// param dir: 要遍历的目录路径
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
	//DEBUG_TRACE_INFOR(("appending audio file: " + audio_path).c_str());

	ISSErrID err = -1;
	int append_size_each_time = 320; // 每次送音频的字节数
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

	timer t;
	if (obj_type == "mvw") {
		DEBUG_TRACE_INFOR("append audio data to mvw");

		DefMVW & defMVW = dynamic_cast<DefMVW &>(def);

		while (current_position < audio_size) {
			if (audio_size - current_position < append_size_each_time) {
				nNumberOfToWrite = (audio_size - current_position) / 2;
			}

			if (first_append) {
				// 判断是否需要去wav头
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
				// 判断是否需要去wav头
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

	timer t;
	if (obj_type == "mvw") {
		DefMVW & defMVW = dynamic_cast<DefMVW &>(def);

		while (!fin.eof()) {
			buffer = new char[append_size_each_time];
			fin.read(buffer, sizeof(char) * append_size_each_time);

			if (first_append) {
				// 判断是否需要去wav头
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
				// 判断是否需要去wav头
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

// AIUI套件识别模块送音频接口，使用文件流持续读文件并送给套件。
void appendAudioToAIUISR(
	void * obj,			// 识别或唤醒句柄
	string pcm_path,	// 音频路径
	Def & def,			// 识别或唤醒的Def类
	int appendSizeEachTime,
	int blank_length	// 送完音频后，再送一段静音的长度，单位：ms
) {
	ISSErrID err = -1;
	int append_size_each_time = appendSizeEachTime; // 每次送音频的字节数
	DEBUG_TRACE("each time append audio size is :%d", append_size_each_time);
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
			// 判断是否需要去wav头
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

	// 送静音
	blank_length = blank_length * 32; // 音频时常转换为字节
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

// 获取目录大小，统计包括目录下的所有子目录和文件
// param root_folder: 待统计的根目录
// param dir_size:	  目录大小
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
