#pragma once
#include "XWebPlug.h"
#include <vector>

class CTestObject;

typedef BOOL (CTestObject::* ObjectMethod)(const VARIANT *args, UINT argCount, VARIANT *lpVal);
enum ObjectItemType { ObjectItem_Property = 0, ObjectItem_Method };
typedef struct _ObjectItem
{
	ObjectItemType		type;
	std::wstring		name;

	union
	{
		VARIANT			val;
		ObjectMethod	call;
	} value;

	_ObjectItem(ObjectItemType t, LPCWSTR lpName, ObjectMethod c)
	{
		type = t;
		name = lpName;
		if(ObjectItem_Property == t)
			VariantInit(&value.val);
		else if(ObjectItem_Method == t)
			value.call = c;
		else
			value.call = NULL;
	}
} OBJECTITEM;

class CTestObject : public VPlugObject
{
private:
	CTestObject(void);
public:
	virtual ~CTestObject(void);
private:
	VPlugDocument*				m_pPlugDocument;
	std::vector<OBJECTITEM>		m_objItem;
	HWND						m_hParentWnd;
	WCHAR						m_szError[1024];

public:
	BOOL			OnTest(const VARIANT *args, UINT argCount, VARIANT *lpVal);
	BOOL			Error(LPCWSTR lpFmt, ...);
public:
	virtual	LPCWSTR GetLastError();
	virtual	BOOL	SetDocument(VPlugDocument* pDocument);
	virtual ULONG	GetIDOfName(LPCWSTR name);
	virtual BOOL	CallMethod(ULONG nMethodID, const VARIANT *args, UINT argCount, VARIANT *lpVal);
	virtual BOOL	SetProperty(ULONG nPropertyID, const VARIANT var);
	virtual BOOL	GetProperty(ULONG nPropertyID, VARIANT* lpVal);
	virtual void	SetWindow(HWND hParent, LPRECT lpRect);
	virtual void	Release();

public:
	static VPlugObject* CreateObject();
};
