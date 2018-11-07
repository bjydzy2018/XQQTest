

#include "ngx_config.h"
#include "ngx_palloc.h"
#include "ngx_alloc.h"
#include "nl_mem.h"

int ngx_errno = 0;

#define  NGX_DEBUG_PALLOC 0


static ngx_inline void *ngx_palloc_small(ngx_pool_t *pool, size_t size,
    ngx_uint_t align);
static void *ngx_palloc_block(ngx_pool_t *pool, size_t size);
static void *ngx_palloc_large(ngx_pool_t *pool, size_t size);


ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    if (p == NULL) 
    {
        NGX_ASSERT(1);
        return NULL;
    }

    p->tData.pLast = (u_char *) p + sizeof(ngx_pool_t);
    /*分配的首地址做对齐*/
    p->tData.pLast = NGX_ALIGN_PTR(p->tData.pLast,NGX_ALIGNMENT);
    p->tData.pEnd = (u_char *) p + size;
    p->tData.pNext = NULL;
    p->tData.nFailed = 0;

    //size = size - sizeof(ngx_pool_t);
    //size = p->d.end - p->d.last;
    //p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    /*一次可以从内存池申请的最大值由创建时的大小决定，而不采用默认的了*/
    p->nMax = p->tData.pEnd - p->tData.pLast;

    p->pCurrent = p;
    p->chain = NULL;
    p->large = NULL;
    p->cleanup = NULL;
    p->log = log;

    return p;
}


void
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    for (c = pool->cleanup; c; c = c->next) 
    {
        if (c->handler) 
        {
            c->handler(c->data);
        }
    }

    for (l = pool->large; l; l = l->pNext) 
    {
        if (l->pAlloc) 
        {
            ngx_free(l->pAlloc);
            l->pAlloc = NULL;
        }
    }

    for (p = pool, n = pool->tData.pNext; /* void */; p = n, n = n->tData.pNext) 
    {
        ngx_free(p);
        if (n == NULL) 
        {
            break;
        }
    }
}


/*reset 时会释放大内存*/
void
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    for (l = pool->large; l; l = l->pNext) 
    {
        if (l->pAlloc) 
        {
            ngx_free(l->pAlloc);
            l->pAlloc = NULL;
            l->pAlign = NULL;
        }
    }

    for (p = pool; p; p = p->tData.pNext) 
    {
        p->tData.pLast = (u_char *) p + sizeof(ngx_pool_t);
        /*分配的首地址做对齐*/
        p->tData.pLast = NGX_ALIGN_PTR(p->tData.pLast,NGX_ALIGNMENT);
        p->tData.nFailed = 0;
    }

    pool->pCurrent = pool;
    pool->chain = NULL;
    pool->large = NULL;
}


void *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
#if !(NGX_DEBUG_PALLOC)
    if ((size + sizeof(TMemInfo) + NGX_ALIGNMENT ) 
        <= pool->nMax) {
        return ngx_palloc_small(pool, size, 1);
    }
#endif

    return ngx_palloc_large(pool, size);
}


void *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
#if !(NGX_DEBUG_PALLOC)
    if ( (size + sizeof(TMemInfo) + NGX_ALIGNMENT ) 
        <= pool->nMax ) {
        return ngx_palloc_small(pool, size, 0);
    }
#endif
    return ngx_palloc_large(pool, size);
}


static ngx_inline void *
ngx_palloc_small(ngx_pool_t *pool, size_t size, ngx_uint_t align)
{
    u_char      *m;
    TMemInfo* header;
    ngx_pool_t  *p;

    /*大小对齐后可以保证申请后的指针还是对齐的*/
    size = NGX_ALIGN_SIZE(size,NGX_ALIGNMENT);
    p = pool->pCurrent;
    do {
        m = p->tData.pLast;

    /*SIZE对齐后，不需要对地址再做对齐了*/
#if 0
        if (align) {
            header = (TMemInfo*)NGX_ALIGN_PTR(m, NGX_ALIGNMENT);
            m = header +1;
        }
        else{
            header = (TMemInfo*)m;
            m = header +1;
        }
#else
        header = (TMemInfo*)m;
        m = (u_char*)(header +1);
#endif

        if ((m + size) <= p->tData.pEnd) 
        {
            p->tData.pLast = m + size;
            header->u32Size = (ngx_uint_t)size;
            header->pLast = p->tData.pLast;
            return m;
        }
        p = p->tData.pNext;
    } while (p);

    return ngx_palloc_block(pool, size);
}


