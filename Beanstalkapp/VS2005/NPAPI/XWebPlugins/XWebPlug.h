#pragma once

struct VPlugObject
{
	virtual ULONG	GetIDOfName(LPCWSTR name) = 0;
	virtual BOOL	CallMethod(ULONG nMethodID, const VARIANT *args, UINT argCount, VARIANT *lpVal) = 0;
	virtual BOOL	SetProperty(ULONG nPropertyID, const VARIANT var) = 0;
	virtual BOOL	GetProperty(ULONG nPropertyID, VARIANT* lpVal) = 0;
	virtual void	SetWindow(HWND hParent, LPRECT lpRect) = 0;
	virtual void	Release() = 0;
};

struct VPlugDocument
{
	virtual BOOL	GetCookie(LPWSTR lpCookie) = 0;
};