/*
 * LinkedBlockingQueue.h
 *
 *  Created on: 2014-8-13
 *      Author: yjren
 */

#ifndef LINKEDBLOCKINGQUEUE_H_
#define LINKEDBLOCKINGQUEUE_H_

#include"BlockingQueue.h"
#include"queue"
using std::queue;
namespace VA{

class LinkedBlockingQueue:public BlockingQueue{

	queue<void *> bq;
	bool isWaiting;
public:
	LinkedBlockingQueue();
	~LinkedBlockingQueue();

	void init(void *p_func);
	void destroy();

	void put(void *);
	void *take();
	void *get();
	void *peek();
	void clear();
};

}
#endif /* LINKEDBLOCKINGQUEUE_H_ */
