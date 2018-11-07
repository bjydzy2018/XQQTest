/** 
 * @file	iFly_Log.i
 * @brief	日志类的实现文件
 *
 *	详细的功能说明请参考头文件中的说明
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2004-05-13
 * 
 * @see		iFly_Log.h
 * 
 * <b>版本记录：</b><br>
 * <table>
 *  <tr> <th>版本	<th>日期			<th>作者	<th>备注 </tr>
 *  <tr> <td>1.0	<td>2002年8月31日	<td>jdyu	<td>创建 </tr>
 * </table>
 */
#include <stdio.h>
#include <wchar.h>
#include <time.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <locale.h>
#include <algorithm>
#include "iFly_Log.h"
#include "../VAString.h"

#ifndef __IFLY_LOG_C__
#define __IFLY_LOG_C__

/* MAX_PATH */
#ifndef MAX_PATH
#  define MAX_PATH 260
#endif /* MAX_PATH */

/* Windows include */
#ifdef WIN32
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

/* ACE_Timer */
#ifdef ACE_ACE_H
# include "ace/High_Res_Timer.h"
#endif /* ACE */

/* fsopen in unicode */
#ifdef _UNICODE
# ifndef _tfsopen
#  define  _tfsopen _wfsopen
# endif
#else
# ifndef _tfsopen
#  define  _tfsopen _fsopen
# endif
#endif /* _UNICODE */

#if defined(__ANDROID__)
#include <sys/types.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)
#endif
// 为了方便调用，在这里加入了一些常用的函数
namespace IFLY_LOG
{
	// 获得当前时间
	inline const char * cur_time(char time[], bool no_path_char = false, bool sht_fmt = false, time_t start_time = 0, unsigned long start_tick = 0);
	// 获得进程ID
	inline int getpid(void);
	// 获得当前线程id
	inline int thr_self(void);
	// 获得当前模块的句柄
	inline LOG_HANDLE mod_handle(void);
	// 获得当前模块路径
	inline std::string mod_path_name(LOG_HANDLE hdl = 0);
	// 获得模块的版本号
	inline std::string mod_version(const char* mod);
	// ln2	
	inline int ln2(unsigned int pw);
	// cat two path
	inline const char* cat_path(char* path, const char* more, const char path_split = '/');
	// normalize path
	inline int normalize_path(char path[MAX_PATH], const char path_split = '/');
	// get bin path
	inline char* get_bin_path(char szPath[MAX_PATH], LOG_HANDLE hmod = 0);
	// is_abs_path
	inline bool is_abs_path(const char* path);
	// 路径名转路径
	inline char* pathname_to_path(char path[MAX_PATH], const char* path_name, const char path_split = '/');
	inline char* pathname_to_name(char name[MAX_PATH], const char* path_name, const char path_split = '/');
	inline char* pathname_to_noextname(char name[MAX_PATH], const char* path_name, const char path_split = '/');
	// 获得文件日期
	inline time_t file_mtime(const char* file);
	inline time_t file_ctime(const char* file);
	// 获得文件的修改时间字符串
	inline std::string file_mtime_str(const char * file, const char * fmt = 0);
	inline std::string file_ctime_str(const char * file, const char * fmt = 0);
	// 切分字符串
	inline int split_str(const char* str, std::vector<std::string> & subs_array, const char spliter[] = ",;", bool trim = true, bool ignore_blank = false, bool supp_quote = false);
	// 忽略大小写的比较
	inline int strcasecmp(const char* str1, const char * str2);
	// 检测目录是否存在
	inline bool is_dir_exist(const char* lpszDir);
	inline bool is_file_exist(const char* file);
	// 使用通配符进行匹配
	inline bool match_string(const char * pattern, const char * str);
	inline bool match_string_i(char * pattern, const char * str);
	// 获得一个目录下的文件列表
	inline int find_files(const char * dir, const char * filter, std::list<std::string> & flist, bool recursive = true);
	// 切分目录成为目录树结构
	typedef std::vector<std::string> STR_ARR;
	inline int path_to_dir_tree(const char * path, STR_ARR & dir_tree);
	// 创建目录
	inline int create_directory(const char * dir, bool fail_if_exist = false, bool recursive = true);
	// 去掉字符串的两边的空格
	inline int trim_str(char* pstr, const char trim_char = ' ');
	// 宽字符和窄字符的转换
	inline std::string  wchar2char(const wchar_t * wstr, size_t * cvt_chrs = 0);
	inline std::wstring char2wchar(const char * str, size_t * cvt_chrs = 0);
	// 字符串转换为小写
	inline std::string & strlwr(std::string & str);

	// 宽窄转换，linux直接转
	inline size_t mbstowcs(wchar_t *wcstr, const char *mbstr, size_t count);
	inline size_t wcstombs(char *mbstr, const wchar_t *wcstr, size_t count);
};

/** 
 * @brief	构造函数
 * @author	jdyu
 * @date	2004年5月13日
 */
template <typename IO, typename LOCK, typename CFG>
Log_Impl_T<IO, LOCK, CFG>::Log_Impl_T(void)
	: log_io_			(0)
	, call_delete_io_	(true)
	, lock_				(0)
	, call_delete_lock_	(false)
	, last_ltime_		(0)
	, alloc_console_	(false)
{
}

/** 
 * @brief	构造函数
 * @author	jdyu
 * @date	2004年5月13日
 * @param	const char* file			缺省文件名
 * @param	const char* title = 0	缺省的标题
 * @param	bool has_head = false		是否包含日志头
 */
template <typename IO, typename LOCK, typename CFG>
Log_Impl_T<IO, LOCK, CFG>::Log_Impl_T(const char* file, const char* title /*= 0*/, bool has_head /*= false*/, IO* io_impl /* = 0 */)
	: log_io_			(0)
	, call_delete_io_	(true)
	, lock_				(0)
	, call_delete_lock_	(false)
	, last_ltime_		(0)
	, alloc_console_	(false)
{
	CFG cfg;
	cfg.file_name(file);
	cfg.title(title);
	cfg.has_head(has_head);
	open(&cfg, io_impl);
}

/** 
 * @brief	析构函数
 * @author	jdyu
 * @date	2004年5月21日
 */
