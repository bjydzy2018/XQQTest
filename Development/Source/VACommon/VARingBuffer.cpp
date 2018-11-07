/*
 * VARingBuffer.cpp
 *
 *  Created on: 2013年10月16日
 *      Author: ycjiang
 */
#include "VARingBuffer.h"

namespace VA {

#if VA_DEBUG
static CTrace RingBufferTrace("RingBuffer", VA_DEBUG_LEVEL_DEBUG);
#else
static CTrace RingBufferTrace("RingBuffer", VA_DEBUG_LEVEL_ERROR);
#endif
#define DEBUG_TRACE RingBufferTrace.Trace
#define DEBUG_TRACE_DEBUG(...) (RingBufferTrace.TraceLevel(VA_DEBUG_LEVEL_DEBUG, __VA_ARGS__))
#define DEBUG_TRACE_INFOR(...) (RingBufferTrace.TraceLevel(VA_DEBUG_LEVEL_INFOR, __VA_ARGS__))
#define DEBUG_TRACE_ERROR(...) (RingBufferTrace.TraceLevel(VA_DEBUG_LEVEL_ERROR, __VA_ARGS__))
    
RingBuffer::RingBuffer(unsigned long maxSize) {
    m_pBuffer = (char*)malloc(maxSize);
    if(m_pBuffer==NULL) {
        DEBUG_TRACE_ERROR("out of memory, size: %d", maxSize);
        throw(RingBuffer_ErrOutOfMemory);
    }
	memset(m_pBuffer,0,maxSize);
    m_iSize = maxSize;
    m_iStart = 0;
    m_iEnd = 0;
	m_ulMaxTopSize = maxSize;
}
RingBuffer::RingBuffer(unsigned long maxSize,unsigned long ulMaxTopSize)
{
	
	if ((ulMaxTopSize < 4*(BUFFERSIZE_PRE_SEC)) ||(ulMaxTopSize >30*(BUFFERSIZE_PRE_SEC)))
	{
		DEBUG_TRACE_INFOR("ulMaxTopSize ERROR size: %d，set default size %d",ulMaxTopSize,maxSize);
		m_ulMaxTopSize = maxSize;
	}
	else
	{
       m_ulMaxTopSize = ulMaxTopSize;
	}
	m_pBuffer = (char*)malloc(m_ulMaxTopSize);
	if(m_pBuffer==NULL) {
		DEBUG_TRACE_ERROR("out of memory, size: %d", m_ulMaxTopSize);
		throw(RingBuffer_ErrOutOfMemory);
	}
	memset(m_pBuffer,0,m_ulMaxTopSize);
	m_iSize = maxSize;
	m_iStart = 0;
	m_iEnd = 0;
}
RingBuffer::~RingBuffer() {

	if(m_pBuffer) {
        free(m_pBuffer);
		m_pBuffer = NULL;
    }
}

RingBufferErrID RingBuffer::AppendData(const char* data, int length) {
    AUTO_LOCK(&m_hMutex);
	RingBufferErrID iRingBufferRet = RingBuffer_Fail;
    int iStart = m_iStart;
    if ((iStart>m_iEnd) && ((iStart-m_iEnd)>length)) {
        memcpy(m_pBuffer + m_iEnd, data, length);
        m_iEnd += length;
    } else if ((m_iEnd>=iStart) && (m_iSize-m_iEnd)>=length) {
        memcpy(m_pBuffer + m_iEnd, data, length);
        m_iEnd += length;
    } else if ((m_iEnd>=iStart) && (m_iSize-m_iEnd+iStart)>length) {
        memcpy(m_pBuffer + m_iEnd, data, m_iSize - m_iEnd);
        memcpy(m_pBuffer, data + (m_iSize-m_iEnd), length - (m_iSize-m_iEnd));
        m_iEnd = length - (m_iSize - m_iEnd);
    } else {
		iRingBufferRet = ReSizeBuffer();
		if (RingBuffer_Success == iRingBufferRet)
		{
             iRingBufferRet = AppendData(data,length);
		}
        return iRingBufferRet;
    }
    return RingBuffer_Success;
}

RingBufferErrID RingBuffer::GetData(char* data, int length, int *plength) {
	AUTO_LOCK(&m_hMutex);
    int iEnd = m_iEnd;
    if(plength)
        *plength=0;	
    if ((iEnd>m_iStart) && (iEnd-m_iStart)<length) {
        memcpy(data, m_pBuffer + m_iStart, iEnd-m_iStart);
        if(plength)
            *plength = iEnd-m_iStart;
        m_iStart = iEnd;
    } else if ((iEnd-m_iStart)>=length) {
        memcpy(data, m_pBuffer + m_iStart, length);
        if(plength)
            *plength = length;
        m_iStart += length;
    } else if ((m_iStart>iEnd) && ((m_iSize-m_iStart)>=length)) {
        memcpy(data, m_pBuffer + m_iStart, length);
        if(plength)
            *plength = length;
        m_iStart += length;
    } else if ((m_iStart>iEnd) && ((m_iSize-m_iStart+iEnd)<length)) {
        memcpy(data, m_pBuffer + m_iStart, m_iSize - m_iStart);
        memcpy(data + (m_iSize-m_iStart), m_pBuffer, iEnd);
        if(plength)
            *plength = m_iSize-m_iStart+iEnd;
        m_iStart = iEnd;
    } else if ((m_iStart>iEnd) && ((m_iSize-m_iStart+iEnd)>=length)) {
        memcpy(data, m_pBuffer + m_iStart, m_iSize - m_iStart);
        memcpy(data + (m_iSize-m_iStart), m_pBuffer, length - (m_iSize-m_iStart));
        if(plength)
            *plength = length;
        m_iStart = length - m_iSize + m_iStart;
    } else {
        return RingBuffer_ErrBufferEmpty;
    }
    return RingBuffer_Success;
}
RingBufferErrID RingBuffer::GetFixedSizeData(char* data, int length) {
	AUTO_LOCK(&m_hMutex);
	int iEnd = m_iEnd;
    if ((iEnd-m_iStart)>=length) {
        if (data != NULL) memcpy(data, m_pBuffer + m_iStart, length);
        m_iStart += length;
    } else if ((m_iStart>iEnd) && ((m_iSize-m_iStart)>=length)) {
        if (data != NULL) memcpy(data, m_pBuffer + m_iStart, length);
        m_iStart += length;
    } else if ((m_iStart>iEnd) && ((m_iSize-m_iStart+iEnd)>=length)) {
        if (data != NULL) memcpy(data, m_pBuffer + m_iStart, m_iSize - m_iStart);
        if (data != NULL) memcpy(data + (m_iSize-m_iStart), m_pBuffer, length - (m_iSize-m_iStart));
        m_iStart = length - m_iSize + m_iStart;
    } 
	else{
		return RingBuffer_ErrBufferEmpty;
    }
    return RingBuffer_Success;
}

RingBufferErrID RingBuffer::GetSpecStartData(char *data, int nStart, int length, int *plength)
{
	AUTO_LOCK(&m_hMutex);
	int iEnd = m_iEnd;
	int iLen = 0;

	if (plength) {
		*plength = 0;
	}
	if (iEnd > nStart ) {
		if ((iEnd - nStart) < length) {
			memcpy(data, m_pBuffer + nStart, iEnd - nStart);
			iLen = iEnd - nStart;
		} else {
			memcpy(data, m_pBuffer + nStart, length);
			iLen = length;
		}
	} else if (iEnd < nStart) {
		if ((m_iSize - nStart) >= length) {
			memcpy(data, m_pBuffer + nStart, length);
			iLen = length;
		} else {
			if ((m_iSize - nStart + iEnd) < length) {
				memcpy(data, m_pBuffer + nStart, m_iSize - nStart);
				memcpy(data + (m_iSize - nStart), m_pBuffer, iEnd);
				iLen = m_iSize - nStart + iEnd;
			} else {
				memcpy(data, m_pBuffer + nStart, m_iSize - nStart);
				memcpy(data + (m_iSize - nStart), m_pBuffer, length - (m_iSize - nStart));
				iLen = length;
			}
		}
	} else {
		return RingBuffer_ErrBufferEmpty;
	}

	if (plength) {
		*plength = iLen;
	}
	return RingBuffer_Success;
}

RingBufferErrID RingBuffer::DropData() 
{
	AUTO_LOCK(&m_hMutex);
    m_iStart = m_iEnd = 0;
    return RingBuffer_Success;
}

int RingBuffer::GetDataSize() {
	int iEnd = m_iEnd;
	if (iEnd >= m_iStart) {
		return iEnd - m_iStart;
	} else {
		return m_iSize-m_iStart+iEnd;
	}
}

int RingBuffer::GetRestSize()
{
	return m_iSize - GetDataSize();
}

int RingBuffer::GetBufSize()
{
	return m_iSize;
}
/*********************************************************************************
*function name: ReSizeBuffer
*function desc：重新分配ringbuffer大小，每次增加4Sbuffer大小，
                如果封顶内存不够分配4S，分配封顶内存。
*in           ：无
*out          ：RingBuffer_Success 成功
				RingBuffer_ErrBufferFull 扩展失败
*other:
*********************************************************************************/
RingBufferErrID RingBuffer::ReSizeBuffer()
{
	RingBufferErrID iRingBufferRet = RingBuffer_Success;
	RingBufferErrID iRet = RingBuffer_Success;
	unsigned long ulReSize = 0;
	char * pNewBuffer = NULL;
	if (m_iSize >= m_ulMaxTopSize)
	{
		//已经封顶
		iRingBufferRet = RingBuffer_ErrBufferFull;
	}
	else
	{
       do 
       {
		   int iLengthGot = 0;
		   ulReSize = (BUFFERSIZE_PRE_SEC) * 4 + m_iSize;
		   if (ulReSize > m_ulMaxTopSize)
		   {   
			   //封顶值不够封顶，以封顶值为准
			   ulReSize = m_ulMaxTopSize;
		   }
		   pNewBuffer = (char*)malloc(ulReSize);
		   if(pNewBuffer==NULL) 
		   {
			   DEBUG_TRACE_INFOR("out of memory when ReSizeBuffer, size: %d", m_iSize);
			   iRingBufferRet = RingBuffer_ErrBufferFull;
			   break;
		   }
		   memset(pNewBuffer,0,ulReSize);
		   iRet = GetAllData(pNewBuffer,&iLengthGot);
		   if (RingBuffer_Success == iRet)
		   {
               //成功获取有数据 
			   m_iEnd = iLengthGot;
		   }
		   else if (RingBuffer_ErrBufferEmpty == iRet)
		   {
			   //数据为空
			   m_iEnd = 0;
		   }		   
		   else
		   {
               //获取数据失败
			   free(pNewBuffer);
               pNewBuffer = NULL;
			   iRingBufferRet = RingBuffer_ErrBufferFull;
			   break;
		   }
		   m_iStart = 0;
		   free(m_pBuffer);
		   m_pBuffer = pNewBuffer;
		   pNewBuffer = NULL;
		   m_iSize = ulReSize;
       } while (0);
	}
    return iRingBufferRet;
}
/*********************************************************************************
*function name: GetAllData
*function desc：获取buffer中数据
*in           ：data 数据存放位置
                plength 获取的数据大小
*out          ：RingBuffer_Success 成功，其他失败
                RingBuffer_Fail    分配失败
				RingBuffer_ErrBufferEmpty
*other:
*********************************************************************************/
RingBufferErrID RingBuffer::GetAllData(char *data,int *plength)
{
     if (NULL == data)
     {
          DEBUG_TRACE_ERROR("GetAllDate Error Date is NULL");
		  return RingBuffer_Fail;
     }
	 if (!plength)
	 {
		 DEBUG_TRACE_ERROR("GetAllDate Error plength is NULL");
		 return RingBuffer_Fail;
	 }
	 int iDateSize = 0;
	 iDateSize = GetDataSize();
	 int iEnd = m_iEnd;
     *plength=0;

	 if ((iEnd>m_iStart) && (iEnd-m_iStart)<iDateSize) 
	 {
		 memcpy(data, m_pBuffer + m_iStart, iEnd-m_iStart);
	     *plength = iEnd-m_iStart;
	 } 
	 else if ((iEnd-m_iStart)>=iDateSize) 
	 {
		 memcpy(data, m_pBuffer + m_iStart, iDateSize);
		 *plength = iDateSize;
	 } 
	 else if ((m_iStart>iEnd) && ((m_iSize-m_iStart)>=iDateSize)) 
	 {
		 memcpy(data, m_pBuffer + m_iStart, iDateSize);
		 *plength = iDateSize;
	 } 
	 else if ((m_iStart>iEnd) && ((m_iSize-m_iStart+iEnd)<iDateSize))
	 {
		 memcpy(data, m_pBuffer + m_iStart, m_iSize - m_iStart);
		 memcpy(data + (m_iSize-m_iStart), m_pBuffer, iEnd);
		 *plength = m_iSize-m_iStart+iEnd;
	 }
	 else if ((m_iStart>iEnd) && ((m_iSize-m_iStart+iEnd)>=iDateSize)) 
	 {
		 memcpy(data, m_pBuffer + m_iStart, m_iSize - m_iStart);
		 memcpy(data + (m_iSize-m_iStart), m_pBuffer, iDateSize - (m_iSize-m_iStart));
		 *plength = iDateSize;
	 } 
	 else 
	 {
		 return RingBuffer_ErrBufferEmpty;
	 }
	 return RingBuffer_Success;
}

}