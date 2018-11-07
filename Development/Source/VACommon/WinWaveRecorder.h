#pragma once
#if defined(_WIN32)
#include "WinWaveCommon.h"

namespace VA
{
	enum 
	{ 
		VA_Wave_Record_NUM_BUFFERS = 10,  //录音缓存数量
		VA_Wave_Record_MAX_BUFFER_SIZE = 3200 //单个录音缓存大小，此设置会影响录音回调的频率
	};
	struct IWaveRecordListener
	{
		//录音线程输出数据回调
		virtual void InWaveRecordData(DWORD nID, LPBYTE pData, DWORD dwBytes) = 0;
	};

	// WaveInCaps struct
	struct WaveInCaps : WAVEINCAPS
	{
	public:
		WaveInCaps()
		{
			memset(this, 0, Size());
		}

		DWORD GetManufacturerId() const { return wMid; }
		DWORD GetProductId() const { return wPid; }
		MMVERSION GetVersion() const { return vDriverVersion; }
		DWORD GetFormats() const { return dwFormats; }
		WORD GetChannels() const { return wChannels; }
		LPCTSTR GetProductName() const { return szPname; }
		UINT Size() const { return sizeof(WAVEINCAPS); }
		operator LPWAVEINCAPS() { return static_cast<WAVEINCAPS*>(this); }
	};

	class CWaveRecorder
	{
		IWaveRecordListener* m_pListener;
		int m_nID;

		HWAVEIN             m_hWaveIn;      ///< Wave Audio Input
		long                m_lDeviceState; ///< Wave state
		UINT m_uDeviceID;

		WaveFormat m_wfmt;
		LONG            m_nNumOfBuffers;
		WaveBuffer*     m_pWaveBuffer;
		size_t          m_nBufferSize;
		HRESULT __AddBuffer( WaveBuffer& wbuf )
		{
			HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
			if ( __IsWaveOpen() )
			{
				MMRESULT mmr = waveInAddBuffer(m_hWaveIn, wbuf, wbuf.Size());
				hr = HRESULT_FROM_WIN32(mmr);
			}
			return hr;
		}
		HRESULT __PrepareBuffer( WaveBuffer& wbuf )
		{
			HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
			if ( __IsWaveOpen() )
			{
				MMRESULT mmr = waveInPrepareHeader(m_hWaveIn, wbuf, wbuf.Size());
				hr = HRESULT_FROM_WIN32(mmr);
			}
			return hr;
		}
		HRESULT __UnprepareBuffer( WaveBuffer& wbuf )
		{
			HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
			if ( __IsWaveOpen() )
			{
				MMRESULT mmr = waveInUnprepareHeader(m_hWaveIn, wbuf, wbuf.Size());
				hr = HRESULT_FROM_WIN32(mmr);
			}
			return hr;
		}
		bool __IsWaveOpen() const
		{
			return ( m_hWaveIn != NULL );
		}

		HRESULT __WaveOpen(UINT uDeviceID, const WaveFormat& wfmt)
		{
			HRESULT hr = HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
			if ( !__IsWaveOpen() )
			{
				MMRESULT mmr = waveInOpen(&m_hWaveIn, uDeviceID, const_cast<WaveFormat&>(wfmt),
					reinterpret_cast<DWORD_PTR>(waveInProc),
					reinterpret_cast<DWORD_PTR>(this),
					CALLBACK_FUNCTION);
				if ( mmr == MMSYSERR_NOERROR )
				{
					::InterlockedExchange(&m_lDeviceState, waveStopped);
				}
				hr = HRESULT_FROM_WIN32(mmr);
			}
			return hr;
		}

		HRESULT __WaveClose()
		{
			HRESULT hr = S_FALSE;
			if ( __IsWaveOpen() )
			{
				hr = __WaveStop();
				MMRESULT mmr = waveInClose(m_hWaveIn);
				if ( mmr == MMSYSERR_NOERROR )
				{
					::InterlockedExchange(&m_lDeviceState, waveClosed);
					m_hWaveIn = NULL;
				}
				hr = HRESULT_FROM_WIN32(mmr);
			}
			return hr;
		}

