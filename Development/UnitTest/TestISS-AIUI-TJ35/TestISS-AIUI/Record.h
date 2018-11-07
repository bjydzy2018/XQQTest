/******************************************************************************
* FileName		       : Record.h
* Description          : Record head file
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

#ifndef _RECORD_H_2005_11_05_INCLUDE_
#define _RECORD_H_2005_11_05_INCLUDE_

#if _MSC_VER > 1000
#pragma once
#endif

#include "VAInc.h"
#if VA_VC

#include <windows.h>
#include <mmsystem.h>
#include "Thread.h"

#define ESR_SAMPLE_RATE (16000)
#define BUFSIZE	(3200)	// buffer size
#define BUFNUM	10		// buffer number

typedef unsigned int RRESULT;
typedef byte BUFFER[BUFSIZE];

// record class define
class CRecord : protected CThread
{
public:
	CRecord();

	/*
	 *	exterior interface function
	 */
public:
	RRESULT OpenDevice(const WAVEFORMATEX* pwf = NULL);
	RRESULT CloseDevice();
	RRESULT StartRecord();
	RRESULT StopRecord();

protected:
	virtual void OnRecordHandle(char* pData, DWORD dwSize);
	virtual DWORD ThreadProc(void);

	/*
	 *	interior member function
	 */
private:
	void Uninitialize();

	/*
	 *	interior member variable
	 */
private:
	HWAVEIN m_hWaveIn;			// record device handle
	BUFFER * m_pBuffer;			// buffer
	PWAVEHDR m_pWaveHDR;		// wave header
	BOOL m_bIsStop;
};

// record error define
#define RECORD_ERR_BASE				0x0100
#define RECORD_ERR_OK				(RECORD_ERR_BASE + 0x00)
#define RECORD_ERR_CREATEBUF		(RECORD_ERR_BASE + 0x01)
#define RECORD_ERR_CREATEHDR		(RECORD_ERR_BASE + 0x02)
#define RECORD_ERR_CREATETHREAD		(RECORD_ERR_BASE + 0x03)
#define RECORD_ERR_OPENDEVICE		(RECORD_ERR_BASE + 0x04)
#define RECORD_ERR_PREPAREHEAD		(RECORD_ERR_BASE + 0x05)
#define RECORD_ERR_STOPDEVICE		(RECORD_ERR_BASE + 0x06)
#define RECORD_ERR_CLOSEDEVICE		(RECORD_ERR_BASE + 0x07)
#define RECORD_ERR_INVALIDHANDLE	(RECORD_ERR_BASE + 0x08)
#define RECORD_ERR_STARTRECORD		(RECORD_ERR_BASE + 0x09)
#define RECORD_ERR_STOPRECORD		(RECORD_ERR_BASE + 0x0A)
#define RECORD_ERR_RESET			(RECORD_ERR_BASE + 0x0B)

#endif	// VA_VC
#endif	// _RECORD_H_2005_11_5_INCLUDE_