static void *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       nPoolSize;
    ngx_pool_t  *p, *pNewBlock;
    TMemInfo* header = NULL;

    nPoolSize = (size_t) (pool->tData.pEnd - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, nPoolSize, pool->log);
    if (m == NULL) 
    {
        NGX_ASSERT(1);
        return NULL;
    }

    pNewBlock = (ngx_pool_t *) m;

    pNewBlock->tData.pEnd = m + nPoolSize;
    pNewBlock->tData.pNext = NULL;
    pNewBlock->tData.nFailed = 0;

    m += sizeof(ngx_pool_data_t);
    header = (TMemInfo*)NGX_ALIGN_PTR(m, NGX_ALIGNMENT);
    m = (u_char*)(header +1);
    pNewBlock->tData.pLast = m + size;
    header->u32Size = (ngx_uint_t)size;
    header->pLast = pNewBlock->tData.pLast;
    NGX_ASSERT(pNewBlock->tData.pLast <= pNewBlock->tData.pEnd);
    for (p = pool->pCurrent; p->tData.pNext; p = p->tData.pNext) 
    {
        if (p->tData.nFailed++ > 4) 
        {
            pool->pCurrent = p->tData.pNext;
        }
    }
    p->tData.pNext = pNewBlock;

    return m;
}


static void *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    ngx_uint_t         n;
    ngx_pool_large_t  *large;
    PMemInfo pHeader;
    void *pAlloc,*pAlign;

    size = size + sizeof(TMemInfo) + NGX_ALIGNMENT;
    pAlloc = ngx_alloc(size, pool->log);
    if (pAlloc == NULL) 
    {
        NGX_ASSERT(1);
        return NULL;
    }
    /*对齐大内存*/
    pAlign = NGX_ALIGN_PTR(pAlloc,NGX_ALIGNMENT);
    pHeader = (PMemInfo)pAlign;
    pHeader->u32Size = (ngx_uint_t)(MAKE_TYPE(TYPE_EXTERN)| size);
    pAlign = pHeader+1;

    n = 0;
    for (large = pool->large; large; large = large->pNext) 
    {
        if (large->pAlloc == NULL) 
        {
            large->pAlloc = pAlloc;
            large->pAlign =pAlign;
            pHeader->pLast = (u_char*)large;
            return large->pAlign;
        }
        if (n++ > 3) 
        {
            break;
        }
    }

    large = ngx_palloc_small(pool, sizeof(ngx_pool_large_t), 1);
    if (large == NULL) 
    {
        NGX_ASSERT(1);
        ngx_free(pAlloc);
        return NULL;
    }

    large->pAlloc = pAlloc;
    large->pAlign = pAlign;

    large->pNext = pool->large;
    pool->large = large;
    pHeader->pLast = (u_char*)large;
    return pAlign;
}

/*自己做对齐，不用系统的*/
#if 0
void * ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    void              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    if (p == NULL) {
        NGX_ASSERT(1);
        return NULL;
    }

    large = ngx_palloc_small(pool, sizeof(ngx_pool_large_t), 1);
    if (large == NULL) {
        NGX_ASSERT(1);
        ngx_free(p);
        return NULL;
    }

    large->pAlloc = p;
    large->pAlign = p;
    large->pNext = pool->large;
    pool->large = large;

    return p;
}
#endif


ngx_int_t
ngx_pfree(ngx_pool_t *pool, void *ptr)
{
    ngx_pool_large_t  *l;
    PMemInfo pHeader = ((PMemInfo)ptr)-1;
    if(TYPE_EXTERN != GET_TYPE(pHeader->u32Size))
    {
        return NGX_OK;
    }
    l = (ngx_pool_large_t*)pHeader->pLast;
    ngx_free(l->pAlloc);
    l->pAlloc = NULL;
    l->pAlign = NULL;

    // 小内存释放，只有在块尾部才释放
    // 查找释放效率特别慢，暂不释放
    /*
    for (p = pool; p; p = p->d.next) {
        //找到内存块
        if(ptr > p && ptr < p->d.end){
            header = ((ngx_small_header_t *)ptr)-1;
            if(header->last == p->d.last){
                p->d.last = header;
                return NGX_OK;
            }
        }
    }*/
    return NGX_OK;
}


