#include "stdafx.h"
#include "IENAPAI.h"
#include <map>
#include <MsHTML.h>

NPNetscapeFuncs		gIENpFuncs		= {
	sizeof(NPNetscapeFuncs),
	14,
	IE_NPN_GetURL,
	IE_NPN_PostURL,
	IE_NPN_RequestRead,
	IE_NPN_NewStream,
	IE_NPN_Write,
	IE_NPN_DestroyStream,
	IE_NPN_Status,
	IE_NPN_UserAgent,
	IE_NPN_MemAlloc,
	IE_NPN_MemFree,
	IE_NPN_MemFlush,
	IE_NPN_ReloadPlugins,
	IE_NPN_GetJavaEnv,
	IE_NPN_GetJavaPeer,
	IE_NPN_GetURLNotify,
	IE_NPN_PostURLNotify,
	IE_NPN_GetValue,
	IE_NPN_SetValue,
	IE_NPN_InvalidateRect,
	IE_NPN_InvalidateRegion,
	IE_NPN_ForceRedraw,
	IE_NPN_GetStringIdentifier,
	IE_NPN_GetStringIdentifiers,
	IE_NPN_GetIntIdentifier,
	IE_NPN_IdentifierIsString,
	IE_NPN_UTF8FromIdentifier,
	IE_NPN_IntFromIdentifier,
	IE_NPN_CreateObject,
	IE_NPN_RetainObject,
	IE_NPN_ReleaseObject,
	IE_NPN_Invoke,
	IE_NPN_InvokeDefault,
	IE_NPN_Evaluate,
	IE_NPN_GetProperty,
	IE_NPN_SetProperty,
	IE_NPN_RemoveProperty,
	IE_NPN_HasProperty,
	IE_NPN_HasMethod,
	IE_NPN_ReleaseVariantValue,
	IE_NPN_SetException,
	IE_NPN_PushPopupsEnabledState,
	IE_NPN_PopPopupsEnabledState,
	IE_NPN_Enumerate,
	IE_NPN_PluginThreadAsyncCall,
	IE_NPN_Construct,
	IE_NPN_GetValueForURL,
	IE_NPN_SetValueForURL,
	IE_NPN_GetAuthenticationInfo
};

NPError      IE_NPN_GetURL(NPP instance, const char* url, const char* window)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPError      IE_NPN_PostURL(NPP instance, const char* url, const char* window
								   , uint32_t len, const char* buf, NPBool file)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPError      IE_NPN_RequestRead(NPStream* stream, NPByteRange* rangeList)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
