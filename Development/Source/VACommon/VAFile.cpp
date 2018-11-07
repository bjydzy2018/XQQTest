#include "VAPlatform.h"

#include <stdio.h>
#include <wchar.h>
#include <time.h>
#include <stdarg.h>
#include <locale.h>
#include <algorithm>

/* Windows include */
#ifdef VA_VC
# include <Windows.h>
# include <WinCon.h>
# include <share.h>
# include <io.h>
#  if _MSC_VER >= 1400
#   pragma warning(disable : 4996)
#  endif /* VC8 */
#  if _MSC_VER < 1300
#   pragma warning(disable : 4786)
#   include <STDDEF.H>
typedef long intptr_t;
#  endif /* VC6 */
#  pragma comment(lib, "version.lib")
#else /* Linux include */
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>
# include <errno.h>
# include <unistd.h>
# include <sys/time.h>
#endif // if WIN32

#include "VAFileSystem.h"
#include "VAString.h"
#if defined(ACTIVE_PATH) && defined(VA_GNUC)
#include <sys/stat.h>
#include <dirent.h>
#else
#include <io.h>
#endif

#include "./Path/Path.h"

#include "VAFile.h"

#define VA_MAX_PATH          (260)

namespace VA {

#ifdef VA_VC

#include <algorithm>
#include <limits>
#include <cmath>

_SYS_HANDLE_TYPE VAFile::Create(const std::string& str,
    int accessFlags,
    int creationFlags)
{
    // If the truncate bit is on AND the file does exist,
    // we need to set the mode to TRUNCATE_EXISTING
    if ((creationFlags & VAFile::TRUNCATE) && IsFileExist(str.c_str()) )
    {
        creationFlags = TRUNCATE_EXISTING;
    }
    else
    {
        creationFlags = ~VAFile::TRUNCATE & creationFlags;
    }

    mHandle = CreateFileA(str.c_str(),
        accessFlags,
        FILE_SHARE_READ, NULL,
        creationFlags,
        FILE_ATTRIBUTE_NORMAL, NULL);

    if (mHandle == SYS_INVALID_HANDLE)
    {
        //throw sys::SystemException(Ctxt(FmtX("Error opening file: [%s]", str.c_str())));
    }
    mPath = str;

    return mHandle;
}

void VAFile::ReadInto(char *buffer, Size_T size)
{
    static const DWORD MAX_READ_SIZE = (std::numeric_limits<DWORD>::max)();
    size_t bytesRead = 0;
    size_t bytesRemaining = size;

    while (bytesRead < size)
    {
        // Determine how many bytes to read
        const DWORD bytesToRead =
            std::min<DWORD>(MAX_READ_SIZE, bytesRemaining);

        // Read from file
        DWORD bytesThisRead = 0;
        if (!ReadFile(mHandle,
            buffer + bytesRead,
            bytesToRead,
            &bytesThisRead,
            NULL))
        {
            /*throw sys::SystemException(Ctxt("Error reading from file"));*/
            break;
        }
        else if (bytesThisRead == 0)
        {
            //! ReadFile does not fail when finding the EOF --
            //  instead it reports 0 bytes read, so this stops an infinite loop
            //  from Unexpected EOF
            //throw sys::SystemException(Ctxt("Unexpected end of file"));
            break;
        }

        bytesRead += bytesThisRead;
        bytesRemaining -= bytesThisRead;
    }
}

void VAFile::WriteFrom(const char *buffer, Size_T size)
{
    static const DWORD MAX_WRITE_SIZE = (std::numeric_limits<DWORD>::max)();
    size_t bytesRemaining = size;
    size_t bytesWritten = 0;

    while (bytesWritten < size)
    {
        // Determine how many bytes to write
        const DWORD bytesToWrite =
            std::min<DWORD>(MAX_WRITE_SIZE, bytesRemaining);

        // Write the data
        DWORD bytesThisWrite = 0;
        if (!WriteFile(mHandle,
            buffer + bytesWritten,
            bytesToWrite,
            &bytesThisWrite,
            NULL))
        {
            //throw sys::SystemException(Ctxt("Writing from file"));
            break;
        }

        // Accumulate this write until we are done
        bytesRemaining -= bytesThisWrite;
        bytesWritten += bytesThisWrite;
    }
}

VAFile::Off_T VAFile::SeekTo(Off_T offset, int whence)
{
    /* Ahhh!!! */
    LARGE_INTEGER largeInt;
    LARGE_INTEGER toWhere;
    largeInt.QuadPart = offset;
    if (!SetFilePointerEx(mHandle, largeInt, &toWhere, whence))
    {
        //throw sys::SystemException(Ctxt("SetFilePointer failed"));
        return -1;
    }

    return (Off_T) toWhere.QuadPart;
}

VAFile::Off_T VAFile::Length()
{
    DWORD highOff;
    DWORD ret = GetFileSize(mHandle, &highOff);
    Uint64_T off = highOff;
    return (Off_T)(off << 32) + ret;
}

VAFile::Off_T VAFile::LastModifiedTime()
{
    FILETIME creationTime, lastAccessTime, lastWriteTime;
    BOOL ret = GetFileTime(mHandle, &creationTime,
        &lastAccessTime, &lastWriteTime);
    if (ret)
    {
        ULARGE_INTEGER uli;
        uli.LowPart = lastWriteTime.dwLowDateTime;
        uli.HighPart = lastWriteTime.dwHighDateTime;
        ULONGLONG stInMillis(uli.QuadPart/10000);
        return (Off_T)stInMillis;
    }

    return -1;
//     throw sys::SystemException(Ctxt(
//         FmtX("Error getting last modified time for path %s",
//         mPath.c_str())));
}

void VAFile::Flush()
{
    if (!FlushFileBuffers(mHandle))
    {
        
    }
}

void VAFile::Close()
{
    CloseHandle(mHandle);
    mHandle = SYS_INVALID_HANDLE;
}
#else
#include <unistd.h>
#include <string.h>
#include <errno.h>

_SYS_HANDLE_TYPE VAFile::Create(const std::string& str, int accessFlags,
                       int creationFlags)
{

    if (accessFlags & VAFile::WRITE_ONLY)
        creationFlags |= VAFile::TRUNCATE;
    mHandle = open(str.c_str(), accessFlags | creationFlags, _SYS_DEFAULT_PERM);

    if (mHandle < 0)
    {
        
    }
    mPath = str;

    return mHandle;
}

void VAFile::ReadInto(char *buffer, Size_T size)
{
    SSize_T bytesRead = 0;
    Size_T totalBytesRead = 0;
    int i;

    /* make sure the user actually wants data */
    if (size <= 0)
        return;

    for (i = 1; i <= _SYS_MAX_READ_ATTEMPTS; i++)
    {
        bytesRead = ::read(mHandle, buffer + totalBytesRead, size
            - totalBytesRead);

        switch (bytesRead)
        {
        case -1: /* Some type of error occured */
            switch (errno)
            {
            case EINTR:
            case EAGAIN: /* A non-fatal error occured, keep trying */
                break;

            default: /* We failed */
                break;
            }
            break;

        case 0: /* EOF (unexpected) */
            break;

        default: /* We made progress */
            totalBytesRead += bytesRead;

        } /* End of switch */

        /* Check for success */
        if (totalBytesRead == size)
        {
            return;
        }
    }
}




void VAFile::WriteFrom(const char *buffer, Size_T size)
{
    Size_T bytesActuallyWritten = 0;

    do
    {
        const SSize_T bytesThisRead = ::write(mHandle,
            buffer + bytesActuallyWritten,
            size - bytesActuallyWritten);
        if (bytesThisRead == -1)
        {
            break;
        }
        bytesActuallyWritten += bytesThisRead;
    }
    while (bytesActuallyWritten < size);
}

VAFile::Off_T VAFile::SeekTo(VAFile::Off_T offset, int whence)
{
    VAFile::Off_T off = ::lseek(mHandle, offset, whence);
    if (off == (VAFile::Off_T) - 1)
    {
        //throw sys::SystemException(Ctxt("Seeking in file"));
    }
    return off;
}

VAFile::Off_T VAFile::Length()
{
    struct stat buf;
    int rval = fstat(mHandle, &buf);
    if (rval == -1)
    {
        //throw sys::SystemException(Ctxt("Error querying file attributes"));
        return -1;
    }
    return buf.st_size;
}

VAFile::Off_T VAFile::LastModifiedTime()
{
    struct stat buf;
    int rval = fstat(mHandle, &buf);
    if (rval == -1)
    {
        return -1;
    }
    return (VAFile::Off_T) buf.st_mtime * 1000;
}

void VAFile::Flush()
{
    if (::fsync(mHandle) != 0)
    {
    }
}

void VAFile::Close()
{
    ::close(mHandle);
    mHandle = SYS_INVALID_HANDLE;
}
#endif // VA_VC
VA::_SYS_HANDLE_TYPE VAFile::GetHandle()
{
    return mHandle;
}

VA::VAFile::Off_T VAFile::GetCurrentOffset()
{
    return SeekTo(0, FROM_CURRENT);
}

}
