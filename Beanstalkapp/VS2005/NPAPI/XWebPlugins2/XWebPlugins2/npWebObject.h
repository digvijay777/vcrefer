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
	static bool _HasMethod(NPObject* obj, NPIdentifier methodName);
	static bool _InvokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result);
	static bool _Invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result);
	static bool _HasProperty(NPObject *obj, NPIdentifier propertyName);
	static bool _GetProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result);
	static bool _SetProperty(NPObject *obj, NPIdentifier name, const NPVariant *value);
	static NPObject* _AllocateScriptable(NPP npp, NPClass *aClass);
	static void _DeallocateScriptable(NPObject *obj);

	static NPClass Object;
};