int32_t      IE_NPN_Write(NPP instance, NPStream* stream, int32_t len, void* buffer)
{
	ATLASSERT(FALSE);
	return 0;
}
NPError      IE_NPN_NewStream(NPP instance, NPMIMEType type, const char* window, NPStream** stream)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPError      IE_NPN_DestroyStream(NPP instance, NPStream* stream, NPReason reason)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
void         IE_NPN_Status(NPP instance, const char* message)
{
	ATLASSERT(FALSE);
}
const char*  IE_NPN_UserAgent(NPP instance)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
void*        IE_NPN_MemAlloc(uint32_t size)
{
	return malloc(size);
}
void         IE_NPN_MemFree(void* ptr)
{
	free(ptr);
}
uint32_t     IE_NPN_MemFlush(uint32_t size)
{
	ATLASSERT(FALSE);
	return 0;
}
void         IE_NPN_ReloadPlugins(NPBool reloadPages)
{
	ATLASSERT(FALSE);
}
void*        IE_NPN_GetJavaEnv()
{
	ATLASSERT(FALSE);
	return NULL;
}
void*        IE_NPN_GetJavaPeer(NPP instance)
{
	ATLASSERT(FALSE);
	return NULL;
}
NPError      IE_NPN_GetURLNotify(NPP instance, const char* url, const char* window, void* notifyData)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPError      IE_NPN_PostURLNotify(NPP instance, const char* url, const char* window
										 , uint32_t len, const char* buf, NPBool file, void* notifyData)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPError      IE_NPN_GetValue(NPP instance, NPNVariable variable, void *ret_value)
{
	if(NPNVWindowNPObject == variable)
	{
		// 获取WINDOWS对像
		IDispatch*					pDisp		= (IDispatch *)instance->ndata;
		CComPtr<IOleObject>			spCtrl;
		CComPtr<IOleClientSite>		spSite;
		HRESULT						hres;
		CComPtr<IOleContainer>		spContainer;
		CComPtr<IHTMLDocument2>		spDoc;
		CComPtr<IHTMLWindow2>		spWnd;

		if(NULL == ret_value)
			return NPERR_INVALID_PARAM;
		if(NULL == pDisp)
			return NPERR_INVALID_INSTANCE_ERROR;
		hres = pDisp->QueryInterface(IID_IOleObject, (void **)&spCtrl);
		if(FAILED(hres))
			return NPERR_INVALID_INSTANCE_ERROR;
		hres = spCtrl->GetClientSite(&spSite);
		if(FAILED(hres))
			return NPERR_INVALID_INSTANCE_ERROR;
		hres = spSite->GetContainer(&spContainer);
		if(FAILED(hres))
			return NPERR_INVALID_INSTANCE_ERROR;
		hres = spContainer->QueryInterface(IID_IHTMLDocument2, (void **)&spDoc);
		if(FAILED(hres))
			return NPERR_INVALID_INSTANCE_ERROR;
		hres = spDoc->get_parentWindow(&spWnd);
		if(FAILED(hres))
			return NPERR_INVALID_INSTANCE_ERROR;
		// 构建返回值
		NPP_t					nppt		= {0};
		CComPtr<IDispatch>		spDisp;

		spWnd->QueryInterface(IID_IDispatch, (void **)&spDisp);
		nppt.ndata = (void *)(IDispatch *)spDisp;
		*((NPObject **)(__int64)ret_value) = gIENpFuncs.createobject(&nppt, &gNPDispatch);
	}
	else
	{
		ATLASSERT(FALSE);
	}
	return NPERR_NO_ERROR;
}
NPError      IE_NPN_SetValue(NPP instance, NPPVariable variable, void *value)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
void         IE_NPN_InvalidateRect(NPP instance, NPRect *rect)
{
	ATLASSERT(FALSE);
}
void         IE_NPN_InvalidateRegion(NPP instance, NPRegion region)
{
	ATLASSERT(FALSE);
}
void         IE_NPN_ForceRedraw(NPP instance)
{
	ATLASSERT(FALSE);
}
NPIdentifier IE_NPN_GetStringIdentifier(const NPUTF8* name)
{
	NPIdentifier		identifiers		= NULL;

	IE_NPN_GetStringIdentifiers(&name, 1, &identifiers);
	return identifiers;
}
void         IE_NPN_GetStringIdentifiers(const NPUTF8** names, int32_t nameCount, NPIdentifier* identifiers)
{
	for(int i = 0; i < nameCount; i++)
	{
		identifiers[i] = IE_GetUTF8Identifier((CHAR*)names[i]);
	}
}
NPIdentifier IE_NPN_GetIntIdentifier(int32_t intid)
{
	NPIdentifier		identifiers		= NULL;

	identifiers = IE_GetIntIdentifier(intid);
	return identifiers;
}
bool         IE_NPN_IdentifierIsString(NPIdentifier identifier)
{
	return NULL != IE_GetIdentifierString(identifier);
}
NPUTF8*      IE_NPN_UTF8FromIdentifier(NPIdentifier identifier)
{
	return (NPUTF8 *)IE_GetIdentifierUTF8(identifier);
}
int32_t      IE_NPN_IntFromIdentifier(NPIdentifier identifier)
{
	return (int32_t)IE_GetIdentifierInt(identifier);
}
NPObject*    IE_NPN_CreateObject(NPP npp, NPClass *aClass)
{
	return IE_NPAllocate(npp, aClass);
}
// 增加对像的引用记数
NPObject*    IE_NPN_RetainObject(NPObject *obj)
{
	//ATLASSERT(NULL != obj);
	if(NULL == obj)
		return NULL;
	
	InterlockedIncrement((LONG *)&obj->referenceCount);
	return obj;
}
// 减小对象的引用记数
void         IE_NPN_ReleaseObject(NPObject *obj)
{
	//ATLASSERT(NULL != obj);
	if(NULL == obj)
		return;

	InterlockedDecrement((LONG *)&obj->referenceCount);
	if(obj->referenceCount <= 0)
	{
		IE_NPDeallocate(obj);
	}
}
bool         IE_NPN_Invoke(NPP npp, NPObject* obj, NPIdentifier methodName
								  , const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	ATLASSERT(FALSE);
	return false;
}
bool         IE_NPN_InvokeDefault(NPP npp, NPObject* obj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return obj->_class->invokeDefault(obj, args, argCount, result);
}
bool         IE_NPN_Evaluate(NPP npp, NPObject *obj, NPString *script, NPVariant *result)
{
	ATLASSERT(FALSE);
	return false;
}
bool         IE_NPN_GetProperty(NPP npp, NPObject *obj, NPIdentifier propertyName, NPVariant *result)
{
	//ATLASSERT(FALSE);
	return obj->_class->getProperty(obj, propertyName, result);
}
bool         IE_NPN_SetProperty(NPP npp, NPObject *obj, NPIdentifier propertyName, const NPVariant *value)
{
	ATLASSERT(FALSE);
	return false;
}
bool         IE_NPN_RemoveProperty(NPP npp, NPObject *obj, NPIdentifier propertyName)
{
	ATLASSERT(FALSE);
	return false;
}
bool         IE_NPN_HasProperty(NPP npp, NPObject *obj, NPIdentifier propertyName)
{
	ATLASSERT(FALSE);
	return false;
}
bool         IE_NPN_HasMethod(NPP npp, NPObject *obj, NPIdentifier propertyName)
{
	ATLASSERT(FALSE);
	return false;
}
void         IE_NPN_ReleaseVariantValue(NPVariant *variant)
{
	ATLASSERT(FALSE);
}
void         IE_NPN_SetException(NPObject *obj, const NPUTF8 *message)
{
	IE_SetLastError(obj, (char *)message);
}
bool         IE_NPN_PushPopupsEnabledState(NPP npp, NPBool enabled)
{
	ATLASSERT(FALSE);
	return false;
}
bool         IE_NPN_PopPopupsEnabledState(NPP npp)
{
	ATLASSERT(FALSE);
	return false;
}
bool         IE_NPN_Enumerate(NPP npp, NPObject *obj, NPIdentifier **identifier, uint32_t *count)
{
	ATLASSERT(FALSE);
	return false;
}
void         IE_NPN_PluginThreadAsyncCall(NPP instance, void (*func)(void *), void *userData)
{
	ATLASSERT(FALSE);
}
bool         IE_NPN_Construct(NPP npp, NPObject* obj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	ATLASSERT(FALSE);
	return false;
}
NPError      IE_NPN_GetValueForURL(NPP npp, NPNURLVariable variable, const char *url, char **value, uint32_t *len)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPError      IE_NPN_SetValueForURL(NPP npp, NPNURLVariable variable, const char *url, const char *value, uint32_t len)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPError      IE_NPN_GetAuthenticationInfo(NPP npp, const char *protocol, const char *host
											 , int32_t port, const char *scheme, const char *realm
											 , char **username, uint32_t *ulen, char **password, uint32_t *plen)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}

