
#include "OsResInfo.h"
#ifdef WIN32
#include "OsPrsApi.h"
#else
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#endif

#ifdef __GNUC__
#pragma message("_LINUX_ARM_")
#endif

static unsigned long OsRES_Pid;                 //监控进程pid
static bool OsRES_InitOK = false;

void _trim(char *str)
{
	if(str == NULL)
		return;
	if( '\n' == str[strlen(str)-1] )
		str[strlen(str)-1] = '\0';
	return;

}

#ifndef WIN32
//static char  OsRES_Name[100] = {0};     //监控进程名称
static char  OsRES_Name[100] = "spw";     //监控进程名称
static OsResResult OsRESResultInfo;

//void GetNameFromPid()
//{
//    FILE   *stream = NULL;
//    unsigned long  wmem = 0;
//    char   cmd[100] = {0};
//	//linux 
//    //sprintf(cmd, "ps -p %d | grep %d | awk '{print $4}'", OsRES_Pid, OsRES_Pid);
//	sprintf(cmd, "ps -p %d | grep %d | awk '{print $13}'", OsRES_Pid, OsRES_Pid);
//    memset(OsRES_Name, 0, 100);
//    stream = popen(cmd, "r" );                      //将cmd命令的输出 通过管道读取（“r”参数）到FILE* stream
//    fread(OsRES_Name, sizeof(char), sizeof(OsRES_Name), stream);  //将刚刚FILE* stream的数据流读取到buf中
//    OsRES_Name[strlen(OsRES_Name)-1] = 0;
//    pclose( stream );
//
//    return;
//}

double GetWMem(unsigned long pid)
{
    FILE   *stream = NULL;
    unsigned long  wmem = 0;
    char   cmd[100] = {0};
    char   rlt[100] = {0};
#ifdef __GNUC__
    sprintf(cmd, "ps -o rss -p %d | grep -v RSS", pid);
#endif

#ifdef _ANDROID_
	sprintf(cmd, "ps -p %d | grep -v RSS | awk '{print $5}'", pid);
//#else
//	return 0.0f;
#endif
    stream = popen(cmd, "r" );                      //将cmd命令的输出 通过管道读取（“r”参数）到FILE* stream
    fread(rlt, sizeof(char), sizeof(rlt), stream);  //将刚刚FILE* stream的数据流读取到buf中
    pclose( stream );

    wmem = atol(rlt);

    //return (double)wmem / 1024.0f;
	return (double)wmem;
}

double GetVMem(unsigned long pid)
{
    FILE   *stream = NULL;
    unsigned long  vmem = 0;
    char   cmd[100] = {0};
    char   rlt[100] = {0};

#ifdef __GNUC__
    sprintf(cmd, "ps -o vsz -p %d | grep -v VSZ", pid);
#endif

//ANDROID
#ifdef _ANDROID_
	sprintf(cmd, "ps -p %d | grep -v VSIZE | awk '{print $4}'", pid);
#endif

//QNX
#ifdef _QNX_
	sprintf(cmd, "showmem -P %d | grep spw | awk '{print $1}'", pid);
#endif

    stream = popen(cmd, "r" );                      //将cmd命令的输出 通过管道读取（“r”参数）到FILE* stream
    fread(rlt, sizeof(char), sizeof(rlt), stream);  //将刚刚FILE* stream的数据流读取到buf中
    pclose( stream );

	_trim(rlt);
    vmem = atol(rlt);
    //return (double)vmem / 1024.0f;
#ifdef _QNX_
	return (double)vmem/1024.0f;
#else
	return (double)vmem;
#endif
}

unsigned long GetHandle(unsigned long pid)
{
    FILE   *stream = NULL;
    unsigned long  hcnt = 0;
    char   cmd[100] = {0};
    char   rlt[100] = {0};
    //sprintf(cmd, "sudo ls /proc/%d/fd -l | grep socket:|wc -l", pid);

#ifndef _QNX_
	sprintf(cmd, "ls /proc/%d/fd -l | grep socket:|wc -l", pid);
#else
	return 0;
#endif

    stream = popen(cmd, "r" );                      //将cmd命令的输出 通过管道读取（“r”参数）到FILE* stream
    fread(rlt, sizeof(char), sizeof(rlt), stream);  //将刚刚FILE* stream的数据流读取到buf中
    pclose( stream );

    hcnt = atol(rlt);

    return hcnt;
}

unsigned long GetThreadCnt(unsigned long pid)
{
    FILE   *stream = NULL;
    unsigned long  tcnt = 0;
    char   cmd[100] = {0};
    char   rlt[100] = {0};

#ifndef _QNX_ 
    sprintf(cmd, "awk '{print $20}' /proc/%d/stat", pid);
#else
	return 0;
#endif

    stream = popen(cmd, "r" );                      //将cmd命令的输出 通过管道读取（“r”参数）到FILE* stream
    fread(rlt, sizeof(char), sizeof(rlt), stream);  //将刚刚FILE* stream的数据流读取到buf中
    pclose( stream );

    tcnt = atol(rlt);

    return tcnt;
}

