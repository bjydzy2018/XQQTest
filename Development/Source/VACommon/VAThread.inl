/*
 * VAThread.inl
 *
 *  Created on: 2017-01-05
 *      Author: zhfu
 */


#include "VAThread.h"

//#include <stdio.h>

#ifndef __VA_THREAD_C__
#define __VA_THREAD_C__

#define AUTO_LOCK(lock_inst)			 auto_lock<mutex_lock> _auto##lock_inst_(lock_inst);
//#define LOCK_GUARD_EX(Type,lock_inst)    auto_lock<Type> _au##lock_inst##Type(lock_inst);
//#define LOCK_GUARD(Type,lock_inst)       auto_lock<Type> _auto_(lock_inst);


struct lock_i 
{
    virtual int  lock()   = 0;
    virtual int  unlock() = 0;
    virtual int  error() = 0;
    virtual VAThread_mutex_t* get_mutex() = 0;
};


class null_lock : public lock_i
{
public:
    int lock() {
        return 0;
    }
    int unlock() {
        return 0;
    }
    int error() {
        return 0;
    }
    VAThread_mutex_t* get_mutex() {
        return 0;
    }
};


class mutex_lock : public lock_i
{
public:
    int lock() {
        VATHREAD_MUTEX_LOCK(&mutex_);
		return 0;
    }
    int unlock() {
        VATHREAD_MUTEX_UNLOCK(&mutex_);
		return 0;
    }
    mutex_lock() {
        VATHREAD_MUTEX_INIT(&mutex_);

		err_ = 0;
    }
	mutex_lock(const mutex_lock&) {
		VATHREAD_MUTEX_INIT(&mutex_);

		err_ = 0;
	}
    ~mutex_lock() {
        VATHREAD_MUTEX_DESTROY(&mutex_);
    }
    int error() {
        return err_;
    }
    VAThread_mutex_t* get_mutex() {
        return &mutex_;
    }
private:
    VAThread_mutex_t mutex_;
    int err_;
};


template<typename T>
class auto_lock
{
public:
    auto_lock(T * lk):lock_(lk) {
        if(lock_) {
            lock_->lock();
        }
    }
    auto_lock(T & lk):lock_(&lk) {
        if(lock_) {
            lock_->lock();
        }
    }
    ~auto_lock() {
        if(lock_) {
            lock_->unlock();
        }
    }
private:
    T*  lock_;
};

#endif // __VA_THREAD_C__
