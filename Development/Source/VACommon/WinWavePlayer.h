#pragma once
#if defined(_WIN32)
#include "WinWaveCommon.h"

namespace VA
{
	enum 
	{ 
		VA_Wave_Play_NUM_BUFFERS = 10,  //播音缓存数量
		VA_Wave_Play_MAX_BUFFER_SIZE = 3200 //单个播音缓存大小，此设置会影响录音回调的频率
	};

	struct IWavePlayListener
	{
		//播音线程获取数据回调，返回值表示实际取得数据，返回0xFFFFFFFF(-1)表示数据已取完
		virtual DWORD OnWavePlayGetData(DWORD nID, LPBYTE pBuff, DWORD dwBytes/*希望获取的数据大小*/) = 0;
		//播放结束回调
		virtual void OnWavePlayCompleted(DWORD nID) = 0;
	};

	// WaveOutCaps struct
	struct WaveOutCaps : WAVEOUTCAPS
	{
	public:
		WaveOutCaps()
		{
			memset(this, 0, Size());
		}

		DWORD GetManufacturerId() const { return wMid; }
		DWORD GetProductId() const { return wPid; }
		MMVERSION GetVersion() const { return vDriverVersion; }
		DWORD GetFormats() const { return dwFormats; }
		WORD GetChannels() const { return wChannels; }
		LPCTSTR GetProductName() { return szPname; }
		UINT Size() const { return sizeof(WAVEOUTCAPS); }
		operator LPWAVEOUTCAPS() { return static_cast<WAVEOUTCAPS*>(this); }
	};

	class CWavePlayer
	{
		IWavePlayListener* m_pListener;
		int m_nID;

		HWAVEOUT             m_hWaveOut;      ///< Wave Audio Input
		long                m_lDeviceState; ///< Wave state
		UINT m_uDeviceID;

		WaveFormat m_wfmt;
		LONG            m_nNumOfBuffers;
		WaveBuffer*     m_pWaveBuffer;
		size_t          m_nBufferSize;
		bool m_bDataEnd;

// 		void PutAudioData(LPBYTE pbData, size_t cbSize, size_t recvBytes, const WaveFormat& wfmt)
// 		{
// 			switch( wfmt.wBitsPerSample )
// 			{
// 			case 8:
// 				{
// 					// fill with silence - no smoothing
// 					if ( cbSize > recvBytes )
// 					{
// 						memset(pbData+recvBytes, 0x80, cbSize-recvBytes);
// 					}
// 				}
// 				break;
// 			case 16:
// 			default:
// 				{
// 					// fill with silence - no smoothing
// 					if ( cbSize > recvBytes )
// 					{
// 						memset(pbData+recvBytes, 0x00, cbSize-recvBytes);
// 					}
// 				}
// 				break;
// 			}
// 		}

