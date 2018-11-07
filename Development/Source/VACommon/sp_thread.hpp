/**
 * @file    sp_thread.hpp
 * @brief   跨平台的线程实现
 * 
 * 使用时集成sp_thread.hpp即可
 * 
 * @author  jjzhuang
 * @version 1.0
 * @date    2015/12/08
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version    <th>Date        <th>Author    <th>Notes</tr>
 *  <tr> <td> 1.0       <td>2015/07/22  <td>jjzhuang  <td>create</tr>
 *  <tr> <td> 1.1       <td>2016/02/22  <td>jjzhuang  <td>add wait api</tr>
 * </table>
 */

#ifndef  _spthread_hpp_20151208
#define  _spthread_hpp_20151208

#include "sp_thr_define.h"
#include "sp_lock.hpp"
#include "stdio.h"


//#define  thread_log     printf
#define  thread_log

/** 
 * @class   thr_i
 * 
 * @brief   thr_i
 * 
 *  跨平台的线程实现
 * 
 * @author  jjzhuang
 * @date    2015-7-22
 * 
 * @see     mutex_lock sp_cond
 */
template<typename LOCK = mutex_lock,typename COND = sp_cond>
class thr_i
{
public:
    thr_i():run_(false){};

    /** 
     * @brief   start
     *  
     *  start a new thread
     *  
     * @author  jjzhuang
     * @date    2016/04/08
     * @return  0 success else error code
     * @param
     **/
    int start()
    {
        LOCK_GUARD(LOCK,th_lock_);
        if(true == run_){
            return 0;
        }
        int err = sp_thread_create(&handle_,0,wrapper_svc,this);
        if(0 == err){
            run_ = true;
        }
        return err;
    }

    /** 
     * @brief   stop
     *  
     *  stop the thread,not impl now
     *  
     * @author  jjzhuang
     * @date    2016/04/08
     * @return  0 success else error code
     * @param
     **/
    int stop()
    {
        return 0;
    }


    /** 
     * @brief   wait
     *  
     *  Deprecated use join instead
     *  
     * @author  jjzhuang
     * @date    2016/04/08
     * @return  0 success else error code
     * @param
     **/
    int wait()
    {
        return join();
    }

    
     /** 
     * @brief   join
     *  
     *  等待线程结束
     *  使用单例模式，会出现线程被强制结束，wait一直block
     *  将一直wait 改为wait 10 秒
     *  
     * @author  jjzhuang
     * @date    2016/04/08
     * @return  0 success else error code
     * @param
     **/
    int join()
    {
        thread_log("thread [0x%x] wait | entr!\n",sp_thread_self());
        LOCK_GUARD(LOCK,th_lock_);
        while(run_){
            thread_log("thread [0x%x] wait!\n",sp_thread_self());
            //th_cond_.wait(th_lock_);
            th_cond_.wait(th_lock_,10000);
        }
        thread_log("thread [0x%x] wait | leave!\n",sp_thread_self());
        return 0;
    }

protected:
    static sp_thread_result_t SP_THREAD_CALL wrapper_svc(void* arg)
    {
        thr_i* pThis = (thr_i*)arg;
        thread_log("thread [0x%x] start!\n",sp_thread_self());
        int ret = pThis->svc();
        LOCK_GUARD(LOCK,pThis->th_lock_);
        thread_log("thread [0x%x] over!\n",sp_thread_self());
        pThis->run_ = false;
        pThis->th_cond_.signal();
        return (sp_thread_result_t)ret;
    }

    /** 
     * @brief   svc
     *  
     *  you need to overwrite this function
     *  
     * @author  jjzhuang
     * @date    2016/04/08
     * @return  0 success else error code
     * @param
     **/
    virtual int svc()
    {
        while(1){
            sp_sleep(1);
            printf("thread [0x%x] sleep 1s\n",sp_thread_self());
        }
        return 0;
    }

private:
    sp_thread_t  handle_;
    bool         run_;
    LOCK         th_lock_;
    COND         th_cond_;
};


#endif



