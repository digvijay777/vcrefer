#pragma once
#include "npWebPlugins.h"

extern NPNetscapeFuncs		gIENpFuncs;


NPError      IE_NPN_GetURLProcPtr(NPP instance, const char* url, const char* window);
NPError      IE_NPN_PostURLProcPtr(NPP instance, const char* url, const char* window, uint32_t len, const char* buf, NPBool file);
NPError      IE_NPN_RequestReadProcPtr(NPStream* stream, NPByteRange* rangeList);
NPError      IE_NPN_NewStreamProcPtr(NPP instance, NPMIMEType type, const char* window, NPStream** stream);NPN_WriteProcPtr write;
NPError      IE_NPN_DestroyStreamProcPtr(NPP instance, NPStream* stream, NPReason reason);
void         IE_NPN_StatusProcPtr(NPP instance, const char* message);
const char*  IE_NPN_UserAgentProcPtr(NPP instance);
void*        IE_NPN_MemAllocProcPtr(uint32_t size);
void         IE_NPN_MemFreeProcPtr(void* ptr);
uint32_t     IE_NPN_MemFlushProcPtr(uint32_t size);
void         IE_NPN_ReloadPluginsProcPtr(NPBool reloadPages);
void*        IE_NPN_GetJavaEnvProcPtr();
void*        IE_NPN_GetJavaPeerProcPtr(NPP instance);
NPError      IE_NPN_GetURLNotifyProcPtr(NPP instance, const char* url, const char* window, void* notifyData);
NPError      IE_NPN_PostURLNotifyProcPtr(NPP instance, const char* url, const char* window, uint32_t len, const char* buf, NPBool file, void* notifyData);
NPError      IE_NPN_GetValueProcPtr(NPP instance, NPNVariable variable, void *ret_value);
NPError      IE_NPN_SetValueProcPtr(NPP instance, NPPVariable variable, void *value);
void         IE_NPN_InvalidateRectProcPtr(NPP instance, NPRect *rect);
void         IE_NPN_InvalidateRegionProcPtr(NPP instance, NPRegion region);
void         IE_NPN_ForceRedrawProcPtr(NPP instance);
NPIdentifier IE_NPN_GetStringIdentifierProcPtr(const NPUTF8* name);
void         IE_NPN_GetStringIdentifiersProcPtr(const NPUTF8** names, int32_t nameCount, NPIdentifier* identifiers);
NPIdentifier IE_NPN_GetIntIdentifierProcPtr(int32_t intid);
bool         IE_NPN_IdentifierIsStringProcPtr(NPIdentifier identifier);
NPUTF8*      IE_NPN_UTF8FromIdentifierProcPtr(NPIdentifier identifier);
int32_t      IE_NPN_IntFromIdentifierProcPtr(NPIdentifier identifier);
NPObject*    IE_NPN_CreateObjectProcPtr(NPP npp, NPClass *aClass);
NPObject*    IE_NPN_RetainObjectProcPtr(NPObject *obj);
void         IE_NPN_ReleaseObjectProcPtr(NPObject *obj);
bool         IE_NPN_InvokeProcPtr(NPP npp, NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool         IE_NPN_InvokeDefaultProcPtr(NPP npp, NPObject* obj, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool         IE_NPN_EvaluateProcPtr(NPP npp, NPObject *obj, NPString *script, NPVariant *result);
bool         IE_NPN_GetPropertyProcPtr(NPP npp, NPObject *obj, NPIdentifier propertyName, NPVariant *result);
bool         IE_NPN_SetPropertyProcPtr(NPP npp, NPObject *obj, NPIdentifier propertyName, const NPVariant *value);
bool         IE_NPN_RemovePropertyProcPtr(NPP npp, NPObject *obj, NPIdentifier propertyName);
bool         IE_NPN_HasPropertyProcPtr(NPP npp, NPObject *obj, NPIdentifier propertyName);
bool         IE_NPN_HasMethodProcPtr(NPP npp, NPObject *obj, NPIdentifier propertyName);
void         IE_NPN_ReleaseVariantValueProcPtr(NPVariant *variant);
void         IE_NPN_SetExceptionProcPtr(NPObject *obj, const NPUTF8 *message);
bool         IE_NPN_PushPopupsEnabledStateProcPtr(NPP npp, NPBool enabled);
bool         IE_NPN_PopPopupsEnabledStateProcPtr(NPP npp);
bool         IE_NPN_EnumerateProcPtr(NPP npp, NPObject *obj, NPIdentifier **identifier, uint32_t *count);
void         IE_NPN_PluginThreadAsyncCallProcPtr(NPP instance, void (*func)(void *), void *userData);
bool         IE_NPN_ConstructProcPtr(NPP npp, NPObject* obj, const NPVariant *args, uint32_t argCount, NPVariant *result);
NPError      IE_NPN_GetValueForURLPtr(NPP npp, NPNURLVariable variable, const char *url, char **value, uint32_t *len);
NPError      IE_NPN_SetValueForURLPtr(NPP npp, NPNURLVariable variable, const char *url, const char *value, uint32_t len);
NPError      IE_NPN_GetAuthenticationInfoPtr(NPP npp, const char *protocol, const char *host, int32_t port, const char *scheme, const char *realm, char **username, uint32_t *ulen, char **password, uint32_t *plen);