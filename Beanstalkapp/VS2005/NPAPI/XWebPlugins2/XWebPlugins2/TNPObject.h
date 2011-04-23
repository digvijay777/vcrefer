#pragma once

template <class T>
NPObject* TNPAllocate(NPP npp, NPClass *aClass)
{
	return (NPObject *)new T(npp);
}

template <class T>
void  TNPDeallocate(NPObject *obj)
{
	T*		pobject		= (T *)obj;

	if(NULL != pobject)
		delete pobject;
}

template <class T>
bool TNPHasMethod(NPObject *npobj, NPIdentifier name)
{
	return ((T *)npobj)->HasMethod(name);
}

template <class T>
bool TNPInvoke(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return ((T *)npobj)->Invoke(name, args, argCount, result);
}

template <class T>
bool TNPInvokeDefault(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return ((T *)npobj)->InvokeDefault(args, argCount, result);
}

template <class T>
bool TNPHasProperty(NPObject *npobj, NPIdentifier name)
{
	return ((T *)npobj)->HasProperty(name);
}

template <class T>
bool TNPGetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
{
	return ((T *)npobj)->GetProperty(name, result);
}

template <class T>
bool TNPSetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
	return ((T *)npobj)->SetProperty(name, value);
}

template <class T>
bool TNPRemoveProperty(NPObject *npobj, NPIdentifier name)
{
	return ((T *)npobj)->RemoveProperty(name);
}

template <class T>
bool TNPEnumeration(NPObject *npobj, NPIdentifier **value, uint32_t *count)
{
	return ((T *)npobj)->Enumeration(value, count);
}

template <class T>
bool TNPConstruct(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return ((T *)npobj)->Construct(args, argCount, result);
}

