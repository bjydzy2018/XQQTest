
#include "MemoryInfo.h"


namespace VA {
Memory* Memory::m_memory = NULL;
mutex_lock Memory::m_hMutexMemory;

Memory::Memory()
{
#ifndef _WIN32
	OsResInfoInitialize();
	pthread_mutex_init(&m_MemoryMutex,NULL);
#endif
}

Memory::~Memory()
{
#ifndef _WIN32
	OsResInfoUnInitialize();
	pthread_mutex_destroy(&m_MemoryMutex);
#endif
}

Memory* Memory::GetInstance()
{
	if(m_memory == NULL)
	{
		AUTO_LOCK(&m_hMutexMemory);
		if(m_memory == NULL)
		{
			m_memory = new Memory();
		}
	}
	return m_memory;
}

void Memory::printMemory()
{
	double VirtualMemorySize;
	double WorkingSetMemorySize;
#ifndef _WIN32
	OsResResult osResResult;
	pthread_mutex_lock(&m_MemoryMutex);
	OsResInfoCollectData();
	OsResInfoGetResult(&osResResult);
	pthread_mutex_unlock(&m_MemoryMutex);
	VirtualMemorySize = osResResult.VirtualMemorySize;
	WorkingSetMemorySize = osResResult.WorkingSetMemorySize;
#else
	HANDLE							curProcessHandle;
	PROCESS_MEMORY_COUNTERS			processMemoryCounters;
	curProcessHandle = GetCurrentProcess();
	if( GetProcessMemoryInfo(
		curProcessHandle,
		&processMemoryCounters,
		sizeof(processMemoryCounters)
		) )
	{	
		VirtualMemorySize = processMemoryCounters.PagefileUsage/1024;
		WorkingSetMemorySize = processMemoryCounters.WorkingSetSize/1024;
	}
#endif
	DEBUG_TRACE_ERROR("VirtualMemorySize = %lf\n"
				"WorkingSetMemorySize = %lf\n",
				 VirtualMemorySize, WorkingSetMemorySize);

}
}
