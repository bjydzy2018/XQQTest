#include "VAInc.h"
#include "Def.h"
#include "test_log.h"
#include <string>

int sleep(int);
string LongToString(long);
string DoubleToString(double);
bool startswith(const std::string& , const std::string& );
bool endswith(const std::string& , const std::string& );
long GetFileSize(string);
void replace_all(std::string &, std::string const &, std::string const &);
std::vector<std::string> getFilesInDir(const std::string &);
std::vector<std::string> readFileByLine(std::string, bool);
std::vector<std::string> readFileByLine(std::string, bool,bool);
std::string readFile(std::string, bool);
void appendAudio(void *, std::string, std::string, Def &);
void appendBigAudio(void *, std::string, std::string, Def &);
void appendAudioToAIUISR(void *, std::string, Def &, int blank_length);
void appendAudioToAIUISRNoSleep(void *, std::string, Def &, int blank_length);
std::wstring widen(const std::string& str);
std::string narrow(const std::wstring& str);
void getFolderSize(std::string, long &);
std::vector<std::string> splitString(string& str, char c);
