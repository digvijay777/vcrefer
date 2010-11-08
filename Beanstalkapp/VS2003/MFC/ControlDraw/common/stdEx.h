#pragma once

#include "NSimple.h"
#include <map>
#include <list>
#include <vector>



class alg
{
public:
	static	void * __cdecl bsearch_bound (
							const void *key,
							const void *base,
							size_t num,
							size_t width,
							int (__cdecl *compare)(const void *, const void *),
							bool *pbSame
							)
	{
		char *lo = (char *)base;
		char *hi = (char *)base + (num - 1) * width;
		char *mid;
		size_t half;
		int result;

		*pbSame = false;
		while (lo <= hi)
		{
			half = num / 2;
			if (half)
			{
				mid = lo + (num & 1 ? half : (half - 1)) * width;
				result = (*compare)(key,mid);
				if (result == 0)
				{
					*pbSame = true;
					return(mid);
				}
				else if (result < 0)
				{
					hi = mid - width;
					num = num & 1 ? half : half-1;
				}
				else
				{
					lo = mid + width;
					num = half;
				}
			}
			else if (num)
			{
				result = (*compare)(key,lo);
				if(result == 0)
					*pbSame = true;

				return lo;
			}
			else
			{
				return lo;
			}
		}

		return lo;
	}
};

class CSortStringList
{
	class MyString
	{
	public:
		MyString()
		{
			m_pString = NULL;
		}
		MyString(const MyString& Old)
		{
			m_pString = NStr::MallocByString(Old.m_pString);
		}
		MyString(CHAR *pString)
		{
			m_pString = NStr::MallocByString(pString);
		}

		~MyString()
		{
			if(m_pString)
				free(m_pString);

			m_pString = NULL;
		}

		int		Compare(const MyString& _Right) const
		{
			return strcmp(m_pString, _Right.m_pString);
		}

		bool operator < (const MyString& _Right) const
		{
			return (Compare(_Right)<0);
		}

		MyString& operator = (const MyString& _Right)
		{
			m_pString = NStr::MallocByString(_Right.m_pString);
			return *this;
		}

		CHAR *	m_pString;
	};

public:
	CSortStringList()
	{

	}

	~CSortStringList()
	{
		Clear();
	}

	BOOL	Clear()
	{
		m_List.clear();

		return TRUE;
	}

	BOOL	AddString(CHAR *pString)
	{
		int			nindex;
		MyString	OneString(pString);

//		pMyString = new MyString(pString);

		nindex = (int)m_List.size();
		m_List[OneString] = nindex;
		return TRUE;
	}

	CHAR *	GetFirst()
	{
		m_Iter = m_List.begin();

		return GetNext();
	}

	CHAR *	GetNext()
	{
		CHAR *		pString;

		if(m_Iter == m_List.end())
			return NULL;

		pString = m_Iter->first.m_pString;
		m_Iter ++;

		return pString;
	}

	std::map<MyString, int>					m_List;
	std::map<MyString, int>::iterator		m_Iter;
};


//	error C2679: binary '=' : no operator found which takes a right-hand operand of type 'CHAR *' (or there is no acceptable conversion)
//	->
//	All overloaded operators except assignment (operator=) are inherited by derived classes.”
//	除了赋值运算符重载函数以外，所有的运算符重载函数都可以被派生类继承。
//	C++标准规定：如果派生类中声明的成员与基类的成员同名，那么，基类的成员会被覆盖，
//		哪怕基类的成员与派生类的成员的数据类型和参数个数都完全不同
//	由于含有缺省赋值运算符重载函数, 导致基类的运算符重载函数失效
class string_NoCase
	: public std::string
{
public:

	string_NoCase& operator=(const CHAR *_Ptr)
	{	// assign [_Ptr, <null>)
		std::string &	ret = assign(_Ptr);
		return (string_NoCase &)ret;
	}

	int		Compare(const string_NoCase& _Right) const
	{
		return stricmp(this->c_str(), _Right.c_str());
	}

	bool operator < (const string_NoCase& _Right) const
	{
		return (Compare(_Right)<0);
	}

};


class ByteMem
	: public std::vector<BYTE, std::allocator<BYTE> >
{
public:
	ByteMem()
	{
	}
	~ByteMem()
	{
	}

	ByteMem& operator = (CHAR * pString)
	{
		setstring(pString);
		return *this;
	}

	bool	setstring(CHAR * pString)
	{
		clear();
		insert(begin(), (BYTE *)pString);
		return true;
	}

	BYTE *	getptr()
	{
		return &(*begin());
	}

	void insert(iterator Where, BYTE * pDat, size_type nSize = -1)
	{
		BYTE *		pEnd;

		if(nSize == -1)
			nSize = strlen((CHAR *)pDat) + 1;

		pEnd = pDat + nSize;
		__super::insert(Where, pDat, pEnd);
		return;
	}

	void insert(iterator Where, ByteMem &add)
	{
		insert(Where, add.getptr(), add.size());
		return;
	}

	void insert(iterator Where, size_type nSize, BYTE Val = 0)
	{
		__super::insert(Where, nSize, Val);
		return;
	}

	void append(BYTE * pDat, size_type nSize = -1)
	{
		insert(end(), pDat, nSize);
		return;
	}

	void append(ByteMem &add)
	{
		insert(end(), add);
		return;
	}
};


