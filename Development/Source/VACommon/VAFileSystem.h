#pragma once
#ifndef VAFILESYSTEM_H_INCLUDED
#define VAFILESYSTEM_H_INCLUDED
#if VA_VC
#include <windows.h>
#elif VA_GNUC
#endif

#include <string>
#include <vector>
using namespace std;


namespace VA
{
	//DWORD GetWorkPath( const char* szPath, DWORD nSize );
	bool CheckFolderExists(const char* path);
	bool CheckFileExists(const char* szPath);

	bool DeleteFolder( const char* szPath );
	bool CopyFiles( const char* szDest, const char* szSrc, const char* szExt /*= NULL*/);//只拷贝指定扩展名的文件，不递归子目录
	//bool CopyFolder(LPCTSTR szSrc, LPCTSTR szDest);	//递归拷贝子目录全部文件

	bool CheckFolderWritePrivilege(const char* path);
	bool MakeDir(const char *path);
	bool GetPathLastName(const char *path, char *ret_name);
	bool VACopyFile(const char* dest, const char* source);
	bool CopyDir(const char* szDest, const char* szSrc);
	bool GetFatherDir(char *dir); /* 获取上级目录 */
	bool IsSamePath(const char srcPath[], const char dstPath[]); /* 判断两个目录是否相同 */

    /**
     * GetAppPath - 获取应用程序所在的启动目录
     *
     * @return: 返回应用程序所在的启动目录
     */
    std::string GetAppPath();

    /**
     * NormalizePath - 规整化文件路径
     *
     * 根据给定的完整路径或者包含相对路径规整化
     * windows平台按照'\\'规整，wince/linux/qnx按照'/'规整
     *
     * @param path为绝对路径或相对路径
     * @return: 返回规整化后的路径。当路径存在错误时返回空字符串，需对返回结果进行empty校验
     */
    std::string NormalizePath(const char* srcPath);

     /**
     * IsAbsolutePath - 判断是否为绝对路径
     *
     * @param path为绝对路径或相对路径
     * @return: 成功返回0，否则返回-1
     */
    int IsAbsolutePath(const char* path);


     /**
     * Pathname2Path - 路径名转路径
     *
     * @param path为绝对路径或相对路径
     * @return: 返回转换后的路径。当路径存在错误时返回空字符串，需对返回结果进行empty校验
     */
    std::string Pathname2Path(const char* path);

     /**
     * Pathname2Name - 路径名转文件名（带后缀）
     *
     * @param path为绝对路径或相对路径
     * @return: 返回转换后的文件名。当路径存在错误时返回空字符串，需对返回结果进行empty校验
     */
    std::string Pathname2Name(const char* path);

     /**
     * Pathname2Noextname - 路径名转不带后缀的文件名
     *
     * @return: 返回转换为不带后缀的文件名。当路径存在错误时返回空字符串，需对返回结果进行empty校验
     */
    std::string Pathname2Noextname(const char* path);

     /**
     * IsDirExist - 检测目录是否存在
     *
     * @param szDir为绝对路径
     * @return: 成功返回0，否则返回-1
     */
    int IsDirExist(const char* szDir);

     /**
     * IsDirExist - 检测文件是否存在
     *
     * @param szDir为绝对路径
     * @return: 成功返回0，否则返回-1
     */
    int IsFileExist(const char* szfile);

     /**
     * PathtoDirTree - 切分目录成为目录树结构
     *
     * @param szDir为绝对路径
     * @param dir_tree为返回的目录树
     * @return: 成功返回0，否则返回-1
     */
    int PathtoDirTree(const char * path, std::vector<std::string> & dir_tree);

     /**
     * PathtoDirTree - 获得一个目录下的文件列表
     *
     * @param szDir为绝对路径
     * @param filter为搜索通配符，如"*.*"
     * @param dir_tree为返回的文件结果
     * @param recursive为是否递归搜索
     * @return: 成功返回0，否则返回-1
     */
    int FindFiles(const char * dir, const char * filter, std::vector<std::string> & flist, bool recursive = true);

     /**
     * Remove - 删除文件或目录
     *
     * @param szDir为绝对路径
     * @return: 成功返回0，否则返回-1
     */
    int Remove(const char* path) ;

     /**
     * Remove - 移动文件或目录
     *
     * @param szDir为绝对路径
     * @return: 成功返回0，否则返回-1
     */
    int Move(const char* path, 
        const char* newPath) ;


     /**
     * Remove - 判断是否为文件
     *
     * @param szDir为绝对路径
     * @return: 成功返回0，否则返回-1
     */
    int IsFile(const char* path) ;

     /**
     * Remove - 判断是否为文件夹
     *
     * @param szDir为绝对路径
     * @return: 成功返回0，否则返回-1
     */
    int IsDirectory(const char* path) ;
}

#include "VAFile.h"
#endif  // VAFILESYSTEM_H_INCLUDED