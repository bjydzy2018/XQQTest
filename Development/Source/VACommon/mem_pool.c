// ycjiang iFlyTek
// 2014年6月14日
#include "mem_pool.h"

static ivnMemPoolErr MemPoolAddMemNode(ivhMemPool hMemPool) {
    if(hMemPool) {
        ivpMemNode *ppTailMemNode = &(hMemPool->pHeadMemNode);
        while(*ppTailMemNode) {
            ppTailMemNode = &((*ppTailMemNode)->pNextMemNode);
        }
        *ppTailMemNode = (ivpMemNode)malloc(hMemPool->nMemNodeSize);
        if(*ppTailMemNode) {
            // 初始化新添加的内存结点
            memset(*ppTailMemNode, 0, hMemPool->nMemNodeSize);
            if(++hMemPool->nMemNodeCount > hMemPool->nMemNodeCountMax)
                (*ppTailMemNode)->bCouldMallocedInPool = 0;
            else
                (*ppTailMemNode)->bCouldMallocedInPool = 1;
            // 初始化这个内存结点上的内存块
            (*ppTailMemNode)->pHeadMemBlock = (ivpMemBlock)((*ppTailMemNode)+1);
            (*ppTailMemNode)->pHeadMemBlock->pLastMemBlock = NULL;
            (*ppTailMemNode)->pHeadMemBlock->nMemBlockType = MemBlockFree;
            (*ppTailMemNode)->pHeadMemBlock->pAddrOutOfPool = NULL;
            (*ppTailMemNode)->pHeadMemBlock->nMemBlockFreedSize = hMemPool->nMemNodeSize-sizeof(ivMemNode)-sizeof(ivMemBlock);
            (*ppTailMemNode)->pHeadMemBlock->pNextMemBlock = NULL;
            (*ppTailMemNode)->pHeadMemBlock->pLastFreedMemBlock=NULL;
            (*ppTailMemNode)->pHeadMemBlock->pNextFreedMemBlock=NULL;
            (*ppTailMemNode)->pHeadFreedMemBlock = (*ppTailMemNode)->pHeadMemBlock;
            (*ppTailMemNode)->pNextMemNode = NULL;
            return MEM_POOL_SUCCESS;
        } else {
            return MEM_POOL_MEM_NOT_ENOUGH;
        }
    } else {
        return MEM_POOL_INVALID_PARAM;
    }
}
static ivnMemPoolErr MemPoolMallocedInPool(ivhMemPool hMemPool, void* pMemAddr, ivpMemNode* ppMemNode) {
    if(hMemPool && pMemAddr) {
        ivpMemNode pMemNode=hMemPool->pHeadMemNode; // 遍历内存池中的内存结点
        while(pMemNode) {
            size_t add_offset = (char*)pMemAddr-(char*)pMemNode;
            if(add_offset>0 && add_offset<hMemPool->nMemNodeSize) {
                if(ppMemNode) {
                    *ppMemNode = pMemNode;
                }
                return MEM_POOL_SUCCESS;
            }
            pMemNode = pMemNode->pNextMemNode;
        }
    } else {
		assert(0);
	}
    return MEM_POOL_FAIL;
}
static void MemPoolDelOldFreeMemBlockFromFreeList(ivpMemNode pMemNode, ivpMemBlock pOldFreeMemBlock) {
    if(pMemNode && pOldFreeMemBlock) {
        assert(pOldFreeMemBlock->nMemBlockType==MemBlockFree);
        if(pMemNode->pHeadFreedMemBlock==pOldFreeMemBlock) {
            // 删除的是头结点
            assert(pOldFreeMemBlock->pLastFreedMemBlock==NULL);
            pMemNode->pHeadFreedMemBlock=pOldFreeMemBlock->pNextFreedMemBlock;
            if(pOldFreeMemBlock->pNextFreedMemBlock) {
                pOldFreeMemBlock->pNextFreedMemBlock->pLastFreedMemBlock=NULL;
            }
        } else {
            // 删除的不是头结点
            if(pOldFreeMemBlock->pLastFreedMemBlock) {
                pOldFreeMemBlock->pLastFreedMemBlock->pNextFreedMemBlock=pOldFreeMemBlock->pNextFreedMemBlock;
            } else {
                assert(0);    // 如果不是头结点的话，前面一定还有结点
            }
            if(pOldFreeMemBlock->pNextFreedMemBlock) {
                pOldFreeMemBlock->pNextFreedMemBlock->pLastFreedMemBlock=pOldFreeMemBlock->pLastFreedMemBlock;
            }
        }
    } else {
        assert(0);
    }
}
static void MemPoolAddNewFreeMemBlockToFreeList(ivpMemNode pMemNode, ivpMemBlock pNewFreeMemBlock){
    if(pMemNode && pNewFreeMemBlock) {
        assert(pNewFreeMemBlock->nMemBlockType==MemBlockFree);
        pNewFreeMemBlock->pLastFreedMemBlock=NULL;
        pNewFreeMemBlock->pNextFreedMemBlock=pMemNode->pHeadFreedMemBlock;
        if(pMemNode->pHeadFreedMemBlock){
            pMemNode->pHeadFreedMemBlock->pLastFreedMemBlock=pNewFreeMemBlock;
        }
        pMemNode->pHeadFreedMemBlock=pNewFreeMemBlock;
        
    } else {
        assert(0);
    }
}
static void MemPoolMakeMemBlockFreed(ivhMemPool hMemPool, ivpMemNode pMemNode, ivpMemBlock pMemBlock) {
    if(hMemPool && pMemNode && pMemBlock) {
        ivpMemBlock pLastMemBlock=pMemBlock->pLastMemBlock;
        ivpMemBlock pNextMemBlock=pMemBlock->pNextMemBlock;
        if( pLastMemBlock && pLastMemBlock->nMemBlockType==MemBlockFree
            && pNextMemBlock && pNextMemBlock->nMemBlockType==MemBlockFree ) {
                // 前后都是Free的MemBlock
                // 从Free的链表中删除合并前的Free的内存块
                MemPoolDelOldFreeMemBlockFromFreeList(pMemNode, pLastMemBlock);
                MemPoolDelOldFreeMemBlockFromFreeList(pMemNode, pNextMemBlock);
                pLastMemBlock->pLastMemBlock=pLastMemBlock->pLastMemBlock;
                pLastMemBlock->nMemBlockType=MemBlockFree;
                pLastMemBlock->nMemBlockFreedSize=(char*)(pNextMemBlock+1)-(char*)pMemBlock+
                    pLastMemBlock->nMemBlockFreedSize+pNextMemBlock->nMemBlockFreedSize;
                pLastMemBlock->pAddrOutOfPool=NULL;
                pLastMemBlock->pNextMemBlock=pNextMemBlock->pNextMemBlock;
                if(pNextMemBlock->pNextMemBlock) {
                    pNextMemBlock->pNextMemBlock->pLastMemBlock=pLastMemBlock;
                }
                // 把合并后的Free的内存块插入到Free的链表头
                MemPoolAddNewFreeMemBlockToFreeList(pMemNode, pLastMemBlock);
        } else if(pLastMemBlock && pLastMemBlock->nMemBlockType==MemBlockFree) {
            // 只有前面是Free的MemBlock，此时pNextMemBlock肯定不为空，因为最后一个MemBlock的类型必为MemBlockFree
            // 从Free的链表中删除合并前的Free的内存块
            MemPoolDelOldFreeMemBlockFromFreeList(pMemNode, pLastMemBlock);
            pLastMemBlock->pLastMemBlock=pLastMemBlock->pLastMemBlock;
            pLastMemBlock->nMemBlockType=MemBlockFree;
            pLastMemBlock->nMemBlockFreedSize=(char*)pNextMemBlock-(char*)pMemBlock+pLastMemBlock->nMemBlockFreedSize;
            pLastMemBlock->pAddrOutOfPool=NULL;
            pLastMemBlock->pNextMemBlock=pNextMemBlock;
            pNextMemBlock->pLastMemBlock=pLastMemBlock;
            // 把合并后的Free的内存块插入到Free的链表头
            MemPoolAddNewFreeMemBlockToFreeList(pMemNode, pLastMemBlock);
        } else if(pNextMemBlock && pNextMemBlock->nMemBlockType==MemBlockFree) {
            // 只有后面是Free的MemBlock
            // 从Free的链表中删除合并前的Free的内存块
            MemPoolDelOldFreeMemBlockFromFreeList(pMemNode, pNextMemBlock);
            pMemBlock->pLastMemBlock=pMemBlock->pLastMemBlock;
            pMemBlock->nMemBlockType=MemBlockFree;
            pMemBlock->nMemBlockFreedSize=(char*)(pNextMemBlock)-(char*)pMemBlock+pNextMemBlock->nMemBlockFreedSize;
            pMemBlock->pAddrOutOfPool=NULL;
            pMemBlock->pNextMemBlock=pNextMemBlock->pNextMemBlock;
            if(pNextMemBlock->pNextMemBlock) {
                pNextMemBlock->pNextMemBlock->pLastMemBlock=pMemBlock;
            }
            // 把合并后的Free的内存块插入到Free的链表头
            MemPoolAddNewFreeMemBlockToFreeList(pMemNode, pMemBlock);
        } else {
            // 前后都不是Free的MemBlock
            pMemBlock->pLastMemBlock=pMemBlock->pLastMemBlock;
            pMemBlock->nMemBlockType=MemBlockFree;
            pMemBlock->nMemBlockFreedSize=(char*)(pNextMemBlock)-(char*)(pMemBlock+1);
            pMemBlock->pAddrOutOfPool=NULL;
            pMemBlock->pNextMemBlock=pMemBlock->pNextMemBlock;
            // 把Free的内存块插入到Free的链表头
            assert(pMemNode->pHeadFreedMemBlock);    // 内存结点上，最后一块内存肯定为MemBlockFree，所以肯定有Free的内存块
            MemPoolAddNewFreeMemBlockToFreeList(pMemNode, pMemBlock);
        }
    } else {
        assert(0);
    }
}

