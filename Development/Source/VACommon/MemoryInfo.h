//#include "stdafx.h"
#ifndef _MEMINFO_H_
#define _MEMINFO_H_
#include "OsResInfo.h"
#include <time.h>
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#include <Psapi.h>
#else
#include <pthread.h>
#endif

#include "VAInc.h"
#include "log/sr_log.h"

#pragma comment(lib, "Psapi")

typedef struct memoryInfo{
	double              VirtualMemorySize;
	double              WorkingSetMemorySize;
}memoryInfo;

namespace VA {
	class Memory
	{
	public:
		~Memory();

		static Memory* GetInstance(void);
		void printMemory();
	private:
		Memory();
		static Memory* m_memory;
		static mutex_lock	  m_hMutexMemory; //创建实例时的锁

		class CGarbo {
		public:
			CGarbo(){};
			~CGarbo(){
				if(Memory::m_memory)
					delete Memory::m_memory;
			}
		};
		static CGarbo garbo;
	private:
#ifndef _WIN32
		pthread_mutex_t m_MemoryMutex;
#endif
};
}

#endif  //_MEMINFO_H_
