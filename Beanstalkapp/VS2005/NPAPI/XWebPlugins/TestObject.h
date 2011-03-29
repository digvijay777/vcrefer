#pragma once
#include "XWebPlug.h"

class CTestObject : public VPlugObject
{
private:
	CTestObject(void);
public:
	virtual ~CTestObject(void);
public:
	virtual ULONG	GetIDOfName(LPCWSTR name);
	virtual BOOL	CallMethod(ULONG nMethodID, const VARIANT *args, UINT argCount, VARIANT *lpVal);
	virtual BOOL	SetProperty(ULONG nPropertyID, const VARIANT var);
	virtual BOOL	GetProperty(ULONG nPropertyID, VARIANT* lpVal);
	virtual void	SetWindow(HWND hParent, LPRECT lpRect);
	virtual void	Release();

public:
	static VPlugObject* CreateObject();
};
