/*
 * VAShareMemory.h
 *
 *  Created on: 2013年8月27日
 *      Author: pffang
 */

#ifndef VASHAREMEMORY_H_
#define VASHAREMEMORY_H_

#include "VACommon.h"

#include <sys/mman.h>

namespace VA {

class CShareMemory {
 public:
    virtual ~CShareMemory() {
    }
    virtual size_t Read(char* outbuff, size_t size) {
        return 0;
    }
    virtual size_t Write(const char* inbuff, size_t size) {
        return 0;
    }
    virtual size_t GetSize() {
        return 0;
    }

    //以下两个必须成对使用，仅用于读取，可以使用GetSize()
    virtual const char* GetBuffer(size_t* buffsize) {
        return NULL;
    }
    virtual int ReleaseBuffer(const char* buff, size_t buffsize) {
        return 0;
    }
};

CShareMemory* CreateShareMemory(const char* szShmName);

int DeleteShareMemory(const char* szShmName);

} /* namespace VA */
#endif /* VASHAREMEMORY_H_ */
