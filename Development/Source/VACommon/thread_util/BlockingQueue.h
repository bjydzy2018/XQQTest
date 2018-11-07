/*
 * BlockingQueue.h
 *
 *  Created on: 2014-8-13
 *      Author: yjren
 */

#ifndef BLOCKINGQUEUE_H_
#define BLOCKINGQUEUE_H_

#include"../VAInc.h"
#include"CMutex.h"
#include"CCondition.h"

namespace VA{

typedef void (*DESTRUCT)(void *) ;

class BlockingQueue{

protected:
	CMutex *mutex;
	CCondition *cond;
	DESTRUCT destruct;
public:
	BlockingQueue();
	virtual ~BlockingQueue();

	virtual void init(void *p_func);
	virtual void destroy();

	virtual void put(void *obj) = 0;
	//block and pop
	virtual void* take() = 0;
	//block not pop
	virtual void* get() = 0;
	//not block and not pop
	virtual void* peek() = 0;
	virtual void clear() = 0;
};

}

#endif /* BLOCKINGQUEUE_H_ */
