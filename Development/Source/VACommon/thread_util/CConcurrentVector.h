/*
 * CConcurrentVector.h
 *
 *  Created on: 2014-8-18
 *      Author: yjren
 */

#ifndef CCONCURRENTVECTOR_H_
#define CCONCURRENTVECTOR_H_

#include"../VAInc.h"
#include"CMutex.h"
using std::vector;
namespace VA{
typedef void(*DESTRUCT)(void *);
const int WRONG_OPREATION_ERR_CConcurrentVector = 101;

class CConcurrentVector {
	CMutex *mutex;
	vector<void *> carrier;
	DESTRUCT destruct ;
public:
	CConcurrentVector();
	~CConcurrentVector();

	//input a destruct function pointer to destroy element in the container
	void init(void *p_func);
	void destroy();

	int push_back(void *);
	void* at(unsigned int pos);
	void clear();
};

}

#endif /* CCONCURRENTVECTOR_H_ */