template <typename IO, typename LOCK, typename CFG>
Log_Impl_T<IO, LOCK, CFG>::~Log_Impl_T()
{
	close();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::open
 *
 *	初始化日志类
 * 
 * @author	jdyu
 * @date	2004年5月21日
 * @return	int - Return 0 in success, otherwise return error code.
 * @param	const PLog_Cfg cfg_p = 0	- [in] 日志的配置信息
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
int Log_Impl_T<IO, LOCK, CFG>::open(const CFG* cfg_p /*= 0*/, IO* io_impl /* = 0 */, void* rev /* = 0 */)
{
	// uninitialize first
	int ret = 0;
	ret = close();

	// configuration info
	if ( io_impl && io_impl->config() != 0 )
		config_.set(io_impl->config());
	if ( cfg_p != 0 )
		config_ = *cfg_p;

	// record start time
	if ( config_.start_time() == 0 )
	{
		#ifdef WIN32
		time_t start_time = time(0);
		unsigned int start_tick = ::GetTickCount();
		// 是否需要和系统对时
		if ( cfg_p->synch_time() )
		{
			time_t cur = start_time;
			while ( ( cur = time(0) ) == start_time )
				::Sleep(1);
			start_time = cur;
			start_tick = ::GetTickCount();
		}
		config_.start_time(start_time);
		config_.start_tick(start_tick);
		#else // LNX
		struct timeval tv;
		gettimeofday(&tv, 0);
		config_.start_tick(tv.tv_usec / 1000);
		#endif // WIN32
	}

	// set locale
	if ( config_.locale() && *config_.locale() )
	{
		std::string lc = config_.locale();
		if ( lc == "null" )
			setlocale(LC_ALL, 0);
		else if ( lc == "empty" )
			setlocale(LC_ALL, "");
		else if ( lc != "skip" )
			setlocale(LC_ALL, config_.locale());
	}

	// Create log mutex
	if ( io_impl == 0 || io_impl->mutex() == 0 )
	{
		char mname[256];
		std::string ttl = config_.title();
		for ( int i = 0; i < (int)ttl.length(); i++)
			if ( ttl.at(i) == ' ' )
				ttl.at(i) = '_';
		sprintf(mname, "iFly_Logger_Mutex_%s", ttl.c_str());
		// open mutex
		lock_ = new LOCK;
		ret = lock_->open(mname);
		if ( ret != 0 )
			printf("Log_Impl_T::open open lock name %s failed, error = %d.\n", mname, ret);

		call_delete_lock_ = true;
	}
	else
	{
		lock_ = io_impl->mutex();
		call_delete_lock_ = false;
	}

	// open file
	if ( config_.output() & lgo_file )
	{
		LOG_GUARD(Log_Mutex, lock_);

		// 创建IO对象
		if ( io_impl != 0 )
		{
			log_io_ = io_impl;
			call_delete_io_ = false;
		}
		else
		{
			log_io_ = new IO;
			call_delete_io_ = true;
			log_io_->mutex(lock_);

			// 获得目录
			char log_dir[MAX_PATH]; log_dir[0] = 0;
			IFLY_LOG::pathname_to_path(log_dir, config_.file_name());
			if ( log_dir[0] != 0 )
				IFLY_LOG::create_directory(log_dir);

			// 初始化
			ret = log_io_->open(config_.file_name(), lock_, config_.mode(), rev);
			if ( ret != 0 )
				return ret;

			// 写文件头信息
			if ( config_.has_head() )
				write_head();

			// config info
			log_io_->config(&config_);
		}
	}

	// 是否需要一个console
	#ifdef WIN32
	if ( config_.output() & lgo_console )
	{
		/* HWND hwc = GetConsoleWindow(); */
		int ret = AllocConsole();
		if ( ret != 0 )
		{
			alloc_console_ = true;
			freopen("CONOUT$", "w+t", stdout);
		}
	}
	#endif // WIN32
	return 0;
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::close
 *
 *	逆初始化日志
 * 
 * @author	jdyu
 * @date	2004年5月21日
 * @return	int	- Return 0 in success, otherwise return error code.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
int Log_Impl_T<IO, LOCK, CFG>::close()
{
	if ( log_io_ != 0 )
	{
		// 关闭文件
		LOG_GUARD(Log_Mutex, lock_);
		if ( call_delete_io_ )
		{
			// 写文件尾信息
			if ( config_.has_head() )
				write_tail();
			log_io_->close();
			IO * io = static_cast<IO*>(log_io_);
			delete io, log_io_ = 0;
		}
	}

	// 关闭互斥对象
	if ( lock_ != 0 )
	{
		if ( call_delete_lock_ )
		{
			lock_->close();
			LOCK * lk = static_cast<LOCK*>(lock_);
			delete lk, lock_ = 0;
		}
	}

	// 是否需要释放console
	#ifdef WIN32
	if ( alloc_console_ )
		FreeConsole();
	#endif // WIN32
	return 0;
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::back_file
 *
 *	备份旧的日志文件
 * 
 * @author	jdyu
 * @date	2004年5月21日
 * @return	int	- Return 0 in success, otherwise return error code.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
int Log_Impl_T<IO, LOCK, CFG>::back_file(void)
{
	int ret = 0;

	// backup file name
	char bak_name[MAX_PATH];
	strcpy(bak_name, config_.file_name());
	if ( !config_.overwrite() )
	{
		// 生成备份文件名
		char* fname = (char*)config_.file_name();
		const char* psz = strrchr(fname, '.');
		if ( 0 == psz )
			psz = fname + strlen(fname);
		strncpy(bak_name, fname, psz - fname);
		bak_name[psz - fname] = 0;

		char times[128];
		sprintf(bak_name, "%s_%s.log", bak_name, cur_time(times, true));
	}

	// 获得文件列表，检查是否超过数量
	if ( config_.max_count() > 0 )
	{
		std::list<std::string> flist;
		char log_dir[MAX_PATH]; log_dir[0] = 0;
		IFLY_LOG::pathname_to_path(log_dir, config_.file_name());
		if ( log_dir[0] == 0 )
			strcpy(log_dir, "./");

		char filter[MAX_PATH];
		IFLY_LOG::pathname_to_noextname(filter, config_.file_name());
		strcat(filter, "_*.log");

		IFLY_LOG::find_files(log_dir, filter, flist, true);
		flist.sort();

		// 仅统计同一组件生成的日志文件，add by ycwang2，2011-07-16
		for (std::list<std::string>::iterator it = flist.begin(); it != flist.end(); )
		{
			if (it->size() != strlen(bak_name))
				it = flist.erase(it);
			else
				++it;
		}

		if ( (int)flist.size() >= config_.max_count() )
		{
			// 不删除最初的那个文件
			if ( flist.size() > 1 )
				flist.pop_front();
			if ( flist.size() > 0 )
				unlink(flist.front().c_str());
		}
	}

	// Move file to old
	if ( config_.has_head() )
		write_tail(false);

	if ( log_io_ )
		ret = log_io_->backup(bak_name);

	if ( config_.has_head() )
		write_head(false);

	return ret;
}


/** 
 * @brief 	LOCK, CFG>::check_config
 *  
 *  检查并重新读取配置信息
 *  
 * @author	jdyu
 * @date	2007-4-24
 * @return	int Log_Impl_T<IO,	- Return 0 in success, otherwise return error code.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
int Log_Impl_T<IO, LOCK, CFG>::check_config(void)
{
//qnx disable by jjzhuang 2016/04/06
#if 1
	time_t tm;
	if ( time(&tm) - last_ltime_ > static_cast<unsigned int>(LOG_CFGCHECK_TIME) )
	{
		LOG_GUARD(Log_Mutex, lock_);
		if ( config_.config_changed() )
		{
			CFG cfg(config_);
			cfg.read_config();
			config_.output(cfg.output());
			config_.level(cfg.level());
			config_.flush_always(cfg.flush_always());
			config_.style(cfg.style());
			config_.perf_thres(cfg.perf_thres());
		}
		last_ltime_ = (long)tm;
	}
#endif

	return 0;
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_msg_i
 *
 *	内部调用的记录日志信息
 *	不带变参(防止多次变参带来的bug)
 * 
 * @author	jdyu
 * @date	2009-3-31
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void Log_Impl_T<IO, LOCK, CFG>::log_msg_i(LOG_LEVEL lvl, const char* msg)
{
	log_msg_i(0, lvl, lgs_default, msg);
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_msg_i
 *
 *	内部调用的记录日志信息
 *	不带变参(防止多次变参带来的bug)
 * 
 * @author	jdyu
 * @date	2009-3-31
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void Log_Impl_T<IO, LOCK, CFG>::log_msg_i(LOG_LEVEL lvl, const wchar_t* msg)
{
	log_msg_i(0, lvl, lgs_default, msg);
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_msg_i
 *
 *	内部调用的记录日志信息，带主题信息的日志记录实现函数体
 *	不带变参(防止多次变参带来的bug)
 * 
 * @author	jdyu
 * @date	2004-5-28
 * @return	No return value.
 * @param	const char* sub		- [in] 主题信息字符串
 * @param	ISF_LOG_LEVEL lvl	- [in] 日志的信息等级
 * @param	int style		- [in] 写入的格式
 * @param	const char* fmt	- [in] 信息字符串
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void Log_Impl_T<IO, LOCK, CFG>::log_msg_i(const char* sub, LOG_LEVEL lvl, int style, const char* msg)
{
	// 检查配置是否修改过
	check_config();

	// 样式处理
	if ( style == 0 )
		style = config_.style();

	// 是否输出
	if ( config_.output() == lgo_none )
		return ;
	// 错误等级是否过滤
	if ( ( lvl & config_.level() ) == 0 )
		return ;
	// 内容过滤
	if ( !config_.is_output(msg, sub) )
		return ;

	// 变参
	char wmsg[LOG_MAXMSGLEN], tmp[200];
	wmsg[0] = 0; tmp[0] = 0;

	// has time?
	if ( style & lgs_time )
		sprintf(wmsg, "[%s]", cur_time(tmp));
	else if ( style & lgs_time_hms )
		sprintf(wmsg, "[%s]", cur_time(tmp, false, true));

	// has level?
	if ( style & lgs_level )
	{
		sprintf(tmp, "[%s]", level_str(lvl));
		strcat(wmsg, tmp);
	}
	
	// has process ID?
	if ( style & lgs_precessid )
	{
		sprintf(tmp, "[Px%04x]", IFLY_LOG::getpid());
		strcat(wmsg, tmp);
	}
	
	// has thread ID?
	if ( style & lgs_threadid )
	{
		sprintf(tmp, "[Tx%04x]", IFLY_LOG::thr_self());
		strcat(wmsg, tmp);
	}
	
	// subject?
	if ( (style & lgs_subject) && sub != 0 )
	{
		sprintf(tmp, "[%-8.8s]", sub);
		strcat(wmsg, tmp);
	}

	if ( wmsg[0] )
		strcat(wmsg, " ");
	strcat(wmsg, msg);

	// has CR & LF ?
	int msg_len = (int)strlen(wmsg);
	if ( style & lgs_crlf )
	{
		char* p = wmsg + msg_len - 1;
		if ( *p != '\n' && *p != '\r' )
			wmsg[msg_len++] = '\n', wmsg[msg_len] = 0;
	}

	// Writing file log message
	if ( (config_.output() & lgo_file) != 0 )
	{
		LOG_GUARD(Log_Mutex, lock_);
		try
		{
			if (log_io_ == NULL) {
				log_io_ = new IO;
				call_delete_io_ = true;
				log_io_->mutex(lock_);

				// 获得目录
				char log_dir[MAX_PATH]; log_dir[0] = 0;
				IFLY_LOG::pathname_to_path(log_dir, config_.file_name());
				if ( log_dir[0] != 0 )
					IFLY_LOG::create_directory(log_dir);

				// 初始化
				int ret = log_io_->open(config_.file_name(), lock_, config_.mode(), NULL);
				if ( ret != 0 ) {
					printf("\nLog_Impl_T<IO, LOCK, CFG>::log_msg_i: Fail to log_io_->open, ret=%d\n", ret);
					return;
				}

				// 写文件头信息
				if ( config_.has_head() )
					write_head();

				// config info
				log_io_->config(&config_);
			} else {
				// 备份旧的日志文件
				if ( log_io_->size() + msg_len > config_.max_size() )
					back_file();
			}

			// Writing message
			if ( config_.encrypt() )
			{
				encrypt(wmsg, msg_len);
				log_io_->write(wmsg, msg_len + 1);
			}
			else if ( config_.charset() == lgc_wchar )
				log_io_->write(IFLY_LOG::char2wchar(wmsg).c_str());
			else
				log_io_->write(wmsg);

			// flush or not
			if ( config_.flush_always() )
				log_io_->flush();
		}
		catch (...)
		{
			printf("\nLog_Impl_T<IO, LOCK, CFG>::log_msg_i catch exception CATCHED.\n");
		}
	}

	// Write console log message
	if ( config_.output() & lgo_console )
		if ( config_.console_level() & lvl )
		{
			if ( config_.charset() == lgc_wchar )
				wprintf(L"%s", IFLY_LOG::char2wchar(wmsg).c_str());
			else
				printf("%s", wmsg);
		}

#ifdef WIN32
	// Write to debugger
	if ( config_.output() & lgo_debugger )
		OutputDebugStringA(wmsg);
	// Write message box message
	if ( config_.output() & lgo_msgbox ) {
		MessageBoxA(0, wmsg, config_.title(),
			MB_OK | (lvl == lgl_error ? MB_ICONSTOP : MB_ICONINFORMATION) );
	}
#endif /* #ifdef WIN32 */
}

template <typename IO, typename LOCK, typename CFG>
void Log_Impl_T<IO, LOCK, CFG>::log_msg_i(const char * sub, LOG_LEVEL lvl, int style, const wchar_t * msg)
{
	// 检查配置是否修改过
	check_config();

	// 样式处理
	if ( style == 0 )
		style = config_.style();

	// 是否输出
	if ( config_.output() == lgo_none )
		return ;
	// 错误等级是否过滤
	if ( ( lvl & config_.level() ) == 0 )
		return ;

	// 变参
	wchar_t wmsg[LOG_MAXMSGLEN], tmp[200];
	wmsg[0] = 0; tmp[0] = 0;

	// has time?
	if ( style & lgs_time )
	{
		char tm[200]; tm[0] = 0;
		#if !defined(WIN32)
		swprintf(wmsg, sizeof wmsg / sizeof wmsg[0], L"[%ls]", IFLY_LOG::char2wchar(cur_time(tm)).c_str());
		#else // no sun
		swprintf(wmsg, L"[%s]", IFLY_LOG::char2wchar(cur_time(tm)).c_str());
		#endif // end
	}
	else if ( style & lgs_time_hms )
	{
		char tm[200]; tm[0] = 0;
		#if !defined(WIN32)
		swprintf(wmsg, sizeof wmsg / sizeof wmsg[0], L"[%ls]", IFLY_LOG::char2wchar(cur_time(tm, false, true)).c_str());
		#else	// no sun
		swprintf(wmsg, L"[%s]", IFLY_LOG::char2wchar(cur_time(tm, false, true)).c_str());
		#endif	// end
	}

	// has level?
	if ( style & lgs_level )
	{
		#if !defined(WIN32)
		swprintf(tmp, sizeof tmp / sizeof tmp[0], L"[%ls]", IFLY_LOG::char2wchar(level_str(lvl)).c_str());
		#else	// no sun
		swprintf(tmp, L"[%s]", IFLY_LOG::char2wchar(level_str(lvl)).c_str());
		#endif	// end
		wcscat(wmsg, tmp);
	}
	
	// has process ID?
	if ( style & lgs_precessid )
	{
		#if !defined(WIN32)
		swprintf(tmp, sizeof tmp / sizeof tmp[0], L"[Px%04x]", IFLY_LOG::getpid());
		#else	// no sun
		swprintf(tmp, L"[Px%04x]", IFLY_LOG::getpid());
		#endif	// end
		wcscat(wmsg, tmp);
	}
	
	// has thread ID?
	if ( style & lgs_threadid )
	{
		#if !defined(WIN32)
		swprintf(tmp, sizeof tmp / sizeof tmp[0], L"[Tx%04x]", IFLY_LOG::thr_self());
		#else	// no sun
		swprintf(tmp, L"[Tx%04x]", IFLY_LOG::thr_self());
		#endif  // end
		wcscat(wmsg, tmp);
	}
	
	// subject?
	if ( (style & lgs_subject) && sub != 0 )
	{
		#if !defined(WIN32)
		swprintf(tmp, sizeof tmp / sizeof tmp[0], L"[%-8.8ls]", sub);
		#else	// no sun
		swprintf(tmp, L"[%-8.8s]", sub);
		#endif	// end
		wcscat(wmsg, tmp);
	}

	wcscat(wmsg, L" ");
	wcscat(wmsg, msg);

	// has CR & LF ?
	int msg_len = (int) wcslen(wmsg);
	if ( style & lgs_crlf )
	{
		wchar_t* p = wmsg + msg_len - 1;
		if ( *p != L'\n' && *p != L'\r' )
			wmsg[msg_len++] = L'\n', wmsg[msg_len] = 0;
	}

	// Writing file log message
	if ( (config_.output() & lgo_file) != 0 )
	{
		LOG_GUARD(Log_Mutex, lock_);
		try
		{
			if (log_io_ == NULL) {
				log_io_ = new IO;
				call_delete_io_ = true;
				log_io_->mutex(lock_);

				// 获得目录
				char log_dir[MAX_PATH]; log_dir[0] = 0;
				IFLY_LOG::pathname_to_path(log_dir, config_.file_name());
				if ( log_dir[0] != 0 )
					IFLY_LOG::create_directory(log_dir);

				// 初始化
				int ret = log_io_->open(config_.file_name(), lock_, config_.mode(), NULL);
				if ( ret != 0 ) {
					printf("\nLog_Impl_T<IO, LOCK, CFG>::log_msg_i: Fail to log_io_->open, ret=%d\n", ret);
					return;
				}

				// 写文件头信息
				if ( config_.has_head() )
					write_head();

				// config info
				log_io_->config(&config_);
			} else {
				// 备份旧的日志文件
				if ( log_io_->size() + msg_len > config_.max_size() )
					back_file();
			}

			// Writing msg
			if ( config_.encrypt() )
			{
				encrypt(wmsg, msg_len);
				log_io_->write(wmsg, msg_len + 1);
			}
			else if ( config_.charset() == lgc_mbcs )
				log_io_->write(IFLY_LOG::wchar2char(wmsg).c_str());
			else
				log_io_->write(wmsg);

			// flush or not
			if ( config_.flush_always() )
				log_io_->flush();
		}
		catch (...)
		{
			printf("\nLog_Impl_T<IO, LOCK, CFG>::log_msg_i_w catch exception CATCHED.\n");
		}
	}

	// Write console log message
	if ( config_.output() & lgo_console )
		if ( config_.console_level() & lvl )
		{
			if ( config_.charset() == lgc_mbcs )
				printf("%s", IFLY_LOG::wchar2char(wmsg).c_str());
			else
				wprintf(L"%s", wmsg);
		}

#ifdef WIN32
	// Write to debugger
	if ( config_.output() & lgo_debugger )
		OutputDebugStringW(wmsg);
	// Write message box message
	if ( config_.output() & lgo_msgbox ) {
		MessageBoxW(0, wmsg, L"Logger",
			MB_OK | (lvl == lgl_error ? MB_ICONSTOP : MB_ICONINFORMATION) );
	}
#endif /* #ifdef WIN32 */
}

/** 
 * @brief	FILL_OPT_PARAMS(msg, fmt)
 *			填充变参信息的宏
 */
//vsprintf(msg, /* sizeof msg, */fmt, lst);

#if defined(WIN32)
# if _MSC_VER >= 1400
#  define FILL_OPT_PARAMS(msg, fmt, lvl)		\
	if ( config_.output() == lgo_none )			\
		return ;								\
	msg[0] = 0;									\
	if ( ( config_.level() & lvl ) != 0 )		\
	{											\
		va_list lst;							\
		va_start(lst, fmt);						\
		stringFormatV(msg, fmt, lst);			\
		va_end(lst);							\
	}
#  define FILL_OPT_PARAMS_W(msg, fmt, lvl)		\
	if ( config_.output() == lgo_none )			\
		return ;								\
	msg[0] = 0;									\
	if ( ( config_.level() & lvl ) != 0 )		\
	{											\
		va_list lst;							\
		va_start(lst, fmt);						\
		wstringFormatV(msg, fmt, lst);			\
		va_end(lst);							\
	}
# else  /* vc 7 or 6 */
#  define FILL_OPT_PARAMS(msg, fmt, lvl)		\
	if ( config_.output() == lgo_none )			\
		return ;								\
	msg[0] = 0;									\
	if ( ( config_.level() & lvl ) != 0 )		\
	{											\
		va_list lst;							\
		va_start(lst, fmt);						\
		stringFormatV(msg, fmt, lst);			\
		va_end(lst);							\
	}
#  define FILL_OPT_PARAMS_W(msg, fmt, lvl)		\
	if ( config_.output() == lgo_none )			\
		return ;								\
	msg[0] = 0;									\
	if ( ( config_.level() & lvl ) != 0 )		\
	{											\
		va_list lst;							\
		va_start(lst, fmt);						\
		wstringFormatV(msg, fmt, lst);			\
		va_end(lst);							\
	}
#endif  /* vc 7 or 6 */
#else	/* Linux */
# define FILL_OPT_PARAMS(msg, fmt, lvl)			\
	if ( config_.output() == lgo_none )			\
		return ;								\
	msg[0] = 0;									\
	if ( ( config_.level() & lvl ) != 0 )		\
	{											\
		va_list lst;							\
		va_start(lst, fmt);						\
        stringFormatV(msg, fmt, lst);			\
		va_end(lst);							\
	}
# define FILL_OPT_PARAMS_W(msg, fmt, lvl)		\
	if ( config_.output() == lgo_none )			\
		return ;								\
	msg[0] = 0;									\
	if ( ( config_.level() & lvl ) != 0 )		\
	{											\
		va_list lst;							\
		va_start(lst, fmt);						\
		wstringFormatV(msg, fmt, lst);			\
		va_end(lst);							\
	}
#endif // end

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_msg
 *
 *	记录日志信息，带主题信息的日志记录实现函数体
 * 
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @param	int sub				- [in] 主题信息标识
 * @param	ISF_LOG_LEVEL lvl	- [in] 日志的信息等级
 * @param	int style			- [in] 写入的格式
 * @param	const char* fmt		- [in] 格式化字符串
 * @param	...					- [in] 后续参数信息内容
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_msg(int sub, LOG_LEVEL lvl, int style, const char* fmt, ...)
{
	// 是否输出
	if ( config_.output() == lgo_none )
		return ;
	// 错误等级是否过滤
	if ( ( lvl & config_.level() ) == 0 )
		return ;
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	// 变参
	std::string szBuff;
	std::string msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = subject(sub, sub_buf, sizeof sub_buf);
	FILL_OPT_PARAMS(szBuff, fmt, lvl);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lvl, style, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lvl, style, msg.c_str());
	} else {
		log_msg_i(buf, lvl, style, szBuff.c_str());
	}
}


