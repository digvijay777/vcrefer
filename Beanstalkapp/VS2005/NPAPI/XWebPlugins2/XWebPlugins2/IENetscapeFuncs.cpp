#include "StdAfx.h"
#include "IENetscapeFuncs.h"

NPNetscapeFuncs		gIENpFuncs		= {
	sizeof(NPNetscapeFuncs),
	0,
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
	ATLASSERT(FALSE);
	return NULL;
}
void         IE_NPN_MemFree(void* ptr)
{
	ATLASSERT(FALSE);
}
uint32_t     IE_NPN_MemFlush(uint32_t size)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
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
	ATLASSERT(FALSE);
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
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
void         IE_NPN_GetStringIdentifiers(const NPUTF8** names, int32_t nameCount, NPIdentifier* identifiers)
{
	ATLASSERT(FALSE);
}
NPIdentifier IE_NPN_GetIntIdentifier(int32_t intid)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
bool         IE_NPN_IdentifierIsString(NPIdentifier identifier)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPUTF8*      IE_NPN_UTF8FromIdentifier(NPIdentifier identifier)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
int32_t      IE_NPN_IntFromIdentifier(NPIdentifier identifier)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPObject*    IE_NPN_CreateObject(NPP npp, NPClass *aClass)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPObject*    IE_NPN_RetainObject(NPObject *obj)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
void         IE_NPN_ReleaseObject(NPObject *obj)
{
	ATLASSERT(FALSE);
}
bool         IE_NPN_Invoke(NPP npp, NPObject* obj, NPIdentifier methodName
								  , const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	ATLASSERT(FALSE);
	return false;
}
bool         IE_NPN_InvokeDefault(NPP npp, NPObject* obj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	ATLASSERT(FALSE);
	return false;
}
bool         IE_NPN_Evaluate(NPP npp, NPObject *obj, NPString *script, NPVariant *result)
{
	ATLASSERT(FALSE);
	return false;
}
bool         IE_NPN_GetProperty(NPP npp, NPObject *obj, NPIdentifier propertyName, NPVariant *result)
{
	ATLASSERT(FALSE);
	return false;
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
	ATLASSERT(FALSE);
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