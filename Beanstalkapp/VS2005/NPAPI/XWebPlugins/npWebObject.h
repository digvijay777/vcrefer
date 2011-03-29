#pragma once
#include "npWebPlugins.h"


class CNPWebObject : NPObject
{
public:
	CNPWebObject(NPP npp);
	~CNPWebObject(void);

private:
	NPP				m_npp;
	VPlugObject*	m_pPlugObject;

public:
	void		Initialize(int16_t argc, char* argn[], char* argv[], VPlugObject* pPlugObject = NULL);
	bool		Error(LPCWSTR lpErr, ...);

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