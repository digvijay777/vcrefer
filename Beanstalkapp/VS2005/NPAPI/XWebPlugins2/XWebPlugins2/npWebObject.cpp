#include "stdafx.h"
#include "npWebObject.h"
#include "TNPObject.h"
#include "NPExt.h"
#include "TNPVariant.h"

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

// 		if(FALSE != ConvertNPVariantToVARIANT(valCookie, var))
// 		{
// 			*lpCookie = var.bstrVal;
// 			return TRUE;
// 		}
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
NPClass CNPWebObject::Object = {
	NP_CLASS_STRUCT_VERSION
		, TNPAllocate<CNPWebObject>
		, TNPDeallocate<CNPWebObject>
		, NULL
		, TNPHasMethod<CNPWebObject>
		, TNPInvoke<CNPWebObject>
		, TNPInvokeDefault<CNPWebObject>
		, TNPHasProperty<CNPWebObject>
		, TNPGetProperty<CNPWebObject>
		, TNPSetProperty<CNPWebObject>
		, NULL
		, NULL
		, NULL
};

CNPWebObject::CNPWebObject(NPP npp)
:m_doc(npp, this)
{
	m_npp = npp;
	m_hWnd = NULL;
//	m_pPlugObject = NULL;
	//m_spDispatch = (IDispatchEx *)new CDispatchNPObject(gpnpf, npp, this);
}

CNPWebObject::~CNPWebObject(void)
{
// 	if(NULL != m_spDispatch)
// 		m_spDispatch->Release();
// 	if(NULL != m_pPlugObject)
// 		m_pPlugObject->Release();
}
// 初始化
void CNPWebObject::Initialize(int16_t argc, char* argn[], char* argv[], VOID* pPlugObject /* = NULL */)
{
// 	m_pPlugObject = pPlugObject;
// 	if(NULL != m_pPlugObject)
// 		m_pPlugObject->SetDocument(&m_doc);
}

// 发出错误
bool CNPWebObject::Error(LPCWSTR lpErr, ...)
{
	va_list		vaArr;
	WCHAR		szError[1024]		= {0};

	va_start(vaArr, lpErr);
	_vsnwprintf(szError, sizeof(szError)/sizeof(WCHAR), lpErr, vaArr);
	va_end(vaArr);

// 	gpnpf->setexception(this, W2U(szError));
	return false;
}
void CNPWebObject::SetWindow(NPWindow* window)
{
	m_hWnd = (HWND)window->window;
}
// 是否成员函数
bool CNPWebObject::HasMethod(NPIdentifier methodName)
{
// 	if(NULL == m_pPlugObject)
// 		return false;

	LPCSTR			lpName			= gpnpf->utf8fromidentifier(methodName);

	if(strcmp("Hello", lpName) == 0)
		return true;
	else if(strcmp("Cookie", lpName) == 0)
		return true;
	
// 	if(-1 != m_pPlugObject->GetIDOfName(szName))
// 		return true;

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

// 	nID = m_pPlugObject->GetIDOfName(U2W(lpstr));
	if(-1 == nID)
		return false;

	if(NULL == result)
		return false;

	VARIANT		var;
	
// 	m_pPlugObject->GetProperty(nID, &var);
// 	ConvertVARIANTtoNPVariant(var, *result);
	return true;
}
// 设置属性
bool CNPWebObject::SetProperty(NPIdentifier name, const NPVariant *value)
{
	LPCSTR	lpstr		= gpnpf->utf8fromidentifier(name);
	ULONG	nID			= -1;

// 	nID = m_pPlugObject->GetIDOfName(U2W(lpstr));
// 	if(-1 == nID)
// 		return false;
// 
// 	if(NULL == value)
// 		return false;
// 
// 	VARIANT		var;
// 
// 	ConvertNPVariantToVARIANT(*value, var);
// 	m_pPlugObject->SetProperty(nID, var);
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
// 	if(NULL == m_pPlugObject)
// 		return Error(L"尚未实现的方法");

	char*			pMethod			= gpnpf->utf8fromidentifier(methodName);

	if(strcmp(pMethod, "Hello") == 0)
	{
		if(1 != argCount)
		{
			gpnpf->setexception(this, "Don't have enough parameter.");
			return false;
		}
		NPObject*			pObject		= NULL;
		TNPVariant<>		var(gpnpf);
		TNPVariant<>		callback(gpnpf);

		if(NPVARIANT_IS_OBJECT(args[0]))
		{
			pObject = NPVARIANT_TO_OBJECT(args[0]);
			NPIdentifier		idH1		= gpnpf->getstringidentifier("h1");
			NPIdentifier		idH2		= gpnpf->getstringidentifier("h2");

			gpnpf->getproperty(m_npp, pObject, idH1, &var);
			gpnpf->getproperty(m_npp, pObject, idH2, &callback);

			if(NPVARIANT_IS_OBJECT(callback))
			{
				TNPVariant<>	varstr(gpnpf);

				varstr = "这是测试的内容";
				gpnpf->invokeDefault(m_npp, NPVARIANT_TO_OBJECT(callback), &varstr, 1, NULL);
			}
		}
		MessageBoxA(m_hWnd, "调用方法'Hello'成功!", "Success", MB_OK|MB_ICONINFORMATION);
		if(NULL != result)
		{
			StringToNPVariant(*result, "这是返回值测试");
		}
		return true;
	}
	if(strcmp("Cookie", pMethod) == 0)
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
			*result = valCookie;
		}
		gpnpf->releaseobject(pDoc);
		return true;
	}
// 	for(int i = 0; i < (int)argCount; i++)
// 	{
// 		ConvertNPVariantToVARIANT(args[i], pVal[i], gpnpf, m_npp);
// 	}
// 	VariantInit(&vRet);
// 	bRet = m_pPlugObject->CallMethod(nID, pVal, argCount, &vRet);
// 	delete [] pVal;
// 	if(FALSE == bRet)
// 		return Error(m_pPlugObject->GetLastError());
// 	if(NULL != result)
// 		ConvertVARIANTtoNPVariant(vRet, *result, gpnpf, m_npp);

	char		szError[512]		={0};

	sprintf_s(szError, sizeof(szError), "Unrealized method '%s'.", pMethod);
	gpnpf->setexception(this, szError);
	return false;
}
