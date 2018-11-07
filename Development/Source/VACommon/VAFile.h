#pragma once
#ifndef VAFILE_H_INCLUDED
#define VAFILE_H_INCLUDED
#if VA_VC
#include <windows.h>
#elif VA_GNUC
#endif

#include <string>
#include <vector>
#include <fcntl.h>  
using namespace std;


namespace VA
{
#ifdef VA_VC
#    define _SYS_SEEK_CUR FILE_CURRENT
#    define _SYS_SEEK_SET FILE_BEGIN
#    define _SYS_SEEK_END FILE_END
#    define _SYS_CREAT    OPEN_ALWAYS
#    define _SYS_OPEN_EXISTING OPEN_EXISTING
#    define _SYS_TRUNC 8
#    define _SYS_RDONLY GENERIC_READ
#    define _SYS_WRONLY GENERIC_WRITE
#    define _SYS_RDWR GENERIC_READ|GENERIC_WRITE
#    define SYS_INVALID_HANDLE INVALID_HANDLE_VALUE
    typedef HANDLE _SYS_HANDLE_TYPE;
#else
#    define _SYS_DEFAULT_PERM 0644
#    define _SYS_MAX_READ_ATTEMPTS 100
#    define _SYS_SEEK_CUR SEEK_CUR
#    define _SYS_SEEK_SET SEEK_SET
#    define _SYS_SEEK_END SEEK_END
#    define _SYS_CREAT O_CREAT
#    define _SYS_OPEN_EXISTING 0
#    define _SYS_TRUNC O_TRUNC
#    define _SYS_RDONLY O_RDONLY
#    define _SYS_WRONLY O_WRONLY
#    define _SYS_RDWR O_RDWR
#    define SYS_INVALID_HANDLE -1
    typedef int _SYS_HANDLE_TYPE;
#endif


class  VAFile
{
public:

    typedef unsigned long long  Uint64_T;
    typedef unsigned long       Uint32_T;

    typedef long long           Int64_T;
    typedef long                Int32_T;

    typedef Int64_T             Off_T;

#   ifndef _SIZE_T_DEFINED
#       ifdef  _WIN64
    typedef Uint64_T   Size_T;
#define SIZEOF_SIZE_T  8
#       else
    typedef Uint32_T   Size_T;
#define SIZEOF_SIZE_T  4
#       endif
#   else
    typedef size_t Size_T;
#define SIZEOF_SIZE_T  4
#   endif

#   if SIZEOF_SIZE_T == 8
    typedef Int64_T   SSize_T;
#   elif SIZEOF_SIZE_T == 4
    typedef Int32_T   SSize_T;
#   else
#error SIZEOF_SIZE_T must be set at configure time
#   endif


    enum
    {
        FROM_START = _SYS_SEEK_SET,
        FROM_CURRENT = _SYS_SEEK_CUR,
        FROM_END = _SYS_SEEK_END,
        CREATE = _SYS_CREAT,
        EXISTING = _SYS_OPEN_EXISTING,
        TRUNCATE = _SYS_TRUNC,
        READ_ONLY = _SYS_RDONLY,
        WRITE_ONLY = _SYS_WRONLY,
        READ_AND_WRITE = _SYS_RDWR
    };

    /*!
     *  Default constructor.  Does nothing
     */
    VAFile() :
        mHandle(SYS_INVALID_HANDLE)
    {
    }

    /*!
     *  Constructor.  Initializes to a file.
     *  \param str A file name to open
     *  \param accessFlags File access flags
     *  \param creationFlags File creation flags
     */
    VAFile(const char* path, int accessFlags = READ_ONLY, 
         int creationFlags = EXISTING)
    {
        Create(path, accessFlags, creationFlags);
    }

    /*!
     *  Constructor.  Initializes to a file.
     *  \param str A file name to open
     *  \param accessFlags File access flags
     *  \param creationFlags File creation flags
     */
    VAFile(const std::string& str, int accessFlags = READ_ONLY, 
         int creationFlags = EXISTING)
    {
        Create(str, accessFlags, creationFlags);
    }

    /*!
     *  Destructor.  Closes file if open.
     */
    ~VAFile()
    {
        if (IsOpen())
            Close();
    }

    /*!
     *  Is the file open?
     *  \return true if open, false if invalid handle
     */
    bool IsOpen()
    {
        return (mHandle != SYS_INVALID_HANDLE);
    }

    /*!
     *  Return the underlying file handle
     *
     */
    _SYS_HANDLE_TYPE GetHandle();

    
    /*!
     *  Initialize the object to a file.
     *  \param str A file name to open
     *  \param accessFlags File access flags
     *  \param creationFlags File creation flags
     */
    _SYS_HANDLE_TYPE Create(const std::string& str, int accessFlags, 
                int creationFlags);

    /*!
     *  Read from the File into a buffer 'size' bytes.
     *  Blocks.
     *  If size is <=0, no OS level read operation occurs.
     *  If size is > length of file, an exception occurs.
     *
     *  \param buffer The buffer to put to
     *  \param size The number of bytes
     */
    void ReadInto(char* buffer, Size_T size);

    /*!
     *  Write from a buffer 'size' bytes into the 
     *  file.
     *  Blocks.
     *  If size is 0, no OS level write operation occurs.
     *  If size is < 0, an exception is thrown.
     *
     *  \param buffer The buffer to read from
     *  \param size The number of bytes to write out
     */
    void WriteFrom(const char* buffer, 
                   Size_T size) ;

    /*!
     *  Seek to the specified offset, relative to 'whence.'
     *  Valid values are FROM_START, FROM_CURRENT, FROM_END.
     *
     *  \return Global offset location.
     */

    Off_T SeekTo(Off_T offset, 
                      int whence) ;

    /*!
     *  Report current offset within file.
     *
     *  \return Current offset;
     */
    Off_T GetCurrentOffset();

    /*!
     * Report the length of the file.
     * \return The length
     */
    Off_T Length() ;

    /*!
     * Returns the last modified time of the file, in millis.
     * \return last modified time
     */
    Off_T LastModifiedTime();

    /*!
     *  Flush the file to disk
     */
    void Flush();

    /*!
     *  Close the handle.
     */
    void Close();

protected:
    _SYS_HANDLE_TYPE mHandle;
    std::string mPath;

};

}
#endif  // VAFILESYSTEM_H_INCLUDED