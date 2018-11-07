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

#define VA_MAX_PATH          (260)

namespace VA {

bool CheckFolderExists(const char* path)
{
	bool rValue = false;
	if(!path || strlen(path) == 0)
		return false;
#if VA_VC
	WIN32_FIND_DATA wfd;
#if VA_VC_WIN_PC
    UString FileSource;
	U16String strPath = U8StrToU16Str(path);
    FileSource.Format(TEXT("%s\\*.*"), strPath.GetBuffer());
	HANDLE hFind = FindFirstFile(FileSource.GetBuffer(), &wfd);
#else
    HANDLE hFind = FindFirstFile(strPath.GetBuffer(), &wfd);
#endif

	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = true;
	}
	FindClose(hFind);
#elif VA_GNUC
	DIR *dir = NULL;

	dir = opendir(path);
	if (NULL == dir)
	{
		rValue = false;
	}
	else
	{
		closedir(dir);
		rValue = true;
	}
#endif

	return rValue;
}

bool CheckFileExists(const char* szPath)
{
	bool bRetValue = false;
	if (!szPath || strlen(szPath) == 0)
    	return false;

#if VA_VC
	WIN32_FIND_DATA wfd;
	HANDLE hFind = ::FindFirstFile(U8StrToU16Str(szPath), &wfd);
	if (hFind == INVALID_HANDLE_VALUE || (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		bRetValue = false;
	}
	else
	{
		bRetValue = true;
	}

	if(hFind)
	    ::FindClose(hFind);
#elif VA_GNUC
	int ret = access(szPath, F_OK | R_OK);
	if(!ret)
		bRetValue = true;
	else
		bRetValue = false;
#endif

	return bRetValue;
}

bool DeleteFolder( const char* szPath )
{
	if (NULL == szPath || szPath[0] == '\0')
		return false;

	bool bRet = true;

	if(!CheckFolderExists(szPath))
	{
		return false;
	}

	char last_word;
	int ret;
	U8String destDir = szPath;

	last_word = destDir.GetAt(destDir.GetLength() - 1);
	if(last_word != '\\' && last_word != '/')
	{
		destDir += (const char*)"/";
	}

#if VA_VC
	long hnd;
	struct _finddata_t finfo;

	if((hnd = _findfirst((destDir + "*.*").GetBuffer(), &finfo)) < 0)
	{
		return RemoveDirectory((LPCWSTR)U8StrToU16Str(destDir.GetBuffer()));
	}

	do
	{
		U8String tmpDest;

		if(!strcmp(finfo.name, ".") ||
			!strcmp(finfo.name, ".."))
			continue;

		tmpDest = destDir + (const char*)finfo.name;

		if(finfo.attrib == _A_SUBDIR)
		{
			if(!DeleteFolder(tmpDest.GetBuffer()))
			{
				bRet = false;
				break;
			}
		}
		else
		{
			if((ret = remove(tmpDest.GetBuffer())) < 0)
			{
				bRet = false;
				break;
			}
		}
	}
	while((ret = _findnext(hnd, &finfo)) == 0);
	_findclose(hnd);

	if(bRet)
	{
		bRet = RemoveDirectory((LPCWSTR)U8StrToU16Str(destDir.GetBuffer()));
	}
#elif VA_GNUC
	DIR *pDir; 
	struct dirent *pDirent;
	pDir = opendir(destDir.GetBuffer());

	if(pDir)
	{
		while((pDirent = readdir(pDir)) != NULL)
		{
			U8String tmpDest;
			struct stat statbuf;

			if(!strcmp(pDirent->d_name, ".")
				|| !strcmp(pDirent->d_name, "..")
				|| !strcmp(pDirent->d_name, ""))
				continue;

			tmpDest = destDir + (const char*)pDirent->d_name;

			lstat(tmpDest.GetBuffer(), &statbuf);

			//if(pDirent->d_type == DT_DIR)
			if(S_ISDIR(statbuf.st_mode))
			{
				if(!DeleteFolder(tmpDest.GetBuffer()))
				{
					bRet = false;
					break;
				}
			}
			else
			{
				if((ret = remove(tmpDest.GetBuffer())) < 0)
				{
					bRet = false;
					break;
				}
			}
		}
		closedir(pDir);

		if(bRet)
		{
			bRet = !(remove(destDir.GetBuffer()));
		}
	}
	else
	{
		bRet = false;
	}
#endif
	return bRet;
}

bool CopyFiles( const char* szDest, const char* szSrc, const char* szExt /*= NULL*/)
{
	if (!szSrc || szSrc[0] == '\0'|| !szDest || szDest[0] == '\0')
		return false;

	if(!CheckFolderExists(szSrc)
		|| !CheckFolderExists(szDest))
	{
		return false;
	}

	bool bRet = true;
	char last_word;
	int ret;
	U8String destDir = szDest;
	U8String srcDir = szSrc;
	U8String findPath;

	last_word = destDir.GetAt(destDir.GetLength() - 1);
	if(last_word != '\\' && last_word != '/')
	{
		destDir += (const char*)"/";
	}

	last_word = srcDir.GetAt(srcDir.GetLength() - 1);
	if(last_word != '\\' && last_word != '/')
	{
		srcDir += (const char*)"/";
	}
#if VA_VC
	long hnd;
	struct _finddata_t finfo;

	if (szExt && szExt[0] != '\0')
	{
		//szFindPath.Format(TEXT("%s\\*.%s"), (LPCTSTR)szSrcPath, szExt);
		findPath = srcDir + "*.";
		findPath += szExt;
	}
	else
	{
		//szFindPath.Format(TEXT("%s\\*.*"), (LPCTSTR)szSrcPath);
		findPath = srcDir + "*.*";
	}

	if((hnd = _findfirst(findPath.GetBuffer(), &finfo)) < 0)
	{
		return false;
	}

	do
	{
		U8String tmpDest, tmpSrc;

		if(!strcmp(finfo.name, ".") ||
			!strcmp(finfo.name, ".."))
			continue;

		tmpDest = destDir + (const char*)finfo.name;
		tmpSrc = srcDir  + (const char*)finfo.name;

		if(finfo.attrib == _A_SUBDIR)
		{
			continue;
		}
		else
		{
			ret = VACopyFile(tmpDest.GetBuffer(), tmpSrc.GetBuffer());
			if(!ret)
			{
				bRet = false;
				break;
			}
		}
	}
	while((ret = _findnext(hnd, &finfo)) == 0);
	_findclose(hnd);

#elif VA_GNUC
	DIR *pDir; 
	struct dirent *pDirent;
	pDir = opendir(srcDir.GetBuffer());
	if(pDir)
	{
		while((pDirent = readdir(pDir)) != NULL)
		{
			U8String tmpDest;
			U8String tmpSrc;
			struct stat statbuf;
			char* pchExt = NULL;

			if(!strcmp(pDirent->d_name, ".") ||
				!strcmp(pDirent->d_name, ".."))
				continue;

			if(szExt && szExt[0] != '\0')
			{
				pchExt = strrchr(pDirent->d_name, '.');
				if(!pchExt || strcmp(pchExt + 1, szExt) != 0)
					continue;
			}

			tmpDest = destDir + (const char*)pDirent->d_name;
			tmpSrc = srcDir + (const char*)pDirent->d_name;

			lstat(tmpSrc.GetBuffer(), &statbuf);

			//if(pDirent->d_type == DT_DIR)
			if(S_ISDIR(statbuf.st_mode))
			{
				continue;
			}
			else
			{
				ret = VACopyFile(tmpDest.GetBuffer(), tmpSrc.GetBuffer());
				if(!ret)
				{
					bRet = false;
					break;
				}
			}
		}

		closedir(pDir);
	}
#endif

	return bRet;
}

bool CheckFolderWritePrivilege(const char* path)
{
	FILE *fp = NULL;
	if(!path)
		return false;

	U8String test_file = path;
	test_file += (const char*)"/test_folder_write_privilege_159";

	if((fp = fopen(test_file.GetBuffer(),"w")) == 0)
	{
		return false;
	}

	fclose(fp);
	fp = NULL;
	remove(test_file.GetBuffer());
	return true;
}

bool MakeDir(const char *path)
{
	if(!path)
		return false;

	U8String dir = path;
	U8String tmpDir;
	int i;
	char c;
    //make sure path end with '\' or '/'
	c = dir.GetAt(dir.GetLength() - 1);
	if(c != '\\' && c != '/')
	{
		dir += "/";
	}
	for(i = 0; (c = dir.GetAt(i)) != '\0'; i++)
	{
		if(c == '\\' || c == '/')
		{
			tmpDir.Empty();
			tmpDir = dir.Left(i + 1);
#if VA_VC
			if(!CheckFolderExists(tmpDir.GetBuffer())
				&& !CreateDirectory(U8StrToU16Str(tmpDir.GetBuffer()), NULL))
			{
                return false;
			}
#elif VA_GNUC
			//printf("%s:%d:%s\n", __FUNCTION__, __LINE__, tmpDir.GetBuffer());
			if(!CheckFolderExists(tmpDir.GetBuffer())
				&& mkdir(tmpDir.GetBuffer(), S_IRWXU | S_IRWXG | S_IRWXO))
			{

				return false;
			}
#endif
		}
	}
	
    return true;
}

bool GetPathLastName(const char *path, char *ret_name)
{
	if(!path || !ret_name)
		return false;

	int start = strlen(path) - 1;
	if(start < 0)
		return false;

    int end = start;
	const char *pstr = path;
	char c = *(pstr + start);
	if(c == '\\' || c == '/')
	{
		end = start - 1;
		start--;
	}
	for(;;)
	{
		if(start < 0)
			break;
        
		c = *(pstr + start);
		if(c == '\\' || c == '/')
		{
			break;
		}
		else
		{
			start--;
		}
	}

	if(start < 0)
		return false;

	start++;
	memcpy(ret_name, pstr + start, end - start + 1);
	ret_name[end - start + 1] = '\0';

	return true;
}

bool VACopyFile(const char* dest, const char* source)
{
	FILE* sourcefp = NULL;
	FILE* desfp = NULL;
	int   nSize = 0;
	char* pchBuffer = NULL;
	bool  bRet = true;

	if(!dest || dest[0] == '\0' || !source || source[0] == '\0')
	{
		return false;
	}

	sourcefp = fopen(source, "rb");
	desfp = fopen(dest, "wb");
	if(sourcefp && desfp)
	{
		fseek(sourcefp, 0, SEEK_END);
		nSize = ftell(sourcefp);
		fseek(sourcefp, 0, SEEK_SET);
		fseek(desfp, 0, SEEK_SET);
		if(nSize < 12*1024*1024)
		{
			pchBuffer = (char*)malloc(nSize);
			if(!pchBuffer
				|| nSize != fread(pchBuffer, 1, nSize, sourcefp)
				|| nSize != fwrite(pchBuffer, 1, nSize, desfp))
			{
				bRet = false;
			}
		}
		else
		{
			pchBuffer = (char*)malloc(12*1024*1024);
			if(pchBuffer)
			{
				while(!feof(sourcefp))
				{
					int buf_len = fread(pchBuffer, 1, 12*1024*1024, sourcefp);
					int out_len = fwrite(pchBuffer, 1, buf_len, desfp);
					if(out_len!=buf_len)
					{
						bRet = false;
						break;
					}
				}
			}
			else
			{
				bRet = false;
			}
		}
	}
	else
	{
		bRet = false;
	}

	if(pchBuffer)
		free(pchBuffer);

	if(sourcefp)
		fclose(sourcefp);

	if (desfp) {
		fflush(desfp);
#ifdef VA_GNUC
		fsync(fileno(desfp));
		//sync();
#endif
		fclose(desfp);
	}

	return bRet;
}

bool CopyDir(const char* szDest, const char* szSrc)
{
	if(!szDest || !szSrc)
		return false;

    if(!CheckFolderExists(szDest)
		&& !MakeDir(szDest))
	{
		//printf("%s:%d:\n", __FUNCTION__, __LINE__);
		return false;
	}

	char last_word;
	U8String destDir = szDest;
	U8String srcDir = szSrc;

	last_word = destDir.GetAt(destDir.GetLength() - 1);
	if(last_word != '\\' && last_word != '/')
	{
		destDir += (const char*)"/";
	}
	last_word = srcDir.GetAt(srcDir.GetLength() - 1);
	if(last_word != '\\' && last_word != '/')
	{
		srcDir += (const char*)"/";
	}

#if VA_VC
	long hnd;
	int ret;
	struct _finddata_t finfo;

    if((hnd = _findfirst((srcDir + "*.*").GetBuffer(), &finfo)) < 0)
	{
		return false;
	}

	do
	{
		U8String tmpDest, tmpSrc;

		if(!strcmp(finfo.name, ".") ||
			!strcmp(finfo.name, ".."))
			continue;

		tmpDest = destDir + (const char*)finfo.name;
		tmpSrc = srcDir  + (const char*)finfo.name;

		if(finfo.attrib == _A_SUBDIR)
		{
			CopyDir(tmpDest.GetBuffer(), tmpSrc.GetBuffer());
		}
		else
		{
			VACopyFile(tmpDest.GetBuffer(), tmpSrc.GetBuffer());
		}
	}
	while((ret = _findnext(hnd, &finfo)) == 0);
	_findclose(hnd);
    
	return true;

#elif VA_GNUC
	DIR *pDir; 
	struct dirent *pDirent;
	pDir = opendir(szSrc);

    if(pDir)
	{
		while((pDirent = readdir(pDir)) != NULL)
		{
			U8String tmpDest;
			U8String tmpSrc;
			struct stat statbuf;
			
			if(!strcmp(pDirent->d_name, ".") ||
				!strcmp(pDirent->d_name, ".."))
				continue;

			tmpDest = destDir + (const char*)pDirent->d_name;
			tmpSrc = srcDir + (const char*)pDirent->d_name;

			lstat(tmpSrc.GetBuffer(), &statbuf);

			//if(pDirent->d_type == DT_DIR)
			if(S_ISDIR(statbuf.st_mode))
			{
				if(!CopyDir(tmpDest.GetBuffer(), tmpSrc.GetBuffer()))
				{
                    closedir(pDir);
					return false;
				}
			}
			else
			{
				if(!VACopyFile(tmpDest.GetBuffer(), tmpSrc.GetBuffer()))
				{
					closedir(pDir);
					return false;
				}
			}
		}

		closedir(pDir);
	}

	return true;
#endif
}

std::string NormalizePath(const char* srcPath)
{
    return sys::Path::normalizePath(srcPath);
}



int IsAbsolutePath( const char* path )
{
    bool bAbs = sys::Path::isAbsolutePath(path);

    return bAbs ? 0 : -1;
}

std::string Pathname2Path(const char* path)
{
    std::string nomalPath = NormalizePath(path);
    if(nomalPath.empty())
        return "";
    sys::Path::StringPair pathPair = sys::Path::splitPath(nomalPath);

    return pathPair.first;
}

std::string Pathname2Name(const char* path)
{
    std::string nomalPath = NormalizePath(path);
    if(nomalPath.empty())
        return "";
    sys::Path::StringPair pathPair = sys::Path::splitPath(nomalPath);

    return pathPair.second;
}


std::string Pathname2Noextname(const char* path)
{
    std::string nomalPath = NormalizePath(path);
    if(nomalPath.empty())
        return "";
    return sys::Path::basename(nomalPath,true);

    //return pathPair.second;
}

int IsDirExist( const char* szDir )
{
#ifdef VA_VC
    DWORD ret = ::GetFileAttributesA(szDir);
    if ( ret != 0xFFFFFFFF && ( ret & FILE_ATTRIBUTE_DIRECTORY ) )
        return 0;
#else
    struct stat fs = { 0 };
    if ( stat(szDir, &fs) == 0 && ( fs.st_mode & S_IFDIR ) != 0 )
        return 0;
#endif /*WIN32*/
    return -1;
}

int IsFileExist( const char* szfile )
{
#ifdef VA_VC
    DWORD ret = ::GetFileAttributesA(szfile);
    if ( ret != 0xFFFFFFFF && ( ret & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
        return 0;
#else
    struct stat fs = { 0 };
    if ( stat(szfile, &fs) == 0 && ( fs.st_mode & S_IFDIR ) == 0 )
        return 0;
#endif /*WIN32*/
    return -1;
}

// 原则上内部使用
bool Match_String_i(char * pattern, const char * str)
{
    if ( !pattern || !str )
        return false;

    char * ps1 = pattern;
    char const * ps2 = str;

    while ( *ps1 != '\0' && *ps2 != '\0' )
    {
        switch ( *ps1 )
        {
        case '?':
            ps1++;
            ps2++;
            break;
        case '*':
            {
                // find existence of next block
                if ( 0 == *++ps1 )
                    return true;

                char *ps = strchr(ps1, '*');
                char *pq = strchr(ps1, '?');
                if ( ps )
                    *ps = '\0';
                if ( pq )
                    *pq = '\0';

                char const * pstr = strstr(ps2, ps1);
                if ( ps )
                    *ps = '*';
                if ( pq )
                    *pq = '?';

                if ( pstr )
                {
                    ps2 = pstr;
                    return Match_String_i(ps1, ps2);
                }
                return false;
                break;
            }
        default:
            if ( *ps1++ != *ps2++ )
                return false;
            break;
        }
    }
    // end ?
    if ( *ps1 == '\0' && *ps2 == '\0' )
        return true;
    else
        return false;
}

// 使用通配符进行匹配
bool Match_String(const char * pattern, const char * str)
{
    if ( !pattern || !str )
        return false;

    bool free_ppat = false;
    char pat[300]; pat[0] = 0;
    char * ppat = pat;
    size_t len = strlen(pattern) + 1;

    // malloc if pattern too long
    if ( sizeof pat < len )
    {
        free_ppat = true;
        ppat = new char[len];
    }
    strcpy(ppat, pattern);

    // call match safe
    bool ret = Match_String_i(ppat, str);

    if ( free_ppat )
        delete [] ppat;

    return ret;
}


int FindFiles( const char * dir, const char * filter, std::vector<std::string> & flist, bool recursive /*= true*/ )
{
    int ret = 0;
#ifdef VA_VC
    intptr_t	find_handle;
    _finddata_t	find_data;
    char		find_file[VA_MAX_PATH];

    sprintf(find_file, "%s/%s", dir, filter);
    find_handle = _findfirst(find_file, &find_data);
    if ( -1 == find_handle )
        return errno;

    // get all file
    do
    {
        // skip . & ..
        if ( !strcmp(find_data.name, ".")
            || !strcmp(find_data.name, "..") )
            continue;

        // recursive find ?
        if ( find_data.attrib & _A_SUBDIR )
        {
            if ( recursive )
            {
                char find_path[VA_MAX_PATH];
                sprintf(find_path, "%s/%s", dir, find_data.name);
                NormalizePath(find_path);
                ret = FindFiles(find_path, filter, flist, recursive);
                if ( ret != 0 )
                    break;
            }
            continue;
        }
        // push to list
        sprintf(find_file, "%s/%s", dir, find_data.name);
        NormalizePath(find_file);
        flist.push_back(find_file);
    } while ( -1 != _findnext(find_handle, &find_data) );

    _findclose(find_handle);
    return ret;

#else  // Linux

	std::string strFindFile;
    DIR	* pdr = opendir(dir);
    if ( 0 == pdr )
        return errno;

    for ( ; ; )
    {
        dirent *pde = readdir(pdr);
        if ( 0 == pde )
            break;

        // skip . & ..
        if ( !strcmp(pde->d_name, ".")
            || !strcmp(pde->d_name, "..") )
            continue;

        if ( !Match_String(filter, pde->d_name ) )
            continue;

		strFindFile=dir;
		char last_word = strFindFile[strFindFile.length() - 1];
		if(last_word != '\\' && last_word != '/')
		{
			strFindFile += (const char*)"/";
		}
		strFindFile += pde->d_name;
        NormalizePath(strFindFile.c_str());

        struct stat	st;
        ret = stat(strFindFile.c_str(), &st);
        if ( 0 != ret )
            break;

        if ( st.st_mode & S_IFDIR )
        {
            ret = FindFiles(strFindFile.c_str(), filter, flist, recursive);
            if ( ret != 0 )
                break;
            continue ;
        }

        flist.push_back(strFindFile.c_str());
    }

    closedir(pdr);
    return ret;

#endif /* #ifdef WIN32 */
}

int PathtoDirTree( const char * path, std::vector<std::string> & dir_tree )
{
    if ( path == 0 || *path == 0 )
        return -1;

    std::string strNormalPath = NormalizePath(path);

    if(strNormalPath.empty())
        return -1;

    const char * normalpath = strNormalPath.c_str();

    int begin = 0;
    int len = (int)strlen(normalpath);
    if ( len > 2 )
    {
        if ( normalpath[0] == '/' )   /* linux skip first / add by jjzhuang 20140430  */
            begin = 1;
        else if ( normalpath[0] == '\\' && normalpath[1] == '\\' )
            begin = 2;
        else if ( normalpath[1] == ':' )
            begin = 3;
    }

    char dir_name[VA_MAX_PATH];
    for ( int i = begin; ; i++ )
    {
        const char c = normalpath[i];
        if ( c == '/'|| c == '\\' || c == '\0' )
        {
            strncpy(dir_name, normalpath, i);
            dir_name[i] = 0;
            dir_tree.push_back(dir_name);
            if ( path[i+1] == 0 )
                break ;
        }
        if ( c == 0 )
            break ;
    }

    return 0;
}
std::string GetAppPath()
{
#if VA_VC
    DWORD nRet;
    char current_absolute_path[VA_MAX_PATH];
    memset(current_absolute_path,0,VA_MAX_PATH);
    nRet = ::GetModuleFileNameA( NULL, current_absolute_path, VA_MAX_PATH);  
    (strrchr(current_absolute_path, '\\'))[0] = 0;//删除文件名，只获得路径字串 
    return std::string(current_absolute_path);
#else
    char current_absolute_path[VA_MAX_PATH];
    memset(current_absolute_path,0,VA_MAX_PATH);
    getcwd(current_absolute_path, VA_MAX_PATH);
    return std::string(current_absolute_path);
#endif
}


int remove( const char * path) 
{
#if VA_VC
    if (IsDirectory(path))
        return (::RemoveDirectoryA(path)) ? (0): (-1);

    return (::DeleteFileA(path)) ? (0) : (-1);
#else
    if (IsDirectory(path))
    {
        if (::rmdir(path) == 0)
            return 0;
    }
    else
    {
        if (::unlink(path) == 0)
            return 0;
    }
    return -1;
#endif

}

int move( const char* path, const char* newPath ) 
{
#if VA_VC
    return (::MoveFileA(path, newPath)) ? (0) : (-1);
#else
    return (::rename(path, newPath)) ? (0) : (-1);
#endif
}

int IsFile( const char* path ) 
{
    //  I'm not 100% sure.  So I'm checking
    //  1) Exists
    //  2) Not Directory
    //  3) Not Archive - we aren't doing that...
#if VA_VC

    const DWORD what = ::GetFileAttributesA(path);
    return (what != INVALID_FILE_ATTRIBUTES &&
        !(what & FILE_ATTRIBUTE_DIRECTORY)) ? (0) : (-1);
#else
    struct stat info;
    if (stat(path, &info) == -1)
        return -1;
    //        throw sys::SystemException("Stat failed");
    return (S_ISREG(info.st_mode)) ? (0) : (-1);
#endif

}

int IsDirectory( const char* path ) 
{
#if VA_VC
    const DWORD what = ::GetFileAttributesA(path);
    return (what != INVALID_FILE_ATTRIBUTES &&
        (what & FILE_ATTRIBUTE_DIRECTORY)) ? (0) : (-1);
#else
    struct stat info;
    if (stat(path, &info) == -1)
        return -1;
    //        throw sys::SystemException("Stat failed");
    return (S_ISDIR(info.st_mode)) ? (0) : (-1);
#endif

}

bool IsSamePath(const char srcPath[], const char dstPath[])
{
	int srcLen = 0, dstLen = 0;
	char srcTempChar = '0', dstTempChar = '0';
	int m = 0, n = 0;
	bool isChkdsk = false;

	if (NULL == srcPath && NULL == dstPath) {
		return true;
	}

	if (NULL == srcPath || NULL == dstPath) {
		return false;
	}

	srcLen = strlen(srcPath);
	dstLen = strlen(dstPath);
	while (m < srcLen && n <dstLen) {
		srcTempChar = dstTempChar = '0';
		if (srcPath[m] == dstPath[n]) {
			if (srcPath[m] == '\\' || srcPath[m] == '/' ) {
				isChkdsk = true;
			} else {
				isChkdsk = false;
			}
			m++;
			n++;
			continue;
		} else {
			srcTempChar = srcPath[m];
			dstTempChar = dstPath[n];
			if (srcPath[m] == '/') {
				srcTempChar = '\\';
			}
			if (dstPath[n] == '/') {
				dstTempChar = '\\';
			}
			if (dstTempChar == '\\' && srcTempChar == dstTempChar) {
				m++;
				n++;
				isChkdsk = true;
				continue;
			} else {
				if (srcTempChar == '\\' && isChkdsk == true) {
					m++;
					continue;
				} else if (dstTempChar == '\\' && isChkdsk == true) {
					n++;
					continue;
				} else {
					return false;
				}
			}
		}
	}

	if ((m == srcLen) && (n == dstLen)) {
		return true;
	} else {
		if ((m == srcLen) && (n < dstLen)) {
			while (n < dstLen) {
				if (dstPath[n] != '/' && dstPath[n] != '\\') {
					return false;
				}
				n++;
			}
			return true;
		} else if ((m < srcLen ) && (n == dstLen -1)) {
			while (m < srcLen) {
				if (srcPath[m] != '/' && srcPath[m] != '\\') {
					return false;
				}
				m++;
			}
			return true;
		} else {
			return false;
		}
	}
}

bool GetFatherDir(char *dir)
{
	char *p = NULL;
	int len = strlen(dir);
	int m  = len - 1;

	if (strlen(dir) == 0) {
		return false;
	}

	p = dir;
	while (m >= 0) {
		if (*(p+m) == '\\' || *(p+m) == '/') {
			if (m == len - 1) {
				*(p+m) = L'\0';
				len = strlen(dir);
				m--;
			} else {
				*(p+m) = L'\0';
				return true;
			}
		} else {
			m--;
		}
	}

	return false;
}

}
