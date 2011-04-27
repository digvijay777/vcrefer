#include "stdafx.h"
#include "IENAPAI.h"
#include <map>

#ifndef arrayof
#define arrayof(x)			( sizeof((x))/sizeof((x)[0]) )
#endif
//////////////////////////////////////////////////////////////////////////
// 连标识符管理器
CIENPIdentifier::CIENPIdentifier()
{
	Clear();
}

CIENPIdentifier::~CIENPIdentifier()
{
	Clear();
}

void CIENPIdentifier::Clear()
{
	memset(m_szUnicode, 0, sizeof(m_szUnicode));
	memset(m_szUtf8, 0, sizeof(m_szUtf8));
	m_ulLong = 0;
	m_bstring = false;
}
bool CIENPIdentifier::IsString()
{
	return m_bstring;
}
LPCWSTR CIENPIdentifier::GetUnicode()
{
	return m_szUnicode;
}
LPCSTR CIENPIdentifier::GetUtf8()
{
	return m_szUtf8;
}
ULONG CIENPIdentifier::GetLong()
{
	return m_ulLong;
}
// 判断相等
bool CIENPIdentifier::operator ==(CIENPIdentifier &id) const
{
	if(m_bstring != id.m_bstring)
		return false;

	if(m_bstring)
	{
		return 0 == wcscmp(m_szUnicode, id.m_szUnicode);
	}
	else
	{
		return m_ulLong == id.m_ulLong;
	}
}
bool CIENPIdentifier::operator ==(LPCWSTR str) const
{
	if(!m_bstring)
		return false;
	
	return 0 == wcscmp(m_szUnicode, str);
}
bool CIENPIdentifier::operator ==(ULONG ul) const
{
	if(m_bstring)
		return false;

	return m_ulLong == ul;
}
bool CIENPIdentifier::operator ==(LPCSTR utf8) const
{
	if(!m_bstring)
		return false;
	
	return 0 == strcmp(m_szUtf8, utf8);
}
// 负值
CIENPIdentifier& CIENPIdentifier::operator =(LPCWSTR str)
{
	Clear();

	m_bstring = true;
	wcsncpy(m_szUnicode, str, arrayof(m_szUnicode));
	WideCharToMultiByte(CP_UTF8, 0, m_szUnicode, wcslen(m_szUnicode)
		, m_szUtf8, arrayof(m_szUtf8), NULL, NULL);

	return *this;
}
CIENPIdentifier& CIENPIdentifier::operator =(LPCSTR utf8)
{
	Clear();

	m_bstring = true;
	strncpy(m_szUtf8, utf8, arrayof(m_szUtf8));
	MultiByteToWideChar(CP_UTF8, 0, m_szUtf8, strlen(m_szUtf8)
		, m_szUnicode, arrayof(m_szUnicode));

	return *this;
}
CIENPIdentifier& CIENPIdentifier::operator =(ULONG ul)
{
	Clear();

	m_bstring = false;
	m_ulLong = ul;
	return *this;
}
//////////////////////////////////////////////////////////////////////////
std::vector<IE_NPIdentifier>		gIdentifierMgr;
CRITICAL_SECTION					gIdentifierLock;


