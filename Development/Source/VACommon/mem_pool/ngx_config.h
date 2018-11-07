/** 
 * @file    ngx_config.h
 * @brief   MEM POOL
 * 
 *  一些内部类型定义，和一些配置项
 *  
 *
 * @author  jjzhuang.
 * @version 1.0
 * @date    2016/08/23
 * 
 * @see
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version    <th>Date        <th>Author    <th>Notes</tr>
 *  <tr> <td>1.0        <td>2016/08/23  <td>jjzhuang <td>Create this file</tr>
 * </table>
 * 
 */

#ifndef _NGX_CONFIG_H_INCLUDED_
#define _NGX_CONFIG_H_INCLUDED_

#define  NGX_PTR_SIZE 4
#define  ngx_memzero(buf, n)       (void) memset(buf, 0, n)
#define  ngx_inline




typedef          int   ngx_int_t;
typedef unsigned int   ngx_uint_t;
typedef unsigned char  u_char;
typedef          int   ngx_err_t;
typedef int            ngx_fd_t;
typedef int            ngx_log_t;
typedef int            ngx_chain_t;

typedef struct ngx_pool_s   ngx_pool_t;
typedef unsigned long       ngxptr_t;


//申请内存的字节对齐
#ifndef NGX_ALIGNMENT
#define NGX_ALIGNMENT   sizeof(unsigned long)    /* platform word */
#endif

#define NGX_ALIGN_SIZE(d, a)     (((d) + (a - 1)) & ~(a - 1))
#define NGX_ALIGN_PTR(p, a)                                                   \
    (u_char *) ( ( (ngxptr_t) (p) +  ((ngxptr_t) a - 1) ) & (~((ngxptr_t) a - 1)) )
    //(u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


#ifndef _SIZE_T_DEFINED
# ifdef  _WIN64
typedef unsigned __int64        size_t;
# else
typedef unsigned int            size_t;
# endif
# define _SIZE_T_DEFINED
#endif /* _SIZE_T_DEFINED */


/* TODO: auto_conf: ngx_inline   inline __inline __inline__ */
#ifndef ngx_inline
#define ngx_inline      inline
#endif

#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6



//////////////////////////////////////////////////////////////////////////
//以下仅仅为了编译通过
//////////////////////////////////////////////////////////////////////////



#define NGX_LOG_STDERR            0
#define NGX_LOG_EMERG             1
#define NGX_LOG_ALERT             2
#define NGX_LOG_CRIT              3
#define NGX_LOG_ERR               4
#define NGX_LOG_WARN              5
#define NGX_LOG_NOTICE            6
#define NGX_LOG_INFO              7
#define NGX_LOG_DEBUG             8

#define NGX_LOG_DEBUG_CORE        0x010
#define NGX_LOG_DEBUG_ALLOC       0x020
#define NGX_LOG_DEBUG_MUTEX       0x040
#define NGX_LOG_DEBUG_EVENT       0x080
#define NGX_LOG_DEBUG_HTTP        0x100
#define NGX_LOG_DEBUG_MAIL        0x200
#define NGX_LOG_DEBUG_STREAM      0x400

/*
 * do not forget to update debug_levels[] in src/core/ngx_log.c
 * after the adding a new debug level
 */

#define NGX_LOG_DEBUG_FIRST       NGX_LOG_DEBUG_CORE
#define NGX_LOG_DEBUG_LAST        NGX_LOG_DEBUG_STREAM
#define NGX_LOG_DEBUG_CONNECTION  0x80000000
#define NGX_LOG_DEBUG_ALL         0x7ffffff0

//typedef intptr_t        ngx_int_t;
//typedef uintptr_t       ngx_uint_t;
//typedef intptr_t        ngx_flag_t;


#endif /* _NGX_CONFIG_H_INCLUDED_ */