double GetProcessTime(unsigned long pid)
{
    FILE   *stream = NULL;
    unsigned long  ptime = 0;
    char   cmd[100] = {0};
    char   rlt[100] = {0};

#ifdef __GNUC__
    sprintf(cmd, "top -p %d -n 1 -b | grep %s | awk '{print $9}'", pid, OsRES_Name);
#endif

#ifdef _ANDROID_
	sprintf(cmd, "top -n 1 | grep %s | awk '{print $3}'", OsRES_Name);
#endif

#ifdef _QNX_
	return 0.0f;
#endif

    stream = popen(cmd, "r" );
    fscanf(stream, "%s", rlt);
	//fread(rlt, sizeof(char), sizeof(rlt), stream);  //将刚刚FILE* stream的数据流读取到buf中
    pclose( stream );
    ptime = atol(rlt);

    return (double)ptime;
}

void OsInfoDataUpdate(unsigned long ProcessId)
{
    memset(&OsRESResultInfo, 0, sizeof(OsResResult));
    OsRESResultInfo.CpuUsage = GetProcessTime(ProcessId);
    OsRESResultInfo.NumberOfHandle = GetHandle(ProcessId);
    OsRESResultInfo.NumberOfThread = GetThreadCnt(ProcessId);
    OsRESResultInfo.VirtualMemorySize = GetVMem(ProcessId);
    OsRESResultInfo.WorkingSetMemorySize = GetWMem(ProcessId);
}

#endif

int ORIFAPI OsResInfoInitialize()
{
    if (OsRES_InitOK)
    {
        return OSRES_STATUS_INITFAIL;
    }

#ifdef WIN32
    INT status = OsPRSInitialize();
    OsRES_InitOK = true;
    return status;
#else
    OsRES_Pid = getpid();
    //GetNameFromPid();
    OsRES_InitOK = true;
    return OSRES_STATUS_SUCCESS;
#endif
}

int ORIFAPI OsResInfoUnInitialize()
{
    if (!OsRES_InitOK)
    {
        return OSRES_STATUS_INITFAIL;
    }

#ifdef WIN32
    INT status = OsPRSUnInitialize();
    OsRES_InitOK = false;
    return status;
#else
    OsRES_InitOK = false;
    return OSRES_STATUS_SUCCESS;
#endif
}

int ORIFAPI  OsResInfoSetParam(const char* Param, const char* Value)
{
    if (!OsRES_InitOK)
    {
        return OSRES_STATUS_INITFAIL;
    }

#ifdef WIN32
    return OsPRSSetParam((CONST PCHAR)Param, (CONST PCHAR)Value);
#else
    if (strcmp(Param,"OsRES_Pid")==0){
        OsRES_Pid = atol(Value);
        //GetNameFromPid();
    }
    else{
        return OSRES_STATUS_PARAMINVALID;
    }
    return OSRES_STATUS_SUCCESS;
#endif
}

int ORIFAPI OsResInfoCollectData()
{
    if (!OsRES_InitOK)
    {
        return OSRES_STATUS_INITFAIL;
    }

#ifdef WIN32
    return OsPRSCollectData();
#else
    static bool firstCall = true;
    if (!OsRES_InitOK){
        return OSRES_STATUS_INITFAIL;
    }
    if (firstCall){
        OsInfoDataUpdate(OsRES_Pid);
        firstCall = false;
        usleep(200000);
    }
    OsInfoDataUpdate(OsRES_Pid);

    return OSRES_STATUS_SUCCESS;
#endif
}

int ORIFAPI OsResInfoGetResult(POsResResult Result)
{
    if (!OsRES_InitOK)
    {
        return OSRES_STATUS_INITFAIL;
    }

#ifdef WIN32
    OsPRSResult TmpResult = {0};
    INT status = OsPRSGetResult(&TmpResult);
    Result->OsRESPid = TmpResult.OsPRSPid;
    Result->CpuKernelUsage = (double)TmpResult.CpuKernelUsage * 100.0f;
    Result->CpuUsage = (double)TmpResult.CpuUsage * 100.0f;
    Result->CpuUserUsage = (double)TmpResult.CpuUserUsage * 100.0f;
    Result->NumberOfHandle = TmpResult.NumberOfHandle;
    Result->NumberOfThread = TmpResult.NumberOfThread;
    Result->VirtualMemorySize = (double)TmpResult.VirtualMemorySize / 1048576.0f;
    Result->WorkingSetMemorySize = (double)TmpResult.WorkingSetMemorySize / 1048576.0f;
    return status;
#else
    memcpy(Result, &OsRESResultInfo, sizeof(OsResResult));
    Result->OsRESPid = OsRES_Pid;
    return OSRES_STATUS_SUCCESS;
#endif
}