// TEMPLATE CLASS 
template<class _Ty, class _Ax = std::allocator<_Ty> >
class sort_vector
	: public std::vector<_Ty, _Ax>
{
public:

	static	int	WINAPIV find_compare(const void *p1, const void *p2)
	{
		_Ty &		v1 = *(_Ty *)p1;
		_Ty &		v2 = *(_Ty *)p2;

		__if_exists(_Ty::Compare)
		{
			return v1.Compare(v2);
		}

		if(v1 < v2)
			return -1;
		if(v2 < v1)
			return 1;
		return 0;
	}

	bool	sort()
	{
		::qsort(&*begin(), size(), sizeof(_Ty), find_compare);	//150ms
//		std::sort(begin(), end());								//312ms 速度慢
		return true;
	}

	iterator	find(_Ty &Key)
	{
		VOID *	pFind;
		pointer	pt;

		if(Key < *begin() || (*this)[size()-1] < Key)
			return end();

//		std::binary_search(begin(), end(), Key)			无返回值
		pFind = bsearch(&Key, &*begin(), size(), sizeof(_Ty), find_compare);
		if(pFind == NULL)
			return end();

		pt = (_Ty *)pFind;
		return iterator(pt);
	}

};


// TEMPLATE CLASS 
template<class _Kty, class _Ty, class _Ax = std::allocator<_Ty> >
class sort_vector2
	: public std::vector<std::pair<_Kty, _Ty>, _Ax>
{
public:

	iterator	add(const _Kty& _Key, const _Ty& _Val)
	{
		iterator		ibound;
		iterator		iInsert;
		bool			bSame;
		int				nCmp;

		ibound = find(_Key, bSame);
		if(bSame)
		{
			ibound->second = _Val;
			return ibound;
		}

		if(ibound != NULL)
		{
			nCmp = find_compare(&_Key, &*ibound);
			if(nCmp > 0)
				ibound ++;
		}

		iInsert = insert(ibound, std::make_pair(_Key, _Val));
//		NData::DbgArry10K(&*begin());

		return ibound;
	}

	static	int	WINAPIV find_compare(const void *p1, const void *p2)
	{
		_Kty &		v1 = *(_Kty *)p1;
		_Kty &		v2 = *(_Kty *)p2;

		if(v1 < v2)
			return -1;
		if(v2 < v1)
			return 1;

		return 0;
	}


	bool	sort()
	{
		::qsort(&*begin(), size(), sizeof(_Ty), find_compare);
//		std::sort(begin(), end());
		return true;
	}

	iterator	find(const _Kty &Key, bool &bSame)
	{
		VOID *		pFind;
		pointer		pt;

		pFind = alg::bsearch_bound(&Key, &*begin(), size(), sizeof(std::pair<_Kty, _Ty>), find_compare, &bSame);

		pt = (std::pair<_Kty, _Ty> *)pFind;
		return iterator(pt);
	}


};



#include <algorithm>

namespace	StdEx
{

/*
	template<class _Ty, class _Ax, class _Rt> inline
	_Rt	FindInList(std::list<_Ty, _Ax> &List, _Ty& Item)
	{
		std::list<_Ty, _Ax>::iterator		Iter;

		for(Iter = List.begin();
			Iter != List.end();
			Iter ++)
		{
			if(Item == *Iter)
				return Iter;
		}

		return Iter;
	}
*/


	template<class _Key, class _Ty, class _Ax>
		_Ty *	FindInMap(std::map<_Key, _Ty, _Ax> &Map, _Key& Key)
	{
		std::map<_Key, _Ty, _Ax>::iterator		Iter;

		Iter = Map.find(Key);
		if(Iter == Map.end())
			return NULL;

		return &Iter->second;
	}


	template<class _Key, class _Ty, class _Ax>
		_Ty *	AddToMap(std::map<_Key, _Ty, _Ax> &Map, _Key& Key, _Ty *pType = NULL)
	{
		_Ty *		pAdd;

		pAdd = &Map[Key];
		if(pType)
			*pAdd = *pType;

		return pAdd;
	}

	template<class _Ty, class _Ax>
		_Ty *	AddToVectorBegin(std::vector<_Ty, _Ax> &vector, _Ty *pType = NULL)
	{
		_Ty *		pAdd;

		std::vector<_Ty, _Ax>::iterator		Iter;

		if(pType)
			Iter = vector.insert(vector.begin(), *pType);
		else
			Iter = vector.insert(vector.begin(), _Ty());

		pAdd = &*Iter;

		return pAdd;
	}

	template<class _Ty, class _Ax>
		_Ty *	AddToVector(std::vector<_Ty, _Ax> &vector, _Ty *pType = NULL)
	{
		_Ty *		pAdd;

		std::vector<_Ty, _Ax>::iterator		Iter;

		if(pType)
			Iter = vector.insert(vector.end(), *pType);
		else
			Iter = vector.insert(vector.end(), _Ty());

		pAdd = &*Iter;

		return pAdd;
	}

	template<class _Ty, class _Ax>
	BOOL	DeleteInList(std::list<_Ty, _Ax> &List, _Ty& Item)
	{
		std::list<_Ty, _Ax>::iterator		Iter;
		std::list<_Ty, _Ax>::iterator		iRetVal;

		for(Iter = List.begin();
			Iter != List.end();
			Iter ++)
		{
			if(Item == *Iter)
			{
				iRetVal = Iter;
				List.erase(Iter);
				return TRUE;
			}
		}

		return FALSE;
	}

}