		MMRESULT __AddBuffer( WaveBuffer& wbuf )
		{
			MMRESULT mmr = MMSYSERR_NOERROR;
			if ( __IsWaveOpen() )
			{
				mmr = waveOutWrite(m_hWaveOut, wbuf, wbuf.Size());
			}
			return mmr;
		}
		MMRESULT __PrepareBuffer( WaveBuffer& wbuf )
		{
			MMRESULT mmr = MMSYSERR_NOERROR;
			if ( __IsWaveOpen() )
			{
				mmr = waveOutPrepareHeader(m_hWaveOut, wbuf, wbuf.Size());
			}
			return mmr;
		}
		MMRESULT __UnprepareBuffer( WaveBuffer& wbuf )
		{
			MMRESULT mmr = MMSYSERR_NOERROR;
			if ( __IsWaveOpen() )
			{
				mmr = waveOutUnprepareHeader(m_hWaveOut, wbuf, wbuf.Size());
			}
			return mmr;
		}
		bool __IsWaveOpen() const
		{
			return ( m_hWaveOut != NULL );
		}
		MMRESULT __WaveOpen(UINT uDeviceID,	const WaveFormat& wfmt)
		{
			MMRESULT mmr = MMSYSERR_NOERROR;
			if ( !__IsWaveOpen() )
			{
				mmr = waveOutOpen(&m_hWaveOut, uDeviceID, const_cast<WaveFormat&>(wfmt),
					reinterpret_cast<DWORD_PTR>(waveOutProc),
					reinterpret_cast<DWORD_PTR>(this),
					CALLBACK_FUNCTION);
				if ( mmr == MMSYSERR_NOERROR )
				{
					::InterlockedExchange(&m_lDeviceState, waveStopped);
				}
			}
			return mmr;
		}
		MMRESULT __WaveClose()
		{
			MMRESULT mmr = MMSYSERR_NOERROR;
			if ( __IsWaveOpen() )
			{
				mmr = __WaveStop();
				mmr = waveOutClose(m_hWaveOut);
				if ( mmr == MMSYSERR_NOERROR )
				{
					::InterlockedExchange(&m_lDeviceState, waveClosed);
					m_hWaveOut = NULL;
				}
			}
			return mmr;
		}
		MMRESULT __WaveStart()
		{
			MMRESULT mmr = MMSYSERR_NOERROR;
			if ( __IsWaveOpen() )
			{
				mmr = waveOutRestart(m_hWaveOut);
				if ( mmr == MMSYSERR_NOERROR )
				{
					::InterlockedExchange(&m_lDeviceState, waveStarted);
				}
			}
			return mmr;
		}
		MMRESULT __WaveStop()
		{
			MMRESULT mmr = MMSYSERR_NOERROR;
			if ( __IsWaveOpen() )
			{
				mmr = waveOutReset(m_hWaveOut);
				if ( mmr == MMSYSERR_NOERROR )
				{
					::InterlockedExchange(&m_lDeviceState, waveStopped);
					mmr = waveOutPause(m_hWaveOut);
				}
			}
			return mmr;
		}
		MMRESULT __WavePause()
		{
			MMRESULT mmr = MMSYSERR_NOERROR;
			if ( __IsWaveOpen() )
			{
				mmr = waveOutPause(m_hWaveOut);
				if ( mmr == MMSYSERR_NOERROR )
				{
					::InterlockedExchange(&m_lDeviceState, wavePaused);
				}
			}
			return mmr;
		}

