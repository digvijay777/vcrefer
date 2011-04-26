#pragma once

template <class T>
class TSortList
{
public:
	TSortList()
	{
		m_head.pFront = m_head.pBack = NULL;
		m_head.pData = NULL;
		m_nCount = 0;
	}
	~TSortList()
	{

	}
	// 连表数据结构(双向链表)
	typedef struct _SortList
	{
		T*			pData;
		_SortList*	pFront;
		_SortList*	pBack;
	}SORTLIST;

protected:
	SORTLIST		m_head;
	int				m_nCount;

public:
	// 得到大小
	int		GetCount() const
	{
		return m_nCount;
	}
	// 得到无素
	T*		GetItem(int i) const
	{
		if(i < 0 || i >= m_nCount)
			return NULL;
		
		SORTLIST	pos		= m_head.pBack;

		while(i-- > 0 && pos)
		{
			pos = pos->pBack;
		}

		if(NULL == pos)
			return NULL;

		return pos->pData;
	}
	// 添加元素
	T*	AddItem(T* pData)
	{

		return pData;
	}
public:
	T* operator[](int i) const
	{
		return GetItem(i);
	};
};