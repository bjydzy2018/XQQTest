/**
 * @file    sp_lock.hpp
 * @brief   跨平台的锁实现
 * 
 *  LOCK GUARD IMPL
 *
 * @author  jjzhuang
 * @version 1.0
 * @date    2015/07/22
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version    <th>Date        <th>Author    <th>Notes</tr>
 *  <tr> <td> 1.0       <td>2015/07/22  <td>jjzhuang  <td>lock</tr>
 *  <tr> <td> 1.1       <td>2016/02/22  <td>jjzhuang  <td>add linux sp_cond</tr>
 * </table>
 */

#ifndef  _lock_hpp_20150722
#define  _lock_hpp_20150722

#include "sp_thr_define.h"



#define LOCK_GUARD_EX(Type,lock_inst)    auto_lock<Type> _au##lock_inst##Type(lock_inst);
#define LOCK_GUARD(Type,lock_inst)       auto_lock<Type> _auto_(lock_inst);


struct lock_i 
{
    /* 获取锁 */
    virtual int  lock()   = 0;
    
    /* 释放锁 */
    virtual int  unlock() = 0;
    
    /* 判断锁创建有没有出错 */
    virtual int  error() = 0;
    
    /* 获取原始锁，linux平台pthread_wait需要使用 */
    virtual sp_thread_mutex_t* get_mutex() = 0;
};


/** 
 * @class   null_lock
 * 
 * @brief   一个锁的空实现
 * 
 *  不需要实际锁的时候，可以使用空锁替代
 * 
 * @author  jjzhuang
 * @date    2015-7-22
 * 
 * @see     lock_i
 */

class null_lock : public lock_i
{
public:
    int lock()
    {
        return 0;
    }
    int unlock()
    {
        return 0;
    }
    int error(){
        return 0;
    }
    sp_thread_mutex_t* get_mutex()
    {
        return 0;
    }
};

/** 
 * @class   mutex_lock
 * 
 * @brief   Mutex Lock
 * 
 *  Windows use Mutex  and linux use pthread_mutex
 * 
 * @author  jjzhuang
 * @date    2015-7-22
 * 
 * @see     lock_i
 */
class mutex_lock : public lock_i
{
public:
    int lock()
    {
        return sp_thread_mutex_lock(&mutex_);
    }
    int unlock()
    {
        return sp_thread_mutex_unlock(&mutex_);
    }
    mutex_lock()
    {
        err_ = sp_thread_mutex_init(&mutex_,NULL);
    }
    ~mutex_lock()
    {
        sp_thread_mutex_destroy(&mutex_);
    }
    int error()
    {
        return err_;
    }
    sp_thread_mutex_t* get_mutex()
    {
        return &mutex_;
    }
private:
    sp_thread_mutex_t mutex_;
    int err_;
};


template<typename T>
class auto_lock
{
public:
    auto_lock(T * lk):lock_(lk){
        if(lock_){
            lock_->lock();
        }
    }
    auto_lock(T & lk):lock_(&lk)
    {
        if(lock_){
            lock_->lock();
        }
    }
    ~auto_lock()
    {
        if(lock_){
            lock_->unlock();
        }
    }
private:
    T*  lock_;
};

struct sp_cond_i
{
    virtual int  error() = 0;
    
    virtual int  signal() = 0;
    
    virtual int  wait(lock_i& lk) = 0;
    
    virtual int  wait(lock_i& lk,long timeout) = 0;//wait ms
    
    /* 重置到无信号状态 */
    virtual void reset() = 0;
};



#ifdef  WIN32
class sp_cond : public sp_cond_i
{
public:
    sp_cond():cond(0),err_(0)
    {
        //auto reset 
        //init = no signal
        cond = CreateEvent( NULL, FALSE, FALSE, NULL );
        if(NULL == cond){
            err_ = -1;
        }
    }
    ~sp_cond()
    {
        if(cond){
            CloseHandle(cond);
        }
    }