		HANDLE   m_hThreadPlay;
		DWORD m_idThreadPlay;
		CRITICAL_SECTION m_cs;
		enum {
			UM_AUDIO_DONE = WM_USER + 0x100,
			UM_START,
			UM_STOP,
			UM_PAUSE,
			UM_RESUME
		};
		static DWORD WINAPI PlayThreadProc(LPVOID lpParameter)
		{
			CWavePlayer *pPlayer = reinterpret_cast<CWavePlayer*>(lpParameter);
			return pPlayer->PlayThreadProcImpl();
		}
		DWORD PlayThreadProcImpl()
		{
			BOOL bRet;
			MSG msg;
			MMRESULT mmr;
			DWORD dwBytes;
			PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

			while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
				if (bRet == -1) {
					// handle the error and possibly exit
				} else if (UM_AUDIO_DONE == msg.message) {
					EnterCriticalSection(&m_cs);
					if(GetDeviceStatus() == waveStarted)
					{
						WaveBuffer* pwbuf = reinterpret_cast<WaveBuffer*>(msg.lParam);
						WaveBuffer& curBuffer = *pwbuf;
						mmr = __UnprepareBuffer(curBuffer);
						assert(MMSYSERR_NOERROR == mmr);
						InterlockedDecrement(&m_nNumOfBuffers);
						if (m_bDataEnd && m_nNumOfBuffers == 0) {  // all buffers done
							// stop
							//OutputDebugString(_T("PCMPlayer Play Done.\n"));
							PostThreadMessage(m_idThreadPlay, UM_STOP, 0, 0);
						}
						else
						{
ToDoGetGetData:
							dwBytes = m_nBufferSize;
							dwBytes = m_pListener->OnWavePlayGetData(m_nID, reinterpret_cast<LPBYTE>(curBuffer.GetBuffer()), dwBytes);
							if (-1 == dwBytes)
							{
								m_bDataEnd = true;
							}
							else if (0 == dwBytes && !m_bDataEnd)
							{
								Sleep(1);
								goto ToDoGetGetData;
							}
							else
							{
								if (!m_bDataEnd) {
									// Append silence if necessary
									//PutAudioData(reinterpret_cast<LPBYTE>(curBuffer.GetBuffer()), m_nBufferSize, dwBytes, m_wfmt);
									curBuffer.SetBufferLength(dwBytes);
									mmr = __PrepareBuffer(curBuffer);
									assert(MMSYSERR_NOERROR == mmr);
									mmr = __AddBuffer(curBuffer);
									assert(MMSYSERR_NOERROR == mmr);
									InterlockedIncrement(&m_nNumOfBuffers);
								}
							}
						}
					}					
					LeaveCriticalSection(&m_cs);
				} else if (UM_START == msg.message) {
					//OutputDebugString(_T("PCMPlayer Play Start.\n"));
					EnterCriticalSection(&m_cs);
					m_nNumOfBuffers = 0;
					m_bDataEnd = false;
					mmr = __WaveOpen(m_uDeviceID, m_wfmt);
					assert(MMSYSERR_NOERROR == mmr);
					for (int index = 0; index < VA_Wave_Play_NUM_BUFFERS; ++index) {
						WaveBuffer& curBuffer = m_pWaveBuffer[index];
						dwBytes = m_nBufferSize;
						dwBytes = m_pListener->OnWavePlayGetData(m_nID, reinterpret_cast<LPBYTE>(curBuffer.GetBuffer()), dwBytes);
						if (-1 == dwBytes) {
							m_bDataEnd = true;
							break;//for
						} 
						else if (0 == dwBytes)
						{
							Sleep(1);
							index--;
							continue;
						}
						else
						{
							curBuffer.SetBufferLength(dwBytes);
							mmr = __PrepareBuffer(curBuffer);
							assert(MMSYSERR_NOERROR == mmr);
							mmr = __AddBuffer(curBuffer);
							assert(MMSYSERR_NOERROR == mmr);
							::InterlockedIncrement(&m_nNumOfBuffers);
						}						
					}
					mmr = __WaveStart();
					assert(MMSYSERR_NOERROR == mmr);
					LeaveCriticalSection(&m_cs);
				} else if (UM_STOP == msg.message) {
					//OutputDebugString(_T("PCMPlayer Play Stop.\n"));
					EnterCriticalSection(&m_cs);
					__WaveClose();
					for (int index = 0; index < VA_Wave_Play_NUM_BUFFERS; ++index) {
						m_pWaveBuffer[index].SetFlags(0);
					}
					m_pListener->OnWavePlayCompleted(m_nID);
					LeaveCriticalSection(&m_cs);
				}  else if (UM_PAUSE == msg.message) {
					//OutputDebugString(_T("PCMPlayer Play Pause.\n"));
					EnterCriticalSection(&m_cs);
					__WavePause();
					LeaveCriticalSection(&m_cs);
				} else if (UM_RESUME == msg.message) {
					//OutputDebugString(_T("PCMPlayer Play Resume.\n"));
					EnterCriticalSection(&m_cs);
					__WaveStart();
					LeaveCriticalSection(&m_cs);
				} else {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			return 0;
		}

		static void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD dwParam1, DWORD dwParam2)
		{
			CWavePlayer* pThis = reinterpret_cast<CWavePlayer*>( dwInstance );
			switch( uMsg )
			{
			case WOM_OPEN:
				{
					//OutputDebugString( _T("WaveOut opened.\n") );
				}
				break;
			case WOM_DONE:
				{
					PostThreadMessage(pThis->m_idThreadPlay, UM_AUDIO_DONE, 0, (LPARAM)dwParam1);
				}
				break;
			case WOM_CLOSE:
				{
					//OutputDebugString( _T("WaveOut closed.\n") );
				}
				break;
			}
		}