CHAR* GetUTF8(LPCWSTR lpStr)
{
	CHAR*	pUtf8		= NULL;
	int		nLen		= 0;

	nLen = WideCharToMultiByte(CP_UTF8, 0, lpStr, wcslen(lpStr), NULL, 0, NULL, NULL);
	nLen++;
	pUtf8 = (char *)malloc(nLen);
	if(NULL == pUtf8)
		return NULL;
	memset(pUtf8, 0, nLen);
	WideCharToMultiByte(CP_UTF8, 0, lpStr, wcslen(lpStr), pUtf8, nLen, NULL, NULL);
	return pUtf8;
}
CHAR* AnsiToUtf8(LPCSTR lpStr)
{
	WCHAR*		pwstr		= NULL;
	CHAR*		putf8		= NULL;
	int			nLen;

	nLen = MultiByteToWideChar(CP_ACP, 0, lpStr, strlen(lpStr), NULL, 0);
	nLen++;
	pwstr = (WCHAR *)malloc(nLen * 2);
	memset(pwstr, 0, nLen * 2);
	MultiByteToWideChar(CP_ACP, 0, lpStr, strlen(lpStr), pwstr, nLen);
	putf8 = GetUTF8(pwstr);
	free(pwstr);
	return putf8;
}
WCHAR* Utf8ToUnicode(LPCSTR lpStr)
{
	WCHAR*	pStr		= NULL;
	int		nLen		= 0;

	nLen = MultiByteToWideChar(CP_UTF8, 0, lpStr, strlen(lpStr), NULL, 0);
	nLen++;
	pStr = (WCHAR*)malloc(nLen * 2);
	if(NULL == pStr)
		return NULL;
	memset(pStr, 0, nLen * 2);
	MultiByteToWideChar(CP_UTF8, 0, lpStr, strlen(lpStr), pStr, nLen);

	return pStr;
}
CHAR* GetUTF8(LPCSTR lpStr)
{
	CHAR*		pStr;
	int			nLen;

	nLen = strlen(lpStr) + 1;
	pStr = (CHAR *)malloc(nLen);
	memset(pStr, 0, nLen);
	strcpy(pStr, lpStr);
	return pStr;
}
// 添加一个标识符
NPIdentifier	IE_GetStringIdentifier(BSTR bstr)
{
	NPIdentifier		identifier		= NULL;

	EnterCriticalSection(&gIdentifierLock);
	for(int i = 0; i < (int)gIdentifierMgr.size(); i++)
	{
		if((*gIdentifierMgr[i]) == bstr)
		{
			identifier = (NPIdentifier)gIdentifierMgr[i];
			break;
		}
	}
	if(NULL == identifier)
	{
		CIENPIdentifier*		pid		= new CIENPIdentifier();

		*pid = bstr;
		identifier = (NPIdentifier)pid;
		gIdentifierMgr.push_back(pid);
	}
	LeaveCriticalSection(&gIdentifierLock);

	return identifier;
}
// 添加一个标识符
NPIdentifier	IE_GetUTF8Identifier(CHAR* pStr)
{
	NPIdentifier		identifier		= NULL;

	EnterCriticalSection(&gIdentifierLock);
	for(int i = 0; i < (int)gIdentifierMgr.size(); i++)
	{
		if((*gIdentifierMgr[i]) == pStr)
		{
			identifier = (NPIdentifier)gIdentifierMgr[i];
			break;
		}
	}
	if(NULL == identifier)
	{
		CIENPIdentifier*		pid		= new CIENPIdentifier();

		*pid = pStr;
		identifier = (NPIdentifier)pid;
		gIdentifierMgr.push_back(pid);
	}
	LeaveCriticalSection(&gIdentifierLock);

	return identifier;
}
// 添加一个标识符
NPIdentifier	IE_GetIntIdentifier(ULONG nInt)
{
	NPIdentifier		identifier		= NULL;

	EnterCriticalSection(&gIdentifierLock);
	for(int i = 0; i < (int)gIdentifierMgr.size(); i++)
	{
		if((*gIdentifierMgr[i]) == nInt)
		{
			identifier = (NPIdentifier)gIdentifierMgr[i];
			break;
		}
	}
	if(NULL == identifier)
	{
		CIENPIdentifier*		pid		= new CIENPIdentifier();

		*pid = nInt;
		identifier = (NPIdentifier)pid;
		gIdentifierMgr.push_back(pid);
	}
	LeaveCriticalSection(&gIdentifierLock);

	return identifier;
}
LPCWSTR			IE_GetIdentifierString(NPIdentifier id)
{
	LPCWSTR		str		= NULL;

	if(NULL == id)
		return NULL;
	EnterCriticalSection(&gIdentifierLock);
	for(int i = 0; i < (int)gIdentifierMgr.size(); i++)
	{
		if(gIdentifierMgr[i] == id)
		{
			if(gIdentifierMgr[i]->IsString())
				str = gIdentifierMgr[i]->GetUnicode();
			break;
		}
	}
	LeaveCriticalSection(&gIdentifierLock);

	return str;
}
LPCSTR			IE_GetIdentifierUTF8(NPIdentifier id)
{
	LPCSTR		bstr		= NULL;

	if(NULL == id)
		return NULL;
	EnterCriticalSection(&gIdentifierLock);
	for(int i = 0; i < (int)gIdentifierMgr.size(); i++)
	{
		if(gIdentifierMgr[i] == id)
		{
			if(gIdentifierMgr[i]->IsString())
				bstr = gIdentifierMgr[i]->GetUtf8();
			break;
		}
	}
	LeaveCriticalSection(&gIdentifierLock);

	return bstr;
}
ULONG			IE_GetIdentifierInt(NPIdentifier id)
{
	ULONG		ul		= 0;

	if(NULL == id)
		return NULL;
	EnterCriticalSection(&gIdentifierLock);
	for(int i = 0; i < (int)gIdentifierMgr.size(); i++)
	{
		if(gIdentifierMgr[i] == id)
		{
			if(!gIdentifierMgr[i]->IsString())
				ul = gIdentifierMgr[i]->GetLong();
			break;
		}
	}
	LeaveCriticalSection(&gIdentifierLock);

	return ul;
}

//////////////////////////////////////////////////////////////////////////
//
std::map<NPObject*, BSTR>		gmaperror;

BSTR			IE_GetLastError(NPObject* pObject)
{
	BSTR									err		= NULL;
	std::map<NPObject*, BSTR>::iterator		item;

	item = gmaperror.find(pObject);
	if(gmaperror.end() != item)
		err = item->second;
	return err;
}
void			IE_SetLastError(NPObject* pObject, CHAR* utf8)
{
	BSTR									err		= NULL;
	std::map<NPObject*, BSTR>::iterator		item;

	item = gmaperror.find(pObject);
	if(gmaperror.end() != item)
	{
		free(item->second);
		item->second = Utf8ToUnicode(utf8);
	}
	else
	{
		gmaperror[pObject] = Utf8ToUnicode(utf8);
	}
}
//////////////////////////////////////////////////////////////////////////
// 初始化NPAPI库
static LONG		glpapi		= 0L;

void	InitNPAPI()
{
	LONG		napi;
	NPError		npErr;

	napi = InterlockedIncrement(&glpapi);
	if(1 == napi)
	{
		InitializeCriticalSection(&gIdentifierLock);
		npErr = NP_Initialize(&gIENpFuncs);
		ATLASSERT(NPERR_NO_ERROR == npErr);
		npErr = NP_GetEntryPoints(&gIEPlugFuncs);
		ATLASSERT( ((NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR) == gIEPlugFuncs.version );
	}
}

// 释放NPAPI库
void	ReleaseNPAPI()
{
	LONG		napi;

	napi = InterlockedDecrement(&glpapi);
	if(0 == napi)
	{
		NP_Shutdown();
		// 清空Identifier表
		for(int i = 0; i < (int)gIdentifierMgr.size(); i++)
		{
			delete gIdentifierMgr[i];
		}
		gIdentifierMgr.clear();
		DeleteCriticalSection(&gIdentifierLock);
		// 清除最后错误
		std::map<NPObject*, BSTR>::iterator		item;
		for(item = gmaperror.begin(); item != gmaperror.end(); item++)
			free(item->second);
		gmaperror.clear();
	}
}

