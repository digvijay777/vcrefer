#include "stdafx.h"
#include "IENAPAI.h"
#include <map>

//////////////////////////////////////////////////////////////////////////
// 连标识符管理器
CIENPIdentifier::CIENPIdentifier()
{
	m_bstr = NULL;
	m_ulLong = 0;
	m_utf8 = NULL;
	m_bstring = false;
}

CIENPIdentifier::~CIENPIdentifier()
{
	Clear();
}

void CIENPIdentifier::Clear()
{
	if(NULL != m_bstr)
		::SysFreeString(m_bstr);
	if(NULL != m_utf8)
		free(m_utf8);
}
// 判断相等
bool CIENPIdentifier::operator ==(CIENPIdentifier &id) const
{
	if(m_bstring != id.m_bstring)
		return false;

	if(m_bstring)
	{
		if(NULL == m_bstr && NULL == id.m_bstring)
			return true;
		else if(NULL == m_bstr || NULL == id.m_bstr)
			return false;
		return 0 == wcscmp(m_bstr, id.m_bstr);
	}
	else
	{
		return m_ulLong == id.m_ulLong;
	}
}
bool CIENPIdentifier::operator ==(BSTR bstr) const
{
	if(!m_bstring)
		return false;
	if(NULL == m_bstr && NULL == bstr)
		return true;
	else if(NULL == m_bstr || NULL == bstr)
		return false;
	return 0 == wcscmp(m_bstr, bstr);
}
bool CIENPIdentifier::operator ==(ULONG ul) const
{
	if(m_bstring)
		return false;
	return m_ulLong == ul;
}
bool CIENPIdentifier::operator ==(CHAR *utf8) const
{
	if(!m_bstring)
		return false;
	if(NULL == m_utf8 && NULL == utf8)
		return true;
	else if(NULL == m_utf8 || NULL == utf8)
		return false;
	return 0 == strcmp(m_utf8, utf8);
}
// 负值
CIENPIdentifier& CIENPIdentifier::operator =(BSTR bstr)
{
	Clear();

	m_bstring = true;
	m_bstr = ::SysAllocString(bstr);
	m_utf8 = GetUTF8(bstr);
	return *this;
}
CIENPIdentifier& CIENPIdentifier::operator =(CHAR *utf8)
{
	Clear();

	m_bstring = true;
	m_bstr = Utf8ToBSTR(utf8);
	m_utf8 = GetUTF8(utf8);
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
BSTR Utf8ToBSTR(LPCSTR lpStr)
{
	WCHAR*	pStr		= NULL;
	int		nLen		= 0;
	BSTR	bstr		= NULL;

	nLen = MultiByteToWideChar(CP_UTF8, 0, lpStr, strlen(lpStr), NULL, 0);
	nLen++;
	pStr = (WCHAR*)malloc(nLen * 2);
	if(NULL == pStr)
		return NULL;
	memset(pStr, 0, nLen * 2);
	MultiByteToWideChar(CP_UTF8, 0, lpStr, strlen(lpStr), pStr, nLen);
	bstr = ::SysAllocString(pStr);
	free(pStr);
	return bstr;
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
BSTR			IE_GetIdentifierString(NPIdentifier id)
{
	BSTR		bstr		= NULL;

	if(NULL == id)
		return NULL;
	EnterCriticalSection(&gIdentifierLock);
	for(int i = 0; i < (int)gIdentifierMgr.size(); i++)
	{
		if(gIdentifierMgr[i] == id)
		{
			if(gIdentifierMgr[i]->m_bstring)
				bstr = gIdentifierMgr[i]->m_bstr;
			break;
		}
	}
	LeaveCriticalSection(&gIdentifierLock);

	return bstr;
}
CHAR*			IE_GetIdentifierUTF8(NPIdentifier id)
{
	CHAR*		bstr		= NULL;

	if(NULL == id)
		return NULL;
	EnterCriticalSection(&gIdentifierLock);
	for(int i = 0; i < (int)gIdentifierMgr.size(); i++)
	{
		if(gIdentifierMgr[i] == id)
		{
			if(gIdentifierMgr[i]->m_bstring)
				bstr = gIdentifierMgr[i]->m_utf8;
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
			if(!gIdentifierMgr[i]->m_bstring)
				ul = gIdentifierMgr[i]->m_ulLong;
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
		::SysFreeString(item->second);
		item->second = Utf8ToBSTR(utf8);
	}
	else
	{
		gmaperror[pObject] = Utf8ToBSTR(utf8);
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
			::SysFreeString(item->second);
		gmaperror.clear();
	}
}

