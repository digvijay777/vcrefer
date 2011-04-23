#include "stdafx.h"
#include "IENAPAI.h"

// 分配空间
NPObject*	IE_NPAllocate(NPP npp, NPClass *aClass)
{
	if(NULL == aClass)
		return NULL;

	NPObject*		pObject	= NULL;

	if(NULL == aClass->allocate)
	{
		pObject = new NPObject;

		pObject->_class = aClass;
		pObject->referenceCount = 1;
	}
	else
	{
		pObject = aClass->allocate(npp, aClass);
		pObject->_class = aClass;
		pObject->referenceCount = 1;
	}
	return pObject;
}
// 释放空间
void		IE_NPDeallocate(NPObject *npobj)
{
	if(NULL == npobj)
		return;
	if(NULL == npobj->_class || NULL == npobj->_class->deallocate)
	{
		delete npobj;
	}
	else
	{
		npobj->_class->deallocate(npobj);
	}
}

void		IE_NPInvalidate(NPObject *npobj)
{
	if(NULL == npobj)
		return ;
	if(NULL != npobj->_class && NULL != npobj->_class->invalidate)
		npobj->_class->invalidate(npobj);
}
// 是否方法
bool		IE_NPHasMethod(NPObject *npobj, NPIdentifier name)
{
	if(NULL == npobj)
		return false;
	if(NULL != npobj->_class && NULL != npobj->_class->hasMethod)
		return npobj->_class->hasMethod(npobj, name);
	ATLASSERT(FALSE);
	return false;
}
bool		IE_NPInvoke(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	if(NULL == npobj)
		return false;
	if(NULL != npobj->_class && NULL != npobj->_class->invoke)
		return npobj->_class->invoke(npobj, name, args, argCount, result);
	ATLASSERT(FALSE);
	return false;
}
bool		IE_NPInvokeDefault(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	if(NULL == npobj)
		return false;
	if(NULL != npobj->_class && NULL != npobj->_class->invokeDefault)
		return npobj->_class->invokeDefault(npobj, args, argCount, result);
	ATLASSERT(FALSE);
	return false;
}
bool		IE_NPHasProperty(NPObject *npobj, NPIdentifier name)
{
	if(NULL == npobj)
		return false;
	if(NULL != npobj->_class && NULL != npobj->_class->hasProperty)
		return npobj->_class->hasProperty(npobj, name);
	ATLASSERT(FALSE);
	return false;
}
bool		IE_NPGetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
{
	if(NULL == npobj)
		return false;
	if(NULL != npobj->_class && NULL != npobj->_class->getProperty)
		return npobj->_class->getProperty(npobj, name, result);
	ATLASSERT(FALSE);
	return false;
}
bool		IE_NPSetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
	if(NULL == npobj)
		return false;
	if(NULL != npobj->_class && NULL != npobj->_class->setProperty)
		return npobj->_class->setProperty(npobj, name, value);
	ATLASSERT(FALSE);
	return false;
}
bool		IE_NPRemoveProperty(NPObject *npobj, NPIdentifier name)
{
	if(NULL == npobj)
		return false;
	if(NULL != npobj->_class && NULL != npobj->_class->removeProperty)
		return npobj->_class->removeProperty(npobj, name);
	ATLASSERT(FALSE);
	return false;
}
bool		IE_NPEnumeration(NPObject *npobj, NPIdentifier **value, uint32_t *count)
{
	if(NULL == npobj)
		return false;
	if(NULL != npobj->_class && NULL != npobj->_class->removeProperty)
		return npobj->_class->enumerate(npobj, value, count);
	ATLASSERT(FALSE);
	return false;
}
bool		IE_NPConstruct(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	if(NULL == npobj)
		return false;
	if(NULL != npobj->_class && NULL != npobj->_class->construct)
		return npobj->_class->construct(npobj, args, argCount, result);
	ATLASSERT(FALSE);
	return false;
}