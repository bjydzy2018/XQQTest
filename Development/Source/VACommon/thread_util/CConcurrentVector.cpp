/*
 * CConcurrentVector.cpp
 *
 *  Created on: 2014-8-18
 *      Author: yjren
 */

#include"CConcurrentVector.h"

using namespace VA;

CConcurrentVector::CConcurrentVector()
{
	mutex = NULL;
	destruct = NULL;
}

CConcurrentVector::~CConcurrentVector()
{
	destroy();
}

void CConcurrentVector::init(void *p_func)
{
	destroy();
	mutex = new CMutex();
	mutex->init();
	destruct = (DESTRUCT)p_func;
}

void CConcurrentVector::destroy()
{
	if(mutex != NULL){
		mutex->destroy();
		delete mutex;
		mutex = NULL;
	}
	carrier.clear();
	destruct = NULL;
}

int CConcurrentVector::push_back(void *data)
{
	int err = 0;
	if(mutex == NULL){
		return WRONG_OPREATION_ERR_CConcurrentVector;
	}

	mutex->lock();
	carrier.push_back(data);
	mutex->unlock();
	return err;
}
void *CConcurrentVector::at(unsigned int pos)
{
	void *tmp = NULL;
	if(mutex == NULL){
		return NULL;
	}
	mutex->lock();
	if(pos >= carrier.size()){
		mutex->unlock();
		return NULL;
	}
	tmp = carrier.at(pos);
	mutex->unlock();
	return tmp;
}

void CConcurrentVector::clear()
{
	if(mutex == NULL ){
		return ;
	}
	mutex->lock();
	void *tmp;
	for(int i=0;i!=carrier.size();i++){
		tmp = carrier.at(i);
		if(destruct != NULL){
			destruct(tmp);
		}
	}
	carrier.clear();
	mutex->unlock();
}

