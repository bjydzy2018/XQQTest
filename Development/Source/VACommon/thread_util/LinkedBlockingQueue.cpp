/*
 * LinkedBlockingQueue.cpp
 *
 *  Created on: 2014-8-13
 *      Author: yjren
 */

#include"LinkedBlockingQueue.h"
#include"..\..\ISS_API\iss_errors.h"
using namespace VA;
#if VA_DEBUG
    static VA::CTrace CLinkedTrace("CLinked", VA_DEBUG_LEVEL_DEBUG);
#endif
#define DEBUG_TRACE(...) (CLinkedTrace.TraceLevel(VA_DEBUG_LEVEL_INFOR, __VA_ARGS__))
LinkedBlockingQueue::LinkedBlockingQueue()
{
	isWaiting = false;
}

LinkedBlockingQueue::~LinkedBlockingQueue()
{
	destroy();
}

void LinkedBlockingQueue::init(void *p_func){
	destroy();
	isWaiting = false;
	BlockingQueue::init(p_func);
}
void LinkedBlockingQueue::destroy(){
	clear();
	BlockingQueue::destroy();
}
void LinkedBlockingQueue::put(void *obj)
{
	
	if(mutex == NULL ){
		return ;
	}
	
	mutex->lock();
	bq.push(obj);
	
	if(isWaiting){
		cond->signal();
	}
	mutex->unlock();
	
}
void *LinkedBlockingQueue::take()
{
	if(mutex == NULL || cond == NULL){
		return NULL;
	}
	void *data = NULL;
	
	mutex->lock();
	if(bq.size() <= 0){
		isWaiting = true;
		mutex->unlock();
		cond->wait();
		mutex->lock();
		isWaiting = false;
	}
	
	data = bq.front();
	bq.pop();
	mutex->unlock();
	return data;
}

void *LinkedBlockingQueue::get()
{
	if(mutex == NULL || cond == NULL){
		return NULL;
	}
	void *data = NULL;
	mutex->lock();
	if(bq.size() <=0 ){
		mutex->unlock();
		cond->wait();
	}else{
		data = bq.front();
		mutex->unlock();
	}
	return data;
}

void *LinkedBlockingQueue::peek()
{
	if(mutex == NULL ){
		return NULL;
	}
	void *data = NULL;
	mutex->lock();
	if(bq.size() > 0){
		data = bq.front();
	}
	mutex->unlock();
	return data;
}
void LinkedBlockingQueue::clear()
{
	if(mutex == NULL ){
		return ;
	}
	mutex->lock();
	void *tmp = NULL;
	while(bq.size() > 0){
		tmp = bq.front();
		bq.pop();
		if(destruct != NULL){
			destruct(tmp);
		}
	}
	mutex->unlock();
}


