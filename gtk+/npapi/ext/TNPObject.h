#pragma once

template <class T>
NPObject* TNPAllocate(NPP npp, NPClass *aClass)
{
	NPObject*		pObject		= (NPObject *)new T(npp);

	if(NULL != pObject)
		pObject->_class = aClass;
	return pObject;
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

//////////////////////////////////////////////////////////////////////////
template <class T>
class TNPClass : NPClass
{
public:
	TNPClass(uint32_t nVersion)
	{
		this->structVersion = nVersion;
		this->allocate = TNPAllocate<T>;
		this->deallocate = TNPDeallocate<T>;
		this->invalidate = NULL;
		this->hasMethod = TNPHasMethod<T>;
		this->invoke = TNPInvoke<T>;
		this->invokeDefault = TNPInvokeDefault<T>;
		this->hasProperty = TNPHasProperty<T>;
		this->getProperty = TNPGetProperty<T>;
		this->setProperty = TNPSetProperty<T>;
		this->removeProperty = TNPRemoveProperty<T>;
		this->enumerate = TNPEnumeration<T>;
		this->construct = TNPConstruct<T>;
	}
};

//////////////////////////////////////////////////////////////////////////
template <class T>
class TNPObject : public NPObject
{
public:
	TNPObject(NPP npp)
	{
		m_npp = npp;
	}
	~TNPObject()
	{
	}
public:
	NPP			m_npp;

public:
	bool	HasMethod(NPIdentifier name)
	{
		return false;
	}
	bool	Invoke(NPIdentifier name, const NPVariant *args
		, uint32_t argCount, NPVariant *result)
	{
		return false;
	}
	bool	InvokeDefault(const NPVariant *args
		, uint32_t argCount, NPVariant *result)
	{
		return false;
	}
	bool	HasProperty(NPIdentifier name)
	{
		return false;
	}
	bool	GetProperty(NPIdentifier name, NPVariant *result)
	{
		return false;
	}
	bool	SetProperty(NPIdentifier name, const NPVariant *value)
	{
		return false;
	}
	bool	RemoveProperty(NPIdentifier name)
	{
		return false;
	}
	bool	Enumeration(NPIdentifier **value, uint32_t *count)
	{
		return false;
	}
	bool	Construct(const NPVariant *args, uint32_t argCount, NPVariant *result)
	{
		return true;
	}
public:
	static TNPClass<T>		Object;
};

template <class T>
TNPClass<T>			TNPObject<T>::Object(NP_CLASS_STRUCT_VERSION);