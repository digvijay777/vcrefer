#pragma once
#include "npWebPlugins.h"

extern NPNetscapeFuncs		gIENpFuncs;


NPError      IE_NPN_GetURL(NPP instance, const char* url, const char* window);
NPError      IE_NPN_PostURL(NPP instance, const char* url, const char* window, uint32_t len, const char* buf, NPBool file);
NPError      IE_NPN_RequestRead(NPStream* stream, NPByteRange* rangeList);
int32_t      IE_NPN_Write(NPP instance, NPStream* stream, int32_t len, void* buffer);
NPError      IE_NPN_NewStream(NPP instance, NPMIMEType type, const char* window, NPStream** stream);
NPError      IE_NPN_DestroyStream(NPP instance, NPStream* stream, NPReason reason);
void         IE_NPN_Status(NPP instance, const char* message);
const char*  IE_NPN_UserAgent(NPP instance);
void*        IE_NPN_MemAlloc(uint32_t size);
void         IE_NPN_MemFree(void* ptr);
uint32_t     IE_NPN_MemFlush(uint32_t size);
void         IE_NPN_ReloadPlugins(NPBool reloadPages);
void*        IE_NPN_GetJavaEnv();
void*        IE_NPN_GetJavaPeer(NPP instance);
NPError      IE_NPN_GetURLNotify(NPP instance, const char* url, const char* window, void* notifyData);
NPError      IE_NPN_PostURLNotify(NPP instance, const char* url, const char* window, uint32_t len, const char* buf, NPBool file, void* notifyData);
NPError      IE_NPN_GetValue(NPP instance, NPNVariable variable, void *ret_value);
NPError      IE_NPN_SetValue(NPP instance, NPPVariable variable, void *value);
void         IE_NPN_InvalidateRect(NPP instance, NPRect *rect);
void         IE_NPN_InvalidateRegion(NPP instance, NPRegion region);
void         IE_NPN_ForceRedraw(NPP instance);
NPIdentifier IE_NPN_GetStringIdentifier(const NPUTF8* name);
void         IE_NPN_GetStringIdentifiers(const NPUTF8** names, int32_t nameCount, NPIdentifier* identifiers);
NPIdentifier IE_NPN_GetIntIdentifier(int32_t intid);
bool         IE_NPN_IdentifierIsString(NPIdentifier identifier);
NPUTF8*      IE_NPN_UTF8FromIdentifier(NPIdentifier identifier);
int32_t      IE_NPN_IntFromIdentifier(NPIdentifier identifier);
NPObject*    IE_NPN_CreateObject(NPP npp, NPClass *aClass);
NPObject*    IE_NPN_RetainObject(NPObject *obj);
void         IE_NPN_ReleaseObject(NPObject *obj);
bool         IE_NPN_Invoke(NPP npp, NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool         IE_NPN_InvokeDefault(NPP npp, NPObject* obj, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool         IE_NPN_Evaluate(NPP npp, NPObject *obj, NPString *script, NPVariant *result);
bool         IE_NPN_GetProperty(NPP npp, NPObject *obj, NPIdentifier propertyName, NPVariant *result);
bool         IE_NPN_SetProperty(NPP npp, NPObject *obj, NPIdentifier propertyName, const NPVariant *value);
bool         IE_NPN_RemoveProperty(NPP npp, NPObject *obj, NPIdentifier propertyName);
bool         IE_NPN_HasProperty(NPP npp, NPObject *obj, NPIdentifier propertyName);
bool         IE_NPN_HasMethod(NPP npp, NPObject *obj, NPIdentifier propertyName);
void         IE_NPN_ReleaseVariantValue(NPVariant *variant);
void         IE_NPN_SetException(NPObject *obj, const NPUTF8 *message);
bool         IE_NPN_PushPopupsEnabledState(NPP npp, NPBool enabled);
bool         IE_NPN_PopPopupsEnabledState(NPP npp);
bool         IE_NPN_Enumerate(NPP npp, NPObject *obj, NPIdentifier **identifier, uint32_t *count);
void         IE_NPN_PluginThreadAsyncCall(NPP instance, void (*func)(void *), void *userData);
bool         IE_NPN_Construct(NPP npp, NPObject* obj, const NPVariant *args, uint32_t argCount, NPVariant *result);
NPError      IE_NPN_GetValueForURL(NPP npp, NPNURLVariable variable, const char *url, char **value, uint32_t *len);
NPError      IE_NPN_SetValueForURL(NPP npp, NPNURLVariable variable, const char *url, const char *value, uint32_t len);
NPError      IE_NPN_GetAuthenticationInfo(NPP npp, const char *protocol, const char *host, int32_t port, const char *scheme, const char *realm, char **username, uint32_t *ulen, char **password, uint32_t *plen);