/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_msg
 *
 *	记录日志信息，带主题信息的日志记录实现函数体
 * 
 * @author	jdyu
 * @date	2004-6-1
 * @return	No return value.
 * @see		log_msg
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_msg(const char* sub, LOG_LEVEL lvl, int style, const char* fmt, ...)
{
	// 是否输出
	if ( config_.output() == lgo_none )
		return ;
	// 错误等级是否过滤
	if ( ( lvl & config_.level() ) == 0 )
		return ;
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	// 变参
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lvl);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop){
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(sub, lvl, style, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(sub, lvl, style, msg.c_str());
	} else {
		log_msg_i(sub, lvl, style, szBuff.c_str());
	}
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_msg
 *
 *	记录无任何主题信息，没有变参
 * 
 * @author	jdyu
 * @date	2004-6-1
 * @return	No return value.
 * @see		log_msg
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_msg(const char* msg)
{
	// 是否输出
	if ( config_.output() == lgo_none )
		return ;
	log_msg_i(0, lgl_info, lgs_none, msg);
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_msg
 *
 *	记录无任何主题信息，没有变参
 * 
 * @author	jdyu
 * @date	2004-6-1
 * @return	No return value.
 * @see		log_msg
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_msg(const wchar_t* msg)
{
	// 是否输出
	if ( config_.output() == lgo_none )
		return ;
	log_msg_i(0, lgl_info, lgs_none, msg);
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::write_msg
 *
 *	写入没有任何格式信息的串
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	void - No return value.
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::write_msg(const char* fmt, ...)
{
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_info);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_info, lgs_none, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_info, lgs_none, msg.c_str());
	} else {
		log_msg_i(0, lgl_info, lgs_none, szBuff.c_str());
	}
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_crit
 *
 *	没有主题的，记录报警信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_crit(const char* fmt, ...)
{
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_crit);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_crit, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_crit, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_crit, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_error
 *
 *	没有主题的，记录错误信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_error(const char* fmt, ...)
{
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_error);
	if (szBuff.length() > static_cast<unsigned int>(LOG_EACHMSGLEN)) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_error, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_error, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_error, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_warn
 *
 *	记录没有主题的警告信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_warn(const char* fmt, ...)
{
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_warning);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_warning, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_warning, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_warning, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_info
 *
 *	记录没有主题的一般信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_info(const char* fmt, ...)
{
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_info);
	if (szBuff.length() > static_cast<unsigned int>(LOG_EACHMSGLEN)) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_info, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_info, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_info, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_verbose
 *
 *	记录没有主题的一般信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_verbose(const char* fmt, ...)
{
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_verbose);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_verbose, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_verbose, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_verbose, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::log_debug
 *
 * 	记录调试信息
 * 
 * @author	jdyu
 * @date	2004-8-10
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_debug(const char* fmt, ...)
{
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_debug);
	if (szBuff.length() > static_cast<unsigned int>(LOG_EACHMSGLEN)) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_debug, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_debug, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_debug, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	log_perf
 *
 * 	记录性能信息
 * 
 * @author	jdyu
 * @date	2004-8-30
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_perf(const char* fmt, ...)
{
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_perf);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_perf, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_perf, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_perf, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	log_trace
 *
 * 	记录调用信息
 * 
 * @author	jdyu
 * @date	2004-8-30
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_trace(const char* fmt, ...)
{
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_trace);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_trace, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_trace, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_trace, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	log_spec
 *
 * 	记录特别信息
 * 
 * @author	jdyu
 * @date	2009-4-22
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_spec(const char* fmt, ...)
{
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_spec);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_spec, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_spec, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_spec, lgs_default, szBuff.c_str());
	}
}


