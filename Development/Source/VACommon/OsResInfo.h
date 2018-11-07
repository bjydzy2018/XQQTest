#ifndef _OS_RESOURCE_INFO_20150330_H_
#define _OS_RESOURCE_INFO_20150330_H_


/**
* C/C++ wchar_t support
*/
#ifdef __cplusplus
# include <cwchar>
#else  /* c */
# include <wchar.h>
#endif /* wchar */

#ifdef __cplusplus
extern "C"{
#endif

/**
* IFR API type
*/
#if defined(_MSC_VER)            /* Microsoft Visual C++ */
#if !defined(ORIFAPI)
#define ORIFAPI __stdcall
#endif
#pragma pack(push, 8)
#elif defined(__BORLANDC__)      /* Borland C++ */
#if !defined(ORIFAPI)
#define ORIFAPI __stdcall
#endif
#pragma option -a8
#elif defined(__WATCOMC__)       /* Watcom C++ */
#if !defined(ORIFAPI)
#define ORIFAPI __stdcall
#endif
#pragma pack(push, 8)
#else                            /* Any other including Unix */
#if !defined(ORIFAPI)
#define ORIFAPI
#endif
#endif


	/**
	* True and false
	*/
#ifndef FALSE
#define FALSE   0
#endif	/* FALSE */

#ifndef TRUE
#define TRUE   1
#endif	/* TRUE */

    //������Դͳ�ƽ����Ϣ
    typedef struct _OsResResult
    {
        unsigned long       OsRESPid;
        double              CpuUsage;
        double              CpuKernelUsage;
        double              CpuUserUsage;
        unsigned long       NumberOfHandle;
        unsigned long       NumberOfThread;
        double              VirtualMemorySize;
        double              WorkingSetMemorySize;

    }OsResResult, *POsResResult;

    typedef enum
    {
        OSRES_STATUS_SUCCESS = 0,
        OSRES_STATUS_NATIVEFAIL=1,
        OSRES_STATUS_MEMORYERROR=2,
        OSRES_STATUS_INITFAIL=3,
        OSRES_STATUS_PARAMINVALID=4,
        OSRES_STATUS_PARAMERROR=5
    }OsResError;

    //��ʼ��
    int ORIFAPI OsResInfoInitialize();
    typedef int(ORIFAPI * Proc_OsResInfoInitialize)();


    //���ʼ��
    int ORIFAPI OsResInfoUnInitialize();
    typedef int(ORIFAPI *Proc_OsResInfoUnInitialize)();


    //���ò���
    //default OsRES_Pid,��ǰ����id
    int ORIFAPI  OsResInfoSetParam(const char* Param, const char* Value);
    typedef int(ORIFAPI *Proc_OsResInfoSetParam)(const char* Param, const char* Value);

    //�ռ�����
    int ORIFAPI OsResInfoCollectData();
    typedef int(ORIFAPI *Proc_OsResInfoCollectData)();

    //��ȡ����ϵͳ����ͳ�ƽ��
    int ORIFAPI OsResInfoGetResult(POsResResult Result);
    typedef int(ORIFAPI *Proc_OsResInfoGetResult)(POsResResult Result);

#ifdef __cplusplus
}
#endif


#endif /* _OS_RESOURCE_INFO_20150330_H_ */
