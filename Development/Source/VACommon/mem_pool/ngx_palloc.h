/** 
 * @file    ngx_palloc.h
 * @brief   MEM POOL
 * 
 * 一些内部使用到的接口，由nginx中提取出来
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


#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "ngx_config.h"


#define  NGX_ASSERT     assert

/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
//#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)
#define NGX_MAX_ALLOC_FROM_POOL    (4095)

#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)

#define NGX_POOL_ALIGNMENT       16

#define NGX_MIN_POOL_SIZE                                        \
    NGX_ALIGN_SIZE((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)), \
              NGX_POOL_ALIGNMENT)


#define  TYPE_STATIC    0
#define  TYPE_EXTERN    1

#define  MAKE_TYPE(v)      (v<<30)
#define  GET_TYPE(v)       ((v>>30)&0x0F)


typedef void (*ngx_pool_cleanup_pt)(void *data);

typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;



struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    void                 *data;
    ngx_pool_cleanup_t   *next;
};


typedef struct ngx_pool_large_s  ngx_pool_large_t;

struct ngx_pool_large_s {
    ngx_pool_large_t     *pNext;
    void                 *pAlloc;    /* 原始的地址 */
    void                 *pAlign;    /* 对齐后的地址 */
};


typedef struct {
    u_char               *pLast;  /* 当前内存块已使用的尾部 */
    u_char               *pEnd;   /* 当前内存块的尾部 */
    ngx_pool_t           *pNext;  /* 下一块内存 */
    ngx_uint_t            nFailed;/* 分配失败的次数 */
} ngx_pool_data_t;


/* u32Size 第32位表示 TYPE */
typedef struct{
    ngx_uint_t   u32Size;  /* 对齐之后的内存大小 */
    u_char*      pLast;    /* 当前小块内存的结尾,大内存时对应ngx_pool_large_t* */
}TMemInfo,*PMemInfo;


struct ngx_pool_s {
    ngx_pool_data_t       tData;
    size_t                nMax;
    ngx_pool_t           *pCurrent;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *log;
};


typedef struct {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;

/* 调用系统的分配函数 */
void *ngx_alloc(size_t size, ngx_log_t *log);
void *ngx_calloc(size_t size, ngx_log_t *log);

ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
void ngx_destroy_pool(ngx_pool_t *pool);
void ngx_reset_pool(ngx_pool_t *pool);

/* 调用内存池的分配函数 */
void *ngx_palloc(ngx_pool_t *pool, size_t size);
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);


//相关的清理，可定义自己的回调，销毁内存池时回调
ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);


//文件相关清理工作，无用
void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);

//文件清理回调
void ngx_pool_cleanup_file(void *data);
void ngx_pool_delete_file(void *data);


#endif /* _NGX_PALLOC_H_INCLUDED_ */
