#pragma once
#include "npWebPlugins.h"
#include <vector>
#include <DispEx.h>


extern NPNetscapeFuncs		gIENpFuncs;
extern NPPluginFuncs		gIEPlugFuncs;
extern NPClass				gNPDispatch;

//////////////////////////////////////////////////////////////////////////
// NPN 函数
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

//////////////////////////////////////////////////////////////////////////
// NPP 默认函数
NPError NP_LOADDS IE_NPP_New(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char* argn[], char* argv[], NPSavedData* saved);
NPError NP_LOADDS IE_NPP_Destroy(NPP instance, NPSavedData** save);
NPError NP_LOADDS IE_NPP_SetWindow(NPP instance, NPWindow* window);
NPError NP_LOADDS IE_NPP_NewStream(NPP instance, NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype);
NPError NP_LOADDS IE_NPP_DestroyStream(NPP instance, NPStream* stream, NPReason reason);
void    NP_LOADDS IE_NPP_StreamAsFile(NPP instance, NPStream* stream, const char* fname);
int32_t NP_LOADDS IE_NPP_WriteReady(NPP instance, NPStream* stream);
int32_t NP_LOADDS IE_NPP_Write(NPP instance, NPStream* stream, int32_t offset, int32_t len, void* buffer);
void    NP_LOADDS IE_NPP_Print(NPP instance, NPPrint* platformPrint);
int16_t NP_LOADDS IE_NPP_HandleEvent(NPP instance, void* event);
void    NP_LOADDS IE_NPP_URLNotify(NPP instance, const char* url, NPReason reason, void* notifyData);
NPError NP_LOADDS IE_NPP_GetValue(NPP instance, NPPVariable variable, void *value);
NPError NP_LOADDS IE_NPP_SetValue(NPP instance, NPNVariable variable, void *value);


//////////////////////////////////////////////////////////////////////////
// NPOBJECT
NPObject*	IE_NPAllocate(NPP npp, NPClass *aClass);
void		IE_NPDeallocate(NPObject *npobj);
void		IE_NPInvalidate(NPObject *npobj);
bool		IE_NPHasMethod(NPObject *npobj, NPIdentifier name);
bool		IE_NPInvoke(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool		IE_NPInvokeDefault(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool		IE_NPHasProperty(NPObject *npobj, NPIdentifier name);
bool		IE_NPGetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result);
bool		IE_NPSetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value);
bool		IE_NPRemoveProperty(NPObject *npobj, NPIdentifier name);
bool		IE_NPEnumeration(NPObject *npobj, NPIdentifier **value, uint32_t *count);
bool		IE_NPConstruct(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result);

//////////////////////////////////////////////////////////////////////////
// NPVariant 管理器
class  CIENPIdentifier
{
private:
	WCHAR			m_szUnicode[512];
	ULONG			m_ulLong;
	bool			m_bstring;
	CHAR			m_szUtf8[512];

public:
	CIENPIdentifier();
	virtual ~CIENPIdentifier();

public:
	void		Clear();
	bool		IsString();
	LPCWSTR		GetUnicode();
	LPCSTR		GetUtf8();
	ULONG		GetLong();
public:
	bool operator == (CIENPIdentifier& id) const;
	bool operator == (LPCWSTR str) const;
	bool operator == (LPCSTR utf8) const;
	bool operator == (ULONG ul) const;
	CIENPIdentifier& operator=(LPCWSTR str);
	CIENPIdentifier& operator=(ULONG ul);
	CIENPIdentifier& operator=(LPCSTR utf8);
};

CHAR* GetUTF8(LPCWSTR lpStr);
// BSTR Utf8ToBSTR(LPCSTR lpStr);
WCHAR* Utf8ToUnicode(LPCSTR lpStr);
CHAR* GetUTF8(LPCSTR lpStr);
CHAR* AnsiToUtf8(LPCSTR lpStr);

typedef CIENPIdentifier*		IE_NPIdentifier;

NPIdentifier	IE_GetStringIdentifier(BSTR bstr);
NPIdentifier	IE_GetUTF8Identifier(CHAR* pStr);
NPIdentifier	IE_GetIntIdentifier(ULONG nInt);
LPCWSTR			IE_GetIdentifierString(NPIdentifier id);
ULONG			IE_GetIdentifierInt(NPIdentifier id);
LPCSTR			IE_GetIdentifierUTF8(NPIdentifier id);
BSTR			IE_GetLastError(NPObject* pObject);
void			IE_SetLastError(NPObject* pObject, CHAR* utf8);

//////////////////////////////////////////////////////////////////////////
// 初始化NPAPI库
void	InitNPAPI();
void	ReleaseNPAPI();

//////////////////////////////////////////////////////////////////////////
// IENPDispatch
class CNPDispatchEx : public NPObject
{
public:
	CNPDispatchEx(NPP npp);
	virtual ~CNPDispatchEx();

private:
	IDispatchEx*		m_pDispatchEx;

public:
	HRESULT		QueryInterface(REFIID riid, void** ppunk);

public:
	bool		HasMethod(NPIdentifier methodName);
	bool		InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result);
	bool		Invoke(NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result);
	bool		HasProperty(NPIdentifier propertyName);
	bool		GetProperty(NPIdentifier propertyName, NPVariant *result);
	bool		SetProperty(NPIdentifier name, const NPVariant *value);
	bool		RemoveProperty(NPIdentifier name);
	bool		Enumeration(NPIdentifier **value, uint32_t *count);
};

//////////////////////////////////////////////////////////////////////////
// 变量操作
void		IEClearNPVariant(NPVariant& npvar);
bool		IEVariantToNPVariant(NPVariant& npvar, const VARIANT& ievar);
bool		IENPVariantToVariant(VARIANT& ievar, const NPVariant& npvar);