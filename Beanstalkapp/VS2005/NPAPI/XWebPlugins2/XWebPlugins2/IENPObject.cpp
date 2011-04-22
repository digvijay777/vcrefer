#include "StdAfx.h"
#include "IENPObject.h"

static LONG		glpapi		= 0L;

// 初始化NPAPI库
void	InitNPAPI()
{
	LONG		napi;
	NPError		npErr;

	napi = InterlockedIncrement(&glpapi);
	if(1 == napi)
	{
		npErr = NP_Initialize(&gIENpFuncs);
		ATLASSERT(NPERR_NO_ERROR == npErr);
		npErr = NP_GetEntryPoints(&gIEPlugFuncs);
		ATLASSERT( ((NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR) == gIEPlugFuncs.version );
	}
}

// 释放NPAPI库
void	ReleaseNPAPI()
{
	LONG		napi;
	
	napi = InterlockedDecrement(&glpapi);
	if(0 == napi)
	{
		NP_Shutdown();
	}
}

//////////////////////////////////////////////////////////////////////////
// IDispatch的NPAPI接口
CDispatchNPObject::CDispatchNPObject(IDispatch* pDispatch)
{
	ATLASSERT(NULL != pDispatch);
	m_pDispatch = pDispatch;
	// 初始化列表
	this->_class->hasMethod = _HasMethod;
	this->_class->invokeDefault = _InvokeDefault;
	this->_class->invoke = _Invoke;
	this->_class->hasMethod = _HasProperty;
	this->_class->getProperty = _GetProperty;
	this->_class->setProperty = _SetProperty;
}

bool	CDispatchNPObject::HasMethod(NPIdentifier methodName)
{
	return false;
}
bool	CDispatchNPObject::InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return false;
}
bool	CDispatchNPObject::Invoke(NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return false;
}
bool	CDispatchNPObject::HasProperty(NPIdentifier propertyName)
{
	return false;
}
bool	CDispatchNPObject::GetProperty(NPIdentifier propertyName, NPVariant *result)
{
	return false;
}
bool	CDispatchNPObject::SetProperty(NPIdentifier name, const NPVariant *value)
{
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool CDispatchNPObject::_HasMethod(NPObject* obj, NPIdentifier methodName)
{
	return ((CDispatchNPObject*)obj)->HasMethod(methodName);
}
bool CDispatchNPObject::_InvokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return ((CDispatchNPObject*)obj)->InvokeDefault(args, argCount, result);
}
bool CDispatchNPObject::_Invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args
								, uint32_t argCount, NPVariant *result)
{
	return ((CDispatchNPObject*)obj)->Invoke(methodName, args, argCount, result);
}
bool CDispatchNPObject::_HasProperty(NPObject *obj, NPIdentifier propertyName)
{
	return ((CDispatchNPObject*)obj)->HasProperty(propertyName);
}
bool CDispatchNPObject::_GetProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result)
{
	return ((CDispatchNPObject*)obj)->GetProperty(propertyName, result);
}
bool CDispatchNPObject::_SetProperty(NPObject *obj, NPIdentifier name, const NPVariant *value)
{
	return ((CDispatchNPObject*)obj)->SetProperty(name, value);
}