	public:
		CWavePlayer():m_hWaveOut(NULL), 
			m_lDeviceState(waveClosed), 
			m_pListener(NULL),
			m_nBufferSize(VA_Wave_Play_MAX_BUFFER_SIZE),
			m_bDataEnd(false)
		{
			m_pWaveBuffer = new WaveBuffer[VA_Wave_Play_NUM_BUFFERS];
			m_nBufferSize = VA_Wave_Play_MAX_BUFFER_SIZE;
			for( int index=0; index < VA_Wave_Play_NUM_BUFFERS; ++index)
			{
				// realloc memory
				bool allocOK = m_pWaveBuffer[index].Allocate(static_cast<DWORD>(m_nBufferSize));
				assert(allocOK);
			}
			InitializeCriticalSection(&m_cs);
			m_hThreadPlay = CreateThread(NULL, 0, PlayThreadProc, (LPVOID) this, NULL, &m_idThreadPlay);
		}
		~CWavePlayer()
		{
			__WaveClose();
			PostThreadMessage(m_idThreadPlay, WM_QUIT, 0, 0);
			WaitForSingleObject(m_hThreadPlay, INFINITE);
			CloseHandle(m_hThreadPlay);
			if (m_pWaveBuffer) {
				delete[] m_pWaveBuffer;
			}
			DeleteCriticalSection(&m_cs);
		}

		WaveStatus GetDeviceStatus() const
		{
			WaveStatus wStatus;
			wStatus = static_cast<WaveStatus>( 
				::InterlockedCompareExchange(const_cast<LONG*>(&m_lDeviceState), waveClosed, waveClosed)
				);
			return wStatus;
		}

		void Open(IWavePlayListener* pListener, UINT uDeviceID, LPWAVEFORMATEX pwfx, DWORD nID = 0)
		{
			assert(pListener);
			assert(pwfx);
			InterlockedExchangePointer(reinterpret_cast<void**>(&m_pListener), pListener);
			m_wfmt = pwfx;
			m_nID = nID;
			m_uDeviceID = uDeviceID;
		}
		void OpenPCM(IWavePlayListener* pListener, UINT uDeviceID, DWORD nSamplesPerSec = 16000, WORD nChannels = 1, WORD wBitsPerSample = 16, DWORD nID = 0)
		{
			WAVEFORMATEX wfx;
			wfx.wFormatTag = WAVE_FORMAT_PCM;
			wfx.cbSize = 0;
			wfx.nSamplesPerSec = nSamplesPerSec;
			wfx.nChannels = nChannels;
			wfx.wBitsPerSample = wBitsPerSample;
			wfx.nBlockAlign = (wBitsPerSample + 7) / 8 * nChannels;
			wfx.nAvgBytesPerSec = wfx.nBlockAlign * nSamplesPerSec;
			return Open(pListener, uDeviceID, &wfx, nID);
		}
		void OpenPCMDefault(IWavePlayListener* pListener, DWORD nID = 0)
		{
			return OpenPCM(pListener, WAVE_MAPPER, 16000, 1, 16, nID);
		}

		void Start() {
			EnterCriticalSection(&m_cs);
			if(waveClosed == GetDeviceStatus())
				PostThreadMessage(m_idThreadPlay, UM_START, 0, 0);
			LeaveCriticalSection(&m_cs);
		}

		void Stop() {
			EnterCriticalSection(&m_cs);
			if(waveStarted == GetDeviceStatus() || wavePaused == GetDeviceStatus())
				PostThreadMessage(m_idThreadPlay, UM_STOP, 0, 0);
			LeaveCriticalSection(&m_cs);
		}

		void Pause() {
			EnterCriticalSection(&m_cs);
			if(waveStarted == GetDeviceStatus())
				PostThreadMessage(m_idThreadPlay, UM_PAUSE, 0, 0);
			LeaveCriticalSection(&m_cs);
		}

		void Resume() {
			EnterCriticalSection(&m_cs);
			if(wavePaused == GetDeviceStatus())
				PostThreadMessage(m_idThreadPlay, UM_RESUME, 0, 0);
			LeaveCriticalSection(&m_cs);
		}

		MMRESULT GetPosition(WaveTime& wti) const
		{
			MMRESULT mmr;
			if ( __IsWaveOpen() )
			{
				mmr = waveOutGetPosition(m_hWaveOut, wti, wti.Size());
			}
			return mmr;
		}

