/*
 * VASemaphore.h
 *
 *  Created on: 2013年8月26日
 *      Author: pffang
 */

#ifndef VASEMAPHORE_H_
#define VASEMAPHORE_H_

#include "VACommon.h"

namespace VA {

class CSemaphore {
 public:
    virtual ~CSemaphore() {
    }

    virtual int Post() {
        return -1;
    }
    virtual int Wait() {
        return -1;
    }
    virtual int GetValue(int *value) {
        return -1;
    }
};

CSemaphore* CreateSemaphore(const char* szSemName);

int DeleteSemaphore(const char* szSemName);

} /* namespace VA */
#endif /* VASEMAPHORE_H_ */
