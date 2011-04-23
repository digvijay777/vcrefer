#pragma once
#include "IENetscapeFuncs.h"
#include "IEPluginFuncs.h"


//////////////////////////////////////////////////////////////////////////
// IDispatchExµÄNPObject¼æÈÝ²ã
class CDispatchNPObject : public NPObject
{
public:
	CDispatchNPObject(IDispatch* pDispatch);

private:
	IDispatch*		m_pDispatch;

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

};