/******************************************************************************
* FileName		       : Thread.cpp
* Description          : Thread source file
* Author               : jcgu
* Date Of Creation     : 2005_10_29
* Platform             : Win32/WinCE
* Modification History : 
*------------------------------------------------------------------------------
* Date        Author     Modifications
*------------------------------------------------------------------------------
* 2005_10_29  jcgu	     Created
******************************************************************************/

#include "VAInc.h"
#if VA_VC

#include "stdafx.h"

#include "Thread.h"

CThread::CThread()
{
	m_dwThreadID = 0;
	m_hThread = NULL;
	m_nPriority = THREAD_PRIORITY_NORMAL;
}

BOOL CThread::SetThreadPriority(int nPriority)
{
	return ::SetThreadPriority(m_hThread, nPriority);
}

int CThread::GetThreadPriority(void) const
{
	return ::GetThreadPriority(m_hThread);
}

DWORD CThread::SuspendThread(void)
{
	return ::SuspendThread(m_hThread);
}

DWORD CThread::ResumeThread(void)
{
	return ::ResumeThread(m_hThread);
}

BOOL CThread::StartThread(void)
{
	m_hThread = ::CreateThread(NULL, 0, ThreadProcS, this, 
		0, &m_dwThreadID);
	return (m_hThread == NULL) ? FALSE : TRUE;
}

DWORD CThread::GetThreadID(void) const
{
	return m_dwThreadID;
}

DWORD WINAPI CThread::ThreadProcS(LPVOID lpParameter)
{
	CThread* pThread = (CThread*)lpParameter;
	if ( pThread != NULL )
		return pThread->ThreadProc();
	return 0;	// error return zero
}

#endif
