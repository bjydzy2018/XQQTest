/*
 * VADebug.h
 *
 *  Created on: 2013-4-8
 *      Author: pffang
 */

#ifndef VADEBUG_H_
#define VADEBUG_H_
#include "VACommon.h"

namespace VA {

class VAMemoryException : public std::exception {
    std::string message;
 public:
    //! The constructor.
    VAMemoryException() throw ()
            : message("An error occured during memory allocation.") {
    }

    //! The destructor.
    virtual ~VAMemoryException(void) throw () {
    }

    //! Returns the thrown error message as a c-style string.
    virtual const char* what(void) const throw () {
        return message.c_str();
    }
};

typedef enum tagVA_DEBUG_LEVEL {
    VA_DEBUG_LEVEL_NONE  = 0,
    VA_DEBUG_LEVEL_ERROR = 1,    /* 打开资源文件失败、内存不足、上传词典失败、编译词典失败、系统或引擎接口返回了错误码 */
    VA_DEBUG_LEVEL_INFOR = 2,    /* 模块初始化成功、上传词典成功、识别过程检测到语音端点、有识别结果 */
    VA_DEBUG_LEVEL_DEBUG = 3,    /* 最详细的trace：打开文件、上传操作编译词典 */
}VA_DEBUG_LEVEL;

class CTrace {
    static VA_DEBUG_LEVEL m_nVaDebugLevelMax;
    std::string m_szModuleA;
    std::wstring m_szModuleW;
#ifdef VA_VC_WINCE//WinCE平台log输出文件
	static FILE* m_fpLog;
#if VA_VC
	static CRITICAL_SECTION m_hMultexLog;
#elif VA_GNUC
	static pthread_mutex_t m_hMultexLog;
#endif
#endif

    void TraceA(const char* fmt, va_list args);
    void TraceW(const wchar_t* fmt, va_list args);

 public:
    CTrace(const char* szModule,
            VA_DEBUG_LEVEL nVaDebugLevelMax=VA_DEBUG_LEVEL_DEBUG);
    CTrace(const wchar_t* szModule,
            VA_DEBUG_LEVEL nVaDebugLevelMax=VA_DEBUG_LEVEL_DEBUG);
    ~CTrace();

    void Trace(const char* fmt, ...);
    void Trace(const wchar_t* fmt, ...);

    void TraceLevel(VA_DEBUG_LEVEL nVaDebugLevel, const char* fmt, ...);
    void TraceLevel(VA_DEBUG_LEVEL nVaDebugLevel, const wchar_t* fmt, ...);

    void SetTraceLevelMax(VA_DEBUG_LEVEL nVaDebugLevelMax);
#ifdef VA_VC_WINCE//WinCE平台log输出文件
	static bool TraceOpenFile(const char* fmt);
	static bool TraceOpenFile(const wchar_t* fmt);
	static void TraceCloseFile();
	void TraceWriteFile(std::string& fmt);
	void TraceWriteFile(std::wstring& fmt);
#endif
};
#if VA_GNUC && !VA_ANDROID && !VA_QNX
void RegisterBacktrace();
#endif

} /* namespace VA */

#endif /* VADEBUG_H_ */
