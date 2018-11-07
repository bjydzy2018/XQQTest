/*
 * CMutexMessageQueue.h
 *
 *  Created on: Aug 30, 2016
 *      Author: yxli5
 */

#ifndef _CMUTEX_MESSAGE_QUEUE_H_
#define _CMUTEX_MESSAGE_QUEUE_H_

#include "../VACommon/VAInc.h"
#include <deque>
#include "../VACommon/log/sr_log.h"

namespace VA{

template<class ValueType>
struct tagMutexMessageQueueMSG{
	int mutex_msg_type;
	ValueType   mutex_msg_value;
};

template<class T>
class CMutexMessageQueue
{
public:
	CMutexMessageQueue(U8String szTAG="MutexMessageQueue")
	{
		Init();
		m_szTAG = szTAG;
		DEBUG_TRACE("%s CMutexMessageQueue::CMutexMessageQueue Begin.", m_szTAG.GetBuffer());
#if VA_GNUC
		pthread_mutex_init(&m_hMutexMsgQueue, NULL);
#elif VA_VC
		InitializeCriticalSection(&m_hMutexMsgQueue);
#endif
		DEBUG_TRACE("%s CMutexMessageQueue::CMutexMessageQueue End.", m_szTAG.GetBuffer());
	}
	~CMutexMessageQueue()
	{
		DEBUG_TRACE("%s CMutexMessageQueue::~CMutexMessageQueue Begin.", m_szTAG.GetBuffer());
#if VA_GNUC
		pthread_mutex_destroy(&m_hMutexMsgQueue);
#elif VA_VC
		DeleteCriticalSection(&m_hMutexMsgQueue);
#endif
		UnInit();
		DEBUG_TRACE("%s CMutexMessageQueue::~CMutexMessageQueue End.", m_szTAG.GetBuffer());
	}

private:
	U8String m_szTAG;
	typename std::deque< tagMutexMessageQueueMSG<T> > m_qMsgQueue;
#if VA_VC
	CRITICAL_SECTION m_hMutexMsgQueue;
#elif VA_GNUC
	pthread_mutex_t m_hMutexMsgQueue;
#endif

	void Init()
	{
		m_szTAG.Empty();
	}

	void UnInit()
	{

	}
public:
/** \brief 清空消息队列.\n
 *  \return  None\n
 */
	void TrashMutexMsg(void)
	{
		EnterMutex(&m_hMutexMsgQueue);
		m_qMsgQueue.clear();
		LeaveMutex(&m_hMutexMsgQueue);
	}

/** \brief 加入一个消息到队列中.\n
 *  \param   mutex_msg   [In] 消息实例 \n
 *  \return  None\n
 */
	void AddMutexMsg(tagMutexMessageQueueMSG<T> mutex_msg)
	{
		DEBUG_TRACE("%s CMutexMessageQueue::AddMutexMsg Called type: %d.",
			m_szTAG.GetBuffer(), mutex_msg.mutex_msg_type);
		EnterMutex(&m_hMutexMsgQueue);
		m_qMsgQueue.push_back(mutex_msg);
		LeaveMutex(&m_hMutexMsgQueue);
	}

/** \brief 获取消息，同时将消息从队列中移除，不阻塞.\n
 *  \param   ret_msg     [Out] 获取到的消息 \n
 *  \retval  true        如果获取消息成功，返回true \n
 *  \retval  false       如果获取消息失败，返回false\n
 */
	bool GetMutexMsg(tagMutexMessageQueueMSG<T> &ret_msg)
	{
		//tagMutexMessageQueueMSG<T> mutex_msg = {-1, -1};
		bool ret = false;

		EnterMutex(&m_hMutexMsgQueue);
		if (!m_qMsgQueue.empty())
		{
			ret_msg = m_qMsgQueue.front();
			m_qMsgQueue.pop_front();
			ret = true;
			//DEBUG_TRACE("%s CMutexMessageQueue::GetMutexMsg type: %d, value: %d.",
			//	m_szTAG.GetBuffer(), ret_msg.mutex_msg_type, ret_msg.mutex_msg_value);
		}
		LeaveMutex(&m_hMutexMsgQueue);
		return ret;
	}

/** \brief 获取消息，不阻塞.\n
 *  \param   ret_msg     [Out] 获取到的消息 \n
 *  \retval  true        如果获取消息成功，返回true \n
 *  \retval  false       如果获取消息失败，返回false\n
 */
    bool GetMutexMsgButNotRemove(tagMutexMessageQueueMSG<T> &ret_msg)
	{
		//MutexMessageQueueMSG mutex_msg = {-1, -1};
		//tagMutexMessageQueueMSG<T> mutex_msg;
		bool ret = false;

		EnterMutex(&m_hMutexMsgQueue);
		if (!m_qMsgQueue.empty())
		{
			ret_msg = m_qMsgQueue.front();
			ret = true;
		}
		LeaveMutex(&m_hMutexMsgQueue);
		//DEBUG_TRACE("%s CMutexMessageQueue::GetMutexMsgButNotRemove type: %d, value: %d.",
		//	m_szTAG.GetBuffer(), mutex_msg.mutex_msg_type, mutex_msg.mutex_msg_value);
		return ret;
	}

/** \brief 检测队列中是否有指定消息类型的消息.\n
 *  \param   mutex_msg_type     [In] 指定检测的消息类型 \n
 *  \retval  true               若检测到，返回true \n
 *  \retval  false              若检测不到，返回false\n
 */
	bool HasSpecialMutexMsg(int mutex_msg_type)
	{
		EnterMutex(&m_hMutexMsgQueue);
		typename std::deque< tagMutexMessageQueueMSG< T > >::iterator ite;
		for(ite=m_qMsgQueue.begin(); ite != m_qMsgQueue.end(); ++ite)
		{
			if (ite->mutex_msg_type == mutex_msg_type)
			{
				DEBUG_TRACE("%s HasSpecialMutexMsg type: %d.", m_szTAG.GetBuffer(),
					mutex_msg_type);
				LeaveMutex(&m_hMutexMsgQueue);
				return true;
			}
		}
		LeaveMutex(&m_hMutexMsgQueue);
		return false;
	}
};

}

#endif