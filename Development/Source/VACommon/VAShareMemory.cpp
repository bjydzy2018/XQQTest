/*
 * VAShareMemory.cpp
 *
 *  Created on: 2013年8月27日
 *      Author: pffang
 */

#include "VAShareMemory.h"
#include "VASemaphore.h"

namespace VA {

static const char* shmpre = "/shm.";
class CShareMemoryImpl : public CShareMemory {

    int m_shm_id;
    char* m_shmname;
    CSemaphore* m_sem;

    int Open();
    int Close();

 public:
    CShareMemoryImpl(const char* szShmName);
    ~CShareMemoryImpl();

    size_t Read(char* outbuff, size_t buffsize);
    size_t Write(const char* inbuff, size_t buffsize);
    size_t GetSize();
    const char* GetBuffer(size_t* buffsize);
    int ReleaseBuffer(const char* buff, size_t buffsize);
};

int CShareMemoryImpl::Open() {
    m_shm_id = shm_open(m_shmname, O_RDWR | O_CREAT, 0644);/*创建共享内存区*/
    if (m_shm_id == -1) {
        printf("shm_open error:");
        switch (errno) {
            case EACCES:
                printf("Permission to the shared memory object was denied.");
                break;
            case EEXIST:
                printf("Both O_CREAT and O_EXCL were specified to shm_open() and the shared memory object specified by name already exists.");
                break;
            case EINVAL:
                printf("The name argument to shm_open() was invalid.");
                break;
            case EMFILE:
                printf("The process already has the maximum number of files open.");
                break;
            case ENAMETOOLONG:
                printf("The length of name exceeds PATH_MAX.");
                break;
            case ENFILE:
                printf("The limit on the total number of files open on the system has been reached.");
                break;
            case ENOENT:
                printf("An attempt was made to shm_open() a name that did not exist, and O_CREAT was not specified.");
                break;
        }
        printf("\n");
        return errno;
    }

    return 0;
}

int CShareMemoryImpl::Close() {
    int ret = 0;
    if (m_shm_id && m_shm_id != -1)
        ret = close(m_shm_id);
    if (-1 == ret) {
        perror("sem_close error:");
        return errno;
    }
    return 0;
}

CShareMemoryImpl::CShareMemoryImpl(const char* szShmName) {
    m_shmname = NULL;
    m_shm_id = -1;
    m_sem = NULL;

    int size = strlen(szShmName) + 1;
    int presize = strlen(shmpre);
    m_shmname = new char[presize + size];
    memset(m_shmname, 0, presize + size);
    strcat(m_shmname, shmpre);
    strcat(m_shmname, szShmName);

    m_sem = CreateSemaphore(m_shmname);

    Open();
}
CShareMemoryImpl::~CShareMemoryImpl() {
    Close();
    if (m_sem)
        delete m_sem;
    if (m_shmname)
        delete[] m_shmname;
}

size_t CShareMemoryImpl::Read(char* outbuff, size_t buffsize) {
    struct stat buf;
    char* ptr;
    m_sem->Wait();
    fstat(m_shm_id, &buf);

    ptr = (char*) mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, m_shm_id, 0);/*连接共享内存区*/
    if (MAP_FAILED == ptr) {
        perror("mmap error:");
        m_sem->Post();
        return 0;
    }
    size_t nsize = buffsize >= (size_t) buf.st_size ?
                    (size_t) buf.st_size : buffsize;
    memcpy(outbuff, ptr, nsize);
    int ret = munmap(ptr, nsize);
    if (-1 == ret) {
        perror("munmap error:");
    }
    m_sem->Post();
    return nsize;
}

