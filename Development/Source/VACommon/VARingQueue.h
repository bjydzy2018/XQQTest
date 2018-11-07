#pragma once
#include "VACommon.h"

using namespace std;	// For min max under gcc by ycjiang

namespace VA
{
	template<class Type>
	class CRingQueue //单进单出定长循环队列
	{
		volatile size_t m_nReadIndex;//读指针
		volatile size_t m_nWriteIndex;//写指针
		size_t m_nCount;//缓冲区，元素个数，请务必保证是2的幂，后面算法依赖

		bool interAlloc;//内部分配
		Type *m_pQueue;//元素缓冲区内存首地址

		int ilog2(int n) {
			int i, n2, result ;
			n2     = n ;
			result = 0 ;
			for(i = 1 ; i <= n ; i++) {
				if(n2 > 1) {
					result = result + 1 ;
					n2 = n2 / 2 ;
				}
				else {
					return result ;
				}
			}
			return result ;
		}

	public:
		CRingQueue(void)
		{
			m_pQueue = NULL;
			m_nReadIndex = m_nWriteIndex = 0;
			m_nCount = 0;
			interAlloc = false;
		}

		CRingQueue(Type* PreAllocBuff, size_t nCount)
		{
			Init(PreAllocBuff, nCount);
		}

		CRingQueue(size_t nCount)
		{
			Alloc(nCount);
		}

		~CRingQueue(void)
		{
			UnInit();
		}

		void Init(Type* PreAllocBuff, size_t nCount)
		{
			if (nCount & (nCount - 1)) {   
				std::bad_alloc exception;
				throw exception;  
			}
			m_pQueue = PreAllocBuff;
			m_nCount = nCount;
			m_nReadIndex = m_nWriteIndex = 0;
			interAlloc = false;
		}

		void UnInit()
		{
			if(m_pQueue && interAlloc)
				delete[] m_pQueue;

			m_pQueue = NULL;
			m_nReadIndex = m_nWriteIndex = 0;
			m_nCount = 0;
			interAlloc = false;
		}

		void Alloc(size_t nCount)
		{
			if (nCount & (nCount - 1)) {   
				assert(nCount <= 0x80000000);   
				nCount = (nCount==1)?1:(1UL<<(ilog2((nCount)-1)+1));   
			} 
			m_pQueue = new Type[nCount];
			m_nCount = nCount;
			m_nReadIndex = m_nWriteIndex = 0;
			interAlloc = true;
		}

		size_t Put(const Type *buffer, size_t nCount)
		{
			size_t len;
//			assert((m_nWriteIndex & (m_nCount - 1)) == (m_nWriteIndex%m_nCount));
			nCount = min(nCount, m_nCount - m_nWriteIndex + m_nReadIndex);   
			len = min(nCount, m_nCount - (m_nWriteIndex & (m_nCount - 1)));//当B为2的幂时，A&(B-1)==A%B
			memcpy(m_pQueue + (m_nWriteIndex & (m_nCount - 1)), buffer, len*sizeof(Type));
			memcpy(m_pQueue, buffer + len, (nCount - len)*sizeof(Type));
			m_nWriteIndex += nCount;

			return nCount;
		}

		size_t Get(Type *buffer, size_t nCount)
		{
			size_t len;
			//assert((m_nReadIndex & (m_nCount - 1)) == (m_nReadIndex%m_nCount));
			nCount = min(nCount, m_nWriteIndex - m_nReadIndex);   
			len = min(nCount, m_nCount - (m_nReadIndex & (m_nCount - 1)));
			memcpy(buffer, m_pQueue + (m_nReadIndex & (m_nCount - 1)), len*sizeof(Type));   
			memcpy(buffer + len, m_pQueue, (nCount - len)*sizeof(Type));   
			m_nReadIndex += nCount;

			return nCount;   
		}

		void Reset()
		{
			m_nReadIndex = m_nWriteIndex = 0;
		}

		size_t DataCount() const
		{
			return m_nWriteIndex - m_nReadIndex;   
		}

		size_t BufferCount() const
		{
			return m_nCount;   
		}

		size_t RestCount() const
		{
			return m_nCount - (m_nWriteIndex - m_nReadIndex); 
		}

		size_t GetWriteIndex() const
		{
			return m_nWriteIndex; 
		}

		size_t GetReadIndex() const
		{
			return m_nReadIndex; 
		}
	};

	typedef CRingQueue<char> CCharRingQueue;
	typedef CRingQueue<wchar_t> CWCharRingQueue;
}
