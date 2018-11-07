/** 
 * @file    nl_mem.h
 * @brief   iFLY Natural Language Mem Pool
 * 
 * 1. 小于MEMPOOL_SIZE 的从内存池分配
 * 2. 大于MEMPOOL_SIZE 的从系统分配
 * 3. 内存池内存不足时，会按MEMPOOL_SIZE的大小扩展一个
 * 4. 默认申请的内存是和long的大小做对齐的
 * 
 * @author  jjzhuang.
 * @version 1.0
 * @date    2016/06/17
 * 
 * @see
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version    <th>Date        <th>Author    <th>Notes</tr>
 *  <tr> <td>1.0        <td>2016/06/17  <td>jjzhuang <td>Create this file</tr>
 *  <tr> <td>1.0        <td>2016/06/17  <td>jjzhuang <td>对申请大内存做对齐</tr>
 * </table>
 * 
 */


#ifndef  _NL_MEM_20160617_
#define  _NL_MEM_20160617_


/* 内存池开始的内存大小和后续每次增长的内存大小 */
#define  MEMPOOL_SIZE_4K    (4095)
#define  MEMPOOL_SIZE_16K   (16*(1<<10)-1)
#define  MEMPOOL_SIZE_20K   (20*(1<<10)-1)
#define  MEMPOOL_SIZE_32K   (32*(1<<10)-1)
#define  MEMPOOL_SIZE_64K   (64*(1<<10)-1)
#define  MEMPOOL_SIZE_80K   (80*(1<<10)-1)
#define  MEMPOOL_SIZE_100K  (100*(1<<10)-1)
#define  MEMPOOL_SIZE_128K  (128*(1<<10)-1)
#define  MEMPOOL_SIZE_256K  (256*(1<<10)-1)
#define  MEMPOOL_SIZE_512K  (512*(1<<10)-1)


#ifdef __cplusplus
extern "C" {
#endif /* C++ */

    typedef void*   MPoolHandle;

    typedef struct _SMemStatus
    {
        int nUsedSize;  /* 内存池已使用大小 */
        int nAllSize;   /* 内存池占用大小 */
    }SMemStatus,*PSMemStatus;


    /* 创建内存池 */
    MPoolHandle  MPoolCreate(int size);
    
    /* 重置内存池，但不释放已申请的内存 */
    void MPoolReset(MPoolHandle pool);

    /* 销毁内存池，同时释放所有向系统申请的内存 */
    void  MPoolDestroy(MPoolHandle pool);

    /* 申请内存 */
    void* MPoolPalloc(MPoolHandle pool,int size);

    /* 调整内存大小 */
    void* MPoolPrealloc(MPoolHandle pool,void*p,int size);

    /* 释放内存 */
    int MPoolPfree(MPoolHandle pool,void* p);

    /* 获取内存状态 */
    void MPoolStatus(MPoolHandle pool,PSMemStatus pStatus);


#ifdef __cplusplus
}
#endif /* C++ */



#endif 
