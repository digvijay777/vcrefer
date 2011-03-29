#include "stdafx.h"
#include "npWebObject.h"

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
{
	m_npp = npp;
	m_pPlugObject = NULL;
	m_spDispatch = (IDispatchEx *)new CDispatchNPObject(gpnpf, npp, this);
	CComPtr<IDispatchEx>		sp;
	sp = m_spDispatch;
}

CNPWebObject::~CNPWebObject(void)
{
}
// ��ʼ��
void CNPWebObject::Initialize(int16_t argc, char* argn[], char* argv[], VPlugObject* pPlugObject /* = NULL */)
{
	m_pPlugObject = pPlugObject;
}

// ��������
bool CNPWebObject::Error(LPCWSTR lpErr, ...)
{
	va_list		vaArr;
	WCHAR		szError[1024]		= {0};
	CHAR		szErrorA[3096]		= {0};

	va_start(vaArr, lpErr);
	_vsnwprintf(szError, sizeof(szError)/sizeof(WCHAR), lpErr, vaArr);
	va_end(vaArr);

	WideCharToMultiByte(CP_UTF8, 0, szError, wcslen(szError), szErrorA, sizeof(szErrorA), NULL, NULL);
	gpnpf->setexception(this, szErrorA);
	return false;
}

// �Ƿ��Ա����
bool CNPWebObject::HasMethod(NPIdentifier methodName)
{
	if(NULL == m_pPlugObject)
		return false;

	WCHAR			szName[512]		= {0};
	LPCSTR			lpName			= gpnpf->utf8fromidentifier(methodName);

	MultiByteToWideChar(CP_UTF8, 0, lpName, strlen(lpName), szName, sizeof(szName)/sizeof(WCHAR));
	if(-1 != m_pPlugObject->GetIDOfName(szName))
		return true;

	return false;
}
// �Ƿ�����
bool CNPWebObject::HasProperty(NPIdentifier propertyName)
{
	return false;
}
// ��ȡ����
bool CNPWebObject::GetProperty(NPIdentifier propertyName, NPVariant *result)
{
	return false;
}
// ��������
bool CNPWebObject::SetProperty(NPIdentifier name, const NPVariant *value)
{
	return false;
}
// Ĭ�ϵ��ù���
bool CNPWebObject::InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return false;
}
// ���ù���
bool CNPWebObject::Invoke(NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	if(NULL == m_pPlugObject)
		return Error(L"��δʵ�ֵķ���");

	char*			pMethod			= gpnpf->utf8fromidentifier(methodName);
	WCHAR			szName[512]		= {0};
	LONG			nID				= -1;
	VARIANT*		pVal			= new VARIANT[argCount];
	BOOL			bRet			= FALSE;

	MultiByteToWideChar(CP_UTF8, 0, pMethod, strlen(pMethod), szName, sizeof(szName)/sizeof(WCHAR));
	nID = m_pPlugObject->GetIDOfName(szName);
	for(int i = 0; i < argCount; i++)
	{
		NPVariantToVARIANT(&pVal[i], args[i]);
	}
	bRet = m_pPlugObject->CallMethod(nID, pVal, argCount, NULL);
	delete [] pVal;
	if(FALSE != bRet)
		return true;

	return Error(L"��δʵ�ֵķ���");
}
//////////////////////////////////////////////////////////////////////////
// ��̬��Ա
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
// ����ռ�
NPObject* CNPWebObject::_AllocateScriptable(NPP npp, NPClass *aClass)
{
	return new CNPWebObject(npp);
}
// �ͷſռ� opera ���ò���Object_DeallocateScriptable(ԭ�����)
void CNPWebObject::_DeallocateScriptable(NPObject *obj)
{
	CNPWebObject*		pobject		= (CNPWebObject *)obj;

	if(NULL != pobject)
		delete pobject;
}