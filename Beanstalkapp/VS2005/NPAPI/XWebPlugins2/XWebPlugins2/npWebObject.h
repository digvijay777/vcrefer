#pragma once
#include "npWebPlugins.h"

class CNPVPlugDocument /*: public VPlugDocument*/
{
public:
	CNPVPlugDocument(NPP npp, NPObject* pobject);
private:
	NPP				m_npp;
	NPObject*		m_pobject;
public:
	virtual BOOL GetCookie(BSTR* lpCookie);
};

class CNPWebObject : NPObject
{
public:
	CNPWebObject(NPP npp);
	~CNPWebObject(void);

private:
	NPP				m_npp;
//	VPlugObject*	m_pPlugObject;
//	IDispatchEx*	m_spDispatch;
	HWND			m_hWnd;
	CNPVPlugDocument	m_doc;
public:
	void		Initialize(int16_t argc, char* argn[], char* argv[], VOID* pPlugObject = NULL);
	bool		Error(LPCWSTR lpErr, ...);
	void		SetWindow(NPWindow* window);

public:
	bool		HasMethod(NPIdentifier methodName);
	bool		InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result);
	bool		Invoke(NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result);
	bool		HasProperty(NPIdentifier propertyName);
	bool		GetProperty(NPIdentifier propertyName, NPVariant *result);
	bool		SetProperty(NPIdentifier name, const NPVariant *value);

public:
	static NPClass Object;
};