    virtual int error()
    {
        return err_;
    }

    virtual int signal()
    {
        int ret = SetEvent( cond );
        return 0 == ret ? GetLastError() : 0;
    }

    virtual int wait(lock_i& lk)
    {
        lk.unlock();
        int ret = WaitForSingleObject(cond, INFINITE );
        lk.lock();
        return WAIT_OBJECT_0 == ret ? 0 : GetLastError();
    }

    virtual int wait(lock_i& lk,long timeout)
    {
        lk.unlock();
        int ret = WaitForSingleObject(cond, timeout );
        lk.lock();
        return WAIT_OBJECT_0 == ret ? 0 : GetLastError();
    }
    
    virtual void reset()
    {
        ResetEvent(cond);
    }
private:
    HANDLE cond;
    int err_;
};

#elif defined(__QNX__)

class sp_cond : public sp_cond_i
{
public:
    virtual int error()
    {
        return err_;
    }
    int signal()
    {
        return pthread_cond_signal(&cond_);
    }
    int wait(lock_i& lk)
    {
        pthread_mutex_t* tmp = (pthread_mutex_t*)lk.get_mutex();
        return pthread_cond_wait(&cond_,tmp);
    }
    virtual int wait(lock_i& lk,long timeout)//ms
    {
        struct timespec outtime;
        struct timespec now_spc;
        clock_gettime(CLOCK_REALTIME,&now_spc);
        uint64_t nnow =  timespec2nsec(&now_spc);
        nsec2timespec( &outtime,nnow+timeout*1000000);
        pthread_mutex_t* tmp = (pthread_mutex_t*)lk.get_mutex();
        pthread_cond_timedwait(&cond_, tmp, &outtime);
        return 0;
    }
    void reset()
    {}

    sp_cond():err_(0)
    {
        pthread_cond_init(&cond_,NULL);
    }

    ~sp_cond()
    {
        pthread_cond_destroy(&cond_);
    }
private:
    pthread_cond_t cond_;
    int err_;
};

#else

class sp_cond : public sp_cond_i
{
public:
    virtual int error()
    {
        return err_;
    }
    int signal()
    {
        return pthread_cond_signal(&cond_);
    }
    int wait(lock_i& lk)
    {
        pthread_mutex_t* tmp = (pthread_mutex_t*)lk.get_mutex();
        return pthread_cond_wait(&cond_,tmp);
    }
    virtual int wait(lock_i& lk,long timeout)//ms
    {
		struct timespec timeToWait;
	    struct timespec now;
	    unsigned long s, us;
		unsigned long int ulquotient  = 0;  //商
		unsigned long int ulremainder  = 0;     //余数
	    clock_gettime(CLOCK_MONOTONIC, &now);
		if (now.tv_nsec > 0)
		{
			ulquotient = now.tv_nsec/1000;  //us
            ulremainder = now.tv_nsec%1000; //ns
		}
		else
		{
            ulquotient = 0;
			ulremainder = 0;
		}

		us = ulquotient + 1000UL * millisecond; //us
        s = us/1000000UL;                       //s
		us = us - s*1000000UL;                  //remainder us

	    timeToWait.tv_sec = now.tv_sec + s;
	    timeToWait.tv_nsec = us * 1000UL + ulremainder;//ns
		pthread_mutex_t* tmp = (pthread_mutex_t*)lk.get_mutex();
        pthread_cond_timedwait(&cond_, tmp, &timeToWait);
        return 0;
    }
    void reset()
    {}

    sp_cond():err_(0)
    {
		pthread_condattr_t cattr;
	    pthread_condattr_init(&(cattr));
	    pthread_condattr_setclock(&(cattr), CLOCK_MONOTONIC);
	    pthread_cond_init(&cond_,&(cattr));
    }
    
    ~sp_cond()
    {
        pthread_cond_destroy(&cond_);
    }
private:
    pthread_cond_t cond_;
    int err_;
};

#endif /*end platform*/

#endif



