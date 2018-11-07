#pragma once
#if defined(_WIN32)

#include <winbase.h>
#include <mmreg.h>
#include <mmsystem.h>
#include <assert.h>

#ifdef WINCE
#pragma comment(lib, "coredll.lib")
#else
#pragma comment(lib, "winmm.lib")
#endif // WINCE

namespace VA
{
	// WaveBuffer class
	struct WaveBuffer : WAVEHDR
	{
	public:
		WaveBuffer()
		{
			memset(this, 0, Size());
		}
		~WaveBuffer()
		{
			FreeMemory();
		}
		LPSTR GetBuffer() const { return lpData; }
		DWORD GetBufferLength() const { return dwBufferLength; }
		void SetBufferLength(DWORD bufferLength) { dwBufferLength = bufferLength; }
		DWORD GetBytesRecorded() const { return dwBytesRecorded; }
		void SetBytesRecorded(DWORD bytesRecorded) { dwBytesRecorded = bytesRecorded; }
		DWORD GetFlags() const { return dwFlags; }
		void SetFlags(DWORD flags) { dwFlags = flags; }
		DWORD GetLoops() const { return dwLoops; }
		void SetLoops(DWORD loops) { dwLoops = loops; }
		bool IsDone() const { return (dwFlags&WHDR_DONE)!=0; }
		bool IsPrepared() const { return (dwFlags&WHDR_PREPARED) != 0; }
		WaveBuffer* GetNext() { reinterpret_cast<WaveBuffer*>( lpNext ); }

		inline bool Allocate(DWORD size){
			if ( lpData != 0 )
			{
				lpData = static_cast<LPSTR>( HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, lpData, size) );
			}
			else
			{
				lpData = static_cast<LPSTR>( HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size) );
			}
			if ( lpData != NULL ) dwBufferLength = size;
			return ( lpData != NULL );
		}
		inline void FreeMemory(){
			if ( lpData != NULL ) {
				HeapFree(GetProcessHeap(), 0, lpData);
				lpData = NULL; 
			}
		}

		UINT Size() const { return sizeof(WAVEHDR); }
		operator LPWAVEHDR() { return static_cast<WAVEHDR*>(this); }

	private:
		WaveBuffer(const WAVEHDR& whdr);    // not allowed
		WaveBuffer(const WaveBuffer& wbuf); // not allowed
	};

	// WaveFormat class
	struct WaveFormat : WAVEFORMATEX
	{
	public:
		WaveFormat()
		{
			memset(this, 0, Size());
		}
		WaveFormat(const WAVEFORMATEX& wfmt)
		{
			memcpy(this, &wfmt, Size());
		}
		WaveFormat(LPWAVEFORMATEX wfmt)
		{
			memcpy(this, wfmt, Size());
		}
		UINT Size() const { return sizeof(WAVEFORMATEX); }
		operator LPWAVEFORMATEX() { return static_cast<WAVEFORMATEX*>(this); }
	};


	struct WaveFormatExtensible : WAVEFORMATEXTENSIBLE
	{
	public:
		WaveFormatExtensible()
		{
			memset(this, 0, Size());
		}
		WaveFormatExtensible(const WAVEFORMATEXTENSIBLE& wfmt)
		{
			memcpy(this, &wfmt, Size());
		}
		UINT Size() const { return sizeof(WAVEFORMATEXTENSIBLE); }
		operator LPWAVEFORMATEX() { return static_cast<WAVEFORMATEX*>(&Format); }
		operator PWAVEFORMATEXTENSIBLE() { return static_cast<WAVEFORMATEXTENSIBLE*>(this); }
	};


	// Wave timing information
	struct WaveTime : MMTIME
	{
	public:
		WaveTime()
		{
			memset(this, 0, Size());
		}
		WaveTime(const MMTIME& mmt)
		{
			memcpy(this, &mmt, Size());
		}
		DWORD GetType() const { return wType; }
		void SetType(DWORD type) { wType = type; }
		DWORD GetTimeInfo() const { return u.ms; }
		// time info can be an smpte struct, just cast it to DWORD
		void SetTimeInfo(DWORD info) { u.ms = info; }
		UINT Size() const { return sizeof(MMTIME); }
		operator LPMMTIME() { return static_cast<MMTIME*>(this); }
	};

	enum WaveStatus
	{
		waveClosed = 0,  ///< Closed
		waveStopped,     ///< Stopped
		waveStarted,     ///< Started
		wavePaused       ///< Paused
	};
}

#endif