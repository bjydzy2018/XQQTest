/*
 * VARingBuffer.h
 *
 *  Created on: 2013年10月16日
 *      Author: ycjiang
 */

#ifndef _VA_RING_BUFFER_H_
#define _VA_RING_BUFFER_H_

#include <string.h>
#include "VAThread.h"
#include "VADebug.h"

namespace VA {

#define BUFFERSIZE_PRE_SEC (16*1024*2)  /*每秒钟BUFFER大小*/
typedef enum tagRingBufferErrID {
    RingBuffer_Success = 0,
    RingBuffer_Fail = -1,
    RingBuffer_ErrBufferEmpty = -2,
    RingBuffer_ErrBufferFull = -3,
    RingBuffer_ErrOutOfMemory = -4,
} RingBufferErrID;
class RingBuffer {
 private:
    char *m_pBuffer;
    int m_iSize;
    int m_iStart;
    int m_iEnd;
	unsigned long m_ulMaxTopSize;
    mutex_lock m_hMutex;
 public:
    RingBuffer(unsigned long maxSize);
	RingBuffer(unsigned long maxSize,unsigned long ulMaxTopSize);
    ~RingBuffer();

    RingBufferErrID AppendData(const char *data, int length);

    RingBufferErrID GetData(char *data, int length, int *plength);
    RingBufferErrID DropData();

    RingBufferErrID GetFixedSizeData(char *data, int length);
	RingBufferErrID GetSpecStartData(char *data, int nStart, int length, int *plength);
    
	int GetDataSize();
	int GetRestSize();
	int GetBufSize();
private:
    RingBufferErrID ReSizeBuffer();
	RingBufferErrID GetAllData(char *data,int *plength);
};

}

#endif