#include "VAInc.h"
#include "Def.h"

#include <string>

int sleep(int);
std::vector<std::string> getFilesInDir(const std::string &);
std::vector<std::string> readFileByLine(std::string, bool);
std::string readFile(std::string, bool);
void appendAudio(void *, std::string, std::string, Def &);
void appendBigAudio(void *, std::string, std::string, Def &);
void appendAudioToAIUISR(void *, std::string, Def &, int appendSizeEachTime, int blank_length = 0);
std::wstring widen(const std::string& str);
std::string narrow(const std::wstring& str);
void getFolderSize(std::string, long &);