		HRESULT __WaveStart()
		{
			HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
			if ( __IsWaveOpen() )
			{
				MMRESULT mmr = waveInStart(m_hWaveIn);
				if ( mmr == MMSYSERR_NOERROR )
				{
					::InterlockedExchange(&m_lDeviceState, waveStarted);
				}
				hr = HRESULT_FROM_WIN32(mmr);
			}
			return hr;
		}

		HRESULT __WaveStop()
		{
			HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
			if ( __IsWaveOpen() )
			{
				MMRESULT mmr = waveInReset(m_hWaveIn);
				if ( mmr == MMSYSERR_NOERROR )
				{
					::InterlockedExchange(&m_lDeviceState, waveStopped);
					mmr = waveInStop(m_hWaveIn);
				}
				hr = HRESULT_FROM_WIN32(mmr);
			}
			return hr;
		}

		HANDLE   m_hThreadRecord;
		DWORD m_idThreadRecord;
		CRITICAL_SECTION m_cs;
		enum {
			UM_AUDIO_DATA = WM_USER + 0x100,
			UM_START,
			UM_STOP
		};
		static DWORD WINAPI RecordThreadProc(LPVOID lpParameter)
		{
			CWaveRecorder *pRecorder = reinterpret_cast<CWaveRecorder*>(lpParameter);
			return pRecorder->RecordThreadProcImpl();
		}
		DWORD RecordThreadProcImpl()
		{
			BOOL bRet;
			MSG msg;
			HRESULT hr;
			PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

			while ((bRet = GetMessage(&msg, 0, 0, 0)) != 0) {
				if (bRet == -1) {
					// handle the error and possibly exit
				} else if (UM_AUDIO_DATA == msg.message) {
					EnterCriticalSection(&m_cs);
					if (waveStarted == GetDeviceStatus())
					{
						WaveBuffer* pwbuf = reinterpret_cast<WaveBuffer*>(msg.lParam);
						WaveBuffer& curBuffer = *pwbuf;
						hr = __UnprepareBuffer(curBuffer);
						assert(S_OK == hr);
						InterlockedDecrement(&m_nNumOfBuffers);
						m_pListener->InWaveRecordData(m_nID, reinterpret_cast<LPBYTE>(curBuffer.GetBuffer()), curBuffer.GetBytesRecorded());
						hr = __PrepareBuffer(curBuffer);
						assert(S_OK == hr);
						hr = __AddBuffer(curBuffer);
						assert(S_OK == hr);
						InterlockedIncrement(&m_nNumOfBuffers);
					}
					LeaveCriticalSection(&m_cs);
				} else if (UM_START == msg.message) {
					OutputDebugString(_T("CWaveRecorder Start.\n"));
					EnterCriticalSection(&m_cs);
					m_nNumOfBuffers = 0;
					hr = __WaveOpen(m_uDeviceID, m_wfmt);
					assert(S_OK == hr);
					for (int index = 0; index < VA_Wave_Record_NUM_BUFFERS; ++index) {
						WaveBuffer& curBuffer = m_pWaveBuffer[index];
						hr = __PrepareBuffer(curBuffer);
						assert(S_OK == hr);
						hr = __AddBuffer(curBuffer);
						assert(S_OK == hr);
						::InterlockedIncrement(&m_nNumOfBuffers);						
					}
					hr = __WaveStart();
					assert(S_OK == hr);
					LeaveCriticalSection(&m_cs);
				} else if (UM_STOP == msg.message) {
					OutputDebugString(_T("CWaveRecorder Stop.\n"));
					EnterCriticalSection(&m_cs);
					hr = __WaveStop();
					assert(S_OK == hr);
					for (int index = 0; index < VA_Wave_Record_NUM_BUFFERS; ++index) {
						m_pWaveBuffer[index].SetFlags(0);
					}
					hr = __WaveClose();
					assert(S_OK == hr);
					LeaveCriticalSection(&m_cs);
				}  else {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			return 0;
		}

		static void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
		{
			CWaveRecorder* pThis = reinterpret_cast<CWaveRecorder*>(dwInstance);
			switch( uMsg )
			{
			case WIM_OPEN:
				{
					OutputDebugString( _T("WaveIn opened.\n") );
				}
				break;
			case WIM_DATA:
				{
					PostThreadMessage(pThis->m_idThreadRecord, UM_AUDIO_DATA, 0, (LPARAM)dwParam1);
				}
				break;
			case WIM_CLOSE:
				{
					OutputDebugString( _T("WaveIn closed.\n") );
				}
				break;
			}
		}

	public:
		CWaveRecorder(): m_hWaveIn(NULL), 
			m_lDeviceState(waveClosed), 
			m_pListener(NULL),
			m_nBufferSize(VA_Wave_Record_MAX_BUFFER_SIZE)
		{
			m_pWaveBuffer = new WaveBuffer[VA_Wave_Record_NUM_BUFFERS];
			m_nBufferSize = VA_Wave_Record_MAX_BUFFER_SIZE;
			for( int index=0; index < VA_Wave_Record_NUM_BUFFERS; ++index)
			{
				// realloc memory
				bool allocOK = m_pWaveBuffer[index].Allocate(static_cast<DWORD>(m_nBufferSize));
				assert(allocOK);
			}
			InitializeCriticalSection(&m_cs);
			m_hThreadRecord = CreateThread(NULL, 0, RecordThreadProc, (LPVOID) this, NULL, &m_idThreadRecord);
			
		}
		~CWaveRecorder()
		{
			__WaveClose();
			PostThreadMessage(m_idThreadRecord, WM_QUIT, 0, 0);
			WaitForSingleObject(m_hThreadRecord, INFINITE);
			CloseHandle(m_hThreadRecord);
			if (m_pWaveBuffer) {
				delete[] m_pWaveBuffer;
			}
			DeleteCriticalSection(&m_cs);
		}

		void Open(IWaveRecordListener* pListener, UINT uDeviceID, LPWAVEFORMATEX pwfx, DWORD nID = 0)
		{
			assert(pListener);
			assert(pwfx);
			InterlockedExchangePointer(reinterpret_cast<void**>(&m_pListener), pListener);
			m_wfmt = pwfx;
			m_nID = nID;
			m_uDeviceID = uDeviceID;
		}
		void OpenPCM(IWaveRecordListener* pListener, UINT uDeviceID = WAVE_MAPPER, DWORD nSamplesPerSec = 16000, WORD nChannels = 1, WORD wBitsPerSample = 16, DWORD nID = 0)
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
		void OpenPCMDefault(IWaveRecordListener* pListener, DWORD nID = 0)
		{
			return OpenPCM(pListener, WAVE_MAPPER, 16000, 1, 16, nID);
		}

		void Start() {
			EnterCriticalSection(&m_cs);
			if(waveStarted != GetDeviceStatus())
				PostThreadMessage(m_idThreadRecord, UM_START, 0, 0);
			LeaveCriticalSection(&m_cs);
		}

		void Stop() {
			EnterCriticalSection(&m_cs);
			if(waveStarted == GetDeviceStatus())
				PostThreadMessage(m_idThreadRecord, UM_STOP, 0, 0);
			LeaveCriticalSection(&m_cs);
		}

		WaveStatus GetDeviceStatus() const
		{
			WaveStatus wStatus;
			wStatus = static_cast<WaveStatus>(::InterlockedCompareExchange(const_cast<LONG*>(&m_lDeviceState), waveClosed, waveClosed));
			return wStatus;
		}

		HRESULT GetPosition(WaveTime& wti) const
		{
			HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
			if ( __IsWaveOpen() )
			{
				MMRESULT mmr = waveInGetPosition(m_hWaveIn, wti, wti.Size());
				hr = HRESULT_FROM_WIN32(mmr);
			}
			return hr;
		}

		UINT GetDevicesCount()
		{
			return waveInGetNumDevs();
		}

		HRESULT GetDeviceCaps(UINT uDeviceID, WaveInCaps& wic)
		{
			HRESULT hr;
			MMRESULT mmr = waveInGetDevCaps(uDeviceID, wic, wic.Size());
			hr = HRESULT_FROM_WIN32(mmr);
			return hr;
		}

	};
}
#endif