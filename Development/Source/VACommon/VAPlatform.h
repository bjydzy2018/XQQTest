#pragma once
#ifndef __VAPLATFORM_H__
#define __VAPLATFORM_H__


#if _WIN32
    #ifdef _DEBUG
    #define VA_DEBUG (1)
    #else
    #define VA_DEBUG (1)
    #endif
#else
    #ifdef NDEBUG
    #define VA_DEBUG (0)
    #else
    #define VA_DEBUG (1)
    #endif
#endif

#if defined(__ANDROID__)
    #ifdef VA_DEBUG
    #undef VA_DEBUG
    #endif
    #define VA_DEBUG (1)
    // #pragma message("VA_DEBUG set to 1 under __ANDROID__")
#endif



#define ACTIVE_PATH 1
#define TAG_A "[TAG_A] "
#if defined __GNUC__ /*检测gcc类编译器*/
#define UNREFERENCED_PARAMETER(P) (P)=(P)
//#pragma message("GCC compiler")
#define VA_GNUC 1
#if defined __i386__ /*检测编译器使用x86指令集*/
//#pragma message("GCC x86")
#define VA_GNUC_x86 1
#elif defined __x86_64__ /*检测编译器使用x86_64指令集*/
//#pragma message("GCC x86_64")
#define VA_GNUC_x64 1
#elif defined __arm__ /*检测编译器使用arm指令集，包含arm指令集的Android*/
//#pragma message("GCC arm")
#define VA_GNUC_ARM 1
#if defined __ARM_NEON__
//#pragma message("GCC arm neon")
#define VA_GNUC_ARM_NEON 1 /*设定使用arm neon指令优化*/
#endif
#endif
#elif defined _MSC_VER /*检测微软编译器*/
//#pragma message("Microsoft Visual C++")
#define VA_VC 1
#if defined _WIN32_WCE
//#pragma message("WinCE")
#define VA_VC_WINCE 1
#if defined _M_ARM
//#pragma message("WinCE ARM")
#define VA_VC_WINCE_ARM 1
#if _M_ARM == 7
#pragma message("WinCE ARMv7")
#define VA_VC_WINCE_ARMv7 1
#elif _M_ARM == 6
#pragma message "WinCE ARMv6"
#define VA_VC_WINCE_ARMv6 1
#elif _M_ARM == 5
#pragma message("WinCE ARMv5")
#define VA_VC_WINCE_ARMv5 1
#elif _M_ARM == 4
#pragma message("WinCE ARMv4")
#define VA_VC_WINCE_ARMv4 1
#endif
#elif defined _M_MRX000
#pragma message("WinCE MIPS")
#define VA_VC_WINCE_MIPS 1
#elif defined _M_X86
#pragma message("WinCE x86")
#define VA_VC_WINCE_x86 1
#endif
#else
#pragma message("Windows Desktop")
#define VA_VC_WIN_PC 1
#if defined _M_X64
#pragma message("Windows Desktop x64")
#define VA_VC_WIN_PC_x64 1
#elif defined _M_X86
#pragma message("Windows Desktop x86")
#define VA_VC_WIN_PC_x86 1
#endif
#endif
#else
#pragma message("Other need add more")
#define VA_OTHER_COMPILER
#endif

#if defined(__ANDROID__) /*检测Android平台，注意这是Android平台宏，不是CPU类型，可能包含arm、mips、x86等多种*/
#define VA_ANDROID 1
#endif
#if defined(__QNX__) /*检测QNX平台，注意这是QNX平台宏，不是CPU类型，可能包含arm、mips、x86等多种*/
#define VA_QNX 1
//#define VA_DEBUG (1)
#endif

#if VA_GNUC
#define VA_SET_PROCESS_PRIORITY_AND_NAME (1)
#endif

#endif