size_t CShareMemoryImpl::Write(const char* inbuff, size_t buffsize) {
    struct stat buf;
    char* ptr;
    int ret;
    m_sem->Wait();
    fstat(m_shm_id, &buf);
    if ((size_t) buf.st_size < buffsize) {
        ret = ftruncate(m_shm_id, buffsize);/*修改共享区大小*/
        if (-1 == ret) {
            printf("ftruncate error:");
            switch (errno) {
                case EACCES:
                    printf("Search permission is denied for a component of the path prefix, or the named file is not writable by the user.");
                    break;
                case EFAULT:
                    printf("Path points outside the process's allocated address space.");
                    break;
                case EFBIG:
                    printf("The argument length is larger than the maximum file size. (XSI)");
                    break;
                case EINTR:
                    printf("While blocked waiting to complete, the call was interrupted by a signal handler.");
                    break;
                case EINVAL:
                    printf("The argument is wrong");
                    break;
                case EIO:
                    printf("An I/O error occurred updating the inode.");
                    break;
                case EISDIR:
                    printf("The named file is a directory.");
                    break;
                case ELOOP:
                    printf("Too many symbolic links were encountered in translating the pathname.");
                    break;
                case ENAMETOOLONG:
                    printf("A component of a pathname exceeded 255 characters, or an entire pathname exceeded 1023 characters.");
                    break;
                case ENOENT:
                    printf("The named file does not exist.");
                    break;
                case ENOTDIR:
                    printf("A component of the path prefix is not a directory.");
                    break;
                case EPERM:
                    printf("The underlying file system does not support extending a file beyond its current size.");
                    break;
                case EROFS:
                    printf("The named file resides on a read-only file system.");
                    break;
                case ETXTBSY:
                    printf("The file is a pure procedure (shared text) file that is being executed.");
                    break;
                case EBADF:
                    printf("fd is not a valid descriptor or fd is not open for writing.");
                    break;
            }
            printf("\n");
            m_sem->Post();
            return 0;
        }
    }
    ptr = (char*) mmap(NULL, buffsize, PROT_WRITE, MAP_SHARED, m_shm_id, 0);/*连接共享内存区*/
    if (MAP_FAILED == ptr) {
        perror("mmap error:");
        m_sem->Post();
        return 0;
    }
    memcpy(ptr, inbuff, buffsize);
    ret = munmap(ptr, buffsize);
    if (-1 == ret) {
        perror("munmap error:");
    }
    m_sem->Post();
    return buffsize;
}
size_t CShareMemoryImpl::GetSize() {
    struct stat buf;
    fstat(m_shm_id, &buf);
    return buf.st_size;
}
const char* CShareMemoryImpl::GetBuffer(size_t* buffsize) {
    struct stat buf;
    const char* ptr;
    m_sem->Wait();
    fstat(m_shm_id, &buf);

    ptr = (const char*) mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, m_shm_id,
                             0);/*连接共享内存区*/
    m_sem->Post();
    if (MAP_FAILED == ptr) {
        perror("mmap error:");
        *buffsize = 0;
        return NULL;
    }
    *buffsize = buf.st_size;
    return ptr;
}
int CShareMemoryImpl::ReleaseBuffer(const char* buff, size_t buffsize) {
    struct stat buf;
    m_sem->Wait();
    fstat(m_shm_id, &buf);

    size_t nsize = buffsize >= (size_t) buf.st_size ?
                    (size_t) buf.st_size : buffsize;
    int ret = munmap((void*) buff, nsize);
    m_sem->Post();
    if (-1 == ret) {
        perror("munmap error:");
    }
    return 0;
}

CShareMemory* CreateShareMemory(const char* szShmName) {
    if (NULL == szShmName)
        return NULL;
    const char* p;
    if ('/' == szShmName[0])
        p = szShmName + 1;
    else
        p = szShmName;
    int size = strlen(p) + 4;
    if (size < 5) {
        printf("shm name too short\n");
        return NULL;
    } else if (size > NAME_MAX) {
        printf("shm name too long\n");
        return NULL;
    }
    return new CShareMemoryImpl(p);
}

int DeleteShareMemory(const char* szShmName) {
    int ret = shm_unlink(szShmName);
    if (ret == -1) {
        perror("shm_unlink error:");
        return -1;
    }
    return 0;
}

} /* namespace VA */
