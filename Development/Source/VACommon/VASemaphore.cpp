/*
 * VASemaphore.cpp
 *
 *  Created on: 2013年8月26日
 *      Author: pffang
 */

#include "VASemaphore.h"

namespace VA {

class CSemaphoreImpl : public CSemaphore {

    sem_t* m_sem;
    char* m_semname;

    int Open();
    int Close();
 public:
    CSemaphoreImpl(const char* szSemName);
    ~CSemaphoreImpl();

    int Post();
    int Wait();
    int GetValue(int * value);
};

int CSemaphoreImpl::Open() {
    m_sem = sem_open(m_semname, O_CREAT, 0644, 1);
    if (SEM_FAILED == m_sem) {
        perror("sem_open error:");
        return errno;
    }
    return 0;
}

int CSemaphoreImpl::Close() {
    int ret = 0;
    if (m_sem)
        ret = sem_close(m_sem);
    if (-1 == ret) {
        perror("sem_close error:");
        return errno;
    }
    return 0;
}

CSemaphoreImpl::CSemaphoreImpl(const char* szSemName) {
    m_semname = NULL;
    m_sem = NULL;
    int size = strlen(szSemName) + 2;
    m_semname = new char[size];
    memset(m_semname, 0, size);

    if ('/' != szSemName[0])
        m_semname[0] = '/';
    strcat(m_semname, szSemName);
    Open();
    //Post();
}

CSemaphoreImpl::~CSemaphoreImpl() {
    Close();
    if (m_semname)
        delete[] m_semname;
}

int CSemaphoreImpl::Post() {
    int ret = sem_post(m_sem);
    if (ret == -1) {
        printf("sem_post error:");
        switch (errno) {
            case EINVAL:
                printf("sem is not a valid semaphore. \n");
                Open();
                break;
            case EOVERFLOW:
                printf("The maximum allowable value for a semaphore would be exceeded. \n");
                break;
        }
        return errno;
    }
    return 0;
}

int CSemaphoreImpl::Wait() {
    int ret = sem_wait(m_sem);
    if (ret == -1) {
        printf("sem_wait error:");
        switch (errno) {
            case EINTR:
                printf("The call was interrupted by a signal handler. \n");
                Open();
                break;
            case EINVAL:
                printf("sem is not a valid semaphore. \n");
                break;
        }
        return errno;
    }
    return 0;
}

int CSemaphoreImpl::GetValue(int * value) {
    int ret = sem_getvalue(m_sem, value);
    if (ret == -1) {
        printf("sem_getvalue error:");
        switch (errno) {
            case EINVAL:
                printf("sem is not a valid semaphore. \n");
                Open();
                break;
        }
        return errno;
    }
    return 0;
}

CSemaphore* CreateSemaphore(const char* szSemName) {
    if (NULL == szSemName)
        return NULL;
    int size = strlen(szSemName) + 2;
    if (size < 5) {
        printf("sem name too short\n");
        return NULL;
    } else if (size > 250) {
        printf("sem name too short\n");
        return NULL;
    }
    return new CSemaphoreImpl(szSemName);
}
int DeleteSemaphore(const char* szSemName) {
    int ret = sem_unlink(szSemName);
    if (ret == -1) {
        perror("sem_unlink error:");
        return -1;
    }
    return 0;
}

} /* namespace VA */