		UINT GetDevicesCount()
		{
			return waveOutGetNumDevs();
		}

		MMRESULT GetDeviceCaps(UINT uDeviceID, WaveOutCaps& woc)
		{
			MMRESULT mmr = waveOutGetDevCaps(uDeviceID, woc, woc.Size());
			return mmr;
		}
	};

	struct IPlaySoundListener {
		virtual DWORD InPlaySoundRead(DWORD nID, LPBYTE pBuff, DWORD dwBytes) 
		{
			assert(false);
			return 0;
		}
		virtual void OnPlaySoundCompleted() = 0;
	};

	class CPlaySound : IWavePlayListener {
		IPlaySoundListener* m_pListener;
		DWORD m_nID;
		bool m_bPlaying;
		LPBYTE m_pData;
		DWORD m_cbSize;
		WaveFormat wfmt;

		CWavePlayer m_Player;

		//播音线程获取数据回调，返回值表示实际取得数据，返回0xFFFFFFFF(-1)表示数据已取完
		DWORD OnWavePlayGetData(DWORD nID, LPBYTE pBuff, DWORD dwBytes/*希望获取的数据大小*/)
		{
			if (m_cbSize)
			{
				int n = min(m_cbSize, dwBytes);
				void* p = alloca(n);
				n = __Read(pBuff, n);
				m_cbSize -= n;
				return n;
			}
			return 0xFFFFFFFF;
		}
		//播放结束回调
		void OnWavePlayCompleted(DWORD nID)
		{
			m_bPlaying = false;
			if (m_pListener)
				m_pListener->OnPlaySoundCompleted();
		}

		DWORD __Read(LPBYTE pBuffer, DWORD nSize)
		{
			if (m_pData) {
				CopyMemory(pBuffer, m_pData, nSize);
				m_pData += nSize;
				return nSize;
			} else {
				assert(m_pListener);
				return m_pListener->InPlaySoundRead(m_nID, pBuffer, nSize);
			}
		}
		UINT32 __ReadU32()
		{
			UINT32 v;
			__Read((LPBYTE)&v, 4);
			return v;
		}

	public:
		CPlaySound(IPlaySoundListener* pListener = NULL, DWORD nID = 0)
		{
			m_pListener = pListener;
			m_nID = nID;
			m_pData = NULL;
			m_bPlaying = false;
		}
		~CPlaySound()
		{
			Stop();
		}

		bool Start()
		{
			Stop();
			m_bPlaying = true;
			if (__ReadU32() != *(UINT32*) "RIFF")
				return false;
			unsigned cbSize = __ReadU32();
			if (__ReadU32() != *(UINT32*) "WAVE")
				return false;
			LPWAVEFORMATEX pwfx = NULL;
			m_cbSize = 0;
			for (;;) {
				unsigned tag = __ReadU32();
				cbSize = __ReadU32();
				if (tag == *(UINT32*) "fmt ") {
					pwfx = (LPWAVEFORMATEX) alloca(cbSize);
					__Read((LPBYTE)pwfx, cbSize);
					wfmt = *pwfx;
				} else if (tag == *(unsigned*) "data") {
					if (!pwfx || !cbSize)
						return false;
					m_cbSize = cbSize;
					break;
				} else {
					BYTE buf[32];
					while (cbSize > 32) {
						__Read(buf, 32);
						cbSize -= 32;
					}
					__Read(buf, cbSize);
				}
			}
			m_Player.Open(this, WAVE_MAPPER, wfmt, m_nID);
			m_Player.Start();
			return true;
		}

		bool Start(const LPVOID pData)
		{
			Stop();
			m_pData = (const LPBYTE) pData;
			return Start();
		}

		void Pause()
		{
			if (m_bPlaying)
				m_Player.Pause();
		}
		void Resume()
		{
			if (m_bPlaying)
				m_Player.Resume();
		}
		void Stop()
		{
			if (m_bPlaying)
			{
				m_Player.Stop();
				m_pData = NULL;
				m_bPlaying = false;
			}			
		}

		bool IsEnd() {
			return m_bPlaying;
		}
	};
}
#endif