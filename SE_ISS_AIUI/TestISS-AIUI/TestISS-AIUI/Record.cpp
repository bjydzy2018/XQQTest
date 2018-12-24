/******************************************************************************
* FileName		       : Record.cpp
* Description          : Record source file
* Author               : jcgu
* Date Of Creation     : 2005-11-5
* Platform             : Win32/WinCE
* Modification History : 
*------------------------------------------------------------------------------
* Date        Author     Modifications
*------------------------------------------------------------------------------
* 2005-11-05  jcgu	     Created
* 2006-08-23  jcgu	     Modify
******************************************************************************/
#include "VAInc.h"
#if VA_VC

#include "stdafx.h"

#include "Record.h"

#ifdef _WIN32_WCE
#pragma comment(lib, "coredll.lib")
#else
#pragma comment(lib, "winmm.lib")
#endif

///////////////////////////////////////////////////////////
// open device
RRESULT CRecord::OpenDevice(const WAVEFORMATEX* pwf /*= NULL*/)
{
	// create buffer
	m_pBuffer = new BUFFER[BUFNUM];
	if ( NULL == m_pBuffer )
		return RECORD_ERR_CREATEBUF;

	// create wave header
	m_pWaveHDR = new WAVEHDR[BUFNUM];
	if ( NULL == m_pWaveHDR )
	{
		Uninitialize();
		return RECORD_ERR_CREATEHDR;
	}

	// create record callback thread
	if ( !StartThread() )
	{
		Uninitialize();
		return RECORD_ERR_CREATETHREAD;
	}

	WAVEFORMATEX waveformatex;
	if ( NULL != pwf )
	{
		waveformatex = *pwf;
	}
	else
	{
		waveformatex.wFormatTag = WAVE_FORMAT_PCM;
		waveformatex.nChannels = 1;
		waveformatex.nSamplesPerSec = ESR_SAMPLE_RATE;
		waveformatex.wBitsPerSample = 16;
		waveformatex.nBlockAlign = (waveformatex.wBitsPerSample * waveformatex.nChannels) / 8;
		waveformatex.nAvgBytesPerSec =  waveformatex.nSamplesPerSec * waveformatex.nBlockAlign;
	}
	// open wave device
	MMRESULT mmhr = ::waveInOpen(&m_hWaveIn, WAVE_MAPPER, &waveformatex, 
		(DWORD)GetThreadID(), (DWORD)this, CALLBACK_THREAD);
	if ( MMSYSERR_NOERROR != mmhr )
	{
		Uninitialize();
		return RECORD_ERR_OPENDEVICE;
	}

	// prepare wave header
	for ( int i = 0; i < BUFNUM; ++i )
	{
		m_pWaveHDR[i].lpData = (LPSTR)m_pBuffer[i];
		m_pWaveHDR[i].dwBufferLength = BUFSIZE;
		m_pWaveHDR[i].dwUser = 0;
		m_pWaveHDR[i].dwLoops = 1;
		m_pWaveHDR[i].dwFlags = 0;

		MMRESULT mmhr1;
		mmhr = ::waveInPrepareHeader(m_hWaveIn, m_pWaveHDR + i, 
			sizeof(WAVEHDR));
		mmhr1 = ::waveInAddBuffer(m_hWaveIn, m_pWaveHDR + i, 
			sizeof(WAVEHDR));
		if ( MMSYSERR_NOERROR != mmhr || MMSYSERR_NOERROR != mmhr1 )
		{
			Uninitialize();
			return RECORD_ERR_PREPAREHEAD;
		}
	}

	m_bIsStop = FALSE;

	// 把录音线程优先级设高
	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);

	return RECORD_ERR_OK;
}

///////////////////////////////////////////////////////////
// close device
RRESULT CRecord::CloseDevice()
{
	MMRESULT mmrt;

	// stop record and reset

	m_bIsStop = TRUE;

	mmrt = ::waveInReset(m_hWaveIn);
	if ( MMSYSERR_NOERROR != mmrt )
		return RECORD_ERR_RESET;

	if ( RECORD_ERR_OK != StopRecord() )
	{
		return RECORD_ERR_CLOSEDEVICE;
	}

	// release resource
	Uninitialize();

	// close wave device
	mmrt = ::waveInClose(m_hWaveIn);
	if ( MMSYSERR_NOERROR != mmrt )
		return RECORD_ERR_CLOSEDEVICE;

	return RECORD_ERR_OK;
}

///////////////////////////////////////////////////////////
// start record
RRESULT CRecord::StartRecord()
{
	if ( NULL != m_hWaveIn )
	{
		MMRESULT mmrt;
		mmrt = ::waveInStart(m_hWaveIn);

		if ( MMSYSERR_NOERROR != mmrt )
			return RECORD_ERR_STARTRECORD;
	}
	else
	{
		return RECORD_ERR_INVALIDHANDLE;
	}

	return RECORD_ERR_OK;
}

///////////////////////////////////////////////////////////
// stop record
RRESULT CRecord::StopRecord()
{
	if ( NULL != m_hWaveIn )
	{
		MMRESULT mmrt;
		mmrt = ::waveInStop(m_hWaveIn);
		if ( MMSYSERR_NOERROR != mmrt )
			return RECORD_ERR_STOPRECORD;
	}
	else
	{
		return RECORD_ERR_INVALIDHANDLE;
	}

	return RECORD_ERR_OK;
}

///////////////////////////////////////////////////////////
// release resource
void CRecord::Uninitialize()
{
	for ( int i = 0; i < BUFNUM; ++i )
	{
		// Unpreparing a buffer that has not been prepared has no effect, and the function returns zero
		::waveInUnprepareHeader(m_hWaveIn, 
			m_pWaveHDR + i, sizeof(WAVEHDR));
	}
}

///////////////////////////////////////////////////////////
// constructor
CRecord::CRecord()
{
	m_hWaveIn = NULL;
	m_pBuffer = NULL;
	m_pWaveHDR = NULL;
}

void CRecord::OnRecordHandle(char * pData, DWORD dwSize)
{
	// do nothing
}

DWORD CRecord::ThreadProc(void)
{
	MSG msg;
	LPWAVEHDR lpwhdr;

	while ( GetMessage(&msg, NULL, 0, 0) )
    {
		switch (msg.message)
		{
		case MM_WIM_OPEN:
			break;

		case MM_WIM_DATA:
			// get recorded data
			lpwhdr = (LPWAVEHDR)msg.lParam;

			// add the buffer to devive
			if ( !m_bIsStop )
			{
				// handle pcm data
				OnRecordHandle((char *)lpwhdr->lpData, lpwhdr->dwBytesRecorded);
				::waveInAddBuffer(m_hWaveIn, lpwhdr, sizeof(WAVEHDR));
			}
			break;

		case MM_WIM_CLOSE:
			if ( m_pWaveHDR != NULL )
			{
				delete []m_pWaveHDR;
				m_pWaveHDR = NULL;
			}

			if ( m_pBuffer != NULL )
			{
				delete []m_pBuffer;
				m_pBuffer = NULL;
			}
			::PostQuitMessage(0);
			break;
		}
    }
	return 1;
}

#endif	// VA_VC