/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::write_msg
 *
 *	宽字符版本写入没有任何格式信息的串
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	void - No return value.
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::write_msg(const wchar_t* fmt, ...)
{
	std::wstring szBuff;
	std::wstring msg;
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_info);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_info, lgs_none, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_info, lgs_none, msg.c_str());
	} else {
		log_msg_i(0, lgl_info, lgs_none, szBuff.c_str());
	}
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_crit
 *
 *	宽字符版本没有主题的，记录报警信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_crit(const wchar_t* fmt, ...)
{
	std::wstring szBuff;
	std::wstring msg;
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_crit);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_crit, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_crit, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_crit, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_error
 *
 *	宽字符版本没有主题的，记录错误信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_error(const wchar_t* fmt, ...)
{
	std::wstring szBuff;
	std::wstring msg;
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_error);
	if (szBuff.length() > static_cast<unsigned int>(LOG_EACHMSGLEN)) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_error, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_error, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_error, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_warn
 *
 *	宽字符版本记录没有主题的警告信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_warn(const wchar_t* fmt, ...)
{
	std::wstring szBuff;
	std::wstring msg;
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_warning);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_warning, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_warning, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_warning, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_info
 *
 *	宽字符版本记录没有主题的一般信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_info(const wchar_t* fmt, ...)
{
	std::wstring szBuff;
	std::wstring msg;
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_info);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_info, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_info, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_info, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_verbose
 *
 *	宽字符版本记录没有主题的一般信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_verbose(const wchar_t* fmt, ...)
{
	std::wstring szBuff;
	std::wstring msg;
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_verbose);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_verbose, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_verbose, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_verbose, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::log_debug
 *
 * 	宽字符版本记录调试信息
 * 
 * @author	jdyu
 * @date	2004-8-10
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_debug(const wchar_t* fmt, ...)
{
	std::wstring szBuff;
	std::wstring msg;
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_debug);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_debug, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_debug, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_debug, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	log_perf
 *
 * 	宽字符版本记录性能信息
 * 
 * @author	jdyu
 * @date	2004-8-30
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_perf(const wchar_t* fmt, ...)
{
	std::wstring szBuff;
	std::wstring msg;
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_perf);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_perf, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_perf, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_perf, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	log_trace
 *
 * 	宽字符版本记录调用信息
 * 
 * @author	jdyu
 * @date	2004-8-30
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_trace(const wchar_t* fmt, ...)
{
	std::wstring szBuff;
	std::wstring msg;
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_trace);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_trace, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_trace, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_trace, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	log_spec
 *
 * 	宽字符版本记录特别信息
 * 
 * @author	jdyu
 * @date	2009-4-22
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_spec(const wchar_t* fmt, ...)
{
	std::wstring szBuff;
	std::wstring msg;
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_spec);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(0, lgl_spec, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(0, lgl_spec, lgs_default, msg.c_str());
	} else {
		log_msg_i(0, lgl_spec, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_crit
 *
 *	记录有主题的关键信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @param	int sub	- [in] 主题标志，通过subject()函数实现
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_crit(int sub, const char* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;
	std::string szBuff;
	std::string msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = subject(sub, sub_buf, sizeof sub_buf);
	FILL_OPT_PARAMS(szBuff, fmt, lgl_crit);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_crit, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_crit, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_crit, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_error
 *
 *	记录有主题的错误信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @param	int sub	- [in] 主题标志，通过subject()函数实现
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_error(int sub, const char* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;
	std::string szBuff;
	std::string msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = subject(sub, sub_buf, sizeof sub_buf);
	FILL_OPT_PARAMS(szBuff, fmt, lgl_error);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_error, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_error, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_error, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_warn
 *
 *	记录有主题的警告信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_warn(int sub, const char* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;
	std::string szBuff;
	std::string msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = subject(sub, sub_buf, sizeof sub_buf);
	FILL_OPT_PARAMS(szBuff, fmt, lgl_warning);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_warning, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_warning, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_warning, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_info
 *
 *	记录有主题的一般信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_info(int sub, const char* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;
	std::string szBuff;
	std::string msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = subject(sub, sub_buf, sizeof sub_buf);
	FILL_OPT_PARAMS(szBuff, fmt, lgl_info);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_info, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_info, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_info, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_verbose
 *
 *	记录有主题的不重要信息
 *
 * @author	jdyu
 * @date	2004年7月8日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_verbose(int sub, const char* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	std::string szBuff;
	std::string msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = subject(sub, sub_buf, sizeof sub_buf);
	FILL_OPT_PARAMS(szBuff, fmt, lgl_verbose);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_verbose, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_verbose, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_verbose, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	log_debug
 *
 * 	记录有主题的调试信息
 * 
 * @author	jdyu
 * @date	2004-8-10
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_debug(int sub, const char* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	std::string szBuff;
	std::string msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = subject(sub, sub_buf, sizeof sub_buf);
	FILL_OPT_PARAMS(szBuff, fmt, lgl_debug);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_debug, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_debug, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_debug, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::log_perf
 *
 * 	记录有主题的性能信息
 * 
 * @author	jdyu
 * @date	2004-8-30
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_perf(int sub, const char* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	std::string szBuff;
	std::string msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = subject(sub, sub_buf, sizeof sub_buf);
	FILL_OPT_PARAMS(szBuff, fmt, lgl_perf);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_perf, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_perf, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_perf, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::log_trace
 *
 * 	记录有主题的调用信息
 * 
 * @author	jdyu
 * @date	2004-8-30
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_trace(int sub, const char* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	std::string szBuff;
	std::string msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = subject(sub, sub_buf, sizeof sub_buf);
	FILL_OPT_PARAMS(szBuff, fmt, lgl_trace);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_trace, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_trace, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_trace, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::log_spec
 *
 * 	记录有主题的特别信息
 * 
 * @author	jdyu
 * @date	2004-8-30
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_spec(int sub, const char* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	std::string szBuff;
	std::string msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = subject(sub, sub_buf, sizeof sub_buf);
	FILL_OPT_PARAMS(szBuff, fmt, lgl_spec);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_spec, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_spec, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_spec, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_crit
 *
 *	记录有主题的报警信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @param	int sub	- [in] 主题标志，通过subject()函数实现
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_crit(int sub, const wchar_t * fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	std::wstring szBuff;
	std::wstring msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = IFLY_LOG::char2wchar(subject(sub, sub_buf, sizeof sub_buf));
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_crit);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_crit, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_crit, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_crit, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_error
 *
 *	记录有主题的错误信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @param	int sub	- [in] 主题标志，通过subject()函数实现
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_error(int sub, const wchar_t* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	std::wstring szBuff;
	std::wstring msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = IFLY_LOG::char2wchar(subject(sub, sub_buf, sizeof sub_buf));
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_error);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_error, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_error, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_error, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_warn
 *
 *	记录有主题的警告信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_warn(int sub, const wchar_t* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	std::wstring szBuff;
	std::wstring msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = IFLY_LOG::char2wchar(subject(sub, sub_buf, sizeof sub_buf));
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_warning);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_warning, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_warning, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_warning, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_info
 *
 *	记录有主题的一般信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_info(int sub, const wchar_t* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	std::wstring szBuff;
	std::wstring msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = IFLY_LOG::char2wchar(subject(sub, sub_buf, sizeof sub_buf));
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_info);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_info, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_info, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_info, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_verbose
 *
 *	记录有主题的不重要信息
 *
 * @author	jdyu
 * @date	2004年7月8日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_verbose(int sub, const wchar_t* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	std::wstring szBuff;
	std::wstring msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = IFLY_LOG::char2wchar(subject(sub, sub_buf, sizeof sub_buf));
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_verbose);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_verbose, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_verbose, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_verbose, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	log_debug
 *
 * 	记录有主题的调试信息
 * 
 * @author	jdyu
 * @date	2004-8-10
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_debug(int sub, const wchar_t* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	std::wstring szBuff;
	std::wstring msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = IFLY_LOG::char2wchar(subject(sub, sub_buf, sizeof sub_buf));
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_debug);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_debug, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_debug, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_debug, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::log_perf
 *
 * 	记录有主题的性能信息
 * 
 * @author	jdyu
 * @date	2004-8-30
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_perf(int sub, const wchar_t* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	std::wstring szBuff;
	std::wstring msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = IFLY_LOG::char2wchar(subject(sub, sub_buf, sizeof sub_buf));
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_perf);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_perf, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_perf, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_perf, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::log_trace
 *
 * 	记录有主题的调用信息
 * 
 * @author	jdyu
 * @date	2004-8-30
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_trace(int sub, const wchar_t* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	std::wstring szBuff;
	std::wstring msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = IFLY_LOG::char2wchar(subject(sub, sub_buf, sizeof sub_buf));
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_trace);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_trace, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_trace, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_trace, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::log_spec
 *
 * 	记录有主题的特别信息
 * 
 * @author	jdyu
 * @date	2004-8-30
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_spec(int sub, const wchar_t* fmt, ...)
{
	// 模块id是否过滤
	if ( !sub_output(sub) )
		return ;

	std::wstring szBuff;
	std::wstring msg;
	char sub_buf[SUBJECT_SIZE];
	char* buf = IFLY_LOG::char2wchar(subject(sub, sub_buf, sizeof sub_buf));
	FILL_OPT_PARAMS_W(szBuff, fmt, lgl_spec);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//msg太长，将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop) {
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(buf, lgl_spec, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(buf, lgl_spec, lgs_default, msg.c_str());
	} else {
		log_msg_i(buf, lgl_spec, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_crits
 *
 *	记录有主题的报警信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @param	int sub	- [in] 主题标志，通过subject()函数实现
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_crits(const char* sub, const char* fmt, ...)
{
	if ( !sub_output(sub) )
		return ;
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_crit);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop){
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(sub, lgl_crit, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(sub, lgl_crit, lgs_default, msg.c_str());
	} else {
		log_msg_i(sub, lgl_crit, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_errors
 *
 *	记录有主题的错误信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @param	int sub	- [in] 主题标志，通过subject()函数实现
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_errors(const char* sub, const char* fmt, ...)
{
	if ( !sub_output(sub) )
		return ;
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_error);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop){
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(sub, lgl_error, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(sub, lgl_error, lgs_default, msg.c_str());
	} else {
		log_msg_i(sub, lgl_error, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_warns
 *
 *	记录有主题的警告信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_warns(const char* sub, const char* fmt, ...)
{
	if ( !sub_output(sub) )
		return ;
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_warning);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop){
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(sub, lgl_warning, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(sub, lgl_warning, lgs_default, msg.c_str());
	} else {
		log_msg_i(sub, lgl_warning, lgs_default, szBuff.c_str());
	}
	flush();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_infos
 *
 *	记录有主题的一般信息
 *
 * @author	jdyu
 * @date	2004-5-29
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_infos(const char* sub, const char* fmt, ...)
{
	if ( !sub_output(sub) )
		return ;
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_info);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop){
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(sub, lgl_info, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(sub, lgl_info, lgs_default, msg.c_str());
	} else {
		log_msg_i(sub, lgl_info, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::log_verboses
 *
 *	记录有主题的不重要信息
 *
 * @author	jdyu
 * @date	2004-5-29
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_verboses(const char* sub, const char* fmt, ...)
{
	if ( !sub_output(sub) )
		return ;
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_verbose);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop){
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(sub, lgl_verbose, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(sub, lgl_verbose, lgs_default, msg.c_str());
	} else {
		log_msg_i(sub, lgl_verbose, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	log_debugs
 *
 * 	记录有主题的调试信息
 *
 * @author	jdyu
 * @date	2004-8-10
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_debugs(const char* sub, const char* fmt, ...)
{
	if ( !sub_output(sub) )
		return ;
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_debug);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop){
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(sub, lgl_debug, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(sub, lgl_debug, lgs_default, msg.c_str());
	} else {
		log_msg_i(sub, lgl_debug, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::log_perfs
 *
 * 	记录有主题的性能信息
 * 
 * @author	jdyu
 * @date	2004-8-30
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_perfs(const char* sub, const char* fmt, ...)
{
	if ( !sub_output(sub) )
		return ;
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_perf);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop){
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(sub, lgl_perf, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(sub, lgl_perf, lgs_default, msg.c_str());
	} else {
		log_msg_i(sub, lgl_perf, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::log_traces
 *
 * 	记录有主题的调用信息
 * 
 * @author	jdyu
 * @date	2004-8-30
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_traces(const char* sub, const char* fmt, ...)
{
	if ( !sub_output(sub) )
		return ;
	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_trace);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop){
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(sub, lgl_trace, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(sub, lgl_trace, lgs_default, msg.c_str());
	} else {
		log_msg_i(sub, lgl_trace, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::log_specs
 *
 * 	记录有主题的特别信息
 * 
 * @author	jdyu
 * @date	2004-8-30
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void LOG_CDECL Log_Impl_T<IO, LOCK, CFG>::log_specs(const char* sub, const char* fmt, ...)
{
	if ( !sub_output(sub) )
		return ;

	std::string szBuff;
	std::string msg;
	FILL_OPT_PARAMS(szBuff, fmt, lgl_spec);
	if (szBuff.length() > LOG_EACHMSGLEN) {
		//将msg分成若干段，分别往下传
		int iMsgLen			= szBuff.length();					//msg的总长度
		int iEachRowMsgLen	= LOG_EACHMSGLEN;	//每一段msg的长度，需预留LOG_HEADMSGLEN，用来存储时间、线程id等信息
		int iLoopMax		= iMsgLen/iEachRowMsgLen;			//可以分多少段msg
		int iModMsgLen		= iMsgLen%iEachRowMsgLen;			//msg均分后余下的msg长度
		int loop;
		for (loop = 0; loop != iLoopMax; ++loop){
			msg = szBuff.substr(loop*iEachRowMsgLen, iEachRowMsgLen);
			log_msg_i(sub, lgl_spec, lgs_default, msg.c_str());
		}
		msg = szBuff.substr(loop*iEachRowMsgLen, iModMsgLen);
		log_msg_i(sub, lgl_spec, lgs_default, msg.c_str());
	} else {
		log_msg_i(sub, lgl_spec, lgs_default, szBuff.c_str());
	}
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::write_head
 *
 *	写文件头
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void Log_Impl_T<IO, LOCK, CFG>::write_head(bool new_file /* = true */)
{
	char buf[MAX_PATH];
	if ( config_.style() != lgs_none )
	{
		std::string mod_path;
		if ( config_.module() != 0 )
			mod_path = IFLY_LOG::mod_path_name(config_.module());
		std::string exe_path = IFLY_LOG::mod_path_name();
		if ( mod_path == exe_path )
			mod_path = "";

		char head[4096];
		sprintf(head,
			"=============================================================\n"
			"\tiFlyTEK log file\n"
			"\tSubject :    %s\n"
			"\t%s-Time :    %s\n"
			"\tApplication: %s %s\n"
			"\tModule:      %s %s\n"
			"\tPID: %d (0x%04x) Version: %s\n"
			"=============================================================\n",
			config_.title(),
			new_file ? "Created" : "Continued", cur_time(buf),
			exe_path.c_str(), exe_path.length() ? IFLY_LOG::file_mtime_str(exe_path.c_str()).c_str() : " ",
			mod_path.c_str(), mod_path.length() ? IFLY_LOG::file_mtime_str(mod_path.c_str()).c_str() : " ",
			IFLY_LOG::getpid(), IFLY_LOG::getpid(), IFLY_LOG::mod_version(mod_path.length() == 0 ? exe_path.c_str() : mod_path.c_str()).c_str());

		if ( config_.charset() == lgc_wchar )
			log_io_->write(IFLY_LOG::char2wchar(head).c_str());
		else
			log_io_->write(head);
		log_io_->flush();
	}
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::write_tail
 *
 *	写文件尾
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void Log_Impl_T<IO, LOCK, CFG>::write_tail(bool end_file /* = true */)
{
	char times[MAX_PATH];
	if ( config_.style() != lgs_none )
	{
		char tail[4096];
		sprintf(tail,
			"=============================================================\n"
			"\t%s %s-Time: %s\n"
			"\tPID: %d (0x%04x)\n"
			"=============================================================\n",
			config_.title(), end_file ? "End" : "Continue", cur_time(times),
			IFLY_LOG::getpid(), IFLY_LOG::getpid());

		if ( config_.charset() == lgc_wchar )
			log_io_->write(IFLY_LOG::char2wchar(tail).c_str());
		else
			log_io_->write(tail);
		log_io_->flush();
	}
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::write_bound
 *
 *	写一个分隔行
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @param	char sep = '-'	- [in] 分割字符
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void Log_Impl_T <IO, LOCK, CFG> ::write_bound(char sep /* = '-' */)
{
	if ( log_io_->handle() )
	{
		char line[300];
		int i = 0;
		for ( ; i < 80; i++ )
			line[i] = sep;
		line[i] = 0;
		
		LOG_GUARD(Log_Mutex, lock_);

		if ( config_.charset() == lgc_wchar )
		{
			log_io_->write(IFLY_LOG::char2wchar(line).c_str());
			log_io_->write(L"\n");
		}
		else
		{
			log_io_->write(line);
			log_io_->write("\n");
		}
		log_io_->flush();
	}
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::lock
 *  
 *  外部可以调用锁定日志的接口
 *  
 * @author	jdyu
 * @date	2006-11-20
 * @return	No return value.
 * @param	int time_out  = -1	- [in] 
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
int Log_Impl_T<IO, LOCK, CFG>::lock(int time_out /* = -1 */)
{
	if ( lock_ )
		return lock_->acquire(time_out);
	return -1;
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::unlock
 *  
 *  外部可以调用解锁日志的接口
 *  
 * @author	jdyu
 * @date	2006-11-20
 * @return	No return value.
 * @param	int time_out  = -1	- [in] 
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
int Log_Impl_T<IO, LOCK, CFG>::unlock(void)
{
	if ( lock_ )
		return lock_->release();
	return -1;
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::empty
 *
 *	清空日志文件
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	int	- Return 0 in success
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
int Log_Impl_T<IO, LOCK, CFG>::empty(void)
{
	LOG_GUARD(Log_Mutex, lock_);
	if ( log_io_ == 0 )
		return -1;
	return log_io_->empty();
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::write
 *
 *	二进制方式写入数据
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	No return value.
 * @param	const void* data	- [in] 待写入的数据缓冲区
 * @param	int size			- [in] 待写入的数据字节数
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
int Log_Impl_T<IO, LOCK, CFG>::write(const void* data, int size)
{
	char * bf = 0; char bbf[10*1024];
	LOG_GUARD(Log_Mutex, lock_);
	if ( log_io_ == 0 )
		return -1;

	// 若需要加密，需要拷贝一份数据
	if ( config_.encrypt() )
	{
		if ( static_cast<unsigned int>(size) > sizeof bbf )
		{
			bf = new char[size];
			memcpy(bf, data, size);
			data = bf;
		}
		else
		{
			memcpy(bbf, data, size);
			data = bbf;
		}
		size = encrypt((char*)data, size);
	}

	// write data
	int ret = log_io_->write(data, size);

	if ( bf )
		delete [] bf, bf = 0;
	return ret;
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::enable
 * 
 *  日志是否被禁止了	
 * 
 * @author	jdyu
 * @date	2005-4-7
 * @return	int	- Return true or false
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
bool Log_Impl_T<IO, LOCK, CFG>::enable(void) const
{
	return config_.output() != lgo_none;
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::size
 * 
 *  日志当前的尺寸	
 * 
 * @author	jdyu
 * @date	2005-4-7
 * @return	int	- Return 0 in success, otherwise return error code.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
size_t Log_Impl_T<IO, LOCK, CFG>::size(void) const
{
	LOG_GUARD(Log_Mutex, lock_);
	if ( log_io_ == 0 )
		return 0;
	return log_io_->size();
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::cur_time
 * 
 *  获得当前的时间	
 * 
 * @author	jdyu
 * @date	2005-4-7
 * @return	返回存放日期的缓冲区地址
 * @param	int sub	- [in] 指定的主题ID
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
const char* Log_Impl_T<IO, LOCK, CFG>::cur_time(char tmbuf[], bool no_path_char /*= false*/, bool sht_fmt /* = false */) const
{
	return IFLY_LOG::cur_time(tmbuf, no_path_char, sht_fmt, config_.start_time(), config_.start_tick());
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::subject
 *  
 *  从一个主题ID获得主题（派生类需要重新实现该函数！）
 *  
 * @author	jdyu
 * @date	2006-9-28
 * @return	const char* 
 * @param	int sub	- [in] 主题
 * @param	char * buf	- [in] 使用的缓冲区，长度必须大于8
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
const char* Log_Impl_T<IO, LOCK, CFG>::subject(int sub, char * buf, int len) const
{
	if ( buf && len > 8 )
		sprintf(buf, "%8x", sub);
	return buf;
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::level_str
 *  
 *  获得level对应的字符串
 *  
 * @author	jdyu
 * @date	2006-11-24
 * @return	const char* 
 * @param	int lvl	- [in] 日志等级
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
const char * Log_Impl_T<IO, LOCK, CFG>::level_str(int lvl) const
{
	/* 日志等级的字符串 */
	static const char LOG_LVLSTRS[][16] =
	{
		"UNK",		// 0
		"CRT",		// 1
		"ERR",		// 2
		"WAN",		// 4
		"INF",		// 8
		"VBS",		// 16
		"DBG",		// 32
		"PRF",		// 64
		"TRC",		// 128
		"SPK"		// 256
	};

	int ind = IFLY_LOG::ln2(lvl) + 1;
	if ( static_cast<unsigned int>(ind) >= sizeof LOG_LVLSTRS / sizeof LOG_LVLSTRS[0] )
		ind = 0;
	return LOG_LVLSTRS[ind];
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::encrypt
 *  
 *  从一个主题ID获得主题（派生类需要重新实现该函数！）
 *  
 * @author	jdyu
 * @date	2007-1-26
 * @return	const char* 
 * @param	int sub		- [in] 主题
 * @param	char * buf	- [in] 使用的缓冲区，长度必须大于8
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
int Log_Impl_T<IO, LOCK, CFG>::encrypt(void * msg, int len)
{
	char * cms = (char *) msg;
	if ( config_.encrypt() != 0 )
	{
		for ( int i = 0; i < len; i++ )
			cms[i] ^= config_.encrypt();
	}
	return len;
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::sub_output
 *
 *	是否需要输出某个主题的过滤函数
 *	派生类可以实现这个函数达到高级的过滤功能
 *
 * @author	jdyu
 * @date	2004-5-29
 * @return	bool	- Return true in success, otherwise return false.
 * @param	int sub	- [in] 指定的主题ID
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
bool Log_Impl_T<IO, LOCK, CFG>::sub_output(int sub) const
{
	if ( sub >= 32 )
		return true;

	if ( ( config_.filter() & (1 << sub) ) == 0 )
		return false;
	else
		return true;
}

/** 
 * @brief	Log_Impl_T<IO, LOCK, CFG>::sub_output
 *
 *	判断指定的主题是否可以输出
 *	派生类可以实现这个函数达到高级的过滤功能
 *
 * @author	jdyu
 * @date	2004-5-29
 * @return	bool	- Return true in success, otherwise return false.
 * @param	const char* sub	- [in] 指定的主题
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
bool Log_Impl_T<IO, LOCK, CFG>::sub_output(const char* /* sub */) const
{
	// 派生类可以重新实现该函数
	return true;
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::flush
 * 
 * 	写到磁盘中
 * 
 * @author	jdyu
 * @date	2005-8-22
 * @return	No return value.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
void Log_Impl_T<IO, LOCK, CFG>::flush(void)
{
	LOG_GUARD(Log_Mutex, lock_);
	if ( log_io_ != 0 )
		log_io_->flush();
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::open
 *
 *  读取配置信息并初始化	
 * 
 * @author	jdyu
 * @date	2004-7-6
 * @return	int	- Return 0 in success, otherwise return error code.
 * @param	const char* cfg_file  = 0	- [in] 配置文件名称
 * @param	const char* key  = 0		- [in] 键位置
 * @param	void* hmod  = 0				- [in] 模块句柄
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
int Log_Singleton<IO, LOCK, CFG>::open(const char* cfg_file, const char* key /*= 0*/, const CFG* dcfg /*= 0*/, LOG_HANDLE hmod /*= -1*/, Log_IO* io /*= 0*/, void* rev /*= 0*/)
{
	CFG cfg;
	cfg.output(lgo_file);

	if ( io )
		cfg.set(io->config());
	if ( dcfg )
		cfg = *dcfg;

	const char * cfile = 0;
	if ( cfg_file )
		cfile = cfg_file;
	else if ( io && io->config() && key )
		cfile = io->config()->cfg_file();
	cfg.read_config(key, cfile);

	if ( hmod )
		cfg.module(hmod);

	return open(cfg, io, rev);
}
// 简化版初始化，比较常用
template <typename IO, typename LOCK, typename CFG>
int Log_Singleton<IO, LOCK, CFG>::open(const CFG & cfg, const char * cfg_file /* = 0 */, const char * key /* = 0 */)
{
	// 调用上面的open
	return open(cfg_file, key, &cfg, LOG_INVALID_HANDLE, 0, 0);
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::open
 *
 *  读取配置信息并初始化	
 * 
 * @author	jdyu
 * @date	2004-7-6
 * @return	int	- Return 0 in success, otherwise return error code.
 * @param	const char* io_lib  = 0	- [in] 日志实现库的路径
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
int Log_Singleton<IO, LOCK, CFG>::open(const char* io_lib, const char* cfg_file, const char* key, const CFG* dcfg, LOG_HANDLE hmod, void* rev /*= 0*/)
{
	CFG cfg;
	cfg.output(lgo_file);
	if ( dcfg )
		cfg = *dcfg;
	
	cfg.read_config(key, cfg_file);
	
	if ( hmod )
		cfg.module(hmod);

	return open(cfg, (Log_IO*)0, rev);
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::open
 *
 *  从指定的配置信息初始化
 *  这个函数直接使用io_impl接口里面的配置信息	
 * 
 * @author	jdyu
 * @date	2004-7-6
 * @return	int	- Return 0 in success, otherwise return error code.
 * @param	const char* io_lib  = 0	- [in] 日志实现库的路径
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
int Log_Singleton<IO, LOCK, CFG>::open(Log_IO * io_impl, LOG_HANDLE mod /*= 0*/, void* rev /*= 0*/)
{
	CFG cfg;
	if ( io_impl == 0 )
		return -1;
	if ( io_impl->config() )
		cfg.set(io_impl->config());
	if ( mod != 0 )
		cfg.module(mod);
	return open(cfg, io_impl, rev);
}

/** 
 * @brief 	Log_Impl_T<IO, LOCK, CFG>::open
 *
 *  从指定的配置信息初始化	
 * 
 * @author	jdyu
 * @date	2004-7-6
 * @return	int	- Return 0 in success, otherwise return error code.
 * @param	const char* io_lib  = 0	- [in] 日志实现库的路径
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
int Log_Singleton<IO, LOCK, CFG>::open(const CFG & cfg, Log_IO * io_impl, void * rev /*= 0*/)
{
	// lock to more safe
	Log_Win32_Process_Mutex lock("iFly_Log_Singleton_Mutex", true);
	lock.acquire();

	// check the instance
	if ( the_parent::instance() != 0 )
	{
		#ifdef _DEBUG
		printf("Log_Singleton<IO, LOCK, CFG>::open already opened.\n");
		#endif // DEBUG
		lock.release();
		return 0;
	}

	char path[MAX_PATH]; path[0] = 0;
	CFG  cfg_tmp = cfg;
	if ( !IFLY_LOG::is_abs_path(cfg.file_name()) )
		cfg_tmp.file_name(IFLY_LOG::cat_path(IFLY_LOG::get_bin_path(path, cfg.module()), cfg.file_name()));

	the_parent::open_singleton();
	int ret = the_parent::instance()->open(&cfg_tmp, (IO*)io_impl, rev);
	lock.release();
	return ret;
}

/** 
 * @brief 	Log_Singleton<IO, LOCK>::close
 *  
 *  关闭日志，并释放单体
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @return	int Log_Singleton<IO,	- Return 0 in success, otherwise return error code.
 * @see		
 */
template <typename IO, typename LOCK, typename CFG>
int Log_Singleton<IO, LOCK, CFG>::close(void)
{
	// lock to more safe
	Log_Win32_Process_Mutex lock("iFly_Log_Singleton_Mutex", true);
	lock.acquire();
	if ( the_parent::instance() )
	{
		the_parent::instance()->close();
		the_parent::close_singleton();
	}
	lock.release();
	return 0;
}

/** 
 * @brief 	Log_IO_FILE::Log_IO_FILE(void)
 *
 * 	构造函数
 * 
 * @author	jdyu
 * @date	2004-8-31
 * @return	bool	- Return TRUE in success, otherwise return FALSE.
 * @see		
 */
inline
Log_IO_FILE::Log_IO_FILE(void)
	: file_(0)
	, lock_(0)
	, cfg_ (0)
{
}

/** 
 * @brief 	inline Log_IO_FILE::~Log_IO_FILE
 *  
 *  析构函数
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @see		
 */
inline
Log_IO_FILE::~Log_IO_FILE(void)
{
	close();
}

/** 
 * @brief 	Log_IO_FILE::open
 *  
 *  初始化/逆初始化
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @return	inline int	- Return 0 in success, otherwise return error code.
 * @param	const char* fn	- [in] 
 * @see		
 */
inline
int Log_IO_FILE::open(const char * fn, Log_Mutex * lock /* = 0 */, int mode /* = 0 */, void * rev /* = 0 */)
{
	FILE* fp = 0;
	const char * om = "a+t";
	if ( mode == lgm_binary )
		om = "a+b";
	else if ( mode == lgm_text_ow )
		om = "wt";
	else if ( mode == lgm_binary_ow )
		om = "wb";

    #ifdef WIN32
	fp = _fsopen(fn, om, _SH_DENYNO);
    #else
	int md = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	int fh = ::open(fn, O_CREAT | O_RDWR | O_APPEND, md);
	if ( fh >= 0 )
		fp = fdopen(fh, om);
    #endif

	if ( fp == 0 )
	{
		#if defined(_DEBUG_LOG)
		printf("Cannot open log file %s, error code = %d.\n", fn, errno);
		#endif
		return errno;
	}

	fseek(fp, 0, SEEK_END);
	if ( ftell(fp) > 0 && mode == lgm_text )
	{
		char c;
		fseek(fp, -1, SEEK_END);
		fread(&c, 1, 1, fp);
		if ( c != '\n' )
			fputs("\n", fp);
		fseek(fp, 0, SEEK_END);
	}

	file_ = fp;
	lock_ = lock;
	#if defined(_DEBUG_LOG) && defined(WIN32)
	printf("Open log file %s successfully.\n", fn);
	#endif
	fn_ = fn;
	rev = rev;
	return 0;
}

/** 
 * @brief 	Log_IO_FILE::close
 *  
 *  关闭日志
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @return	int	- Return 0 in success, otherwise return error code.
 * @see		
 */
inline
int Log_IO_FILE::close(void)
{
	// 关闭文件
	if ( file_ != 0 )
		fclose(file_), file_ = 0;
	return 0;
}

/** 
 * @brief 	Log_IO_FILE::write
 *  
 *  写一条记录，文本方式
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @return	inline int	- Return 0 in success, otherwise return error code.
 * @param	const char* msg	- [in] 
 * @see		
 */
inline
int Log_IO_FILE::write(const char* msg)
{
	if ( file_ )
	{
		fseek(file_, 0, SEEK_END);
		return fputs(msg, file_);
	}
	return 0;
}

/** 
 * @brief 	Log_IO_FILE::write
 *  
 *  宽字符版本，写一条记录，文本方式
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @return	inline int	- Return 0 in success, otherwise return error code.
 * @param	const char* msg	- [in] 
 * @see		
 */
inline
int Log_IO_FILE::write(const wchar_t * msg)
{
	if ( file_ )
	{
		fseek(file_, 0, SEEK_END);
		return fputws(msg, file_);
	}
	return 0;
}

/** 
 * @brief 	Log_IO_FILE::write
 *  
 *  二进制方式写入
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @return	inline int	- Return 0 in success, otherwise return error code.
 * @param	const void* data	- [in] 
 * @param	int size	- [in] 
 * @see		
 */
inline int Log_IO_FILE::write(const void * data, int size)
{
	if ( file_ )
	{
		fseek(file_, 0, SEEK_END);
		return (int)fwrite(data, 1, size, file_);
	}
	return 0;
}

/** 
 * @brief 	Log_IO_FILE::empty
 *  
 *  清空文件
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @return	inline int	- Return 0 in success, otherwise return error code.
 * @see		
 */
inline int Log_IO_FILE::empty(void)
{
	if ( file_ == 0 )
		return -1;
	
	#ifdef WIN32
	int fn = fileno(file_);
	_chsize(fn, 0);
	#else
	int fn = fileno(file_);
	ftruncate(fn, 0);
	#endif
	return 0;
}

/** 
 * @brief 	Log_IO_FILE::flush
 *  
 *  从缓存刷新日志
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @return	No return value.
 * @see		
 */
inline
void Log_IO_FILE::flush(void)
{
	if ( file_ )
		fflush(file_);
}

/** 
 * @brief 	int Log_IO_FILE::size
 *  
 *  获得当前尺寸
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @return	inline size_t 
 * @see		
 */
inline
size_t Log_IO_FILE::size(void) const
{
	if ( file_ == 0 )
		return 0;

	size_t pos  = ftell(file_);
	fseek(file_, 0, SEEK_END);
	size_t size = ftell(file_);
	if ( size != pos )
		fseek(file_, (long)pos, SEEK_SET);
	return size;
}

/** 
 * @brief 	Log_IO_FILE::backup
 *  
 *  备份旧的文件
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @return	inline int	- Return 0 in success, otherwise return error code.
 * @param	const char* bname  = 0	- [in] 
 * @see		
 */
inline
int Log_IO_FILE::backup(const char* bname /* = 0 */)
{
	int ret = 0;

	// 备份/删除旧文件
	bool backup_old_file = false;
	if ( IFLY_LOG::strcasecmp(bname, fn_.c_str()) != 0 )
		backup_old_file = true;

#ifndef WIN32
	if ( file_ == 0 || backup_old_file )
	{
		close();
		ret = rename(fn_.c_str(), bname);
		ret = open(fn_.c_str(), lock_);
	}
	else
	{
		int fn = fileno(file_);
		ret = ftruncate(fn, 0);
	}
#else
	// 需要备份，拷贝文件
	if ( backup_old_file )
	{
		if ( !::CopyFileA(fn_.c_str(), bname, FALSE) )
			ret = GetLastError();
	}
	// 把原来的文件长度设为0
	if ( ret == 0 && file_ != 0 )
	{
		rewind(file_);
		int fn = _fileno(file_);
		_chsize(fn, 0);
	}
#endif

	// update member
	return ret;
}

/** 
 * @brief 	Log_IO_FILE::handle
 *  
 *  日志的句柄
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @return	inline LOG_HANDLE 
 * @see		
 */
inline
LOG_HANDLE Log_IO_FILE::handle(void)
{
	// 返回文件指针
	return (LOG_HANDLE)file_;
}

/** 
 * @brief 	Log_IO_FILE::mutex
 *  
 *  日志的互斥量
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @return	inline Log_Mutex 
 * @see		
 */
inline
Log_Mutex * Log_IO_FILE::mutex(void)
{
	// 返回互斥量指针
	return lock_;
}

/** 
 * @brief 	Log_IO_FILE::mutex
 *  
 *  设置日志的互斥量
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @see		
 */
inline
void Log_IO_FILE::mutex(Log_Mutex * mtx)
{
	lock_ = mtx;
}

/** 
 * @brief 	Log_IO_FILE::config
 *  
 *  日志的配置接口
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @return	inline Log_Mutex 
 * @see		
 */
inline
Log_Cfg * Log_IO_FILE::config(void)
{
	// 返回配置接口指针
	return cfg_;
}

/** 
 * @brief 	Log_IO_FILE::config
 *  
 *  设置日志的配置接口
 *  
 * @author	jdyu
 * @date	2005-10-13
 * @return	none
 * @see		
 */
inline
void Log_IO_FILE::config(Log_Cfg * cfg)
{
	cfg_ = cfg;
}

namespace IFLY_LOG {

// 获得字符串映射的值
template<class T = int>
struct token_map
	{
	T			 token_;
	const char * mapstr_;
	};

template <class T>
inline
int get_maped_token(T & tok, const std::string & fd, const token_map<T> * tokms)
{
	std::string fds = fd;
	strlwr(fds);

	for ( int i = 0; tokms[i].mapstr_; i++ )
	{
		std::string mps = tokms[i].mapstr_;
		strlwr(mps);
		std::vector<std::string> sts;
		split_str(mps.c_str(), sts, ",;", true, true, true);

		if ( std::find(sts.begin(), sts.end(), fds) != sts.end() )
		{
			tok = tokms[i].token_;
			return 0;
		}
	}
	return -1;
}
};

/** 
 * @brief	Log_Cfg<CFG_HEAP>::read_config
 *
 *	读取日志的配置信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	int	- Return 0 in success, otherwise return error code.
 * @param	const char* key		- [in] 日志的配置键位置
 * @see		
 */
template <class CFG_HEAP, typename BASE>
int Log_Cfg_T<CFG_HEAP, BASE>::read_config(const char* key, const char* cfg_file /* = 0 */)
{
#define LOGCFG_OPEN(file, section)	\
	Log_Cfg_Heap cfg_heap;	cfg_heap.open(section, file);
#define LOGCFG_CLOSE()				\
	cfg_heap.close();

#define LOGCFG_GETINT(name, val)	\
	do {	\
		std::string tmp;	\
		if ( cfg_heap.get_string_value(name, tmp) == 0 )	\
			if ( tmp.length() > 0 ) val = strtol(tmp.c_str(), 0, 0);  } while(0);
#define LOGCFG_GETINT_EX(name, val, set)	\
	do {	\
		set = false;	\
		std::string tmp;	\
		if ( cfg_heap.get_string_value(name, tmp) == 0 )	\
			if ( tmp.length() > 0 ) set = true, val = strtol(tmp.c_str(), 0, 0);  } while(0);
#define LOGCFG_SETINT(name, val)	\
	do {	\
		char tmp[30]; \
		sprintf(tmp, "%d", val);	\
		cfg_heap.set_string_value(name, tmp); } while(0);
#define LOGCFG_GETSTR(name, val)	\
	do {	\
		std::string stmp;	\
		cfg_heap.get_string_value (name, stmp);	\
		if ( stmp.length() )	\
			val = stmp;  } while(0);
#define LOGCFG_SETSTR(name, val)	\
	cfg_heap.set_string_value(0, name, val);
#define LOGCFG_GETBOOL(name, val)	\
	do {	\
		std::string tmp;	\
		if ( cfg_heap.get_string_value(name, tmp) == 0 )	\
			if ( tmp.length() > 0 ) {	\
				if ( IFLY_LOG::strcasecmp("true", tmp.c_str()) == 0 ) { val = true; break; }	\
				if ( IFLY_LOG::strcasecmp("false", tmp.c_str()) == 0 ) { val = false; break; }	\
				int nv = strtol(tmp.c_str(), 0, 0);	if ( nv != 0 ) val = true; \
			} while(0);

	// open cfg file
	if ( key != 0 )
		section_ = key;

	CFG_HEAP cfg_heap;
	if ( cfg_heap.open(section_.c_str(), cfg_file ? cfg_file : cfg_file_.c_str() ) == -1 )
		return -1;
	if ( cfg_file != 0 )
		cfg_file_ = cfg_file;

	if ( !IFLY_LOG::is_abs_path(cfg_file_.c_str()) )
	{
		char tmp[MAX_PATH];
		IFLY_LOG::get_bin_path(tmp, LOG_HANDLE(-1));
		cfg_file_ = IFLY_LOG::cat_path(tmp, cfg_file_.c_str());
	}

	// record file time
	cfg_mtime_ = (long)IFLY_LOG::file_mtime(cfg_file_.c_str());

	int ow = 0, fa = 0, hh = has_head_, cs = charset_, pth = perf_thres_;
	bool lv_set(false), st_set(false), cl_set(false);

	/* warning C4127: conditional expression is constant */
#if _MSC_VER >= 1400
#pragma warning(push)
#pragma warning(disable:4127)
#endif // _MSC_VER >= 1400
	LOGCFG_GETINT("output",				output_);
	LOGCFG_GETINT("subjects",			filter_);
	LOGCFG_GETINT("maxsize",			max_size_);
	LOGCFG_GETINT("overwrite",			ow);
	LOGCFG_GETINT("flush",				fa);
	LOGCFG_GETINT("maxcount",			max_count_);
	LOGCFG_GETINT("mode",				mode_);
	LOGCFG_GETINT("head",				hh);
	LOGCFG_GETSTR("locale",				locale_);
	LOGCFG_GETINT("charset",			cs);
	LOGCFG_GETINT("perfthres",			pth);

	LOGCFG_GETINT_EX("level",			level_,			lv_set);
	LOGCFG_GETINT_EX("style",			style_,			st_set);
	LOGCFG_GETINT_EX("console_level",	console_level_,	cl_set);
#if _MSC_VER >= 1400
#pragma warning(pop)
#endif // _MSC_VER >= 1400

	overwrite_		= ow != 0;
	flush_always_	= fa != 0;
	has_head_		= hh != 0;
	if ( cs >= lgc_auto && cs <= lgc_wchar )
		charset_ = (LOG_CHARSET)cs;

	std::string stmp;
	
	cfg_heap.get_string_value ("title", stmp);
	if ( stmp.length() )
		title(stmp.c_str());
	stmp = "";
	cfg_heap.get_string_value ("file",	stmp);
	if ( stmp.length() ) {
		if (IFLY_LOG::is_abs_path(stmp.c_str())) {
			file_name(stmp.c_str());
		} else {
			char buf[MAX_PATH] = {0};
			std::string logfile_path_t =  IFLY_LOG::pathname_to_path(buf, cfg_file_.c_str());
			logfile_path_t += stmp.c_str();
			file_name(logfile_path_t.c_str());
		}
	}
	

	cfg_heap.get_string_arr ("include",	includes_);
	cfg_heap.get_string_arr ("exclude",	excludes_);

	// log level strings
	const IFLY_LOG::token_map<> lvl_map[] = 
		{
			{ lgl_low,		"low,warning" },
			{ lgl_normal,	"normal,medium,info" },
			{ lgl_detail,	"detail,debug" },
			{ lgl_all,		"all,full" },
			{ lgl_none,		"none" },
			{ 0,			0 },
		};
	const IFLY_LOG::token_map<> sty_map[] =
		{
			{ lgs_short,	"short,simple" },
			{ lgs_long,		"long,normal" },
			{ lgs_all,		"all,full" },
			{ lgs_none,		"none" },
			{ 0,			0 },
		};

	if ( level_ == 0 && lv_set )
	{
		stmp = "";
		cfg_heap.get_string_value("level", stmp);
		if ( stmp.length() > 0 )
			IFLY_LOG::get_maped_token(level_, stmp, lvl_map);
	}

	if ( st_set )
	{
		stmp = "";
		cfg_heap.get_string_value("style", stmp);
		if ( stmp.length() > 0 )
			IFLY_LOG::get_maped_token(style_, stmp, sty_map);
	}

	if ( console_level_ == 0 && cl_set )
	{
		stmp = "";
		cfg_heap.get_string_value("console_level", stmp);
		if ( stmp.length() > 0 )
			IFLY_LOG::get_maped_token(console_level_, stmp, lvl_map);
	}

	if ( lv_set && !cl_set )
		console_level_ = level_;

	if ( max_size_ < static_cast<unsigned int>(LOG_MAXMSGLEN) )
		max_size_ = LOG_MAXMSGLEN;
		
	perf_thres_ = pth;

	cfg_heap.close();
	return 0;
}

/** 
 * @brief	Log_Cfg_T<CFG_HEAP>::write_config
 *
 *	写入日志的配置信息
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	int	- Return 0 in success, otherwise return error code.
 * @param	const Log_Cfg & cfg			- [in,out] 日志的配置信息
 * @param	const char* key				- [in,out] 根注册键
 * @see		
 */
template <class CFG_HEAP, typename BASE>
int Log_Cfg_T<CFG_HEAP, BASE>::write_config(const char* key, const char* cfg_file /* = 0 */)
{
	CFG_HEAP cfg_heap;
	if ( cfg_heap.open(key == 0 ? "logger" : key, cfg_file) == -1 )
		return -1;

	LOGCFG_SETINT("output",		output_);
	LOGCFG_SETINT("level",		level_);
	LOGCFG_SETINT("maxsize",	max_size_);
	LOGCFG_SETINT("overwrite",	overwrite_);
	LOGCFG_SETINT("flush",		flush_always_);
	LOGCFG_SETINT("style",		style_);
	LOGCFG_SETINT("maxcount",	max_count_);
	cfg_heap.set_string_value(0, "file", file_name());
	cfg_heap.set_string_value(0, "title", title());

	cfg_heap.close();
	return 0;
}

/** 
 * @brief	Log_Cfg_T<CFG_HEAP>::config_changed
 *
 *	判断配置信息是否改变了
 *
 * @author	jdyu
 * @date	2004年5月21日
 * @return	int	- Return 0 in success, otherwise return error code.
 * @param	const Log_Cfg& cfg	- [in,out] 日志的配置信息
 * @param	const char* key				- [in,out] 根注册键
 * @see		
 */
template <class CFG_HEAP, typename BASE>
bool Log_Cfg_T<CFG_HEAP, BASE>::config_changed(void)
{
	time_t mt = IFLY_LOG::file_mtime(cfg_file_.c_str());
	if ( cfg_mtime_ != mt )
	{
		cfg_mtime_ = mt;
		return true;
	}
	return false;
}

/** 
 * @brief	IFLY_LOG::cur_time
 *
 *	获得当前时间
 * 
 * @author	jdyu
 * @date	2004/05/21
 * @return	const char* 
 * @param	char times[]	- [out] 返回时间信息的缓冲区
 * @param	bool no_path	- [in]  是否不包含路径禁止的符号
 * @see		
 */
inline
const char* IFLY_LOG::cur_time(char times[], bool no_path /* = false */, bool sht_fmt /* = false */, time_t start_time /* = 0 */, unsigned long start_tick /* = 0 */)
{
    times[0] = '\x0';
	/* Display operating system-style date and time. */
	time_t tmc, tmr;
	tmr = time(&tmc);

	char ms[8]; ms[0] = '\0';
#ifdef WIN32
	const DWORD max_sec = (DWORD)(0xFFFFFFFFU) / 1000;
	if ( tmc >= start_time && start_tick != 0 )
	{
		DWORD cost_tick = ::GetTickCount() - start_tick;
		tmr = start_time + (tmc - start_time) / max_sec + cost_tick / 1000;
		sprintf(ms, " %03d", cost_tick % 1000);
	}
	else
		sprintf(ms, " %03d", ::GetTickCount() % 1000);
#else	// linux
	struct timeval tv;
	gettimeofday(&tv, 0);
	sprintf(ms, " %03d", (tv.tv_usec / 1000) % 1000);
#endif	// end

	if ( no_path )
	{
		strftime(times, 128, "%Y-%m-%d_%H-%M-%S", localtime(&tmr));
	}
	else
	{
		if ( sht_fmt )
			strftime(times, 128, "%H:%M:%S", localtime(&tmr));
		else
			strftime(times, 128, "%y/%m/%d-%H:%M:%S", localtime(&tmr));
		// cat msec
		strcat(times, ms);
	}

	return times;
}

// 获得进程ID
inline
int IFLY_LOG::getpid(void)
{
#if defined (WIN32)
	return ::GetCurrentProcessId ();
#elif defined (ACE_ACE_H)
	return ACE_OS::getpid();
#else
	return ::getpid ();
#endif /* WIN32 */
}

// 获得当前线程id
inline
int IFLY_LOG::thr_self(void)
{
#if defined (WIN32)
	return ::GetCurrentThreadId();
#elif defined (ACE_ACE_H)
	return ACE_OS::thr_self();
#elif defined (__ANDROID__)
	return gettid();
#else
	return ::pthread_self();
#endif /* WIN32 */
}

// 获得当前模块路径
inline
std::string IFLY_LOG::mod_path_name(LOG_HANDLE hdl /* = 0 */)
{
	char path[MAX_PATH];
	path[0] = '\0';
	
#ifdef WIN32
	if ( hdl == (LOG_HANDLE)-1 )
		hdl = mod_handle();
	::GetModuleFileNameA((HMODULE)hdl, path, MAX_PATH);
#elif defined(_ISPUTILS_H_)
	ISP_OS::get_bin_pathname(path, hdl);
#endif
	return path;
}

// 获得模块的版本号
inline
std::string IFLY_LOG::mod_version(const char* mod)
{
	std::string sDetailInfo;
	char file[MAX_PATH];
	file[0] = 0;

#ifdef WIN32
	DWORD dwData = 0, dwSize;

	UINT uSize;
	char* lpBuffer;
	struct LANGANDCODEPAGE
	{
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate;

	// Get File Name
	if ( mod == 0 )
		GetModuleFileNameA(0, file, MAX_PATH);
	else
		strcpy(file, mod);

	// GetVersionInfo
	dwSize = ::GetFileVersionInfoSizeA(file, &dwData);
	if ( dwSize ==  0 )
		return "";

	PBYTE pData = new BYTE[dwSize];
	GetFileVersionInfoA(file, 0, dwSize, pData);
	VerQueryValueA(pData, "\\VarFileInfo\\Translation", (PVOID*)&lpTranslate, &uSize);
	// Get info detail
	for( UINT i = 0; i < ( uSize / sizeof(LANGANDCODEPAGE) ); i++ )
	{
		char szSubBlock[128];
		sprintf(szSubBlock, 
			"\\StringFileInfo\\%04x%04x\\%s",
			lpTranslate[i].wLanguage, lpTranslate[i].wCodePage, "FileVersion");

		// Retrieve file description for language and code page "i". 
		VerQueryValueA(pData, szSubBlock, (void**)&lpBuffer, (UINT*)&dwSize);
		sDetailInfo = lpBuffer;
	}
	delete [] pData;
#else
	// IU_ASSERT( 0 && "Not support.");
#endif
	return sDetailInfo;
}


//# define LOG_POW(X) (((X) == 0)?1:(X-=1,X|=X>>1,X|=X>>2,X|=X>>4,X|=X>>8,X|=X>>16,(++X)))
inline int IFLY_LOG::ln2(unsigned int pw)
{
	int ln = 0;
	if ( pw != 0 && pw != 1 )
	{
		//pw = LOG_POW(pw);
		if(pw == 0)
		{
			pw = 1;
		}
		else
		{
			pw -= 1;
			pw |= pw >> 1;
			pw |= pw >> 2;
			pw |= pw >> 4;
			pw |= pw >> 8;
			pw |= pw >> 16;
			++pw;
		}
	    while ( (( pw>>++ln) & 1 ) == 0 ); 
	}
	return ln;
}
	
inline int IFLY_LOG::normalize_path(char path[MAX_PATH], const char path_split /* = '/' */)
{
	int len = 0;
	if ( 0 == path )
		return 0;

	char dest[MAX_PATH];
	bool slash_begin = false;
	bool need_copy = false;
	int  begin = 0;

	// 检查\\server的情况
	const char bsl = '\\';
	if ( path[0] == bsl && path[1] == bsl )
	{
		dest[begin++] = bsl;
		dest[begin++] = bsl;
	}

	// 把所有的和区分符不同的字符都转化相同
	for ( int i = begin; ; i++ )
	{
		char & c = path[i];
		if ( c == '/' || c == '\\' )
		{
			c = path_split;
			if ( slash_begin )
			{
				need_copy = true;
				continue;
			}
			else
				slash_begin = true;
		}
		else
			slash_begin = false;

		dest[len] = c;
		if ( c == '\0' )
			break;
		++len;
	}

	// 是否需要再拷贝一遍？
	if ( need_copy )
		strcpy(path, dest);
	return len;
}

inline const char* IFLY_LOG::cat_path(char* path, const char* more, const char path_split /* = '/' */)
{
	if ( 0 == path )
		return 0; 

	int len_path = (int)strlen(path);
	int len_more = (int)strlen(more);
	if ( len_path + len_more > MAX_PATH )
		return 0;

	char tmp[MAX_PATH]; tmp[0] = 0;
	len_path = normalize_path(path, path_split);
	strcpy(tmp, more);
	len_more = normalize_path(tmp, path_split);
	if ( len_path > 0 )
	{
		if ( tmp[0] == path_split )
			strcat(path, tmp + 1);
		else
			strcat(path, tmp);
	}
	else
		strcpy(path, tmp);

	return path;
}

inline
char* IFLY_LOG::get_bin_path(char szPath[MAX_PATH], LOG_HANDLE hmod /* = 0 */)
{
	strcpy(szPath, mod_path_name(hmod).c_str());
	return pathname_to_path(szPath, szPath);
}

inline
bool IFLY_LOG::is_abs_path(const char* path)
{
	return path[0] == '/'
		|| (path[0] == '\\' && path[1] == '\\')
		|| strchr(path, ':');
}

inline
char* IFLY_LOG::pathname_to_path(char path[MAX_PATH], const char* path_name, const char path_split /* = PATH_CHR */)
{
	if ( path != path_name )
		strcpy(path, path_name);
	normalize_path(path, path_split);

	char* p = strrchr(path, path_split);
	if ( 0 != p )
		*(p + 1) = '\0';
	else
		path[0] = 0;
	return path;
}

inline
char* IFLY_LOG::pathname_to_name(char name[MAX_PATH], const char* path_name, const char path_split /* = '/' */)
{
	char tmp[MAX_PATH];
	strcpy(tmp, path_name);
	normalize_path(tmp, path_split);

	char* p = strrchr(tmp, path_split);
	p == 0 ? p = tmp : p++;
	strcpy(name, p);
	return name;
}

inline
char* IFLY_LOG::pathname_to_noextname(char name[MAX_PATH], const char* path_name, const char path_split /* = '/' */)
{
	pathname_to_name(name, path_name, path_split);
	char* p = strrchr(name, '.');
	if ( p != 0 )
		*p = 0;
	return name;
}

inline
time_t IFLY_LOG::file_mtime(const char* file)
{
	struct stat si;
	if ( file != 0 )
	{
		int ret = stat(file, &si);
		if ( ret == 0 )
			return si.st_mtime;
	}
	return 0;
}

inline
std::string IFLY_LOG::file_mtime_str(const char * file, const char * fmt /* = 0 */)
{
	char mts[128]; mts[0] = 0;
	time_t mt = file_mtime(file);
	if ( mt != 0 )
	{
		char tmf[100] = "%Y-%m-%d %H:%M:%S";
		if ( fmt != 0 )
			strcpy(tmf, fmt);
		strftime(mts, sizeof mts, tmf, localtime(&mt));
	}
	return mts;
}

inline
time_t IFLY_LOG::file_ctime(const char* file)
{
	struct stat si;
	if ( file != 0 )
	{
		int ret = stat(file, &si);
		if ( ret == 0 )
			return si.st_ctime;
	}
	return 0;
}

inline
std::string IFLY_LOG::file_ctime_str(const char * file, const char * fmt /* = 0 */)
{
	char mts[128]; mts[0] = 0;
	time_t mt = file_ctime(file);
	if ( mt != 0 )
	{
		char tmf[100] = "%Y-%m-%d %H:%M:%S";
		if ( fmt != 0 )
			strcpy(tmf, fmt);
		strftime(mts, sizeof mts, tmf, localtime(&mt));
	}
	return mts;
}

inline 
int IFLY_LOG::split_str(const char* str, std::vector<std::string> & subs_array,
				  const char spliter[] /* = ",;" */, bool trim /* = true */,
				  bool ignore_blank /* = false */, bool supp_quote /* = false */)
{
	char subs[400];
	bool quote_start = false;
	char last_quote = 0;
	
	if ( str == 0 || *str == 0 )
		return 0;

	for ( int i = 0, cursor = 0; ; i++)
	{
		const char & c = str[i];
		
		// 检查引号
		if ( supp_quote )
		{
			if ( c == '\'' || c == '\"')
			{
				if ( quote_start == false )
				{
					quote_start = true;
					last_quote  = c;
					continue;
				}
				else if ( c == last_quote )
				{
					quote_start = false;
					last_quote  = 0;
				}
			}
		}

		// 分段检查
		if ( str[i] == 0
			|| ( !quote_start && strchr(spliter, str[i]) )
			)
		{
			subs[0] = 0;
			#if defined(WIN32) && _MSC_VER < 1300 // VC++ 6.0
			int len = min((i - cursor), (sizeof subs));
			#else
			int len = std::min<int>(i - cursor, sizeof subs);
			#endif
			if ( len > 0 )
			{
				strncpy(subs, str + cursor, len);
				subs[len] = 0;
			}
			cursor = i + 1;

			// trim it
			if ( trim || ignore_blank )
				trim_str(subs);

			if ( !ignore_blank || subs[0] != 0 )
				subs_array.push_back(subs);
		}
		// end loop
		if ( str[i] == 0 )
			break;
	}

	return 0;
}

// 忽略大小写的比较
inline
int IFLY_LOG::strcasecmp(const char* str1, const char * str2)
{
	#ifdef WIN32
	return ::stricmp(str1, str2);
	#else
	return ::strcasecmp(str1, str2);
	#endif
}

// 检测目录是否存在
inline
bool IFLY_LOG::is_dir_exist(const char* lpszDir)
{
#ifdef WIN32
	DWORD ret = ::GetFileAttributesA(lpszDir);
	if ( ret != 0xFFFFFFFF && ( ret & FILE_ATTRIBUTE_DIRECTORY ) )
		return true;
#else
	struct stat fs = { 0 };
	if ( stat(lpszDir, &fs) == 0 && ( fs.st_mode & S_IFDIR ) != 0 )
		return true;
#endif /*WIN32*/
	return false;
}

inline
bool IFLY_LOG::is_file_exist(const char* file)
{
#ifdef WIN32
	DWORD ret = ::GetFileAttributesA(file);
	if ( ret != 0xFFFFFFFF && ( ret & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
		return true;
#else
	struct stat fs = { 0 };
	if ( stat(file, &fs) == 0 && ( fs.st_mode & S_IFDIR ) == 0 )
		return true;
#endif /*WIN32*/
	return false;
}

// 使用通配符进行匹配
inline
bool IFLY_LOG::match_string(const char * pattern, const char * str)
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
	bool ret = match_string_i(ppat, str);

	if ( free_ppat )
		delete [] ppat;

	return ret;
}

// 原则上内部使用
inline
bool IFLY_LOG::match_string_i(char * pattern, const char * str)
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
					return match_string_i(ps1, ps2);
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

// 获得一个目录下的文件列表
inline
int IFLY_LOG::find_files(const char * dir, const char * filter, std::list<std::string> & flist, bool recursive /* = true */)
{
	int ret = 0;
#ifdef WIN32
	intptr_t	find_handle;
	_finddata_t	find_data;
	char		find_file[MAX_PATH];

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
				char find_path[MAX_PATH];
				sprintf(find_path, "%s/%s", dir, find_data.name);
				normalize_path(find_path);
				ret = find_files(find_path, filter, flist, recursive);
				if ( ret != 0 )
					break;
			}
			continue;
		}
		// push to list
		sprintf(find_file, "%s/%s", dir, find_data.name);
		normalize_path(find_file);
		flist.push_back(find_file);
	} while ( -1 != _findnext(find_handle, &find_data) );

	_findclose(find_handle);
	return ret;

#else  // Linux

	char find_file[MAX_PATH];
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

		if ( !match_string(filter, pde->d_name ) )
			continue;

		strcpy(find_file, dir);
		strcat(find_file, pde->d_name);
		normalize_path(find_file);

		struct stat	st;
		ret = stat(find_file, &st);
		if ( 0 != ret )
			break;

		if ( st.st_mode & S_IFDIR )
		{
			ret = find_files(find_file, filter, flist, recursive);
			if ( ret != 0 )
				break;
			continue ;
		}

		flist.push_back(find_file);
	}

	closedir(pdr);
	return ret;

#endif /* #ifdef WIN32 */
}

// 获得当前模块的句柄
#ifdef WIN32
#if _MSC_VER >= 1300    // for VC 7.0 and 8.0
# ifndef _delayimp_h
   extern "C" IMAGE_DOS_HEADER __ImageBase;
# endif
#endif // _MSC_VER >= 1300
#endif // WIN32

inline
LOG_HANDLE IFLY_LOG::mod_handle(void)
{
#ifdef WIN32
# if _MSC_VER < 1300    // earlier than .NET compiler (VC 6.0)
	MEMORY_BASIC_INFORMATION mbi;
	static int dummy;
	VirtualQuery( &dummy, &mbi, sizeof(mbi) );
	return reinterpret_cast<HMODULE>(mbi.AllocationBase);
# else    // VC 7.0 or later
	return reinterpret_cast<HMODULE>(&__ImageBase);
# endif
#else // LINUX
	return 0;
#endif // WIN32
}

// 切分目录成为目录树结构
inline
int IFLY_LOG::path_to_dir_tree(const char * path, STR_ARR & dir_tree)
{
	if ( path == 0 || *path == 0 )
		return -1;

	int begin = 0;
	int len = (int)strlen(path);
	if ( len > 2 )
	{
		if ( path[0] == '/' )   /* linux skip first / add by jjzhuang 20140430  */
			begin = 1;
		else if ( path[0] == '\\' && path[1] == '\\' )
			begin = 2;
		else if ( path[1] == ':' )
			begin = 3;
	}

	char dir_name[MAX_PATH];
	for ( int i = begin; ; i++ )
	{
		const char c = path[i];
		if ( c == '/'|| c == '\\' || c == '\0' )
		{
			strncpy(dir_name, path, i);
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

// 创建目录
inline 
int IFLY_LOG::create_directory(const char * dir, bool fail_if_exist /* = false */, bool recursive /* = true */)
{
	if ( IFLY_LOG::is_dir_exist(dir) )
		return fail_if_exist ? -1 : 0;

	STR_ARR dirs;
	int ret = path_to_dir_tree(dir, dirs);
	if ( ret != 0 )
		return ret;

	if ( !recursive &&
		dirs.size() > 1 )
		return -1;

	for ( int i = 0; i < (int)dirs.size(); i++ )
	{
		const char * pdr = dirs[i].c_str();
		if ( IFLY_LOG::is_dir_exist(pdr) )
			continue ;
	#ifdef WIN32
		BOOL res = CreateDirectoryA(pdr, NULL);
		if ( res == FALSE )
			return GetLastError();
	#else  // linux
		ret = mkdir(pdr, 0755);
		if ( ret != 0 )
			return errno;
	#endif // WIN32
	}

	return 0;
}

inline
int IFLY_LOG::trim_str(char* pstr, const char trim_char /* = ' ' */)
{
	const char* p = pstr;
	if ( 0 == p )
		return 0;
	// Get start and end position
	int start = 0, end = 0;
	while ( *p )
	{
		// 半角
		if ( ( (unsigned char)*p < 0x20 || trim_char == *p ) )
		{
			if ( end == 0 )
				start ++;
		}
		else
			end = (int) (p - pstr + 1);
		p++;
	}
	// trim it
	end > 0 ? pstr[end] = 0 : end = (int) (p - pstr);
	if ( end == start )
		pstr[0] = 0;
	else if ( start > 0 )
		memmove(pstr, pstr+start, end - start + sizeof(char));
	
	return end - start;
}

// mbstowcs and wcstombs
inline
size_t IFLY_LOG::mbstowcs(wchar_t *wcstr, const char *mbstr, size_t count)
{
	size_t cvts = ::mbstowcs(wcstr, mbstr, count);

#ifndef WIN32 // Non Win32
	if ( cvts == static_cast<unsigned int>(-1) )
	{
		size_t i = 0;
		for ( ; i < count; i++ )
		{
			if ( wcstr )
				wcstr[i] = mbstr[i];
			if ( mbstr[i] == 0 )
				break;
		}
		cvts = i - 1;
	}
#endif // LINUX
	return cvts;
}

inline
size_t IFLY_LOG::wcstombs(char *mbstr, const wchar_t *wcstr, size_t count)
{
	size_t cvts = ::wcstombs(mbstr, wcstr, count);

#ifndef WIN32 // Non Win32
	if ( cvts == static_cast<unsigned int>(-1) )
	{
		size_t i = 0;
		for ( ; i < count; i++ )
		{
			if ( mbstr )
				mbstr[i] = wcstr[i];
			if ( wcstr[i] == 0 )
				break;
		}
		cvts = i - 1;
	}
#endif // LINUX
	return cvts;
}

inline
std::wstring IFLY_LOG::char2wchar(const char * str, size_t * cvt_chrs /* = 0 */)
{
	wchar_t dest_buf[20 * 1024]; dest_buf[0] = 0;
	wchar_t * dest_ptr = dest_buf;

	size_t count = strlen(str) + 1;
	if ( count >= sizeof dest_buf / sizeof dest_buf[0] )
		dest_ptr = new wchar_t [count];
	 dest_ptr[0] = 0;

#if _MSC_VER >= 1400
	size_t siw = std::max<>(sizeof dest_buf / sizeof dest_buf[0], count);
	mbstowcs_s(cvt_chrs, dest_ptr, siw, str, count);
#else	// not vc 8
	size_t cvts = mbstowcs(dest_buf, str, count);
	if ( cvt_chrs )
		*cvt_chrs = cvts;
#endif	// not vc 8

	if ( dest_ptr == dest_buf )
		return dest_ptr;

	std::wstring out_str = dest_ptr;
	delete [] dest_ptr;
	return out_str;
}

/* 
 * convert wchar based string to char based string
 *  for local hacking purposes
 */
inline
std::string IFLY_LOG::wchar2char(const wchar_t * wstr, size_t * cvt_chrs /* = 0 */)
{
	char dest_buf[20*1024]; dest_buf[0] = 0;
	char * dest_ptr = dest_buf;

	size_t count = (wcslen(wstr) + 1) * sizeof wstr[0];
	if ( count >= sizeof dest_buf )
		dest_ptr = new char [count];
	 dest_ptr[0] = 0;

#if _MSC_VER >= 1400
	size_t sib = std::max<size_t>(count, sizeof dest_buf);
	wcstombs_s(cvt_chrs, dest_ptr, sib, wstr, count);
#else	// not vc 8
	size_t sib = count > sizeof(dest_buf) ? count : sizeof(dest_buf);
	size_t cvts = wcstombs(dest_ptr, wstr, sib);
	if ( cvt_chrs )
		*cvt_chrs = cvts;
#endif	// not vc 8

	if ( dest_ptr == dest_buf )
		return dest_ptr;

	std::string out_str = dest_ptr;
	delete [] dest_ptr;
	return out_str;
}

// 字符串转换为小写
inline
std::string & IFLY_LOG::strlwr(std::string & str)
{
	size_t len = str.size();
	for ( size_t i = 0; i < len; i++ )
	{
		char & cc = str.at(i);
		if ( cc >= 'A' && cc <= 'Z' )
			cc += 'a' - 'A';
	}
	return str;
}

// open / close
inline
int Log_Cfg_Heap_Reg::open(const char* section, const char* file /* = 0 */)
{
	int ret = 0;
	handle_ = 0;

#ifdef WIN32
	if ( file != 0 )
	{
		ini_ = true;
		if ( !IFLY_LOG::is_abs_path(file) )
		{
			char tmp[MAX_PATH];
			IFLY_LOG::get_bin_path(tmp, LOG_HANDLE(-1));
			file_ = IFLY_LOG::cat_path(tmp, file);
		}
		else
			file_ = file;
	}
	else
		ret = RegOpenKeyExA(HKEY_LOCAL_MACHINE, section, 0, KEY_READ, (PHKEY)&handle_);
#else  /* linux */
	if ( file != 0 )
		file_ = file;
#endif /* WIN32 */
	if ( section != 0 )
		root_ = section;
	return ret;
}

inline
int Log_Cfg_Heap_Reg::close(void)
{
#ifdef WIN32
	if ( !ini_ )
		RegCloseKey((HKEY)handle_);
#endif /* WIN32 */
	handle_ = 0;
	return 0;
}

// value get /set
inline
int Log_Cfg_Heap_Reg::get_string_value(const char* key, std::string & value, const char* sub /*= 0*/)
{
	int ret = 0;
#ifdef WIN32
	char buf [1024]; buf [0] = 0;
	char dest[1024]; dest[0] = 0;
	DWORD size = sizeof buf;
	DWORD type = REG_NONE;

	if ( !ini_ )
	{
		ret = RegQueryValueExA((HKEY)handle_, key, 0, &type, (LPBYTE)buf, &size);
		if ( ret == 0 )
		{
			if ( type == REG_DWORD )
				itoa(*(int*)buf, dest, 10);
			else if ( type == REG_SZ )
				strcpy(dest, buf);
		}
	}
	else
	{
		ret = GetPrivateProfileStringA(sub ? sub : root_.c_str(),
			key, value.c_str(), dest, size, file_.c_str());
		ret = 0;
	}

	// 赋值
	value = dest;

#else /* Linux */
	FILE * fp = fopen(file_.c_str(), "rt");
	if ( fp == 0 )
		return -1;

	std::string cur_sec;
	while ( !feof(fp) )
	{
		char line[1000]; line[0] = 0;
		fgets(line, sizeof line, fp);
		int len = IFLY_LOG::trim_str(line);
		if ( len <= 0 )
			continue ;

		if ( line[0] == '#' || line[0] == ';' )
			continue ;

		if ( line[0] == '[' && line[len-1] == ']' )
		{
			cur_sec = line + 1;
			cur_sec.at(len-2) = '\0';
			continue ;
		}

		IFLY_LOG::STR_ARR strs;
		IFLY_LOG::split_str(line, strs, "=", true, false, true);
		if ( strs.size() < 1 )
			continue ;

		if ( IFLY_LOG::strcasecmp(strs[0].c_str(), key) != 0 )
			continue ;

		const char * spec_sec = sub ? sub : root_.c_str();
		if ( IFLY_LOG::strcasecmp(cur_sec.c_str(), spec_sec) == 0 )
		{
			if ( strs.size() > 1 && strs[1].length() > 0 )
				value = strs[1];
			break;
		}
	}
	fclose(fp);

#endif /* WIN32 */
	return ret;
}

inline
int Log_Cfg_Heap_Reg::get_string_arr(const char* key, std::vector<std::string> & arr)
{
	std::string val;
	int ret = get_string_value(key, val);
	IFLY_LOG::split_str(val.c_str(), arr, ";,", true, true, true);
	return ret;
}

// construct
inline
Log_Cfg_Heap_Reg::Log_Cfg_Heap_Reg(void)
	: handle_		(0)
	, ini_			(false)
{
}

inline
Log_Cfg_Heap_Reg::~Log_Cfg_Heap_Reg(void)
{
	close();
}

inline
Log_Win32_Process_Mutex::Log_Win32_Process_Mutex(const char* name /* = 0 */, bool create /* = false */)
	: mutex_	(0)
{
	name_[0] = 0;
	if ( name )
		strcpy(name_, name);
	if ( create )
		open(name);
}

inline
Log_Win32_Process_Mutex::~Log_Win32_Process_Mutex(void)
{
	close();
}

inline
const char * Log_Win32_Process_Mutex::name(void)
{
	return name_;
}

inline
int Log_Win32_Process_Mutex::open(const char* name /* = 0 */)
{
#ifdef WIN32
	close();

	if ( name )
		strcpy(name_, name);
	
	mutex_ = ::CreateMutexA(0, 0, name);
	if ( mutex_ == 0 )
		return GetLastError();
	return 0;
#else
	return 0;
#endif /* WIN32 */
}

inline
int Log_Win32_Process_Mutex::close(bool /* call_delete = false */)
{
#ifdef WIN32
	if ( mutex_ != 0 )
	{
		::CloseHandle(mutex_);
		mutex_ = 0;
	}
	return 0;
#else
	return 0;
#endif /* WIN32 */
}

inline
int Log_Win32_Process_Mutex::acquire(int time_out /* = -1 */)
{
#ifdef WIN32
	int ret = ::WaitForSingleObject(mutex_, time_out);
	if ( ret == WAIT_FAILED )
		return GetLastError();
	return 0;
#else
	return 0;
#endif /* WIN32 */
}

inline
int Log_Win32_Process_Mutex::release(void)
{
#ifdef WIN32
	::ReleaseMutex(mutex_);
	return 0;
#else
	return 0;
#endif /* WIN32 */
}

/*
 * 在Linux、Solaris下实现的进程锁
 */
inline
int Log_Unix_Process_Mutex::open(const char* name /*= 0*/)
{
	return Log_Thread_Mutex::open(name);
}

inline
int Log_Unix_Process_Mutex::close(bool call_delete /*= false*/)
{
	call_delete = call_delete;
	return Log_Thread_Mutex::close();
}

inline
int Log_Unix_Process_Mutex::acquire(int time_out /*= -1*/)
{
	return Log_Thread_Mutex::acquire(time_out);
}

inline
int Log_Unix_Process_Mutex::release(void)
{
	return Log_Thread_Mutex::release();
}

inline
Log_Unix_Process_Mutex::Log_Unix_Process_Mutex(const char* name /*= 0*/, bool create /*= false*/)
	: Log_Thread_Mutex(create)
{
	name = name;
}

inline
Log_Unix_Process_Mutex::~Log_Unix_Process_Mutex(void)
{
}

inline
Log_Thread_Mutex::Log_Thread_Mutex(bool create /* = false */)
	: mutex_	(0)
{
	if ( create )
		open();
}

inline
Log_Thread_Mutex::~Log_Thread_Mutex(void)
{
	close();
}

inline
int Log_Thread_Mutex::open(const char* /* name = 0 */)
{
	close();

#ifdef WIN32
	mutex_ = new CRITICAL_SECTION;
	if ( mutex_ == 0 )
		return -1;
	::InitializeCriticalSection((CRITICAL_SECTION*)mutex_);
	return 0;
#else
	pthread_mutex_t * mtx = new pthread_mutex_t;
	int ret = pthread_mutex_init(mtx, 0);
	mutex_ = mtx;
	return ret;
#endif /* WIN32 */
}

inline
int Log_Thread_Mutex::close(bool /* call_delete = false */)
{
#ifdef WIN32
	if ( mutex_ != 0 )
	{
		DeleteCriticalSection((CRITICAL_SECTION*)mutex_);
		delete mutex_;
		mutex_ = 0;
	}
	return 0;
#else
	if ( mutex_ != 0 )
	{
		pthread_mutex_t * mtx = (pthread_mutex_t*)mutex_;
		pthread_mutex_destroy(mtx);
		delete mtx;
		mutex_ = 0;
	}
	return 0;
#endif /* WIN32 */
}

inline
int Log_Thread_Mutex::acquire(int /* time_out = -1 */)
{
#ifdef WIN32
	::EnterCriticalSection((CRITICAL_SECTION*)mutex_);
#else
	pthread_mutex_lock((pthread_mutex_t*)mutex_);
#endif /* WIN32 */
	return 0;
}

inline
int Log_Thread_Mutex::release(void)
{
#ifdef WIN32
	::LeaveCriticalSection((CRITICAL_SECTION*)mutex_);
	return 0;
#else
	pthread_mutex_unlock((pthread_mutex_t*)mutex_);
	return 0;
#endif /* WIN32 */
}

#endif /* __IFLY_LOG_C__ */
