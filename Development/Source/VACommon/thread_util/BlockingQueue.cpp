/*
 * BlockingQueue.cpp
 *
 *  Created on: 2014-8-13
 *      Author: yjren
 */
#include"BlockingQueue.h"
#include"../../ISS_API/iss_errors.h"
using namespace VA;
BlockingQueue::BlockingQueue()
{
	mutex = NULL;
	cond = NULL;
	destruct = NULL;
}

BlockingQueue::~BlockingQueue()
{
	destroy();
}

void BlockingQueue::init(void *p_func)
{
	destroy();
	mutex = new CMutex();
	mutex->init();
	cond = new CCondition();
	cond->init();
	destruct = (DESTRUCT)p_func;
}

void BlockingQueue::destroy()
{
	if(mutex != NULL){
		delete mutex;
		mutex = NULL;
	}
	if(cond != NULL){
		delete cond;
		cond = NULL;
	}
	destruct = NULL;
}

