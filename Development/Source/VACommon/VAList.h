
#ifndef VALIST_H_
#define VALIST_H_

namespace VA
{
	template <class Type, class TypeArg>
	class CList
	{
		struct ITEM { ITEM *pPrev, *pNext; Type item; };
		ITEM *m_pHead, *m_pTail;
		int m_nCount;

	public:
		typedef void* POSITION;

		CList() { m_pHead = m_pTail = NULL; m_nCount = 0; }
		~CList() { RemoveAll(); }

		CList(const CList& list)
		{
			m_pHead = m_pTail = NULL;
			m_nCount = 0;
			for (ITEM* pItem = list.m_pHead; pItem; pItem = pItem->pNext)
				Add(pItem->item);
		}
		CList& operator=(const CList& list)
		{
			RemoveAll();
			for (ITEM* pItem = list.m_pHead; pItem; pItem = pItem->pNext)
				Add(pItem->item);
			return *this;
		}

		POSITION Add(TypeArg item)
		{
			ITEM* pItem = new ITEM;
			pItem->item = item;
			pItem->pPrev = m_pTail;
			pItem->pNext = NULL;
			if (m_pTail)
				m_pTail->pNext = pItem;
			else
				m_pHead = pItem;
			m_pTail = pItem;
			++m_nCount;
			return pItem;
		}
		void InsertBefore(POSITION posNext, TypeArg item)
		{
			ITEM* pNext = (ITEM*)posNext;
			ITEM* pPrev = pNext->pPrev;
			ITEM* pItem = new ITEM;
			pItem->item = item;
			pItem->pPrev = pPrev;
			pItem->pNext = pNext;
			if (pPrev)
				pPrev->pNext = pItem;
			else
				m_pHead = pItem;
			pNext->pPrev = pItem;
			++m_nCount;
		}
		void InsertAfter(POSITION posPrev, TypeArg item)
		{
			ITEM* pPrev = (ITEM*)posPrev;
			ITEM* pNext = pPrev->pNext;
			ITEM* pItem = new ITEM;
			pItem->item = item;
			pItem->pPrev = pPrev;
			pItem->pNext = pNext;
			if (pNext)
				pNext->pPrev = pItem;
			else
				m_pTail = pItem;
			pPrev->pNext = pItem;
			++m_nCount;
		}
		void Remove(TypeArg item)
		{
			POSITION position = Find(item);
			if (position)
				RemoveAt(position);
		}
		void RemoveAll()
		{
			ITEM* pItem = m_pHead;
			while (pItem)
			{
				ITEM* pNext = pItem->pNext;
				delete pItem;
				pItem = pNext;
			}
			m_pHead = m_pTail = NULL;
			m_nCount = 0;
		}

		int GetCount() const { return m_nCount; }
		bool IsEmpty() const { return m_nCount == 0; }

		POSITION GetHeadPosition() const { return m_pHead; }
		POSITION GetTailPosition() const { return m_pTail; }

		static POSITION GetPreviousPosition(POSITION posCurrent) { return ((ITEM*)posCurrent)->pPrev; }
		static POSITION GetNextPosition(POSITION posCurrent) { return ((ITEM*)posCurrent)->pNext; }

		static Type& GetAt(POSITION position) { return ((ITEM*)position)->item; }

		Type& operator[](POSITION position) { return ((ITEM*)position)->item; }
		const Type& operator[](POSITION position) const { return ((ITEM*)position)->item; }

		POSITION Find(TypeArg item) const
		{
			for (ITEM* pItem = m_pHead; pItem; pItem = pItem->pNext)
				if (pItem->item == item)
					return pItem;
			return NULL;
		}
		void RemoveAt(POSITION position)
		{
			ITEM* pItem = (ITEM*)position;
			ITEM* pPrev = pItem->pPrev;
			ITEM* pNext = pItem->pNext;
			if (pPrev)
				pPrev->pNext = pNext;
			else
				m_pHead = pNext;
			if (pNext)
				pNext->pPrev = pPrev;
			else
				m_pTail = pPrev;
			delete pItem;
			--m_nCount;
		}
	};
}
#endif /* VALIST_H_ */