void *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    void *p;

    p = ngx_palloc(pool, size);
    if (p) {
        ngx_memzero(p, size);
    }
    NGX_ASSERT(NULL!=p);
    return p;
}


ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, sizeof(ngx_pool_cleanup_t));
    if (c == NULL) {
        return NULL;
    }

    if (size) {
        c->data = ngx_palloc(p, size);
        if (c->data == NULL) {
            return NULL;
        }

    } else {
        c->data = NULL;
    }

    c->handler = NULL;
    c->next = p->cleanup;

    p->cleanup = c;

    return c;
}





void* ngx_pool_realloc(ngx_pool_t *pool,void* ptr,size_t size)
{
    ngx_pool_large_t  *l;
    void *pAlloc,*pAlign;
    PMemInfo pHeader = ((PMemInfo)ptr)-1;

    /*大内存重新分配*/
    if(TYPE_EXTERN == GET_TYPE(pHeader->u32Size))
    {
        l = (ngx_pool_large_t*)pHeader->pLast;
        size = size+sizeof(TMemInfo)+NGX_ALIGNMENT;
        pAlloc = ngx_realloc(l->pAlloc,size);
        NGX_ASSERT(NULL != pAlloc);
        if(NULL == pAlloc){
            return NULL;
        }
        l->pAlloc = pAlloc;
        pAlign = NGX_ALIGN_PTR(pAlloc,NGX_ALIGNMENT);
        pHeader = (PMemInfo)pAlign;
        pHeader->u32Size = (ngx_uint_t)(MAKE_TYPE(TYPE_EXTERN)| size);
        l->pAlign = pHeader+1;
        return l->pAlign;
    }

    //小内存重新分配,查找耗时，暂不使用
#if 0
    for (p = pool; p; p = p->tData.pNext) {
        //找到内存块
        if(ptr > p && ptr < p->tData.pEnd){
            //size_t* p_size = (u_char*)ptr - NGX_ALIGNMENT;
            if(pHeader->pLast == p->tData.pLast)
            {
                u_char* tmp = ptr;
                // 当前Block还有空余，可以继续向后扩展
                //size += NGX_ALIGNMENT;
                size = ngx_align(size,NGX_ALIGNMENT);
                if(tmp+size <= p->tData.pEnd){
                    p->tData.pLast = tmp+size;
                    pHeader->u32Size = size;
                    pHeader->pLast = p->tData.pLast;
                    return tmp;
                }
            }
            else{
                break;
            }
        }
    }
#endif
    /*重新申请->copy*/
    {
        void* tmp = ngx_palloc(pool,size);
        NGX_ASSERT(pHeader->u32Size<=size);
        memcpy(tmp,ptr,pHeader->u32Size);
        return tmp;
    }
    NGX_ASSERT(1);
    return NULL;
}



//////////////////////////////////////////////////////////////////////////
//内存池的封装接口
//////////////////////////////////////////////////////////////////////////

//创建内存池
MPoolHandle  MPoolCreate(int size)
{
    return (MPoolHandle)ngx_create_pool(size,NULL);
}

void MPoolReset(MPoolHandle pool)
{
    ngx_reset_pool((ngx_pool_t*)pool);
}

//销毁内存池
void MPoolDestroy(MPoolHandle pool)
{
    ngx_destroy_pool((ngx_pool_t*)pool);
}

//申请内存
void* MPoolPalloc(MPoolHandle pool,int size)
{
    return ngx_palloc((ngx_pool_t*)pool,size);
}

//释放内存
int MPoolPfree(MPoolHandle pool,void* p)
{
    if(NULL == p){
        return NGX_OK;
    }
    return ngx_pfree((ngx_pool_t*)pool,p);
}

void* MPoolPrealloc(MPoolHandle pool,void*p,int size)
{
    if(NULL == p){
        return ngx_palloc(pool,size);
    }
    else{
        return ngx_pool_realloc(pool,p,size);
    }
}

void MPoolStatus(MPoolHandle hPool,PSMemStatus pStatus)
{
    ngx_pool_t  *p;
    int nBlock = 0;
    
    p = (ngx_pool_t*)hPool;
    
    nBlock = (int)(p->tData.pEnd - (u_char*)p);
    
    pStatus->nAllSize = 0;
    pStatus->nUsedSize = 0;

    do {
        pStatus->nAllSize += nBlock;
        pStatus->nUsedSize += (int)(p->tData.pLast - (u_char*)p);
        p = p->tData.pNext;
    } while (p);
}


