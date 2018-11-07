// ycjiang iFlyTek
// 2014年6月14日
#ifndef _MEM_POOL_H_
#define _MEM_POOL_H_

#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MEM_NODE_SIZE_DEFAULT          (2 * 1024 * 1024)    // 内存结点大小，大小大于 MEM_NODE_SIZE_DEFAULT/2 的内存申请，直接向系统申请，而不是在池中申请
#define MEM_NODE_COUNT_DEFAULT         (10)                  // 理论上 初始化后占用内存值 MEM_NODE_COUNT_DEFAULT*MEM_NODE_SIZE_DEFAULT
#define MEM_NODE_COUNT_MAX_DEFAULT     (15)                 // 理论上 内存池的内存峰值 (MEM_NODE_COUNT_MAX_DEFAULT+1)*MEM_NODE_SIZE_DEFAULT

typedef enum tagMemPoolErrorCode {
    MEM_POOL_FAIL             = -1,
    MEM_POOL_SUCCESS          = 0,
    MEM_POOL_MEM_NOT_ENOUGH   = 1,
    MEM_POOL_INVALID_PARAM    = 2,
}ivnMemPoolErr;

typedef enum tagMemBlockType {
    MemBlockFree               =  0,    // 还没有被分配
    MemBlockMallocedInPool     =  1,    // 内存池内部分配
    MemBlockMallocedOutOfPool  =  2,    // 内存池的结点的内存大小有限，这块内存可能是从外部分配的
}ivMemBlockType;

typedef struct tagMemBlock {
    struct tagMemBlock* pLastMemBlock;      // 双向链表管理内存结点上的内存块，用于快速释放内存
    ivMemBlockType nMemBlockType;           // 描述此内存块的类型
    size_t nMemBlockFreedSize;              // nMemBlockType==MemBlockFree。内存结点上最后一个内存块的类型为MemBlockFree，它依赖nMemBlockFreedSize 表明内存块大小。
    void* pAddrOutOfPool;                   // nMemBlockType==MemBlockMallocedOutOfPool
    struct tagMemBlock* pLastFreedMemBlock; // 用双向链表维护Free的内存块，用于快速分配内存
    struct tagMemBlock* pNextFreedMemBlock; // 用双向链表维护Free的内存块，用于快速分配内存
    struct tagMemBlock* pNextMemBlock;      // 双向链表管理内存结点上的内存块，用于快速释放内存
}ivMemBlock, *ivpMemBlock;

typedef struct tagMemNode {
    int bCouldMallocedInPool;           // 是否允许在此内存结点上分配内存，否则向系统要内存。因为没有释放内存结点的机制，可以避免内存池结点过多
    ivpMemBlock pHeadMemBlock;          // 内存块在这个内存结点上分配
    ivpMemBlock pHeadFreedMemBlock;     // Free的内存块
    struct tagMemNode* pNextMemNode;    // 单向链表管理内存池里的内存结点
}ivMemNode, *ivpMemNode;

typedef struct tagMemPool {
    ivpMemNode pHeadMemNode;            // 内存池通过内存结点管理内存
    size_t nMemNodeSize;                // 一个内存结点大小
    size_t nMemNodeCount;               // 当前内存池中内存结点的个数
    size_t nMemNodeCountMax;            // 内存池中允许直接在结点上分配内存的最大结点数
}ivMemPool, *ivhMemPool;

ivnMemPoolErr MemPoolCreate(
    ivhMemPool* phMemPool,
    size_t nMemNodeSize,                // 内存池结点的大小
    size_t nMemNodeCount,               // 初始时内存池结点数
    size_t nMemNodeCountMax             // 内存池结点最大值 超过了就只允许向系统要内存
    );
void* MemPoolRealloc( ivhMemPool hMemPool, void* pMemAddr, size_t size );
void* MemPoolMalloc( ivhMemPool hMemPool, size_t size );
void MemPoolFree( ivhMemPool hMemPool, void* pMemAddr );
ivnMemPoolErr MemPoolDestroy( ivhMemPool hMemPool );

#ifdef __cplusplus
}
#endif

#endif
