#pragma once
#include <map>
#include <string>
#include <assert.h>

#if defined(UNICODE) || defined(_UNICODE)
	#define String			std::wstring
#else
	#define String			std::string
#endif

template <class T>
class TSkinBase
{
public:
	TSkinBase()
	{
		T		obj;
		
		ASSERT(TRUE == obj.IsKindOf(RUNTIME_CLASS(CGdiObject)));
	}
	~TSkinBase()
	{
		DeleteAllObject();
	}
	
public:
	//////////////////////////////////////////////////////////////////////////
	// 删除所有对像
	void DeleteAllObject()
	{
		std::map<String, T>::iterator		iItem;

		for(iItem = m_mapObjects.begin()
			; iItem != m_mapObjects.end()
			; iItem++)
		{
			if(NULL != iItem->second.GetSafeHandle())
				iItem->second.DeleteObject();
		}

		m_mapObjects.clear();
	}

	//////////////////////////////////////////////////////////////////////////
	// 得到对像
	T* GetItem(String strID)
	{
		std::map<String, T>::iterator		iItem;

		iItem = m_mapObjects.find(strID);

		if(m_mapObjects.end() == iItem)
			return NULL;

		return &iItem->second;
	}

	//////////////////////////////////////////////////////////////////////////
	// 添加对像
	T* AddItem(String strID)
	{
		T		obj;
		T*		pItem;

		pItem = GetItem(strID);
		if(NULL != pItem && pItem->GetSafeHandle())
			pItem->DeleteObject();

		m_mapObjects[strID] = obj;
	}
private:
	std::map<String, T>		m_mapObjects;
};

typedef TSkinBase<CFont>	CSkinFonts;
typedef TSkinBase<CBrush>	CSkinBrushs;
typedef TSkinBase<CBitmap>	CSkinBitmaps;
typedef TSkinBase<CPen>		CSkinPens;
typedef TSkinBase<CRgn>		CSkinRgns;
