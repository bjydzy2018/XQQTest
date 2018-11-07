/*
 * VADebug.cpp
 *
 *  Created on: 2013-4-8
 *      Author: pffang
 */

#include "VADebug.h"
#include "VAString.h"
#include "MutexSignalCommon.h"

#if VA_ANDROID
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "VA_iFlytek", __VA_ARGS__))
#endif

#if VA_GNUC && !VA_ANDROID && !VA_QNX
#include <sys/syslog.h>
#endif

#if VA_QNX
#include <stdio.h>
#include <sys/slog.h>
#define IFLYTEK_LOG_MAJOR_ID_QNX (234)
#define IFLYTEK_LOG_MINOR_ID_QNX (0)
#endif

namespace VA {
VA_DEBUG_LEVEL CTrace::m_nVaDebugLevelMax = VA_DEBUG_LEVEL_DEBUG;
#ifdef VA_VC_WINCE//WinCE平台log输出文件
FILE* CTrace::m_fpLog = NULL;
#if VA_VC
CRITICAL_SECTION CTrace::m_hMultexLog;
#elif VA_GNUC
pthread_mutex_t CTrace::m_hMultexLog;
#endif
#endif

CTrace::CTrace(const char* szModule, VA_DEBUG_LEVEL nVaDebugLevelMax) {
    m_nVaDebugLevelMax = nVaDebugLevelMax;
    m_szModuleA = szModule;
#if VA_VC
    m_szModuleW = U8StrToU16Str(szModule);
#elif VA_GNUC
    m_szModuleW = (const wchar_t*)((const U32Char*)U8StrToU32Str(szModule));
#endif
}
CTrace::CTrace(const wchar_t* szModule, VA_DEBUG_LEVEL nVaDebugLevelMax) {
    m_nVaDebugLevelMax = nVaDebugLevelMax;
    m_szModuleW = szModule;
#if VA_VC
    m_szModuleA = U16StrToU8Str(szModule);
#elif VA_GNUC
    m_szModuleA = U32StrToU8Str((const U32Char*)szModule);
#endif
}
CTrace::~CTrace() {
}

void CTrace::TraceA(const char* fmt, va_list args) {
    std::string szOut;

    std::string t;
    std::string szBuff;

#if VA_VC
    DWORD tval;
    tval = ::GetTickCount();
    stringFormat(t, "%02u:%02u:%02u.%03u", tval/3600000, (tval/60000)%60, (tval/1000)%60, tval%1000);
#elif VA_GNUC
    char tx[28];
    int len = 28;
    struct timeval now;
    struct tm tm;
    gettimeofday(&now, NULL);
    localtime_r(&now.tv_sec, &tm);
    strftime(tx, len, "%Y-%m-%d %H:%M:%S", &tm);
    t = tx;
    stringFormatAppend(t, ".%6.6d", (int) (now.tv_usec));
#endif

    stringFormatV(szBuff, fmt, args);
    szOut.resize(t.size() + m_szModuleA.size() + szBuff.size() + 4);
    stringFormat(szOut, "[%s][%s]%s", t.c_str(), m_szModuleA.c_str(),
                    szBuff.c_str());
#if VA_VC
#if VA_VC_WINCE
    OutputDebugStringW((const wchar_t*)GBKStrToU16Str(szOut.c_str()));
    OutputDebugStringW(L"\r\n");
#else
	fprintf(stdout, "%s\n", szOut.c_str());
    OutputDebugStringA(szOut.c_str());
    OutputDebugStringA("\r\n");
#endif
#elif defined VA_ANDROID
    // for logcat
    LOGI("%s", szOut.c_str());
#elif VA_GNUC && !VA_ANDROID && !VA_QNX
    syslog(LOG_DEBUG, "%s", szOut.c_str());
#elif VA_QNX
	fprintf(stderr, "%s\n", szOut.c_str());
	slogf(_SLOG_SETCODE(IFLYTEK_LOG_MAJOR_ID_QNX, IFLYTEK_LOG_MINOR_ID_QNX),
		_SLOG_DEBUG1, "%s", szOut.c_str());
#else
    fprintf(stderr, "%s\n", szOut.c_str());
#endif
#ifdef VA_VC_WINCE//WinCE平台log输出文件
	TraceWriteFile(szOut);
#endif
}
void CTrace::TraceW(const wchar_t* fmt, va_list args) {
    std::wstring t;

#if VA_VC
    DWORD tval;
    tval = ::GetTickCount();
    wstringFormat(t, L"%02u:%02u:%02u.%03u", tval/3600000, (tval/60000)%60, (tval/1000)%60, tval%1000);
#elif VA_GNUC
    wchar_t tx[28];
    size_t len = 28;
    struct timeval now;
    struct tm tm;
    gettimeofday(&now, NULL);
    localtime_r(&now.tv_sec, &tm);
    wcsftime(tx, len, L"%Y-%m-%d %H:%M:%S", &tm);
    t = tx;
    wstringFormatAppend(t, L".%6.6d", (int) (now.tv_usec));
#endif
    std::wstring szBuff;
    wstringFormatV(szBuff, fmt, args);
    std::wstring szOut;
    szOut.resize(t.size() + m_szModuleW.size() + szBuff.size() + 4);
    wstringFormat(szOut, L"[%ls][%ls]%ls", t.c_str(), m_szModuleW.c_str(),
                  szBuff.c_str());

#if VA_VC
    OutputDebugStringW(szOut.c_str());
    OutputDebugStringW(L"\r\n");
#if VA_VC_WIN_PC
	fwprintf(stdout, L"%ls\n", szOut.c_str());
#endif
#elif VA_ANDROID
    // for logcat
    LOGI("%s", (const U8Char*)(U32StrToU8Str(szOut.c_str())));
#else
    fwprintf(stderr, L"%ls\n", szOut.c_str());
#endif
#ifdef VA_VC_WINCE//WinCE平台log输出文件
	TraceWriteFile(szOut);
#endif
}

void CTrace::Trace(const char* fmt, ...) {
    if(VA_DEBUG_LEVEL_DEBUG<=m_nVaDebugLevelMax) {
        va_list marker;
        va_start(marker, fmt);
        TraceA(fmt, marker);
        va_end(marker);
    }
}
void CTrace::Trace(const wchar_t* fmt, ...) {
    if(VA_DEBUG_LEVEL_DEBUG<=m_nVaDebugLevelMax) {
        va_list marker;
        va_start(marker, fmt);
#if VA_VC
        TraceW(fmt, marker);
#elif VA_GNUC
        TraceA(U32StrToU8Str((const U32Char*)fmt), marker);
#endif
        va_end(marker);
    }
}
void CTrace::TraceLevel(VA_DEBUG_LEVEL nVaDebugLevel, const char* fmt, ...) {
    if(VA_DEBUG_LEVEL_NONE<nVaDebugLevel
        && nVaDebugLevel<=m_nVaDebugLevelMax) {
            va_list marker;
            va_start(marker, fmt);
            TraceA(fmt, marker);
            va_end(marker);
    }
}
void CTrace::TraceLevel(VA_DEBUG_LEVEL nVaDebugLevel, const wchar_t* fmt, ...) {
    if(VA_DEBUG_LEVEL_NONE<nVaDebugLevel
        && nVaDebugLevel<=m_nVaDebugLevelMax) {
            va_list marker;
            va_start(marker, fmt);
#if VA_VC
            TraceW(fmt, marker);
#elif VA_GNUC
            TraceA(U32StrToU8Str((const U32Char*)fmt), marker);
#endif
            va_end(marker);
    }
}
void CTrace::SetTraceLevelMax(VA_DEBUG_LEVEL nVaDebugLevelMax) {
    m_nVaDebugLevelMax = nVaDebugLevelMax;
}
#ifdef VA_VC_WINCE//WinCE平台log输出文件
#if VA_GNUC
bool CTrace::TraceOpenFile(const char* fmt) {
	pthread_mutex_init(&m_hMultexLog, NULL);
	if (!m_fpLog) {
		m_fpLog = fopen(fmt, "w+");
	}
#elif VA_VC
bool CTrace::TraceOpenFile(const wchar_t* fmt) {
	InitializeCriticalSection(&m_hMultexLog);
	if (!m_fpLog) {
		m_fpLog = _wfopen(fmt, L"w+");
	}
#endif
	return m_fpLog != NULL;
}

void CTrace::TraceCloseFile() {
	if (m_fpLog) {
		EnterMutex(&m_hMultexLog);
		fclose(m_fpLog);
		m_fpLog = NULL;
		LeaveMutex(&m_hMultexLog);
	}

#if VA_VC
	DeleteCriticalSection(&m_hMultexLog);
#elif VA_GNUC
	pthread_mutex_destroy(&m_hMultexLog);
#endif
}

void CTrace::TraceWriteFile(std::string& fmt) {
	if (m_fpLog) {
		EnterMutex(&m_hMultexLog);
		fprintf(m_fpLog, "%s\n", fmt.c_str());
		LeaveMutex(&m_hMultexLog);
	}
}

void CTrace::TraceWriteFile(std::wstring& fmt) {
	if (m_fpLog) {
		EnterMutex(&m_hMultexLog);
		fwprintf(m_fpLog, L"%ls\n", fmt.c_str());
		LeaveMutex(&m_hMultexLog);
	}
}
#endif
#if 0
static void crit_err_hdlr(int sig_num, siginfo_t * info, void * ucontext) {
    FILE* out = stderr;
    size_t max_frames = 63;
    fprintf(out, "signal %d(%s), address is %#x\n", sig_num, strsignal(sig_num),
            (unsigned int) (info->si_addr));
    fprintf(out, "stack trace:\n");
// storage array for stack trace address data
    void* addrlist[max_frames + 1];
// retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));
    if (addrlen == 0) {
        fprintf(out, "  <empty, possibly corrupt>\n");
        return;
    }
// resolve addresses into strings containing "filename(function+address)",
// this array must be free()-ed
    char** symbollist = backtrace_symbols(addrlist, addrlen);
// allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    char* funcname = (char*) malloc(funcnamesize);
// iterate over the returned symbol lines. skip the first, it is the
// address of this function.
    for (int i = 1; i < addrlen; i++) {
        char *begin_name = 0, *begin_offset = 0, *end_offset = 0;
        // find parentheses and +address offset surrounding the mangled name:
        // ./module(function+0x15c) [0x8048a6d]
        for (char *p = symbollist[i]; *p; ++p) {
            if (*p == '(')
                begin_name = p;
            else if (*p == '+')
                begin_offset = p;
            else if (*p == ')' && begin_offset) {
                end_offset = p;
                break;
            }
        }

        if (begin_name && begin_offset && end_offset
                        && begin_name < begin_offset) {
            *begin_name++ = '\0';
            *begin_offset++ = '\0';
            *end_offset = '\0';

            // mangled name is now in [begin_name, begin_offset) and caller
            // offset in [begin_offset, end_offset). now apply
            // __cxa_demangle():
            int status;
            char* ret = abi::__cxa_demangle(begin_name, funcname, &funcnamesize,
                                            &status);
            if (status == 0) {
                funcname = ret;    // use possibly realloc()-ed string
                fprintf(out, "  %s : %s+%s\n", symbollist[i], funcname,
                        begin_offset);
            } else {
                // demangling failed. Output function name as a C function with
                // no arguments.
                fprintf(out, "  %s : %s()+%s\n", symbollist[i], begin_name,
                        begin_offset);
            }
        } else {
            // couldn't parse the line? print the whole line.
            fprintf(out, "  %s\n", symbollist[i]);
        }
    }
    free(funcname);
    free(symbollist);
    abort();
}

/** Print a demangled stack backtrace of the caller function to FILE* out. */
static void print_stacktrace(const char* source, FILE *out = stderr,
                             unsigned int max_frames = 63) {
    fprintf(out, "stack trace:\n");
// storage array for stack trace address data
    void* addrlist[max_frames + 1];
// retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));
    if (addrlen == 0) {
        fprintf(out, "  <empty, possibly corrupt>\n");
        return;
    }
// resolve addresses into strings containing "filename(function+address)",
// this array must be free()-ed
    char** symbollist = backtrace_symbols(addrlist, addrlen);
// allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    char* funcname = (char*) malloc(funcnamesize);
// iterate over the returned symbol lines. skip the first, it is the
// address of this function.
    for (int i = 1; i < addrlen; i++) {
        char *begin_name = 0, *begin_offset = 0, *end_offset = 0;
        // find parentheses and +address offset surrounding the mangled name:
        // ./module(function+0x15c) [0x8048a6d]
        for (char *p = symbollist[i]; *p; ++p) {
            if (*p == '(')
                begin_name = p;
            else if (*p == '+')
                begin_offset = p;
            else if (*p == ')' && begin_offset) {
                end_offset = p;
                break;
            }
        }

        if (begin_name && begin_offset && end_offset
                        && begin_name < begin_offset) {
            *begin_name++ = '\0';
            *begin_offset++ = '\0';
            *end_offset = '\0';

            // mangled name is now in [begin_name, begin_offset) and caller
            // offset in [begin_offset, end_offset). now apply
            // __cxa_demangle():
            int status;
            char* ret = abi::__cxa_demangle(begin_name, funcname, &funcnamesize,
                                            &status);
            if (status == 0) {
                funcname = ret;    // use possibly realloc()-ed string
                fprintf(out, "  %s : %s+%s\n", symbollist[i], funcname,
                        begin_offset);
            } else {
                // demangling failed. Output function name as a C function with
                // no arguments.
                fprintf(out, "  %s : %s()+%s\n", symbollist[i], begin_name,
                        begin_offset);
            }
        } else {
            // couldn't parse the line? print the whole line.
            fprintf(out, "  %s\n", symbollist[i]);
        }
    }
    free(funcname);
    free(symbollist);
    abort();
}

static void terminate_handler() {
    print_stacktrace("terminate_handler");
}

static void unexpected_handler() {
    print_stacktrace("unexpected_handler");
}

static void exit_handler(int sig) {
    fprintf(stderr, "exit_handler\n");
    //print_stacktrace("exit_handler");
    signal(sig, SIG_IGN);
    exit(EXIT_FAILURE);
}

void RegisterBacktrace() {
    struct sigaction sigact;
    sigact.sa_flags = SA_RESTART | SA_SIGINFO;
    sigact.sa_sigaction = crit_err_hdlr;
    sigemptyset(&sigact.sa_mask);

    sigaction(SIGSEGV, &sigact, (struct sigaction *) NULL);
    sigaction(SIGSTKFLT, &sigact, (struct sigaction *) NULL);
    sigaction(SIGFPE, &sigact, (struct sigaction *) NULL);
//    sigaction(SIGABRT, &sigact, (struct sigaction *) NULL);

//    signal(SIGINT, exit_handler);
//    signal(SIGTERM, exit_handler);
//    signal(SIGQUIT, exit_handler);
//    signal(SIGHUP, exit_handler);
//    signal(SIGKILL, exit_handler);

    std::set_terminate(terminate_handler);
    std::set_unexpected(unexpected_handler);
//    fprintf(stderr, "RegisterBacktrace\n");
    openlog("[iFLYTEK]", LOG_PERROR, LOG_DEBUG);
}
#endif

} /* namespace VA */

