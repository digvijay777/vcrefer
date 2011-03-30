#include "stdafx.h"
#include "npWebObject.h"

//////////////////////////////////////////////////////////////////////////
CNPVPlugDocument::CNPVPlugDocument(NPP npp, NPObject* pobject)
{
	m_npp = npp;
	m_pobject = pobject;
}

BOOL CNPVPlugDocument::GetCookie(BSTR *lpCookie)
{
	NPObject*		pDoc		= NULL;
	NPIdentifier	cookie;
	NPVariant		valCookie;

	gpnpf->getvalue(m_npp, NPNVWindowNPObject, &pDoc);
	cookie = gpnpf->getstringidentifier((NPUTF8*)"document");
	gpnpf->getproperty(m_npp, pDoc, cookie, &valCookie);
	if(NPVARIANT_IS_OBJECT(valCookie))
	{
		pDoc = NPVARIANT_TO_OBJECT(valCookie);
	}
	cookie = gpnpf->getstringidentifier((NPUTF8*)"cookie");
	gpnpf->getproperty(m_npp, pDoc, cookie, &valCookie);
	if(NPVARIANT_IS_STRING(valCookie))
	{
		VARIANT		var;

		if(FALSE != ConvertNPVariantToVARIANT(valCookie, var))
		{
			*lpCookie = var.bstrVal;
			return TRUE;
		}
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
NPClass CNPWebObject::Object = {
	NP_CLASS_STRUCT_VERSION
		, CNPWebObject::_AllocateScriptable
		, CNPWebObject::_DeallocateScriptable
		, NULL
		, CNPWebObject::_HasMethod
		, CNPWebObject::_Invoke
		, CNPWebObject::_InvokeDefault
		, CNPWebObject::_HasProperty
		, CNPWebObject::_GetProperty
		, CNPWebObject::_SetProperty
		, NULL
		, NULL
		, NULL
};

CNPWebObject::CNPWebObject(NPP npp)
:m_doc(npp, this)
{
	m_npp = npp;
	m_pPlugObject = NULL;
	m_spDispatch = (IDispatchEx *)new CDispatchNPObject(gpnpf, npp, this);
}

CNPWebObject::~CNPWebObject(void)
{
	if(NULL != m_spDispatch)
		m_spDispatch->Release();
	if(NULL != m_pPlugObject)
		m_pPlugObject->Release();
}
// 初始化
void CNPWebObject::Initialize(int16_t argc, char* argn[], char* argv[], VPlugObject* pPlugObject /* = NULL */)
{
	m_pPlugObject = pPlugObject;
	if(NULL != m_pPlugObject)
		m_pPlugObject->SetDocument(&m_doc);
}

// 发出错误
bool CNPWebObject::Error(LPCWSTR lpErr, ...)
{
	va_list		vaArr;
	WCHAR		szError[1024]		= {0};

	va_start(vaArr, lpErr);
	_vsnwprintf(szError, sizeof(szError)/sizeof(WCHAR), lpErr, vaArr);
	va_end(vaArr);

	gpnpf->setexception(this, W2U(szError));
	return false;
}
void CNPWebObject::SetWindow(NPWindow* window)
{
	if(NULL != m_pPlugObject)
	{
		RECT		rect = {0, 0, window->width, window->height};
		m_pPlugObject->SetWindow((HWND)window->window, &rect);
	}
}
// 是否成员函数
bool CNPWebObject::HasMethod(NPIdentifier methodName)
{
	if(NULL == m_pPlugObject)
		return false;

	WCHAR			szName[512]		= {0};
	LPCSTR			lpName			= gpnpf->utf8fromidentifier(methodName);

	MultiByteToWideChar(CP_UTF8, 0, lpName, (int)strlen(lpName), szName, sizeof(szName)/sizeof(WCHAR));
	if(-1 != m_pPlugObject->GetIDOfName(szName))
		return true;

	return false;
}
// 是否属性
bool CNPWebObject::HasProperty(NPIdentifier propertyName)
{
	return false;
}
// 获取属性
bool CNPWebObject::GetProperty(NPIdentifier propertyName, NPVariant *result)
{
	LPCSTR	lpstr		= gpnpf->utf8fromidentifier(propertyName);
	ULONG	nID			= -1;

	nID = m_pPlugObject->GetIDOfName(U2W(lpstr));
	if(-1 == nID)
		return false;

	if(NULL == result)
		return false;

	VARIANT		var;
	
	m_pPlugObject->GetProperty(nID, &var);
	ConvertVARIANTtoNPVariant(var, *result);
	return true;
}
// 设置属性
bool CNPWebObject::SetProperty(NPIdentifier name, const NPVariant *value)
{
	LPCSTR	lpstr		= gpnpf->utf8fromidentifier(name);
	ULONG	nID			= -1;

	nID = m_pPlugObject->GetIDOfName(U2W(lpstr));
	if(-1 == nID)
		return false;

	if(NULL == value)
		return false;

	VARIANT		var;

	ConvertNPVariantToVARIANT(*value, var);
	m_pPlugObject->SetProperty(nID, var);
	return true;
}
// 默认调用过程
bool CNPWebObject::InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return false;
}
// 调用过程
bool CNPWebObject::Invoke(NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	if(NULL == m_pPlugObject)
		return Error(L"尚未实现的方法");

	char*			pMethod			= gpnpf->utf8fromidentifier(methodName);
	WCHAR			szName[512]		= {0};
	LONG			nID				= -1;
	VARIANT*		pVal			= new VARIANT[argCount];
	BOOL			bRet			= FALSE;

	MultiByteToWideChar(CP_UTF8, 0, pMethod, (int)strlen(pMethod), szName, sizeof(szName)/sizeof(WCHAR));
	nID = m_pPlugObject->GetIDOfName(szName);
	for(int i = 0; i < (int)argCount; i++)
	{
		NPVariantToVARIANT(&pVal[i], args[i]);
	}
	bRet = m_pPlugObject->CallMethod(nID, pVal, argCount, NULL);
	delete [] pVal;
	if(FALSE != bRet)
		return true;

	return Error(L"尚未实现的方法");
}
//////////////////////////////////////////////////////////////////////////
// 静态成员
bool CNPWebObject::_HasMethod(NPObject* obj, NPIdentifier methodName)
{
	return ((CNPWebObject*)obj)->HasMethod(methodName);
}
bool CNPWebObject::_HasProperty(NPObject *obj, NPIdentifier propertyName)
{
	return ((CNPWebObject*)obj)->HasProperty(propertyName);
}
bool CNPWebObject::_GetProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result)
{
	return ((CNPWebObject*)obj)->GetProperty(propertyName, result);
}
bool CNPWebObject::_SetProperty(NPObject *obj, NPIdentifier name, const NPVariant *value)
{
	return ((CNPWebObject*)obj)->SetProperty(name, value);
}
bool CNPWebObject::_InvokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return ((CNPWebObject*)obj)->InvokeDefault(args, argCount, result);
}
bool CNPWebObject::_Invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return ((CNPWebObject*)obj)->Invoke(methodName, args, argCount, result);
}
// 分配空间
NPObject* CNPWebObject::_AllocateScriptable(NPP npp, NPClass *aClass)
{
	return new CNPWebObject(npp);
}
// 释放空间 opera 调用不了Object_DeallocateScriptable(原因代查)
void CNPWebObject::_DeallocateScriptable(NPObject *obj)
{
	CNPWebObject*		pobject		= (CNPWebObject *)obj;

	if(NULL != pobject)
		delete pobject;
}