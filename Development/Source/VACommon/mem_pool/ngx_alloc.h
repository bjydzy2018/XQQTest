/** 
 * @file    ngx_alloc.h
 * @brief   MEM POOL
 * 
 *  内存池申请内存的系统函数
 *  不同的系统实现可能会不一样
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

#ifndef _NGX_ALLOC_H_INCLUDED_
#define _NGX_ALLOC_H_INCLUDED_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "ngx_config.h"

#define  USE_NLU_MEM   0 

#if USE_NLU_MEM
#include "utils/memory_manager.h"

//释放内存函数
#define ngx_free          nluFree

//调整大小函数
#define ngx_realloc       nluReAlloc

#else

//释放内存函数
#define ngx_free          free
//调整大小函数
#define ngx_realloc       realloc

#endif


//普通的分配函数
void *ngx_alloc(size_t size, ngx_log_t *log);

//清零的分配函数
void *ngx_calloc(size_t size, ngx_log_t *log);




/*
 * Linux has memalign() or posix_memalign()
 * Solaris has memalign()
 * FreeBSD 7.0 has posix_memalign(), besides, early version's malloc()
 * aligns allocations bigger than page size at the page boundary
 */

#if (NGX_HAVE_POSIX_MEMALIGN || NGX_HAVE_MEMALIGN)

void *ngx_memalign(size_t alignment, size_t size, ngx_log_t *log);

#else

#define ngx_memalign(alignment, size, log)  ngx_alloc(size, log)

#endif



#endif /* _NGX_ALLOC_H_INCLUDED_ */