ivnMemPoolErr MemPoolCreate(ivhMemPool* phMemPool, size_t nMemNodeSize, size_t nMemNodeCount, size_t nMemNodeCountMax) {
    if(phMemPool && nMemNodeSize && nMemNodeCount) {
        ivhMemPool hMemPool = (ivhMemPool)malloc(sizeof(ivMemPool));
        if(hMemPool) {
            memset(hMemPool, 0, sizeof(ivMemPool));
            hMemPool->pHeadMemNode = NULL;
            hMemPool->nMemNodeSize = nMemNodeSize;
            hMemPool->nMemNodeCount = 0;
            hMemPool->nMemNodeCountMax = nMemNodeCountMax;
            while(nMemNodeCount--) {
                // 把新内存结点挂到内存池中
                ivnMemPoolErr err = MemPoolAddMemNode(hMemPool);
                if(err!=MEM_POOL_SUCCESS) {
                    *phMemPool = NULL;
                    MemPoolDestroy(hMemPool);
                    return err;
                }
            }
            *phMemPool = hMemPool;
            return MEM_POOL_SUCCESS;
        } else {
            *phMemPool = NULL;
            return MEM_POOL_MEM_NOT_ENOUGH;
        }
    } else {
        return MEM_POOL_INVALID_PARAM;
    }
}
void* MemPoolRealloc(ivhMemPool hMemPool, void* pMemAddr, size_t size) {
    ivnMemPoolErr err = MEM_POOL_SUCCESS;
    if(hMemPool && size) {
        if(pMemAddr==NULL) {
            size_t size_aligned = size+3-((size-1)&3); // 当B为2的幂时，A&(B-1)==A%B
            ivpMemNode pMemNode=NULL;
            ivpMemBlock pMemBlock=NULL;
            // 从Free链表上查找可用的内存
            pMemNode=hMemPool->pHeadMemNode; // 遍历内存池中的内存结点
            while (pMemNode) {
                // pMemBlock = pMemNode->pHeadMemBlock; // 遍历内存结点上的内存块
                pMemBlock = pMemNode->pHeadFreedMemBlock; // 遍历内存结点上Free的内存块
                while (pMemBlock) {
                    assert(pMemBlock->nMemBlockType==MemBlockFree);
                    if(pMemBlock->nMemBlockType==MemBlockFree
                        && sizeof(ivMemBlock)<=pMemBlock->nMemBlockFreedSize) {
                        if(size_aligned<=(hMemPool->nMemNodeSize>>1)
                            && pMemNode->bCouldMallocedInPool) {
                            // 内存申请的内存大小 小于 MEM_NODE_SIZE_DEFAULT/2 且此内存结点允许分配内存，则在此内存结点上申请内存
                            if(sizeof(ivMemBlock)+size_aligned<=pMemBlock->nMemBlockFreedSize) {
                                ivpMemBlock pOldNextMemBlock = pMemBlock->pNextMemBlock;
                                size_t newMemBlockFreedSize = pMemBlock->nMemBlockFreedSize-sizeof(ivMemBlock)-size_aligned;
                                // 从Free的链表中删除拆分前的内存块
                                MemPoolDelOldFreeMemBlockFromFreeList(pMemNode, pMemBlock);
                                pMemBlock->pLastMemBlock = pMemBlock->pLastMemBlock;
                                pMemBlock->nMemBlockType = MemBlockMallocedInPool;
                                pMemBlock->nMemBlockFreedSize = 0;
                                pMemBlock->pAddrOutOfPool = NULL;
                                pMemBlock->pNextMemBlock = (ivpMemBlock)(((char*)(pMemBlock+1))+size_aligned);
                                // 后面一定跟着一个 MemBlockFree
                                pMemBlock->pNextMemBlock->pLastMemBlock = pMemBlock;
                                pMemBlock->pNextMemBlock->nMemBlockType = MemBlockFree;
                                pMemBlock->pNextMemBlock->nMemBlockFreedSize = newMemBlockFreedSize;
                                pMemBlock->pNextMemBlock->pAddrOutOfPool=NULL;
                                pMemBlock->pNextMemBlock->pNextMemBlock=pOldNextMemBlock;
                                // 修改后一结点的pLastMemBlock
                                if(pMemBlock->pNextMemBlock->pNextMemBlock) {
                                    pMemBlock->pNextMemBlock->pNextMemBlock->pLastMemBlock=pMemBlock->pNextMemBlock;
                                }
                                // 向Free的链表中插入拆分前的内存块
                                MemPoolAddNewFreeMemBlockToFreeList(pMemNode, pMemBlock->pNextMemBlock);
                                return ((void*)(pMemBlock+1)); // 内存结点上分配出来的内存地址紧跟在内存块结构体的后面
                            }
                        } else {
                            // 如果申请的内存大小 小于 MEM_NODE_SIZE_DEFAULT/2 或此内存结点不允许分配内存，则从外部分配内存
                            if(sizeof(ivMemBlock)<=pMemBlock->nMemBlockFreedSize) {
                                void* pMemOutOutOfPool = malloc(size);
                                if(pMemOutOutOfPool) {
                                    ivpMemBlock pOldNextMemBlock = pMemBlock->pNextMemBlock;
                                    size_t newMemBlockFreedSize = pMemBlock->nMemBlockFreedSize-sizeof(ivMemBlock);
                                    // 从Free的链表中删除拆分前的内存块
                                    MemPoolDelOldFreeMemBlockFromFreeList(pMemNode, pMemBlock);
                                    pMemBlock->pLastMemBlock = pMemBlock->pLastMemBlock;
                                    pMemBlock->nMemBlockType = MemBlockMallocedOutOfPool;
                                    pMemBlock->nMemBlockFreedSize = 0;
                                    pMemBlock->pAddrOutOfPool = pMemOutOutOfPool;
                                    pMemBlock->pNextMemBlock = (ivpMemBlock)((char*)(pMemBlock+1));
                                    // 后面一定跟着一个 MemBlockFree
                                    pMemBlock->pNextMemBlock->pLastMemBlock = pMemBlock;
                                    pMemBlock->pNextMemBlock->nMemBlockType = MemBlockFree;
                                    pMemBlock->pNextMemBlock->nMemBlockFreedSize = newMemBlockFreedSize;
                                    pMemBlock->pNextMemBlock->pAddrOutOfPool=NULL;
                                    pMemBlock->pNextMemBlock->pNextMemBlock=pOldNextMemBlock;
                                    // 修改后一结点的pLastMemBlock
                                    if(pMemBlock->pNextMemBlock->pNextMemBlock) {
                                        pMemBlock->pNextMemBlock->pNextMemBlock->pLastMemBlock=pMemBlock->pNextMemBlock;
                                    }
                                    // 向Free的链表中插入拆分前的内存块
                                    MemPoolAddNewFreeMemBlockToFreeList(pMemNode, pMemBlock->pNextMemBlock);
                                }
                                return pMemOutOutOfPool;
                            }
                        }
                    }
                    pMemBlock = pMemBlock->pNextFreedMemBlock;
                }
                pMemNode = pMemNode->pNextMemNode;
            }
            err = MemPoolAddMemNode(hMemPool); // 内存结点不够用了
            if(err!=MEM_POOL_MEM_NOT_ENOUGH) {
                return MemPoolRealloc(hMemPool, pMemAddr, size);
            } else {
                return NULL;
            }
        } else {
            ivpMemNode pMemNode = hMemPool->pHeadMemNode;
            while(pMemNode) {
                ivpMemBlock pMemBlock = pMemNode->pHeadMemBlock;
                ivpMemBlock pLastMemBlock=NULL;
                while(pMemBlock) {
                    if(pMemBlock->nMemBlockType==MemBlockMallocedInPool
                        && ((void*)(pMemBlock+1))==pMemAddr) {
                            ivpMemBlock pNextMemBlock = pMemBlock->pNextMemBlock;
                            size_t nOldMemSize=(char*)pNextMemBlock-(char*)(pMemBlock+1);
                            void* pNewMemAddr=MemPoolRealloc(hMemPool, NULL, size);
                            if(pNewMemAddr) {
                                memcpy(pNewMemAddr, pMemAddr, nOldMemSize>size?size:nOldMemSize);
                            }
                            MemPoolFree(hMemPool, pMemAddr);
                            return pNewMemAddr;
                    } else if(pMemBlock->nMemBlockType==MemBlockMallocedOutOfPool
                        && pMemBlock->pAddrOutOfPool==pMemAddr){
                            void* pNewMem=realloc(pMemBlock->pAddrOutOfPool, size);
                            pMemBlock->nMemBlockType=MemBlockMallocedOutOfPool;
                            pMemBlock->nMemBlockFreedSize=0;
                            pMemBlock->pAddrOutOfPool=pNewMem;
                            return pNewMem;
                    }
                    pLastMemBlock=pMemBlock;
                    pMemBlock = pMemBlock->pNextMemBlock;
                }
                pMemNode = pMemNode->pNextMemNode;
            }
            return NULL;
        }
    } else {
        return NULL;
    }
}
void* MemPoolMalloc(ivhMemPool hMemPool, size_t size) {
    return MemPoolRealloc(hMemPool, NULL, size);
}
void MemPoolFree(ivhMemPool hMemPool, void* pMemAddr) {
    if(hMemPool && pMemAddr) {
        ivpMemNode pSomeMemNode=NULL;
        if(MemPoolMallocedInPool(hMemPool, pMemAddr, &pSomeMemNode)==MEM_POOL_SUCCESS) {
            // 如果是在池内分配的，就不需要在内存池中查找这个内存块了，直接通过MemBlock的头找到这个内存块
            ivpMemBlock pMemBlock = ((ivpMemBlock)pMemAddr)-1;
            if(pMemBlock->nMemBlockType==MemBlockMallocedInPool) {
                MemPoolMakeMemBlockFreed(hMemPool, pSomeMemNode, pMemBlock);
                return;
            } else {
                assert(0); // 不应该Free已经Free过的内存，或 Free一个无效的内存地址
                return;
            }
        } else {
            ivpMemNode pMemNode = hMemPool->pHeadMemNode; // 遍历内存池中的内存结点
            while(pMemNode) {
                ivpMemBlock pMemBlock = pMemNode->pHeadMemBlock; // 遍历内存结点上的内存块
                while(pMemBlock) {
                    if(pMemBlock->nMemBlockType==MemBlockMallocedInPool
                        && ((void*)(pMemBlock+1))==pMemAddr) {
                            assert(0); // 正常调用，不该运行到这里，应该直接通过MemBlock的头找到这个内存块
                            MemPoolMakeMemBlockFreed(hMemPool, pMemNode, pMemBlock);
                            return;
                    } else if(pMemBlock->nMemBlockType==MemBlockMallocedOutOfPool
                        && pMemBlock->pAddrOutOfPool==pMemAddr){
                            // 从外部分配的，释放掉这块内存，然后把这个MemBlock置为Free
                            free(pMemBlock->pAddrOutOfPool);
                            pMemBlock->pAddrOutOfPool=NULL;
                            MemPoolMakeMemBlockFreed(hMemPool, pMemNode, pMemBlock);
                            return;
                    }
                    pMemBlock=pMemBlock->pNextMemBlock;
                }
                pMemNode=pMemNode->pNextMemNode;
            }
        }
        assert(0); // 不应该Free已经Free过的内存，或 Free一个无效的内存地址
    } else {
        return;
    }
}
ivnMemPoolErr MemPoolDestroy(ivhMemPool hMemPool) {
    if(hMemPool) {
        ivpMemNode pMemNode=hMemPool->pHeadMemNode; // 遍历内存池中的内存结点
        while(pMemNode) {
            ivpMemNode pNextMemNode = pMemNode->pNextMemNode;
            ivpMemBlock pMemBlock = pMemNode->pHeadMemBlock; // 遍历内存结点上的内存块
            while(pMemBlock) {
                if(pMemBlock->nMemBlockType==MemBlockMallocedOutOfPool
                    && pMemBlock->pAddrOutOfPool) {
                        free(pMemBlock->pAddrOutOfPool); // 释放此内存结点上管理的从外部分配的内存
                }
                pMemBlock = pMemBlock->pNextMemBlock;
            }
            free(pMemNode); // 释放此内存结点
            pMemNode = pNextMemNode;
        }
        free(hMemPool); // 释放内存池句柄
        return MEM_POOL_SUCCESS;
    } else {
        return MEM_POOL_INVALID_PARAM;
    }
}
