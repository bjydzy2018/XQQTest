/******************************************************************************
* FileName		       : Thread.h
* Description          : Thread head file
* Author               : jcgu
* Date Of Creation     : 2005_10_29
* Platform             : Win32/WinCE
* Modification History : 
*------------------------------------------------------------------------------
* Date        Author     Modifications
*------------------------------------------------------------------------------
* 2005_10_29  jcgu	     Created
******************************************************************************/

#ifndef _THREAD_H_2005_10_29_INCLUDE_
#define _THREAD_H_2005_10_29_INCLUDE_

#include "VAInc.h"
#if VA_VC

#include <windows.h>

class CThread
{
protected:
	BOOL SetThreadPriority(int nPriority);
	int GetThreadPriority(void) const;
	DWORD SuspendThread(void);
	DWORD ResumeThread(void);
	BOOL StartThread(void);
	DWORD GetThreadID(void) const;

protected:
	virtual DWORD ThreadProc(void) = 0;
	static DWORD WINAPI ThreadProcS(LPVOID lpParameter);

protected:
	CThread();

private:
	DWORD	m_dwThreadID;
	HANDLE  m_hThread;
	int		m_nPriority;
};

#endif	// VA_VC
#endif	// _THREAD_H_2005_10_29_INCLUDE_
