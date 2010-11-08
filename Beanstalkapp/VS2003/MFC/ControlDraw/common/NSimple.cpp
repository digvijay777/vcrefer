#include "NSimple.h"
#include <assert.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <shlwapi.h>
#include <ShlObj.h>
#include <stdio.h>
#include <map>
#include <Iphlpapi.h>
#include <NTSecAPI.h>
#include <Ras.h>
#include <Sddl.h>
#include <WinInet.h>
#include "stdEx.h"

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "mpr.lib")

static CHAR HexTable[17]= "0123456789ABCDEF";
static CHAR	szLastPrint[1024] = "Input";

#pragma warning(disable: 4311 4312)

#define STRING_RN		"\r\n"
BOOL	inline	UprCharNoCaseEqu2(BYTE cUpr, BYTE c);


BOOL	NMsg::SetMsgParams(HWND hMsgWnd, CHAR *pMsgCpt, UINT uType)
{
	if(hMsgWnd != (HWND)-1)
		m_hMsgWnd = hMsgWnd;

	if(pMsgCpt)
		NStr::StrnCopy(m_szMsgCpt, pMsgCpt, sizeof(m_szMsgCpt));

	if(uType != -1)
		m_uType = uType;

	return TRUE;
}

int		NMsg::MsgBox(CHAR *pText, CHAR *pTitle, HWND hWnd, UINT uType)
{
	if(pTitle == NULL)
		pTitle = m_szMsgCpt;
	if(hWnd == NULL)
		hWnd = m_hMsgWnd;
	if(uType == -1)
		uType = m_uType;

	if(hWnd == NULL)
		hWnd = GetActiveWindow();

	return ::MessageBoxA(hWnd, pText, pTitle, uType);
}

int		NMsg::Msg(CONST CHAR * lpszFormat, ...)
{
	int		re = -1, size = 4096;
	CHAR	*buf = NULL;
	va_list args;

	va_start(args, lpszFormat);
	for(; re<0; size*=2)
	{
		if(buf)	
			free(buf);
		buf = (CHAR *)malloc(size);
		re = _vsnprintf(buf, size, lpszFormat, args);
	}
	va_end(args);

	re = MsgBox(buf);
	if(buf)
		free(buf);
	return re;
}


int		NMsg::Msgs(CONST CHAR * pMessage)
{
	return MsgBox((CHAR *)pMessage);
}

int		NMsg::MsgErr(int nError)
{
	_bstr_t			bStrText;
	CHAR			szMsgCpt[1024];

	nError = NSys::GetErrorString(&bStrText, nError);
	sprintf(szMsgCpt, "Error: %08X - %d", nError, nError);
	return MsgBox(bStrText);
}

DWORD	NMsg::MsgAry(void * ptr, DWORD n, NData::DATA_PTR_TPYE type, DWORD size)
{
	CHAR			* mbuf, abuf[4096];
	DWORD			i, nedsize = 0;

	switch(type)
	{
	case NData::DataPtrType_P_CHAR:
		for(i=0;i<n;i++)
			nedsize += (DWORD)strlen( *((char**)ptr+i) ) + 26;
		break;

	case NData::DataPtrType_CHAR_Ary:	
		nedsize = n*(size+10);	
		break;

	default:	
		nedsize = n*(10+26);
	}

	mbuf = (char *)malloc(nedsize);
	mbuf[0] = 0;
	for(i=0;i<n;i++)
	{
		switch(type)
		{
		case NData::DataPtrType_DWORD:		sprintf(abuf,"%03d: %08X", i, *((DWORD * )ptr+i));	break;
		case NData::DataPtrType_int  :		sprintf(abuf,"%03d: %ld",  i, *((int *   )ptr+i));	break;
		case NData::DataPtrType_float:		sprintf(abuf,"%03d: %G",   i, *((float * )ptr+i));	break;
		case NData::DataPtrType_double:		sprintf(abuf,"%03d: %G",   i, *((double *)ptr+i));	break;

		default:	
			sprintf(abuf,"%03d: ",i);	
			break;
		}

		strcat(mbuf, abuf);
		switch(type)
		{
		case NData::DataPtrType_P_CHAR: strcat(mbuf, *((char **)ptr+i));	break;
		case NData::DataPtrType_CHAR_Ary: strcat(mbuf, ((char *)ptr+size*i));	break;
		default:	;
		}
		strcat(mbuf,"          \r\n");
	}

	//type 0:DWORD[] 1:int[] 2:char*[] 3:char[][256] 4:char[][size] 5:float[] 6:double[]
	switch(type)
	{
	case NData::DataPtrType_DWORD:		strcpy(abuf,"DWORD  []");	break;
	case NData::DataPtrType_int  :		strcpy(abuf,"int    []");	break;
	case NData::DataPtrType_P_CHAR:		strcpy(abuf,"char  *[]");	break;
	case NData::DataPtrType_CHAR_Ary:	sprintf(abuf,"char   [][%d]",size);	break;
	case NData::DataPtrType_float :		strcpy(abuf,"float  []");	break;
	case NData::DataPtrType_double:		strcpy(abuf,"double []");	break;
	default:	;
	}
	Msg(mbuf, abuf);
	free(mbuf);
	return nedsize;
}

int		NMsg::Msgb(int nType)
{
	return MessageBeep(nType);
}


int		NMsg::MsgBeep(int nType)
{
	return Msgb(nType);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//						NReg
/////////////////////////////////////////////////////////////////////////////////////////////////

BOOL	NReg::GetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, BYTE *pData, DWORD cbData)
{
	HKEY		hSubKey;
	LONG		lRetVal;
	DWORD		dwType;

	lRetVal = RegOpenKeyExA(hKey, pSubKey, 0, KEY_READ, &hSubKey);
	if(lRetVal != ERROR_SUCCESS)
	{
		SetLastError(lRetVal);
		pData[0] = 0;
		return FALSE;
	}

	lRetVal = RegQueryValueExA(hSubKey, pValueName, NULL, &dwType, pData, &cbData);
	if(lRetVal != ERROR_SUCCESS)
	{
		RegCloseKey(hSubKey);
		pData[0] = 0;
		SetLastError(lRetVal);
		return FALSE;
	}

	RegCloseKey(hSubKey);
	return TRUE;
}


BOOL	NReg::GetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, CHAR *pBuffer, int nBufSize)
{
	HKEY		hSubKey;
	LONG		lRetVal;
	DWORD		dwType;

	nBufSize --;
	lRetVal = RegOpenKeyExA(hKey, pSubKey, 0, KEY_READ, &hSubKey);
	if(lRetVal != ERROR_SUCCESS)
	{
		pBuffer[0] = 0;
		return FALSE;
	}

	lRetVal = RegQueryValueExA(hSubKey, pValueName, NULL, &dwType, (BYTE *)pBuffer, (DWORD *)&nBufSize);
	if(lRetVal != ERROR_SUCCESS)
	{
		RegCloseKey(hSubKey);
		pBuffer[0] = 0;
		return FALSE;
	}

	RegCloseKey(hSubKey);
	return TRUE;
}


BOOL	NReg::SHGetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, BYTE *pData, DWORD cbData)
{
	DWORD		dwRetVal;
	DWORD		dwType;

	dwRetVal = SHGetValueA(hKey, pSubKey, pValueName, &dwType, pData, &cbData);
	if(dwRetVal == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL	NReg::SHGetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, CHAR *pData, int nBufSize)
{
	DWORD		dwRetVal;
	DWORD		dwType;

	dwRetVal = SHGetValueA(hKey, pSubKey, pValueName, &dwType, (BYTE *)pData, (DWORD *)&nBufSize);
	if(dwRetVal == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}


BOOL	NReg::SHGetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, DWORD *pdwValue)
{
	DWORD		dwRetVal;
	DWORD		dwType;
	DWORD		cbData;

	dwType = REG_DWORD;
	cbData = sizeof(DWORD);

	dwRetVal = SHGetValueA(hKey, pSubKey, pValueName, &dwType, pdwValue, &cbData);
	if(dwRetVal == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}


BOOL	NReg::GetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, DWORD *pdwValue)
{
	HKEY		hSubKey;
	LONG		lRetVal;
	DWORD		dwType;
	DWORD		cbData;

	lRetVal = RegOpenKeyExA(hKey, pSubKey, 0, KEY_READ, &hSubKey);
	if(lRetVal != ERROR_SUCCESS)
	{
		return FALSE;
	}

	cbData = sizeof(DWORD);
	lRetVal = RegQueryValueExA(hSubKey, pValueName, NULL, &dwType, (BYTE *)pdwValue, &cbData);
	if(lRetVal != ERROR_SUCCESS)
	{
		RegCloseKey(hSubKey);
		return FALSE;
	}

	RegCloseKey(hSubKey);
	return TRUE;
}



BOOL	NReg::SetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, CHAR *pData, BOOL bFlush)
{
	HKEY		hSubKey;
	DWORD		dwDisposition;
	LONG		lRetVal;
	DWORD		cbdata;

	lRetVal = RegCreateKeyExA(hKey, pSubKey, 0, NULL, 0, 
		KEY_WRITE, NULL, &hSubKey, &dwDisposition);
	if(lRetVal != ERROR_SUCCESS)
		return FALSE;

	cbdata = (DWORD)strlen(pData) + 1;
	RegSetValueExA(hSubKey, pValueName, 0, REG_SZ, (BYTE *)pData, cbdata);
	if(lRetVal != ERROR_SUCCESS)
	{
		RegCloseKey(hSubKey);
		return FALSE;
	}

	if(bFlush)
        RegFlushKey(hSubKey);

	RegCloseKey(hSubKey);
	return TRUE;
}


BOOL	NReg::SHSetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, CHAR *pData)
{
	DWORD		dwRetVal;
	DWORD		cbData;

	cbData = (DWORD)strlen(pValueName);

	dwRetVal = SHSetValueA(hKey, pSubKey, pValueName, REG_SZ, pData, cbData);
	if(dwRetVal == 0)
		return TRUE;

	return FALSE;
}

BOOL	NReg::SHSetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, DWORD dwValue)
{
	DWORD		dwRetVal;
	DWORD		cbData;

	cbData = (DWORD)sizeof(DWORD);

	dwRetVal = SHSetValueA(hKey, pSubKey, pValueName, REG_DWORD, &dwValue, cbData);
	if(dwRetVal == 0)
		return TRUE;

	return FALSE;
}

BOOL	NReg::SHSetRegExsz(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, CHAR *pData)
{
	DWORD		dwRetVal;
	DWORD		cbData;

	cbData = (DWORD)strlen(pData);

	dwRetVal = SHSetValueA(hKey, pSubKey, pValueName, REG_EXPAND_SZ, pData, cbData);
	if(dwRetVal == 0)
		return TRUE;

	return FALSE;
}


BOOL	NReg::SetRegMultisz(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, CHAR *pData, BOOL bFlush)
{
	HKEY		hSubKey;
	DWORD		dwDisposition;
	LONG		lRetVal;
	DWORD		cbdata;

	lRetVal = RegCreateKeyExA(hKey, pSubKey, 0, NULL, 0, 
		KEY_WRITE, NULL, &hSubKey, &dwDisposition);
	if(lRetVal != ERROR_SUCCESS)
		return FALSE;

	cbdata = (DWORD)strlen(pData) + 1;
	RegSetValueExA(hSubKey, pValueName, 0, REG_MULTI_SZ, (BYTE *)pData, cbdata);
	if(lRetVal != ERROR_SUCCESS)
	{
		RegCloseKey(hSubKey);
		return FALSE;
	}

	if(bFlush)
		RegFlushKey(hSubKey);

	RegCloseKey(hSubKey);
	return TRUE;
}


BOOL	NReg::SetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, DWORD dwValue, BOOL bFlush)
{
	HKEY		hSubKey;
	DWORD		dwDisposition;
	LONG		lRetVal;
	DWORD		cbdata;

	lRetVal = RegCreateKeyExA(hKey, pSubKey, 0, NULL, 0, 
		KEY_WRITE, NULL, &hSubKey, &dwDisposition);
	if(lRetVal != ERROR_SUCCESS)
		return FALSE;

	cbdata = sizeof(DWORD);
	RegSetValueExA(hSubKey, pValueName, 0, REG_DWORD, (BYTE *)&dwValue, cbdata);
	if(lRetVal != ERROR_SUCCESS)
	{
		RegCloseKey(hSubKey);
		return FALSE;
	}

	if(bFlush)
		RegFlushKey(hSubKey);

	RegCloseKey(hSubKey);
	return TRUE;
}


BOOL	NReg::DelReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName)
{
	HKEY		hSubKey;
	LONG		lRetVal;
	DWORD		dwDisposition;

	lRetVal = RegCreateKeyExA(hKey, pSubKey, 0, NULL, 0, 
		KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisposition);
	if(lRetVal != ERROR_SUCCESS)
		return FALSE;

	lRetVal = RegDeleteValueA(hSubKey, pValueName);
	if(lRetVal != ERROR_SUCCESS)
	{
		RegCloseKey(hSubKey);
		return FALSE;
	}

	RegCloseKey(hSubKey);
	return TRUE;
}


BOOL	NReg::DelRegTree(HKEY hKey, CHAR *pSubKey)
{
	LONG			lResult;
	HKEY			hSubKey = NULL;
	DWORD			dwIndex, cbName;
	CHAR			szSubKey[MAX_PATH*2];
	FILETIME		ft;

	lResult = RegOpenKeyExA(hKey, pSubKey, 0, KEY_ALL_ACCESS,&hSubKey);
	if (lResult == ERROR_SUCCESS)
	{
		for(dwIndex=0; ; dwIndex=0)
		{
			cbName = sizeof(szSubKey);

			lResult = RegEnumKeyExA(hSubKey, dwIndex, szSubKey, &cbName, NULL, NULL, NULL, &ft);
			if(lResult == ERROR_SUCCESS)
				DelRegTree(hSubKey, szSubKey);
			else							
				break;
		}
		RegCloseKey(hSubKey);
	}

	lResult = RegDeleteKeyA(hKey, pSubKey);
	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}


BOOL	NReg::CopyRegTree(HKEY hDestKey, CHAR *pDestSubKey, HKEY hSrcKey, CHAR * pSrcSubKey)
{
	LONG		lResult;
	DWORD		dwDisposition;
	HKEY		hDestSubKey = NULL;
	HKEY		hSrcSubKey = NULL;
	DWORD		dwIndex, dwType, cbName, cbData;
	CHAR		szSubKey[MAX_PATH];
	CHAR		szValueName[MAX_PATH];
	BYTE		szValueData[1024];
	FILETIME	ft;
	BOOL		bRetVal;

	if(pSrcSubKey)
	{
		lResult = RegOpenKeyExA(hSrcKey, pSrcSubKey, 0, KEY_ALL_ACCESS, &hSrcSubKey);
		if (lResult != ERROR_SUCCESS)
			goto done;

		hSrcKey = hSrcSubKey;
	}

	lResult = RegCreateKeyExA(hDestKey, pDestSubKey, 0, NULL, 0,	KEY_WRITE, NULL,
		&hDestSubKey, &dwDisposition);
	if(lResult != ERROR_SUCCESS)
		goto done;


	dwIndex = 0;
	cbName = sizeof(szSubKey);
	lResult = RegEnumKeyExA(hSrcKey, dwIndex, szSubKey, &cbName,
		NULL, NULL, NULL, &ft);
	for(; lResult == ERROR_SUCCESS; )
	{
		bRetVal = CopyRegTree(hDestSubKey, szSubKey, hSrcKey, szSubKey);
		if (bRetVal == FALSE)
		{
			lResult = ERROR_INVALID_DATA;
			goto done;
		}

		dwIndex ++;
		cbName = sizeof(szSubKey);
		lResult = RegEnumKeyExA(hSrcKey, dwIndex, szSubKey, &cbName,
			NULL, NULL, NULL, &ft);
	}


	dwIndex = 0;
	cbName = sizeof(szValueName);
	cbData = sizeof(szValueData);
	lResult = RegEnumValueA(hSrcKey, dwIndex, szValueName, &cbName, 
		NULL, &dwType, szValueData, &cbData);
	for(; lResult == ERROR_SUCCESS; )
	{
		lResult = RegSetValueExA(hDestSubKey, szValueName, 0, dwType, szValueData, cbData);
		if (lResult)
			goto done;

		cbName = sizeof(szValueName);
		cbData = sizeof(szValueData);
		dwIndex ++;
		lResult = RegEnumValueA(hSrcKey, dwIndex, szValueName, &cbName, 
			NULL, &dwType, szValueData, &cbData);
	}

	lResult = ERROR_SUCCESS;

done:
	if (hSrcSubKey)		
		RegCloseKey(hSrcSubKey);
	if (hDestSubKey)	
		RegCloseKey(hDestSubKey);

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}


CHAR *	NReg::GetRootKeyStr(HKEY hKey, CHAR *pBuf)
{
	if(hKey == HKEY_LOCAL_MACHINE)
	{
		strcpy(pBuf, "HKLM");
	}
	else if(hKey == HKEY_CURRENT_USER)
	{
		strcpy(pBuf, "HKCU");
	}
	else if(hKey == HKEY_CLASSES_ROOT)
	{
		strcpy(pBuf, "HKCR");
	}
	else if(hKey == HKEY_USERS)
	{
		strcpy(pBuf, "HKU");
	}
	else if(hKey == HKEY_CURRENT_CONFIG)
	{
		strcpy(pBuf, "HKCC");
	}
	else
	{
		strcpy(pBuf, "");
	}

	return pBuf;
}


CHAR *	NReg::GetRootKeyLongStr(HKEY hKey, CHAR *pBuf)
{
	if(hKey == HKEY_LOCAL_MACHINE)
	{
		strcpy(pBuf, "HKEY_LOCAL_MACHINE");
	}
	else if(hKey == HKEY_CURRENT_USER)
	{
		strcpy(pBuf, "HKEY_CURRENT_USER");
	}
	else if(hKey == HKEY_CLASSES_ROOT)
	{
		strcpy(pBuf, "HKEY_CLASSES_ROOT");
	}
	else if(hKey == HKEY_USERS)
	{
		strcpy(pBuf, "HKEY_USERS");
	}
	else if(hKey == HKEY_CURRENT_CONFIG)
	{
		strcpy(pBuf, "HKEY_CURRENT_CONFIG");
	}
	else
	{
		strcpy(pBuf, "");
	}

	return pBuf;
}

BOOL	NReg::ParseRootKey(CHAR *pRootKey, HKEY *pKey)
{
	if(stricmp(pRootKey, "HKEY_LOCAL_MACHINE") == 0 ||
		stricmp(pRootKey, "HKLM") == 0 ||
		stricmp(pRootKey, "MACHINE") == 0)
	{
		*pKey = HKEY_LOCAL_MACHINE;
		return TRUE;
	}
	if(stricmp(pRootKey, "HKEY_CURRENT_USER") == 0 ||
		stricmp(pRootKey, "HKCU") == 0)
	{
		*pKey = HKEY_CURRENT_USER;
		return TRUE;
	}
	if(stricmp(pRootKey, "HKEY_CLASSES_ROOT") == 0 ||
		stricmp(pRootKey, "HKCR") == 0)
	{
		*pKey = HKEY_CLASSES_ROOT;
		return TRUE;
	}
	if(stricmp(pRootKey, "HKEY_USERS") == 0 ||
		stricmp(pRootKey, "HKU") == 0 ||
		stricmp(pRootKey, "USER") == 0)
	{
		*pKey = HKEY_USERS;
		return TRUE;
	}
	if(stricmp(pRootKey, "HKEY_CURRENT_CONFIG") == 0 ||
		stricmp(pRootKey, "HKCC") == 0)
	{
		*pKey = HKEY_CURRENT_CONFIG;
		return TRUE;
	}

	return FALSE;
}


BOOL	NReg::ParseKrnRegPath(CHAR *pFullPath, HKEY *pRootKey, CHAR *pSubKey)
{
	CHAR *			pHead;
	CHAR *			pFind;
	CHAR			szRootKey[MAX_PATH];
	int				nRootLen;

	pHead = "REGISTRY\\";
	if(NStr::StrHeadCmp(pFullPath, pHead) == 0)
		pFullPath += strlen(pHead);

	pFind = strchr(pFullPath, '\\');
	if(pFind == NULL)
	{
		pSubKey[0] = 0;
		return ParseRootKey(pFullPath, pRootKey);
	}

	nRootLen = (int)(pFind-pFullPath);
	memcpy(szRootKey, pFullPath, nRootLen);
	szRootKey[nRootLen] = 0;

	strcpy(pSubKey, pFind+1);
	return ParseRootKey(szRootKey, pRootKey);


}


BOOL	NReg::ParseRegPath(CHAR *pFullPath, HKEY *pRootKey, CHAR *pSubKey)
{
	CHAR *			pFind;
	CHAR			szRootKey[MAX_PATH];
	int				nRootLen;

	pFind = strchr(pFullPath, '\\');
	if(pFind == NULL)
	{
		pSubKey[0] = 0;
		return ParseRootKey(pFullPath, pRootKey);
	}

	nRootLen = (int)(pFind-pFullPath);
	memcpy(szRootKey, pFullPath, nRootLen);
	szRootKey[nRootLen] = 0;

	strcpy(pSubKey, pFind+1);
	return ParseRootKey(szRootKey, pRootKey);
}


BOOL	NReg::ParseRegPath(CHAR *pFullPath, HKEY *pRootKey, CHAR *pSubKeyName, CHAR *pValueName)
{
	CHAR *			pFind1;
	CHAR *			pFind2;
	CHAR			szRootKey[MAX_PATH];
	int				nPartLen;
	BOOL			bRetVal;

	pFind1 = strchr(pFullPath, '\\');
	pFind2 = strrchr(pFullPath, '\\');
	if(pFind1 == NULL || pFind2 == NULL)
	{
		return FALSE;
	}

	nPartLen = (int)(pFind1-pFullPath);
	memcpy(szRootKey, pFullPath, nPartLen);
	szRootKey[nPartLen] = 0;

	nPartLen = (int)(pFind2-(pFind1+1));
	memcpy(pSubKeyName, pFind1+1, nPartLen);
	pSubKeyName[nPartLen] = 0;

	strcpy(pValueName, pFind2+1);

	bRetVal = ParseRootKey(szRootKey, pRootKey);
	if(bRetVal == FALSE)
		return FALSE;

	return TRUE;
}


BOOL	NReg::ConvertRegPath(CHAR *pFullPath, HKEY RootKey, CHAR *pSubKey, CHAR *pValueName, BOOL bRootLong)
{
	CHAR		szFullPath[MAX_PATH];
	CHAR		szBuffer[MAX_PATH];

	if(bRootLong)
        GetRootKeyLongStr(RootKey, szBuffer);
	else
		GetRootKeyStr(RootKey, szBuffer);
	strcpy(szFullPath, szBuffer);

	if(pValueName)
		sprintf(pFullPath, "%s\\%s\\%s", szBuffer, pSubKey, pValueName);
	else
		sprintf(pFullPath, "%s\\%s", szBuffer, pSubKey);

	return TRUE;
}


BOOL	NReg::RegisterEnum(REGISTER_ENUM_PROC pEnumProc, VOID *pParam, HKEY hRootKey, CHAR *pSubKey, BOOL bEnumSub)
{
	LONG			lResult;
	HKEY			hSubKey = NULL;
	DWORD			dwIndex;
	DWORD			cbName, cbData;
	DWORD			dwType;
	CHAR			szSubKey[MAX_PATH*2];
	CHAR			szFullSubKey[MAX_PATH*2];
	CHAR			szValueName[MAX_PATH*2];
	BYTE			szValueData[10*1024];
	FILETIME		ft;
	BOOL			bContinue;

	lResult = RegOpenKeyExA(hRootKey, pSubKey, 0, KEY_ALL_ACCESS, &hSubKey);
	if(lResult != ERROR_SUCCESS)
		return FALSE;

	dwIndex = 0;
	cbName = sizeof(szSubKey);
	lResult = RegEnumKeyExA(hSubKey, dwIndex, szSubKey, &cbName,
		NULL, NULL, NULL, &ft);
	for(; lResult == ERROR_SUCCESS; )
	{
		bContinue = pEnumProc(pParam, hRootKey, pSubKey, szSubKey, REG_SUBITEM, NULL, 0);
		if(bContinue == FALSE)
		{
			RegCloseKey(hSubKey);
			return TRUE;
		}

		if(bEnumSub)
		{
			if(pSubKey[0])
				sprintf(szFullSubKey, "%s\\%s", pSubKey, szSubKey);
			else
				strcpy(szFullSubKey, szSubKey);
			RegisterEnum(pEnumProc, pParam, hRootKey, szFullSubKey);
		}

		dwIndex ++;
		cbName = sizeof(szSubKey);
		lResult = RegEnumKeyExA(hSubKey, dwIndex, szSubKey, &cbName,
			NULL, NULL, NULL, &ft);
	}


	dwIndex = 0;
	cbName = sizeof(szValueName);
	cbData = sizeof(szValueData);
	szValueData[0] = 0;
	szValueName[0] = 0;
	lResult = RegEnumValueA(hSubKey, dwIndex, szValueName, &cbName, 
		NULL, &dwType, szValueData, &cbData);
	for(; lResult == ERROR_SUCCESS; )
	{
		bContinue = pEnumProc(pParam, hRootKey, pSubKey, szValueName, dwType, szValueData, cbData);
		if(bContinue == FALSE)
			break;

		cbName = sizeof(szValueName);
		cbData = sizeof(szValueData);
		dwIndex ++;
		szValueData[0] = 0;
		szValueName[0] = 0;
		lResult = RegEnumValueA(hSubKey, dwIndex, szValueName, &cbName, 
			NULL, &dwType, szValueData, &cbData);
	}

	RegCloseKey(hSubKey);
	return TRUE;
}



BOOL	NReg::DbgSaveLastMsg(CHAR *pMessage)
{
#ifndef _DEBUG
	return TRUE;
#endif

	static	HKEY		hKey = NULL;
	LONG				lRetVal;

	if(hKey == NULL)
	{
		RegCreateKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\Yofoo", &hKey);
	}

	lRetVal = ::RegSetValueExA(hKey, "DbgSaveLastMsg", 0, REG_SZ, (CONST BYTE *)pMessage, (DWORD)strlen(pMessage)+1);
	if(lRetVal == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}


BOOL WINAPI MUICacheEnumProc(VOID *pParam, HKEY hRootKey, CHAR *pSubKey, CHAR *pValueName, DWORD dwType, BYTE *pValueData, DWORD cbData)
{
	std::map<std::string, std::string> * pmAppCache;

	pmAppCache = (std::map<std::string, std::string> *)pParam;
	std::string			sName;
	std::string			sFilePath;

	if(dwType != REG_SZ)
		return TRUE;

	sName = (CHAR *)pValueData;
	sFilePath = pValueName;

	(*pmAppCache)[sName] = sFilePath;
	return TRUE;
}


BOOL	NReg::GetMUICache(std::map<std::string, std::string> &mAppCache)
{
	BOOL		bRetVal;

	mAppCache.clear();
	bRetVal = RegisterEnum(MUICacheEnumProc, &mAppCache, HKEY_CURRENT_USER, 
		"Software\\Microsoft\\Windows\\ShellNoRoam\\MUICache", FALSE);
	if(bRetVal == FALSE)
		return FALSE;

	return TRUE;
}


BOOL WINAPI UninstallEnumProc(VOID *pParam, HKEY hRootKey, CHAR *pSubKey, CHAR *pValueName, DWORD dwType, BYTE *pValueData, DWORD cbData)
{
	std::map<std::string, std::string> * pmAppCache;

	pmAppCache = (std::map<std::string, std::string> *)pParam;
	std::string			sName;
	std::string			sFilePath;
	CHAR *				pName;

	if(dwType != REG_SZ)
		return TRUE;

	if(stricmp("DisplayName", pValueName) != 0)
		return TRUE;

	pName = (CHAR *)pValueData;
	if(NStr::StrHeadCmp(pName, "Windows") == 0)
		return TRUE;

	sName = pName;
	sFilePath = pSubKey;

	(*pmAppCache)[sName] = sFilePath;
	return TRUE;
}

BOOL	NReg::GetUninstallList(std::map<std::string, std::string> &mAppUninstall)
{
	BOOL		bRetVal;

	mAppUninstall.clear();
	bRetVal = RegisterEnum(UninstallEnumProc, &mAppUninstall, HKEY_LOCAL_MACHINE, 
		"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall", TRUE);
	if(bRetVal == FALSE)
		return FALSE;

	return TRUE;
}


BOOL	NReg::OpenAndSelectRegItem(HKEY hKey, CHAR *pSubKey, CHAR *pValueName)
{
	HWND				hRegEditWnd;
	HWND				hRegTreeWnd;
	HWND				hListWnd;
	SHELLEXECUTEINFOA	ExecInfo;
	BOOL				bRetVal;
	DWORD				dwProcId;
	CAutoHandle			hProc;
	int					i;
	int					nCount;
	std::vector<std::string>	ItemPath;
	CHAR						szRootKey[MAX_PATH];


	hRegEditWnd = FindWindowA("RegEdit_RegEdit", NULL);
	if(hRegEditWnd == NULL)
	{
		memset(&ExecInfo, 0, sizeof(ExecInfo));
		ExecInfo.cbSize = sizeof(ExecInfo);
		ExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_DDEWAIT | SEE_MASK_FLAG_NO_UI;
		ExecInfo.lpVerb = "open";
		ExecInfo.lpFile = "Regedit.exe";
		ExecInfo.nShow = SW_SHOWNORMAL;

		bRetVal = ShellExecuteExA(&ExecInfo);
		if(bRetVal == FALSE)
			return FALSE;

		hProc = ExecInfo.hProcess;
		WaitForInputIdle(hProc, INFINITE);
	}
	else
	{
		GetWindowThreadProcessId(hRegEditWnd, &dwProcId);
		hProc = OpenProcess(SYNCHRONIZE, FALSE, dwProcId);
		if(hProc == NULL)
			return FALSE;
	}
	hRegEditWnd = FindWindowA("RegEdit_RegEdit", NULL);
	if(hRegEditWnd == NULL)
		return FALSE;

	NDlg::SwitchAppWnd(hRegEditWnd);

	hRegTreeWnd = FindWindowExA(hRegEditWnd, NULL, "SysTreeView32", NULL);
	if(hRegTreeWnd == NULL)
		return FALSE;

	for(i=0; i<10; i++)
	{
		nCount = TreeView_GetCount(hRegTreeWnd);
		if(nCount)
			break;
		Sleep(10);
	}

	NStr::SplitString(pSubKey, ItemPath, "\\");
	NReg::GetRootKeyLongStr(hKey, szRootKey);
	ItemPath.insert(ItemPath.begin(), szRootKey);

	bRetVal = NDlg::TreeCtrl_RmtSelectItem(hRegTreeWnd, ItemPath);
	if(pValueName == NULL)
		return bRetVal;

	WaitForInputIdle(hProc, INFINITE);
	hListWnd = FindWindowExA(hRegEditWnd, NULL, "SysListView32", NULL);
	if(hListWnd == NULL)
		return FALSE;

	NDlg::SetWndFocus(hListWnd);
	for(i=0; i<10; i++)
	{
		bRetVal = NDlg::ListCtrl_RmtSelectItem(hListWnd, pValueName);
		if(bRetVal)
			break;

		Sleep(10);
	}

	if(bRetVal == FALSE)
		ATLTRACE("ListCtrl_RmtSelectItem Failed: %s\r\n", pValueName);

	return bRetVal;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//						NLog
/////////////////////////////////////////////////////////////////////////////////////////////////



int		NLog::Logs(CHAR *pString, CHAR *pLogFile)
{
	HANDLE			hFile;
	int				nLen, nError;
	BOOL			bRetVal;
	DWORD			dwRetVal, dwLenWrite;

	if(pLogFile == NULL)
	{
		pLogFile = s_ThisLogFile;
	}

	if(pString == NULL)
	{
		hFile = CreateFileA(pLogFile, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ,
			NULL, CREATE_ALWAYS , 0, NULL);
		CloseHandle(hFile);
		return 0;
	}

	hFile = CreateFileA(pLogFile, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL, OPEN_ALWAYS, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	nLen = (int)strlen(pString);
	dwRetVal = SetFilePointer(hFile, 0, 0, FILE_END);
	if(dwRetVal == 0xFFFFFFFF)
	{
		nError = ::GetLastError();
		if(nError != NO_ERROR)
		{
			CloseHandle(hFile);
			::SetLastError(nError);
			return -1;
		}
	}

	bRetVal = WriteFile(hFile, pString, nLen, &dwLenWrite, NULL);
	if(bRetVal == FALSE)
	{
		nError = ::GetLastError();
		CloseHandle(hFile);
		::SetLastError(nError);
		return -1;
	}
	CloseHandle(hFile);
	return dwLenWrite;
}

BOOL	NLog::SetLogFile(CHAR *pNewLogFile, BOOL bClear)
{
	if(pNewLogFile == NULL || strlen(pNewLogFile) == 0)
		return FALSE;

	strcpy(s_ThisLogFile, pNewLogFile);

	if(bClear)
		Logs(NULL, NULL);

	return TRUE;
}


int		NLog::LogFile(CHAR *pLogFile, CONST CHAR * lpszFormat, ...)
{
	CHAR *				buf = NULL;
	int					nRetVal = -1, nSize = 4096;
	va_list				args;

	if(lpszFormat == NULL)
		return Logs(NULL, pLogFile);

	va_start(args, lpszFormat);

	for(; nRetVal<0 ; nSize*=2)
	{
		if(buf)	
			free(buf);

		buf = (CHAR *)malloc( (nSize+2)*sizeof(CHAR) );
		nRetVal = _vsnprintf(buf, nSize, lpszFormat, args);
	}

	va_end(args);
	strcat(buf, "\r\n");

	nRetVal = Logs(buf, pLogFile);

	if(buf)	
		free(buf);
	return nRetVal;
}


int		NLog::SetTimedLogType(TIMED_LOG_TYPE nType)
{
	s_TimeLogType = nType;

	if(s_TimeLogType > LT_TYPE_TIME_mS ||
		s_TimeLogType < LT_TYPE_DAY)
		s_TimeLogType = LT_TYPE_TIME;

	return s_TimeLogType;
}

int		NLog::LogString(CONST CHAR * lpszFormat, ...)
{
	CHAR *			buf = NULL;
	int				nRetVal = -1, nSize = 4096;
	va_list			args;

	if(lpszFormat == NULL)
		return Logs(NULL);

	va_start(args, lpszFormat);

	for(; nRetVal<0 ; nSize*=2)
	{
		if(buf)	
			free(buf);

		buf = (CHAR *)malloc( (nSize+2)*sizeof(CHAR) );
		nRetVal = _vsnprintf(buf, nSize, lpszFormat, args);
	}

	va_end(args);
	strcat(buf, "\r\n");

	nRetVal = Logs(buf);

	if(buf)	
		free(buf);
	return nRetVal;
}



int		NLog::LogPrintf(CONST CHAR * lpszFormat, ...)
{
	CHAR *			buf = NULL;
	int				nRetVal = -1, nSize = 4096;
	va_list			args;

	if(lpszFormat == NULL)
		return Logs(NULL);

	va_start(args, lpszFormat);

	for(; nRetVal<0 ; nSize*=2)
	{
		if(buf)	
			free(buf);

		buf = (CHAR *)malloc( (nSize+2)*sizeof(CHAR) );
		nRetVal = _vsnprintf(buf, nSize, lpszFormat, args);
	}

	va_end(args);

	strncpy(szLastPrint, buf, sizeof(szLastPrint));
	nRetVal = Logs(buf);

	if(buf)	
		free(buf);
	return nRetVal;

}


int		NLog::TimedLog(CONST CHAR * lpszFormat, ...)
{
	CHAR *			buf = NULL;
	int				nRetVal = -1, nSize = 4096;
	va_list			args;
	CHAR			szTime[1024];
	int				nAddLen;

	if(lpszFormat == NULL)
		return Logs(NULL);

	switch(s_TimeLogType)
	{
	case LT_TYPE_DAY:
		NStr::Day2String(szTime);
		break;
	case LT_TYPE_TIME:
		NStr::Time2String(szTime);
		break;
	case LT_TYPE_TIME_mS:
		NStr::Time2String(szTime, 128, TRUE);
		break;
	default:
		szTime[0] = 0;
	}
	strcat(szTime, " ");
	nAddLen = (int)strlen(szTime);


	va_start(args, lpszFormat);

	for(; nRetVal<0 ; nSize*=2)
	{
		if(buf)	
			free(buf);

		buf = (CHAR *)malloc( (nSize)*sizeof(CHAR) );
		nRetVal = _vsnprintf(buf+nAddLen, nSize-nAddLen-4, lpszFormat, args);
	}

	va_end(args);

	memcpy(buf, szTime, nAddLen);
	memcpy(buf+nAddLen+nRetVal, "\r\n\0", 3);
	nRetVal = Logs(buf);

	if(buf)	
		free(buf);
	return nRetVal;
}


int		NLog::TimedLogFile(CHAR *pLogFile, CONST CHAR * lpszFormat, ...)
{
	CHAR *			buf = NULL;
	int				nRetVal = -1, nSize = 4096;
	va_list			args;
	CHAR			szTime[1024];
	int				nAddLen;

	if(lpszFormat == NULL)
		return Logs(NULL, pLogFile);

	switch(s_TimeLogType)
	{
	case LT_TYPE_DAY:
		NStr::Day2String(szTime);
		break;
	case LT_TYPE_TIME:
		NStr::Time2String(szTime);
		break;
	case LT_TYPE_TIME_mS:
		NStr::Time2String(szTime, 128, TRUE);
		break;
	default:
		szTime[0] = 0;
	}
	strcat(szTime, " ");
	nAddLen = (int)strlen(szTime);


	va_start(args, lpszFormat);

	for(; nRetVal<0 ; nSize*=2)
	{
		if(buf)	
			free(buf);

		buf = (CHAR *)malloc( (nSize)*sizeof(CHAR) );
		nRetVal = _vsnprintf(buf+nAddLen, nSize-nAddLen-4, lpszFormat, args);
	}

	va_end(args);

	memcpy(buf, szTime, nAddLen);
	memcpy(buf+nAddLen+nRetVal, "\r\n\0", 3);
	nRetVal = Logs(buf, pLogFile);

	if(buf)	
		free(buf);
	return nRetVal;
}

int		NLog::FolderFileLog(CHAR *pLogFolder, CHAR *pLogFile, CONST CHAR * lpszFormat, ...)
{
	CHAR *				buf = NULL;
	int					nRetVal = -1, nSize = 4096;
	va_list				args;
	CHAR				szFileName[MAXPATH];
	CHAR				szTime[1024];
	CHAR				szDate[64];
	int					nAddLen;

	if(GetFileAttributesA(pLogFolder) == 0xFFFFFFFF)
		return 0;

	NStr::GetDate(szDate);
	sprintf(szFileName, "%s\\%s_%s", pLogFolder, szDate, pLogFile);

	if(lpszFormat == NULL)
		return Logs(NULL, szFileName);

	switch(s_TimeLogType)
	{
	case LT_TYPE_DAY:
		NStr::Day2String(szTime);
		break;
	case LT_TYPE_TIME:
		NStr::Time2String(szTime);
		break;
	case LT_TYPE_TIME_mS:
		NStr::Time2String(szTime, 128, TRUE);
		break;
	default:
		szTime[0] = 0;
	}
	strcat(szTime, " ");
	nAddLen = (int)strlen(szTime);


	va_start(args, lpszFormat);

	for(; nRetVal<0 ; nSize*=2)
	{
		if(buf)
			free(buf);
		buf = (CHAR *)malloc( (nSize)*sizeof(CHAR) );
		if(buf == NULL)
		{
			assert(0);
			return 0;
		}

		nRetVal = _vsnprintf(buf+nAddLen, nSize-nAddLen-4, lpszFormat, args);
	}

	va_end(args);

	memcpy(buf, szTime, nAddLen);
	memcpy(buf+nAddLen+nRetVal, "\r\n\0", 3);
	nRetVal = Logs(buf, szFileName);

	if(buf)	
		free(buf);
	return nRetVal;
}





int		NLog::LogMem(VOID *pMem, DWORD dwSize, BOOL bUnicode)
{
	CHAR *		pText;
	int			nRetVal;

	pText = NStr::MemToHexView(pMem, dwSize, (DWORD)pMem, bUnicode);
	nRetVal = Logs(pText);

	return	nRetVal;
}


int		NLog::DbgLog(CONST CHAR * lpszFormat, ...)
{
	CHAR *			buf = NULL;
	int				nRetVal = -1, nSize = 4096;
	va_list			args;

#ifndef _DEBUG
	return 0;
#endif // _DEBUG

	if(lpszFormat == NULL)
		return Logs(NULL, s_ThisDbgLog);

	va_start(args, lpszFormat);

	for(; nRetVal<0 ; nSize*=2)
	{
		if(buf)	
			free(buf);

		buf = (CHAR *)malloc( (nSize+2)*sizeof(CHAR) );
		nRetVal = _vsnprintf(buf, nSize, lpszFormat, args);
	}

	va_end(args);
	strcat(buf, "\r\n");

	nRetVal = Logs(buf, s_ThisDbgLog);

	if(buf)	
		free(buf);
	return nRetVal;
}



int		NLog::DbgLogMem(VOID *pMem, DWORD dwSize, BOOL bUnicode)
{
	_bstr_t				bStrText;

#ifndef _DEBUG
	return 0;
#endif // _DEBUG

	NStr::MemToHex(pMem, dwSize, &bStrText, (DWORD)pMem, bUnicode);
	return Logs(bStrText, s_ThisDbgLog);
}



int		NLog::DbgLogFile(CHAR *pLogFile, CONST CHAR * lpszFormat, ...)
{
	CHAR *				buf = NULL;
	int					nRetVal = -1, nSize = 4096;
	va_list				args;

#ifndef _DEBUG
	return 0;
#endif // _DEBUG


	if(lpszFormat == NULL)
		return Logs(NULL, pLogFile);

	va_start(args, lpszFormat);

	for(; nRetVal<0 ; nSize*=2)
	{
		if(buf)	
			free(buf);

		buf = (CHAR *)malloc( (nSize+2)*sizeof(CHAR) );
		nRetVal = _vsnprintf(buf, nSize, lpszFormat, args);
	}

	va_end(args);
	strcat(buf, "\r\n");

	nRetVal = Logs(buf, pLogFile);

	if(buf)	
		free(buf);
	return nRetVal;
}


int		NLog::DbgOutput(CONST CHAR * lpszFormat, ...)
{
	CHAR *			buf = NULL;
	int				nRetVal = -1, nSize = 4096;
	va_list			args;

#ifndef _DEBUG
	return 0;
#endif // _DEBUG

	if(lpszFormat == NULL)
		return Logs(NULL, s_ThisDbgLog);

	va_start(args, lpszFormat);

	for(; nRetVal<0 ; nSize*=2)
	{
		if(buf)	
			free(buf);

		buf = (CHAR *)malloc( (nSize+2)*sizeof(CHAR) );
		nRetVal = _vsnprintf(buf, nSize, lpszFormat, args);
	}

	va_end(args);
	strcat(buf, "\r\n");

	OutputDebugStringA(buf);

	if(buf)	
		free(buf);
	return nRetVal;
}


DWORD	NLog::LogTimeStart()
{
	s_dwTimer = GetTickCount();
	return s_dwTimer;
}


int		NLog::LogTime(CONST CHAR * lpszFormat, ...)
{
	CHAR *			buf = NULL;
	int				nRetVal = -1, nSize = 4096;
	va_list			args;
	DWORD			dwTimes;
	CHAR			szBuf[1024];

	dwTimes = GetTickCount() - s_dwTimer;

	if(lpszFormat == NULL)
		return Logs(NULL, s_ThisDbgLog);

	sprintf(szBuf, "Time: %6d\t", dwTimes);
	Logs(szBuf);

	va_start(args, lpszFormat);

	for(; nRetVal<0 ; nSize*=2)
	{
		if(buf)	
			free(buf);

		buf = (CHAR *)malloc( (nSize+sizeof(szBuf))*sizeof(CHAR) );
		nRetVal = _vsnprintf(buf, nSize, lpszFormat, args);
	}

	va_end(args);
	strcat(buf, "\r\n");

	nRetVal = Logs(buf);

	if(buf)	
		free(buf);
	return nRetVal;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//						NStr
/////////////////////////////////////////////////////////////////////////////////////////////////
static BYTE Char2Type_Table[256] =
{
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x03, 0xFF, 0x03, 0x03, 0x03, 0x03, 0xFF,	 0xFF, 0xFF, 0x03, 0x03, 0xFF, 0x03, 0x03, 0x03,
//        !     "     #     $     %     &     '      (     )     *     +     ,     -     .     /

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	 0x00, 0x00, 0x03, 0xFF, 0x03, 0x03, 0x03, 0x03,
//  0     1     2     3     4     5     6     7      8     9     :     ;     <     =     >     ?

	0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,	 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
//  @     A     B     C     D     E     F     G      H     I     J     K     L     M     N     O

	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,	 0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03,
//  P     Q     R     S     T     U     V     W      X     Y     Z     [     \     ]     ^     _

	0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,	 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
//  `     a     b     c     d     e     f     g      h     i     j     k     l     m     n     o

	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,	 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0xFF,
//  p     q     r     s     t     u     v     w      x     y     z     {     |     }     ~

	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

#define Char2Type_Number	0
#define Char2Type_AlphUpr	1
#define Char2Type_AlphLwr	2
#define Char2Type_Operat	3

BOOL	NStr::IsNumber(CHAR c)
{
	if(Char2Type_Table[(BYTE)c] == Char2Type_Number)
		return TRUE;
	return FALSE;
}

BOOL	NStr::IsAlph(CHAR c)
{
	if(Char2Type_Table[(BYTE)c] == Char2Type_AlphUpr ||
		Char2Type_Table[(BYTE)c] == Char2Type_AlphLwr)
		return TRUE;
	return FALSE;
}

BOOL	NStr::IsAlphUpr(CHAR c)
{
	if(Char2Type_Table[(BYTE)c] == Char2Type_AlphUpr)
		return TRUE;
	return FALSE;
}

BOOL	NStr::IsAlphLwr(CHAR c)
{
	if(Char2Type_Table[(BYTE)c] == Char2Type_AlphLwr)
		return TRUE;
	return FALSE;
}

BOOL	NStr::IsNumberAlph(CHAR c)
{
	if(Char2Type_Table[(BYTE)c] == Char2Type_Number ||
		Char2Type_Table[(BYTE)c] == Char2Type_AlphUpr ||
		Char2Type_Table[(BYTE)c] == Char2Type_AlphLwr)
		return TRUE;
	return FALSE;
}

BOOL	NStr::IsOperatChar(CHAR c)
{
	if(Char2Type_Table[(BYTE)c] == Char2Type_Operat)
		return TRUE;
	return FALSE;
}


CHAR	NStr::CharUpr(CHAR c)
{
	if(IsAlphLwr(c))
	{
		c = c - 'a' + 'A';
	}

	return c;
}

CHAR * NStr::StrInsert(CHAR *str, int p, CHAR *InsertStr)
{
	size_t	n;
	n = strlen(InsertStr);
	memmove(str+p+n, str+p, strlen(str)+1-p);	
	memcpy(str+p, InsertStr, strlen(InsertStr));
	return str;
}


CHAR * NStr::StrDel(CHAR *str, int p, int n)
{
	memmove(str+p,str+p+n,strlen(str)+1-p-n);
	return str;
}


int		NStr::StrReplace(CHAR *pString, CHAR cOld, CHAR cNew)
{
	int		nCount;

	for(nCount=0; *pString; pString++)
	{
		if(*pString == cOld)
		{
			*pString = cNew;
			nCount ++;
		}
	}

	return nCount;
}

int  NStr::StrReplace(CHAR *str, CHAR *scr_str, CHAR *dest_str)
{
	int n = 0;
	char *padd = NULL;
	char *pfind = NULL;
	size_t len1,len2,addn;
	len1 = strlen(scr_str);
	len2 = strlen(dest_str);
	pfind = ::strstr(str,scr_str);
	while(pfind != NULL)
	{
		if(len1 > len2)
		{
			StrDel(str,(int)(pfind-str),(int)(len1-len2));
		}
		else if(len1 < len2)
		{
			addn = len2-len1;
			padd = (char *)::malloc(addn+1);
			BREAKIF(padd == NULL);
			memset(padd, ' ',addn);
			padd[addn] = 0;
			StrInsert(str, (int)(pfind-str), padd);
			free(padd);
		}
		memcpy(pfind, dest_str, len2);
		n++;
		pfind = ::strstr(pfind+len2,scr_str);
	}
	return n;
}

int		NStr::StrIReplace(CHAR *str, CHAR *scr_str, CHAR *dest_str)
{
	int n = 0;
	char *padd = NULL;
	char *pfind = NULL;
	size_t len1,len2,addn;
	len1 = strlen(scr_str);
	len2 = strlen(dest_str);
	pfind = ::StrStrIA(str,scr_str);
	while(pfind != NULL)
	{
		if(len1 > len2)
		{
			StrDel(str,(int)(pfind-str),(int)(len1-len2));
		}
		else if(len1 < len2)
		{
			addn = len2-len1;
			padd = (char *)::malloc(addn+1);
			BREAKIF(padd == NULL);
			memset(padd, ' ',addn);
			padd[addn] = 0;
			StrInsert(str, (int)(pfind-str), padd);
			free(padd);
		}
		memcpy(pfind, dest_str, len2);
		n++;
		pfind = ::StrStrIA(pfind+len2,scr_str);
	}
	return n;
}


int		NStr::StrCopy(CHAR *pDes, CONST CHAR *pSor, int nDestSize)
{
	CHAR * cp;
	CHAR * pEnd;

	assert(pDes != NULL);

	if(nDestSize == -1)
	{
		for(cp = pDes;
			*pSor;
			cp++, pSor++)
		{
			*cp = *pSor;
		}
		*cp = 0;

		return (int)(cp-pDes);
	}


	pEnd = pDes + nDestSize - 1;

	for(cp = pDes;
		*pSor;
		cp++, pSor++)
	{
		if(cp >= pEnd)
			break;

		*cp = *pSor;
	}
	*cp = 0;

	return (int)(cp-pDes);
}


int		NStr::WcsCopy(WCHAR *pDes, CONST WCHAR *pSor, int nDestSize)
{
	WCHAR * cp;
	WCHAR * pEnd;

	assert(pDes != NULL);

	if(nDestSize == -1)
	{
		for(cp = pDes;
			*pSor;
			cp++, pSor++)
		{
			*cp = *pSor;
		}
		*cp = 0;

		return (int)(cp-pDes);
	}


	pEnd = pDes + nDestSize/sizeof(WCHAR) - 1;

	for(cp = pDes;
		*pSor;
		cp++, pSor++)
	{
		if(cp >= pEnd)
			break;

		*cp = *pSor;
	}
	*cp = 0;

	return (int)(cp-pDes);
}


BOOL	NStr::CharCmp(CHAR a, CHAR b, BOOL bNoCase)
{
	if(bNoCase == FALSE)
	{
		if(a == b)
			return TRUE;
		return FALSE;
	}

	CHAR		c;

	if(a == b)
		return TRUE;

	if(a > b)
	{
		c = b;
	}
	else
	{
		c = a;
		a = b;
	}

	if(c < 'A' || c > 'Z')
		return FALSE;

	if((a-c) != ('a'-'A'))
		return FALSE;

	return TRUE;
}

CHAR	* NStr::StrnCopy(CHAR *pDes, CONST CHAR *pSor, size_t nCount)
{
	CHAR			*pRetVal;

	if(pDes == NULL)
		return NULL;

	pRetVal = strncpy(pDes, pSor, nCount);
	pDes[nCount] = 0;

	return pRetVal;
}


int		NStr::StrHeadCmp(CHAR *pLongStr, CHAR *pStr, BOOL bNoCase)
{
	int			nSize;

	nSize = (int)strlen(pStr);

	if(bNoCase)
		return strnicmp(pLongStr, pStr, nSize);

	return strncmp(pLongStr, pStr, nSize);
}

int		NStr::StrHeadCmp(WCHAR *pLongStr, WCHAR *pStr, BOOL bNoCase)
{
	int			nSize;

	nSize = (int)wcslen(pStr);

	if(bNoCase)
		return wcsnicmp(pLongStr, pStr, nSize);

	return wcsncmp(pLongStr, pStr, nSize);
}

int		NStr::StrTailCmp(CHAR *pLongStr, CHAR *pStr, BOOL bNoCase)
{
	int			nNeedPos;
	int			nLen;

	nLen = (int)strlen(pStr);
	nNeedPos = (int)strlen(pLongStr) - nLen;

	if(nNeedPos < 0)
		nNeedPos = 0;

	if(bNoCase)
        return stricmp(pLongStr+nNeedPos, pStr);

	return strcmp(pLongStr+nNeedPos, pStr);
}


DWORD	NStr::MemReplaceBYTE(VOID * pMem, DWORD dwMemSize, BYTE oldval, BYTE newval)
{
	BYTE		* pByte;
	DWORD		i;

	pByte = (BYTE *)pMem;
	for(i=0; i<dwMemSize; i++)
	{
		if(pByte[i] == oldval)
			pByte[i] = newval;
	}

	return dwMemSize;
}

DWORD	NStr::MemReplaceWORD(void * Mem, DWORD dwMemSize, WORD oldval, WORD newval)	//Mem Use Aglin
{
	WORD		* pWord;
	DWORD		i;

	pWord = (WORD *)Mem;
	dwMemSize = dwMemSize / 2;
	for(i=0; i<dwMemSize; i++)
	{
		if(pWord[i] == oldval)
			pWord[i] = newval;
	}

	return dwMemSize;
}


BOOL	NStr::MemToStr(VOID * pMem, int nSize, _bstr_t * pbString, BOOL bUniCode)
{
	BYTE			* pBuf;

	pBuf = (BYTE *)malloc(nSize+2);

	memcpy(pBuf, pMem, nSize);
	memset(pBuf+nSize, 0, 2);

	if(bUniCode)
	{
		MemReplaceWORD(pBuf, nSize, 0x00, ' ');
		MemReplaceWORD(pBuf, nSize, 0x08, ' ');
		MemReplaceWORD(pBuf, nSize, 0x0A, ' ');
		MemReplaceWORD(pBuf, nSize, 0x0C, ' ');
		MemReplaceWORD(pBuf, nSize, 0x0D, ' ');

		*pbString = (WCHAR *)pBuf;
	}
	else
	{
		MemReplaceBYTE(pBuf, nSize, 0x00, ' ');
		MemReplaceBYTE(pBuf, nSize, 0x08, ' ');
		MemReplaceBYTE(pBuf, nSize, 0x0A, ' ');
		MemReplaceBYTE(pBuf, nSize, 0x0C, ' ');
		MemReplaceBYTE(pBuf, nSize, 0x0D, ' ');

		*pbString = (CHAR *)pBuf;
	}

	free(pBuf);
	return TRUE;
}


BOOL	NStr::MemToStr(VOID * pMem, int nSize, BOOL bUniCode, VOID *pBuffer)
{
	memcpy(pBuffer, pMem, nSize);

	if(bUniCode)
	{
		memset((BYTE *)pBuffer+nSize, 0, 2);
		MemReplaceWORD(pBuffer, nSize, 0x00, ' ');
		MemReplaceWORD(pBuffer, nSize, 0x08, ' ');
		MemReplaceWORD(pBuffer, nSize, 0x0A, ' ');
		MemReplaceWORD(pBuffer, nSize, 0x0C, ' ');
		MemReplaceWORD(pBuffer, nSize, 0x0D, ' ');

	}
	else
	{
		memset((BYTE *)pBuffer+nSize, 0, 1);
		MemReplaceBYTE(pBuffer, nSize, 0x00, ' ');
		MemReplaceBYTE(pBuffer, nSize, 0x08, ' ');
		MemReplaceBYTE(pBuffer, nSize, 0x0A, ' ');
		MemReplaceBYTE(pBuffer, nSize, 0x0C, ' ');
		MemReplaceBYTE(pBuffer, nSize, 0x0D, ' ');
	}

	return TRUE;
}


BOOL	NStr::MemToString(VOID *pMem, int nSize, std::string &sText, BOOL bUniCode)
{
	if(bUniCode == FALSE)
	{
		sText.clear();
		sText.insert(0, (const char *)pMem, nSize);
		sText.insert(nSize, 1, '\0');
	}
	else
	{
		std::wstring		wString;

		nSize /= sizeof(WCHAR);
		wString.insert(0, (const wchar_t *)pMem, nSize);
		wString.insert(nSize, 1, '\0');

		NStr::WChar2Char(wString, sText);
	}
	return TRUE;
}

BOOL	NStr::MemToHex(VOID * pMem, int nSize, _bstr_t * pbString, DWORD dwAddress, BOOL bUniCode)
{
	BYTE			* pDat;
	CHAR			buf[1024];
	CHAR			buf1[1024], buf2[1024];
	CHAR			szOneLine[1024];
	int				i;
	int				a, b, p;
	_bstr_t			bStrText;
	_bstr_t			bStrOne;

	pDat = (BYTE *)pMem;
	a = nSize/16;
	b = nSize%16;
	bStrText = "";

	for(i=0; i<a; i++)
	{
		p = i*16;
		memcpy(buf, pDat+p, 16);
		buf[16]=0;

		MemToStr(buf, 16, &bStrOne, bUniCode);

		sprintf(szOneLine, "%08X: %02X %02X %02X %02X %02X %02X %02X %02X-%02X %02X %02X %02X %02X %02X %02X %02X   %s\r\n",
			dwAddress,
			pDat[p+0],pDat[p+1],pDat[p+2],pDat[p+3],pDat[p+4],pDat[p+5],pDat[p+6],pDat[p+7],
			pDat[p+8],pDat[p+9],pDat[p+10],pDat[p+11],pDat[p+12],pDat[p+13],pDat[p+14],pDat[p+15],
			(CHAR *)bStrOne);

		dwAddress += 0x10;
		bStrText += szOneLine;
	}

	if(b == 0)
	{
		*pbString = bStrText;
		return TRUE;
	}

	sprintf(buf2, "%08X: ", dwAddress);
	for(i=0;i<b;i++)
	{
		p=a*16+i;
		if(i == 7)	
			sprintf(buf1, "%02X-", pDat[p]);
		else		
			sprintf(buf1, "%02X ", pDat[p]);
		strcat(buf2, buf1);
	}
	sprintf(buf1, "%*s", 50-b*3, " ");
	strcat(buf2, buf1);

	memset(buf, 0, 16);
	memcpy(buf, pDat+a*16, b);
	buf[b]=0;
	MemToStr(buf, 16, &bStrOne, bUniCode);

	bStrText += buf2;
	bStrText += bStrOne;
	bStrText += "\r\n";

	*pbString = bStrText;
	return TRUE;
}


int		StringAppend(CHAR *pBuffer, CHAR *pAppendStr, int *pHasLen, int nMaxLen = -1)
{
	int			nAddLen;

	nAddLen = (int)strlen(pAppendStr);

	if(nMaxLen != -1 &&
		*pHasLen + nAddLen >= nMaxLen)
	{
		*pHasLen = nMaxLen;
		return 0;
	}

	strcpy(pBuffer+*pHasLen, pAppendStr);
	*pHasLen += nAddLen;

	return nAddLen;
}


int	_MemToHexView(VOID * pMem, int nSize, CHAR* pBuffer)
{
	BYTE *			pData;
	int				nHasLen;
	int				i;
	CHAR			szOneByte[10];
	BYTE			bHi, bLow;

	nHasLen = 0;
	pData = (BYTE *)pMem;
	for(i=0; i<nSize; i++)
	{
		bHi = pData[i] >> 4;
		bLow = pData[i] & 0x0F;

		szOneByte[0] = HexTable[bHi];
		szOneByte[1] = HexTable[bLow];
		szOneByte[2] = ' ';
		szOneByte[3] = 0;

		StringAppend(pBuffer, szOneByte, &nHasLen);
	}

	if(nSize >= 8)
		pBuffer[8*3-1] = '-';

	return nSize;
}


CHAR *	NStr::MemToHexView(VOID * pMem, int nSize, DWORD dwAddress, BOOL bUniCode)
{
	BYTE		* pDat;
	BYTE		OneLineData[1024];
	CHAR		HexNub[1024], DatString[1024];
	CHAR		szOneLine[1024];
	int			i;
	int			a, b, p;
	CHAR		*pNewString;
	int			nHasLen;
	int			nAllocSize;

	nHasLen = 0;
	nAllocSize = nSize*5 + 80;
	pNewString = (CHAR *)malloc(nAllocSize);

	pDat = (BYTE *)pMem;
	a = nSize/16;
	b = nSize%16;

	for(i=0; i<a; i++)
	{
		p = i*16;
		memcpy(OneLineData, pDat+p, 16);
		OneLineData[16]=0;

		MemToStr(OneLineData, 16, bUniCode, DatString);
		if(bUniCode)
			NStr::WChar2Char((WCHAR *)DatString, DatString);

		_MemToHexView(pDat+p, 16, HexNub);

		sprintf(szOneLine, "%08X: %s  %s\r\n", dwAddress, HexNub, DatString);

		dwAddress += 0x10;
		StringAppend(pNewString, szOneLine, &nHasLen, nAllocSize);
	}

	if(b == 0)
	{
		return pNewString;
	}

	sprintf(szOneLine, "%08X: ", dwAddress);

	_MemToHexView(pDat+p, b, HexNub);
	for(i=b*3; i<48; i++)
		HexNub[i] = ' ';
	HexNub[i] = 0;

	memcpy(OneLineData, pDat+p, b);
	OneLineData[b]=0;

	MemToStr(OneLineData, 16, bUniCode, DatString);
	if(bUniCode)
		NStr::WChar2Char((WCHAR *)DatString, DatString);

	sprintf(szOneLine, "%08X: %s  %s\r\n", dwAddress, HexNub, DatString);
	StringAppend(pNewString, szOneLine, &nHasLen, nAllocSize);

	return pNewString;
}


CHAR *	NStr::MallocByString(CHAR *pString)
{
	size_t		dwSize;
	CHAR *		pNewString;

	if(pString == NULL)
		return NULL;

	dwSize = strlen(pString) + 1;
	pNewString = (CHAR *)malloc(dwSize);
	memcpy(pNewString, pString, dwSize);

	return pNewString;
}

VOID	WINAPIV NStr::FreeString(CHAR *pString)
{
	free(pString);
	return;
}


CHAR	*NStr::StrFormatStr(CHAR *pString, CHAR *pFormat)	//´ø?µÄ²éÕÒ
{
	if(strchr(pFormat, '?') == NULL)
		return strstr(pString, pFormat);

	char *cp = (char *) pString;
	char *s1, *format;

	if ( !*pFormat )
		return((char *)pString);

	while (*cp)
	{
		s1 = cp;
		format = (char *) pFormat;

		while (	*s1 &&
				*format && 
				(!(*s1-*format) || *format == '?')
			  )
			s1++, format++;

		if (!*format)
			return(cp);

		cp++;
	}

	return(NULL);
}


typedef	struct _MATCH_DATA
{
	CHAR *	pData1;
	int		nDatLen1;
	CHAR *	pData2;
	int		nDatLen2;

	BOOL	bMatch;
	int		nFirtFindPos;
}MATCH_DATA;

typedef BOOL (WINAPI *MATCH_COMPARE)(CHAR a, CHAR b, BOOL *pMuliti);

BOOL WINAPI	DFA_MatchCaseProc(CHAR a, CHAR b, BOOL *pMuliti)
{
	if(b == '*')
	{
		*pMuliti = TRUE;
		return TRUE;
	}
	else
		*pMuliti = FALSE;

	if(b == '?')
		return TRUE;

	if(b == a)
		return TRUE;

	return FALSE;
}

BOOL WINAPI	DFA_MatchNoCaseProc(CHAR a, CHAR b, BOOL *pMuliti)
{
	if(b == '*')
	{
		*pMuliti = TRUE;
		return TRUE;
	}
	else
		*pMuliti = FALSE;

	if(b == '?')
		return TRUE;

	if(b == a)
		return TRUE;

	CHAR		c;

	if(a > b)
	{
		c = b;
	}
	else
	{
		c = a;
		a = b;
	}

	if(c < 'A' || c > 'Z')
		return FALSE;

	if((a-c) != ('a'-'A'))
		return FALSE;

	return TRUE;
}


BOOL	DfaMatch(MATCH_DATA *pData, MATCH_COMPARE fnCompare = DFA_MatchCaseProc)
{
	int			i, j;
	BOOL		bCompare;
	BOOL		bMulti;
	BOOL		bValue;
	int			nFirstNoMulti;
	BYTE **		ppTreeMap;
	CMiniMem	Mem;

	CHAR *		pData1 = pData->pData1;
	CHAR *		pData2 = pData->pData2;

	nFirstNoMulti = -1;
	pData->nFirtFindPos = -1;

	if(pData->nDatLen1 <= 0)
		pData->nDatLen1 = (int)strlen(pData->pData1);
	if(pData->nDatLen2 <= 0)
		pData->nDatLen2 = (int)strlen(pData->pData2);

	ppTreeMap = (BYTE **)Mem.Alloc2dArry(pData->nDatLen1+1, pData->nDatLen2+1);
	ppTreeMap[0][0] = TRUE;

	bCompare = fnCompare(pData1[0], pData2[0], &bMulti);
	if(bMulti)
		ppTreeMap[0][1] = TRUE;


	for(i=0; pData1[i]; i++)
	{
		for(j=0; pData2[j]; j++)
		{
			bCompare = fnCompare(pData1[i], pData2[j], &bMulti);
			if(bCompare == FALSE)
				continue;

			bValue = ppTreeMap[i][j];
			if(bValue == FALSE)
			{
				if(bMulti == FALSE)
					continue;

				bValue = ppTreeMap[i][j+1];			//	Tree.Get(MAKE_ULONG64(i-1, j), FALSE);		// *Í¨Åä¶à¸ö
				if(bValue == FALSE)
					bValue = ppTreeMap[i+1][j];		//	Tree.Get(MAKE_ULONG64(i, j-1), FALSE);	// *Í¨Åä¿Õ

				if(bValue == FALSE)
					continue;
			}

			ppTreeMap[i+1][j+1] = TRUE;				//	Tree.Set(MAKE_ULONG64(i, j), TRUE);
			if(bMulti == FALSE && nFirstNoMulti == -1)
				nFirstNoMulti = j;
		}
	}

	pData->bMatch = ppTreeMap[i][j];

	//ÕÒµ½µÚÒ»¸ö·¢ÏÖµÄÎ»ÖÃ

	if(pData->bMatch == FALSE)
		return TRUE;


	//	ÕÒµ½Í·
	j = pData->nDatLen2-1;
	for(i=0; i<pData->nDatLen1; i++)
	{
		bValue = ppTreeMap[i][j];
		if(bValue)
			break;
	}

	//	ÖØ¸´µ½µÚÒ»¸ö·Ç*
	for(; i>0 && j>0; )
	{
		bValue = ppTreeMap[i-1][j-1];
		if(bValue)
		{
			i --;
			j --;

			if(j == nFirstNoMulti)
			{
				pData->nFirtFindPos = i;
				break;
			}
			continue;
		}

		bValue = ppTreeMap[i-1][j];		//	*Í¨Åä¶à¸ö
		if(bValue)
		{
			i --;
			continue;
		}

		bValue = ppTreeMap[i][j-1];		//	*Í¨Åä¿Õ
		if(bValue)
		{
			j --;
			continue;
		}

		break;
	}

	return TRUE;
}


BOOL	NStr::WildcardCompare(CHAR *pString, CHAR *pWildcard, BOOL bNoCase)
{
	MATCH_DATA				Data;

	Data.pData1 = pString;
	Data.pData2 = pWildcard;

	if(bNoCase == FALSE)
		DfaMatch(&Data);
	else
		DfaMatch(&Data, DFA_MatchNoCaseProc);

	return Data.bMatch;
}


CHAR *	NStr::WildcardFind(CHAR *pString, CHAR *pWildcard, BOOL bNoCase)
{
	CHAR			szNewWildText[1024];
	MATCH_DATA		Data;

	if(strlen(pWildcard)+3 > sizeof(szNewWildText))
	{
		assert(0);			//±í´ïÊ½Ì«³¤
		return NULL;
	}

	strcpy(szNewWildText, "*");
	strcat(szNewWildText, pWildcard);
	strcat(szNewWildText, "*");

	Data.pData1 = pString;
	Data.pData2 = szNewWildText;

	if(bNoCase == FALSE)
		DfaMatch(&Data);
	else
		DfaMatch(&Data, DFA_MatchNoCaseProc);

	if(Data.bMatch == FALSE)
		return NULL;

	if(Data.nFirtFindPos <= 0)
        assert(0);

	return pString+Data.nFirtFindPos;
}


int		NStr::StrTrimRight(CHAR *pString, CHAR *pTrim)
{
	int			nLen = (int)strlen(pString);
	CHAR		* pEnd;
	CHAR		* pStart;

	pStart = pString;
	pEnd = pString+nLen;

	for(pString=pEnd-1; pString>pStart; pString--)
	{
		if(strchr(pTrim, *pString))
			*pString = 0;
		else
			break;
	}

	return (int)(pEnd-pString-1);
}


int		NStr::StrTrimRightRN(CHAR *pString)
{
	int			nLen;

	if(pString == NULL)
		return 0;

	nLen = (int)strlen(pString);
	if(pString[nLen-2] == '\r' && pString[nLen-2] == '\n')
	{
		pString[nLen-2] = 0;
		return 2;
	}
	else if(pString[nLen-1] == '\r')
	{
		pString[nLen-1] = 0;
		return 1;
	}
	else if(pString[nLen-1] == '\n')
	{
		pString[nLen-1] = 0;
		return 1;
	}

	return 0;
}


int		NStr::StrTrimTwoSide(CHAR *pString, CHAR *pTrim)
{
	int			nLen = (int)strlen(pString);
	CHAR		* pEnd;
	CHAR		* pStart;
	CHAR		* pRet;

	pStart = pString;
	pEnd = pString+nLen;
	pRet = pString;

	for(pString=pStart; pString<pEnd; pString++)
	{
		if(strchr(pTrim, *pString))
			pRet ++;
		else
			break;
	}

	for(pString=pEnd-1; pString>pStart; pString--)
	{
		if(strchr(pTrim, *pString))
			*pString = 0;
		else
			break;
	}

	strcpy(pStart, pRet);
	return TRUE;
}


CHAR *	NStr::StrChr(CHAR *pString, CHAR *pFind)
{
	int			i;

	for(i=0; pString[i]; i++)
	{
		if(strchr(pFind, pString[i]))
			return pString + i;
	}

	return NULL;
}


CHAR *	NStr::StrStr(CHAR *pString, CHAR *pFind, BOOL bNoCase)
{
	if(bNoCase)
		return StrFindNoCase(pString, pFind);
	else
		return strstr(pString, pFind);
}

int		NStr::StrCompare(CHAR *pString1, CHAR *pString2, BOOL bNoCase)
{
	if(bNoCase)
		return stricmp(pString1, pString2);
	else
		return strcmp(pString1, pString2);
}

int		NStr::GetLine(CHAR ** ppLastPos, CHAR * pBuffer, CHAR * pOneLine, int nLineMaxSize)	
{
	static  CHAR	* pCur = NULL;
	int				i = 0;
	int				nRetVal;

	if(ppLastPos == NULL)
		ppLastPos = &pCur;


	if(pOneLine == NULL)		//³õÊ¼»¯
	{
		(*ppLastPos) = pBuffer;
		return 1;
	}

	if((*ppLastPos) == NULL)			//Ã»ÓÐ³õÊ¼»¯
		return 0;

	if((*ppLastPos)[0] == 0)
		return 0;

	nRetVal = 0;
	for(i=0; ; i++)
	{
		if(((*ppLastPos))[i] == 0x0D && (*ppLastPos)[i+1] == 0x0A)
		{
			(*ppLastPos) += i+2;
			nRetVal = 1;
			break;
		}

		if((*ppLastPos)[i] == 0x0D)
		{
			(*ppLastPos) += i+1;
			nRetVal = 1;
			break;
		}
		if((*ppLastPos)[i] == 0x0A)
		{
			(*ppLastPos) += i+1;
			nRetVal = 1;
			break;
		}
		if((*ppLastPos)[i] == 0)
		{
			(*ppLastPos) += i;
			nRetVal = 2;
			break;
		}

		if(i < nLineMaxSize)
			pOneLine[i] = (*ppLastPos)[i];
	}

	if(i < nLineMaxSize)
		pOneLine[i] = 0;
	else
		pOneLine[nLineMaxSize-1] = 0;

	return nRetVal;
}



DWORD	NStr::StrtoFormatArry(CHAR *str, CHAR * Arry, DWORD ArryCount, DWORD ArrySize, BOOL bFormat)
{
	CHAR *	pArry[1024];
	DWORD	len,i,j;
	DWORD	stringf = 0, Arryn = 0;
	char	c;
	if(ArryCount >= 1024)
		return 0;
	for(i=0;i<ArryCount;i++)
	{
		pArry[i] = Arry + i*ArrySize;
		pArry[i][0] = 0;
	}
	len = (DWORD) strlen(str);
	for(i=0, j=0; i<len ;i++)
	{
		if(str[i] == ' ' || str[i] == 0x09)	//½áÊø
		{
			if(stringf == 0)
			{
				if(j == 0)	
					continue;
				pArry[Arryn][j] = 0;
				Arryn++;
				if(Arryn >= ArryCount)
				{
					return Arryn;
				}
				j = 0;
			}
			else
			{
				pArry[Arryn][j] = str[i];
				j++;
				if(j >= ArrySize)
				{
					j--;
					pArry[Arryn][j] = 0;
				}
			}
		}
		else if(str[i] == '"')			//×Ö´®
		{

			if(stringf == 0)
			{
				if(j == 0)		stringf = 1;
				else					//ÖÐ¼äµÄ["]×÷Îª×Ö·û´¦Àí
				{
					pArry[Arryn][j] = str[i];
					j++;
					if(j >= ArrySize)
					{
						j--;
						pArry[Arryn][j] = 0;
					}
				}
			}
			else				stringf = 0;
		}
		else if(bFormat && str[i] == '\\')			//×ªÒå×Ö·û
		{
			if     (str[i+1] == '\\')	c = '\\';
			else if(str[i+1] == 't')	c = 0x09;
			else if(str[i+1] == 'r')	c = 0x0D;
			else if(str[i+1] == 'n')	c = 0x0A;
			else						c = str[i+1];
			pArry[Arryn][j] = c;
			i++;
			j++;
			if(j >= ArrySize)
			{
				j--;
				pArry[Arryn][j] = 0;
			}
		}
		else
		{
			pArry[Arryn][j] = str[i];
			j++;
			if(j >= ArrySize)
			{
				j--;
				pArry[Arryn][j] = 0;
			}
		}
	}
	pArry[Arryn][j] = 0;
	return Arryn+1;
}


inline	BOOL	StringAddChar(LPSTR & pString, int &nStringLen, int nMaxLen, CHAR c)
{
	if(nMaxLen - nStringLen <= 1)
		return FALSE;

	pString[nStringLen] = c;
	nStringLen ++;

	return TRUE;
}


//	pSplitChars = NULL  ->   '\0'ÎªItem½áÊø, "\0\0"Îª´®½áÊø
//	ÒÔ pSplitChars µÄ×Ö·ûÎª·Ö¸îºÅ, ·Ö¸î³É¶à¸ö×Ö·û´® 
int		NStr::SplitString(CHAR *pString, CHAR * pItems, int nItemCount, int nItemSize, CHAR *pSplitChars)
{
	int				i;
	int				nCount;
	int				nOneItemLen;
	CHAR *			pFind;
	CHAR *			pOneItem;

	if(pSplitChars == NULL)
		return MultiString2Arrys(pString, pItems, nItemCount, nItemSize);

	nCount = 0;
	nOneItemLen = 0;
	pOneItem = pItems;
	for(i=0; pString[i]; i++)
	{
		pFind = strchr(pSplitChars, pString[i]);
		if(pFind == NULL)
		{
			StringAddChar(pOneItem, nOneItemLen, nItemSize, pString[i]);
			continue;
		}
		pOneItem[nOneItemLen] = 0;

		nCount ++;
		if(nCount >= nItemCount)
			return nCount;

		pOneItem = pItems+nCount*nItemSize;
		nOneItemLen = 0;
	}
	pOneItem[nOneItemLen] = 0;


	return nCount+1;
}


int		NStr::SplitString(CHAR *pString, std::vector<std::string> &strList, CHAR *pSplitChars)
{
	std::string			sOneItem;
	int					i;

	if(pSplitChars == NULL)
		return MultiString2Arrys(pString, strList);

	for(i=0; pString[i]; i++)
	{
		if(strchr(pSplitChars, pString[i]) == NULL)
		{
			sOneItem += pString[i];
			continue;
		}

		strList.push_back(sOneItem);
		sOneItem = "";
	}
	strList.push_back(sOneItem);

	return (int)strList.size();
}


//	³¤×Ö·û´®»ñÈ¡Ò»¶Î, pSplitÎª·Ö¸î×Ö·û¼¯, µ±pBuffer=NULLÊ±³õÊ¼»¯
CHAR *	NStr::GetOnePart(CHAR ** ppLastPos, CHAR *pString, CHAR *pBuffer, int nBufSize, CHAR *pSplit)
{
	static	CHAR *	pCurPos = NULL;
	CHAR *		pFind;
	CHAR *		pRetVal;
	size_t		nFindPos;
	size_t		nRestLen;

	if(ppLastPos == NULL)
		ppLastPos = &pCurPos;

	if(pBuffer == NULL)
	{
		(*ppLastPos) = pString;
		return (*ppLastPos);
	}

	if((*ppLastPos) == NULL)		//Íê³É, Ã»³õÊ¼»¯
		return NULL;

	nRestLen = strlen((*ppLastPos));
	nFindPos = strcspn((*ppLastPos), pSplit);
	if(nFindPos >= nRestLen)	//Ã»ÓÐÕÒµ½
	{
		NStr::StrCopy(pBuffer, (*ppLastPos), nBufSize);
		pRetVal = (*ppLastPos);
		(*ppLastPos) = NULL;
		return pRetVal;
	}

	pFind = (*ppLastPos) + nFindPos;
	if((int)nFindPos >= nBufSize)
	{
		nFindPos = nBufSize - 1;
	}

	memcpy(pBuffer, (*ppLastPos), nFindPos);
	pBuffer[nFindPos] = 0;

	pRetVal = (*ppLastPos);

	//	¼ÆËãºóÃæµÄÎ»ÖÃ
	nRestLen = strlen(pFind);
	nFindPos = strspn(pFind, pSplit);
	if(nFindPos >= nRestLen)	//È«ÊÇ·Ö¸î×Ö·û
	{
		(*ppLastPos) = NULL;
	}
	else
	{
		(*ppLastPos) = pFind + nFindPos;
	}

	return pRetVal;
}

BOOL	NStr::CopyToClip(CHAR *pString, DWORD dwLen)
{
	HGLOBAL		hData;
	BYTE *		pData;

	if(dwLen == 0)
        dwLen =(DWORD)strlen(pString) + 1;

	if(OpenClipboard(NULL) == FALSE)
		return FALSE;

	if (EmptyClipboard() == FALSE)
		return FALSE;

	hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, dwLen);
	if (hData == NULL)
		return FALSE;

	pData = (BYTE *)GlobalLock(hData);
	memcpy(pData, pString, dwLen+1);
	GlobalUnlock(pData);
	SetClipboardData(CF_TEXT, hData);

	CloseClipboard();
	return TRUE;
}


BOOL	NStr::CopyToClip(WCHAR *pString, DWORD dwLen)
{
	HGLOBAL		hData;
	BYTE *		pData;

	if(dwLen == 0)
		dwLen =(DWORD)((wcslen(pString)+1) * sizeof(WCHAR));

	if(OpenClipboard(NULL) == FALSE)
		return FALSE;

	if (EmptyClipboard() == FALSE)
		return FALSE;

	hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, dwLen);
	if (hData == NULL)
		return FALSE;

	pData = (BYTE *)GlobalLock(hData);
	memcpy(pData, pString, dwLen+1);
	GlobalUnlock(pData);
	SetClipboardData(CF_UNICODETEXT, hData);

	CloseClipboard();
	return TRUE;
}


BOOL	NStr::GetClipString(std::string &sText)
{
	HANDLE		hString;

	if(OpenClipboard(NULL) == FALSE)
		return FALSE;

	hString = GetClipboardData(CF_TEXT);
	if(hString == NULL)
	{
		sText = "";
		return FALSE;
	}
	sText = (CHAR *)hString;

	CloseClipboard();
	return TRUE;
}



//	"\n"»ò"\r"  »»³É "\r\n"
BOOL	NStr::FormatMulitiLine(CHAR *pString, CHAR *pNewString, int nBufSize, BOOL bDelEmpty)
{
	CHAR			szOneLine[4096];
	int				nLen;
	int				nRtnLen;
	CHAR *			pLastSave;

	nRtnLen = (int)strlen("\r\n");
	
	for(NStr::GetLine(&pLastSave, pString);
		NStr::GetLine(&pLastSave, pString, szOneLine, sizeof(szOneLine));
		)
	{
		nLen = (int)strlen(szOneLine);
		if(nLen == 0 && bDelEmpty)
			continue;

		nBufSize -= nLen;
		if(nBufSize <= 0)
			return FALSE;

		strcpy(pNewString, szOneLine);
		pNewString += nLen;

		nBufSize -= nRtnLen;
		if(nBufSize <= 0)
			return FALSE;

		strcpy(pNewString, "\r\n");
		pNewString += nRtnLen;
	}

	return TRUE;
}

BOOL	NStr::GetFormatedTime(CHAR *pString, FILETIME& ft, BOOL bShowMS)
{
	SYSTEMTIME			st;
	BOOL				bRetVal;

	bRetVal = FileTimeToSystemTime(&ft, &st);
	if(bRetVal == FALSE)
	{
		pString[0] = 0;
		return FALSE;
	}

	return GetFormatedTime(pString, st, bShowMS);
}

BOOL	NStr::GetFormatedTime(CHAR *pString, SYSTEMTIME& st, BOOL bShowMS)
{
	if(bShowMS)
	{
		sprintf(pString, "%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth,
			st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	}
	else
	{
		sprintf(pString, "%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth,
			st.wDay, st.wHour, st.wMinute, st.wSecond);
	}

	return TRUE;
}


BOOL	NStr::MultiString2MultiLine(CHAR *pSor, CHAR *pDest)
{
	int				i, j;

	for(i=0, j=0; ;i++)
	{
		if(pSor[i] != 0)
		{
			pDest[j] = pSor[i];
			j ++;
			continue;
		}

		if(pSor[i+1] == 0)
			break;

		pDest[j] = '\r';
		j ++;
		pDest[j] = '\n';
		j ++;
	}
	pDest[j] = 0;

	return TRUE;
}


int		NStr::MultiString2Arrys(CHAR *pString, CHAR *pItems, int nItemCount, int nItemSize)
{
	int				i, j;
	int				nItems;
	CHAR *			pDest;

	nItems = 0;
	pDest = pItems;
	for(i=0, j=0; ;i++)
	{
		if(pString[i] != 0)
		{
			if(j < nItemSize-1)
			{
				pDest[j] = pString[i];
				j ++;
			}
			continue;
		}
		pDest[j] = 0;

		if(pString[i+1] == 0)
			break;

		nItems ++;
		pDest = pItems + nItemSize * nItems;
		j = 0;

		if(nItems >= nItemCount)
			break;
	}

	return nItems;
}


int		NStr::MultiString2Arrys(CHAR *pString, std::vector<std::string> &strList)
{
	std::string		sOneItem;

	for(;;)
	{
		sOneItem = pString;
		strList.push_back(sOneItem);

		pString += strlen(pString) + 1;
		if(pString[0] == 0)
			break;
	}

	return (int)sOneItem.size();
}


int		NStr::Vector2MultiText(std::vector<std::string> &vStrList, std::string &sMultiText, CHAR *pSplit)
{
	std::vector<std::string>::iterator		Iter;

	sMultiText = "";
	for(Iter = vStrList.begin();
		Iter != vStrList.end();
		Iter ++)
	{
		sMultiText += *Iter;
		sMultiText += pSplit;
	}

	return TRUE;
}

int		NStr::TChar2Char(TCHAR *pSor, CHAR *pDest, int nDestLen)
{
#ifndef UNICODE
#ifndef _UNICODE
	return StrCopy(pDest, pSor, nDestLen);
#endif
#endif

	return WChar2Char((WCHAR *)pSor, pDest, nDestLen);
}


int		NStr::TChar2WChar(TCHAR *pSor, WCHAR *pDest, int nDestLen)
{
#ifndef UNICODE
#ifndef _UNICODE
	return Char2WChar((CHAR *)pSor, pDest, nDestLen);
#endif
#endif

	return WcsCopy(pDest, (WCHAR *)pSor, nDestLen);
}


int		NStr::Char2TChar(CHAR *pSor, TCHAR *pDest, int nDestLen)
{
#ifndef UNICODE
#ifndef _UNICODE
	return StrCopy((CHAR *)pDest, pSor, nDestLen);
#endif
#endif

	return Char2WChar(pSor, (WCHAR *)pDest, nDestLen);
}

int		NStr::Char2WChar(CHAR *pSor, WCHAR *pDest, int nDestLen)
{
	int		nRetVal;

	if(nDestLen == -1)
		nDestLen = MultiByteToWideChar( CP_ACP, 0, pSor, -1, NULL, 0);

	nRetVal = MultiByteToWideChar( CP_ACP, 0, pSor, -1, pDest, nDestLen);
	return nRetVal;
}


int		NStr::Char2WChar(CHAR *pSor, std::wstring &wString)
{
	int		nRetVal;
	int		nDestLen;

	wString.clear();
	nDestLen = MultiByteToWideChar( CP_ACP, 0, pSor, -1, NULL, 0);
	nDestLen *= 2;

	wString.resize(nDestLen);

	nRetVal = MultiByteToWideChar( CP_ACP, 0, pSor, -1, &*wString.begin(), nDestLen);
	return nRetVal;
}

int		NStr::WChar2TChar(WCHAR *pSor, TCHAR *pDest, int nDestLen)
{
#ifndef UNICODE
#ifndef _UNICODE
	return WChar2Char(pSor, (CHAR *)pDest, nDestLen);
#endif
#endif

	return WcsCopy((WCHAR *)pDest, pSor, nDestLen);
}

int		NStr::WChar2Char(WCHAR *pSor, CHAR *pDest, int nDestLen)
{
	int		nRetVal;

	if(nDestLen == -1)
		nDestLen = ::WideCharToMultiByte(CP_ACP, 0, pSor, -1, NULL, 0, NULL, NULL);

	nRetVal = ::WideCharToMultiByte(CP_ACP, 0, pSor, -1, pDest, nDestLen, NULL, NULL);

	return nRetVal;
}


int		NStr::WChar2Char(std::wstring &wString, std::string &sText)
{
	int		nRetVal;
	int		nDestLen;

	nDestLen = ::WideCharToMultiByte(CP_ACP, 0, wString.c_str(), -1, NULL, 0, NULL, NULL);
	sText.resize(nDestLen);

	nRetVal = ::WideCharToMultiByte(CP_ACP, 0, wString.c_str(), -1, &*sText.begin(), nDestLen, NULL, NULL);

	return nRetVal;
}


int		NStr::WChar2Char(WCHAR *pSor, std::string &sText)
{
	int		nRetVal;
	int		nDestLen;

	nDestLen = ::WideCharToMultiByte(CP_ACP, 0, pSor, -1, NULL, 0, NULL, NULL);
	sText.resize(nDestLen);

	nRetVal = ::WideCharToMultiByte(CP_ACP, 0, pSor, -1, &*sText.begin(), nDestLen, NULL, NULL);

	return nRetVal;
}

VOID	NStr::String2Time(CHAR *pString, SYSTEMTIME *pTime)
{
	memset(pTime, 0, sizeof(SYSTEMTIME));

	sscanf(pString, "%04d-%02d-%02d %02d:%02d:%02d.%03d", &pTime->wYear, &pTime->wMonth,
		&pTime->wDay, &pTime->wHour, &pTime->wMinute, &pTime->wSecond, &pTime->wMilliseconds);

	return;
}


CHAR *	NStr::Time2String(CHAR *pString, size_t nSize, BOOL bMs, SYSTEMTIME *pTime)
{
	SYSTEMTIME	LocalTime;

	if(pTime == NULL)
	{
		GetLocalTime(&LocalTime);
		pTime = &LocalTime;
	}

	if(bMs)
	{
		_snprintf(pString, nSize, "%04d-%02d-%02d %02d:%02d:%02d.%03d", pTime->wYear, pTime->wMonth,
			pTime->wDay, pTime->wHour, pTime->wMinute, pTime->wSecond, pTime->wMilliseconds);
	}
	else
	{
		_snprintf(pString, nSize, "%04d-%02d-%02d %02d:%02d:%02d", pTime->wYear, pTime->wMonth,
			pTime->wDay, pTime->wHour, pTime->wMinute, pTime->wSecond);
	}

	return pString;
}


CHAR *	NStr::Day2String(CHAR *pString, size_t nSize, SYSTEMTIME *pTime)
{
	SYSTEMTIME	LocalTime;

	if(pTime == NULL)
	{
		GetLocalTime(&LocalTime);
		pTime = &LocalTime;
	}

	_snprintf(pString, nSize, "%04d-%02d-%02d", 
		pTime->wYear, pTime->wMonth, pTime->wDay);

	return pString;
}


CHAR *	NStr::GetDateTime(CHAR *pString, SYSTEMTIME *pTime, BOOL bMs)
{
	SYSTEMTIME	LocalTime;

	if(pTime == NULL)
	{
		GetLocalTime(&LocalTime);
		pTime = &LocalTime;
	}

	if(bMs)
	{
		sprintf(pString, "%04d-%02d-%02d %02d:%02d:%02d.%03d", pTime->wYear, pTime->wMonth,
			pTime->wDay, pTime->wHour, pTime->wMinute, pTime->wSecond, pTime->wMilliseconds);
	}
	else
	{
		sprintf(pString, "%04d-%02d-%02d %02d:%02d:%02d", pTime->wYear, pTime->wMonth,
			pTime->wDay, pTime->wHour, pTime->wMinute, pTime->wSecond);
	}

	return pString;
}


CHAR *	NStr::GetDateTime(CHAR *pString, FILETIME &ft, BOOL bMs)
{
	SYSTEMTIME	st;
	FILETIME	lcft;
	CHAR *		pRetVal;

	FileTimeToLocalFileTime(&ft, &lcft);
	FileTimeToSystemTime(&lcft, &st);

	pRetVal = GetDateTime(pString, &st, bMs);

	return pRetVal;
}


CHAR *	NStr::GetDateTime(CHAR *pString, time_t t)
{
	tm *		pTm;

	pTm = localtime(&t);
	sprintf(pString, "%04d-%02d-%02d %02d:%02d:%02d", 
		pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday,
		pTm->tm_hour, pTm->tm_min, pTm->tm_sec);

	return pString;
}


WCHAR *	NStr::GetDateTime(WCHAR *pString, SYSTEMTIME *pTime, BOOL bMs)
{
	SYSTEMTIME	LocalTime;

	if(pTime == NULL)
	{
		GetLocalTime(&LocalTime);
		pTime = &LocalTime;
	}

	if(bMs)
	{
		swprintf(pString, L"%04d-%02d-%02d %02d:%02d:%02d.%03d", pTime->wYear, pTime->wMonth,
			pTime->wDay, pTime->wHour, pTime->wMinute, pTime->wSecond, pTime->wMilliseconds);
	}
	else
	{
		swprintf(pString, L"%04d-%02d-%02d %02d:%02d:%02d", pTime->wYear, pTime->wMonth,
			pTime->wDay, pTime->wHour, pTime->wMinute, pTime->wSecond);
	}

	return pString;
}


WCHAR *	NStr::GetDateTime(WCHAR *pString, FILETIME &ft, BOOL bMs)
{
	SYSTEMTIME	st;
	FILETIME	lcft;
	WCHAR *		pRetVal;

	FileTimeToLocalFileTime(&ft, &lcft);
	FileTimeToSystemTime(&lcft, &st);

	pRetVal = GetDateTime(pString, &st, bMs);

	return pRetVal;
}


WCHAR *	NStr::GetDateTime(WCHAR *pString, time_t t)
{
	tm *		pTm;

	pTm = localtime(&t);
	swprintf(pString, L"%04d-%02d-%02d %02d:%02d:%02d", 
		pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday,
		pTm->tm_hour, pTm->tm_min, pTm->tm_sec);

	return pString;
}


CHAR *	NStr::GetDate(CHAR *pString, SYSTEMTIME *pTime)
{
	SYSTEMTIME	LocalTime;

	if(pTime == NULL)
	{
		GetLocalTime(&LocalTime);
		pTime = &LocalTime;
	}

	sprintf(pString, "%04d-%02d-%02d", 
		pTime->wYear, pTime->wMonth, pTime->wDay);

	return pString;
}

CHAR *	NStr::GetTime(CHAR *pString, BOOL bMs, SYSTEMTIME *pTime)
{
	SYSTEMTIME	LocalTime;

	if(pTime == NULL)
	{
		GetLocalTime(&LocalTime);
		pTime = &LocalTime;
	}

	if(bMs)
	{
		sprintf(pString, "%02d:%02d:%02d.%03d", 
			pTime->wHour, pTime->wMinute, pTime->wSecond, pTime->wMilliseconds);
	}
	else
	{
		sprintf(pString, "%02d:%02d:%02d", 
			pTime->wHour, pTime->wMinute, pTime->wSecond);
	}

	return pString;
}


CHAR *	NStr::GetTime(CHAR *pString, FILETIME &ft, BOOL bMs)
{
	SYSTEMTIME	st;
	FILETIME	lcft;
	CHAR *		pRetVal;

	FileTimeToLocalFileTime(&ft, &lcft);
	FileTimeToSystemTime(&lcft, &st);

	pRetVal = GetTime(pString, bMs, &st);

	return pRetVal;
}

CHAR *	NStr::GetTime(CHAR *pString, __time64_t tm64, BOOL bLocalConvrt)
{
	tm *			ptm;

	if(bLocalConvrt)
        ptm = _localtime64(&tm64);
	else
		ptm = _gmtime64(&tm64);

	if(ptm == NULL)
	{
		pString = 0;
		return pString;
	}

	strftime(pString, 128, "%H:%M:%S", ptm);
	return pString;
}


CHAR *	NStr::TickCount2Second(DWORD dwTime, CHAR *pBuffer)
{
	static CHAR			szTimeText[256];

	if(pBuffer == NULL)
		pBuffer = szTimeText;

	sprintf(pBuffer, "%d.%03d", dwTime/1000, dwTime%1000);
	return pBuffer;
}


DWORD	NStr::Str2Dowrd(CHAR *pString)
{
	DWORD		dwValue;

	if(StrHeadCmp(pString, "0x") == 0)
	{
		dwValue = strtoul(pString+2, NULL, 16);
	}
	else if(StrTailCmp(pString, "h") == 0)
	{
		dwValue = strtoul(pString, NULL, 16);
	}
	else
	{
		dwValue = strtoul(pString, NULL, 10);
	}

	return dwValue;
}



BOOL	NStr::Big5ToGbk(CHAR *pBig, CHAR *pGbk, int nBufSize)
{
	BOOL	bRetVal;
	UINT	uPageBig5;
	UINT	uPageGbk;

	uPageBig5 = 950;
	uPageGbk = 936;
	bRetVal = CodePageConvert(uPageBig5, uPageGbk, pBig, pGbk, nBufSize);

	return bRetVal;
}


BOOL	NStr::CodePageConvert(UINT uInCodePage, UINT uOutCodePage, CHAR *pIn, CHAR *pOut, int nOutSize)
{
	int			nNeedLen;
	WCHAR *		pTempBuf;
	int			nChars;
	BOOL		bRetVal;

	nNeedLen = (int)strlen(pIn) + 1;
	pTempBuf = (WCHAR *) malloc(sizeof(WCHAR) * nNeedLen);

	nChars = MultiByteToWideChar(uInCodePage, 0, pIn, nNeedLen, pTempBuf, nNeedLen + 1);

	if(nOutSize == -1)
		nOutSize = nNeedLen;

	nChars = WideCharToMultiByte(uOutCodePage, 0, pTempBuf, nChars, pOut, nOutSize, "?", &bRetVal);
	free(pTempBuf);

	if(nChars <= 0 || bRetVal)
		return FALSE;

	return TRUE;
}


BOOL	NStr::CreateRandomString(CHAR *pBuffer, int nSize, CHAR *pSeed)
{
	int		i;
	int		nLen;
	static	DWORD	dwThreadId = 0;

	if(dwThreadId != GetCurrentThreadId())
	{
		srand(GetTickCount());
		dwThreadId = GetCurrentThreadId();
	}

	nLen = (int)strlen(pSeed);
	nSize --;

	for(i=0; i<nSize; i++)
	{
		pBuffer[i] = pSeed[rand()%nLen];
	}

	pBuffer[nSize] = 0;
	return TRUE;
}


int		NStr::ConvertValBySeedString(CHAR c, CHAR *pSeed)
{
	CHAR *		pFind;

	pFind = strchr(pSeed, c);
	if(pFind == NULL)
		return 0;

	return (int)(pFind-pSeed);
}


int		NStr::ConvertValBySeedString(CHAR *pBuffer, DWORD &dwValue, CHAR *pSeed)
{
	int			nRadio;
	int			i;
	int			nOneVal;

	dwValue = 0;
	nRadio = (int)strlen(pSeed);
	for(i=0; pBuffer[i]; i++)
	{
		dwValue *= nRadio;
		nOneVal = ConvertValBySeedString(pBuffer[i]);
		dwValue += nOneVal;
	}

	return i;
}


int		NStr::ConvertValBySeedString(CHAR *pBuffer, ULONGLONG &uuValue, CHAR *pSeed)
{
	int			nRadio;
	int			i;
	int			nOneVal;

	uuValue = 0;
	nRadio = (int)strlen(pSeed);
	for(i=0; pBuffer[i]; i++)
	{
		uuValue *= nRadio;
		nOneVal = ConvertValBySeedString(pBuffer[i]);
		uuValue += nOneVal;
	}

	return i;
}

//	LOOP_FIND_PROC ·µ»Ø TRUE ¼ÌÐø, ·µ»ØFALSEÍ£Ö¹²éÕÒ
DWORD	NStr::LoopFindNoCaseString(LOOP_FIND_PROC LoopProc, CHAR *pBuffer, CHAR *pSearch, VOID *pParam)
{
	CHAR *		pFind;
	CHAR *		pNextPos;
	int			nAddLen;
	BOOL		bRetVal;
	DWORD		dwCount;
	CHAR *		pUprString;

	nAddLen = (int)strlen(pSearch);

	pUprString = (CHAR *)malloc(nAddLen+1);
	strcpy(pUprString, pSearch);
	strupr(pUprString);

	dwCount = 0;

	for(pNextPos = pBuffer;
		pFind = StrFindNoCase(pNextPos, pUprString);
		pNextPos = pFind + nAddLen)
	{
		bRetVal = LoopProc(pFind, pParam);
		dwCount ++;
		if(bRetVal == FALSE)
			break;
	}

	free(pUprString);
	return dwCount;
}


CHAR *	NStr::StrFindNoCase(CHAR *pString, CHAR *pUprStr)
{
	CHAR *cp = pString;
	CHAR *s1, *s2;

	if ( !*pUprStr )
		return(pString);

	while (*cp)
	{
		s1 = cp;
		s2 = pUprStr;

		while ( *s1 && *s2 && 
			UprCharNoCaseEqu2(*s2, *s1) )
			s1++, s2++;

		if (!*s2)
			return(cp);

		cp++;
	}

	return NULL;
}

#define CodeString_K		2

//	CÓïÑÔÐÎÊ½µÄ×Ö·û´®×ª»»
CHAR *	NStr::CodeString2String(CHAR *pCodeString, CHAR *pBuffer, int * pnCount)
{
	int		nLen;
	int		nAlcLen;
	CHAR *	pNewString;
	int		nCount;

	nLen = (int)strlen(pCodeString);

	if(pBuffer == NULL)
	{
		nAlcLen = (nLen+1) * CodeString_K;
		pNewString = (CHAR *)malloc(nAlcLen);
		pBuffer = pNewString;
		DbgMemset(pBuffer, 0, nAlcLen);
	}
	else
	{
		pNewString = pBuffer;
	}

	nCount = 0;
	for(; *pCodeString; pCodeString++)
	{
		if(*pCodeString != '\\')
		{
			*pNewString = *pCodeString;
			pNewString ++;
			nCount ++;
			continue;
		}

		pCodeString ++;
		if(pCodeString[0] == 0)
			break;

		switch(*pCodeString)
		{
		case 't':
		case 'T':
			*pNewString = '\t';
			pNewString ++;
			nCount ++;
			break;
		case 'r':
		case 'R':
			*pNewString = '\r';
			pNewString ++;
			nCount ++;
			break;
		case 'n':
		case 'N':
			*pNewString = '\n';
			pNewString ++;
			nCount ++;
			break;

		default:
			if(NData::IsHexChar(*pCodeString) == FALSE)
			{
				*pNewString = *pCodeString;
				pNewString ++;
				nCount ++;
				break;
			}

			*pNewString = (CHAR)NData::Hex2Byte(pCodeString[0], pCodeString[1]);
			pNewString ++;
			nCount ++;
			pCodeString ++;
		}
	}

	*pNewString = 0;
	if(pnCount)
		*pnCount = nCount;
	return pBuffer;
}

//	×ª»»³ÉCÓïÑÔÐÎÊ½µÄ×Ö·û´®
CHAR *	NStr::String2CodeString(CHAR *pString, CHAR *pBuffer)
{
	int		nLen;
	int		nAlcLen;
	CHAR *	pNewString;

	nLen = (int)strlen(pString);

	if(pBuffer == NULL)
	{
		nAlcLen = (nLen+1) * CodeString_K;
		pNewString = (CHAR *)malloc(nAlcLen);
		pBuffer = pNewString;
		DbgMemset(pBuffer, 0, nAlcLen);
	}
	else
	{
		pNewString = pBuffer;
	}

	for(; *pString; pString++)
	{
		switch(*pString)
		{
		case '\\':
			*pNewString = '\\';
			pNewString ++;
			*pNewString = '\\';
			pNewString ++;
			break;
		case '\"':
			*pNewString = '\\';
			pNewString ++;
			*pNewString = '\"';
			pNewString ++;
			break;
		case '\r':
			*pNewString = '\\';
			pNewString ++;
			*pNewString = 'r';
			pNewString ++;
			break;
		case '\n':
			*pNewString = '\\';
			pNewString ++;
			*pNewString = 'n';
			pNewString ++;
			break;
		case '\t':
			*pNewString = '\\';
			pNewString ++;
			*pNewString = 't';
			pNewString ++;
			break;

		default:
			*pNewString = *pString;
			pNewString ++;
		}
	}

	*pNewString = 0;
	return pBuffer;
}


CHAR *	NStr::Mem2CodeArry(VOID *pMem, DWORD dwMemSize, std::string &sText, int nOneLineCount)
{
	DWORD			i;
	BYTE			bHi, bLow;
	BYTE *			pData;
	int				nFindSize;
	CHAR			szOne[64];
	int				nHasLen;


	nFindSize = (dwMemSize / nOneLineCount + 1) * (nOneLineCount + 2) * 6;
	sText.clear();
	sText.resize(nFindSize);
	nHasLen = 0;

	pData = (BYTE *)pMem;
	for(i=0; i<dwMemSize; i++)
	{
		bHi = pData[i] >> 4;
		bLow = pData[i] & 0x0F;

		szOne[0] = '0';
		szOne[1] = 'x';
		szOne[2] = HexTable[bHi];
		szOne[3] = HexTable[bLow];
		szOne[4] = ',';
		szOne[5] = ' ';
		szOne[6] = 0;

		if(((i+1) % nOneLineCount) == 0)
			strcat(szOne, "\r\n");

		StringAppend(&sText[0], szOne, &nHasLen);
	}

	return (CHAR *)sText.c_str();
}

BOOL	IdaLineStr2Mem(CHAR *pIdaString, ByteMem &Mem)
{
	CHAR *		pCur;
	CHAR *		pNextPos;
	BYTE		c;

	Mem.clear();
	pCur = strstr(pIdaString, " db ");
	if(pCur == NULL)
		return FALSE;

	pCur += 4;
	for(;
		pCur && pCur[0];
		)
	{
		if(pCur[0] == ' ' ||
			pCur[0] == ',')
		{
			pCur ++;
			continue;
		}

		if(pCur[0] == '\'')
		{
			pNextPos = strchr(pCur+1, '\'');
			if(pNextPos == NULL)
				return FALSE;

			pCur = pNextPos;
			Mem.append((BYTE *)pCur+1, pNextPos-pCur-1);
		}
		else
		{
			c = (BYTE)strtoul(pCur, NULL, 16);
			Mem.append(&c, 1);
			pCur = strchr(pCur, ',');
		}
	}

	return TRUE;
}

BOOL	NStr::IdaString2Mem(CHAR *pIdaString, std::vector<ByteMem> &MemList)
{
	CHAR			szOneLine[4096];
	CHAR *			pLastPos;
	BOOL			bRetVal;
	ByteMem			OneMem;

	for(GetLine(&pLastPos, pIdaString);
		bRetVal = GetLine(&pLastPos, pIdaString, szOneLine, sizeof(szOneLine));
		)
	{
		IdaLineStr2Mem(szOneLine, OneMem);

		MemList.push_back(OneMem);
	}
	return TRUE;
}


BOOL	NStr::IdaString2Mem(CHAR *pIdaString, ByteMem &Mem)
{
	CHAR			szOneLine[4096];
	CHAR *			pLastPos;
	BOOL			bRetVal;
	ByteMem			OneMem;

	Mem.clear();
	for(GetLine(&pLastPos, pIdaString);
		bRetVal = GetLine(&pLastPos, pIdaString, szOneLine, sizeof(szOneLine));
		)
	{
		IdaLineStr2Mem(szOneLine, OneMem);
		Mem.append(OneMem);
	}
	return TRUE;
}

BOOL	NStr::IdaString2CodeString(CHAR *pIdaString, std::string &sText)
{
	CHAR			szOneLine[4096];
	BOOL			bRetVal;
	CHAR			*pCur;
	CHAR			*pNextPos;
	CHAR			szOneSection[4096];
	CHAR			szOneCLine[4096];
	DWORD			dwLines;
	CHAR *			pLastPos;

	sText = "";
	dwLines = 0;
	for(GetLine(&pLastPos, pIdaString);
		bRetVal = GetLine(&pLastPos, pIdaString, szOneLine, sizeof(szOneLine));
		)
	{
		pCur = strstr(szOneLine, " db ");
		if(pCur == NULL)
			continue;

		pCur += 4;
		if(dwLines != 0)
			sText += "\r\n";

		dwLines ++;
		sText += "\"";

		for(pNextPos = pCur;
			*pCur;
			)
		{
			if(pCur[0] == '\'')
			{
				pNextPos = strchr(pCur+1, '\'');
				if(pNextPos == NULL)
					return FALSE;

				NStr::StrnCopy(szOneSection, pCur+1, pNextPos-pCur-1);
			}
			else
			{
				szOneSection[0] = (CHAR)(BYTE)strtoul(pCur, NULL, 16);
				szOneSection[1] = 0;

				pNextPos = strchr(pCur, ',');
			}

			String2CodeString(szOneSection, szOneCLine);
			sText += szOneCLine;

			if(pNextPos == NULL)
				break;
			pCur = pNextPos + 1;
		}
		sText += "\"";
	}

	return TRUE;
}


BOOL	NStr::HtmString2String(CHAR *pHtmStr, std::string &sText)
{
	int				i;
	char			ch, cl;
	BYTE			c;

	sText.clear();
	for(i=0; pHtmStr[i]; i++)
	{
		if(pHtmStr[i] != '%')
		{
			sText += pHtmStr[i];
			continue;
		}
		i ++;
		ch = pHtmStr[i];
		i ++;
		cl = pHtmStr[i];

		if(ch == 0 || cl == 0)
			break;

		c = NData::Hex2Byte(ch, cl);
		sText += (char) c;
	}
	return TRUE;
}

int		NStr::GetGroupText(CHAR *pMutiKey, std::vector<std::string> &sTextList, CHAR cHead, CHAR cTail)
{
	CHAR *		pFindHead;
	CHAR *		pFindTail;
	int			nCopyLen;
	int			i;
	CHAR		szOneText[4096];

	sTextList.clear();

	pFindHead = NULL;
	for(i=0; pMutiKey[i]; i++)
	{
		if(pMutiKey[i] == cHead)
		{
			if(pFindHead == NULL)
				pFindHead = pMutiKey+i;

			continue;
		}

		if(pMutiKey[i] == cTail)
		{
			if(pFindHead == NULL)
			{
				assert(0);
				continue;
			}

			pFindTail = pMutiKey+i;
			nCopyLen = (int)(pFindTail - pFindHead - 1);
			memcpy(szOneText, pFindHead+1, nCopyLen);
			pFindHead = NULL;
			szOneText[nCopyLen] = 0;

			sTextList.push_back(szOneText);
		}
	}

	return (int)sTextList.size();
}


CHAR *	NStr::Value2StringThud(int nValue, CHAR *pBuffer)
{
	CHAR			szBuffer[64];
	static	CHAR	szStore[64];
	int				i;
	int				nLen;

	nLen = 0;
	szBuffer[0] = 0;
	for(i=0; ; i++)
	{
		szBuffer[nLen] = (nValue % 10) + '0';
		nLen ++;

		nValue /= 10;
		if(nValue == 0)
			break;

		if((i%3) == 2)
		{
			szBuffer[nLen] = ',';
			nLen ++;
		}
	}
	szBuffer[nLen] = 0;

	strrev(szBuffer);
	if(pBuffer == NULL)
		pBuffer = szStore;

	strcpy(pBuffer, szBuffer);
	return pBuffer;
}


WCHAR *	NStr::Value2StringThud(int nValue, WCHAR *pBuffer)
{
	static	WCHAR	szStore[64];
	CHAR			szBuffer[64];

	Value2StringThud(nValue, szBuffer);
	if(pBuffer == NULL)
		pBuffer = szStore;

	NStr::Char2WChar(szBuffer, pBuffer, sizeof(szStore));
	return pBuffer;
}


BOOL	NStr::CheckStrBufferOver(CHAR *pBuffer, int nBufSize)
{
	int			i;

	for(i=0; i<nBufSize; i++)
	{
		if(pBuffer[i] == 0)
			return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//						NSys
/////////////////////////////////////////////////////////////////////////////////////////////////


BOOL	NSys::LoopSleep(DWORD dwSleepSecond)
{
	DWORD		i;

	for(i=0; (i<dwSleepSecond) || (dwSleepSecond != -1); i++)
	{
		Sleep(100);
	}

	return TRUE;
}


BOOL	NSys::EnablePrivilege(TCHAR * name, BOOL fEnable, HANDLE hToken)
{
	BOOL				bRetVal;
	int					nError;
	HANDLE				hNewToken = NULL;
	TOKEN_PRIVILEGES	priv = { 1, {0, 0, fEnable ? SE_PRIVILEGE_ENABLED : 0} };

	LookupPrivilegeValue(0, name, &priv.Privileges[0].Luid);
	if(hToken == NULL)
		OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hNewToken);
	else
		hNewToken = hToken;

	bRetVal = AdjustTokenPrivileges(hNewToken, FALSE, &priv, sizeof(priv), 0, 0);
	if(bRetVal == FALSE)
	{
		nError = ::GetLastError();
		if(hToken == NULL)	
			CloseHandle(hNewToken);

		::SetLastError(nError);
		return FALSE;
	}

	if(hToken == NULL)	
		CloseHandle(hNewToken);
	return TRUE;
}


BOOL	NSys::EnableProcPrivilege(DWORD PID)
{
	HANDLE					hProc, hToken; 
	BOOL					bRetVal;

	bRetVal = EnablePrivilege(SE_DEBUG_NAME);
	if(bRetVal == FALSE)
		return FALSE;

	if(PID == 0)
		return TRUE;

	hProc = ::OpenProcess(PROCESS_ALL_ACCESS, TRUE, PID);
	if(hProc == NULL)
		return FALSE;

	bRetVal = OpenProcessToken(hProc, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	if(bRetVal == FALSE)
	{
		CloseHandle(hProc);
		return FALSE;
	}

	bRetVal = EnablePrivilege(SE_DEBUG_NAME, TRUE, hToken);
	CloseHandle(hToken);
	CloseHandle(hProc);
	if(bRetVal == FALSE)
		return FALSE;

	return TRUE;
}


BOOL	NSys::AppMultiRunCheck(CHAR * pMutexName)
{
	static HANDLE	hMutex = NULL;

	if(pMutexName == NULL)
	{
		if(hMutex)
		{
			CloseHandle(hMutex);
			hMutex = NULL;
		}
		return TRUE;
	}

	hMutex = ::CreateMutexA(NULL, FALSE, pMutexName);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		return FALSE;
	}

	return TRUE;
}

BOOL	NSys::SetWorkDir(CHAR *pProDir)
{
	CHAR			szWorkDir[MAX_PATH*2];
	BOOL			bRetVal;

	bRetVal = GetProcWorkDir(szWorkDir);
	if(bRetVal == FALSE)
		return FALSE;

	SetCurrentDirectoryA(szWorkDir);
	if(pProDir)
		strcpy(pProDir, szWorkDir);

	return TRUE;
}


BOOL	NSys::GetProcWorkDir(CHAR *pWorkDir)
{
	CHAR			szModFileName[MAX_PATH*2];
	DWORD			dwRetVal;
	CHAR			*pFind;

	dwRetVal = GetModuleFileNameA(NULL, szModFileName, sizeof(szModFileName));
	if(dwRetVal == 0 || dwRetVal >= sizeof(szModFileName))
		return FALSE;

	pFind = strrchr(szModFileName, '\\');
	if(pFind == NULL)
		return FALSE;

	pFind[0] = 0;
	if(pWorkDir)
		strcpy(pWorkDir, szModFileName);

	return TRUE;
}

VOID	NSys::IdleSleep(DWORD dwTime, HWND hWnd)
{
	MSG		msg;
	DWORD	Tstart, Time;
	BOOL	bRetVal;

	Tstart = GetTickCount();
	for(;;)
	{
		bRetVal = PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE);
		if(bRetVal)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Time = GetTickCount();
			if(Time - Tstart >= dwTime)
				return;
			Sleep(1);
		}
	}
}


VOID	NSys::DoMessageLoop(HWND hWnd)
{
	MSG			msg;
	BOOL		bRetVal;

	for(; IsWindow(hWnd);)
	{
		bRetVal = PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE);
		if(bRetVal == FALSE)
		{
			Sleep(1);
			continue;
		}

		if(msg.message == WM_QUIT)
			break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return;
}


FARPROC	NSys::GetDllProcAdders(CHAR *pDllName, CHAR *pProcName)
{
	HMODULE			hMod;
	FARPROC			fun;

	hMod = GetModuleHandleA(pDllName);
	if(hMod == NULL)
	{
		hMod = LoadLibraryA(pDllName);
		if(hMod == NULL)
			return NULL;
	}

	fun = GetProcAddress(hMod, pProcName);

	return fun;
}


BOOL	NSys::Execute(CHAR *pExeFile, CHAR *pCmdLine, BOOL bWait, BOOL bHide, DWORD *pExitCode)
{
	CHAR					szCommandLine[1024];
	STARTUPINFOA			si;
	PROCESS_INFORMATION		pi;
	BOOL					bRetVal;

	memset(&si, 0, sizeof(si));

	if(pCmdLine)
	{
		if(pExeFile == NULL)
		{
			strcpy(szCommandLine, pCmdLine);
		}
		else
		{
			if(pExeFile[0] == '"')
				sprintf(szCommandLine, "%s %s", pExeFile, pCmdLine);
			else
				sprintf(szCommandLine, "\"%s\" %s", pExeFile, pCmdLine);
		}
	}
	else
	{
		strcpy(szCommandLine, pExeFile);
	}


	si.cb = sizeof(si);

	if(bHide)
	{
		si.wShowWindow = SW_HIDE;
		si.dwFlags |= STARTF_USESHOWWINDOW;
	}

	bRetVal = CreateProcessA(NULL, szCommandLine, NULL, NULL, 
		FALSE, 0, NULL, NULL, &si, &pi);
	if(bRetVal == FALSE)
		return FALSE;

	if(bWait)
		WaitForSingleObject(pi.hProcess, INFINITE);

	if(pExitCode)
	{
		*pExitCode = -1;
        bRetVal = GetExitCodeProcess(pi.hProcess, pExitCode);
		if(bRetVal == FALSE)
		{
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			return FALSE;
		}
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return TRUE;
}



BOOL	NSys::ExecuteBk(CHAR *pCmdLine, DWORD *pExitCode, DWORD dwTmLimt)
{
	STARTUPINFOA			si;
	PROCESS_INFORMATION		pi;
	BOOL					bRetVal;
	DWORD					dwRetVal;
	BOOL					bResult;

	bResult = FALSE;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags |= STARTF_USESHOWWINDOW;

	*pExitCode = -1;
	bRetVal = CreateProcessA(NULL, pCmdLine, NULL, NULL, 
		FALSE, 0, NULL, NULL, &si, &pi);
	if(bRetVal == FALSE)
		return FALSE;

	dwRetVal = WaitForSingleObject(pi.hProcess, dwTmLimt);
	if(dwRetVal == WAIT_FAILED || 
		dwRetVal == WAIT_TIMEOUT)
	{
		::TerminateProcess(pi.hProcess, ERROR_PROCESS_ABORTED);
		*pExitCode = ERROR_PROCESS_ABORTED;
		goto ExecuteBk_End;
	}

	bRetVal = GetExitCodeProcess(pi.hProcess, pExitCode);
	if(bRetVal == FALSE)
		goto ExecuteBk_End;

	bResult = TRUE;

ExecuteBk_End:
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return bResult;
}

HANDLE	NSys::RunProcess(CHAR *pExeFile, CHAR *pCmdLine, STARTUPINFOA *pSi, PROCESS_INFORMATION *pPi)
{
	CHAR				szCommandLine[1024];
	STARTUPINFOA		si;
	PROCESS_INFORMATION	pi;
	BOOL				bRetVal;

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);

	if(pCmdLine)
	{
		if(pExeFile == NULL)
		{
			strcpy(szCommandLine, pCmdLine);
		}
		else
		{
			if(pExeFile[0] == '"')
				sprintf(szCommandLine, "%s %s", pExeFile, pCmdLine);
			else
				sprintf(szCommandLine, "\"%s\" %s", pExeFile, pCmdLine);
		}
	}
	else
	{
		strcpy(szCommandLine, pExeFile);
	}

	if(pSi == NULL)
		pSi = &si;
	if(pPi == NULL)
		pPi = &pi;

	bRetVal = CreateProcessA(NULL, szCommandLine, NULL, NULL, 
		FALSE, 0, NULL, NULL, pSi, pPi);
	if(bRetVal == FALSE)
		return NULL;

	if(pPi == &pi)
	{
		CloseHandle(pi.hThread);
	}
   
	return pPi->hProcess;
}


BOOL	NSys::RunBat(CHAR *pBatString, BOOL bHide, BOOL bWait, BOOL bDelSelf)
{
	CHAR		szBatFile[MAXPATH];
	BOOL		bRetVal;
	CHAR		szDelSelf[1024];


	NFile::GetATempFile(szBatFile, ".bat");
	NFile::DumpToFile(szBatFile, pBatString);

	if(bDelSelf && bWait == FALSE)
	{
		sprintf(szDelSelf, "\r\ndel \"%s\"\r\n", szBatFile);
		NFile::DumpAppend(szBatFile, szDelSelf);
	}

	bRetVal = Execute(szBatFile, NULL, bWait, bHide);

	if(bDelSelf && bWait)
        DeleteFileA(szBatFile);

	return bRetVal;
}


BOOL	NSys::CreateAndStartService(CHAR *pDrvPath, CHAR *pServiceName, DWORD dwStartType,LPCSTR lpLoadOrderGroup,BOOL bStart)
{
	SC_HANDLE			hManager;
	SC_HANDLE			hService;
	BOOL				bRetVal;
	int					nError;
	CHAR				szWorkDir[MAX_PATH];
	CHAR				szFullPath[MAX_PATH];
	BOOL				bResult;

	if(pDrvPath[1] != ':')
	{
		GetProcWorkDir(szWorkDir);
		sprintf(szFullPath, "\"%s\\%s\"", szWorkDir, pDrvPath);
		pDrvPath = szFullPath;
	}

	hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hManager == NULL)
		return FALSE;

	hService = CreateServiceA(hManager, pServiceName, pServiceName, SERVICE_ALL_ACCESS, 
		SERVICE_KERNEL_DRIVER, dwStartType, SERVICE_ERROR_NORMAL, 
		pDrvPath, lpLoadOrderGroup, NULL, NULL, NULL, NULL);
	if(hService != NULL)
	{
		CloseServiceHandle(hService);
		CloseServiceHandle(hManager);

		if(bStart)
		{
			bRetVal = StartUpService(pServiceName);
			if(bRetVal == FALSE)
				nError = GetLastError();
		}

		return TRUE;
	}
	CloseServiceHandle(hManager);

	nError = GetLastError();
	if(nError != ERROR_SERVICE_EXISTS)
		return FALSE;


	DWORD		dwOldStartType;

	bResult = TRUE;

	bRetVal = NSys::ChangeServerDriverImgPath(pServiceName, pDrvPath);
	if(bRetVal == FALSE)
	{
		bResult = FALSE;
		nError = GetLastError();
	}

	dwOldStartType = NSys::GetServcieStartType(pServiceName);
	if(dwOldStartType != dwStartType)
	{
		bRetVal = NSys::SetServcieStartType(pServiceName, dwStartType);
		if(bRetVal == FALSE)
		{
			bResult = FALSE;
			nError = GetLastError();
		}
	}

	if(bResult == FALSE)
		SetLastError(nError);

	if(bStart)
	{
		bRetVal = StartUpService(pServiceName);
		if(bRetVal == FALSE)
			nError = GetLastError();
	}

	return bResult;
}


BOOL	NSys::CreateService(CHAR *pFilePath, CHAR *pServiceName, 
					  DWORD dwServiceType,
					  DWORD dwStartType,
					  CHAR *pDisplayName)
{
	CHAR				szFullPath[MAX_PATH];
	SC_HANDLE			hManager;
	SC_HANDLE			hService;
	int					nError;
	BOOL				bRetVal;
	BOOL				bResult;

	if(pFilePath[1] != ':')
	{
		NFile::GetFileInModulePath(szFullPath, pFilePath);
		pFilePath = szFullPath;
	}

	if(pDisplayName == NULL)
		pDisplayName = pServiceName;

	hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hManager == NULL)
		return FALSE;

	hService = CreateServiceA(hManager, pServiceName, pDisplayName, SERVICE_ALL_ACCESS, 
		dwServiceType, dwStartType, SERVICE_ERROR_NORMAL, 
		pFilePath, NULL, NULL, NULL, NULL, NULL);
	if(hService != NULL)
	{
		CloseServiceHandle(hService);
		CloseServiceHandle(hManager);
		return TRUE;
	}

	CloseServiceHandle(hManager);

	nError = GetLastError();
	if(nError != ERROR_SERVICE_EXISTS)
		return FALSE;


	DWORD		dwOldStartType;
	DWORD		dwOldType;

	bResult = TRUE;

	bRetVal = NSys::ChangeServerImgPath(pServiceName, pFilePath);
	if(bRetVal == FALSE)
	{
		bResult = FALSE;
		nError = GetLastError();
	}

	dwOldStartType = NSys::GetServcieStartType(pServiceName);
	if(dwOldStartType != dwStartType)
	{
		bRetVal = NSys::SetServcieStartType(pServiceName, dwStartType);
		if(bRetVal == FALSE)
		{
			bResult = FALSE;
			nError = GetLastError();
		}
	}

	dwOldType = NSys::GetServcieType(pServiceName);
	if(dwOldType != dwServiceType)
	{
		bRetVal = NSys::SetServcieType(pServiceName, dwServiceType);
		if(bRetVal == FALSE)
		{
			bResult = FALSE;
			nError = GetLastError();
		}
	}

	if(pDisplayName)
	{
		bRetVal = NSys::ChangeServerDisplayName(pServiceName, pDisplayName);
		if(bRetVal == FALSE)
		{
			bResult = FALSE;
			nError = GetLastError();
		}
	}

	if(bResult == FALSE)
		SetLastError(nError);

	return bResult;
}


//	pdwStatus: SERVICE_STOP_PENDING, ...
BOOL	NSys::GetServiceStatus(CHAR *pServiceName, DWORD *pdwStatus)
{
	SC_HANDLE			hManager;
	SC_HANDLE			hService;
	SERVICE_STATUS		ServiceStatus;
	BOOL				bRetVal;
	int					nError;

	bRetVal = FALSE;
	hManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (hManager == NULL)
		goto GetServiceStatusDone;

	hService = OpenServiceA(hManager, pServiceName, SERVICE_QUERY_STATUS);
	if (hService == NULL)
		goto GetServiceStatusDone;

	bRetVal = QueryServiceStatus(hService, &ServiceStatus);
	if(bRetVal == FALSE)
		goto GetServiceStatusDone;

	*pdwStatus = ServiceStatus.dwCurrentState;
	bRetVal = TRUE;

GetServiceStatusDone:

	if(bRetVal == FALSE)
		nError = GetLastError();

	if(hService)
		CloseServiceHandle(hService);
	if(hManager)
		CloseServiceHandle(hManager);
	if(bRetVal == FALSE)
		SetLastError(nError);

	return bRetVal;

}


BOOL	NSys::CheckServerExist(CHAR *pServiceName)
{
	SC_HANDLE			hManager;
	SC_HANDLE			hService;
	int					nError;
	BOOL				bResult;

	hManager = NULL;
	hService = NULL;
	bResult = FALSE;
	nError = ERROR_SUCCESS;

	hManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (hManager == NULL)
		goto CheckServerExistDone;

	hService = OpenServiceA(hManager, pServiceName, SERVICE_QUERY_STATUS);
	if (hService == NULL)
		goto CheckServerExistDone;

	bResult = TRUE;

CheckServerExistDone:
	if(bResult == FALSE)
		nError = GetLastError();
	if(hService)
		CloseServiceHandle(hService);
	if(hManager)
		CloseServiceHandle(hManager);
	if(bResult == FALSE)
		SetLastError(nError);

	return bResult;
}

BOOL	NSys::StartUpService(CHAR *pServiceName, DWORD dwWait)
{
	SC_HANDLE			hManager;
	SC_HANDLE			hService;
	SERVICE_STATUS		ServiceStatus;
	BOOL				bRetVal;
	int					nError;
	DWORD				dwTime;
	DWORD				dwEndTime;
	BOOL				bSuccess;

	hManager = NULL;
	hService = NULL;
	bSuccess = FALSE;
	nError = ERROR_SUCCESS;

	hManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (hManager == NULL)
		goto StartUpServiceDone;

	hService = OpenServiceA(hManager, pServiceName, SERVICE_START | SERVICE_QUERY_STATUS);
	if (hService == NULL)
		goto StartUpServiceDone;

	bRetVal = QueryServiceStatus(hService, &ServiceStatus);
	if(bRetVal == FALSE)
		goto StartUpServiceDone;

	if(ServiceStatus.dwCurrentState == SERVICE_RUNNING)
	{
		bSuccess = TRUE;
		goto StartUpServiceDone;
	}

	bRetVal = StartServiceA(hService, 0, NULL);
	if(bRetVal == FALSE)
		goto StartUpServiceDone;

	dwEndTime = GetTickCount() + dwWait;
	for(dwTime = GetTickCount();
		dwTime < dwEndTime;
		dwTime = GetTickCount())
	{
		bRetVal = QueryServiceStatus(hService, &ServiceStatus);
		if(bRetVal == FALSE)
			goto StartUpServiceDone;

		if(ServiceStatus.dwCurrentState == SERVICE_RUNNING)
		{
			bSuccess = TRUE;
			break;
		}

		Sleep(100);
	}

StartUpServiceDone:

	if(bSuccess == FALSE)
		nError = GetLastError();
	if(hService)
        CloseServiceHandle(hService);
	if(hManager)
		CloseServiceHandle(hManager);
	if(bSuccess == FALSE)
		SetLastError(nError);

	return bSuccess;
}


BOOL	NSys::PauseService(CHAR *pServiceName, BOOL bResume, DWORD dwWait)
{
	SC_HANDLE			hManager;
	SC_HANDLE			hService;
	SERVICE_STATUS		ServiceStatus;
	DWORD				dwTime;
	DWORD				dwEndTime;
	BOOL				bRetVal;
	int					nError;
	BOOL				bSuccess;
	DWORD				dwControl;

	hManager = NULL;
	hService = NULL;
	bSuccess = FALSE;
	nError = ERROR_SUCCESS;

	if(bResume == FALSE)
		dwControl = SERVICE_CONTROL_PAUSE;
	else
		dwControl = SERVICE_CONTROL_CONTINUE;

	hManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (hManager == NULL)
		goto PauseServiceDone;

	hService = OpenServiceA(hManager, pServiceName, SERVICE_QUERY_STATUS | SERVICE_PAUSE_CONTINUE);
	if (hService == NULL)
		goto PauseServiceDone;

	QueryServiceStatus(hService, &ServiceStatus);
	bRetVal = ControlService(hService, dwControl, &ServiceStatus);
	if(bRetVal == FALSE)
		goto PauseServiceDone;

	dwEndTime = GetTickCount() + dwWait;
	for(dwTime = GetTickCount();
		dwTime < dwEndTime;
		dwTime = GetTickCount())
	{
		bRetVal = QueryServiceStatus(hService, &ServiceStatus);
		if(bRetVal == FALSE)
			goto PauseServiceDone;

		if(	((ServiceStatus.dwCurrentState == SERVICE_PAUSED) && (bResume == FALSE)) ||
			((ServiceStatus.dwCurrentState == SERVICE_RUNNING) && (bResume != FALSE))
			)
		{
			bSuccess = TRUE;
			break;
		}

		Sleep(100);
	}

PauseServiceDone:

	if(bSuccess == FALSE)
		nError = GetLastError();
	if(hService)
		CloseServiceHandle(hService);
	if(hManager)
		CloseServiceHandle(hManager);
	if(bSuccess == FALSE)
		SetLastError(nError);

	return bSuccess;
}


BOOL	NSys::StopService(CHAR *pServiceName, DWORD dwWait)
{
	SC_HANDLE			hManager;
	SC_HANDLE			hService;
	SERVICE_STATUS		ServiceStatus;
	DWORD				dwTime;
	DWORD				dwEndTime;
	BOOL				bRetVal;
	int					nError;
	BOOL				bSuccess;

	hManager = NULL;
	hService = NULL;
	bSuccess = FALSE;
	nError = ERROR_SUCCESS;

	hManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (hManager == NULL)
		goto StopServiceDone;

	hService = OpenServiceA(hManager, pServiceName, SERVICE_STOP | SERVICE_QUERY_STATUS);
	if (hService == NULL)
		goto StopServiceDone;

	QueryServiceStatus(hService, &ServiceStatus);
	bRetVal = ControlService(hService, SERVICE_CONTROL_STOP, &ServiceStatus);
	if(bRetVal == FALSE)
		goto StopServiceDone;

	dwEndTime = GetTickCount() + dwWait;
	for(dwTime = GetTickCount();
		dwTime < dwEndTime;
		dwTime = GetTickCount())
	{
		bRetVal = QueryServiceStatus(hService, &ServiceStatus);
		if(bRetVal == FALSE)
			goto StopServiceDone;

		if(ServiceStatus.dwCurrentState == SERVICE_STOPPED)
		{
			bSuccess = TRUE;
			break;
		}

		Sleep(200);
	}

StopServiceDone:

	if(bSuccess == FALSE)
		nError = GetLastError();
	if(hService)
		CloseServiceHandle(hService);
	if(hManager)
		CloseServiceHandle(hManager);
	if(bSuccess == FALSE)
		SetLastError(nError);

	return bSuccess;
}


BOOL	NSys::DeleteService(CHAR *pServiceName, DWORD dwWait)
{
	SC_HANDLE			hManager;
	SC_HANDLE			hService;
	SERVICE_STATUS		ServiceStatus;
	DWORD				dwTime;
	DWORD				dwEndTime;
	BOOL				bRetVal;
	int					nError;
	BOOL				bSuccess;

	hManager = NULL;
	hService = NULL;
	bSuccess = FALSE;
	nError = ERROR_SUCCESS;

	bRetVal = StopService(pServiceName);
	if(bRetVal == FALSE)
	{
		nError = GetLastError();
	}

	hManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (hManager == NULL)
		goto DeleteServiceDone;

	hService = OpenServiceA(hManager, pServiceName, SERVICE_QUERY_STATUS | DELETE);
	if (hService == NULL)
		goto DeleteServiceDone;

	bRetVal = ::DeleteService(hService);
	if (hService == NULL)
		goto DeleteServiceDone;

	dwEndTime = GetTickCount() + dwWait;
	for(dwTime = GetTickCount();
		dwTime < dwEndTime;
		dwTime = GetTickCount())
	{
		bRetVal = QueryServiceStatus(hService, &ServiceStatus);
		if(bRetVal == FALSE)
			goto DeleteServiceDone;

		if(ServiceStatus.dwCurrentState != SERVICE_STOP_PENDING)
		{
			bSuccess = TRUE;
			break;
		}

		Sleep(100);
	}

DeleteServiceDone:

	if(bSuccess == FALSE)
		nError = GetLastError();
	if(hService)
		CloseServiceHandle(hService);
	if(hManager)
		CloseServiceHandle(hManager);
	if(bSuccess == FALSE)
		SetLastError(nError);

	return bSuccess;
}

BOOL	NSys::ChangeServerDisplayName(CHAR *pServiceName, CHAR *pServiceDisplayName)
{
	CHAR	szRegpath[MAXPATH];
	BOOL	bRetVal;

	sprintf(szRegpath, "SYSTEM\\CurrentControlSet\\Services\\%s", pServiceName);
	bRetVal = NReg::SetReg(HKEY_LOCAL_MACHINE, szRegpath, "DisplayName", pServiceDisplayName);

	return bRetVal;

}

BOOL	NSys::ChangeServerDescrip(CHAR *pServiceName, CHAR *pServiceDescrip)
{
	CHAR	szRegpath[MAXPATH];
	BOOL	bRetVal;

	sprintf(szRegpath, "SYSTEM\\CurrentControlSet\\Services\\%s", pServiceName);
	bRetVal = NReg::SetReg(HKEY_LOCAL_MACHINE, szRegpath, "Description", pServiceDescrip);

	return bRetVal;
}


BOOL	NSys::ChangeServerImgPath(CHAR *pServiceName, CHAR *pExePath)
{
	CHAR		szRegpath[MAXPATH];
	CHAR		szFmtPath[MAXPATH];
	CHAR		szSavePath[MAXPATH];
	CHAR *		pFind;
	BOOL		bRetVal;

	sprintf(szRegpath, "SYSTEM\\CurrentControlSet\\Services\\%s", pServiceName);

	NFile::FormatFilePath(pExePath, szFmtPath);
	pFind = strchr(szFmtPath, ' ');
	if(pFind)
		sprintf(szSavePath, "\"%s\"", szFmtPath);
	else
		sprintf(szSavePath, "%s", szFmtPath);

	bRetVal = NReg::SHSetRegExsz(HKEY_LOCAL_MACHINE, szRegpath, "ImagePath", szSavePath);
	return bRetVal;
}


BOOL	NSys::ChangeServerDriverImgPath(CHAR *pServiceName, CHAR *pDriverPath)
{
	CHAR		szRegpath[MAXPATH];
	CHAR		szFmtPath[MAXPATH];
	CHAR		szSavePath[MAXPATH];
	BOOL		bRetVal;

	sprintf(szRegpath, "SYSTEM\\CurrentControlSet\\Services\\%s", pServiceName);

	NFile::FormatFilePath(pDriverPath, szFmtPath);
	sprintf(szSavePath, "\\??\\%s", szFmtPath);

	bRetVal = NReg::SHSetRegExsz(HKEY_LOCAL_MACHINE, szRegpath, "ImagePath", szSavePath);
	return bRetVal;
}

BOOL	NSys::SetServerDependOnService(CHAR *pServiceName, CHAR *pDependSvr)
{
	CHAR		szRegpath[MAXPATH];
	BOOL		bRetVal;

	sprintf(szRegpath, "SYSTEM\\CurrentControlSet\\Services\\%s", pServiceName);

	if(pDependSvr == NULL)
		bRetVal = NReg::DelReg(HKEY_LOCAL_MACHINE, szRegpath, "DependOnService");
	else
        bRetVal = NReg::SetRegMultisz(HKEY_LOCAL_MACHINE, szRegpath, "DependOnService", pDependSvr);

	return bRetVal;
}

BOOL	NSys::SetServerDependOnGroup(CHAR *pServiceName, CHAR *pDependGrp)
{
	CHAR		szRegpath[MAXPATH];
	BOOL		bRetVal;

	sprintf(szRegpath, "SYSTEM\\CurrentControlSet\\Services\\%s", pServiceName);
	if(pDependGrp == NULL)
		bRetVal = NReg::DelReg(HKEY_LOCAL_MACHINE, szRegpath, "DependOnGroup");
	else
		bRetVal = NReg::SetRegMultisz(HKEY_LOCAL_MACHINE, szRegpath, "DependOnGroup", pDependGrp);

	return bRetVal;
}


DWORD	NSys::GetServcieStartType(CHAR *pSvrName)
{
	CHAR		szRegpath[MAXPATH];
	DWORD		dwRetVal;
	BOOL		bRetVal;

	sprintf(szRegpath, "SYSTEM\\CurrentControlSet\\Services\\%s", pSvrName);

	dwRetVal = -1;
	bRetVal = NReg::SHGetReg(HKEY_LOCAL_MACHINE, szRegpath, "Start", &dwRetVal);
	if(bRetVal == FALSE)
		return -1;

	return dwRetVal;
}


BOOL	NSys::SetServcieStartType(CHAR *pSvrName, DWORD dwStartType)
{
	CHAR		szRegpath[MAXPATH];
	DWORD		dwRetVal;
	BOOL		bRetVal;

	sprintf(szRegpath, "SYSTEM\\CurrentControlSet\\Services\\%s", pSvrName);

	dwRetVal = -1;
	bRetVal = NReg::SHSetReg(HKEY_LOCAL_MACHINE, szRegpath, "Start", dwStartType);
	if(bRetVal == FALSE)
		return -1;

	return dwRetVal;
}


DWORD	NSys::GetServcieType(CHAR *pSvrName)
{
	CHAR		szRegpath[MAXPATH];
	DWORD		dwRetVal;
	BOOL		bRetVal;

	sprintf(szRegpath, "SYSTEM\\CurrentControlSet\\Services\\%s", pSvrName);

	dwRetVal = -1;
	bRetVal = NReg::SHGetReg(HKEY_LOCAL_MACHINE, szRegpath, "Type", &dwRetVal);
	if(bRetVal == FALSE)
		return -1;

	return dwRetVal;
}

BOOL	NSys::SetServcieType(CHAR *pSvrName, DWORD dwServiceType)
{
	CHAR		szRegpath[MAXPATH];
	DWORD		dwRetVal;
	BOOL		bRetVal;

	sprintf(szRegpath, "SYSTEM\\CurrentControlSet\\Services\\%s", pSvrName);

	dwRetVal = -1;
	bRetVal = NReg::SHSetReg(HKEY_LOCAL_MACHINE, szRegpath, "Type", dwServiceType);
	if(bRetVal == FALSE)
		return -1;

	return dwRetVal;
}


BOOL	NSys::GetServiceDescrip(CHAR *pSvrName, CHAR *pServiceDescrip)
{
	CHAR	szRegpath[MAXPATH];
	BOOL	bRetVal;

	sprintf(szRegpath, "SYSTEM\\CurrentControlSet\\Services\\%s", pSvrName);
	bRetVal = NReg::GetReg(HKEY_LOCAL_MACHINE, szRegpath, "Description", pServiceDescrip);

	return bRetVal;
}


BOOL	NSys::KillProc(DWORD dwProcId, DWORD dwExitCode)
{
	HANDLE			hProc;
	BOOL			bRetVal;
	int				nError;

	if(dwProcId == 0)
		dwProcId = GetCurrentProcessId();

	hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, 
		FALSE, dwProcId);
	if(hProc == NULL)
		return FALSE;

	bRetVal = TerminateProcess(hProc, dwExitCode);
	if(bRetVal == FALSE)
	{
		nError = GetLastError();
		CloseHandle(hProc);
		SetLastError(nError);
		return FALSE;
	}

	CloseHandle(hProc);
	return TRUE;
}


BOOL	NSys::KillProc(CHAR *pProcName, DWORD dwExitCode)
{
	std::vector<DWORD>				vProcList;
	std::vector<DWORD>::iterator	Iter;
	BOOL			bRetVal;
	BOOL			bResult;

	bRetVal = GetProcIdFromName(pProcName, vProcList);
	if(bRetVal == FALSE)
		return FALSE;

	bResult = TRUE;
	for(Iter = vProcList.begin();
		Iter != vProcList.end();
		Iter ++)
	{
		bRetVal = KillProc(*Iter, dwExitCode);
		if(bRetVal == FALSE)
			bResult = FALSE;
	}

	return bResult;
}


BOOL	NSys::GetProcIdFromNameNt4(CHAR *pProcName, DWORD *pdwProcId)
{
	CHAR			szModuleFileName[MAX_PATH*2];
	CHAR			szModName[MAX_PATH*2];
	int				i;
	DWORD			dwNeedSize, dwSize;
	DWORD			* pdwProID;
	int				nCount;
	BOOL			bRetVal;
	DWORD			dwRetVal;
	HANDLE			hProc;

	nCount = 256;
	pdwProID = NULL;
	for(i=0; ; i++)
	{
		if(pdwProID)
			delete [] pdwProID;

		EnumProcesses(pdwProID, nCount, &dwNeedSize);
		pdwProID = new DWORD[nCount];
		dwSize = sizeof(DWORD) * nCount;

		bRetVal = EnumProcesses(pdwProID, dwSize, &dwNeedSize);
		if(bRetVal == FALSE)
		{
			if(pdwProID)
				delete [] pdwProID;
			return FALSE;
		}

		if(dwSize != dwNeedSize)
			break;
	}

	nCount = dwNeedSize / sizeof(DWORD);

	for(i=0; i<nCount; i++)
	{
		hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
			FALSE, pdwProID[i]);
		if(hProc == NULL)
			continue;

		dwRetVal = GetModuleFileNameExA(hProc, NULL, 
			szModuleFileName, sizeof(szModuleFileName));
		if(dwRetVal != 0)
		{
			NFile::SpliteFilePath(szModuleFileName, NULL, szModName);

			if(stricmp(pProcName, szModName) == 0)
			{
				*pdwProcId = pdwProID[i];
				CloseHandle(hProc);
				return TRUE;
			}
		}

		CloseHandle(hProc);
	}

	if(pdwProID)
		delete [] pdwProID;

	return FALSE;
}


BOOL	NSys::GetProcIdFromName(CHAR *pProcName, DWORD *pdwProcId)
{
	std::vector<DWORD>	PidList;
	BOOL				bRetVal;

	*pdwProcId = 0;
	bRetVal = GetProcIdFromName(pProcName, PidList);
	if(bRetVal == FALSE || PidList.size() <= 0)
		return FALSE;

	*pdwProcId = PidList[0];
	return TRUE;
}


BOOL	NSys::GetProcIdFromName(CHAR *pProcName, std::vector<DWORD> &PidList)
{
	BOOL					bRetVal;
	HANDLE					hProcShot;
	PROCESSENTRY32W			ProcEntry;
	WCHAR					szExeFile[MAX_PATH];

	PidList.clear();
	NStr::Char2WChar(pProcName, szExeFile);

	hProcShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcShot == INVALID_HANDLE_VALUE)
		return FALSE;

	ProcEntry.dwSize = sizeof(ProcEntry);
	bRetVal = Process32FirstW(hProcShot, &ProcEntry);
	for(; bRetVal; )
	{
		if(wcsicmp(szExeFile, ProcEntry.szExeFile) == 0)
			PidList.push_back(ProcEntry.th32ProcessID);

		bRetVal = Process32NextW(hProcShot, &ProcEntry);
	}

	CloseHandle(hProcShot);
	return TRUE;
}


BOOL	GetProcNameFromIdOther(DWORD dwProcId, CHAR *pProcName)
{
	BOOL					bRetVal;
	HANDLE					hProcShot, hProc;
	PROCESSENTRY32W			ProcEntry;

	*pProcName = '\0';
	hProcShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcShot == INVALID_HANDLE_VALUE)
		return FALSE;

	ProcEntry.dwSize = sizeof(ProcEntry);
	bRetVal = Process32FirstW(hProcShot, &ProcEntry);
	for(;bRetVal;)
	{
		hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 
			FALSE, ProcEntry.th32ProcessID);
		if(hProc == FALSE)
		{
			bRetVal = Process32NextW(hProcShot, &ProcEntry);
			continue;
		}

		if(dwProcId == ProcEntry.th32ProcessID)
		{
			NStr::WChar2Char(ProcEntry.szExeFile, pProcName);
			CloseHandle(hProc);
			CloseHandle(hProcShot);
			return TRUE;
		}

		CloseHandle(hProc);
		bRetVal = Process32NextW(hProcShot, &ProcEntry);
	}

	CloseHandle(hProcShot);
	return FALSE;
}


BOOL	NSys::GetProcNameFromId(DWORD dwProcId, CHAR *pProcName)
{
	HANDLE					hProc;
	DWORD					dwRetVal;
	CHAR					szProcPath[MAXPATH];

	strcpy(pProcName, "None or Exit!");
	hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcId);
	if(hProc != NULL)
	{
		strcpy(szProcPath, "None or Exit!");	//	µ¼ÖÂSafeIEÒì³£
		dwRetVal = GetModuleFileNameExA(hProc, NULL, szProcPath, sizeof(szProcPath));
		CloseHandle(hProc);

		if(dwRetVal != 0)
		{
			NFile::SpliteFilePath(szProcPath, NULL, pProcName);
			return TRUE;
		}
	}
	return GetProcNameFromIdOther(dwProcId, pProcName);
}


DWORD	NSys::GetProcModAddr(DWORD dwPid, CHAR *pModName)
{
	BOOL					bRetVal;
	DWORD					dwModAddr;
	HANDLE					hProcShot;
	MODULEENTRY32 			ModEntry;
	TCHAR					szModName[MAX_PATH];

	dwModAddr = 0;
	*pModName = '\0';

	hProcShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	if(hProcShot == INVALID_HANDLE_VALUE)
		return FALSE;

	NStr::Char2TChar(pModName, szModName, sizeof(szModName));
	ModEntry.dwSize = sizeof(ModEntry);
	bRetVal = Module32First(hProcShot, &ModEntry);
	for(;
		bRetVal;
		bRetVal = Module32Next(hProcShot, &ModEntry))
	{
		if(_tcsicmp(ModEntry.szModule, szModName) == 0)
		{
			dwModAddr = (DWORD)ModEntry.hModule;
			break;
		}
	}

	CloseHandle(hProcShot);
	return dwModAddr;
}


DWORD	NSys::GetProcModAddr(CHAR *pProcName, CHAR *pModName)
{
	DWORD					dwProcId;
	BOOL					bRetVal;

	*pModName = '\0';

	bRetVal = GetProcIdFromName(pProcName, &dwProcId);
	if(bRetVal == FALSE)
		return FALSE;

	return GetProcModAddr(dwProcId, pModName);
}


int		NSys::GetExeMultiProcess(CHAR *pProcName)
{
	int						nCount;
	BOOL					bRetVal;
	HANDLE					hProcShot, hProc;
	PROCESSENTRY32W			ProcEntry;
	WCHAR					szProcName[MAXPATH];

	hProcShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcShot == INVALID_HANDLE_VALUE)
		return 0;

	ProcEntry.dwSize = sizeof(ProcEntry);
	bRetVal = Process32FirstW(hProcShot, &ProcEntry);
	if(bRetVal == FALSE)
		return 0;

	NStr::Char2WChar(pProcName, szProcName);
	for(nCount=0; ; )
	{
		hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 
			FALSE, ProcEntry.th32ProcessID);
		if(hProc == FALSE)
		{
			bRetVal = Process32NextW(hProcShot, &ProcEntry);
			if(bRetVal == FALSE)
				break;

			continue;
		}

		if(wcsicmp(szProcName, ProcEntry.szExeFile) == 0)
		{
			nCount ++;
		}

		CloseHandle(hProc);
		bRetVal = Process32NextW(hProcShot, &ProcEntry);
		if(bRetVal == FALSE)
			break;
	}

	CloseHandle(hProcShot);
	return nCount;
}


BOOL	NSys::GetOperateSystemName(_bstr_t *pbStrOSVersion)
{
	CHAR				szOSVersion[256];
	OSVERSIONINFOEXA	OsInfoExA;
	BOOL				bOsVersionInfoEx;
	static	_bstr_t		bStrOSName = "";

	if(bStrOSName.length() != 0)
	{
		*pbStrOSVersion = bStrOSName;
		return TRUE;
	}

	*pbStrOSVersion = "Unknown";
	memset(&OsInfoExA, 0, sizeof(OsInfoExA));

	OsInfoExA.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
	bOsVersionInfoEx = GetVersionExA((OSVERSIONINFOA *)&OsInfoExA);
	if(bOsVersionInfoEx == FALSE)
	{
		OsInfoExA.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

		if(GetVersionExA((OSVERSIONINFOA *)&OsInfoExA) == FALSE) 
		{
			return FALSE;
		}
	}

	switch (OsInfoExA.dwPlatformId)
	{
	case VER_PLATFORM_WIN32s:
		strcpy(szOSVersion, "Microsoft Win32s");
		break;

	case VER_PLATFORM_WIN32_WINDOWS:
		if (OsInfoExA.dwMajorVersion == 4) 
		{
			if (OsInfoExA.dwMinorVersion == 0) 
			{
				strcpy(szOSVersion, "Windows 95 ");
				if (OsInfoExA.szCSDVersion[1] == 'C' || OsInfoExA.szCSDVersion[1] == 'B')
					strcat(szOSVersion, "OSR2 ");
			}
			else if (OsInfoExA.dwMinorVersion == 10) 
			{
				strcpy(szOSVersion, "Windows 98 ");
				if (OsInfoExA.szCSDVersion[1] == 'A')
					strcat(szOSVersion, "SE ");
			}
			else if (OsInfoExA.dwMinorVersion == 90) 
			{
				strcpy(szOSVersion, "Windows ME");
			}
			else 
			{
				strcpy(szOSVersion, "Unknown");
			}
		}
		break;

	case VER_PLATFORM_WIN32_NT:
		if ((OsInfoExA.dwMajorVersion == 6) &&
			(OsInfoExA.dwMinorVersion == 1)) 
		{
			strcpy(szOSVersion, "Windows 7 ");
		} 
		else if((OsInfoExA.dwMajorVersion == 6) &&
			(OsInfoExA.dwMinorVersion == 0)) 
		{
			strcpy(szOSVersion, "Windows Vista ");
		} 
		else if((OsInfoExA.dwMajorVersion == 5) &&
			(OsInfoExA.dwMinorVersion == 2)) 
		{
			strcpy(szOSVersion, "Windows Server 2003 ");
		} 
		else if((OsInfoExA.dwMajorVersion == 5) &&
			(OsInfoExA.dwMinorVersion == 1)) 
		{
			strcpy(szOSVersion, "Windows XP ");
		} 
		else if((OsInfoExA.dwMajorVersion == 5) &&
			(OsInfoExA.dwMinorVersion == 0)) 
		{
			strcpy(szOSVersion, "Windows 2000 ");
		}
		else if(OsInfoExA.dwMajorVersion <= 4) 
		{
			strcpy(szOSVersion, "Windows NT ");
		}

		*pbStrOSVersion = szOSVersion;

		if (bOsVersionInfoEx) 
		{
			if (OsInfoExA.wProductType == VER_NT_WORKSTATION)				//NT
			{
				if(OsInfoExA.dwMajorVersion == 4)
					strcat(szOSVersion, "Workstation 4.0 ");
				else if( OsInfoExA.wSuiteMask & VER_SUITE_PERSONAL )
					strcat(szOSVersion, "Home Edition " );
				else
					strcat(szOSVersion, "Professional " );
			}
			else if ( OsInfoExA.wProductType == VER_NT_SERVER )
			{
				if( OsInfoExA.dwMajorVersion == 5 && OsInfoExA.dwMinorVersion == 2 )		//2003
				{
					if( OsInfoExA.wSuiteMask & VER_SUITE_DATACENTER )
						strcat(szOSVersion, "Datacenter Edition " );
					else if( OsInfoExA.wSuiteMask & VER_SUITE_ENTERPRISE )
						strcat(szOSVersion, "Enterprise Edition " );
					else if ( OsInfoExA.wSuiteMask == VER_SUITE_BLADE )
						strcat(szOSVersion, "Web Edition " );
					else
						strcat(szOSVersion, "Standard Edition " );
				}
				else if( OsInfoExA.dwMajorVersion == 5 && OsInfoExA.dwMinorVersion == 0 )	//Xp
				{
					if( OsInfoExA.wSuiteMask & VER_SUITE_DATACENTER )
						strcat(szOSVersion, "Datacenter Server " );
					else if( OsInfoExA.wSuiteMask & VER_SUITE_ENTERPRISE )
						strcat(szOSVersion, "Advanced Server " );
					else
						strcat(szOSVersion, "Server " );
				}
				else  // Windows NT 4.0
				{
					if( OsInfoExA.wSuiteMask & VER_SUITE_ENTERPRISE )
						strcat(szOSVersion, "Server 4.0, Enterprise Edition " );
					else
						strcat(szOSVersion, "Server 4.0 ");
				}
			}
		}
		else 
		{
			HKEY hKey;
			char szProductType[256];
			DWORD dwBufLen = 256;
			LONG lRet;

			lRet = RegOpenKeyExA( HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ProductOptions", 0, KEY_QUERY_VALUE, &hKey);
			if(lRet != ERROR_SUCCESS )
				return FALSE;

			lRet = RegQueryValueExA(hKey, "ProductType", NULL, NULL,	(LPBYTE)szProductType, &dwBufLen);
			if((lRet != ERROR_SUCCESS) || (dwBufLen > 256))
				return FALSE;

			RegCloseKey( hKey );

			if (stricmp("WINNT", szProductType) == 0)
				strcpy(szOSVersion, "Workstation ");
			if ( stricmp( "LANMANNT", szProductType) == 0 )
				strcpy(szOSVersion, "Server ");
			if ( lstrcmpiA( "SERVERNT", szProductType) == 0 )
				stricmp(szOSVersion, "Advanced Server ");
		}

		break;

	default:
		return FALSE;
	}

	*pbStrOSVersion = szOSVersion;
	bStrOSName = szOSVersion;
	return TRUE;
}

// VER_PLATFORM_WIN32s             0
// VER_PLATFORM_WIN32_WINDOWS      1
// VER_PLATFORM_WIN32_NT           2
BOOL NSys::GetOsPlatform(DWORD * pdwPlatformId)
{
	OSVERSIONINFOEXA	OsInfoExA;
	BOOL				bRetVal;

	memset(&OsInfoExA, 0, sizeof(OsInfoExA));

	OsInfoExA.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
	bRetVal = GetVersionExA((OSVERSIONINFOA *)&OsInfoExA);
	if(bRetVal == FALSE)
	{
		OsInfoExA.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
		bRetVal = GetVersionExA((OSVERSIONINFOA *)&OsInfoExA);
		if(bRetVal == FALSE)
			return FALSE;
	}

	*pdwPlatformId = OsInfoExA.dwPlatformId;
	return TRUE;
}


int		NSys::GetOsMajorVersion()
{
	OSVERSIONINFOEXA	OsInfoExA;
	BOOL				bRetVal;

	memset(&OsInfoExA, 0, sizeof(OsInfoExA));

	OsInfoExA.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
	bRetVal = GetVersionExA((OSVERSIONINFOA *)&OsInfoExA);
	if(bRetVal == FALSE)
	{
		OsInfoExA.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
		bRetVal = GetVersionExA((OSVERSIONINFOA *)&OsInfoExA);
		if(bRetVal == FALSE)
			return -1;
	}

	return OsInfoExA.dwMajorVersion;
}


BOOL	NSys::GetVersions(int &nV1, int &nV2, int &nSp)
{
	OSVERSIONINFOEXA	OsInfoExA;
	BOOL				bRetVal;

	memset(&OsInfoExA, 0, sizeof(OsInfoExA));

	OsInfoExA.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
	bRetVal = GetVersionExA((OSVERSIONINFOA *)&OsInfoExA);
	if(bRetVal == FALSE)
	{
		OsInfoExA.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
		bRetVal = GetVersionExA((OSVERSIONINFOA *)&OsInfoExA);
		if(bRetVal == FALSE)
			return -1;
	}

	nV1 = OsInfoExA.dwMajorVersion;
	nV2 = OsInfoExA.dwMinorVersion;
	nSp = OsInfoExA.wServicePackMajor;

	return TRUE;
}

DWORD	NSys::GetServicePack()
{
	OSVERSIONINFOEXA	OsInfoExA;
	BOOL				bRetVal;

	memset(&OsInfoExA, 0, sizeof(OsInfoExA));

	OsInfoExA.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
	bRetVal = GetVersionExA((OSVERSIONINFOA *)&OsInfoExA);
	if(bRetVal == FALSE)
	{
		OsInfoExA.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
		bRetVal = GetVersionExA((OSVERSIONINFOA *)&OsInfoExA);
		if(bRetVal == FALSE)
			return -1;
	}

	return OsInfoExA.wServicePackMajor;
}

DWORD	NSys::GetVersions()
{
	int		nV1;
	int		nV2;
	int		nSp;
	DWORD	dwValue;

	nV1 = nV2 = nSp = 0;
	GetVersions(nV1, nV2, nSp);

	dwValue = (nV1 << 24) | (nV2 << 16) | (nSp << 8) | 0;
	return dwValue;
}

DWORD	NSys::GetSysBuildNumber()
{
	OSVERSIONINFOEXA	OsInfoExA;
	BOOL				bRetVal;

	memset(&OsInfoExA, 0, sizeof(OsInfoExA));

	OsInfoExA.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
	bRetVal = GetVersionExA((OSVERSIONINFOA *)&OsInfoExA);
	if(bRetVal == FALSE)
	{
		OsInfoExA.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
		bRetVal = GetVersionExA((OSVERSIONINFOA *)&OsInfoExA);
		if(bRetVal == FALSE)
			return 0;
	}

	return OsInfoExA.dwBuildNumber;
}

int		NSys::GetErrorString(_bstr_t * pbStrErrStr, int nErrorCode)
{
	CHAR	szMsgBuf[1000];
	if(nErrorCode == 0)
		nErrorCode = GetLastError();

	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, nErrorCode,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), szMsgBuf, sizeof(szMsgBuf), NULL);

	*pbStrErrStr = szMsgBuf;
	return nErrorCode;
}





CHAR *	NSys::GetErrorString(CHAR *pBuffer, int nErrorCode)
{
	static	CHAR		s_szLastError[1024];

	if(nErrorCode == 0)
		nErrorCode = GetLastError();

	if(pBuffer == NULL)
		pBuffer = s_szLastError;

	strcpy(pBuffer, "Unknow Error!");
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, nErrorCode,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), pBuffer, 1024, NULL);

	return pBuffer;
}


BOOL	NSys::MacPathParse(CHAR *pPath, CHAR *pRealPath)
{
	CHAR	szTempPath[MAXPATH];
	CHAR *	pStart;
	CHAR *	pEnd;
	int		nSize;
	CHAR	szMacName[MAXPATH];
	CHAR	szParsed[MAXPATH];
	BOOL	bResult;

	static	std::map<std::string, int>		MacNameMap;
	static	BOOL	bInit = FALSE;

	strcpy(pRealPath, pPath);
	strcpy(szTempPath, pPath);

	pStart = strchr(szTempPath, '%');
	if(pStart == NULL)
		return TRUE;
	pStart ++;
	pEnd = strchr(pStart, '%');
	if(pEnd == NULL)
		return TRUE;

	if(bInit == FALSE)
	{
		MacNameMap["CURDIR"]			= SYSPATH_CurrentDirectory;
		MacNameMap["WIN"]				= SYSPATH_WindowsDirectory;
		MacNameMap["WINDOWS"]			= SYSPATH_WindowsDirectory;
		MacNameMap["SYSTEM"]			= SYSPATH_SystemDirectory;
		MacNameMap["SYSTEMROOT"]		= SYSPATH_SystemDirectory;
		MacNameMap["PROGRAMFILES"]		= SYSPATH_ProgramsDirectory;
		MacNameMap["PROGRAM FILES"]		= SYSPATH_ProgramsDirectory;
		MacNameMap["ALLUSERSTARTMENUPRGM"]	= SYSPATH_AllUserStartMenuPrgm;
		MacNameMap["CURUSERSTARTMENUPRGM"]	= SYSPATH_CurUserStartMenuPrgm;
		MacNameMap["ALLUSERDESKTOP"]	= SYSPATH_AllUserDesktTop;
		MacNameMap["CURUSERDESKTOP"]	= SYSPATH_CurUserDesktTop;
		MacNameMap["ALLUSERSTARTUP"]	= SYSPATH_AllUserStartUp;
		MacNameMap["CURUSERSTARTUP"]	= SYSPATH_CurUserStartUp;
		MacNameMap["APP"]				= SYSPATH_CurrentModule;
		MacNameMap["WINDIR"]			= SYSPATH_WindowsDirectory;
		MacNameMap["APPDATA"]			= SYSPATH_AppData;
		MacNameMap["FAVORITES"]			= SYSPATH_Favorites;


		bInit = TRUE;
	}

	std::map<std::string, int>::iterator		Iter;
	int				nFindId;

	nSize = (int)(pEnd - pStart);
	memset(szMacName, 0, sizeof(szMacName));
	memcpy(szMacName, pStart, nSize);
	strupr(szMacName);

	bResult = FALSE;
	strcpy(szParsed, szMacName);

	Iter = MacNameMap.find(szMacName);
	if(Iter != MacNameMap.end())
	{
		nFindId = Iter->second;
		bResult = GetSystemPath(nFindId, szParsed, sizeof(szParsed));
	}

	if(bResult == FALSE)
	{
		bResult = MacPathParseOffice(szMacName, szParsed);
	}

	if(bResult == FALSE)
	{
		assert(0);
		return FALSE;
	}

	pStart --;
	pStart[0] = 0;
	pStart = szTempPath;
	pEnd ++;

	strcpy(pRealPath, pStart);
	strcat(pRealPath, szParsed);
	strcat(pRealPath, pEnd);
	return bResult;
}


BOOL	NSys::MacPathParseOffice(CHAR *pMacName, CHAR *pRealPath)
{
	static	BOOL		bInit = FALSE;
	static	CHAR 		pOfficePath[][3][MAXPATH] = 
	{
		{"%Office2000Dir%",	"SOFTWARE\\Microsoft\\Office\\9.0\\Common\\InstallRoot"},
		{"%Office2002Dir%",	"SOFTWARE\\Microsoft\\Office\\10.0\\Common\\InstallRoot"},
		{"%Office2003Dir%",	"SOFTWARE\\Microsoft\\Office\\11.0\\Common\\InstallRoot"},
		{"%Office2007Dir%",	"SOFTWARE\\Microsoft\\Office\\12.0\\Common\\InstallRoot"},
		{"%Word2000Dir%",	"SOFTWARE\\Microsoft\\Office\\9.0\\Word\\InstallRoot"},
		{"%Word2002Dir%",	"SOFTWARE\\Microsoft\\Office\\10.0\\Word\\InstallRoot"},
		{"%Word2003Dir%",	"SOFTWARE\\Microsoft\\Office\\11.0\\Word\\InstallRoot"},
		{"%Word2007Dir%",	"SOFTWARE\\Microsoft\\Office\\12.0\\Word\\InstallRoot"},
		{"%Access2000Dir%",	"SOFTWARE\\Microsoft\\Office\\9.0\\Access\\InstallRoot"},
		{"%Access2002Dir%",	"SOFTWARE\\Microsoft\\Office\\10.0\\Access\\InstallRoot"},
		{"%Access2003Dir%",	"SOFTWARE\\Microsoft\\Office\\11.0\\Access\\InstallRoot"},
		{"%Access2007Dir%",	"SOFTWARE\\Microsoft\\Office\\12.0\\Access\\InstallRoot"},
		{"%Excel2000Dir%",	"SOFTWARE\\Microsoft\\Office\\9.0\\Excel\\InstallRoot"},
		{"%Excel2002Dir%",	"SOFTWARE\\Microsoft\\Office\\10.0\\Excel\\InstallRoot"},
		{"%Excel2003Dir%",	"SOFTWARE\\Microsoft\\Office\\11.0\\Excel\\InstallRoot"},
		{"%Excel2007Dir%",	"SOFTWARE\\Microsoft\\Office\\12.0\\Excel\\InstallRoot"},
		{"%Outlook2000Dir%",	"SOFTWARE\\Microsoft\\Office\\9.0\\Outlook\\InstallRoot"},
		{"%Outlook2002Dir%",	"SOFTWARE\\Microsoft\\Office\\10.0\\Outlook\\InstallRoot"},
		{"%Outlook2003Dir%",	"SOFTWARE\\Microsoft\\Office\\11.0\\Outlook\\InstallRoot"},
		{"%Outlook2007Dir%",	"SOFTWARE\\Microsoft\\Office\\12.0\\Outlook\\InstallRoot"},
		{"%PowerPoint2000Dir%",	"SOFTWARE\\Microsoft\\Office\\9.0\\PowerPoint\\InstallRoot"},
		{"%PowerPoint2002Dir%",	"SOFTWARE\\Microsoft\\Office\\10.0\\PowerPoint\\InstallRoot"},
		{"%PowerPoint2003Dir%",	"SOFTWARE\\Microsoft\\Office\\11.0\\PowerPoint\\InstallRoot"},
		{"%PowerPoint2007Dir%",	"SOFTWARE\\Microsoft\\Office\\12.0\\PowerPoint\\InstallRoot"},
		{"", ""}
	};

	int			i;
	CHAR		szMacName[MAXPATH];
	CHAR		szParsed[MAXPATH];
	int			nLen;
	BOOL		bRetVal;

	if(bInit == FALSE)
	{
		for(i=0; pOfficePath[i][0][0]; i++)
		{
			strcpy(szMacName, pOfficePath[i][0]+1);
			nLen = (int)strlen(szMacName);
			szMacName[nLen-1] = 0;
			strupr(szMacName);

			szParsed[0] = 0;
			bRetVal = NReg::SHGetReg(HKEY_LOCAL_MACHINE, pOfficePath[i][1], "Path", szParsed, sizeof(szParsed));
			if(bRetVal == FALSE)
				szParsed[0] = 0;

			strcpy(pOfficePath[i][0], szMacName);
			strcpy(pOfficePath[i][2], szParsed);
		}

		bInit = TRUE;
	}

	strcpy(szMacName, pMacName);
	strupr(szMacName);

	for(i=0; pOfficePath[i][0][0]; i++)
	{
		if(strcmp(szMacName, pOfficePath[i][0]) == 0)
		{
			strcpy(pRealPath, pOfficePath[i][2]);
			return TRUE;
		}
	}

	pRealPath[0] = 0;
	return FALSE;
}


BOOL	NSys::MacPathUprParse(CHAR *pPath, CHAR *pRealPath)
{
	CHAR	szFolderPath[MAXPATH];

	if(strchr(pPath, '%') == NULL)
	{
		strcpy(pRealPath, pPath);
		return TRUE;
	}

	strcpy(pRealPath, pPath);

	GetSystemPath(1, szFolderPath, sizeof(szFolderPath));
	NStr::StrReplace(pRealPath, "%SYSTEMROOT%", szFolderPath);
	NStr::StrReplace(pRealPath, "%WINDIR%", szFolderPath);
	NStr::StrReplace(pRealPath, "%WINDOW%", szFolderPath);

	GetSystemPath(2, szFolderPath, sizeof(szFolderPath));
	NStr::StrReplace(pRealPath, "%SYSTEM%", szFolderPath);

	GetSystemPath(3, szFolderPath, sizeof(szFolderPath));
	NStr::StrReplace(pRealPath, "%PROGRAM FILES%", szFolderPath);
	NStr::StrReplace(pRealPath, "%PROGRAMFILES%", szFolderPath);

	GetSystemPath(4, szFolderPath, sizeof(szFolderPath));
	NStr::StrReplace(pRealPath, "%ALLUSERSTARTMENUPRGM%", szFolderPath);

	GetSystemPath(5, szFolderPath, sizeof(szFolderPath));
	NStr::StrReplace(pRealPath, "%ALLUSERDESKTOP%", szFolderPath);

	return TRUE;
}


BOOL	NSys::GetSpecialPath(int nFolder, CHAR *pPath, int nBufSize)
{
	BOOL	bRetVal;
	CHAR	szPath[MAX_PATH];

	szPath[0] = 0;
	bRetVal = SHGetSpecialFolderPathA(NULL, szPath, nFolder, FALSE);
	NStr::StrCopy(pPath, szPath, nBufSize);

	return bRetVal;
}


BOOL	NSys::GetProgramFilesDir(CHAR *pProgramDir, int nBufSize)
{
	CHAR		szProgramDir[MAX_PATH*2] = "";
	BOOL		bRetVal;
	int			nLen;

	bRetVal = NReg::GetReg(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion", 
		"ProgramFilesDir", (BYTE *)szProgramDir, sizeof(szProgramDir));
	if(bRetVal == FALSE)
	{
		bRetVal = GetEnvironmentVariableA("ProgramFiles", szProgramDir, sizeof(szProgramDir));
		if(bRetVal == FALSE)
			return FALSE;
	}

	nLen = (int)strlen(szProgramDir);
	if(nLen == 0)
		return FALSE;

	if(szProgramDir[nLen-1] == '\\')
		szProgramDir[nLen-1] = 0;

	NStr::StrCopy(pProgramDir, szProgramDir, nBufSize);
	return TRUE;
}


BOOL	NSys::GetAllUserStartMenuPrgm(CHAR *pStartMenuPrgmDir, int nBufSize)
{
	return GetSpecialPath(CSIDL_COMMON_PROGRAMS, pStartMenuPrgmDir, nBufSize);
}
BOOL	NSys::GetCurUserStartMenuPrgm(CHAR *pStartMenuPrgmDir, int nBufSize)
{
	return GetSpecialPath(CSIDL_PROGRAMS, pStartMenuPrgmDir, nBufSize);
}


BOOL	NSys::GetAllUserStartUp(CHAR *pStartUpDir, int nBufSize)
{
	return GetSpecialPath(CSIDL_COMMON_STARTUP, pStartUpDir, nBufSize);
}
BOOL	NSys::GetCurUserStartUp(CHAR *pStartUpDir, int nBufSize)
{
	return GetSpecialPath(CSIDL_STARTUP, pStartUpDir, nBufSize);
}

BOOL	NSys::GetAllUserDesktop(CHAR *pDesktopDir, int nBufSize)
{
	return GetSpecialPath(CSIDL_COMMON_DESKTOPDIRECTORY, pDesktopDir, nBufSize);
}
BOOL	NSys::GetCurUserDesktop(CHAR *pDesktopDir, int nBufSize)
{
	return GetSpecialPath(CSIDL_DESKTOP, pDesktopDir, nBufSize);
}

BOOL	NSys::GetAppData(CHAR *pAppDatDir, int nBufSize)
{
	return GetSpecialPath(CSIDL_APPDATA, pAppDatDir, nBufSize);
}
BOOL	NSys::GetFavorites(CHAR *pFavorites, int nBufSize)
{
	return GetSpecialPath(CSIDL_FAVORITES, pFavorites, nBufSize);
}



BOOL	NSys::GetSystemPath(int nIndex, CHAR *pBuffer, size_t nBufSize)
{
	static	BOOL	bInit = FALSE;
	static	CHAR	szWinDir[MAXPATH];
	static	CHAR	szSysDir[MAXPATH];
	static	CHAR	szProgramDir[MAXPATH];
	static	CHAR	szAllUserStartMenuPrgm[MAXPATH];
	static	CHAR	szCurUserStartMenuPrgm[MAXPATH];
	static	CHAR	szAllUserDesktop[MAXPATH];
	static	CHAR	szCurUserDesktop[MAXPATH];
	static	CHAR	szAllUserStartUp[MAXPATH];
	static	CHAR	szCurUserStartUp[MAXPATH];
	static	CHAR	szCurrentModule[MAXPATH];
	static	CHAR	szAppData[MAXPATH];
	static	CHAR	szFavorites[MAXPATH];

	if(bInit == FALSE)
	{
		GetWindowsDirectoryA(szWinDir, sizeof(szWinDir));
		GetSystemDirectoryA(szSysDir, sizeof(szSysDir));
		GetProgramFilesDir(szProgramDir, sizeof(szProgramDir));
		GetAllUserStartMenuPrgm(szAllUserStartMenuPrgm, sizeof(szAllUserStartMenuPrgm));
		GetCurUserStartMenuPrgm(szCurUserStartMenuPrgm, sizeof(szCurUserStartMenuPrgm));
		GetAllUserDesktop(szAllUserDesktop, sizeof(szAllUserDesktop));
		GetCurUserDesktop(szCurUserDesktop, sizeof(szCurUserDesktop));
		GetAllUserStartUp(szAllUserStartUp, sizeof(szAllUserStartUp));
		GetCurUserStartUp(szCurUserStartUp, sizeof(szCurUserStartUp));
		NFile::GetModuleDir(szCurrentModule);
		GetAppData(szAppData, sizeof(szCurUserStartUp));
		GetFavorites(szFavorites, sizeof(szFavorites));

		bInit = TRUE;
	}

	switch(nIndex)
	{
	case SYSPATH_CurrentDirectory:
		GetCurrentDirectoryA((DWORD)nBufSize, pBuffer);
		break;
	case SYSPATH_WindowsDirectory:	
		NStr::StrCopy(pBuffer, szWinDir, (int)nBufSize);
		break;
	case SYSPATH_SystemDirectory:
		NStr::StrCopy(pBuffer, szSysDir, (int)nBufSize);
		break;
	case SYSPATH_ProgramsDirectory:
		NStr::StrCopy(pBuffer, szProgramDir, (int)nBufSize);
		break;

	case SYSPATH_AllUserStartMenuPrgm:
		NStr::StrCopy(pBuffer, szAllUserStartMenuPrgm, (int)nBufSize);
		break;
	case SYSPATH_CurUserStartMenuPrgm:
		NStr::StrCopy(pBuffer, szCurUserStartMenuPrgm, (int)nBufSize);
		break;
	case SYSPATH_AllUserDesktTop:
		NStr::StrCopy(pBuffer, szAllUserDesktop, (int)nBufSize);
		break;
	case SYSPATH_CurUserDesktTop:
		NStr::StrCopy(pBuffer, szCurUserDesktop, (int)nBufSize);
		break;
	case SYSPATH_AllUserStartUp:
		NStr::StrCopy(pBuffer, szAllUserStartUp, (int)nBufSize);
		break;
	case SYSPATH_CurUserStartUp:
		NStr::StrCopy(pBuffer, szCurUserStartUp, (int)nBufSize);
		break;
	case SYSPATH_CurrentModule:
		NStr::StrCopy(pBuffer, szCurrentModule, (int)nBufSize);
		break;
	case SYSPATH_AppData:
		NStr::StrCopy(pBuffer, szAppData, (int)nBufSize);
		break;
	case SYSPATH_Favorites:
		NStr::StrCopy(pBuffer, szFavorites, (int)nBufSize);
		break;

	default:
		assert(0);
		strcpy(pBuffer, "");
		return FALSE;
	}

	return TRUE;
}


BOOL	NSys::GetExeFromCmdLine(CHAR *pCmdLine, CHAR *pExePath)
{
	CHAR *		pFind;
	CHAR *		pHead;
	CHAR		szCmdLine[4096];
	BOOL		bRetVal;

	*pExePath = 0;

	if(pCmdLine == NULL)
		return FALSE;

	pHead = "\\??\\";
	if(NStr::StrHeadCmp(pCmdLine, pHead) == 0)
	{
		strcpy(pExePath, pCmdLine + strlen(pHead));
		return TRUE;
	}

	strcpy(szCmdLine, pCmdLine);
	if(szCmdLine[0] == '"')
	{
		pFind = strchr(szCmdLine+1, '"');
		if(pFind == NULL)
			return FALSE;

		*pFind = 0;
		strcpy(pExePath, szCmdLine+1);
		return TRUE;
	}

	pFind = strchr(szCmdLine, ' ');
	if(pFind == NULL)
	{
		strcpy(pExePath, szCmdLine);
		return TRUE;
	}

	pHead = ".exe ";
	pFind = (CHAR *)NData::MemFindStrNoCase(pHead, szCmdLine);
	if(pFind != NULL)
	{
		pFind += strlen(pHead) - 1;
		pFind[0] = 0;
		strcpy(pExePath, szCmdLine);
		return TRUE;
	}

	bRetVal = NFile::FileExist(szCmdLine);
	if(bRetVal)
	{
		strcpy(pExePath, szCmdLine);
		return TRUE;
	}

	pHead = ".exe";
	if(NStr::StrTailCmp(szCmdLine, pHead) == 0)
	{
		strcpy(pExePath, szCmdLine);
		return TRUE;
	}

	pFind = strchr(szCmdLine, ' ');
	if(pFind)
	{
		pFind[0] = 0;	
		strcpy(pExePath, szCmdLine);
		return TRUE;
	}

	BREAKIF(1);
	strcpy(pExePath, szCmdLine);
	return TRUE;
}


BOOL	NSys::GetClipString(_bstr_t * pbString)
{
	HANDLE		hString;

	*pbString = "";

	if(OpenClipboard(NULL) == FALSE)
		return FALSE;

	hString = GetClipboardData(CF_TEXT);
	if(hString == NULL)
	{
		int		xxx = GetLastError();
		return FALSE;
	}
	CloseClipboard();

	*pbString = (CHAR *)hString;
	return TRUE;
}

int		NSys::GetPlatformId()
{
	OSVERSIONINFOA		OsInfo = {0};

	OsInfo.dwOSVersionInfoSize = sizeof(OsInfo);
	GetVersionExA(&OsInfo);
	return OsInfo.dwPlatformId;
}


BOOL	NSys::DbgClassVirtual(VOID * pClass)
{
	FARPROC		dwFunList[1024];
	VOID		* pClassVTB;

	pClassVTB = *(DWORD **)pClass;
	memcpy(dwFunList, pClassVTB, sizeof(dwFunList));

	return TRUE;
}


//	Byte, Kb, Mb, Gb  BufferSize > 100
BOOL	NSys::GetUnitSize(double dSize, CHAR *pBuffer)
{
	if(dSize < 1024.0)
	{
		sprintf(pBuffer, "%d Byte", (int)dSize);
	}
	else if(dSize < 1024.0*1024)
	{
		sprintf(pBuffer, "%.2f KB", dSize/1024);
	}
	else if(dSize < 1024.0*1024*1024)
	{
		sprintf(pBuffer, "%.2f MB", dSize/1024/1024);
	}
	else if(dSize < 1024.0*1024*1024*1024)
	{
		sprintf(pBuffer, "%.2f GB", dSize/1024/1024/1024);
	}
	else if(dSize < 1024.0*1024*1024*1024*1024)
	{
		sprintf(pBuffer, "%.2f TB", dSize/1024/1024/1024/1024);
	}

	return TRUE;
}


BOOL	NSys::RegisterFile(CHAR *pFile, DWORD dwWaitTime)
{
	CHAR					szCommandLine[1024];
	CHAR					szFullPath[MAX_PATH];
	STARTUPINFOA			si;
	PROCESS_INFORMATION		pi;
	BOOL					bRetVal;
	DWORD					dwExitCode;

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	GetFullPathNameA(pFile, sizeof(szFullPath), szFullPath, NULL);
	sprintf(szCommandLine, "regsvr32.exe /s \"%s\"", szFullPath);

	bRetVal = CreateProcessA(NULL, szCommandLine, NULL, NULL, 
		FALSE, 0, NULL, NULL, &si, &pi);
	if(bRetVal == FALSE)
		return FALSE;

	WaitForSingleObject(pi.hProcess, dwWaitTime);

	dwExitCode = 0;
	GetExitCodeProcess(pi.hProcess, &dwExitCode);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if(dwExitCode != 0)
	{
		SetLastError(dwExitCode);
		return FALSE;
	}

	return TRUE;
}


BOOL	NSys::UnRegisterFile(CHAR *pFile, DWORD dwWaitTime)
{
	CHAR					szCommandLine[1024];
	CHAR					szFullPath[MAX_PATH];
	STARTUPINFOA			si;
	PROCESS_INFORMATION		pi;
	BOOL					bRetVal;
	DWORD					dwExitCode;

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	GetFullPathNameA(pFile, sizeof(szFullPath), szFullPath, NULL);
	sprintf(szCommandLine, "regsvr32.exe /u /s \"%s\"", szFullPath);

	bRetVal = CreateProcessA(NULL, szCommandLine, NULL, NULL, 
		FALSE, 0, NULL, NULL, &si, &pi);
	if(bRetVal == FALSE)
		return FALSE;

	WaitForSingleObject(pi.hProcess, dwWaitTime);

	dwExitCode = 0;
	GetExitCodeProcess(pi.hProcess, &dwExitCode);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if(dwExitCode != 0)
	{
		SetLastError(dwExitCode);
		return FALSE;
	}

	return TRUE;
}


static	int		LID2Index[100][2] = 
{
	//	mk:@MSITStore:D:\Program%20Files\Microsoft%20Visual%20Studio\MSDN98\98VS\2052\winbase.chm::/devdoc/live/pdwbase/nls_8xo3.htm
	{0x0409, 1},			//English (United States) 
	{0x0809, 1},			//English (United Kingdom) 
	{0x0C09, 1},			//English (Australian) 
	{0x1009, 1},			//English (Canadian) 
	{0x1409, 1},			//English (New Zealand) 
	{0x1809, 1},			//English (Ireland) 
	{0x1C09, 1},			//English (South Africa) 
	{0x2009, 1},			//English (Jamaica) 
	{0x2409, 1},			//English (Caribbean) 
	{0x2809, 1},			//English (Belize) 
	{0x2C09, 1},			//English (Trinidad) 

	{0x0404, 2},			//Chinese (Taiwan Region) 
	{0x0804, 2},			//Chinese (PRC) 
	{0x0C04, 2},			//Chinese (Hong Kong SAR, PRC) 
	{0x1004, 2},			//Chinese (Singapore) 

	{0, 0}					//End
};


BOOL	NSys::GetOsLanguage(SYS_LANGUAGE *pOsLang)
{
	int			i;
	LANGID		Lid;

	Lid = GetSystemDefaultLangID();
	if(Lid == 0)
	{
		*pOsLang = Lang_Error;
		return FALSE;
	}

	for(i=0; LID2Index[i][0]; i++)
	{
		if(LID2Index[i][0] == Lid)
		{
			*pOsLang = (SYS_LANGUAGE)LID2Index[i][1];
			return TRUE;
		}
	}

	*pOsLang = Lang_UnKnow;
	return TRUE;
}


HANDLE	NSys::QucikRunThread(PTHREAD_START_ROUTINE pThread, VOID *pParam, DWORD *pdwThdId)
{
	HANDLE		hThread;
	DWORD		dwTid;

	hThread = CreateThread(NULL, 0, pThread, pParam, 0, &dwTid);
	if(pdwThdId)
		*pdwThdId = dwTid;

	return hThread;
}


BOOL	NSys::ThreadOneceRun(PTHREAD_START_ROUTINE ThreadProc, HANDLE &hThread, VOID *pParam)
{
	BOOL		bRetVal;
	DWORD		dwExitCode;

	dwExitCode = 0;
	bRetVal = GetExitCodeThread(hThread, &dwExitCode);
	if(bRetVal == TRUE && STILL_ACTIVE == dwExitCode)
	{
		return FALSE;
	}

	CloseHandle(hThread);

	hThread = NSys::QucikRunThread(ThreadProc, pParam);
	return TRUE;
}


DWORD WINAPI	QucikRunThreadProc(LPVOID lpThreadParameter)
{
	FARPROC		pThread;
	DWORD		dwRetVal;

	memcpy(&pThread, lpThreadParameter, sizeof(pThread));
	free(lpThreadParameter);


	dwRetVal = (DWORD)pThread();

	return dwRetVal;
}


HANDLE	NSys::QucikRunThread(FARPROC pThread, DWORD *pdwThdId)
{
	HANDLE		hThread;
	DWORD		dwTid;
	VOID *		pParam;

	pParam = malloc(sizeof(pThread));
	memcpy(pParam, &pThread, sizeof(pThread));

	hThread = CreateThread(NULL, 0, QucikRunThreadProc, pParam, 0, &dwTid);
	if(pdwThdId)
		*pdwThdId = dwTid;

	return hThread;
}


VOID *	NSys::InvokeFunctionArg(VOID* pFun, BOOL bWinAPi, int nFunParams, ...)
{
	int			i;
	DWORD		dwRetVal, dwParam[10];
	DWORD		dwTemp, dwEsp;
	va_list		marker;

	dwEsp = nFunParams*sizeof(DWORD);

	va_start(marker, nFunParams);
	pFun = (VOID *)va_arg(marker, VOID *);

	for(i=0; i<nFunParams; i++)
	{
		dwParam[i] = va_arg(marker, DWORD);
	}

	if(bWinAPi)
	{
		for(i=nFunParams-1; i>=0; i--)	//²ÎÊýÑ¹Õ»ÊÇµ¹ÐòµÄ
		{
			dwTemp = dwParam[i];
			__asm push dwTemp
		}

		__asm
		{
			call pFun
			mov dwRetVal, eax
		}
	}
	else
	{
		for(i=nFunParams-1; i>=0; i--)	//²ÎÊýÑ¹Õ»ÊÇµ¹ÐòµÄ
		{
			dwTemp = dwParam[i];
			__asm push dwTemp
		}
		__asm
		{
			call pFun
			mov dwRetVal, eax
			add esp, dwEsp			//CÐÎÊ½º¯ÊýÐèÒª»Ö¸´Õ»
		}
	}

	va_end(marker);

	return (VOID *)(__int64)dwRetVal;
}


VOID *	NSys::InvokeFunction(VOID* pFun, BOOL bWinAPi, int nFunParams, VOID *ppParmas[], double *pDoubleRet)
{
	int			i;
	VOID *		RetVal;
	DWORD		dwEsp;
	VOID *		pTemp;
	double		dRetVal;

	dwEsp = nFunParams*sizeof(DWORD);
	if(bWinAPi)
	{
		for(i=nFunParams-1; i>=0; i--)	//²ÎÊýÑ¹Õ»ÊÇµ¹ÐòµÄ
		{
			pTemp = ppParmas[i];
			__asm push pTemp;
		}

		__asm
		{
			call	pFun;
			fstp	dRetVal;
			mov		RetVal, eax;
		}
	}
	else
	{
		for(i=nFunParams-1; i>=0; i--)	//²ÎÊýÑ¹Õ»ÊÇµ¹ÐòµÄ
		{
			pTemp = ppParmas[i];
			__asm push pTemp;
		}
		__asm
		{
			call	pFun;
			add		esp, dwEsp;			//CÐÎÊ½º¯ÊýÐèÒª»Ö¸´Õ»
			fstp	dRetVal;
			mov		RetVal, eax;
		}
	}

	if(pDoubleRet)
		*pDoubleRet = dRetVal;

	return RetVal;
}


DWORDLONG	NSys::GetPerformanceCount(DWORDLONG *pFreq)
{
	LARGE_INTEGER		lnValue;

	if(pFreq)
	{
		QueryPerformanceFrequency(&lnValue);
		*pFreq = (DWORDLONG)lnValue.QuadPart;
	}

	QueryPerformanceCounter(&lnValue);

	return (DWORDLONG)lnValue.QuadPart;
}

DWORDLONG	NSys::GetPerformanceTime()
{
	DWORDLONG			ulTime;
	DWORDLONG			ulFreq;
	static	DWORDLONG	s_ulFreq = 0;

	if(s_ulFreq == 0)
        ulTime = GetPerformanceCount(&s_ulFreq);
	else
		ulTime = GetPerformanceCount();

	ulFreq = s_ulFreq / (1000 * 1000);
	ulTime /= ulFreq;

	return ulTime;
}


DWORD	NSys::WaitEvent(CHAR *pEventName, DWORD dwMilliseconds)
{
	HANDLE			hEvent;
	DWORD			dwRetVal;

	hEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, pEventName);
	dwRetVal = WaitForSingleObject(hEvent, dwMilliseconds);
	CloseHandle(hEvent);

	return dwRetVal;
}


BOOL	NSys::WaitOject(HANDLE hMutex, DWORD dwMilliseconds)
{
	DWORD		dwRetVal;

	dwRetVal = WaitForSingleObject(hMutex, dwMilliseconds);
	if(dwRetVal == WAIT_OBJECT_0 || dwRetVal == ERROR_WAIT_NO_CHILDREN)
		return TRUE;

	if(dwRetVal == WAIT_TIMEOUT || dwRetVal == WAIT_FAILED)
		return FALSE;

	assert(0);
	return FALSE;
}


BOOL	NSys::WaitMutex(HANDLE hMutex, DWORD dwMilliseconds)
{
	DWORD		dwRetVal;

	dwRetVal = WaitForSingleObject(hMutex, dwMilliseconds);
	if(dwRetVal == WAIT_OBJECT_0 || dwRetVal == ERROR_WAIT_NO_CHILDREN)
		return TRUE;

	if(dwRetVal == WAIT_TIMEOUT || dwRetVal == WAIT_FAILED)
		return FALSE;

	return FALSE;
}


BOOL	NSys::WaitProcess(DWORD dwProcId, DWORD dwMilliseconds)
{
	DWORD			dwRetVal;
	CAutoHandle		Proc;

	Proc = OpenProcess(SYNCHRONIZE, FALSE, dwProcId);
	if(Proc == NULL)
		return FALSE;

	dwRetVal = WaitForSingleObject(Proc, dwMilliseconds);
	if(dwRetVal == WAIT_OBJECT_0 || dwRetVal == ERROR_WAIT_NO_CHILDREN)
		return TRUE;

	if(dwRetVal == WAIT_TIMEOUT || dwRetVal == WAIT_FAILED)
		return FALSE;

	return FALSE;
}


HMODULE	NSys::GetModuleFromAddress(VOID *pAddr)
{
	MEMORY_BASIC_INFORMATION mbi;
	DWORD		dwRetVal;
	HMODULE		hMod = NULL;
	static	int	s_nCurDat = 0;

	if(pAddr == NULL)
		pAddr = &s_nCurDat;

	dwRetVal = (DWORD)VirtualQuery(pAddr, &mbi, sizeof(mbi));
	if(dwRetVal != 0)
		hMod = (HMODULE) mbi.AllocationBase;

	return	hMod;
}

/*
00000100: 33C9                         xor         ecx,ecx
00000102: 8B1545633546                 mov         edx,[dwParamCountAddr]
00000108: 3BCA                         cmp         ecx,edx
0000010A: 7D16                         jge         000000122  ----- (1)
0000010C: BB78563412                   mov         ebx,012345678 ;
00000111: 90                           nop
00000112: 8BC1                         mov         eax,ecx
00000114: 6BC004                       imul        eax,eax,004 ;"?"
00000117: 03C3                         add         eax,ebx
00000119: 8B00                         mov         eax,[eax]
0000011B: 50                           push        eax
0000011C: 41                           inc         ecx
0000011D: E9E0FFFFFF                   jmp         000000102  ----- (2)
00000122: E80D535364                   call        dwCallFunAddr  -----X
00000127: A363F3FF0F                   mov         [dwRetValAddr],eax
0000012C: E8CEFEFFFF                   call        dwErrFunAddr  -----X
00000131: A363FFFFC2                   mov         [dwErrValAddr],eax
00000136: C20400                       retn        00004 ;"?"
00000139: CC                           int 3
*/


BOOL NSys::CreateRemoteFunCode(BYTE * pCode, DWORD dwCodeAdr, REMOTEFUNINFO * pInfo)
{
	DWORD		dwSize = 1024;
	DWORD		dwParamCountAddr;
	DWORD		dwParamsAddr;
	DWORD		dwCallFunAddr;
	DWORD		dwRetValAddr;
	DWORD		dwErrFunAddr;
	DWORD		dwErrValAddr;

	dwParamCountAddr = dwCodeAdr + 0x05 + 0x01*4;
	dwParamsAddr = dwCodeAdr + 0x05 + 0x5*4;
	dwCallFunAddr = (DWORD)pInfo->pFun - (dwCodeAdr + 0x127);
	dwRetValAddr = dwCodeAdr + 0x05 + 0x02*4;
	dwErrFunAddr = (DWORD)pInfo->pGetLastError - (dwCodeAdr + 0x131);
	dwErrValAddr = dwCodeAdr + 0x05 + 0x04*4;

	memset(pCode, 0xCC, dwSize);
	memcpy(pCode+0x05, pInfo, sizeof(REMOTEFUNINFO));


	*( BYTE *)(pCode + 0x00) = 0xE9;				//Jmp
	*(DWORD *)(pCode + 0x01) = 0x000000FB;			//Jmps  100

	*( WORD *)(pCode + 0x100) = 0xC933;				//xor   ecx, ecx
	*( WORD *)(pCode + 0x102) = 0x158B;				//mov   edx, []
	*(DWORD *)(pCode + 0x104) = dwParamCountAddr;	//mov   edx, [dwParamCountAddr]
	*( WORD *)(pCode + 0x108) = 0xCA3B;				//cmp   ecx, edx
	*( WORD *)(pCode + 0x10A) = 0x167D;				//jge   000000122
	*( BYTE *)(pCode + 0x10C) = 0xBB;				//mov   ebx,
	*(DWORD *)(pCode + 0x10D) = dwParamsAddr;		//mov   ebx, dwParamsAddr
	*( BYTE *)(pCode + 0x111) = 0x90;				//nop
	*( WORD *)(pCode + 0x112) = 0xC18B;				//mov   eax,ecx
	*( WORD *)(pCode + 0x114) = 0xC06B;				//imul  eax,eax
	*( WORD *)(pCode + 0x116) = 0x04;				//imul  eax,eax,004
	*( WORD *)(pCode + 0x117) = 0xC303;				//add   eax,ebx
	*( WORD *)(pCode + 0x119) = 0x008B;				//mov   eax,[eax]
	*( WORD *)(pCode + 0x11B) = 0x50;				//push  eax
	*( WORD *)(pCode + 0x11C) = 0x41;				//inc   ecx
	*( WORD *)(pCode + 0x11D) = 0xE9;				//jmp
	*(DWORD *)(pCode + 0x11E) = 0xFFFFFFE0;			//jmp   000000102
	*( WORD *)(pCode + 0x122) = 0xE8;				//call
	*(DWORD *)(pCode + 0x123) = dwCallFunAddr;		//call  dwCallFunAddr
	*( WORD *)(pCode + 0x127) = 0xA3;				//mov   [],eax
	*(DWORD *)(pCode + 0x128) = dwRetValAddr;		//mov   [dwRetValAddr],eax
	*( WORD *)(pCode + 0x12C) = 0xE8;				//call
	*(DWORD *)(pCode + 0x12D) = dwErrFunAddr;		//call  dwErrFunAddr
	*( WORD *)(pCode + 0x131) = 0xA3;				//mov   [],eax
	*(DWORD *)(pCode + 0x132) = dwErrValAddr;		//mov   [dwErrValAddr],eax
	*( WORD *)(pCode + 0x136) = 0xC2;				//retn
	*( WORD *)(pCode + 0x137) = 0x0004;				//retn  00004

	return TRUE;
}




/*
00000100: 33C9                         xor         ecx,ecx
00000102: 8B1500000000                 mov         edx,[00000000]
00000108: 3BCA                         cmp         ecx,edx
0000010A: 7D16                         jge         000000122  ----- (1)
0000010C: BBFFFFFFFF                   mov         ebx,0FFFFFFFF ;"ÿÿÿÿ"
00000111: 90                           nop
00000112: 8BC1                         mov         eax,ecx
00000114: 6BC004                       imul        eax,eax,004 ;""
00000117: 03C3                         add         eax,ebx
00000119: 8B00                         mov         eax,[eax]
0000011B: 50                           push        eax
0000011C: 41                           inc         ecx
0000011D: E900000000                   jmp         000000122  ----- (2)
00000122: E800000000                   call        000000127  ----- (3)	dwCallFunAddr
00000127: 83C400                       add         esp,000 ;" "
0000012A: A300000000                   mov         [00000000],eax			dwRetValAddr
0000012F: E800000000                   call        000000134  ----- (4)	dwErrFunAddr
00000134: A300000000                   mov         [00000000],eax			dwErrValAddr
00000139: C20400                       retn        00004 ;" "
0000013C: CC                           int 3
*/
BOOL	NSys::CreateRemoteFunCodeC(BYTE * pCode, DWORD dwCodeAdr, REMOTEFUNINFO * pInfo)
{
	DWORD		dwSize = 1024;
	DWORD		dwParamCountAddr;
	DWORD		dwParamsAddr;
	DWORD		dwCallFunAddr;
	DWORD		dwRetValAddr;
	DWORD		dwErrFunAddr;
	DWORD		dwErrValAddr;
	BYTE		bEspAdd;

	dwParamCountAddr = dwCodeAdr + 0x05 + 0x01*4;
	dwParamsAddr = dwCodeAdr + 0x05 + 0x5*4;
	dwCallFunAddr = (DWORD)pInfo->pFun - (dwCodeAdr + 0x127);
	dwRetValAddr = dwCodeAdr + 0x05 + 0x02*4;
	dwErrFunAddr = (DWORD)pInfo->pGetLastError - (dwCodeAdr + 0x134);
	dwErrValAddr = dwCodeAdr + 0x05 + 0x04*4;
	bEspAdd = (BYTE)(pInfo->dwParams * sizeof(DWORD));

	memset(pCode, 0xCC, dwSize);
	memcpy(pCode+0x05, pInfo, sizeof(REMOTEFUNINFO));


	*( BYTE *)(pCode + 0x00) = 0xE9;				//Jmp
	*(DWORD *)(pCode + 0x01) = 0x000000FB;			//Jmps  100

	*( WORD *)(pCode + 0x100) = 0xC933;				//xor   ecx, ecx
	*( WORD *)(pCode + 0x102) = 0x158B;				//mov   edx, []
	*(DWORD *)(pCode + 0x104) = dwParamCountAddr;	//mov   edx, [dwParamCountAddr]
	*( WORD *)(pCode + 0x108) = 0xCA3B;				//cmp   ecx, edx
	*( WORD *)(pCode + 0x10A) = 0x167D;				//jge   000000122
	*( BYTE *)(pCode + 0x10C) = 0xBB;				//mov   ebx,
	*(DWORD *)(pCode + 0x10D) = dwParamsAddr;		//mov   ebx, dwParamsAddr
	*( BYTE *)(pCode + 0x111) = 0x90;				//nop
	*( WORD *)(pCode + 0x112) = 0xC18B;				//mov   eax,ecx
	*( WORD *)(pCode + 0x114) = 0xC06B;				//imul  eax,eax
	*( WORD *)(pCode + 0x116) = 0x04;				//imul  eax,eax,004
	*( WORD *)(pCode + 0x117) = 0xC303;				//add   eax,ebx
	*( WORD *)(pCode + 0x119) = 0x008B;				//mov   eax,[eax]
	*( WORD *)(pCode + 0x11B) = 0x50;				//push  eax
	*( WORD *)(pCode + 0x11C) = 0x41;				//inc   ecx
	*( WORD *)(pCode + 0x11D) = 0xE9;				//jmp
	*(DWORD *)(pCode + 0x11E) = 0xFFFFFFE0;			//jmp   000000102
	*( WORD *)(pCode + 0x122) = 0xE8;				//call
	*(DWORD *)(pCode + 0x123) = dwCallFunAddr;		//call  dwCallFunAddr
	*(WORD  *)(pCode + 0x127) = 0xC483;				//Add	esp, 
	*(BYTE  *)(pCode + 0x129) = bEspAdd;			//Add	esp, bEspAdd

	*( WORD *)(pCode + 0x12A) = 0xA3;				//mov   [],eax
	*(DWORD *)(pCode + 0x12B) = dwRetValAddr;		//mov   [dwRetValAddr],eax
	*( WORD *)(pCode + 0x12F) = 0xE8;				//call
	*(DWORD *)(pCode + 0x130) = dwErrFunAddr;		//call  dwErrFunAddr
	*( WORD *)(pCode + 0x134) = 0xA3;				//mov   [],eax
	*(DWORD *)(pCode + 0x135) = dwErrValAddr;		//mov   [dwErrValAddr],eax
	*( WORD *)(pCode + 0x139) = 0xC2;				//retn
	*( WORD *)(pCode + 0x13A) = 0x0004;				//retn  00004
	*( BYTE *)(pCode + 0x13C) = 0xCC;				//int 3

	return TRUE;
}




#include "CProcessMem.h"

BOOL NSys::RemotProcRun(DWORD dwPid, VOID *pFun, VOID *pParams, DWORD dwParams, int *pRetVal, BOOL bWinApi, HANDLE hThreadToken)
{
	REMOTEFUNINFO		Info;
	BYTE				bzCode[1024];
	CProcessMem			PMem;
	HANDLE				hThread;
	DWORD				dwFlag;

	memset(&Info, 0, sizeof(Info));

	Info.pFun = pFun;
	Info.pGetLastError = (VOID *)(DWORD)&GetLastError;
	Info.dwParams = dwParams;
	memcpy(Info.pParams, pParams, sizeof(VOID *)*dwParams);


	PMem.OpenAndAlloc(dwPid, 1024);
	if(bWinApi)
        CreateRemoteFunCode(bzCode, (DWORD)PMem.GetData(), &Info);
	else
		CreateRemoteFunCodeC(bzCode, (DWORD)PMem.GetData(), &Info);
	PMem.Wirte(bzCode, 1024);

	if(hThreadToken != NULL)
		dwFlag = CREATE_SUSPENDED;
	else
		dwFlag = 0;

	hThread = ::CreateRemoteThread(PMem.GetHandle(), 0, 0, 
		(LPTHREAD_START_ROUTINE)PMem.GetData(), 0, dwFlag, NULL);
	if(hThread == NULL)
	{
		return FALSE;
	}

	if(hThreadToken != NULL)
	{
		SetThreadToken(&hThread, hThreadToken);
		ResumeThread(hThread);
	}

	while(1)
	{
		Sleep(1);
		if(WaitForSingleObject(hThread, 10) != WAIT_TIMEOUT)
			break;
	}


	CloseHandle(hThread);

	PMem.Read(bzCode, 1024);
	memcpy(&Info, bzCode+0x05, sizeof(REMOTEFUNINFO));
	SetLastError(Info.nErrorCode);

	if(pRetVal)
		*pRetVal = Info.nFunRetVal;

	return TRUE;
}


BOOL	NSys::RemotProcLoadLibary(DWORD dwPid, CHAR *pDllPath)
{
	VOID *			pFun;
	VOID *			Params[2];
	int				nRetVal;
	BOOL			bRetVal;
	CProcessMem		PMem;

	pFun = GetProcAddress(GetModuleHandleA("Kernel32"), "LoadLibraryA");
	if(pFun == NULL)
		return FALSE;

	PMem.OpenAndAlloc(dwPid, (DWORD)strlen(pDllPath)+1, pDllPath);

	Params[0] = PMem.GetData();
	bRetVal = RemotProcRun(dwPid, pFun, Params, 1, &nRetVal);
	if(bRetVal == FALSE)
		return FALSE;

	if(nRetVal == 0)
		return FALSE;

	return TRUE;
}


BOOL	NSys::ProcCloseHandle(DWORD dwPid, HANDLE hHandle)
{
	VOID *		pFun;
	VOID *		Params[2];
	int			nRetVal;
	BOOL		bRetVal;

	pFun = GetProcAddress(GetModuleHandleA("Kernel32"), "CloseHandle");
	if(pFun == NULL)
		return FALSE;

	Params[0] = hHandle;
	bRetVal = RemotProcRun(dwPid, pFun, Params, 1, &nRetVal);
	if(bRetVal == FALSE)
		return FALSE;

	if(nRetVal == 0)
		return FALSE;

	return TRUE;
}


int		NSys::Trace(CONST CHAR * lpszFormat, ...)
{
	CHAR *			buf = NULL;
	int				nRetVal = -1, nSize = 4096;
	va_list			args;

	va_start(args, lpszFormat);

	for(; nRetVal<0 ; nSize*=2)
	{
		if(buf)	
			free(buf);

		buf = (CHAR *)malloc( (nSize+2)*sizeof(CHAR) );
		nRetVal = _vsnprintf(buf, nSize, lpszFormat, args);
	}

	va_end(args);

	OutputDebugStringA(buf);

	if(buf)	
		free(buf);
	return nRetVal;
}


int		NSys::DebugOut(CONST CHAR * lpszFormat, ...)
{
	static	BOOL	bConsol = FALSE;

	if(bConsol == FALSE)
	{
		bConsol = NSys::ActiveConsole();
		if(bConsol == FALSE)
		{
			AllocConsole();
			freopen("CONOUT$","w+t",stdout); 
			freopen("CONIN$","r+t",stdin); 
		}
		bConsol = TRUE;
	}

	CHAR *			buf = NULL;
	int				nRetVal = -1, nSize = 4096;
	va_list			args;

	va_start(args, lpszFormat);

	for(; nRetVal<0 ; nSize*=2)
	{
		if(buf)	
			free(buf);

		buf = (CHAR *)malloc( (nSize+2)*sizeof(CHAR) );
		nRetVal = _vsnprintf(buf, nSize, lpszFormat, args);
	}

	va_end(args);

	puts(buf);

	if(buf)	
		free(buf);
	return nRetVal;
}


typedef	struct	_RegisterSystemObject_Data
{
	HANDLE		hFileMap;
	DWORD		dwDataSize;
	BYTE		bzStoreData[ANY_SIZE];
}RegisterSystemObject_Data;


//	¸ù¾ÝÎ¨Ò»×Ö·ûÃû pUniqName, °ÑObject×¢²á³ÉÈ«ÏµÍ³¿ÉÓÃ
BOOL	NSys::RegisterSystemObject(CHAR *pUniqName, VOID *pObject, DWORD dwObjectSize)
{
	RegisterSystemObject_Data *		pMapData;
	CHAR		szFileMapName[1024];
	HANDLE		hFileMap;
	DWORD		dwNeedSize;
	BOOL		bFirst;

	dwNeedSize = sizeof(RegisterSystemObject_Data) + dwObjectSize;
	sprintf(szFileMapName, "Global\\RegisterSystemObject_FileMap_%s", pUniqName);

	hFileMap = OpenFileMappingA(FILE_MAP_WRITE, 0, szFileMapName);
	if(hFileMap != NULL)
	{
		bFirst = FALSE;
		goto GetFileMapDone;
	}

	bFirst = TRUE;
	hFileMap = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, 
		PAGE_READWRITE, 0, dwNeedSize, szFileMapName);
	if(hFileMap == NULL)
	{
		return FALSE;
	}

GetFileMapDone:

	pMapData = (RegisterSystemObject_Data *)MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0, 0, dwNeedSize);
	if(pMapData == NULL)
	{
		CloseHandle(hFileMap);
		return FALSE;
	}

	if(bFirst)
        pMapData->hFileMap = hFileMap;

	pMapData->dwDataSize = dwObjectSize;
	memcpy(pMapData->bzStoreData, pObject, dwObjectSize);

	UnmapViewOfFile(pMapData);

	if(!bFirst)
		CloseHandle(hFileMap);

	return TRUE;
}


BOOL	NSys::GetRegistedSystemObject(CHAR *pUniqName, VOID *pObject, DWORD dwObjectSize)
{
	RegisterSystemObject_Data *		pMapData;
	CHAR		szFileMapName[1024];
	HANDLE		hFileMap;
	DWORD		dwNeedSize;

	dwNeedSize = sizeof(RegisterSystemObject_Data) + dwObjectSize;
	sprintf(szFileMapName, "Global\\RegisterSystemObject_FileMap_%s", pUniqName);

	hFileMap = OpenFileMappingA(FILE_MAP_WRITE, 0, szFileMapName);
	if(hFileMap == NULL)
	{
		return FALSE;
	}

	pMapData = (RegisterSystemObject_Data *)MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0, 0, dwNeedSize);
	if(pMapData == NULL)
	{
		CloseHandle(hFileMap);
		return FALSE;
	}

	if(pMapData->dwDataSize != dwObjectSize)
	{
		UnmapViewOfFile(pMapData);
		CloseHandle(hFileMap);
		return FALSE;
	}

	memcpy(pObject, pMapData->bzStoreData, dwObjectSize);

	UnmapViewOfFile(pMapData);
	CloseHandle(hFileMap);

	return TRUE;
}

BOOL	NSys::UnRegisterSystemObject(CHAR *pUniqName)
{
	RegisterSystemObject_Data *		pMapData;
	CHAR		szFileMapName[1024];
	HANDLE		hFileMap;
	DWORD		dwNeedSize;

	dwNeedSize = sizeof(RegisterSystemObject_Data);
	sprintf(szFileMapName, "Global\\RegisterSystemObject_FileMap_%s", pUniqName);

	hFileMap = OpenFileMappingA(FILE_MAP_WRITE, 0, szFileMapName);
	if(hFileMap == NULL)
	{
		return FALSE;
	}

	pMapData = (RegisterSystemObject_Data *)MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0, 0, dwNeedSize);
	if(pMapData == NULL)
	{
		CloseHandle(hFileMap);
		return FALSE;
	}

	CloseHandle(pMapData->hFileMap);

	UnmapViewOfFile(pMapData);
	CloseHandle(hFileMap);

	return TRUE;
}


BOOL	NSys::RegisterProcessObject(CHAR *pUniqName, VOID *pObject, DWORD dwObjectSize)
{
	CHAR	szName[1024];
	DWORD	dwProcId;
	BOOL	bRetVal;

	dwProcId = GetCurrentProcessId();
	sprintf(szName, "Proc%04X_%s", dwProcId, pUniqName);

	bRetVal = RegisterSystemObject(szName, pObject, dwObjectSize);

	return bRetVal;
}

BOOL	NSys::GetRegistedProcessObject(CHAR *pUniqName, VOID *pObject, DWORD dwObjectSize)
{
	CHAR	szName[1024];
	DWORD	dwProcId;
	BOOL	bRetVal;

	dwProcId = GetCurrentProcessId();
	sprintf(szName, "Proc%04X_%s", dwProcId, pUniqName);

	bRetVal = GetRegistedSystemObject(szName, pObject, dwObjectSize);

	return bRetVal;
}

BOOL	NSys::UnRegisterProcessObject(CHAR *pUniqName)
{
	CHAR	szName[1024];
	DWORD	dwProcId;
	BOOL	bRetVal;

	dwProcId = GetCurrentProcessId();
	sprintf(szName, "Proc%04X_%s", dwProcId, pUniqName);

	bRetVal = UnRegisterSystemObject(szName);

	return bRetVal;
}


BOOL	NSys::GetProcCommandLine(DWORD dwProcId, CHAR *pBuffer, int nBufSize)
{
	HANDLE		hProc;
	HMODULE		hMod;
	DWORD		dwAddr,dwReadSize;
	CHAR		szCmdLine[4096];
	BYTE		bzBuf[100];
	BOOL		bRetVal;
	BOOL		bResult;


	bResult = FALSE;
	hProc = NULL;
	szCmdLine[0] = 0;

	hProc = OpenProcess(PROCESS_VM_READ, FALSE, dwProcId);
	if(hProc == NULL)
		goto GetProcCommandLineEnd;

	hMod = GetModuleHandleA("kernel32.dll");
	dwAddr = (DWORD)GetProcAddress(hMod, "GetCommandLineA");

	bRetVal = ::ReadProcessMemory(hProc, (LPVOID)dwAddr, bzBuf, 5, &dwReadSize);
	if(bRetVal == FALSE || dwReadSize != 5)
		goto GetProcCommandLineEnd;

	if(bzBuf[0] != 0xA1 )	//°æ±¾²»¼æÈÝ
		goto GetProcCommandLineEnd;

	//0xa1 : Mov EAX,
	dwAddr = *(DWORD *)(&bzBuf[1]);

	bRetVal = ::ReadProcessMemory(hProc, (LPVOID)dwAddr, bzBuf, 4, &dwReadSize);
	if(bRetVal == FALSE || dwReadSize != 4)
		goto GetProcCommandLineEnd;

	memset(szCmdLine, 0, sizeof(szCmdLine));

	dwAddr = *(DWORD *)(&bzBuf[0]);
	bRetVal = ::ReadProcessMemory(hProc, (LPVOID)dwAddr, szCmdLine, sizeof(szCmdLine), &dwReadSize);
	if(bRetVal == FALSE || dwReadSize <= 0)
		goto GetProcCommandLineEnd;

	bResult = TRUE;

GetProcCommandLineEnd:
	if(hProc)
		CloseHandle(hProc);

	NStr::StrCopy(pBuffer, szCmdLine, nBufSize);
	return bResult;
}


BOOL	NSys::ReadDiskSector(__int64 nSector, BYTE *pBuffer, int nDiskNub)
{
	CHAR		szDriverName[MAXPATH];
	HANDLE		hFile;
	DWORD		dwReadSize;
	DWORD		dwRetVal;
	BOOL		bResult;

	LARGE_INTEGER	llOffset;

	bResult = FALSE;

	sprintf(szDriverName, "\\\\.\\PhysicalDrive%d", nDiskNub);
	hFile = CreateFileA(szDriverName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	llOffset.QuadPart = nSector * 0x200;
	dwRetVal = SetFilePointer(hFile, llOffset.LowPart, &llOffset.HighPart, FILE_BEGIN);
	if(dwRetVal == INVALID_SET_FILE_POINTER)
		goto ReadDiskSectorDone;

	bResult = ReadFile(hFile, pBuffer, 0x200, &dwReadSize, NULL);
	if(bResult && dwReadSize != dwReadSize)
		bResult = FALSE;

ReadDiskSectorDone:

	CloseHandle(hFile);
	return bResult;
}


//	½ø³Ì¼Ì³ÐµÄ·½Ê½µ¼ÖÂ¾ä±ú, ¶Ë¿Ú»á±»Õ¼ÓÃ
//	·Ç¼Ì³Ð·½Ê½ SetStdHandle, ÐÞ¸ÄPEB 
//	ÆäËûÒ»Ð©½ø³ÌÓÐÐ§, ¶Ôcmd.exeÎÞÐ§
//	
BOOL	NSys::CatchAppInterface(CHAR *pCmLine, HANDLE &hProc, HANDLE& hInput, HANDLE& hOut, BOOL bShow)
{
	SECURITY_ATTRIBUTES	sa;
	HANDLE				hShellRead, hInputPipe;
	HANDLE				hShellWrite, hOutPipe;
	STARTUPINFOA		si;
	PROCESS_INFORMATION	pi;
	BOOL				bRetVal;
	BOOL				bResult;

	bResult = FALSE;

	hShellRead = NULL;
	hInputPipe = NULL;
	hShellWrite = NULL;
	hOutPipe = NULL;

	memset(&pi, 0, sizeof(pi));

	sa.nLength              = sizeof(sa);
	sa.bInheritHandle       = TRUE;			//	CreateProcessA ¼Ì³ÐÊ±ÐèÒªÒ² ¼Ì³Ð
	sa.lpSecurityDescriptor = NULL;

	if(CreatePipe(&hShellRead, &hInputPipe, &sa, 0)==0)              
	{
		assert(0);
		goto CatchAppInterface_Done;
	}

	if(CreatePipe(&hOutPipe, &hShellWrite, &sa, 0)==0)                
	{
		assert(0);
		goto CatchAppInterface_Done;
	}

	memset(&si, 0, sizeof(si));
	si.cb           = sizeof(si);
	si.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
 	si.hStdInput    = hShellRead;
 	si.hStdOutput   = hShellWrite;
 	si.hStdError    = hShellWrite;
	if(bShow == FALSE)
        si.wShowWindow  = SW_HIDE;
	else
        si.wShowWindow  = SW_SHOW;

	bRetVal = CreateProcessA(NULL, pCmLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if(bRetVal == FALSE)
	{
		assert(0);
		goto CatchAppInterface_Done;
	}

	hProc = pi.hProcess;
	hInput = hInputPipe;
	hOut = hOutPipe;
	bResult = TRUE;

CatchAppInterface_Done:
	if(pi.hThread)
        CloseHandle(pi.hThread);
	if(hShellRead)
        CloseHandle(hShellRead);
	if(hShellWrite)
        CloseHandle(hShellWrite);

	if(bResult == FALSE)
	{
		if(hInputPipe)
			CloseHandle(hInputPipe);
		if(hOutPipe)
			CloseHandle(hOutPipe);
	}

	return bResult;
}


BOOL	NSys::CatchAppInterfaceClose(HANDLE& hProc, HANDLE& hInput, HANDLE& hOut)
{
	TerminateProcess(hProc, -1);

	CloseHandle(hProc);
	CloseHandle(hInput);
	CloseHandle(hOut);

	hProc = NULL;
	hInput = NULL;
	hOut = NULL;

	return TRUE;
}



BOOL	NSys::CloseHandleSafe(HANDLE &hObj)
{
	BOOL	bRetVal;

	bRetVal = TRUE;

	if(hObj != NULL && hObj != INVALID_HANDLE_VALUE)
		bRetVal = CloseHandle(hObj);
	hObj = NULL;

	return bRetVal;
}


BOOL	NSys::TerminateProcTree(DWORD dwProcId)
{
	std::map<DWORD, BOOL>	ProcList;
	std::map<DWORD, BOOL>::iterator	Iter;
	BOOL					bRetVal;
	HANDLE					hProcShot;
	PROCESSENTRY32			ProcEntry;


	hProcShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcShot == INVALID_HANDLE_VALUE)
		return FALSE;

	ProcEntry.dwSize = sizeof(ProcEntry);
	bRetVal = Process32First(hProcShot, &ProcEntry);
	if(bRetVal == FALSE)
		return FALSE;

	ProcList[dwProcId] = FALSE;
	for(;;)
	{
		Iter = ProcList.find(ProcEntry.th32ParentProcessID);
		if(Iter != ProcList.end())
		{
			ProcList[ProcEntry.th32ProcessID] = 0;
		}

		bRetVal = Process32Next(hProcShot, &ProcEntry);
		if(bRetVal == FALSE)
			break;
	}
	CloseHandle(hProcShot);

	for(Iter = ProcList.begin();
		Iter != ProcList.end();
		Iter ++)
	{
		Iter->second = KillProc(Iter->first);
	}

	return TRUE;
}


BOOL	NSys::WaitForNewWindow(CHAR *pClassName, CHAR * pWindowName, DWORD dwMilliseconds)
{
	HWND		hWnd;
	DWORD		dwTickEnd;
	DWORD		dwTick;

	dwTickEnd = GetTickCount() + dwMilliseconds;
	for(;;)
	{
		hWnd = FindWindowA(pClassName, pWindowName);
		if(hWnd != NULL)
			return TRUE;

		dwTick = GetTickCount();
		if(dwTick > dwTickEnd)
			break;
	}

	return FALSE;
}


HFONT	NSys::QuickCreateFont(CHAR * pFontName, double dFontSize, BOOL bBold/* = FALSE*/)
{
	int		fnWeight, nFontSize;
	HDC		hDC;
	int		nDpi;
	hDC = ::GetDC(NULL);
	nDpi = GetDeviceCaps(hDC, LOGPIXELSY);
	::ReleaseDC(NULL, hDC);
	nFontSize = (int)(dFontSize * nDpi / 72);

	if(bBold)
		fnWeight = FW_BOLD;
	else
		fnWeight = FW_NORMAL;

	return CreateFontA(nFontSize, 
		0 , 0 , 0 , fnWeight , FALSE , FALSE , FALSE ,ANSI_CHARSET , 
		OUT_DEFAULT_PRECIS ,CLIP_DEFAULT_PRECIS , DEFAULT_QUALITY ,
		DEFAULT_PITCH | FF_DONTCARE, pFontName);
}


HFONT	NSys::CreateFixedFont(CHAR * pFontName, double dFontSize, BYTE bCharSet/* = GB2312_CHARSET*/, BOOL bBold/* = FALSE*/)
{
	int		fnWeight, nFontSize;
	HDC		hDC;
	int		nDpi;
	hDC = ::GetDC(NULL);
	nDpi = GetDeviceCaps(hDC, LOGPIXELSY);
	::ReleaseDC(NULL, hDC);
	nFontSize = (int)(dFontSize * nDpi / 72);

	if(bBold)
		fnWeight = FW_BOLD;
	else
		fnWeight = FW_NORMAL;

	return CreateFontA(nFontSize, 
		0, 0, 0, fnWeight, FALSE, FALSE, FALSE, bCharSet, 
		OUT_DEFAULT_PRECIS ,CLIP_DEFAULT_PRECIS , DEFAULT_QUALITY ,
		FIXED_PITCH, pFontName);
}


DWORD	NSys::GetDiskVolumeSn(CHAR *pVolume/* = "C:\\"*/)
{
	DWORD	dwVolumeSn = 0;

	GetVolumeInformationA(pVolume, NULL, 0, &dwVolumeSn, NULL, NULL, NULL, 0);

	return dwVolumeSn;
}


BOOL	NSys::GetDiskVolName(CHAR *pVolName, CHAR *pVolume)
{
	BOOL	bRetVal;

	bRetVal = GetVolumeInformationA(pVolume, pVolName, 0, NULL, NULL, NULL, NULL, 0);

	return bRetVal;
}

DWORD	NSys::GetDaysByTime(SYSTEMTIME &st)
{
	FILETIME		ft;

	SystemTimeToFileTime(&st, &ft); 

	return GetDaysByTime(ft);
}

DWORD	NSys::GetDaysByTime(FILETIME &ft)
{
	ULONGLONG		uuTimes;
	DWORD			dwDays;

	//	100-nanosecond intervals since January 1, 1601.
	uuTimes = ft.dwHighDateTime;
	uuTimes = uuTimes << 32;
	uuTimes += ft.dwLowDateTime;
	uuTimes = uuTimes / 10 / 1000 / 1000;
	uuTimes = uuTimes / 24 / 60 / 60;
	dwDays = (DWORD)uuTimes;

	return dwDays;
}

DWORD	NSys::GetLocalTimeAsDays()
{
	SYSTEMTIME		st;

	GetLocalTime(&st);

	return GetDaysByTime(st);
}


PSID	NSys::GetUserSID(CHAR *pUserName)
{
	CHAR		szUserName[128];
	DWORD		dwSize;
	PSID		pSid = NULL;
	CHAR		szDomainName[128];
	DWORD		dwDomainSize;
	SID_NAME_USE    pNameUse;

	if(pUserName == NULL)
	{
		dwSize = sizeof(szUserName);
		GetUserNameA(szUserName, &dwSize);
		pUserName = szUserName;
	}

	dwDomainSize = sizeof(szDomainName);
	pSid = NULL;
	dwSize = 0; 
	LookupAccountNameA(NULL, pUserName, pSid, &dwSize, szDomainName, &dwDomainSize, &pNameUse); 
	if(dwSize == 0)
	{
		return FALSE;
	}
	pSid = (PSID)malloc(dwSize); 
	LookupAccountNameA(NULL, pUserName, pSid, &dwSize, szDomainName, &dwDomainSize, &pNameUse); 

	return pSid;
}

BOOL	NSys::GetUserSIDString(CHAR *pBuffer, CHAR *pUserName)
{
	PSID		pSid = NULL;
	CHAR *		pSIDString;

	pSid = GetUserSID(pUserName); 
	ConvertSidToStringSidA(pSid, &pSIDString);
	free(pSid);

	strcpy(pBuffer, pSIDString);
	LocalFree(pSIDString);

	return TRUE;
}


DWORD	NSys::GetLsaPrivateData(WCHAR *pKeyName, WCHAR *pData, DWORD dwBufSize)
{
	HANDLE		hLsaHandle;
	LONG		lRetVal;
	DWORD		dwResult;
	int			nError;

	LSA_OBJECT_ATTRIBUTES	LsaObjAttr;
	CMiniMem				MiniMem;
	LSA_UNICODE_STRING		LsaKeyName;
	LSA_UNICODE_STRING		LsaKeyData;
	LSA_UNICODE_STRING *	pLsaKeyData;

	nError = ERROR_SUCCESS;
	dwResult = 0;
	hLsaHandle = NULL;
	memset(&LsaObjAttr, 0, sizeof(LsaObjAttr));
	LsaObjAttr.Length = sizeof(LsaObjAttr);
	LsaOpenPolicy(NULL, &LsaObjAttr, POLICY_ALL_ACCESS, &hLsaHandle);
	if(hLsaHandle == NULL)
		return FALSE;

	LsaKeyName.Buffer = pKeyName;
	LsaKeyName.Length = (USHORT)(wcslen(pKeyName) * sizeof(WCHAR));
	LsaKeyName.MaximumLength = LsaKeyName.Length + 2;

	LsaKeyData.Buffer = pData;
	LsaKeyData.Length = (USHORT)dwBufSize;
	LsaKeyData.MaximumLength = (USHORT)dwBufSize;

	pLsaKeyData = &LsaKeyData;
	lRetVal = LsaRetrievePrivateData(hLsaHandle, &LsaKeyName, &pLsaKeyData);
	if(lRetVal != ERROR_SUCCESS)
	{
		nError = LsaNtStatusToWinError(lRetVal);
		goto GetLsaPrivateData_Done;
	}

	if(dwBufSize > pLsaKeyData->Length)
		dwBufSize = pLsaKeyData->Length;
	memcpy(pData, pLsaKeyData->Buffer, dwBufSize);
	LsaFreeMemory(pLsaKeyData);

	dwResult = dwBufSize;

GetLsaPrivateData_Done:
	if(hLsaHandle != NULL)
		LsaClose(hLsaHandle);
	if(dwResult == 0)
		SetLastError(nError);

	return dwResult;
}


BOOL	NSys::IsVaildPEFile(CHAR *pFilePath, BOOL bDefault)
{
	//	GetBinaryType

	HANDLE			hFile;
	BOOL			bRetVal;
	CHAR			szHead[16];
	DWORD			dwRead;

	hFile = CreateFileA(pFilePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
		NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return bDefault;
	}

	dwRead = 0;
	bRetVal = ReadFile(hFile, szHead, 2, &dwRead, NULL);
	if(bRetVal == FALSE || dwRead != 2)
	{
		CloseHandle(hFile);
		return bDefault;
	}

	CloseHandle(hFile);
	if(szHead[0] != 'M')
		return FALSE;

	if(szHead[1] != 'Z')
		return FALSE;

	return TRUE;
}


BOOL	NSys::ImpersonateLogin(CHAR *pUser, CHAR *pUserPwd, DWORD dwLogonType)
{
	HANDLE			hToken;
	BOOL			bRetVal;

	bRetVal = LogonUserA(pUser, "", pUserPwd, dwLogonType, LOGON32_PROVIDER_DEFAULT, &hToken);
	if(bRetVal == FALSE)
		return FALSE;

	bRetVal = ImpersonateLoggedOnUser(hToken);
	if(bRetVal == FALSE)
		return FALSE;

	return TRUE;
}


BOOL	NSys::RunCmdCathOut(CHAR *pCmdLine, std::string& sOutput, DWORD dwWaitTime, DWORD * pdwExitCode, BOOL bHide)
{
	STARTUPINFOA			si;
	PROCESS_INFORMATION		pi;
	BOOL					bRetVal;
	BOOL					bResult;
	std::string				sResult;
	SECURITY_ATTRIBUTES		sa;
	HANDLE					hRead, hWrite;
	CHAR					szOutBuf[4096];
	DWORD					dwRead;
	DWORD					dwWait;

	sa.nLength              = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle       = TRUE;  // make it inheritable

	bResult = FALSE;
	hRead = NULL;
	hWrite = NULL;

	bRetVal = CreatePipe(&hRead, &hWrite, &sa, 0);
	if(bRetVal == FALSE)
	{
		goto RunCmdCathOut_Done;
	}

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;

	if(bHide)
	{
		si.dwFlags |= STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
	}

	bRetVal = CreateProcessA(NULL, pCmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if(bRetVal == FALSE)
		goto RunCmdCathOut_Done;

	dwWait = WaitForSingleObject(pi.hProcess, dwWaitTime);
	if(dwWait == WAIT_TIMEOUT || dwWait == WAIT_FAILED)
	{
		TerminateProcess(pi.hProcess, 259);
	}

	if(pdwExitCode)
        GetExitCodeProcess(pi.hProcess, pdwExitCode);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	CloseHandle(hWrite);
	hWrite = NULL;

	sOutput = "";
	for(;;)
	{
		szOutBuf[0] = 0;
		bRetVal = ReadFile(hRead, szOutBuf, sizeof(szOutBuf)-1, &dwRead, NULL);
		if(bRetVal == FALSE)
			break;

		szOutBuf[dwRead] = 0;
		sOutput += szOutBuf;
	}

	bResult = TRUE;

RunCmdCathOut_Done:
	if(hWrite)
		CloseHandle(hWrite);
	if(hRead)
		CloseHandle(hRead);

	return bResult;
}


BOOL	NSys::ActiveConsole()
{
	CONSOLE_SCREEN_BUFFER_INFO	bInfo;
	HANDLE				hOut;
	COORD				Pos;
	BOOL				bRetVal;

	typedef	BOOL (WINAPI * Fun_AttachConsole)( IN DWORD dwProcessId);
	Fun_AttachConsole			_AttachConsole;
	_AttachConsole = (Fun_AttachConsole)GetDllProcAdders("Kernel32.dll", "AttachConsole");
	if(_AttachConsole)
	{
		bRetVal = _AttachConsole(-1);
		if(bRetVal == FALSE)
			return FALSE;
	}

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if(hOut == INVALID_HANDLE_VALUE)
		return FALSE;

	freopen("CONOUT$", "a", stdout); // Redirect stdout to console 
	freopen("CONOUT$", "a", stderr); // Redirect stderr to console 

	Pos.X = 0;
	Pos.Y = 0;
	GetConsoleScreenBufferInfo(hOut, &bInfo); 
	FillConsoleOutputCharacter(hOut, ' ', bInfo.dwSize.X * bInfo.dwSize.Y, Pos, NULL);
	SetConsoleCursorPosition(hOut, Pos);

	CloseHandle(hOut);

	return TRUE;
}





/*
#define VK_OEM_NEC_EQUAL  0x92   // '=' key on numpad
#define VK_OEM_FJ_JISHO   0x92   // 'Dictionary' key
#define VK_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
#define VK_OEM_FJ_TOUROKU 0x94   // 'Register word' key
#define VK_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
#define VK_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5
#define VK_BROWSER_BACK        0xA6
#define VK_BROWSER_FORWARD     0xA7
#define VK_BROWSER_REFRESH     0xA8
#define VK_BROWSER_STOP        0xA9
#define VK_BROWSER_SEARCH      0xAA
#define VK_BROWSER_FAVORITES   0xAB
#define VK_BROWSER_HOME        0xAC
#define VK_VOLUME_MUTE         0xAD
#define VK_VOLUME_DOWN         0xAE
#define VK_VOLUME_UP           0xAF
#define VK_MEDIA_NEXT_TRACK    0xB0
#define VK_MEDIA_PREV_TRACK    0xB1
#define VK_MEDIA_STOP          0xB2
#define VK_MEDIA_PLAY_PAUSE    0xB3
#define VK_LAUNCH_MAIL         0xB4
#define VK_LAUNCH_MEDIA_SELECT 0xB5
#define VK_LAUNCH_APP1         0xB6
#define VK_LAUNCH_APP2         0xB7
#define VK_OEM_1          0xBA   // ';:' for US
#define VK_OEM_PLUS       0xBB   // '+' any country
#define VK_OEM_COMMA      0xBC   // ',' any country
#define VK_OEM_MINUS      0xBD   // '-' any country
#define VK_OEM_PERIOD     0xBE   // '.' any country
#define VK_OEM_2          0xBF   // '/?' for US
#define VK_OEM_3          0xC0   // '`~' for US
#define VK_OEM_4          0xDB  //  '[{' for US
#define VK_OEM_5          0xDC  //  '\|' for US
#define VK_OEM_6          0xDD  //  ']}' for US
#define VK_OEM_7          0xDE  //  ''"' for US
#define VK_OEM_8          0xDF
#define VK_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
#define VK_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
#define VK_ICO_HELP       0xE3  //  Help key on ICO
#define VK_ICO_00         0xE4  //  00 key on ICO
#define VK_PROCESSKEY     0xE5
#define VK_ICO_CLEAR      0xE6
#define VK_PACKET         0xE7
#define VK_OEM_RESET      0xE9
#define VK_OEM_JUMP       0xEA
#define VK_OEM_PA1        0xEB
#define VK_OEM_PA2        0xEC
#define VK_OEM_PA3        0xED
#define VK_OEM_WSCTRL     0xEE
#define VK_OEM_CUSEL      0xEF
#define VK_OEM_ATTN       0xF0
#define VK_OEM_FINISH     0xF1
#define VK_OEM_COPY       0xF2
#define VK_OEM_AUTO       0xF3
#define VK_OEM_ENLW       0xF4
#define VK_OEM_BACKTAB    0xF5
#define VK_ATTN           0xF6
#define VK_CRSEL          0xF7
#define VK_EXSEL          0xF8
#define VK_EREOF          0xF9
#define VK_PLAY           0xFA
#define VK_ZOOM           0xFB
#define VK_NONAME         0xFC
#define VK_PA1            0xFD
#define VK_OEM_CLEAR      0xFE
*/

typedef	struct _Key_Name
{
	CHAR	szKeyName[16];
	UINT	uVirKey;
	BOOL	bExt;
}Key_Name;

Key_Name	KeyTable[] = 
{
	{"LBUTTON",			VK_LBUTTON,			FALSE},
	{"RBUTTON",			VK_LBUTTON,			FALSE},
	{"CANCEL",			VK_CANCEL,			FALSE},
	{"MBUTTON",			VK_MBUTTON,			FALSE},
	{"XBUTTON1",		VK_XBUTTON1,		FALSE},
	{"XBUTTON2",		VK_XBUTTON2,		FALSE},
	{"CLEAR",			VK_CLEAR,			FALSE},
	{"CAPITAL",			VK_CAPITAL,			FALSE},
	{"KANA",			VK_KANA,			FALSE},
	{"HANGEUL",			VK_HANGEUL,			FALSE},
	{"HANGUL",			VK_HANGUL,			FALSE},
	{"VK_JUNJA",		VK_JUNJA,			FALSE},
	{"VK_FINAL",		VK_FINAL,			FALSE},
	{"VK_HANJA",		VK_HANJA,			FALSE},
	{"VK_KANJI",		VK_KANJI,			FALSE},
	{"VK_CONVERT",		VK_CONVERT,			FALSE},
	{"VK_NONCONVERT",	VK_NONCONVERT,		FALSE},
	{"VK_ACCEPT",		VK_ACCEPT,			FALSE},
	{"VK_MODECHANGE",	VK_MODECHANGE,		FALSE},
	{"VK_PRINT",		VK_PRINT,			FALSE},
	{"VK_SELECT",		VK_SELECT,			FALSE},
	{"VK_EXECUTE",		VK_EXECUTE,			FALSE},
	{"VK_HELP",			VK_HELP,			FALSE},
	{"VK_SLEEP",		VK_SLEEP,			FALSE},
	{"VK_SEPARATOR",	VK_SEPARATOR,		FALSE},


	{"Backspace",	VK_BACK,			FALSE},
	{"Tab",			VK_TAB,				FALSE},
	{"Enter",		VK_RETURN,			FALSE},
	{"Shift",		VK_SHIFT,			FALSE},
	{"Ctrl",		VK_CONTROL,			FALSE},
	{"Alt",			VK_MENU,			FALSE},
	{"Pause",		VK_PAUSE,			FALSE},
	{"Esc",			VK_ESCAPE,			FALSE},
	{"Space",		VK_SPACE,			FALSE},

	{"Page Up",		VK_PRIOR,			TRUE},
	{"Page Down",	VK_NEXT,			TRUE},
	{"End",			VK_END,				TRUE},
	{"Home",		VK_HOME,			TRUE},
	{"Up",			VK_UP,				TRUE},
	{"Left",		VK_LEFT,			TRUE},
	{"Right",		VK_RIGHT,			TRUE},
	{"Down",		VK_DOWN,			TRUE},
	{"Prnt Scrn",	VK_SNAPSHOT,		TRUE},
	{"Insert",		VK_INSERT,			TRUE},
	{"Delete",		VK_DELETE,			TRUE},
	{"Left Windows",	VK_LWIN,		TRUE},
	{"Right Windows",	VK_RWIN,		TRUE},
	{"Application",		VK_APPS,		TRUE},

//	0-9
//	A-Z
//	a-z

	{"Num 0",		VK_NUMPAD0,			FALSE},
	{"Num 1",		VK_NUMPAD1,			FALSE},
	{"Num 2",		VK_NUMPAD2,			FALSE},
	{"Num 3",		VK_NUMPAD3,			FALSE},
	{"Num 4",		VK_NUMPAD4,			FALSE},
	{"Num 5",		VK_NUMPAD5,			FALSE},
	{"Num 6",		VK_NUMPAD6,			FALSE},
	{"Num 7",		VK_NUMPAD7,			FALSE},
	{"Num 8",		VK_NUMPAD8,			FALSE},
	{"Num 9",		VK_NUMPAD9,			FALSE},
	{"Num *",		VK_MULTIPLY,		FALSE},
	{"Num +",		VK_ADD,				FALSE},
	{"Num -",		VK_SUBTRACT,		FALSE},
	{"Num Del",		VK_DECIMAL,			FALSE},

	{"Num /",		VK_DIVIDE,			TRUE},

	{"F1",		VK_F1,			FALSE},
	{"F2",		VK_F2,			FALSE},
	{"F3",		VK_F3,			FALSE},
	{"F4",		VK_F4,			FALSE},
	{"F5",		VK_F5,			FALSE},
	{"F6",		VK_F6,			FALSE},
	{"F7",		VK_F7,			FALSE},
	{"F8",		VK_F8,			FALSE},
	{"F9",		VK_F9,			FALSE},
	{"F10",		VK_F10,			FALSE},
	{"F11",		VK_F11,			FALSE},
	{"F12",		VK_F12,			FALSE},
	{"F13",		VK_F13,			FALSE},
	{"F14",		VK_F14,			FALSE},
	{"F15",		VK_F15,			FALSE},
	{"F16",		VK_F16,			FALSE},
	{"F17",		VK_F17,			FALSE},
	{"F18",		VK_F18,			FALSE},
	{"F19",		VK_F19,			FALSE},
	{"F20",		VK_F20,			FALSE},
	{"F21",		VK_F21,			FALSE},
	{"F22",		VK_F22,			FALSE},
	{"F23",		VK_F23,			FALSE},
	{"F24",		VK_F24,			FALSE},

	{"Num Lock",	VK_NUMLOCK,		TRUE},
	{"Scroll Lock",	VK_SCROLL,		FALSE},

	{";",		VK_OEM_1,			FALSE},
	{"+",		VK_OEM_PLUS,		FALSE},
	{",",		VK_OEM_COMMA,		FALSE},
	{"-",		VK_OEM_MINUS,		FALSE},
	{".",		VK_OEM_PERIOD,		FALSE},
	{"/",		VK_OEM_2,			FALSE},
	{"`",		VK_OEM_3,			FALSE},
	{"[",		VK_OEM_4,			FALSE},
	{"\\",		VK_OEM_5,			FALSE},
	{"]",		VK_OEM_6,			FALSE},
	{"'",		VK_OEM_7,			FALSE},

	{"",		0,			FALSE}
};

static	std::map<std::string, NSys::KEYINPUT>	s_VkeyMap;


BOOL	InitKeyMap()
{
	std::string			sMapKey;
	NSys::KEYINPUT			OneKey;

	if(s_VkeyMap.size() <= 0)
	{
		int		i;
		CHAR	c;

		for(c='0'; c<='9'; c++)
		{
			memset(&OneKey, 0, sizeof(OneKey));
			OneKey.uVirKey = c;
			OneKey.bExt = FALSE;
			sMapKey = c;
			s_VkeyMap[sMapKey] = OneKey;
		}
		for(c='a'; c<='z'; c++)
		{
			memset(&OneKey, 0, sizeof(OneKey));
			OneKey.uVirKey = c-'a'+'A';
			OneKey.bExt = FALSE;
			sMapKey = c;
			s_VkeyMap[sMapKey] = OneKey;
		}
		for(c='A'; c<='Z'; c++)
		{
			memset(&OneKey, 0, sizeof(OneKey));
			OneKey.uVirKey = c;
			OneKey.bExt = FALSE;
			sMapKey = c;
			s_VkeyMap[sMapKey] = OneKey;
		}
		for(i=0; KeyTable[i].szKeyName[0]; i++)
		{
			memset(&OneKey, 0, sizeof(OneKey));
			OneKey.uVirKey = KeyTable[i].uVirKey;
			OneKey.bExt = KeyTable[i].bExt;
			s_VkeyMap[KeyTable[i].szKeyName] = OneKey;
		}
	}

	return TRUE;
}

BOOL	NSys::GetVKeyByName(CHAR *pTextName, UINT *puVirKey, BOOL *pbExt)
{
	std::map<std::string, KEYINPUT>::iterator	iFind;
	std::string			sMapKey;

	*puVirKey = 0;
	*pbExt = FALSE;

	InitKeyMap();

	sMapKey = pTextName;
	iFind = s_VkeyMap.find(sMapKey);
	if(iFind == s_VkeyMap.end())
		return FALSE;

	*puVirKey = iFind->second.uVirKey;
	*pbExt = iFind->second.bExt;
	return TRUE;
}


BOOL	NSys::GetNameByVKey(UINT uVirKey, BOOL bExt, CHAR *pKeyName)
{
	std::map<std::string, KEYINPUT>::iterator	iFind;
	CHAR		szName[64];

	InitKeyMap();

	szName[0] = 0;
	for(iFind = s_VkeyMap.begin();
		iFind != s_VkeyMap.end();
		iFind ++)
	{
		if(uVirKey == iFind->second.uVirKey)
		{
			strcpy(szName, iFind->first.c_str());

			if(bExt == iFind->second.bExt)
			{
				strcpy(pKeyName, szName);
				return TRUE;
			}
		}
	}
	if(szName[0] == 0)
		assert(0);

	strcpy(pKeyName, szName);
	return FALSE;
}


//	nFlags  0: °´ÏÂ  
//	KEYEVENTF_KEYUP: µ¯ÉÏ  
//	KEYEVENTF_EXTENDEDKEY: À©Õ¹¼ü
BOOL	NSys::InputOneKey(UINT uVirKey, BOOL bExt, BOOL bDown)
{
	DWORD		dwDownlParam;
	UINT		uScanCode;
	DWORD		dwFlag;

	dwFlag = 0;
	if(bDown == FALSE)
		dwFlag |= KEYEVENTF_KEYUP;
	if(bExt)
		dwFlag |= KEYEVENTF_EXTENDEDKEY;

	uScanCode = MapVirtualKeyA(uVirKey, 0);

	dwDownlParam = 0;
	dwDownlParam |= 0x0001;					//0-15	repeat count
	dwDownlParam |= uScanCode << 16;		//16-23
	dwDownlParam |= (dwFlag & 0x0F) <<24;	//24,25

	keybd_event(uVirKey, uScanCode, dwFlag, dwDownlParam);

	return TRUE;
}


BOOL	NSys::InputOneKey(CHAR *pKeyName, BOOL bDown)
{
	UINT		uVirKey;
	BOOL		bExt;
	BOOL		bRetVal;

	bRetVal = GetVKeyByName(pKeyName, &uVirKey, &bExt);
	if(bRetVal == FALSE)
		return FALSE;

	bRetVal = InputOneKey(uVirKey, bExt, bDown);

	return bRetVal;
}


int		NSys::InputKeys(std::vector<KEYINPUT> &vKeyList)
{
	std::vector<KEYINPUT>::iterator		Iter;
	BOOL		bRetVal;
	int			nCount;

	nCount = 0;
	for(Iter = vKeyList.begin();
		Iter != vKeyList.end();
		Iter ++)
	{
		bRetVal = InputOneKey(Iter->uVirKey, Iter->bExt, Iter->bDown);
		if(bRetVal)
			nCount ++;

		Sleep(Iter->dwSleep);
	}
	return nCount;
}


int		NSys::InputString(CHAR *pText, DWORD dwKeySleep)
{
	std::vector<KEYINPUT>		vKeyList;
	int				i;
	UINT			uVirKey;
	KEYINPUT		OneKey;
	int				nCount;

	OneKey.dwSleep = dwKeySleep;

	for(i=0; pText[i]; i++)
	{
		if(i>=1 && pText[i-1] == '\r' && pText[i] == '\n')
			continue;

		uVirKey = VkKeyScan(pText[i]);
		if(uVirKey & 0x0100)
		{
			OneKey.uVirKey = VK_SHIFT;
			OneKey.bExt = FALSE;
			OneKey.bDown = TRUE;
			vKeyList.push_back(OneKey);
		}

		if(uVirKey & 0xFE00)
		{
			assert(0);
		}

		OneKey.uVirKey = uVirKey;
		OneKey.bExt = FALSE;
		OneKey.bDown = TRUE;
		vKeyList.push_back(OneKey);
		OneKey.bDown = FALSE;
		vKeyList.push_back(OneKey);

		if(uVirKey & 0x0100)
		{
			OneKey.uVirKey = VK_SHIFT;
			OneKey.bExt = FALSE;
			OneKey.bDown = FALSE;
			vKeyList.push_back(OneKey);
		}
	}

	nCount = InputKeys(vKeyList);

	return nCount;
}



//	¿É½âÊÍ×éºÏ¼ü		"Ctrl + Shift+Num +"
BOOL	NSys::ParseOneKey(CHAR *pKeyText, std::vector<KEYINPUT> &vKeyList, DWORD dwKeySleep)
{
	KEYINPUT		OneKey;
	BOOL			bRetVal;
	BOOL			bCtrl;
	BOOL			bAlt;
	BOOL			bShift;
	int				i;
	UINT			uMainKey;
	BOOL			bMainExt;

	memset(&OneKey, 0, sizeof(OneKey));
	vKeyList.clear();


	bRetVal = GetVKeyByName(pKeyText, &OneKey.uVirKey, &OneKey.bExt);
	if(bRetVal)
	{
		OneKey.bDown = TRUE;
		OneKey.dwSleep = dwKeySleep;
		vKeyList.push_back(OneKey);

		OneKey.bDown = FALSE;
		vKeyList.push_back(OneKey);
		return TRUE;
	}

	bCtrl = FALSE;
	bAlt = FALSE;
	bShift = FALSE;
	for(i=0; pKeyText[i]; )
	{
		if(strchr(" +", pKeyText[i]))
		{
			i ++;
			continue;
		}

		if(NStr::StrHeadCmp(pKeyText+i, "Ctrl") == 0)
		{
			i += (int)strlen("Ctrl");
			bCtrl = TRUE;
		}
		else if(NStr::StrHeadCmp(pKeyText+i, "Alt") == 0)
		{
			i += (int)strlen("Alt");
			bAlt = TRUE;
		}
		else if(NStr::StrHeadCmp(pKeyText+i, "Shift") == 0)
		{
			i += (int)strlen("Shift");
			bShift = TRUE;
		}
		else
			break;
	}

	bRetVal = GetVKeyByName(pKeyText+i, &uMainKey, &bMainExt);
	if(bRetVal == FALSE)
	{
		assert(0);
		return FALSE;
	}


	OneKey.bExt = FALSE;
	OneKey.bDown = TRUE;
	OneKey.dwSleep = dwKeySleep;
	if(bCtrl)
	{
		OneKey.uVirKey = VK_CONTROL;
		vKeyList.push_back(OneKey);
	}
	if(bAlt)
	{
		OneKey.uVirKey = VK_MENU;
		vKeyList.push_back(OneKey);
	}
	if(bShift)
	{
		OneKey.uVirKey = VK_SHIFT;
		vKeyList.push_back(OneKey);
	}

	OneKey.bExt = bMainExt;
	OneKey.bDown = TRUE;
	OneKey.uVirKey = uMainKey;
	OneKey.dwSleep = dwKeySleep;
	vKeyList.push_back(OneKey);
	OneKey.bDown = FALSE;
	vKeyList.push_back(OneKey);

	OneKey.bExt = FALSE;
	OneKey.bDown = FALSE;
	OneKey.dwSleep = dwKeySleep;
	if(bCtrl)
	{
		OneKey.uVirKey = VK_CONTROL;
		vKeyList.push_back(OneKey);
	}
	if(bAlt)
	{
		OneKey.uVirKey = VK_MENU;
		vKeyList.push_back(OneKey);
	}
	if(bShift)
	{
		OneKey.uVirKey = VK_SHIFT;
		vKeyList.push_back(OneKey);
	}

	return TRUE;
}


//	ÓÃ{}·Ö¿ª
BOOL	NSys::ParseMutiKeyText(CHAR *pMutiKey, std::vector<KEYINPUT> &vKeyList, DWORD dwKeySleep)
{
	std::vector<std::string>			sTextList;
	std::vector<std::string>::iterator	Iter;
	CHAR						szOneText[256];
	std::vector<KEYINPUT>		vOneKeyList;

	NStr::GetGroupText(pMutiKey, sTextList);
	for(Iter = sTextList.begin();
		Iter != sTextList.end();
		Iter ++)
	{
		strcpy(szOneText, Iter->c_str());

		ParseOneKey(szOneText, vOneKeyList);

		vKeyList.insert(vKeyList.end(), vOneKeyList.begin(), vOneKeyList.end());
	}

	return TRUE;
}


int		NSys::InputKeys(CHAR *pMutiKey, DWORD dwKeySleep)
{
	std::vector<KEYINPUT>		vKeyList;
	int				nCount;

	ParseMutiKeyText(pMutiKey, vKeyList, dwKeySleep);

#ifdef _DEBUG
	NData::DbgArry10K((KEYINPUT *)&vKeyList[0]);
#endif

	nCount = InputKeys(vKeyList);

	return nCount;
}


size_t	NSys::ReadProcMem(HANDLE hProc, VOID *pAddr, VOID *pBuffer, size_t dwSize)
{
	MEMORY_BASIC_INFORMATION	mbi;
	size_t		dwRetVal;
	DWORD		dwNewProtect;
	DWORD		dwOldProtect;
	size_t		dwRead;
	BOOL		bRetVal;

	if(pAddr == NULL)
	{
		SetLastError(ERROR_INVALID_ADDRESS);
		return 0;
	}

	dwRead = 0;
	bRetVal = ::ReadProcessMemory(hProc, pAddr, pBuffer, dwSize, (DWORD *)&dwRead);
	if(bRetVal)
		return dwRead;

	dwOldProtect = -1;

	dwRetVal = (DWORD)VirtualQueryEx(hProc, pAddr, &mbi, sizeof(mbi));
	if(dwRetVal != sizeof(mbi))
		return 0;

	if(dwSize > mbi.RegionSize)
		dwSize = mbi.RegionSize;

	dwOldProtect = mbi.Protect;
	dwNewProtect = PAGE_EXECUTE_READWRITE;

	bRetVal = VirtualProtectEx(hProc, pAddr, mbi.RegionSize, dwNewProtect, &dwOldProtect);

	dwRead = 0;
	bRetVal = ::ReadProcessMemory(hProc, pAddr, pBuffer, dwSize, (DWORD *)&dwRead);
	if(bRetVal == FALSE || dwRead == 0)
		assert(0);

	bRetVal = VirtualProtectEx(hProc, pAddr, mbi.RegionSize, dwOldProtect, &dwNewProtect);
	return dwRead;
}


size_t	NSys::ReadProcMem(DWORD dwProcId, VOID *pAddr, VOID *pBuffer, size_t dwSize)
{
	CAutoHandle		hProc;
	size_t			dwRetVal;

	if(dwProcId == 0)
		hProc = GetCurrentProcess();
	else
		hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_OPERATION, FALSE, dwProcId);

	if(hProc == NULL)
		return 0;

	dwRetVal = ReadProcMem(hProc, pAddr, pBuffer, dwSize);

	return dwRetVal;
}


SYSTEM_INFO *	NSys::GetSystemInfo()
{
	static SYSTEM_INFO	SysInfo = {0};

	if(SysInfo.dwPageSize == 0)
	{
		::GetSystemInfo(&SysInfo);
	}
	return &SysInfo;
}

VOID *	NSys::GetAppMinAddr()
{
	return GetSystemInfo()->lpMinimumApplicationAddress;
}

VOID *	NSys::GetAppMaxAddr()
{
	return GetSystemInfo()->lpMaximumApplicationAddress;
}


CHAR *	NSys::GetAppFileInfo(CHAR *pBuffer)
{
	static CHAR	s_szAppFileInfo[4096] = "";

	CHAR		szModPath[MAXPATH];
	CHAR *		pCmdLine;
	CHAR *		pFileVer;
	CHAR		szFileEdtTime[256];
	CHAR		szFileTimeStamp[256];
	time_t		t;
	FILETIME	ft;

	NFile::FILE_VERSION	Ver;

	if(pBuffer == NULL)
		pBuffer = s_szAppFileInfo;

	GetModuleFileNameA(NULL, szModPath, sizeof(szModPath));
	NFile::GetFileVersions(szModPath, &Ver);
	pFileVer = Ver.szVersion;

	NFile::GetFileTime(szModPath, &ft);
	NStr::GetDateTime(szFileEdtTime, ft);

	t = (time_t)NPe::GetModuleTimeStamp();
	NStr::GetDateTime(szFileTimeStamp, t);

	pCmdLine = GetCommandLineA();

	sprintf(pBuffer, 
		"Version: %s, EdtTime: %s, TimeStamp: %s, Cmd: %s",
		pFileVer, szFileEdtTime, szFileTimeStamp, pCmdLine);

	return pBuffer;
}


LOGFONTA *	NSys::GetSystemFont(LOGFONTA *pLogFont)
{
	static	LOGFONTA			LogFont = {0};
	static	NONCLIENTMETRICSA	SysMtr = {0};
	BOOL		bRetVal;

	if(pLogFont == NULL)
		pLogFont = &LogFont;
//	bRetVal = SystemParametersInfoA(SPI_GETICONTITLELOGFONT, 0, pLogFont, 0);
	bRetVal = SystemParametersInfoA(SPI_GETNONCLIENTMETRICS, 0, &SysMtr, 0);
	if(bRetVal == FALSE)
		assert(0);

	pLogFont = &SysMtr.lfMessageFont;

	return pLogFont;
}


#define 	GetObjAs(_obj, TYPE)	(*(TYPE *)&_obj)

BOOL	NSys::GetProcessTime(DWORD dwPid, FILETIME *pftCreate, FILETIME *pftExec)
{
	FILETIME	ftCreate;
	FILETIME	ftExit;
	FILETIME	ftUser;
	FILETIME	ftKrn;
	CAutoHandle	hProc;
	BOOL		bRetVal;
	ULONGLONG	uuTotal;

	GetObjAs(*pftCreate, ULONGLONG) = 0;
	GetObjAs(*pftExec, ULONGLONG) = 0;

	hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
	if(hProc == NULL)
		return FALSE;

	bRetVal = GetProcessTimes(hProc, &ftCreate, &ftExit, &ftKrn, &ftUser);
	if(bRetVal == FALSE)
		return FALSE;

	uuTotal = GetObjAs(ftKrn, ULONGLONG) + GetObjAs(ftUser, ULONGLONG);
	GetObjAs(*pftExec, ULONGLONG) = uuTotal;
	*pftCreate = ftCreate;

	return TRUE;
}


BOOL	NSys::MsgLoop()
{
	MSG		msg;
	BOOL	bRetVal;

	for(;;)
	{
		bRetVal = PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE);
		if(bRetVal)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Sleep(1);
		}
	}
}



HMODULE	NSys::GetProcModuleHandle(HANDLE hProc, CHAR *pDllName)
{
	CHAR		szDllName[MAXPATH];

	if(pDllName == NULL)
	{
		GetModuleFileNameExA(hProc, NULL, szDllName, sizeof(szDllName));
	}

	return NULL;
}

#pragma pack(push,1)

typedef struct _OneceHookCode
{
	BYTE	_00Debug;
	BYTE	_01bMoveEax;
	DWORD	_02dwHookAddr;
	BYTE	_06bPushEax;
	struct _VirtualProtectParam
	{
		BYTE		bParamPush;
		DWORD		dwParam;
	}VirtualProtectParam[4];
	BYTE	_1BVirtualProtectCall;
	DWORD	_1CVirtualProtectFun;
	BYTE	_20PopEax;
	BYTE	_21PushEax;
	WORD	_22wMovEaxPtr1;
	BYTE	_24bOldCode;
	WORD	_25bMoveEaxPtr2;
	BYTE	_27bDlt;
	DWORD	_28dwOldCode;
	BYTE	_2CbJmp;
	DWORD	_2DdwDatJmp;
	DWORD	_31dwOldPrct;
	BYTE	_35bzData[1020];
	BYTE	_431bzCode[1024];	//	ÐèÒª·µ»Ø ret
}OneceHookCode;

#pragma pack(pop)


//	ÐèÒª¼ÓÉÏ´úÂë·µ»Ø	ret
BOOL	CreateOneceHookCode(DWORD dwHookAddr,	BYTE bzHookedCode[], DWORD dwCodeAddr, BYTE bzCode[], 
								  BYTE *pDat, BYTE *pAddCode)
{
	OneceHookCode *		pCode;

	pCode = (OneceHookCode *)bzCode;
	pCode->_00Debug = 0x90;
	pCode->_01bMoveEax = 0xB8;
	pCode->_02dwHookAddr = dwHookAddr;
	pCode->_06bPushEax = 0x50;
	pCode->_1BVirtualProtectCall = 0xE8;
	pCode->_1CVirtualProtectFun = (DWORD)(__int64)&VirtualProtect - (DWORD)(__int64)&((OneceHookCode *)dwCodeAddr)->_1BVirtualProtectCall-5;
	pCode->_20PopEax = 0x58;
	pCode->_21PushEax = 0x50;
	pCode->_22wMovEaxPtr1 = 0x00C6;
	pCode->_24bOldCode = bzHookedCode[0];
	pCode->_25bMoveEaxPtr2 = 0x40C7;
	pCode->_27bDlt = 0x01;
	pCode->_28dwOldCode = *(DWORD *)(bzHookedCode+1);
	pCode->_2CbJmp = 0xE9;
	pCode->_2DdwDatJmp = sizeof(pCode->_31dwOldPrct) + sizeof(pCode->_35bzData);

	pCode->VirtualProtectParam[0].bParamPush = 0x68;
	pCode->VirtualProtectParam[1].bParamPush = 0x68;
	pCode->VirtualProtectParam[2].bParamPush = 0x68;
	pCode->VirtualProtectParam[3].bParamPush = 0x68;

	pCode->VirtualProtectParam[0].dwParam = (DWORD)(__int64)&((OneceHookCode *)dwCodeAddr)->_31dwOldPrct;
	pCode->VirtualProtectParam[1].dwParam = PAGE_EXECUTE_READWRITE;
	pCode->VirtualProtectParam[2].dwParam = 5;
	pCode->VirtualProtectParam[3].dwParam = dwHookAddr;

	memcpy(pCode->_35bzData, pDat, sizeof(pCode->_35bzData));
	memcpy(pCode->_431bzCode, pAddCode, sizeof(pCode->_431bzCode));

	return TRUE;
}


BOOL	NSys::CreateProcessWithDll(CHAR *pExeFile, CHAR *pArgument, CHAR *pDllPath)
{
	BOOL					bRetVal;
	STARTUPINFOA			si;
	PROCESS_INFORMATION		pi;
	BOOL					bResult;
	IMAGE_NT_HEADERS 		NtHead;
	CHAR					szCmdLine[4096];
	CHAR *					pCmdLine;
	DWORD					dwLoadLibary;
	DWORD					dwHookAddr;
	CProcessMem				ProcMem;
	BYTE					bzCode[4096];
	BYTE					bzOldCode[10];
	BYTE					bzAddCode[1024];
	BYTE					bzJmpHead[10];

	if(pArgument)
	{
		sprintf(szCmdLine, "\"%s\" %s", pExeFile, pArgument);
		pCmdLine = szCmdLine;
	}
	else
	{
		pCmdLine = pExeFile;
	}

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);

	bRetVal = CreateProcessA(NULL, pCmdLine, NULL, NULL, FALSE, CREATE_SUSPENDED, 
		NULL, NULL, &si, &pi);
	if(bRetVal == FALSE)
		return FALSE;

	bResult = TRUE;

	NPe::GetFileNtHead(pExeFile, &NtHead);
	dwHookAddr = NtHead.OptionalHeader.ImageBase + NtHead.OptionalHeader.AddressOfEntryPoint;
	dwLoadLibary = (DWORD)(__int64)NSys::GetDllProcAdders("Kernel32", "LoadLibraryA");


	ProcMem.OpenAndAlloc(pi.dwProcessId, sizeof(bzCode));
	ProcMem.Read((VOID *)dwHookAddr, bzOldCode, sizeof(bzOldCode));

	bzJmpHead[0] = 0xE9;
	*(DWORD *)(bzJmpHead + 1) = (DWORD)(__int64)ProcMem.GetData() - dwHookAddr - 5;
	ProcMem.Wirte((VOID *)dwHookAddr, bzJmpHead, 5);


	OneceHookCode *		pFullCode;
	pFullCode = (OneceHookCode *)ProcMem.GetData();

	*(BYTE *)(bzAddCode+0) = 0xB8;
	*(DWORD *)(bzAddCode+1) = (DWORD)(__int64)pFullCode->_35bzData;
	*(BYTE *)(bzAddCode+5) = 0x50;
	*(BYTE *)(bzAddCode+6) = 0xE8;
	*(DWORD *)(bzAddCode+7) = dwLoadLibary - ((DWORD)(__int64)&(pFullCode->_431bzCode) + 6 + 5);
	*(BYTE *)(bzAddCode+0x0B) = 0xC3;

	CreateOneceHookCode(dwHookAddr, bzOldCode, (DWORD)(__int64)ProcMem.GetData(), bzCode, (BYTE *)pDllPath, bzAddCode);

	ProcMem.Wirte(bzCode, sizeof(bzCode));
	ProcMem.KeepMem();

	ResumeThread(pi.hThread);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//	NPE
/////////////////////////////////////////////////////////////////////////////////////////////////


IMAGE_NT_HEADERS *	NPe::GetModuleNtHead(HMODULE hMod)
{
	IMAGE_DOS_HEADER *		pDosHead;
	IMAGE_NT_HEADERS *		pNtHead;

	if(hMod == NULL)
		hMod = GetModuleHandleA(NULL);

	pDosHead = (IMAGE_DOS_HEADER *)hMod;
	pNtHead = (IMAGE_NT_HEADERS *)((BYTE *)hMod + pDosHead->e_lfanew);

	return pNtHead;
}

BOOL	NPe::GetProcModuleNtHead(HANDLE hProc, HMODULE hMod, IMAGE_NT_HEADERS *pNtHead)
{
	return FALSE;
}


BOOL	NPe::GetFileNtHead(CHAR *pFilePath, IMAGE_NT_HEADERS *pFileNtHead)
{
	HANDLE			hFile;
	BOOL			bRetVal;
	DWORD			dwRead;
	BYTE			bzData[4096];
	CHAR			szRealPath[MAXPATH];

	IMAGE_DOS_HEADER *			pDosHead;
	IMAGE_NT_HEADERS32 *		pNtHead;

	NFile::FormatFilePath(pFilePath, szRealPath);

	hFile = CreateFileA(szRealPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
		NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	dwRead = 0;
	memset(bzData, 0, sizeof(bzData));
	bRetVal = ReadFile(hFile, bzData, sizeof(bzData), &dwRead, NULL);
	if(bzData == FALSE)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	CloseHandle(hFile);

	pDosHead = (IMAGE_DOS_HEADER *)bzData;
	pNtHead = (IMAGE_NT_HEADERS *)(bzData + pDosHead->e_lfanew);

	if(pDosHead->e_magic != IMAGE_DOS_SIGNATURE)	//MZ
		return FALSE;
	if(pNtHead->FileHeader.SizeOfOptionalHeader != sizeof(IMAGE_OPTIONAL_HEADER32))
		return FALSE;
	if(pNtHead->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC)
		return FALSE;
	if(pNtHead->FileHeader.NumberOfSections < 1)
		return FALSE;

	*pFileNtHead = *pNtHead;

	return TRUE;
}

FARPROC	NPe::GetModuleEntry(HMODULE hMod)
{
	IMAGE_NT_HEADERS *		pNtHead;
	FARPROC					fRetVal;

	pNtHead = GetModuleNtHead(hMod);
	fRetVal = (FARPROC)((BYTE *)hMod + pNtHead->OptionalHeader.AddressOfEntryPoint);

	return fRetVal;
}


DWORD	NPe::GetModuleTimeStamp(HMODULE hMod)
{
	IMAGE_NT_HEADERS *		pNtHead;

	pNtHead = GetModuleNtHead(hMod);
	return pNtHead->FileHeader.TimeDateStamp;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//						NFile
/////////////////////////////////////////////////////////////////////////////////////////////////

//	need free()
BYTE	* NFile::GetFileData(CHAR *pFileName, DWORD * pdwSize)
{
	BY_HANDLE_FILE_INFORMATION	HFileInfo;
	HANDLE						hFile;
	BOOL						bRetVal;
	DWORD						dwSize, dwRead;
	CHAR						* MainBuf;

	hFile = CreateFileA(pFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
		NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	bRetVal = GetFileInformationByHandle(hFile, &HFileInfo);
	if(bRetVal == FALSE)
	{
		CloseHandle(hFile);
		return NULL;
	}

	if(HFileInfo.nFileSizeHigh != 0)
	{
		CloseHandle(hFile);
		return NULL;
	}

	dwSize = HFileInfo.nFileSizeLow;
	if(pdwSize)
		*pdwSize = dwSize;

	MainBuf = (CHAR *)malloc(dwSize+2);
	if(MainBuf == NULL)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	bRetVal = ::ReadFile(hFile, MainBuf, dwSize, &dwRead, NULL);
	if(bRetVal == FALSE || dwRead != dwSize)
	{
		CloseHandle(hFile);
		free(MainBuf);
		return NULL;
	}

	MainBuf[dwSize] = 0;
	MainBuf[dwSize+1] = 0;
	CloseHandle(hFile);
	return (BYTE *)MainBuf;
}


BYTE *	NFile::GetFileData(WCHAR *pFileName, DWORD * pdwSize)
{
	BY_HANDLE_FILE_INFORMATION	HFileInfo;
	HANDLE						hFile;
	BOOL						bRetVal;
	DWORD						dwSize, dwRead;
	CHAR						* MainBuf;

	hFile = CreateFileW(pFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
		NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	bRetVal = GetFileInformationByHandle(hFile, &HFileInfo);
	if(bRetVal == FALSE)
	{
		CloseHandle(hFile);
		return NULL;
	}

	if(HFileInfo.nFileSizeHigh != 0)
	{
		CloseHandle(hFile);
		return NULL;
	}

	dwSize = HFileInfo.nFileSizeLow;
	if(pdwSize)
		*pdwSize = dwSize;

	MainBuf = (CHAR *)malloc(dwSize+2);
	if(MainBuf == NULL)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	bRetVal = ::ReadFile(hFile, MainBuf, dwSize, &dwRead, NULL);
	if(bRetVal == FALSE || dwRead != dwSize)
	{
		CloseHandle(hFile);
		free(MainBuf);
		return NULL;
	}

	MainBuf[dwSize] = 0;
	MainBuf[dwSize+1] = 0;
	CloseHandle(hFile);
	return (BYTE *)MainBuf;
}


DWORD	NFile::DumpToFile(CHAR *pFileName, VOID *pMem, DWORD dwSize)
{
	DWORD			dwRet;
	FILE			* fp;

	fp=fopen(pFileName, "wb");
	if(fp == NULL)  
		return 0;

	if(dwSize == 0)
		dwSize = (DWORD)strlen((const char *)pMem);

	dwRet = (DWORD)fwrite(pMem, 1, dwSize, fp);

	fclose(fp);
	return dwRet;
}


DWORD	NFile::DumpAppend(CHAR *pFileName, VOID *pMem, DWORD dwSize)
{
	DWORD			dwRet;
	FILE			* fp;

	fp=fopen(pFileName, "ab");
	if(fp == NULL)  
		return 0;

	if(dwSize == 0)
		dwSize = (DWORD)strlen((const char *)pMem);

	dwRet = (DWORD)fwrite(pMem, 1, dwSize, fp);

	fclose(fp);
	return dwRet;
}


BOOL	NFile::DeleteDirTree(CHAR *pRootDir, BOOL bSkipFailed)
{
	WIN32_FIND_DATAA		FindData;
	HANDLE					hFind;
	CHAR					szFileName[MAX_PATH*2];
	CHAR					szDelDir[MAX_PATH*2];
	BOOL					bFindMore;
	BOOL					bRetVal;
	int						nError;

	nError = ERROR_SUCCESS;
	ZeroMemory(szFileName, sizeof(szFileName));
	ZeroMemory(&FindData, sizeof(FindData));

	strcpy(szDelDir, pRootDir);
	if(szDelDir[strlen(szDelDir) - 1] != '\\')
	{
		strcat(szDelDir, "\\");
	}

	strcpy(szFileName, szDelDir);
	strcat(szFileName, "*");

	hFind = FindFirstFileA(szFileName, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return RemoveDirectoryA(pRootDir);
	}

	for(bFindMore = TRUE;
		bFindMore;
		bFindMore = FindNextFileA(hFind, &FindData))
	{
		if(strcmp(FindData.cFileName, ".") == 0 ||
			strcmp(FindData.cFileName, "..") == 0)
		{
			continue;
		}

		if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)       
		{
			strcpy(szFileName, szDelDir);
			strcat(szFileName, FindData.cFileName);
			DeleteDirTree(szFileName);
		}
		else
		{
			strcpy(szFileName, szDelDir);
			strcat(szFileName, FindData.cFileName);
			bRetVal = DeleteFileA(szFileName);
			if(bRetVal == FALSE)
			{
				nError = GetLastError();
				if(bSkipFailed == FALSE)
					break;
			}
		}
	}
	FindClose(hFind);

	bRetVal = RemoveDirectoryA(pRootDir);
	if(bRetVal == FALSE && (nError == ERROR_SUCCESS))
	{
		nError = GetLastError();
	}

	if(nError != ERROR_SUCCESS)
	{
		SetLastError(nError);
		return FALSE;
	}

	return TRUE;
}

//	É¾³ýÎÄ¼þ¼Ð×îºóµÄÐ±¸Ë	'\\'
BOOL	NFile::DelFolderSlash(CHAR *pFolder, CHAR *pNewFolder)
{
	int		nLen;

	if(pNewFolder == NULL)
		pNewFolder = pFolder;
	else
		strcpy(pNewFolder, pFolder);

	nLen = (int)strlen(pNewFolder);
	if(pNewFolder[nLen-1] == '\\')
		pNewFolder[nLen-1] = 0;

	return TRUE;
}


BOOL	NFile::DelFolderSlash(WCHAR *pFolder, WCHAR *pNewFolder)
{
	int		nLen;

	if(pNewFolder == NULL)
		pNewFolder = pFolder;
	else
		wcscpy(pNewFolder, pFolder);

	nLen = (int)wcslen(pNewFolder);
	if(pNewFolder[nLen-1] == '\\')
		pNewFolder[nLen-1] = 0;

	return TRUE;
}

BOOL	NFile::CopyDirTree(CHAR *pExistFolder, CHAR *pNewFolder, BOOL bSkipFailed)
{
	WIN32_FIND_DATAA		FindData;
	HANDLE					hFind;
	CHAR					szFindPath[MAX_PATH];
	CHAR					szExistFile[MAXPATH];
	CHAR					szNewFile[MAXPATH];
	CHAR					szExistFolder[MAXPATH];
	CHAR					szNewFolder[MAXPATH];

	BOOL					bFindMore;
	BOOL					bRetVal;
	int						nError;

	nError = ERROR_SUCCESS;
	ZeroMemory(&FindData, sizeof(FindData));

	DelFolderSlash(pExistFolder, szExistFolder);
	DelFolderSlash(pNewFolder, szNewFolder);

	strcpy(szFindPath, szExistFolder);
	strcat(szFindPath, "\\*");

	hFind = FindFirstFileA(szFindPath, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return TRUE;
	}

	CreateDirectoryA(szNewFolder, NULL);
	for(bFindMore = TRUE;
		bFindMore;
		bFindMore = FindNextFileA(hFind, &FindData))
	{
		if(strcmp(FindData.cFileName, ".") == 0 ||
			strcmp(FindData.cFileName, "..") == 0)
		{
			continue;
		}

		strcpy(szExistFile, szExistFolder);
		strcat(szExistFile, "\\");
		strcat(szExistFile, FindData.cFileName);

		strcpy(szNewFile, szNewFolder);
		strcat(szNewFile, "\\");
		strcat(szNewFile, FindData.cFileName);

		if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)       
		{
			CopyDirTree(szExistFile, szNewFile, bSkipFailed);
		}
		else
		{
			bRetVal = CopyFileA(szExistFile, szNewFile, FALSE);
			if(bRetVal == FALSE)
			{
				nError = GetLastError();
				if(bSkipFailed == FALSE)
					break;
			}
		}
	}
	FindClose(hFind);

	if(nError != ERROR_SUCCESS)
	{
		SetLastError(nError);
		return FALSE;
	}

	return TRUE;
}


BOOL	NFile::CopyDirTree(WCHAR *pExistFolder, WCHAR *pNewFolder, BOOL bSkipFailed)
{
	WIN32_FIND_DATAW		FindData;
	HANDLE					hFind;
	WCHAR					szFindPath[MAX_PATH];
	WCHAR					szExistFile[MAXPATH];
	WCHAR					szNewFile[MAXPATH];
	WCHAR					szExistFolder[MAXPATH];
	WCHAR					szNewFolder[MAXPATH];

	BOOL					bFindMore;
	BOOL					bRetVal;
	int						nError;

	nError = ERROR_SUCCESS;
	ZeroMemory(&FindData, sizeof(FindData));

	DelFolderSlash(pExistFolder, szExistFolder);
	DelFolderSlash(pNewFolder, szNewFolder);

	wcscpy(szFindPath, szExistFolder);
	wcscat(szFindPath, L"\\*");

	hFind = FindFirstFileW(szFindPath, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return TRUE;
	}

	CreateDirectoryW(szNewFolder, NULL);
	for(bFindMore = TRUE;
		bFindMore;
		bFindMore = FindNextFileW(hFind, &FindData))
	{
		if(wcscmp(FindData.cFileName, L".") == 0 ||
			wcscmp(FindData.cFileName, L"..") == 0)
		{
			continue;
		}

		wcscpy(szExistFile, szExistFolder);
		wcscat(szExistFile, L"\\");
		wcscat(szExistFile, FindData.cFileName);

		wcscpy(szNewFile, szNewFolder);
		wcscat(szNewFile, L"\\");
		wcscat(szNewFile, FindData.cFileName);

		if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)       
		{
			CopyDirTree(szExistFile, szNewFile, bSkipFailed);
		}
		else
		{
			bRetVal = CopyFileW(szExistFile, szNewFile, FALSE);
			if(bRetVal == FALSE)
			{
				nError = GetLastError();
				if(bSkipFailed == FALSE)
					break;
			}
		}
	}
	FindClose(hFind);

	if(nError != ERROR_SUCCESS)
	{
		SetLastError(nError);
		return FALSE;
	}

	return TRUE;
}

BOOL	NFile::FileExist(CHAR *pFileName)
{
	DWORD		dwAttr;

	dwAttr = GetFileAttributesA(pFileName);
	if(dwAttr == -1)
		return FALSE;

	return TRUE;
}


BOOL	NFile::GetFileTime(CHAR *pFileName, LPFILETIME lpLastWriteTime, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime)
{
	HANDLE			hFile;
	FILETIME		LastWriteTime = {0};
	FILETIME		CreationTime = {0};
	FILETIME		LastAccessTime = {0};
	BOOL			bRetVal = FALSE;

    hFile = CreateFileA(pFileName, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		::GetFileTime(hFile, &CreationTime, &LastAccessTime, &LastWriteTime);                 
		CloseHandle(hFile);
		bRetVal = TRUE;
	}

	if(lpLastWriteTime)
		*lpLastWriteTime = LastWriteTime;
	if(lpCreationTime)
		*lpCreationTime = CreationTime;
	if(lpLastAccessTime)
		*lpLastAccessTime = LastAccessTime;

	return bRetVal;
}


DWORD	NFile::GetFileSize(CHAR *pFileName, LPDWORD lpFileSizeHigh)
{
	HANDLE			hFile;
	DWORD			dwFileSizeHigh = 0;
	DWORD			dwFileSize = 0;

	hFile = CreateFileA(pFileName, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		dwFileSize = ::GetFileSize(hFile, &dwFileSizeHigh);
		if(lpFileSizeHigh)
			*lpFileSizeHigh = dwFileSizeHigh;

		CloseHandle(hFile);
	}

	return dwFileSize;
}


BOOL	NFile::GetFileInfo(CHAR *pFileName, BY_HANDLE_FILE_INFORMATION *pInfo)
{
	HANDLE			hFile;
	BOOL			bRetVal;
	int				nError;

	hFile = CreateFileA(pFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE
		, NULL,	OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		nError = GetLastError();
		hFile = CreateFileA(pFileName, 0, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS , NULL);
	}
	if(hFile == INVALID_HANDLE_VALUE)
	{
		SetLastError(nError);
		return FALSE;
	}

	bRetVal = GetFileInformationByHandle(hFile, pInfo);
	CloseHandle(hFile);

	return bRetVal;
}


BOOL	NFile::GetFileInfo(WCHAR *pFileName, BY_HANDLE_FILE_INFORMATION *pInfo)
{
	HANDLE			hFile;
	BOOL			bRetVal;
	int				nError;

	hFile = CreateFileW(pFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE
		, NULL,	OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		nError = GetLastError();
		return FALSE;
	}

	bRetVal = GetFileInformationByHandle(hFile, pInfo);
	CloseHandle(hFile);

	return bRetVal;
}


BOOL	NFile::GetFileInfoByFind(WCHAR *pFileName, BY_HANDLE_FILE_INFORMATION *pInfo)
{
	WIN32_FIND_DATAW	FindData;
	HANDLE				hFind;

	memset(pInfo, 0, sizeof(BY_HANDLE_FILE_INFORMATION));
	hFind = FindFirstFileW(pFileName, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	FindClose(hFind);

	pInfo->dwFileAttributes = FindData.dwFileAttributes;
	pInfo->ftCreationTime = FindData.ftCreationTime;
	pInfo->ftLastAccessTime = FindData.ftLastAccessTime;
	pInfo->ftLastWriteTime = FindData.ftLastWriteTime;
	pInfo->nFileSizeHigh = FindData.nFileSizeHigh;
	pInfo->nFileSizeLow = FindData.nFileSizeLow;

	return TRUE;
}


BOOL	NFile::GetFileInfoByFind(CHAR *pFileName, BY_HANDLE_FILE_INFORMATION *pInfo)
{
	WIN32_FIND_DATAA	FindData;
	HANDLE				hFind;

	memset(pInfo, 0, sizeof(BY_HANDLE_FILE_INFORMATION));
	hFind = FindFirstFileA(pFileName, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	FindClose(hFind);

	pInfo->dwFileAttributes = FindData.dwFileAttributes;
	pInfo->ftCreationTime = FindData.ftCreationTime;
	pInfo->ftLastAccessTime = FindData.ftLastAccessTime;
	pInfo->ftLastWriteTime = FindData.ftLastWriteTime;
	pInfo->nFileSizeHigh = FindData.nFileSizeHigh;
	pInfo->nFileSizeLow = FindData.nFileSizeLow;

	return TRUE;
}

BOOL	NFile::SetFileTime(CHAR *pFileName, FILETIME *lpLastWriteTime, FILETIME *lpCreationTime, FILETIME *lpLastAccessTime)
{
	HANDLE			hFile;
	BOOL			bRetVal = FALSE;

	hFile = CreateFileA(pFileName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		::SetFileTime(hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime);                 
		CloseHandle(hFile);
		bRetVal = TRUE;
	}

	return bRetVal;
}


BOOL	NFile::SpliteFilePath(CHAR *pFullPath, CHAR *pFileDir, CHAR *pFileName)
{
	CHAR			*pFind;

	pFind = strrchr(pFullPath, '\\');
	if(pFind == NULL)
		pFind = strrchr(pFullPath, '/');
	if(pFind == NULL)
	{
		if(pFileDir)
            NStr::StrCopy(pFileDir, pFullPath);
		if(pFileName)
            NStr::StrCopy(pFileName, pFullPath);

		return FALSE;
	}
	else
	{
		if(pFileDir)
			NStr::StrnCopy(pFileDir, pFullPath, pFind-pFullPath);
		if(pFileName)
			NStr::StrCopy(pFileName, pFind+1);
	}

	return TRUE;
}


BOOL	NFile::SpliteFilePath(WCHAR *pFullPath, WCHAR *pFileDir, WCHAR *pFileName)
{
	WCHAR			*pFind;

	pFind = wcsrchr(pFullPath, '\\');
	if(pFind == NULL)
		pFind = wcsrchr(pFullPath, '/');
	if(pFind == NULL)
	{
		if(pFileDir)
			wcscpy(pFileDir, pFullPath);
		if(pFileName)
			wcscpy(pFileName, pFullPath);

		return FALSE;
	}
	else
	{
		if(pFileDir)
		{
			wcsncpy(pFileDir, pFullPath, pFind-pFullPath);
			pFileDir[pFind-pFullPath] = 0;
		}
		if(pFileName)
			wcscpy(pFileName, pFind+1);
	}

	return TRUE;
}

BOOL	NFile::SpliteFileName(CHAR *pFileName, CHAR *pPartName, CHAR *pExName)
{
	CHAR			*pFind;

	pFind = strrchr(pFileName, '.');
	if(pFind == NULL)
	{
		if(pPartName)
			NStr::StrCopy(pPartName, pFileName);
		if(pExName)
			NStr::StrCopy(pExName, "");
	}
	else
	{
		if(pPartName)
			NStr::StrnCopy(pPartName, pFileName, pFind-pFileName);
		if(pExName)
			NStr::StrCopy(pExName, pFind+1);
	}

	return TRUE;
}

BOOL	NFile::SpliteFilePathEx(CHAR *pFullPath, CHAR *pFileDir, CHAR *pFileName, 
		CHAR *pDrive, CHAR *pPartDir, CHAR *pPartName, CHAR *pExtName)
{
	_splitpath(pFullPath, pDrive, pPartDir, pPartName, pExtName);

	if(pFileDir || pFileName)
		SpliteFilePath(pFullPath, pFileDir, pFileName);

	return TRUE;
}


BOOL	NFile::FileFindEnum(FILEENUMPROC FileEnumProc, VOID *ProcParam, CHAR *pFindPath)
{
	WIN32_FIND_DATAA		FindData;
	HANDLE					hFind;
	CHAR					szFindDir[MAXPATH];
	BOOL					bRetVal;
	BOOL					bBreak;

	ZeroMemory(&FindData, sizeof(FindData));
	hFind = FindFirstFileA(pFindPath, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	NFile::SpliteFilePath(pFindPath, szFindDir);
	for(bRetVal = TRUE;
		bRetVal;
		bRetVal = FindNextFileA(hFind, &FindData))
	{
		if(FindData.cFileName[0] == '.' &&
			(FindData.cFileName[1] == 0 || (FindData.cFileName[1] == '.' && FindData.cFileName[2] == 0))
			)
			continue;

		bBreak = FileEnumProc(ProcParam, szFindDir, &FindData);
		if(bBreak)
		{
			FindClose(hFind);
			return FALSE;
		}
	}

	FindClose(hFind);
	return TRUE;
}


BOOL	NFile::FileFindEnumW(FILEENUMPROCW FileEnumProc, VOID *ProcParam, WCHAR *pFindPath)
{
	WIN32_FIND_DATAW		FindData;
	HANDLE					hFind;
	WCHAR					szFindDir[MAXPATH];
	BOOL					bRetVal;
	BOOL					bBreak;

	ZeroMemory(&FindData, sizeof(FindData));
	hFind = FindFirstFileW(pFindPath, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	NFile::SpliteFilePath(pFindPath, szFindDir);
	for(bRetVal = TRUE;
		bRetVal;
		bRetVal = FindNextFileW(hFind, &FindData))
	{
		if(FindData.cFileName[0] == '.' &&
			(FindData.cFileName[1] == 0 || (FindData.cFileName[1] == '.' && FindData.cFileName[2] == 0))
			)
			continue;

		bBreak = FileEnumProc(ProcParam, szFindDir, &FindData);
		if(bBreak)
		{
			FindClose(hFind);
			return FALSE;
		}
	}

	FindClose(hFind);
	return TRUE;
}


BOOL	NFile::WildcardCopyFiles(CHAR *pSrcDir, CHAR *pDstDir, CHAR *pFileName, BOOL bSkipFailed)
{
	CHAR				szFileName[MAX_PATH*2];
	WIN32_FIND_DATAA	FindData;
	HANDLE				hFind;
	int					nEndPos;
	_bstr_t				bStrSrcPath;
	_bstr_t				bStrDstPath;
	BOOL				bFindMore;
	BOOL				bRetVal;
	CHAR				szSrcFile[MAX_PATH*2];
	CHAR				szDstFile[MAX_PATH*2];
	int					nLastError;

	nEndPos = (int)strlen(pSrcDir) - 1;
	if(pSrcDir[nEndPos] != '\\')
	{
		bStrSrcPath = pSrcDir;
		bStrSrcPath += "\\";
		pSrcDir = bStrSrcPath;
	}

	nEndPos = (int)strlen(pDstDir) - 1;
	if(pDstDir[nEndPos] != '\\')
	{
		bStrDstPath = pDstDir;
		bStrDstPath += "\\";
		pDstDir = bStrDstPath;
	}

	sprintf(szFileName, "%s%s", pSrcDir, pFileName);
	hFind = FindFirstFileA(szFileName, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	nLastError = 0;
	
	for(bFindMore = TRUE;
		bFindMore;
		bFindMore = FindNextFileA(hFind, &FindData))
	{
		if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)       
			continue;

		sprintf(szSrcFile, "%s%s", pSrcDir, FindData.cFileName);
		sprintf(szDstFile, "%s%s", pDstDir, FindData.cFileName);

		SetFileAttributesA(szSrcFile, FILE_ATTRIBUTE_NORMAL);
		SetFileAttributesA(szDstFile, FILE_ATTRIBUTE_NORMAL);

		bRetVal = CopyFileA(szSrcFile, szDstFile, FALSE);
		if(bRetVal == FALSE)
		{
			nLastError = GetLastError();
			if(bSkipFailed == FALSE)
			{
				FindClose(hFind);
				SetLastError(nLastError);
				return FALSE;
			}
		}

	}
	FindClose(hFind);

	if(nLastError == ERROR_SUCCESS)
        return TRUE;

	SetLastError(nLastError);
	return FALSE;
}


BOOL	NFile::WildcardDelFiles(CHAR *pSrcDir, CHAR *pFileName, BOOL bSkipFailed)
{
	CHAR				szFileName[MAX_PATH*2];
	WIN32_FIND_DATAA	FindData;
	HANDLE				hFind;
	int					nEndPos;
	_bstr_t				bStrSrcPath;
	BOOL				bFindMore;
	BOOL				bRetVal;
	CHAR				szSrcFile[MAX_PATH*2];
	int					nLastError;

	if(pSrcDir == NULL || strlen(pSrcDir) == 0)
	{
		strcpy(szFileName, pFileName);
	}
	else
	{
		nEndPos = (int)strlen(pSrcDir) - 1;
		if(pSrcDir[nEndPos] != '\\')
		{
			bStrSrcPath = pSrcDir;
			bStrSrcPath += "\\";
			pSrcDir = bStrSrcPath;
		}
		sprintf(szFileName, "%s%s", pSrcDir, pFileName);
	}

	hFind = FindFirstFileA(szFileName, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	nLastError = 0;

	for(bFindMore = TRUE;
		bFindMore;
		bFindMore = FindNextFileA(hFind, &FindData))
	{
		if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)       
			continue;

		sprintf(szSrcFile, "%s%s", pSrcDir, FindData.cFileName);

		SetFileAttributesA(szSrcFile, FILE_ATTRIBUTE_NORMAL);
		bRetVal = DeleteFileA(szSrcFile);
		if(bRetVal == FALSE)
		{
			nLastError = GetLastError();
//			NLog::DbgLog("WildcardDelFiles Failed: %s, Error: %d", szSrcFile, nLastError);
			if(bSkipFailed == FALSE)
			{
				FindClose(hFind);
				SetLastError(nLastError);
				return FALSE;
			}
		}
	}
	FindClose(hFind);

	if(nLastError == ERROR_SUCCESS)
		return TRUE;

	SetLastError(nLastError);
	return FALSE;
}


BOOL	NFile::WildcardDelFiles(CHAR *pFilePath, BOOL bSkipFailed)
{
	CHAR				szFindDir[MAXPATH];
	CHAR				szFindFilePath[MAXPATH];
	WIN32_FIND_DATAA	FindData;
	HANDLE				hFind;
	BOOL				bFindMore;
	BOOL				bRetVal;
	int					nLastError;

	NFile::SpliteFilePath(pFilePath, szFindDir);

	hFind = FindFirstFileA(pFilePath, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	nLastError = 0;

	for(bFindMore = TRUE;
		bFindMore;
		bFindMore = FindNextFileA(hFind, &FindData))
	{
		if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)       
			continue;

		sprintf(szFindFilePath, "%s\\%s", szFindDir, FindData.cFileName);

		SetFileAttributesA(szFindFilePath, FILE_ATTRIBUTE_NORMAL);
		bRetVal = DeleteFileA(szFindFilePath);
		if(bRetVal == FALSE)
		{
			nLastError = GetLastError();
			if(bSkipFailed == FALSE)
			{
				FindClose(hFind);
				SetLastError(nLastError);
				return FALSE;
			}
		}
	}
	FindClose(hFind);

	if(nLastError == ERROR_SUCCESS)
		return TRUE;

	SetLastError(nLastError);
	return FALSE;
}

BOOL	NFile::WildcardFileExist(CHAR *pWidFileName)
{
	WIN32_FIND_DATAA		FindData;
	HANDLE					hFind;

	ZeroMemory(&FindData, sizeof(FindData));

	hFind = FindFirstFileA(pWidFileName, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	FindClose(hFind);
	return TRUE;
}


BOOL	WINAPI	WildcardGetFileList_CallBack(VOID *pParam, CHAR *pFilePath, WIN32_FIND_DATAA *pFileData)
{
	std::vector<std::string> *		pFileList;
	std::string						sFilePath;

	pFileList = (std::vector<std::string> *)pParam;

	sFilePath = pFilePath;
	sFilePath += "\\";
	sFilePath += pFileData->cFileName;
	pFileList->push_back(sFilePath);

	return FALSE;
}


BOOL	NFile::WildcardGetFileList(CHAR *pWidFileName, std::vector<std::string> &vFilePath)
{
	BOOL			bRetVal;

	vFilePath.clear();
	bRetVal = FileFindEnum(WildcardGetFileList_CallBack, &vFilePath, pWidFileName);

	return bRetVal;
}


BOOL	NFile::GetFilePath(CHAR *pFileName, CHAR *pFilePath, int nPathLen)
{
	DWORD			dwRetVal;

	dwRetVal = SearchPathA(NULL, pFileName, NULL, nPathLen, pFilePath, NULL);
	if(dwRetVal == 0)
		return FALSE;

	return TRUE;
}


BOOL	NFile::GetFileShortName(CHAR *pFileName, CHAR *pRecvBuf)
{
	DWORD		dwRetVal;
	CHAR		szBuffer[MAX_PATH*2];

	dwRetVal = GetShortPathNameA(pFileName, szBuffer, sizeof(szBuffer));
	if(dwRetVal == 0)
		strcpy(szBuffer, pFileName);

	if(pRecvBuf)
		strcpy(pRecvBuf, szBuffer);
	else
		strcpy(pFileName, szBuffer);

	return TRUE;
}


BOOL	NFile::GetFileLongName(CHAR *pFileName, CHAR *pRecvBuf)
{
	DWORD		dwRetVal;
	CHAR		szBuffer[MAX_PATH*2];

	dwRetVal = GetLongPathNameA(pFileName, szBuffer, sizeof(szBuffer));
	if(dwRetVal == 0)
		strcpy(szBuffer, pFileName);

	if(pRecvBuf)
		strcpy(pRecvBuf, szBuffer);
	else
		strcpy(pFileName, szBuffer);

	return TRUE;
}


BOOL	NFile::IsDirectory(CHAR *pFilePath)
{
	DWORD		dwAttr;

	dwAttr = GetFileAttributesA(pFilePath);
	if(dwAttr == 0xFFFFFFFF)
		return FALSE;

	if(dwAttr & FILE_ATTRIBUTE_DIRECTORY)
		return TRUE;

	return FALSE;
}



BOOL	NFile::OpenFileDlg(CHAR *pStrFile, HWND hParWnd, CHAR * pTitle, CHAR * pFilter)
{
	CHAR			szFileName[1024] = "";
	OPENFILENAMEA	ofn ;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hParWnd;
	ofn.lpstrFilter = pFilter;
	ofn.nFilterIndex = 0;
	ofn.nMaxFile = MAX_PATH ;
	ofn.lpstrTitle = pTitle;
	ofn.lpstrFile = szFileName;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER;

	if(!GetOpenFileNameA( &ofn))//Ñ¡ÔñÁËÈ¡Ïû°´Å¥
		return FALSE;

	strcpy(pStrFile, szFileName);
	return TRUE;
}


BOOL	NFile::SaveFileDlg(CHAR *pStrFile, HWND hParWnd, CHAR * pTitle, CHAR * pFilter)
{
	CHAR			szFileName[1024] = "";
	OPENFILENAMEA	ofn ;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hParWnd;
	ofn.lpstrFilter = pFilter;
	ofn.nFilterIndex = 0;
	ofn.nMaxFile = MAX_PATH ;
	ofn.lpstrTitle = pTitle;
	ofn.lpstrFile = szFileName;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER;

	if(!GetSaveFileNameA( &ofn))//Ñ¡ÔñÁËÈ¡Ïû°´Å¥
		return FALSE;

	strcpy(pStrFile, szFileName);
	return TRUE;
}


static	int CALLBACK OpenFolderProc(
								HWND hwnd, 
								UINT uMsg, 
								LPARAM lParam, 
								LPARAM lpData
								)
{
	CHAR *		pUserPath;

	switch(uMsg)
	{
	case BFFM_INITIALIZED:
		SendMessageA(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	case BFFM_SELCHANGED:
		break;
	case BFFM_VALIDATEFAILED:
		pUserPath = (CHAR *)lParam;
		break;
	}

	return 0;
}


BOOL	NFile::OpenFolderDlg(HWND hWnd, CHAR *pFileName, CHAR *pTitle)
{
	CHAR		szPath[MAX_PATH]={0};
	BROWSEINFOA	BroInfo;
	IMalloc *	pMalloc;

	BroInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_VALIDATE | BIF_EDITBOX; 
	BroInfo.hwndOwner = hWnd;
	BroInfo.pidlRoot = NULL;
	BroInfo.lpszTitle = pTitle;
	BroInfo.lpfn = OpenFolderProc;
	BroInfo.lParam = (LPARAM)pFileName;
	BroInfo.pszDisplayName = szPath;

	CoInitialize(NULL);

	LPITEMIDLIST pidl = ::SHBrowseForFolderA(&BroInfo);

	if(pidl)
	{
		if( !::SHGetPathFromIDListA(pidl, szPath) )  
			szPath[0]=0;

		pMalloc = NULL;
		if( SUCCEEDED(::SHGetMalloc(&pMalloc)) )  // È¡µÃIMalloc·ÖÅäÆ÷½Ó¿Ú
		{
			pMalloc->Free(pidl);    // ÊÍ·ÅÄÚ´æ
			pMalloc->Release();       // ÊÍ·Å½Ó¿Ú
		}

		strcpy(pFileName, szPath);
		return TRUE;
	}

	return FALSE;
}


//pName	"CompanyName", "FileDescription", "FileVersion",
//		"InternalName", "LegalCopyright", "OriginalFilename",
//		"ProductName", "ProductVersion",
BOOL	NFile::GetFileVersions(CHAR * pFileName, _bstr_t * pRecvStr, CHAR * pName /* = NULL */)
{
	DWORD	dwDummyHandle; // will always be set to zero
	DWORD	dwLen;
	BYTE	* pVersionInfo;
	BOOL	bRetVal;

	VS_FIXEDFILEINFO	FileVersion;
	*pRecvStr = "";

	struct TRANSLATION {
		WORD langID;			// language ID
		WORD charset;			// character set (code page)
	} Translation;

	HMODULE		hVerDll;
	hVerDll = LoadLibraryA("VERSION.dll");
	if(hVerDll == NULL)
		return FALSE;

	typedef DWORD (WINAPI *Fun_GetFileVersionInfoSizeA)(CHAR *, DWORD *);
	typedef BOOL  (WINAPI *Fun_GetFileVersionInfoA)(CHAR *, DWORD, DWORD, LPVOID);
	typedef BOOL  (WINAPI *Fun_VerQueryValueA)(LPVOID, CHAR *, LPVOID, PUINT);


	Fun_GetFileVersionInfoSizeA		pGetFileVersionInfoSizeA;
	Fun_GetFileVersionInfoA			pGetFileVersionInfoA;
	Fun_VerQueryValueA				pVerQueryValueA;

	pGetFileVersionInfoSizeA = (Fun_GetFileVersionInfoSizeA)::GetProcAddress(hVerDll, "GetFileVersionInfoSizeA");
	pGetFileVersionInfoA	 = (Fun_GetFileVersionInfoA)::GetProcAddress(hVerDll, "GetFileVersionInfoA");
	pVerQueryValueA			 = (Fun_VerQueryValueA)::GetProcAddress(hVerDll, "VerQueryValueA");

	Translation.langID = 0x0409;	//
	Translation.charset = 1252;		// default = ANSI code page

	dwLen = pGetFileVersionInfoSizeA(pFileName, &dwDummyHandle);
	if (dwLen == 0)
		return FALSE;

	pVersionInfo = new BYTE[dwLen]; // allocate version info
	bRetVal = pGetFileVersionInfoA(pFileName, 0, dwLen, pVersionInfo);
	if(bRetVal == FALSE)
	{
		delete [] pVersionInfo;
		return FALSE;
	}

	VOID	* pVI;
	UINT	uLen;

	bRetVal = pVerQueryValueA(pVersionInfo, "\\", &pVI, &uLen);
	if(bRetVal == FALSE)
	{
		delete [] pVersionInfo;
		return FALSE;
	}

	memcpy(&FileVersion, pVI, sizeof(VS_FIXEDFILEINFO));

	bRetVal = pVerQueryValueA(pVersionInfo, "\\VarFileInfo\\Translation", 
		&pVI, &uLen);
	if(bRetVal && uLen >= 4)
	{
		memcpy(&Translation, pVI, sizeof(TRANSLATION));
	}

	BREAKIF(FileVersion.dwSignature != VS_FFI_SIGNATURE);

	CHAR		szQuery[1024];
	CHAR		* pVal;
	UINT		iLenVal;

	if(pName == NULL)
	{
		sprintf(szQuery, "%d.%d.%d.%d",
			HIWORD(FileVersion.dwFileVersionMS), LOWORD(FileVersion.dwFileVersionMS),
			HIWORD(FileVersion.dwFileVersionLS), LOWORD(FileVersion.dwFileVersionLS));
		*pRecvStr = szQuery;
	}
	else
	{
		sprintf(szQuery, "\\StringFileInfo\\%04X%04X\\%s",
			Translation.langID, Translation.charset, pName);

		bRetVal = pVerQueryValueA(pVersionInfo, szQuery, (LPVOID*)&pVal, &iLenVal);
		if(bRetVal)
			*pRecvStr = pVal;
		else
			*pRecvStr = "";
	}

	delete [] pVersionInfo;
	return TRUE;
}


BOOL	NFile::GetFileVersions(CHAR * pFileName, FILE_VERSION * pVerInfo)
{
	DWORD	dwDummyHandle; // will always be set to zero
	DWORD	dwLen;
	BYTE	* pVersionInfo;
	BOOL	bRetVal;

	VS_FIXEDFILEINFO	FileVersion;

	memset(pVerInfo, 0, sizeof(FILE_VERSION));

	struct TRANSLATION {
		WORD langID;			// language ID
		WORD charset;			// character set (code page)
	} Translation;

	HMODULE		hVerDll;
	hVerDll = GetModuleHandleA("VERSION.dll");
	if(hVerDll == NULL)
        hVerDll = LoadLibraryA("VERSION.dll");
	if(hVerDll == NULL)
		return FALSE;

	typedef DWORD (WINAPI *Fun_GetFileVersionInfoSizeA)(CHAR *, DWORD *);
	typedef BOOL  (WINAPI *Fun_GetFileVersionInfoA)(CHAR *, DWORD, DWORD, LPVOID);
	typedef BOOL  (WINAPI *Fun_VerQueryValueA)(LPVOID, CHAR *, LPVOID, PUINT);


	Fun_GetFileVersionInfoSizeA		pGetFileVersionInfoSizeA;
	Fun_GetFileVersionInfoA			pGetFileVersionInfoA;
	Fun_VerQueryValueA				pVerQueryValueA;

	pGetFileVersionInfoSizeA = (Fun_GetFileVersionInfoSizeA)::GetProcAddress(hVerDll, "GetFileVersionInfoSizeA");
	pGetFileVersionInfoA	 = (Fun_GetFileVersionInfoA)::GetProcAddress(hVerDll, "GetFileVersionInfoA");
	pVerQueryValueA			 = (Fun_VerQueryValueA)::GetProcAddress(hVerDll, "VerQueryValueA");

	Translation.langID = 0x0409;	//
	Translation.charset = 1252;		// default = ANSI code page

	dwLen = pGetFileVersionInfoSizeA(pFileName, &dwDummyHandle);
	if (dwLen == 0)
		return FALSE;

	pVersionInfo = new BYTE[dwLen]; // allocate version info
	bRetVal = pGetFileVersionInfoA(pFileName, 0, dwLen, pVersionInfo);
	if(bRetVal == FALSE)
	{
		delete [] pVersionInfo;
		return FALSE;
	}

	VOID	* pVI;
	UINT	uLen;

	bRetVal = pVerQueryValueA(pVersionInfo, "\\", &pVI, &uLen);
	if(bRetVal == FALSE)
	{
		delete [] pVersionInfo;
		return FALSE;
	}

	memcpy(&FileVersion, pVI, sizeof(VS_FIXEDFILEINFO));

	bRetVal = pVerQueryValueA(pVersionInfo, "\\VarFileInfo\\Translation", 
		&pVI, &uLen);
	if(bRetVal && uLen >= 4)
	{
		memcpy(&Translation, pVI, sizeof(TRANSLATION));
	}

	BREAKIF(FileVersion.dwSignature != VS_FFI_SIGNATURE);

	CHAR		szQuery[1024];
	CHAR		* pVal;
	UINT		iLenVal;

	{
		sprintf(szQuery, "%d.%d.%d.%d",
			HIWORD(FileVersion.dwFileVersionMS), LOWORD(FileVersion.dwFileVersionMS),
			HIWORD(FileVersion.dwFileVersionLS), LOWORD(FileVersion.dwFileVersionLS));
		NStr::StrCopy(pVerInfo->szVersion, szQuery, MAX_PATH);
	}

	{
		sprintf(szQuery, "\\StringFileInfo\\%04X%04X\\CompanyName",
			Translation.langID, Translation.charset);

		bRetVal = pVerQueryValueA(pVersionInfo, szQuery, (LPVOID*)&pVal, &iLenVal);
		if(bRetVal)
			NStr::StrCopy(pVerInfo->szCompany, pVal, MAX_PATH);
		else
			NStr::StrCopy(pVerInfo->szCompany, "", MAX_PATH);
	}

	{
		sprintf(szQuery, "\\StringFileInfo\\%04X%04X\\FileDescription",
			Translation.langID, Translation.charset);

		bRetVal = pVerQueryValueA(pVersionInfo, szQuery, (LPVOID*)&pVal, &iLenVal);
		if(bRetVal)
			NStr::StrCopy(pVerInfo->szDescription, pVal, MAX_PATH);
		else
			NStr::StrCopy(pVerInfo->szDescription, "", MAX_PATH);
	}

	delete [] pVersionInfo;
	return TRUE;
}


BOOL	NFile::GetFileInModulePath(CHAR *pFilePath, CHAR *pFileName, HMODULE hMod)
{
	CHAR			szModPath[MAX_PATH*2];
	CHAR *			pFind;

    GetModuleFileNameA(hMod, szModPath, sizeof(szModPath));
	pFind = strrchr(szModPath, '\\');
	if(pFind == NULL)
	{
		pFilePath[0] = 0;
		return FALSE;
	}
	pFind[0] = 0;

	strcpy(pFilePath, szModPath);
	if(pFileName)
	{
		strcat(pFilePath, "\\");
		strcat(pFilePath, pFileName);
	}

	return TRUE;
}

BOOL	NFile::GetModuleDir(CHAR *pDir, HMODULE hMod, int nSize)
{
	CHAR			szModPath[MAX_PATH*2];
	CHAR *			pFind;

	GetModuleFileNameA(hMod, szModPath, sizeof(szModPath));
	pFind = strrchr(szModPath, '\\');
	if(pFind == NULL)
	{
		return FALSE;
	}
	pFind[0] = 0;

	NStr::StrCopy(pDir, szModPath, nSize);

	return TRUE;
}


BOOL	NFile::GetATempFile(CHAR *pFilePath, CHAR *pExName)
{
	CHAR		szTempDir[MAXPATH];
	CHAR		szTempName[MAXPATH];
	CHAR		szFilePath[MAXPATH];
	int			i;

	GetTempPathA(sizeof(szTempDir), szTempDir);
	NFile::GetFileLongName(szTempDir, szTempDir);
	NStr::StrTrimRight(szTempDir, "\\");

	for(i=0; i<500; i++)
	{
		NStr::CreateRandomString(szTempName, 8+1);
		sprintf(szFilePath, "%s\\%s%s", szTempDir, szTempName, pExName);
		if(NFile::FileExist(szFilePath))
			continue;

		strcpy(pFilePath, szFilePath);
		return TRUE;
	}

	strcpy(pFilePath, szTempDir);
	return FALSE;
}


HICON	NFile::GetFileSmallIcon(CHAR *pFilePath)
{
	SHFILEINFOA		SFI;
	DWORD			dwRetVal;
	HICON			hIcon;
	UINT			uRetVal;

	hIcon = NULL;
	if(pFilePath[0] == 0)
		return NULL;

	dwRetVal = (DWORD)SHGetFileInfoA(pFilePath, 0, &SFI, sizeof(SFI), SHGFI_SMALLICON | SHGFI_ICON);
	if(dwRetVal > 0)
		return SFI.hIcon;

	uRetVal = ExtractIconExA(pFilePath, 0, NULL, &hIcon, 1);
	if(uRetVal > 0)
	{
		return hIcon;
	}

	return hIcon;
}


HICON	NFile::GetFileSmallIcon(WCHAR *pFilePath)
{
	SHFILEINFOW		SFI;
	DWORD			dwRetVal;
	HICON			hIcon;
	UINT			uRetVal;

	hIcon = NULL;
	if(pFilePath[0] == 0)
		return NULL;

	dwRetVal = (DWORD)SHGetFileInfoW(pFilePath, 0, &SFI, sizeof(SFI), SHGFI_SMALLICON | SHGFI_ICON);
	if(dwRetVal > 0)
		return SFI.hIcon;

	uRetVal = ExtractIconExW(pFilePath, 0, NULL, &hIcon, 1);
	if(uRetVal > 0)
	{
		return hIcon;
	}

	return hIcon;
}


BOOL	NFile::FolderFileEnum(FILEENUMPROC FileEnumProc, VOID *ProcParam, CHAR *pPath, BOOL bSubDir)
{
	WIN32_FIND_DATAA		FindData;
	HANDLE					hFind;
	CHAR					szFindDir[MAX_PATH*2];
	CHAR					szFulllFindFile[MAX_PATH*2];
	BOOL					bRetVal;
	BOOL					bBreak;

	strcpy(szFindDir, pPath);
	if(szFindDir[strlen(szFindDir)-1] == '\\')
		szFindDir[strlen(szFindDir)-1] = 0;

	strcpy(szFulllFindFile, szFindDir);
	if(strstr(szFulllFindFile, "*") == NULL &&
		strstr(szFulllFindFile, "?") == NULL)
	{
		strcat(szFulllFindFile, "\\*.*");
	}

	ZeroMemory(&FindData, sizeof(FindData));
	hFind = FindFirstFileA(szFulllFindFile, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	for(bRetVal = TRUE;
		bRetVal;
		bRetVal = FindNextFileA(hFind, &FindData))
	{
		if(FindData.cFileName[0] == '.' &&
			(FindData.cFileName[1] == 0 || (FindData.cFileName[1] == '.' && FindData.cFileName[2] == 0))
			)
			continue;

		bBreak = FileEnumProc(ProcParam, szFindDir, &FindData);
		if(bBreak)
		{
			FindClose(hFind);
			return FALSE;
		}

		if( bSubDir &&
			(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			)
		{
			CHAR			szNewFolder[MAX_PATH*2];

			strcpy(szNewFolder, szFindDir);
			strcat(szNewFolder, "\\");
			strcat(szNewFolder, FindData.cFileName);

			FolderFileEnum(FileEnumProc, ProcParam, szNewFolder, TRUE);
		}
	}

	FindClose(hFind);

	return TRUE;
}



BOOL	WINAPI	GetFileCount_CallBack(VOID *pParam, CHAR *pFilePath, WIN32_FIND_DATAA *pFileData)
{
	int *			pCount;

	pCount = (int *)pParam;

	if(pFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		pCount[0] ++;
	else
		pCount[1] ++;

	return FALSE;
}


BOOL	NFile::GetFileCount(CHAR *pTestDir, int *pnFileCount, int *pnSubDirCount)
{
	int				nCount[2];
	BOOL			bRetVal;

	nCount[0] = 0;
	nCount[1] = 0;

	bRetVal = FolderFileEnum(GetFileCount_CallBack, nCount, pTestDir, TRUE);

	if(pnSubDirCount)
		*pnSubDirCount = nCount[0];
	if(pnFileCount)
		*pnFileCount = nCount[1];

	return bRetVal;
}


typedef struct _GetFileList_dat
{
	VOID *		pFileList;
	BOOL		bFullPath;
}GetFileList_dat;

BOOL	WINAPI	GetFileList_CallBack(VOID *pParam, CHAR *pFileFolder, WIN32_FIND_DATAA *pFileData)
{
	GetFileList_dat *				pDat;
	std::list<std::string> *		pFileList;
	CHAR		szFullPath[MAXPATH];

	pDat = (GetFileList_dat *)pParam;
	pFileList = (std::list<std::string> *)pDat->pFileList;

	if(pDat->bFullPath)
	{
        sprintf(szFullPath, "%s\\%s", pFileFolder, pFileData->cFileName);
    	pFileList->push_back(szFullPath);
	}
	else
	{
		pFileList->push_back(pFileData->cFileName);
	}

	return FALSE;
}

BOOL	NFile::GetFileList(std::list<std::string> &FileList, CHAR *pFolder, CHAR *pFindType,  BOOL bFullPath)
{
	CHAR		szFullPath[MAXPATH];
	BOOL		bRetVal;
	GetFileList_dat	CalbakDat;

	CalbakDat.bFullPath = bFullPath;
	CalbakDat.pFileList = &FileList;

	FileList.clear();
	sprintf(szFullPath, "%s\\%s", pFolder, pFindType);

	bRetVal = FileFindEnum(GetFileList_CallBack, &CalbakDat, szFullPath);

	return bRetVal;
}


BOOL	WINAPI	GetFileList_CallBackW(VOID *pParam, WCHAR *pFileFolder, WIN32_FIND_DATAW *pFileData)
{
	GetFileList_dat *				pDat;
	std::list<std::wstring> *		pFileList;
	WCHAR		szFullPath[MAXPATH];

	pDat = (GetFileList_dat *)pParam;
	pFileList = (std::list<std::wstring> *)pDat->pFileList;

	if(pDat->bFullPath)
	{
		wsprintfW(szFullPath, L"%s\\%s", pFileFolder, pFileData->cFileName);
		pFileList->push_back(szFullPath);
	}
	else
	{
		pFileList->push_back(pFileData->cFileName);
	}

	return FALSE;
}


BOOL	NFile::GetFileListW(std::list<std::wstring> &FileList, WCHAR *pFolder, WCHAR *pFindType, BOOL bFullPath)
{
	WCHAR		szFullPath[MAXPATH];
	BOOL		bRetVal;
	GetFileList_dat	CalbakDat;

	CalbakDat.bFullPath = bFullPath;
	CalbakDat.pFileList = &FileList;

	FileList.clear();
	wsprintfW(szFullPath, L"%s\\%s", pFolder, pFindType);

	bRetVal = FileFindEnumW(GetFileList_CallBackW, &CalbakDat, szFullPath);

	return bRetVal;
}


BOOL	NFile::QuickEnumFolder(NFile::QFILEENUMPROC fnCallBack, WCHAR *pPath, VOID *pParam)
{
	WCHAR		szFindPath[MAXPATH];
	WCHAR		szSubPath[MAXPATH];
	HANDLE		hFind;
	BOOL		bContinue;\
		BOOL		bRetVal;
	WIN32_FIND_DATAW	FindData;

	wcscpy(szFindPath, pPath);
	wcscat(szFindPath, L"\\*");
	ZeroMemory(&FindData, sizeof(FindData));
	hFind = FindFirstFileW(szFindPath, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	for(bRetVal = TRUE;
		bRetVal;
		bRetVal = FindNextFileW(hFind, &FindData))
	{
		if(FindData.cFileName[0] == '.' &&
			(FindData.cFileName[1] == 0 || (FindData.cFileName[1] == '.' && FindData.cFileName[2] == 0))
			)
			continue;

		if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			wcscpy(szSubPath, pPath);
			wcscat(szSubPath, L"\\");
			wcscat(szSubPath, FindData.cFileName);

			QuickEnumFolder(fnCallBack, szSubPath, pParam);
		}

		bContinue = fnCallBack(pParam, pPath, &FindData);
		if(bContinue == FALSE)
			break;
	}

	FindClose(hFind);
	return bContinue;
}


BOOL	WINAPI QuickGetCountProc(VOID *pParam, WCHAR *pFolderPath, WIN32_FIND_DATAW *pFileData)
{
	NFile::FolderFileCountInfo *pCountInfo;

	pCountInfo = (NFile::FolderFileCountInfo *)pParam;
	if(*pCountInfo->pbStop)
		return FALSE;

	pCountInfo->uuTotalCount ++;
	return TRUE;
}


BOOL	NFile::QuickGetFolderFileCount(CHAR *pDir, NFile::FolderFileCountInfo *pCountInfo)
{
	CHAR		szFilePath[MAXPATH];
	WCHAR		wzFindPath[MAXPATH];
	BOOL		bRetVal;

	strcpy(szFilePath, pDir);
	NStr::StrTrimRight(szFilePath, "\\");
	NStr::Char2WChar(szFilePath, wzFindPath);

	memset(pCountInfo, 0, sizeof(FolderFileCountInfo));
	bRetVal = QuickEnumFolder(QuickGetCountProc, wzFindPath, pCountInfo);

	return bRetVal;
}


CHAR *	NFile::GetFilePathInTemp(CHAR *pName, CHAR *pFilePath)
{
	static CHAR			s_szRetPath[MAX_PATH*2];

	if(pFilePath == NULL)
		pFilePath = s_szRetPath;

	GetTempPathA(MAX_PATH*2, pFilePath);

	return pFilePath;
}


BOOL	NFile::FoceCopyFile(CHAR *pSorFile, CHAR *pDestFile, CHAR *pTempDir)
{
	BOOL		bRetVal;
	CHAR		szTempDir[MAXPATH];
	CHAR		szTempFile[MAXPATH];
	CHAR		szName[MAXPATH];
	DWORD		dwRetVal;
	DWORD		dwAttr;

	dwAttr = GetFileAttributesA(pDestFile);
    if(dwAttr != -1)
    {
        SetFileAttributesA(pDestFile, FILE_ATTRIBUTE_NORMAL);
    }

	bRetVal = CopyFileA(pSorFile, pDestFile, FALSE);
	if(bRetVal)
		return bRetVal;

	NFile::SpliteFilePath(pDestFile, szTempDir, szName);
	if(pTempDir == NULL)
	{
		pTempDir = szTempDir;
	}
	dwRetVal = GetTempFileNameA(pTempDir, szName, 0, szTempFile);
	if(dwRetVal == 0)
	{
		pTempDir = szTempDir;
		dwRetVal = GetTempFileNameA(pTempDir, szName, 0, szTempFile);
	}

	bRetVal = MoveFileExA(pDestFile, szTempFile, MOVEFILE_REPLACE_EXISTING);
	if(bRetVal == FALSE)
	{
		return FALSE;
	}
	bRetVal = MoveFileExA(szTempFile, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	if(bRetVal == FALSE)
	{
		bRetVal = FALSE;
	}

	bRetVal = CopyFileA(pSorFile, pDestFile, FALSE);
	return bRetVal;
}


BOOL	NFile::DelFileNextBoot(CHAR *pFilePath)
{
	BOOL	bRetVal;

	bRetVal = MoveFileExA(pFilePath, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);

	return bRetVal;
}



BOOL	NFile::ReloadFileNeed(CHAR *pFileName, NFile::RELOAD_DATA *pData)
{
	WCHAR		szFilePath[MAXPATH];

	NStr::Char2WChar(pFileName, szFilePath, sizeof(szFilePath));

	return ReloadFileNeed(szFilePath, pData);
}


BOOL	NFile::ReloadFileNeed(WCHAR *pFileName, NFile::RELOAD_DATA *pData)
{
	static		NFile::RELOAD_DATA		Data = {0, 0, 0, 0};
	DWORD						dwCurTime;
	BY_HANDLE_FILE_INFORMATION	FileInfo;

	if(pData == NULL)
		pData = &Data;

	if(pData->dwMinTime)
	{
		dwCurTime = GetTickCount();
		if(dwCurTime > pData->dwLastTime &&
			(dwCurTime - pData->dwLastTime) < pData->dwMinTime)
			return FALSE;

		pData->dwLastTime = dwCurTime;
	}

	memset(&FileInfo, 0, sizeof(FileInfo));
	GetFileInfo(pFileName, &FileInfo);
	if(FileInfo.nFileSizeLow != pData->dwSize)
	{
		pData->dwSize = FileInfo.nFileSizeLow;
		memcpy(&pData->EditTime, &FileInfo.ftLastWriteTime, sizeof(FILETIME));
		return TRUE;
	}
	if(memcmp(&pData->EditTime, &FileInfo.ftLastWriteTime, sizeof(FILETIME)) != 0)
	{
		pData->dwSize = FileInfo.nFileSizeLow;
		memcpy(&pData->EditTime, &FileInfo.ftLastWriteTime, sizeof(FILETIME));
		return TRUE;
	}

	return FALSE;
}


BOOL   NFile::DelSelfByBat(BOOL bDelExeTree)
{
	CHAR					szModule[MAXPATH];
	CHAR					szTree[MAXPATH];
	DWORD					dwRetVal;
	STARTUPINFOA			si;
	PROCESS_INFORMATION		pi;
	BOOL					bRetVal;
	CHAR					szFileData[4096];
	CHAR					szBatFile[MAXPATH];
	int						nOneceDelay;

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow =  SW_HIDE;

	GetTempPathA(sizeof(szBatFile), szBatFile);   
	GetTempFileNameA(szBatFile, "DelSelf", 0, szBatFile);
	strcat(szBatFile, ".bat");

	dwRetVal = GetModuleFileNameA(0, szModule, sizeof(szModule));
	if(dwRetVal == 0)
		return FALSE;

	NFile::SpliteFilePath(szModule, szTree);

	nOneceDelay = 1;
	sprintf(szFileData, 
		"cd\\"								STRING_RN
		":delexe"							STRING_RN
		"ping -n %d 127.0.0.1"				STRING_RN
		"del \"%s\""						STRING_RN
		"if exist \"%s\" goto :delexe"		STRING_RN
		"del \"%s\" /Q /F /A"				STRING_RN
		"rd \"%s\""							STRING_RN
		"del \"%s\""						STRING_RN
		,
		nOneceDelay,	//ping
		szModule,		//del
		szModule,		//if exist
		szTree,			//del
		szTree,			//rd
		szBatFile,		//del
		0);

	NLog::Logs(szFileData, szBatFile);
	NLog::DbgLog("File: %s  Data:\r\n%s", szBatFile, szFileData);


#ifdef _DEBUG
	strcat(szBatFile, " > C:\\DelSelfByBat.log");
#endif

	bRetVal = CreateProcessA(NULL, szBatFile, NULL, NULL, FALSE, 0,
		NULL, NULL, &si, &pi);
	if(bRetVal == FALSE)
	{
		return FALSE;
	}
	SetThreadPriority(pi.hThread, THREAD_PRIORITY_IDLE);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return   TRUE;
}


BOOL	NFile::GetDropFilePath(HDROP hDropInfo, CHAR *pFullPath, int nBufSize)
{
	UINT		nChars;
	int			nFileCount;

	pFullPath[0] = 0;
	nFileCount = ::DragQueryFileA(hDropInfo, 0xFFFFFFFF, NULL, nBufSize);
	if(nFileCount <= 0)
		return FALSE;

	nChars = ::DragQueryFileA(hDropInfo, 0, pFullPath, nBufSize);
	::DragFinish(hDropInfo);

	if(nChars <= 0)
		return FALSE;

	return TRUE;
}


BOOL	NFile::FormatFilePath(CHAR *pPath, CHAR *pBuffer)
{
	CHAR		szTemp[MAXPATH];
	CHAR *		pConvert;
	CHAR		szResult[MAXPATH];
	CHAR *		pHead;
	DWORD		dwRetVal;

	if(pBuffer == NULL)
		pBuffer = pPath;

	strcpy(szResult, pPath);
	pConvert = szResult;

	NStr::StrTrimTwoSide(pConvert, "\" ");

	pHead = "\\??\\";
	if(NStr::StrHeadCmp(pConvert, pHead) == 0)
	{
		pConvert = pConvert + strlen(pHead);
	}

	pHead = "\\Systemroot";
	if(NStr::StrHeadCmp(pConvert, pHead) == 0)
	{
		strcpy(szTemp, pConvert + strlen(pHead));
		NSys::GetSystemPath(1, szResult, sizeof(szResult));
		strcat(szResult, szTemp);
		pConvert = szResult;
	}

	pHead = "System32\\";
	if(NStr::StrHeadCmp(pConvert, pHead) == 0)
	{
		strcpy(szTemp, pConvert + strlen(pHead));
		NSys::GetSystemPath(SYSPATH_SystemDirectory, szResult, sizeof(szResult));
		strcat(szResult, "\\");
		strcat(szResult, szTemp);
		pConvert = szResult;
	}

	pHead = "\\Windows\\";
	if(NStr::StrHeadCmp(pConvert, pHead) == 0)
	{
		strcpy(szTemp, pConvert + strlen(pHead));
		NSys::GetSystemPath(SYSPATH_WindowsDirectory, szResult, sizeof(szResult));
		strcat(szResult, "\\");
		strcat(szResult, szTemp);
		pConvert = szResult;
	}

	strcpy(szTemp, pConvert);
	dwRetVal = GetLongPathNameA(pConvert, szTemp, sizeof(szTemp));
	strcpy(pBuffer, szTemp);

	if(dwRetVal == 0)
		return FALSE;

	return TRUE;
}


BOOL	NFile::GetFirstFile(CHAR *pFindPath, CHAR *pRecvPath)
{
	CHAR				szFindDir[MAXPATH];
	HANDLE				hFind;
	WIN32_FIND_DATAA	FindDat;
	BOOL				bRetVal;

	NFile::SpliteFilePath(pFindPath, szFindDir);
	hFind = FindFirstFileA(pFindPath, &FindDat);
	if(hFind == NULL || hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	for(;;)
	{
		if(strcmp(FindDat.cFileName, ".") != 0 &&
			strcmp(FindDat.cFileName, "..") != 0)
			break;

		bRetVal = FindNextFileA(hFind, &FindDat);
		if(bRetVal == FALSE)
		{
			FindClose(hFind);
			return FALSE;
		}
	}

	sprintf(pRecvPath, "%s\\%s", szFindDir, FindDat.cFileName);
	FindClose(hFind);

	return TRUE;
}


BOOL	NFile::CreateDirTree(CHAR *pTreePath)
{
	CHAR		szParDir[MAXPATH];
	BOOL		bRetVal;

	bRetVal = NFile::SpliteFilePath(pTreePath, szParDir);
	if(bRetVal == FALSE)
		return FALSE;

	bRetVal = CreateDirTree(szParDir);
	bRetVal = CreateDirectoryA(pTreePath, NULL);

	return bRetVal;
}


BOOL	NFile::ParseURL(CHAR *pFullUrl, CHAR *pHostName, int *pnPort, CHAR *pSubPath, CHAR *pFileName)
{
	URL_COMPONENTSA		UrlCop;
	CHAR				szHostName[1024];
	CHAR				szSubUrl[1024];
	BOOL				bRetVal;

	memset(szHostName, 0, sizeof(szHostName));
	memset(szSubUrl, 0, sizeof(szSubUrl));
	memset(&UrlCop, 0, sizeof(UrlCop));
	UrlCop.dwStructSize = sizeof(UrlCop);
	UrlCop.lpszHostName = szHostName;
	UrlCop.dwHostNameLength = sizeof(szHostName);
	UrlCop.lpszUrlPath = szSubUrl;
	UrlCop.dwUrlPathLength = sizeof(szSubUrl);

	bRetVal = InternetCrackUrlA(pFullUrl, 0, ICU_DECODE, &UrlCop);
	if(pHostName)
		strcpy(pHostName, szHostName);
	if(pnPort)
		*pnPort = UrlCop.nPort;
	if(pSubPath)
		strcpy(pSubPath, szSubUrl);
	if(pFileName)
		strcpy(pFileName, "NotImp");

	return bRetVal;
}

//	ÎÞ·¨¼àÊÓ¸ùÄ¿Â¼
BOOL	NFile::WaitForFileChange(CHAR *pFilePath, DWORD dwTimeOut)
{
	HANDLE		hFileChange;
	DWORD		dwRetVal;
	CHAR		szFileDir[MAXPATH];
	CHAR		szFileName[MAXPATH];
	BOOL		bRetVal;
	BOOL		bResult;

	BY_HANDLE_FILE_INFORMATION	FileInfoSave;
	BY_HANDLE_FILE_INFORMATION	FileInfoNew;

	hFileChange = NULL;
	bRetVal = NFile::GetFileInfo(pFilePath, &FileInfoSave);
	if(bRetVal == FALSE)
		return FALSE;

	SpliteFilePath(pFilePath, szFileDir, szFileName);
	strcat(szFileDir, "\\");

	hFileChange = FindFirstChangeNotificationA(szFileDir, FALSE, 
		FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE);
	if(hFileChange == NULL || 
		hFileChange == INVALID_HANDLE_VALUE)
		return FALSE;

	bResult = TRUE;
	for(;;)
	{
		dwRetVal = WaitForSingleObject(hFileChange, dwTimeOut);
		if(dwRetVal == WAIT_TIMEOUT)
		{
			bResult = FALSE;
			goto WaitForFileChange_Done;
		}

		bRetVal = NFile::GetFileInfo(pFilePath, &FileInfoNew);
		if(bRetVal == FALSE)
			goto WaitForFileChange_Done;

		if(memcmp(&FileInfoSave.ftLastWriteTime, &FileInfoNew.ftLastWriteTime, sizeof(FileInfoSave.ftLastWriteTime)) != 0)
			goto WaitForFileChange_Done;

		if(FileInfoSave.nFileSizeLow != FileInfoNew.nFileSizeLow)
			goto WaitForFileChange_Done;

		bRetVal = FindNextChangeNotification(hFileChange);
	}

WaitForFileChange_Done:
	if(hFileChange)
		FindCloseChangeNotification(hFileChange);

	return TRUE;
}


BOOL	NFile::WriteFile(HANDLE hFile, VOID *pData, int nSize)
{
	DWORD			dwWrite;
	DWORD			dwSize;
	BOOL			bRetVal;

	if(nSize == -1)
		dwSize = (DWORD)strlen((CHAR *)pData);
	else
		dwSize = (DWORD)nSize;

	bRetVal = ::WriteFile(hFile, pData, dwSize, &dwWrite, NULL);
	if(bRetVal == FALSE || dwWrite != dwSize)
		return FALSE;

	return TRUE;
}


int		NFile::ReadFile(HANDLE hFile, VOID *pData, int nSize)
{
	DWORD			dwRead;
	BOOL			bRetVal;

	((BYTE *)pData)[0] = 0;
	bRetVal = ::ReadFile(hFile, pData, nSize, &dwRead, NULL);
	if(bRetVal == FALSE)
		return 0;

	if(dwRead != nSize)
        ((BYTE *)pData)[dwRead] = 0;
	return (int)dwRead;
}


BOOL	NFile::WriteFile(HANDLE hFile, ULONGLONG uuFilePos, VOID *pData, int nSize)
{
	LARGE_INTEGER		llPos;
	BOOL				bRetVal;

	llPos.QuadPart = uuFilePos;
	SetFilePointer(hFile, llPos.LowPart, &llPos.HighPart, FILE_BEGIN);
	bRetVal = WriteFile(hFile, pData, nSize);

	return bRetVal;
}

int		NFile::ReadFile(HANDLE hFile, ULONGLONG uuFilePos, VOID *pData, int nSize)
{
	LARGE_INTEGER		llPos;
	BOOL				bRetVal;

	llPos.QuadPart = uuFilePos;
	SetFilePointer(hFile, llPos.LowPart, &llPos.HighPart, FILE_BEGIN);
	bRetVal = ReadFile(hFile, pData, nSize);

	return bRetVal;
}



BOOL	NFile::FilePropDlg(CHAR *pFilePath, HWND hWnd)
{
	SHELLEXECUTEINFOA   sInfo;
	BOOL				bRetVal;

	sInfo.hwnd			= hWnd;
	sInfo.lpVerb		= "properties";
	sInfo.lpFile		= pFilePath;
	sInfo.lpDirectory	= NULL;
	sInfo.lpParameters	= NULL;
	sInfo.nShow			= SW_SHOWNORMAL;
	sInfo.fMask			= SEE_MASK_INVOKEIDLIST;
	sInfo.lpIDList		= NULL;
	sInfo.cbSize		= sizeof(SHELLEXECUTEINFO);

	bRetVal = ::ShellExecuteExA(&sInfo);

	return bRetVal;
}

BOOL	NFile::CreateBigFile(CHAR *pFilePath, ULONGLONG uuSize)
{
	CAutoHandle			hFile;
	CAutoHandle			hMapFile;
	ULARGE_INTEGER		uuFileSize;

	hFile = CreateFileA(pFilePath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	uuFileSize.QuadPart = uuSize;
	hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, uuFileSize.HighPart, uuFileSize.LowPart, NULL);
	if( hMapFile == NULL )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////////////////////
//					NData
/////////////////////////////////////////////////////////////////////////////////////////////

DWORD	NData::GetBigDataMax(VOID *ptr, DWORD dwCount, DWORD *pMaxIndex, DWORD dwMaxCount, DATA_PTR_TPYE type)
{
	DWORD				* pTestBuf;
	DWORD				* pTestOrder;
	DWORD				* pFindData;
	DWORD				* pFindDataIndex;
	DWORD				* pFindDataSortIndex;
	DWORD				dwTestCount;
	DWORD				dwCriticalVal;
	DWORD				dwCriticalIndex;
	DWORD				dwCriticalCount;
	DWORD				i;
	DWORD				j;
	DWORD				dwIndex;
	DWORD				dwFindDataMaxCount;
	DWORD				dwFindDataRealCount;
	DWORD				dwValue;
	DWORD				dwScanExK = 6;		//À©³äÉ¨Ãè±¶Êý



	if(dwCount < dwMaxCount)
		return 0;


	if(dwCount < 5)
	{
		dwTestCount = dwCount;
	}
	else if(dwCount < 10)
	{
		dwTestCount = dwCount * 60/100;
	}
	else if(dwCount < 1000)
	{
		dwTestCount = dwCount * 6/100;
		dwScanExK = 6;
	}
	else if(dwCount < 100000)
	{
		dwTestCount = dwCount * 4/100;
		dwScanExK = 13;
	}
	else if(dwCount < 10000000)
	{
		dwTestCount = dwCount * 2/100;
		dwScanExK = 26;
	}
	else
	{
		dwTestCount = dwCount * 1/100;
		dwScanExK = 62;
	}


	dwFindDataMaxCount = dwMaxCount*dwScanExK*dwScanExK;


	pTestBuf = new DWORD [dwTestCount];
	pTestOrder = new DWORD [dwTestCount];
	pFindData = new DWORD [dwFindDataMaxCount];
	pFindDataIndex = new DWORD [dwFindDataMaxCount];
	pFindDataSortIndex = new DWORD [dwFindDataMaxCount];
	
	srand(GetTickCount());
	for(i=0; i<dwTestCount; i++)
	{
		dwIndex = rand() % dwCount;
		pTestBuf[i] = ((DWORD *)ptr)[dwIndex];
	}

	qSort(pTestBuf, pTestOrder, dwTestCount, DataPtrType_DWORD);

	dwCriticalCount = (DWORD)((DWORDLONG)dwMaxCount * dwScanExK * dwTestCount / dwCount);

	dwCriticalIndex = pTestOrder[dwTestCount - dwCriticalCount];
	dwCriticalVal = pTestBuf[dwCriticalIndex];


	for(j=0, i=0; i<dwCount; i++)
	{
		dwValue = ((DWORD *)ptr)[i];
		if( dwValue < dwCriticalVal)
			continue;

		pFindData[j] = dwValue;
		pFindDataIndex[j] = i;
		j ++;
	}

	dwFindDataRealCount = j;

	assert(dwFindDataRealCount < dwFindDataMaxCount);
	assert(dwFindDataRealCount > dwMaxCount);

	qSort(pFindData, pFindDataSortIndex, dwFindDataRealCount, DataPtrType_DWORD);

	NLog::DbgLog("GetBigDataMax: Count: %d OutMaxCount: %d \r\n"
		"CriticalCount: %d, FindDataRealCount: %d"
		, dwCount, dwMaxCount, dwCriticalCount, dwFindDataRealCount);

	DWORD			dwFindDataIndex;
	for(i=0; i<dwMaxCount; i++)
	{
		dwFindDataIndex =  pFindDataSortIndex[dwFindDataRealCount-i-1];
		pMaxIndex[i] = pFindDataIndex[dwFindDataIndex];
	}


	delete [] pTestBuf;
	delete [] pTestOrder;
	delete [] pFindData;
	delete [] pFindDataSortIndex;

	return dwFindDataRealCount;
}


DWORD	NData::Sort(VOID * ptr, DWORD pOrder[], DWORD n, DATA_PTR_TPYE type)
{
	DWORD i,j;
	DWORD* chain_p;		//ÏÂÒ»¸öÁ´µÄÏÂ±ê
	int cmpf,cthis,cold = -1,head;
	chain_p = (DWORD*) malloc(sizeof(DWORD) * n);
	head = 0;
	cold = 0;
	for(i=1;i<n;i++)
	{
		cthis = head;		//±éÀúÁ´
#ifdef _Sort_DBG
		char buf[1024],buf1[1024];
		DWORD tmp1;
		buf[0] = 0;
		tmp1 = head;
		for(j=0;j<i;j++)	
		{
			sprintf(buf1,"%d,",tmp1);
			strcat(buf,buf1);
			tmp1 = chain_p[tmp1];
		}
		strcat(buf,"\r\n");
		NLog::Logs(buf);
#endif
		for(j=0;j<i;j++)
		{
			switch(type)
			{
			case DataPtrType_DWORD :	cmpf = *((DWORD  *)ptr+i) >= *((DWORD  *)ptr+cthis) ? 1 : -1;	break;
			case DataPtrType_int   :	cmpf = *((int    *)ptr+i) >= *((int    *)ptr+cthis) ? 1 : -1;	break;
			case DataPtrType_float :	cmpf = *((float  *)ptr+i) >= *((float  *)ptr+cthis) ? 1 : -1;	break;
			case DataPtrType_double:	cmpf = *((double *)ptr+i) >= *((double *)ptr+cthis) ? 1 : -1;	break;
			case DataPtrType_CHAR:		cmpf = strcmp ( *((char**)ptr+i),*((char**)ptr+cthis)); 	break;
			case DataPtrType_CHAR_NoCase:	cmpf = stricmp( *((char**)ptr+i),*((char**)ptr+cthis)); 	break;
			default: cmpf = 0;
			}
			if(cmpf < 0)	//Ð¡ÓÚÔò´ÓÇ°Ãæ²åÈë
			{
				if(head == cthis)		//²åÈëµ½×îÇ°¶Ë
				{
					chain_p[i] = head;	//Ö¸ÏòÔ­À´µÄÍ·
					head = i;			//×Ô¼º³ÉÎªÍ·
				}
				else
				{
					chain_p[cold] = i;
					chain_p[i]	 = cthis;
				}
				break;
			}
			cold  = cthis;
			cthis = chain_p[cthis];		//ÏÂÒ»¸öÁ´
		}
		if(j >= i)		//±ÈËûÃÇÈ«²¿¶¼´ó ²åÈëµ½×îºó
		{
			chain_p[cold] = i;
		}
	}
	int		nNext;
	nNext = head;
	for(i=0;i<n;i++)
	{
		pOrder[i] = nNext;
		nNext = chain_p[nNext];
	}
	free(chain_p);
	return head;
}
#undef _Sort_DBG


double	NData::Lagrange(int n, double *px, double *py, double x)
{
	double s1, s2, y = 0.0;
	int i1, i2;

	for(i1=0; i1<n; i1++) 
	{
		s1 = 1.0;
		s2 = 1.0;

		for(i2=0; i2<n; i2++) 
		{
			if (i2 != i1) 
			{
				s1 *= (x - px[i2]);
				s2 *= (px[i1] - px[i2]);
			}
		}
		y += py[i1] * s1 / s2;
	}

	return y;
}


VOID *		g_qSortData = NULL;

int __cdecl MyqSortCompareDWORD(const void *elem1, const void *elem2 )
{
	DWORD			dwIndex1, dwIndex2;
	DWORD *		pData;

	dwIndex1 = *(DWORD *) elem1;
	dwIndex2 = *(DWORD *) elem2;
	pData = (DWORD *)g_qSortData;

	if(pData[dwIndex1] > pData[dwIndex2])
		return 1;
	else if(pData[dwIndex1] < pData[dwIndex2])
		return -1;

	return 0;
}

int __cdecl MyqSortCompareDouble(const void *elem1, const void *elem2 )
{
	DWORD			dwIndex1, dwIndex2;
	double *		pData;

	dwIndex1 = *(DWORD *) elem1;
	dwIndex2 = *(DWORD *) elem2;
	pData = (double *)g_qSortData;

	if(pData[dwIndex1] > pData[dwIndex2])
		return 1;
	else if(pData[dwIndex1] < pData[dwIndex2])
		return -1;

	return 0;
}

int __cdecl MyqSortCompareBYTE(const void *elem1, const void *elem2 )
{
	DWORD			dwIndex1, dwIndex2;
	BYTE *			pData;

	dwIndex1 = *(DWORD *) elem1;
	dwIndex2 = *(DWORD *) elem2;
	pData = (BYTE *)g_qSortData;

	if(pData[dwIndex1] > pData[dwIndex2])
		return 1;
	else if(pData[dwIndex1] < pData[dwIndex2])
		return -1;

	return 0;
}
DWORD	NData::qSort(VOID * ptr, DWORD pOrder[], DWORD dwDataCount, DATA_PTR_TPYE type)
{
	DWORD				i;

	switch(type)
	{
	case DataPtrType_DWORD:
		g_qSortData = ptr;

		for(i=0; i<dwDataCount; i++)
			pOrder[i] = i;

		qsort(pOrder, dwDataCount, sizeof(DWORD), MyqSortCompareDWORD);

		return dwDataCount;
		break;

	case DataPtrType_double:
		g_qSortData = ptr;

		for(i=0; i<dwDataCount; i++)
			pOrder[i] = i;

		qsort(pOrder, dwDataCount, sizeof(DWORD), MyqSortCompareDouble);

		return dwDataCount;
		break;

	case DataPtrType_BYTE:
		g_qSortData = ptr;

		for(i=0; i<dwDataCount; i++)
			pOrder[i] = i;

		qsort(pOrder, dwDataCount, sizeof(DWORD), MyqSortCompareBYTE);

		return dwDataCount;
		break;
	default:
		return 0;
	}

	return 0;
}



DWORD	NData::DataToHex(VOID * pMem, DWORD dwMemSize, _bstr_t *pbStrHex, int nOneLineChars)
{
	CHAR			* pBuf;
	DWORD			dwBufSize;
	DWORD			dwRetVal;

	dwBufSize = dwMemSize*2+5+dwMemSize*4/nOneLineChars;
	pBuf = (CHAR *)malloc(dwBufSize);

	dwRetVal = DataToHex(pMem, dwMemSize, pBuf, dwBufSize, nOneLineChars);

	*pbStrHex = pBuf;
	free(pBuf);

	return dwRetVal;
}

DWORD	NData::DataToHex(VOID * pMem, DWORD dwMemSize, CHAR *pHexStr, DWORD dwHexSize, int nOneLineChars)
{
	DWORD			i, j;
	int				nLinePos;
	BYTE			* pByte;
	int				nByteHi, nByteLo;

	if(dwHexSize < dwMemSize*2+1)
		return 0;

	nLinePos = 0;
	pByte = (BYTE *)pMem;
	for(i=0, j=0; i<dwMemSize; i++)
	{
		nByteHi = pByte[i] >> 4;
		nByteLo = pByte[i] & 0x0F;

		pHexStr[j] = HexTable[nByteHi];	
		j++;
		pHexStr[j] = HexTable[nByteLo];	
		j++;

		if(nOneLineChars <= 0)
			continue;

		nLinePos += 2;
		if(nLinePos >= nOneLineChars)
		{
			pHexStr[j] = '\r';
			j ++;
			pHexStr[j] = '\n';
			j ++;

			nLinePos = 0;
		}
	}
	pHexStr[j] = 0;

	return j;
}



static BYTE Hex2Val_Table[256] =
{
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

BOOL	NData::IsHexChar(int ch)
{
	if(ch >= '0' && ch <= '9')
		return TRUE;
	if(ch >= 'A' && ch <= 'F')
		return TRUE;
	if(ch >= 'a' && ch <= 'f')
		return TRUE;

	return FALSE;
}


BYTE	NData::Hex2Byte(CHAR c1, CHAR c2)
{
	BYTE			bByteHi, bByteLo;

	bByteHi = Hex2Val_Table[(BYTE)c1];
	bByteLo = Hex2Val_Table[(BYTE)c2];

	return 	((bByteHi << 4) | bByteLo);
}

ULONGLONG	NData::HexToInt64(CHAR * pHexString)
{
	ULONGLONG		uuValue;
	BYTE			bByte;

	uuValue = 0;
	for(; *pHexString; pHexString++)
	{
		bByte = Hex2Val_Table[*pHexString];
		if(bByte == 0xFF)
			break;

		uuValue <<= 4;
		uuValue |= bByte;
	}

	return uuValue;
}

DWORD	NData::HexToData(CHAR * pHexString, VOID * pMem, DWORD dwMemSize)
{
	DWORD			i, j;
	BYTE			* pByte;
	BYTE			bByteHi, bByteLo;

	pByte = (BYTE *)pMem;
	for(i=0, j=0; i<dwMemSize; j++)
	{
		bByteHi = pHexString[j];
		if(bByteHi == 0)
			break;

		bByteHi = Hex2Val_Table[bByteHi];
		if(bByteHi > 0x0F)
			continue;

		j++;

		bByteLo = pHexString[j];
		if(bByteLo == 0)
			break;

		bByteLo = Hex2Val_Table[bByteLo];
		if(bByteLo > 0x0F)
			continue;

#if 0
		struct ONEBYTE
		{
			BYTE	Hi:4;
			BYTE	Low:4;
		}OneByte;

		OneByte.Hi = bByteHi;
		OneByte.Low = bByteLo;
		pByte[i] = *(BYTE *)&OneByte;
#else
		pByte[i] = (bByteHi << 4) | bByteLo;
#endif


		i ++;
	}

	return i;
}

#define _KEY_NEXT(_KEY)	\
{\
	_KEY = (_KEY * 214013L + + 2531011L) >> 5;\
}\

DWORD	NData::SimpleCrypt(VOID *pMem, DWORD dwSize, int nKey)
{
	BYTE		* pData;
	DWORD		i;

	pData = (BYTE *)pMem;
	for(i=0; i<dwSize; i++)
	{
		_KEY_NEXT(nKey);
		pData[i] ^= nKey & 0xFF;
	}

	return i;
}


VOID *	NData::Align(VOID *p, int nSize)
{
	DWORD   dwVal;

	dwVal = (DWORD)p;
	dwVal += nSize-1;
	dwVal /= nSize;
	dwVal *= nSize;
	return  (VOID *)dwVal;
}


DWORD	NData::Align(DWORD dwVal, DWORD dwSize)
{
	dwVal += dwSize-1;
	dwVal /= dwSize;
	dwVal *= dwSize;
	return  dwVal;
}

ULONGLONG	NData::Align(ULONGLONG uuVal, ULONGLONG uuAglinSize)
{
	uuVal += uuAglinSize-1;
	uuVal /= uuAglinSize;
	uuVal *= uuAglinSize;
	return  uuVal;
}

VOID *	NData::MemFindMem(VOID * pMem, UINT uMemSize, VOID * pFind, UINT uFindSize)
{
	BYTE *		pCur;
	BYTE *		pEnd;
	BYTE *		p1;
	BYTE *		p2;
	BYTE *		p2End;

	if(pMem == NULL || pFind == NULL || uFindSize == 0)
		return NULL;

	pCur = (BYTE *)pMem;
	pEnd = (BYTE *)pMem + uMemSize - uFindSize;
	p2End = (BYTE *)pFind + uFindSize - 1;

	for(; pCur<=pEnd; pCur++)
	{
		p1 = pCur;
		p2 = (BYTE *)pFind;

		for(; *p1==*p2; )
		{
			if(p2 >= p2End)
				return pCur;
			p1++, p2++;
		}
	}

	return NULL;
}


VOID *	NData::MemFindStr(VOID * pMem, UINT uMemSize, CHAR * pString)
{
	BYTE *		pCur;
	BYTE *		pEnd;
	BYTE *		p1;
	BYTE *		p2;

	if(pMem == NULL || pString == NULL)
		return NULL;

	pCur = (BYTE *)pMem;
	pEnd = (BYTE *)pMem + uMemSize - strlen(pString);

	for(; pCur<=pEnd; pCur++)
	{
		p1 = pCur;
		p2 = (BYTE *)pString;

		while((*p1 == *p2))
		{
			if(p2[1] == 0)
				return pCur;

			p1++, p2++;
		}
	}

	return NULL;
}

//	×îÖÕÓÅ»¯: 5792  ±È ChrCmpIA ËÙ¶ÈÌá¸ß 30±¶
BOOL	inline	UprCharNoCaseEqu2(BYTE cUpr, BYTE c)
{
	if(cUpr == c)
		return TRUE;

	if((c-cUpr) != ('a' - 'A'))
		return FALSE;

	if(c < 'a' || c > 'z')
		return FALSE;

	return TRUE;
}

//	9308
BOOL	inline	UprCharNoCaseEqu1(BYTE cUpr, BYTE c)
{
	if(cUpr == c)
		return TRUE;

	if(c < 'a' || c > 'z')
		return FALSE;

	c -= ('a' - 'A');
	if(cUpr == c)
		return TRUE;

	return FALSE;
}


//	294471
BOOL	inline	UprCharNoCaseEqu(BYTE cUpr, BYTE c)
{
	return !ChrCmpIA(cUpr, c);
}


VOID *	NData::MemFindStrNoCase(CHAR *pString, VOID *pMem, UINT uMemSize)
{
	BYTE *		pCur;
	BYTE *		pEnd;
	BYTE *		p1;
	BYTE *		p2;
	UINT		uLen;
	BYTE		szUprString[1024];
	BYTE *		pUprString;
	BYTE *		pFree;

	if(pMem == NULL || pString == NULL)
		return NULL;

	if(uMemSize == -1)
		uMemSize = (UINT)strlen((CHAR *)pMem) + 1;

	uLen = (UINT)strlen(pString);
	if(uLen < sizeof(szUprString) - 1)
	{
		pUprString = szUprString;
		pFree = NULL;
	}
	else
	{
		pUprString = (BYTE *)malloc(uLen+1);
		pFree = pUprString;
	}

	strcpy((CHAR *)pUprString, pString);
	strupr((CHAR *)pUprString);

	pCur = (BYTE *)pMem;
	pEnd = (BYTE *)pMem + uMemSize - uLen;

	for(; pCur<=pEnd; pCur++)
	{
		p1 = pCur;
		p2 = (BYTE *)pUprString;

		while(UprCharNoCaseEqu(*p2, *p1))
		{
			if(p2[1] == 0)
			{
				if(pFree)
					free(pFree);
				return pCur;
			}

			p1++, p2++;
		}
	}

	if(pFree)
		free(pFree);
	return NULL;
}


DWORD	NData::HashData(VOID *pData, DWORD dwSize)
{
	DWORD	dwHash;
	DWORD	i;
	DWORD	dwHight;
	BYTE *	pByteData;

	pByteData = (BYTE *)pData;
	if(dwSize == 0)
		dwSize = (DWORD)strlen((CONST CHAR *)pData);

	dwHash = 0;
	for(i=0; i<dwSize; i++)
	{
		dwHash = (dwHash << 3) + pByteData[i];
		dwHight = dwHash >> 29;
		if(dwHight)
		{
			dwHash ^= dwHight;
		}
	}

	return dwHash;
}


int		NData::RandomInt(int nMin, int nMax)
{
	int		nRand;
	int		nRect;

	nRect = nMax - nMin;
	nRand = rand() % nRect;

	return (nMin+nRand);
}

//	10987654 32109876 54321098 76543210
//	´ÓÖ¸¶¨BitÎ»ÖÃ»ñÈ¡Ö¸¶¨ÊýÄ¿2½øÖÆ
DWORD	NData::GetBit(DWORD dwValue, int nBitPos, int nBitLen)
{
	DWORD		dwRetVal;
	DWORD		dwLeft;
	DWORD		dwRight;

	dwLeft = (32-nBitPos-1);
	dwRight = (32-nBitLen);
	dwRetVal = dwValue << dwLeft;
	dwRetVal = dwRetVal >> dwRight;

	return dwRetVal;
}


DWORD	NData::NextKey(DWORD dwKey)
{
	DWORD		dwResult;
	dwResult = (dwKey * 49 + 32653) >> 4;
	return dwResult;
}

//					NData
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
//					NNet
/////////////////////////////////////////////////////////////////////////////////////////////
CHAR	*NNet::Ip2String(DWORD dwIp, _bstr_t *pbStrIp)
{
	CHAR		szBuf[100];

	Ip2String(dwIp, szBuf);
	*pbStrIp = szBuf;

	return *pbStrIp;
}


CHAR	*NNet::Ip2String(DWORD dwIp, CHAR *pBuffer)
{
	static	CHAR	szBuf[32];
	BYTE			bVal[4];

	if(pBuffer == NULL)
		pBuffer = szBuf;

	memcpy(bVal, &dwIp, sizeof(DWORD));

	sprintf(pBuffer, "%d.%d.%d.%d", bVal[0], bVal[1], bVal[2], bVal[3]);
	return pBuffer;
}


DWORD	NNet::String2Ip(CHAR *pBuffer)
{
	return inet_addr(pBuffer);
}


BOOL	NNet::GetMacAddr(BYTE *pMac)
{
	PIP_ADAPTER_INFO			pAdapterInfo;
	DWORD						dwAdapterInfoSize;
	char						szMac[32] = {0};
	DWORD						dwErr;

	dwAdapterInfoSize = 0;
	dwErr = GetAdaptersInfo(NULL, &dwAdapterInfoSize);
	if((dwErr != 0) && (dwErr != ERROR_BUFFER_OVERFLOW))
	{
		return FALSE;
	}

	//   ·ÖÅäÍø¿¨ÐÅÏ¢ÄÚ´æ
	pAdapterInfo = (PIP_ADAPTER_INFO)malloc(dwAdapterInfoSize);
	if(pAdapterInfo == NULL) 
	{
		return FALSE;
	}

	if(GetAdaptersInfo(pAdapterInfo, &dwAdapterInfoSize) != 0)
	{
		free(pAdapterInfo);
		return FALSE;
	}

	for(; pAdapterInfo; pAdapterInfo=pAdapterInfo->Next)	//±éÀúÃ¿Ò»ÕÅÍø¿¨
	{
		if(strstr(pAdapterInfo->Description, "PPP"))
			continue;

		if(strstr(pAdapterInfo->Description, "ppp"))
			continue;

		if(stricmp(pAdapterInfo->Description, "Microsoft Loopback Adapter") == 0)
			continue;

		if(pAdapterInfo->Type != MIB_IF_TYPE_ETHERNET)
			continue;

		memcpy(pMac, pAdapterInfo->Address, sizeof(pAdapterInfo->Address));
		if(*(DWORD *)pMac != 0)
		{
			free(pAdapterInfo);
			return TRUE;
		}
	}

	free(pAdapterInfo);
	return FALSE;
}


BOOL	NNet::GetMacAddr(CHAR *pMac, CHAR *pSplite)
{
	BYTE		bzMac[8];
	BOOL		bRetVal;

	bRetVal = GetMacAddr(bzMac);
	if(bRetVal == FALSE)
		return FALSE;

	Mac2String(bzMac, pMac, pSplite);

	return TRUE;
}

CHAR *	NNet::Mac2String(BYTE *pMac, CHAR *pBuffer, CHAR *pSplit)
{
	static	CHAR	szBuf[64];

	if(pBuffer == NULL)
		pBuffer = szBuf;

	sprintf(pBuffer, "%02X%s%02X%s%02X%s%02X%s%02X%s%02X", 
		pMac[0], pSplit, 
		pMac[1], pSplit, 
		pMac[2], pSplit, 
		pMac[3], pSplit, 
		pMac[4], pSplit, 
		pMac[5]);

	return pBuffer;
}


BOOL	NNet::String2Mac(CHAR *pMacString, BYTE *pMac)
{
	DWORD	dwRetVal;

	memset(pMac, 0, 8);
	dwRetVal = NData::HexToData(pMacString, pMac, 6);

	if(dwRetVal == 6)
		return TRUE;

	return FALSE;
}


CHAR *	NNet::GetHostNameByIp(DWORD dwIp, CHAR *pHostName)
{
	static	CHAR	szBuf[64];
	hostent *		hp;

	if(pHostName == NULL)
		pHostName = szBuf;

	pHostName[0] = 0;
	hp = gethostbyaddr((CHAR *)(&dwIp), sizeof(dwIp), AF_INET);
	if(hp == NULL)
		return NULL;

	strcpy(pHostName, hp->h_name);
	return pHostName;
}


typedef struct _PROTO2NAME
{ 
	int  cProto ;
	char *pProtoName ;
}PROTO2NAME; 

PROTO2NAME _ProtoTable [ IPPROTO_MAX] = 
{  
	{ IPPROTO_IP   , "IP" },
	{ IPPROTO_ICMP , "ICMP" },  
	{ IPPROTO_IGMP , "IGMP" }, 
	{ IPPROTO_GGP  , "GGP" },  
	{ IPPROTO_TCP  , "TCP" },  
	{ IPPROTO_PUP  , "PUP" },  
	{ IPPROTO_UDP  , "UDP" },  
	{ IPPROTO_IDP  , "IDP" },  
	{ IPPROTO_ND   , "NP"  },  
	{ IPPROTO_RAW  , "RAW" },  
	{ IPPROTO_MAX  , "MAX" },
	{ -1 , "UnKnow" } 
} ;  

CHAR	*NNet::IpProtoName(unsigned char cProto, CHAR *pBuffer)
{
	int		i;

	for(i=0; _ProtoTable[i].cProto != -1; i++)
	{
		if(_ProtoTable[i].cProto == cProto)
		{
			if(pBuffer)
				strcpy(pBuffer, _ProtoTable[i].pProtoName);
			return _ProtoTable[i].pProtoName;
		}
	}

	if(pBuffer)
		strcpy(pBuffer, _ProtoTable[i].pProtoName);
	return _ProtoTable[i].pProtoName;
}


CHAR *	NNet::GetLocalHostIp(CHAR *pBuffer)
{
	CHAR		szHostName[MAX_PATH];   
	hostent *	pHostEnt;
	int			nRetVal;
	CHAR *		pIpString;

	strcpy(pBuffer, "");
	nRetVal = ::gethostname(szHostName, sizeof(szHostName));
	if(nRetVal == SOCKET_ERROR)
		return pBuffer;

	pHostEnt = ::gethostbyname(szHostName);
	if(pHostEnt == NULL)
		return pBuffer;

	if(pHostEnt->h_addr_list[0] == 0)
		return pBuffer;

	pIpString = ::inet_ntoa((in_addr &)pHostEnt->h_addr_list[0]);

	strcpy(pBuffer, pIpString);
	return pBuffer;
}

DWORD	NNet::GetLocalHostIp()
{
	CHAR szIp[MAX_PATH] = "";

	GetLocalHostIp(szIp);

	return String2Ip(szIp);
}


DWORD	NNet::GetHostIp(CHAR *pHostName)
{
	DWORD		dwIp;
	hostent *	pHostEnt;

	dwIp = -1;
	pHostEnt = ::gethostbyname(pHostName);
	if(pHostEnt == NULL)
		return dwIp;

	if(pHostEnt->h_addr_list[0] == 0)
		return dwIp;

	dwIp = *(DWORD *)pHostEnt->h_addr_list[0];

	return dwIp;
}


BOOL	NNet::SpliteServerPort(CHAR *pString, CHAR *pServer, int *pnPort)
{
	CHAR *		pFind;

	strcpy(pServer, pString);

	pFind = strchr(pServer, ':');
	if(pFind == NULL)
	{
		*pnPort = 0;
		return FALSE;
	}

	*pFind = 0;
	*pnPort = atoi(pFind+1);

	return TRUE;
}


BOOL	NNet::ClearCatchByUrl(CHAR *pUrl)
{
	DWORD							dwEntrySize;  
	INTERNET_CACHE_ENTRY_INFOA	*	pCacheEntry;
	BOOL				bRetVal;
	BOOL				bResult;

	bResult = FALSE;
	dwEntrySize = 0;
	pCacheEntry = NULL;
	bRetVal = GetUrlCacheEntryInfoA(pUrl, NULL, &dwEntrySize);
	if(bRetVal != FALSE || ERROR_INSUFFICIENT_BUFFER != GetLastError())
		goto ClearCatchByUrl_Done;

	pCacheEntry = (INTERNET_CACHE_ENTRY_INFOA *)malloc(dwEntrySize);
	bRetVal = GetUrlCacheEntryInfoA(pUrl, pCacheEntry, &dwEntrySize);
	if(bRetVal == FALSE)
		goto ClearCatchByUrl_Done;

	DeleteFileA(pCacheEntry->lpszLocalFileName);
	bResult = TRUE;

ClearCatchByUrl_Done:
	DeleteUrlCacheEntryA(pUrl);
	if(pCacheEntry)
		free(pCacheEntry);

	return bResult;
}


BOOL	NNet::GetWebServerByUrl(CHAR *pUrl, CHAR *pWebSvr, int nBufSize)
{
	URL_COMPONENTSA			UrlCop;
	CHAR		szHostName[MAXPATH];
	BOOL		bRetVal;

	memset(&UrlCop, 0, sizeof(UrlCop));
	UrlCop.dwStructSize = sizeof(UrlCop);
	UrlCop.lpszHostName = szHostName;
	UrlCop.dwHostNameLength = sizeof(szHostName);

	bRetVal = InternetCrackUrlA(pUrl, 0, ICU_DECODE, &UrlCop);
	if(bRetVal == FALSE)
	{
		*pWebSvr = 0;
		return FALSE;
	}

	NStr::StrCopy(pWebSvr, szHostName, nBufSize);
	return TRUE;
}



CHAR		szWebSuffix[][10] = 
{
	"com",
	"net",
	"org",
	"gov",
	"biz",
	"cc",
	""
};

BOOL	_CheckSuffix(CHAR *pFixName)
{
	int			i;

	for(i=0; szWebSuffix[i][0]; i++)
	{
		if(stricmp(szWebSuffix[i], pFixName) == 0)
			return TRUE;
	}

	return FALSE;
}

//	"www.Image.sina.com.cn"
//	Head:		www
//	Type:		com
//	Country:	cn
//	Layer:		Image.sina
BOOL	_SpliteWebFixName(CHAR *pFullName, CHAR *pLayerName,	CHAR *pHead, CHAR *pType, CHAR *pCountry)
{
	CHAR		szFullName[MAXPATH];
	CHAR		szHead[MAXPATH];
	CHAR		szType[MAXPATH];
	CHAR		szCountry[MAXPATH];
	CHAR *		pFind;

	strcpy(szFullName, pFullName);
	//	Country
	szCountry[0] = 0;
	pFind = strrchr(szFullName, '.');
	if(pFind == NULL)
		goto CountryDone;
	if(strlen(pFind) != 3)
		goto CountryDone;
	pFind[0] = 0;
	strcpy(szCountry, pFind+1);

CountryDone:

	//	Type
	szType[0] = 0;
	pFind = strrchr(szFullName, '.');
	if(pFind == NULL)
		goto TypeDone;
	if(_CheckSuffix(pFind+1) == FALSE)
		goto TypeDone;
	pFind[0] = 0;
	strcpy(szType, pFind+1);

TypeDone:

	//	Head
	szHead[0] = 0;
	pFind = szFullName;
	if(NStr::StrHeadCmp(szFullName, "www.") != 0)
		goto HeadDone;

	strncpy(szHead, szFullName, 4);
	szHead[4] = 0;
	pFind = szFullName+4;

HeadDone:

	strcpy(pLayerName, pFind);

	if(pHead)
		strcpy(pHead, szHead);
	if(pType)
		strcpy(pType, szType);
	if(pCountry)
		strcpy(pCountry, szCountry);

	return TRUE;
}


BOOL	NNet::GetWebServerTopName(CHAR *pWebSvr, CHAR *pTopName, int nBufSize)
{
	CHAR		szIp[100];
	DWORD		dwIp;
	CHAR *		pFind;
	CHAR		szLayerName[MAXPATH];

	NStr::StrCopy(pTopName, pWebSvr, nBufSize);

	dwIp = inet_addr(pWebSvr);
	Ip2String(dwIp, szIp);
	if(stricmp(pWebSvr, szIp) == 0)
	{
		return TRUE;	//	IP
	}

	_SpliteWebFixName(pWebSvr, szLayerName, NULL, NULL, NULL);
	pFind = strrchr(szLayerName, '.');
	if(pFind == NULL)
		NStr::StrCopy(pTopName, szLayerName, nBufSize);
	else
		NStr::StrCopy(pTopName, pFind+1, nBufSize);

	return TRUE;
}

BOOL	NNet::IpcFormatServer(CHAR *pServrPath, CHAR *pServer)
{
	CHAR			szServer[MAXPATH];

	strcpy(szServer, pServer);
	NStr::StrTrimTwoSide(szServer, "\\");
	sprintf(pServrPath, "\\\\%s", szServer);

	return TRUE;
}


BOOL	NNet::IpcConnect(CHAR *pServer, CHAR *pUser, CHAR *pPassword)
{
	NETRESOURCEA	NetRes;
	CHAR			szNetPath[MAXPATH];
	DWORD			dwRetVal;

	IpcFormatServer(szNetPath, pServer);
	memset(&NetRes, 0, sizeof(NetRes));
	NetRes.dwType = RESOURCETYPE_ANY;
	NetRes.lpRemoteName = szNetPath;
	dwRetVal = WNetAddConnection2A(&NetRes, pPassword, pUser, 0);
	if(dwRetVal == ERROR_SUCCESS)
		return TRUE;

	if(dwRetVal != ERROR_SESSION_CREDENTIAL_CONFLICT)
	{
		SetLastError(dwRetVal);
		return FALSE;
	}

	DelConnect(pServer);
	dwRetVal = WNetAddConnection2A(&NetRes, pPassword, pUser, 0);
	if(dwRetVal == ERROR_SUCCESS)
		return TRUE;

	SetLastError(dwRetVal);
	return FALSE;
}


BOOL	NNet::DelConnect(CHAR *pServer)
{
	CHAR			szNetPath[MAXPATH];
	DWORD			dwRetVal;

	IpcFormatServer(szNetPath, pServer);

	dwRetVal = WNetCancelConnection2A(szNetPath, 0, TRUE);
	if(dwRetVal == ERROR_SUCCESS)
		return TRUE;

	SetLastError(dwRetVal);
	return FALSE;
}


BOOL	NNet::IpcCreateRmtPath(CHAR *pNetPath, CHAR *pServer, CHAR *pPath)
{
	CHAR			szSvrPath[MAXPATH];

	IpcFormatServer(szSvrPath, pServer);
	strcpy(pNetPath, szSvrPath);
	strcat(pNetPath, "\\");
	strcat(pNetPath, pPath);

	NStr::StrReplace(pNetPath, ':', '$');

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//					NShell
ITEMIDLIST *	NShell::GetIDListByPath(CHAR *pFilePath)
{
	WCHAR		szFilePath[MAXPATH];

	NStr::Char2WChar(pFilePath, szFilePath, sizeof(szFilePath));

	return GetIDListByPath(szFilePath);
}

ITEMIDLIST *	NShell::GetIDListByPath(WCHAR *pFilePath)
{
	IShellFolder	*pDesktopFolder;
	HRESULT			hr;
	ITEMIDLIST		*pidl;


	pDesktopFolder = NULL;
	pidl = NULL;

	hr = SHGetDesktopFolder(&pDesktopFolder);
	if(FAILED(hr))
		goto GetIDListByPath_Done;

	hr = pDesktopFolder->ParseDisplayName(NULL, 0, pFilePath, NULL, &pidl, NULL);
	if(FAILED(hr))
		goto GetIDListByPath_Done;


GetIDListByPath_Done:

	if(pDesktopFolder)
		pDesktopFolder->Release();

	return pidl;
}


BOOL	NShell::OpenFolderAndSelect(LPCITEMIDLIST pidl)
{
	HRESULT			hr;
	hr = SHOpenFolderAndSelectItems(pidl, 0, NULL, 0);
	if(FAILED(hr))
		return FALSE;

	return TRUE;
}


BOOL	NShell::BrowseOneFile(CHAR *pFilePath)
{
	LPCITEMIDLIST	pidl;

	pidl = GetIDListByPath(pFilePath);

	OpenFolderAndSelect(pidl);

	return TRUE;
}


BOOL	NShell::AddTrayIcon(HWND hWnd, HICON hIcon, UINT uMsg, CHAR *pNtyTip)
{
	NOTIFYICONDATAA		NtfDat;
	BOOL				bRetVal;

	memset(&NtfDat, 0, sizeof(NtfDat));
	NtfDat.cbSize = sizeof(NtfDat);
	NtfDat.hWnd = hWnd;
	NtfDat.hIcon = hIcon;
	NtfDat.uID = 0;

	NtfDat.uFlags = NIF_ICON;
	if(uMsg != WM_NULL)
	{
		NtfDat.uCallbackMessage = uMsg;
		NtfDat.uFlags |= NIF_MESSAGE;
	}

	if(pNtyTip != NULL)
	{
		strcpy(NtfDat.szTip, pNtyTip);
		NtfDat.uFlags |= NIF_TIP;
	}

	bRetVal = Shell_NotifyIconA(NIM_ADD, &NtfDat);

	return bRetVal;
}


BOOL	NShell::DeleteTrayIcon(HWND hWnd)
{
	NOTIFYICONDATAA		NtfDat;
	BOOL				bRetVal;

	memset(&NtfDat, 0, sizeof(NtfDat));
	NtfDat.cbSize = sizeof(NtfDat);
	NtfDat.hWnd = hWnd;
	NtfDat.uID = 0;
	NtfDat.uFlags = NIF_ICON;

	bRetVal = Shell_NotifyIconA(NIM_DELETE, &NtfDat);

	return bRetVal;
}


BOOL	NShell::AddTrayIconInfo(HWND hWnd, CHAR* pText, CHAR *pTitle, UINT uTimeout)
{
	NOTIFYICONDATAA		NtfDat;
	BOOL				bRetVal;

	memset(&NtfDat, 0, sizeof(NtfDat));

	NtfDat.cbSize = sizeof(NtfDat);
	NtfDat.hWnd = hWnd;
	NtfDat.uID = 0;
	NtfDat.uFlags = NIF_INFO;
	NtfDat.uTimeout = uTimeout;
	NtfDat.dwInfoFlags = NIIF_INFO;
	strcpy(NtfDat.szInfo, pText);
	strcpy(NtfDat.szInfoTitle, pTitle);

	bRetVal = Shell_NotifyIconA(NIM_MODIFY, &NtfDat);

	return bRetVal;
}


IContextMenu *	NShell::GetOneFileShellMenu(WCHAR *pFilePath)
{
	IShellFolder	*pDesktopFolder;
	IShellFolder	*pCurFolder;
	HRESULT			hr;
	ITEMIDLIST		*pDirIdl;
	ITEMIDLIST		*pFileIdl;
	IContextMenu	*pCtxMenu;
	WCHAR			szFileDir[MAXPATH];
	WCHAR			szFileName[MAXPATH];

	pDesktopFolder = NULL;
	pCurFolder = NULL;
	pDirIdl = NULL;
	pFileIdl = NULL;
	pCtxMenu = NULL;

	NFile::SpliteFilePath(pFilePath, szFileDir, szFileName);
	wcscat(szFileDir, L"\\");

	hr = SHGetDesktopFolder(&pDesktopFolder);
	if(FAILED(hr))
		goto GetOneFileShellMenu_Done;

 	hr = pDesktopFolder->ParseDisplayName(NULL, 0, szFileDir, NULL, &pDirIdl, NULL);
 	if(FAILED(hr))
 		goto GetOneFileShellMenu_Done;
 
 	hr = pDesktopFolder->BindToObject(pDirIdl, NULL, IID_IShellFolder, (VOID **)&pCurFolder);
 	if(FAILED(hr))
		goto GetOneFileShellMenu_Done;

	hr = pCurFolder->ParseDisplayName(NULL, 0, szFileName, NULL, &pFileIdl, NULL);
	if(FAILED(hr))
		goto GetOneFileShellMenu_Done;

// 	hr = pCurFolder->ParseDisplayName(NULL, 0, pFilePath, NULL, &pFileIdl, NULL);
// 	if(FAILED(hr))
// 		goto GetOneFileShellMenu_Done;

	hr = pCurFolder->GetUIObjectOf(NULL, 1, (LPCITEMIDLIST *)&pFileIdl, IID_IContextMenu, NULL, (VOID **)&pCtxMenu);
	if(FAILED(hr))
		goto GetOneFileShellMenu_Done;

	ATLTRACE("%S\n", pFilePath);

GetOneFileShellMenu_Done:

	if(pDesktopFolder)
		pDesktopFolder->Release();
	if(pCurFolder)
		pDesktopFolder->Release();

	return pCtxMenu;
}

BOOL			NShell::FileShellMenuClose(IContextMenu *pContextMenu)
{
	if(pContextMenu == NULL)
		return FALSE;

	return TRUE;
}

//					NShell
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
//					NAcl


BOOL	NAcl::DeleteAceByMode(PACL pAcl, ACCESS_MODE dwMode)
{
	DWORD				dwCount;
	PEXPLICIT_ACCESS_A	pExpAcsList;
	DWORD				dwRetVal;
	DWORD				i;
	DWORD				dwDelCount;

	if(pAcl == NULL)
		return FALSE;

	dwRetVal = GetExplicitEntriesFromAclA(pAcl, &dwCount, &pExpAcsList);
	if(dwRetVal != 0)
		return FALSE;

	dwDelCount = 0;
	for(i=0; i<dwCount; i++)
	{
		if(pExpAcsList->grfAccessMode == dwMode)
		{
			DeleteAce(pAcl, i-dwDelCount);
			dwDelCount ++;
		}

		pExpAcsList ++;
	}

	return TRUE;
}


BOOL	NAcl::DeleteAceByUser(PACL pAcl, BYTE bAceType, CHAR *pUserName)
{
	DWORD					i;
	LPVOID					pAce;
	BOOL					bRetVal;
	ACL_SIZE_INFORMATION	AclSizeInfo; 
	ACE_HEADER *			pAceHeader;
	PSID					pSid;

	pSid = NSys::GetUserSID(pUserName);
	if(pSid == NULL)
	{
		return FALSE;
	}

	GetAclInformation(pAcl, &AclSizeInfo, sizeof(AclSizeInfo), AclSizeInformation); 

	for(i=0; i<AclSizeInfo.AceCount; i++) 
	{
		bRetVal = GetAce(pAcl, i, &pAce);
		if(bRetVal == FALSE)
			return FALSE;

		pAceHeader = (ACE_HEADER *)pAce;

		ACCESS_ALLOWED_ACE *		pAllowedAce; 
		ACCESS_DENIED_ACE *			pDeniedAce; 
		SYSTEM_AUDIT_ACE *			pSysAuditAce;

		if(bAceType != pAceHeader->AceType && bAceType != 0xFF)
			continue;

		switch(pAceHeader->AceType)
		{
		case ACCESS_ALLOWED_ACE_TYPE:
			pAllowedAce = (ACCESS_ALLOWED_ACE *)pAce;
			bRetVal = EqualSid(pSid, (PSID) &pAllowedAce->SidStart);
			if(bRetVal)
                DeleteAce(pAcl, i);
			break;
		case ACCESS_DENIED_ACE_TYPE:
			pDeniedAce = (ACCESS_DENIED_ACE *)pAce;
			bRetVal = EqualSid(pSid, (PSID) &pDeniedAce->SidStart);
			if(bRetVal)
				DeleteAce(pAcl, i);
			break;
		case SYSTEM_AUDIT_ACE_TYPE:
			pSysAuditAce = (SYSTEM_AUDIT_ACE *)pAce;
			bRetVal = EqualSid(pSid, (PSID) &pSysAuditAce->SidStart);
			if(bRetVal)
				DeleteAce(pAcl, i);
			break;

		default:
			assert(0);
		}

	}

	free(pSid);
	return TRUE;
}


BOOL	NAcl::DeleteObjSecurityInfo(CHAR *pObjName, SE_OBJECT_TYPE nType, CHAR *pUserName)
{
	DWORD		dwRetVal;
	PACL		pAcl;
	CHAR		szUserName[128];
	DWORD		dwSize;

	PSECURITY_DESCRIPTOR	pSecDesp;

	if(pObjName == NULL)
		return FALSE;

	if(pUserName == NULL)
	{
		dwSize = sizeof(szUserName);
		GetUserNameA(szUserName, &dwSize);
		pUserName = szUserName;
	}

	dwRetVal = GetNamedSecurityInfoA(pObjName, nType, DACL_SECURITY_INFORMATION, NULL, 
		NULL, &pAcl, NULL, &pSecDesp);
	if(dwRetVal != 0)
		return FALSE;

	DeleteAceByUser(pAcl, -1, pUserName);

	dwRetVal = SetNamedSecurityInfoA(pObjName, nType, DACL_SECURITY_INFORMATION, 
		NULL, NULL, pAcl, NULL);

	if(pSecDesp)
		LocalFree(pSecDesp);

	if(dwRetVal != 0)
		return FALSE;

	return TRUE;
}

BOOL	NAcl::SetObjSecurityInfo(CHAR *pObjName, SE_OBJECT_TYPE nType,
								 CHAR *pUserName, ACCESS_MODE dwMode, DWORD dwAcsPermis)
{
	DWORD		dwRetVal;
	PACL		pAcl;
	PACL		pNewAcl;
	CHAR		szUserName[128];
	DWORD		dwSize;

	PSECURITY_DESCRIPTOR	pSecDesp;
	EXPLICIT_ACCESSA		ExpAcs;

	if(pObjName == NULL)
		return FALSE;

	if(pUserName == NULL)
	{
		dwSize = sizeof(szUserName);
		GetUserNameA(szUserName, &dwSize);
		pUserName = szUserName;
	}

	dwRetVal = GetNamedSecurityInfoA(pObjName, nType, DACL_SECURITY_INFORMATION, NULL, 
		NULL, &pAcl, NULL, &pSecDesp);
	if(dwRetVal != 0)
		return FALSE;

	if(dwMode != GRANT_ACCESS)
	{
		DeleteAceByMode(pAcl, DENY_ACCESS);
	}

	BuildExplicitAccessWithNameA(&ExpAcs, pUserName, dwAcsPermis, dwMode, 
		OBJECT_INHERIT_ACE | CONTAINER_INHERIT_ACE);

	dwRetVal = SetEntriesInAclA(1, &ExpAcs, pAcl, &pNewAcl);
	if(dwRetVal != 0)
	{
		if(pSecDesp)
			LocalFree(pSecDesp);
		return FALSE;
	}

	dwRetVal = SetNamedSecurityInfoA(pObjName, nType, DACL_SECURITY_INFORMATION, 
		NULL, NULL, pNewAcl, NULL);

	if(pNewAcl)
		LocalFree(pNewAcl);
	if(pSecDesp)
		LocalFree(pSecDesp);

	if(dwRetVal != 0)
		return FALSE;

	return TRUE;
}


BOOL	NAcl::SetFileOwner(CHAR *pFilePath, CHAR *pUserName)
{
	BOOL		bRetVal;

	bRetVal = SetObjSecurityInfo(pFilePath, SE_FILE_OBJECT, pUserName, GRANT_ACCESS,  GENERIC_ALL);

	return TRUE;
}

BOOL	NAcl::SetFileFullAccess(CHAR *pFilePath, CHAR *pUserName)
{
	BOOL		bRetVal;

	bRetVal = SetObjSecurityInfo(pFilePath, SE_FILE_OBJECT, pUserName, SET_ACCESS,  GENERIC_ALL);

	return bRetVal;
}


BOOL	NAcl::DeleteFileAccess(CHAR *pFilePath, CHAR *pUserName)
{
	BOOL		bRetVal;

	bRetVal = DeleteObjSecurityInfo(pFilePath, SE_FILE_OBJECT, pUserName);

	return bRetVal;
}

BOOL	NAcl::SetRegFullAccess(CHAR *pRegPath, CHAR *pUserName)
{
	BOOL		bRetVal;

	bRetVal = SetObjSecurityInfo(pRegPath, SE_REGISTRY_KEY, pUserName, GRANT_ACCESS,  KEY_ALL_ACCESS);

	return bRetVal;
}


BOOL	NAcl::SetRegFullAccess(HKEY hRoot, CHAR *pRegPath, CHAR *pUserName)
{
	BOOL		bRetVal;
	CHAR		szObjName[MAX_PATH];
	CHAR		*pRootName;

	if(hRoot == HKEY_CLASSES_ROOT)
		pRootName = "CLASSES_ROOT";
	else if(hRoot == HKEY_CURRENT_USER)
		pRootName = "CURRENT_USER";
	else if(hRoot == HKEY_LOCAL_MACHINE)
		pRootName = "MACHINE";
	else if(hRoot == HKEY_USERS)
		pRootName = "USERS";
	else
		return FALSE;

	sprintf(szObjName, "%s\\%s", pRootName, pRegPath);


	bRetVal = SetObjSecurityInfo(szObjName, SE_REGISTRY_KEY, pUserName, GRANT_ACCESS,  KEY_ALL_ACCESS);

	return bRetVal;
}

//	_T("S:(ML;;NW;;;LW)")  Õâ¸öÃèÊö·û "µÍÈ¨ÏÞ"
BOOL	NAcl::SetSecurityDescriptorByString(SECURITY_DESCRIPTOR *pSd, TCHAR *pSdString)
{
	PSECURITY_DESCRIPTOR 		pNewSd;
	PACL		pAcl = NULL;
	PACL		pDcl = NULL;
	BOOL		bSaclPresent = FALSE;
	BOOL		bSaclDefaulted = FALSE;
	BOOL		bRetVal;

	bRetVal = ConvertStringSecurityDescriptorToSecurityDescriptor(pSdString, SDDL_REVISION_1, &pNewSd, NULL);
	if(bRetVal == FALSE)
	{
		assert(0);
		return FALSE;
	}

	GetSecurityDescriptorSacl(pNewSd, &bSaclPresent, &pAcl, &bSaclDefaulted);
	GetSecurityDescriptorDacl(pNewSd, &bSaclPresent, &pDcl, &bSaclDefaulted);

	if(pAcl)
        SetSecurityDescriptorSacl(pSd, TRUE, pAcl, FALSE);
	if(pDcl)
        SetSecurityDescriptorDacl(pSd, TRUE, pDcl, FALSE);

	return TRUE;
}


BOOL	NAcl::CreateEveryoneAcl(SECURITY_DESCRIPTOR *pSd)
{
	SID_IDENTIFIER_AUTHORITY SidWorld = SECURITY_WORLD_SID_AUTHORITY;
	PSID		pSidEveryone = NULL;
	BYTE		bzAcl[1024];
	PACL		pAcl;
	BOOL		bRetVal;
	DWORD		dwAclLength;

	bRetVal = AllocateAndInitializeSid(&SidWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pSidEveryone);
	pAcl = (PACL)bzAcl;
	dwAclLength = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD) + GetLengthSid(pSidEveryone);

	bRetVal = InitializeAcl(pAcl, dwAclLength, ACL_REVISION);
	bRetVal = AddAccessAllowedAce(pAcl, ACL_REVISION, GENERIC_ALL, pSidEveryone);
	bRetVal = SetSecurityDescriptorDacl(pSd, TRUE, pAcl, FALSE);

	if(pSidEveryone)
        FreeSid(pSidEveryone);

	return TRUE;
}



//					NAcl
/////////////////////////////////////////////////////////////////////////////////////////////




BOOL	NWin7::RunAsAdmin(CHAR * pFile, CHAR *pParameters, HWND hWnd)
{
	SHELLEXECUTEINFOA   ExInfo;
	BOOL				bRetVal;

	ZeroMemory(&ExInfo, sizeof(ExInfo));

	ExInfo.cbSize          = sizeof(SHELLEXECUTEINFOW);
	ExInfo.hwnd            = hWnd;
	ExInfo.fMask           = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_FLAG_NO_UI;
	ExInfo.lpVerb          = "runas";//_TEXT("runas");
	ExInfo.lpFile          = pFile;
	ExInfo.lpParameters    = pParameters;
	ExInfo.nShow           = SW_SHOWNORMAL;

	bRetVal = ShellExecuteExA(&ExInfo);

	return bRetVal;
}


BOOL	NWin7::RequestAdmin()
{
	return FALSE;

}


/////////////////////////////////////////////////////////////////////////////////////////////
//					NMac
/////////////////////////////////////////////////////////////////////////////////////////////

#define MAKE_MACS_DATA(_X)	{_X, #_X}


BOOL	NMac::GetMacString_WndStyle(int nWndStyle, _bstr_t *pbStrMac, CHAR *pSptStr)
{

#ifndef DS_USEPIXELS
#define DS_USEPIXELS 0x8000L
#endif

	int						i;
	int						nCount;
	CHAR					szBuf[1024];
	static	MAC_STRING		table[] = 
	{
		{WS_OVERLAPPEDWINDOW, "WS_OVERLAPPEDWINDOW(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)"}, 
		{WS_POPUPWINDOW, "WS_POPUPWINDOW(WS_POPUP | WS_BORDER | WS_SYSMENU)"},
		MAKE_MACS_DATA(WS_POPUPWINDOW), 
		MAKE_MACS_DATA(WS_POPUP), 
		MAKE_MACS_DATA(WS_CHILDWINDOW), 
		MAKE_MACS_DATA(WS_ICONIC), 
		MAKE_MACS_DATA(WS_VISIBLE), 
		MAKE_MACS_DATA(WS_DISABLED), 
		MAKE_MACS_DATA(WS_CLIPSIBLINGS), 
		MAKE_MACS_DATA(WS_CLIPCHILDREN), 
		MAKE_MACS_DATA(WS_MAXIMIZE), 
		MAKE_MACS_DATA(WS_CAPTION), 
		MAKE_MACS_DATA(WS_BORDER), 
		MAKE_MACS_DATA(WS_DLGFRAME), 
		MAKE_MACS_DATA(WS_VSCROLL), 
		MAKE_MACS_DATA(WS_HSCROLL), 
		MAKE_MACS_DATA(WS_SYSMENU), 
		MAKE_MACS_DATA(WS_SIZEBOX), 
		MAKE_MACS_DATA(WS_MINIMIZEBOX), 
		MAKE_MACS_DATA(WS_MAXIMIZEBOX), 

		{DS_SHELLFONT, "DS_SHELLFONT(DS_SETFONT | DS_FIXEDSYS)"}, 
		MAKE_MACS_DATA(DS_ABSALIGN), 
		MAKE_MACS_DATA(DS_SYSMODAL), 
		MAKE_MACS_DATA(DS_LOCALEDIT), 
		MAKE_MACS_DATA(DS_SETFONT), 
		MAKE_MACS_DATA(DS_MODALFRAME), 
		MAKE_MACS_DATA(DS_NOIDLEMSG), 
		MAKE_MACS_DATA(DS_SETFOREGROUND), 
		MAKE_MACS_DATA(DS_3DLOOK), 
		MAKE_MACS_DATA(DS_FIXEDSYS), 
		MAKE_MACS_DATA(DS_NOFAILCREATE), 
		MAKE_MACS_DATA(DS_CONTROL), 
		MAKE_MACS_DATA(DS_CENTER), 
		MAKE_MACS_DATA(DS_CENTERMOUSE), 
		MAKE_MACS_DATA(DS_CONTEXTHELP), 
		MAKE_MACS_DATA(DS_USEPIXELS)
	};
	

	nCount = sizeof(table) / sizeof(MAC_STRING);
	*pbStrMac = "";

	if(nWndStyle == 0)
		return TRUE;

	for(i=0; i<nCount; i++)
	{
		if( (nWndStyle & table[i].dwValue) == table[i].dwValue )
		{
			nWndStyle ^= table[i].dwValue;
			*pbStrMac += table[i].szMacString;
			*pbStrMac += pSptStr;
			if(nWndStyle == 0)
				return TRUE;
		}
	}

	sprintf(szBuf, "%08X", nWndStyle);
	*pbStrMac += szBuf;

	return FALSE;
}

BOOL	NMac::GetMacString_WndExStyle(int nWndExStyle, _bstr_t *pbStrMac, CHAR *pSptStr)
{
	int						i;
	int						nCount;
	CHAR					szBuf[1024];
	static	MAC_STRING		table[] = 
	{
		{WS_EX_OVERLAPPEDWINDOW, "WS_EX_OVERLAPPEDWINDOW(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE)"}, 
		{WS_EX_PALETTEWINDOW, "WS_EX_PALETTEWINDOW(WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST)"},
		MAKE_MACS_DATA(WS_EX_DLGMODALFRAME), 
		MAKE_MACS_DATA(WS_EX_NOPARENTNOTIFY), 
		MAKE_MACS_DATA(WS_EX_TOPMOST), 
		MAKE_MACS_DATA(WS_EX_ACCEPTFILES), 
		MAKE_MACS_DATA(WS_EX_TRANSPARENT), 
		MAKE_MACS_DATA(WS_EX_MDICHILD), 
		MAKE_MACS_DATA(WS_EX_TOOLWINDOW), 
		MAKE_MACS_DATA(WS_EX_WINDOWEDGE), 
		MAKE_MACS_DATA(WS_EX_CLIENTEDGE), 
		MAKE_MACS_DATA(WS_EX_CONTEXTHELP), 
		MAKE_MACS_DATA(WS_EX_RIGHT), 
		MAKE_MACS_DATA(WS_EX_RTLREADING), 
		MAKE_MACS_DATA(WS_EX_LEFTSCROLLBAR), 
		MAKE_MACS_DATA(WS_EX_CONTROLPARENT), 
		MAKE_MACS_DATA(WS_EX_STATICEDGE), 
		MAKE_MACS_DATA(WS_EX_APPWINDOW), 
		MAKE_MACS_DATA(WS_EX_LAYERED), 
		MAKE_MACS_DATA(WS_EX_NOINHERITLAYOUT), 
		MAKE_MACS_DATA(WS_EX_LAYOUTRTL), 
		MAKE_MACS_DATA(WS_EX_COMPOSITED), 
		MAKE_MACS_DATA(WS_EX_NOACTIVATE),
	};

	nCount = sizeof(table) / sizeof(MAC_STRING);
	*pbStrMac = "";

	if(nWndExStyle == 0)
		return TRUE;

	for(i=0; i<nCount; i++)
	{
		if( (nWndExStyle & table[i].dwValue) == table[i].dwValue )
		{
			nWndExStyle ^= table[i].dwValue;
			*pbStrMac += table[i].szMacString;
			*pbStrMac += pSptStr;
			if(nWndExStyle == 0)
				return TRUE;
		}
	}

	sprintf(szBuf, "%08X", nWndExStyle);
	*pbStrMac += szBuf;

	return FALSE;
}

BOOL	NMac::GetMacString_CtlStyle(int nCtlStyle, _bstr_t *pbStrMac, CHAR *pSptStr)
{
	int						i;
	int						nCount;
	CHAR					szBuf[1024];
	static	MAC_STRING		table[] = 
	{
		{WS_OVERLAPPEDWINDOW, "WS_OVERLAPPEDWINDOW(WS_CAPTION WS_SYSMENU WS_THICKFRAME WS_MINIMIZEBOX WS_MAXIMIZEBOX)"}, 
		{WS_POPUPWINDOW, "WS_POPUPWINDOW(WS_POPUP WS_BORDER WS_SYSMENU)"},
		MAKE_MACS_DATA(WS_POPUP), 
		MAKE_MACS_DATA(WS_CHILD), 
		MAKE_MACS_DATA(WS_ICONIC), 
		MAKE_MACS_DATA(WS_VISIBLE), 
		MAKE_MACS_DATA(WS_DISABLED), 
		MAKE_MACS_DATA(WS_CLIPSIBLINGS), 
		MAKE_MACS_DATA(WS_CLIPCHILDREN), 
		MAKE_MACS_DATA(WS_MAXIMIZE), 
		MAKE_MACS_DATA(WS_CAPTION), 
		MAKE_MACS_DATA(WS_BORDER), 
		MAKE_MACS_DATA(WS_DLGFRAME), 
		MAKE_MACS_DATA(WS_VSCROLL), 
		MAKE_MACS_DATA(WS_HSCROLL), 
		MAKE_MACS_DATA(WS_SYSMENU), 
		MAKE_MACS_DATA(WS_THICKFRAME), 
		MAKE_MACS_DATA(WS_GROUP), 
		MAKE_MACS_DATA(WS_TABSTOP)
	};


	nCount = sizeof(table) / sizeof(MAC_STRING);
	*pbStrMac = "";

	if(nCtlStyle == 0)
		return TRUE;

	for(i=0; i<nCount; i++)
	{
		if( (nCtlStyle & table[i].dwValue) == table[i].dwValue )
		{
			nCtlStyle ^= table[i].dwValue;
			*pbStrMac += table[i].szMacString;
			*pbStrMac += pSptStr;
			if(nCtlStyle == 0)
				return TRUE;
		}
	}

	sprintf(szBuf, "%08X", nCtlStyle);
	*pbStrMac += szBuf;

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//		NWnd

HWND	NWnd::GetWindowFromPos(int x, int y)
{
	HWND	hRetWnd;
	HWND	hWndTop = NULL;
	HWND	hParentWnd;
	HWND	hWndChild = NULL;
	LONG	lWindowStyle = 0;
	POINT	pt;

	if(x == -1 && y == -1)
	{
		GetCursorPos(&pt);

		x = pt.x;
		y = pt.y;
	}
	else
	{
		pt.x = x;
		pt.y = y;
	}

	hWndTop = ::WindowFromPoint(pt);
	if(hWndTop == NULL)
		return NULL;

	hParentWnd = GetParent(hWndTop);
	lWindowStyle = GetWindowLong(hWndTop, GWL_STYLE);

	if(	hParentWnd == NULL || 
		hParentWnd == GetDesktopWindow() ||
		(lWindowStyle & WS_CHILDWINDOW) == 0)
	{
		hRetWnd = hWndTop;
	}
	else
	{
		hRetWnd = hParentWnd;
	}


	while (TRUE)
	{
		pt.x = x;
		pt.y = y;

		::ScreenToClient(hRetWnd, &pt);
		hWndChild = RealChildWindowFromPoint(hRetWnd, pt);

		if (hWndChild && (hWndChild != hRetWnd))
			hRetWnd = hWndChild;
		else
			break;
	}

	return hRetWnd;
}


BOOL	NWnd::SetDlgItemTextA(HWND hDlg, int nIDDlgItem, IN LPCSTR lpString)
{
	BOOL	bRetVal;
	CHAR	szText[1024];

	szText[0] = 0;
	::GetDlgItemTextA(hDlg, nIDDlgItem, szText, sizeof(szText));
	if(strcmp(szText, lpString) == 0)
		return TRUE;

	bRetVal = ::SetDlgItemTextA(hDlg, nIDDlgItem, lpString);

	return bRetVal;
}
//		NWnd
//////////////////////////////////////////////////////////////////////////////////////////////////




#pragma comment(lib, "rasapi32.lib")

BOOL	NSafe::GetUserPassword(CHAR *pPwd)
{
	int		nPlatformId;
	CHAR	szUserName[100];
	DWORD	dwSize;
	BOOL	bRetVal;
	WCHAR *	pKeyName;

	CMiniMem				MiniMem;
	LSA_OBJECT_ATTRIBUTES	LsaObj;
	LSA_HANDLE				hLsa;
	LSA_UNICODE_STRING 		LsaKey;
	LSA_UNICODE_STRING *	pLsaData;
	NTSTATUS				lRetVal;
	BOOL					bResult;
	WCHAR					szPwd[100];


	bResult = FALSE;
	hLsa = NULL;
	strcpy(pPwd, "");

	nPlatformId = NSys::GetPlatformId();
	if(nPlatformId != VER_PLATFORM_WIN32_NT)
		return FALSE;

	dwSize = sizeof(szUserName);
	bRetVal = GetUserNameA(szUserName, &dwSize);
	if(bRetVal == FALSE)
		return FALSE;

	memset(&LsaObj, 0, sizeof(LsaObj));
	LsaObj.Length = sizeof(LsaObj);

	LsaOpenPolicy(NULL, &LsaObj, POLICY_LOOKUP_NAMES, &hLsa);

	dwSize = 1024;
	pKeyName = L"DefaultPassword";
	LsaKey.Buffer = (WCHAR *)MiniMem.Alloc(dwSize);
	LsaKey.MaximumLength = (USHORT)dwSize;
	LsaKey.Length = (USHORT)((wcslen(pKeyName)) * sizeof(WCHAR));
	wcscpy(LsaKey.Buffer, pKeyName);

	dwSize = 4096;
	pLsaData =  (LSA_UNICODE_STRING *)MiniMem.Alloc(sizeof(LSA_UNICODE_STRING));
	pLsaData->Buffer = (WCHAR *)MiniMem.Alloc(dwSize);
	pLsaData->Length = 0;
	pLsaData->MaximumLength = (USHORT)dwSize;

	lRetVal = LsaRetrievePrivateData(hLsa, &LsaKey, &pLsaData);
	if(lRetVal != 0)
	{
		lRetVal = LsaNtStatusToWinError(lRetVal);
		goto GetUserPassword_Done;
	}

	memset(szPwd, 0, sizeof(szPwd));
	memcpy(szPwd, pLsaData->Buffer, pLsaData->Length);
	NStr::WChar2Char(szPwd, pPwd);
	bResult = TRUE;

GetUserPassword_Done:
	if(lRetVal == 0)
		LsaFreeMemory(pLsaData);

	if(hLsa)
        LsaClose(hLsa);


	return bResult;
}


BOOL	DiaPwdDecode(BYTE *pData, int dwDatSize, CHAR *pUser, int nParamsUID, CHAR *pPassword)
{
	int			i, j;
	WCHAR *		pString;
	WCHAR		szParamsUID[64];
	WCHAR		szDiaUserName[128];
	WCHAR *		pPwd;

	NStr::Char2WChar(pUser, szDiaUserName, sizeof(szDiaUserName));
	swprintf(szParamsUID, L"%d", nParamsUID);

	pString = (WCHAR *)pData;

	for(i=0; i<dwDatSize; i++) 
	{ 
		if(wcscmp(pString+i, szParamsUID) == 0 ) 
		{ 
			for(j=i; j <dwDatSize; j++) 
			{ 
				if(wcscmp(pString+j, szDiaUserName) == 0 ) 
				{ 
					pPwd = pString + j + wcslen(szDiaUserName) + 1; 
					NStr::WChar2Char(pPwd, pPassword);
					return TRUE; 
				} 
			} 
			break; 
		} 
	}

	pPassword[0] = 0;
	return FALSE;
}


BOOL	NSafe::GetDialupPassword(std::vector<Dialup_Account> &DiaList)
{
	int			nPlatformId;
	CHAR		szPbkFilePath[MAXPATH];
	CHAR		szPbkFileDir[MAXPATH];
	DWORD		dwSize;
	DWORD		dwEntries;
	DWORD		dwRetVal;
	int			i;
	BOOL		bRetVal;

	RASENTRYNAMEA *		pRasEntryName;
	CMiniMem			MiniMem;

	std::vector<std::string>	PbkFileList;
	std::string					sOnePath;

	DiaList.clear();

	nPlatformId = NSys::GetPlatformId();
	if(nPlatformId == VER_PLATFORM_WIN32_NT)
	{
		szPbkFilePath[0] = 0;
		GetEnvironmentVariableA("ALLUSERSPROFILE", szPbkFileDir, sizeof(szPbkFileDir));
		if(szPbkFileDir[0] != 0)
		{
			sprintf(szPbkFilePath, "%s\\%s", szPbkFileDir, "Application Data\\Microsoft\\Network\\Connections\\Pbk\\rasphone.pbk");
			sOnePath = szPbkFilePath;
			PbkFileList.push_back(sOnePath);

			sprintf(szPbkFilePath, "%s\\%s", szPbkFileDir, "Application Data\\Microsoft\\Network\\Connections\\rasphone.pbk");
			sOnePath = szPbkFilePath;
			PbkFileList.push_back(sOnePath);
		}
		else
		{
			SHGetFolderPathA(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPbkFileDir);

			sprintf(szPbkFilePath, "%s\\%s", szPbkFileDir, "Microsoft\\Network\\Connections\\rasphone.pbk");
			sOnePath = szPbkFilePath;
			PbkFileList.push_back(sOnePath);

			sprintf(szPbkFilePath, "%s\\%s", szPbkFileDir, "Microsoft\\Network\\Connections\\rasphone.pbk");
			sOnePath = szPbkFilePath;
			PbkFileList.push_back(sOnePath);
		}

		szPbkFilePath[0] = 0;
		GetEnvironmentVariableA("USERPROFILE", szPbkFileDir, sizeof(szPbkFileDir));
		if(szPbkFileDir[0] != 0)
		{
			sprintf(szPbkFilePath, "%s\\%s", szPbkFileDir, "Application Data\\Microsoft\\Network\\Connections\\Pbk\\rasphone.pbk");
			sOnePath = szPbkFilePath;
			PbkFileList.push_back(sOnePath);

			sprintf(szPbkFilePath, "%s\\%s", szPbkFileDir, "Application Data\\Microsoft\\Network\\Connections\\rasphone.pbk");
			sOnePath = szPbkFilePath;
			PbkFileList.push_back(sOnePath);
		}
		else
		{
			SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, szPbkFileDir);

			sprintf(szPbkFilePath, "%s\\%s", szPbkFileDir, "Microsoft\\Network\\Connections\\rasphone.pbk");
			sOnePath = szPbkFilePath;
			PbkFileList.push_back(sOnePath);

			sprintf(szPbkFilePath, "%s\\%s", szPbkFileDir, "Microsoft\\Network\\Connections\\rasphone.pbk");
			sOnePath = szPbkFilePath;
			PbkFileList.push_back(sOnePath);
		}
	}

	dwSize = sizeof(RASENTRYNAMEA);
	pRasEntryName = (RASENTRYNAMEA *)MiniMem.Alloc(dwSize);
	pRasEntryName->dwSize = sizeof(RASENTRYNAMEA);
	dwEntries = 0;
	dwRetVal = RasEnumEntriesA(NULL, NULL, pRasEntryName, &dwSize, &dwEntries);

#ifndef ERROR_BUFFER_TOO_SMALL
#define ERROR_BUFFER_TOO_SMALL	603
#endif

	if(dwRetVal == ERROR_BUFFER_TOO_SMALL || dwRetVal == ERROR_NOT_ENOUGH_MEMORY)
	{
		pRasEntryName = (RASENTRYNAMEA *)MiniMem.Alloc(dwSize);
		pRasEntryName->dwSize = sizeof(RASENTRYNAMEA);
		dwRetVal = RasEnumEntriesA(NULL, NULL, pRasEntryName, &dwSize, &dwEntries);
		if(dwRetVal != 0)
			return FALSE;
	}

	RASDIALPARAMSA *		pRasParams;
	BOOL					bPassword;
	RASENTRYA				RasEntry;
	DWORD					dwPropSize;
	int						nParamsUID;
	CHAR					szBuffer[MAXPATH];
	WCHAR					szPrivateData[MAXPATH];
	WCHAR					szKeyPath[MAXPATH];
	WCHAR					szUserSID[MAXPATH];
	CHAR					szPassword[MAXPATH];

	for(i=0; i<(int)dwEntries; i++)
	{
		dwSize = sizeof(RASDIALPARAMSA);
		pRasParams = (RASDIALPARAMSA *)MiniMem.Alloc(dwSize);
		strcpy(pRasParams->szEntryName, pRasEntryName[i].szEntryName);
		pRasParams->dwSize = sizeof(RASDIALPARAMSA); 
		RasGetEntryDialParamsA(NULL, pRasParams, &bPassword);

		dwPropSize = sizeof(RasEntry);
		memset(&RasEntry, 0, sizeof(RasEntry));
		RasEntry.dwSize = dwPropSize;
		RasGetEntryPropertiesA(NULL, pRasEntryName[i].szEntryName, &RasEntry, &dwPropSize, NULL, NULL);

		if(nPlatformId == VER_PLATFORM_WIN32_NT)
		{
			std::vector<std::string>::iterator		Iter;
			for(Iter = PbkFileList.begin();
				Iter != PbkFileList.end();
				Iter ++)
			{
				nParamsUID = GetPrivateProfileIntA(pRasEntryName[i].szEntryName, "DialParamsUID", 0, Iter->c_str());
				if(nParamsUID != 0)
					break;
			}
		}

		bRetVal = NSys::GetUserSIDString(szBuffer);
		NStr::Char2WChar(szBuffer, szUserSID, sizeof(szUserSID));
		swprintf(szKeyPath, L"RasDialParams!%s#0", szUserSID);

		dwRetVal = NSys::GetLsaPrivateData(szKeyPath, szPrivateData, sizeof(szPrivateData));
		if(dwRetVal == 0)
		{
			dwRetVal = NSys::GetLsaPrivateData(L"L$_RasDefaultCredentials#0", szPrivateData, sizeof(szPrivateData));
		}

		DiaPwdDecode((BYTE *)szPrivateData, dwRetVal, pRasParams->szUserName, nParamsUID, szPassword);


		Dialup_Account		Item;

		Item.sDiaName = pRasParams->szEntryName;
		Item.sPhoneNub = RasEntry.szLocalPhoneNumber;
		Item.sUserName = pRasParams->szUserName;
		Item.sPassword = szPassword;

		DiaList.push_back(Item);
	}

	return TRUE;
}


BOOL WINAPI OutlookAccountsENUM_PROC(VOID *pParam, HKEY hRootKey, CHAR *pSubKey, CHAR *pValueName, DWORD dwType, BYTE *pValueData, DWORD cbData)
{
	std::vector<NSafe::Outlook_Account> *		pAccList;
	NSafe::Outlook_Account		Item;
	BYTE				bzData[1024];
	DWORD				dwSize;
	BOOL				bRetVal;

	if(dwType == REG_NONE || pValueName == NULL)
		return TRUE;

	pAccList = (std::vector<NSafe::Outlook_Account> *)pParam;

	if(stricmp(pValueName, "HTTPMail User Name") == 0)
	{
		Item.sUserName = (CHAR *)pValueData;

		dwSize = sizeof(bzData);
		memset(bzData, 0, sizeof(bzData));
		bRetVal = NReg::GetReg(hRootKey, pSubKey, "HTTPMail Password2", bzData, dwSize);
		if(bRetVal == FALSE)
			return TRUE;

		Item.sPassword = (CHAR *)bzData;
		Item.sServer = "Hotmail";
		pAccList->push_back(Item);

		return TRUE;
	}
	if(stricmp(pValueName, "POP3 User Name") == 0)
	{
		Item.sUserName = (CHAR *)pValueData;

		dwSize = sizeof(bzData);
		memset(bzData, 0, sizeof(bzData));
		bRetVal = NReg::GetReg(hRootKey, pSubKey, "HTTPMail Password2", bzData, dwSize);
		if(bRetVal == FALSE)
			return TRUE;
		Item.sPassword = (CHAR *)bzData;

		dwSize = sizeof(bzData);
		memset(bzData, 0, sizeof(bzData));
		bRetVal = NReg::GetReg(hRootKey, pSubKey, "POP3 Server", bzData, dwSize);
		if(bRetVal == FALSE)
			return TRUE;

		Item.sServer = (CHAR *)bzData;

		pAccList->push_back(Item);
		return TRUE;
	}

	return TRUE;
}


BOOL	NSafe::GetOutlookAccounts(std::vector<Outlook_Account> &OutlList)
{
	BOOL	bRetVal;

	bRetVal = NReg::RegisterEnum(OutlookAccountsENUM_PROC, &OutlList, HKEY_CURRENT_USER, 
		"Software\\Microsoft\\Internet Account Manager\\Accounts");
	if(bRetVal == FALSE)
		return FALSE;

	return TRUE;
}


#include "PStorage.h"

VOID CALLBACK PStorageAccountsEnumProc(CHAR *pType, CHAR *pResName, CHAR *pUserName, CHAR *pPassWord, VOID *pParam)
{
	std::vector<NSafe::PStorage_Account> *pPstolList;

	pPstolList = (std::vector<NSafe::PStorage_Account> *)pParam;

	NSafe::PStorage_Account		Item;

	Item.sType = pType;
	Item.sResName = pResName;
	Item.sUserName = pUserName;
	Item.sPassword = pPassWord;

	pPstolList->push_back(Item);

	return;
}


BOOL	NSafe::GetPStorageAccounts(std::vector<PStorage_Account> &PstolList)
{
	EnumPStorage(PStorageAccountsEnumProc, &PstolList);

	return TRUE;
}



BOOL	NSafe::GetAccessFilePwd(CHAR *pMdbFile, CHAR *pPassword)
{
	HANDLE			hFile;
	BYTE			bzPassword[64];
	BYTE			bzData[160];
	DWORD			dwRead;
	BOOL			bRetVal;
	BOOL			bResult;
	CHAR *			pData;
	int				i;
	int				nType;
	BYTE			bKey2;
	BYTE			bKey3;
	BYTE			bzKey2K[] = 
	{
		0xBA, 0x6A, 0xEC, 0x37, 0x61, 0xD5, 0x9C, 0xFA, 0xFA, 0xCF, 0x28, 0xE6, 0x2F, 0x27, 0x8A, 0x60, 
		0x68, 0x05, 0x7B, 0x36, 0xC9, 0xE3, 0xDF, 0xB1, 0x4B, 0x65, 0x13, 0x43, 0xF3, 0x3E, 0xB1, 0x33, 
		0x08, 0xF0, 0x79, 0x5B, 0xAE, 0x24, 0x7C, 0x2A
	};

	BYTE			bzKey97[] = 
	{
		0x86, 0xFB, 0xEC, 0x37, 0x5D, 0x44, 0x9C, 0xFA, 0xC6, 0x5E, 0x28, 0xE6, 0x13, 0xB6
	};


	bResult = FALSE;
	hFile = NULL;
	strcpy(pPassword, "");

	hFile = CreateFileA(pMdbFile, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL, OPEN_ALWAYS, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	bRetVal = ReadFile(hFile, bzData, sizeof(bzData), &dwRead, NULL);
	if(bRetVal == FALSE || dwRead != sizeof(bzData))
		goto GetAccessFilePwd_Done;

	CloseHandle(hFile);
	hFile = INVALID_HANDLE_VALUE;

	pData = (CHAR *)(bzData + 4);
	if(NStr::StrHeadCmp(pData, "Standard Jet DB") != 0)
		goto GetAccessFilePwd_Done;

	pData = (CHAR *)(bzData + 0x9C);
	if(NStr::StrHeadCmp(pData, "4.0") == 0)
		nType = 2000;
	else
		nType = 97;


	pData = (CHAR *)(bzData + 0x42);
	if(nType == 2000)
	{
		bKey2 = pData[0x24];
		bKey2 = bKey2 ^ 0xAE;

		bKey3 = pData[0x25];
		bKey3 = bKey3 ^ 0x24;

		for(i=0; i<40; i++)
		{
			bzPassword[i] = (BYTE)pData[i] ^ bzKey2K[i];

			if(i%4 == 0)
			{
				bzPassword[i] = bzPassword[i] ^ bKey2;
				continue;
			}

			if(i%4 == 1)
			{
				bzPassword[i] = bzPassword[i] ^ bKey3;
				continue;
			}
		}
		NStr::WChar2Char((WCHAR *)bzPassword, pPassword, 20);
	}
	else if(nType == 97)
	{
		for(i=0; i<14; i++)
		{
			bzPassword[i] = (BYTE)pData[i] ^ bzKey97[i];
		}
		strcpy(pPassword, (CONST CHAR *)bzPassword);
	}
	else
	{
		goto GetAccessFilePwd_Done;
	}
	bResult = TRUE;

GetAccessFilePwd_Done:

	if(hFile != NULL && hFile != INVALID_HANDLE_VALUE)
        CloseHandle(hFile);

	return bResult;
}


int		NSafe::LsaCryptUnprotectData(VOID *pData, int nSize, VOID *pRecv, int nRecvSize)
{
	DWORD		dwProcId;
	BOOL		bRetVal;
	CProcessMem			LsaMemIn;
	CProcessMem			LsaMemOut;
	CProcessMem			LsaDatSize;
	CProcessMem			LsaMem;

	NSys::GetProcIdFromName("lsass.exe", &dwProcId);
	if(dwProcId == 0)
		return 0;

	LsaMemIn.OpenAndAlloc(dwProcId, 4096);
	LsaMemOut.OpenAndAlloc(dwProcId, 4096);
	LsaDatSize.OpenAndAlloc(dwProcId, 4096);
	LsaMem.OpenAndAlloc(dwProcId, 4096);

	VOID *		pFun;
	VOID *		pParams[10];
	int			nRetVal;
	int			hLsaMod;
	int			nFunaddr;

	LsaMem.Wirte("LSASRV.dll");
	pFun = (VOID *)LoadLibraryA;
	pParams[0] = LsaMem.GetData();
	NSys::RemotProcRun(dwProcId, pFun, pParams, 1, &hLsaMod);

	LsaMem.Wirte("LsaICryptUnprotectData");
	pFun = (VOID *)GetProcAddress;
	pParams[1] = (VOID *)hLsaMod;
	pParams[0] = LsaMem.GetData();
	NSys::RemotProcRun(dwProcId, pFun, pParams, 2, &nFunaddr);


	HANDLE			hToken;
	HANDLE			hDumpToken;

	hToken = NULL;
	hDumpToken = NULL;

	NSys::EnableProcPrivilege();
	bRetVal = OpenProcessToken(GetCurrentProcess(), TOKEN_IMPERSONATE|TOKEN_QUERY|TOKEN_DUPLICATE, &hToken);
	if(bRetVal == FALSE)
		hToken = NULL;

	DuplicateToken(hToken, SecurityImpersonation, &hDumpToken);

	LsaMemIn.Wirte(pData, nSize);
	pFun = (VOID *)nFunaddr;
	pParams[0] = (VOID *)LsaDatSize.GetData();
	pParams[1] = (VOID *)LsaMemOut.GetData();
	pParams[2] = NULL;
	pParams[3] = (VOID *)0x20000041;
	pParams[4] = NULL;
	pParams[5] = NULL;
	pParams[6] = NULL;
	pParams[7] = NULL;
	pParams[8] = (VOID *)nSize;
	pParams[9] = (VOID *)LsaMemIn.GetData();
	NSys::RemotProcRun(dwProcId, pFun, pParams, 10, &nRetVal, TRUE, hDumpToken);
//	typedef DWORD (WINAPI *LsaICryptUnprotectData) (PVOID pBuffer, DWORD dwSize, DWORD, DWORD, DWORD, DWORD, DWORD flags, DWORD, PVOID pOutput, DWORD* cbSize);


	VOID *		pDecodeAddr;
	DWORD		dwDecodeSize;
	DWORD		dwReadSize;

	LsaMemOut.Read(&pDecodeAddr, sizeof(pDecodeAddr));
	LsaDatSize.Read(&dwDecodeSize, sizeof(dwDecodeSize));

	if(nRecvSize < (int)dwDecodeSize)
	{
		pFun = (VOID *)LocalFree;
		pParams[0] = (VOID *)pDecodeAddr;
		NSys::RemotProcRun(dwProcId, pFun, pParams, 1, &nRetVal, TRUE, hDumpToken);

		if(hToken)
			CloseHandle(hToken);
		if(hDumpToken)
			CloseHandle(hDumpToken);

		return 0;
	}

	ReadProcessMemory(LsaMemOut.GetHandle(), pDecodeAddr, pRecv, dwDecodeSize, &dwReadSize);

	pFun = (VOID *)LocalFree;
	pParams[0] = (VOID *)pDecodeAddr;
	NSys::RemotProcRun(dwProcId, pFun, pParams, 1, &nRetVal, TRUE, hDumpToken);

	if(hToken)
		CloseHandle(hToken);
	if(hDumpToken)
		CloseHandle(hDumpToken);

	return dwReadSize;
}

BOOL	NSafe::ParseCredItems(VOID *pData, int nSize, std::vector<MsCred_Account> &MsCredAccList)
{
	BYTE *		pCredData;
	int			nPos;
	DWORD		dwOneSize;

	pCredData = (BYTE *)pData;
	if(nSize < 31)
		return FALSE;

	for(nPos = 8;
		nPos < nSize;
		)
	{
		dwOneSize = PV_DWORD(pCredData+nPos);
		if(dwOneSize == 0)
			return FALSE;

		dwOneSize += nPos;




	}

	return TRUE;
}


#define INPUT_ID_EDT1	0x1001
#define INPUT_ID_BTN1	IDOK
#define INPUT_ID_BTN2	IDCANCEL


//	lParam -> 0:
BOOL CALLBACK NDlg::InputProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static INPUT_DLG_DATA *	pData;
	HFONT					hFont;

	if (uMsg == WM_INITDIALOG)
	{
		pData = (INPUT_DLG_DATA *)lParam;

		if(pData->hParentWnd == NULL)
			hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		else
			hFont = (HFONT)::SendMessage(pData->hParentWnd, WM_GETFONT, 0, 0);

		::SendMessage(::GetDlgItem(hwndDlg, INPUT_ID_EDT1), WM_SETFONT, (LPARAM)hFont, MAKELPARAM(TRUE, 0));
		::SendMessage(::GetDlgItem(hwndDlg, INPUT_ID_BTN1), WM_SETFONT, (LPARAM)hFont, MAKELPARAM(TRUE, 0));
		::SendMessage(::GetDlgItem(hwndDlg, INPUT_ID_BTN2), WM_SETFONT, (LPARAM)hFont, MAKELPARAM(TRUE, 0));
		::SetDlgItemTextA(hwndDlg, INPUT_ID_EDT1, pData->pText);
		return TRUE;   
	}
	else if(uMsg == WM_COMMAND)
	{
		if(LOWORD(wParam) == IDOK)
		{
			::GetDlgItemTextA(hwndDlg, INPUT_ID_EDT1, pData->pText, pData->nTextBufSize);
			EndDialog (hwndDlg, IDOK);
			return TRUE;
		}
		if(LOWORD(wParam) == IDCANCEL)
		{
			EndDialog (hwndDlg, IDCANCEL);
			return TRUE;
		}
	}   
	else if ((uMsg == WM_SYSCOMMAND) && (wParam == SC_CLOSE)) 
	{     
		EndDialog (hwndDlg, IDCANCEL);
		return TRUE;
	}
	return FALSE;
}


int	NDlg::GetInput(HWND hWnd, CHAR * pText, int dwBufSize, CHAR *pCapt, int nWidth)
{
	WORD    		*p, *pdlgtemplate;
	RECT			AglinRct;
	int     		nchar;
	int     		nButtonW = 40, nButtonH = 12, nDis = 6;
	int				nHeigh = 41;
	DWORD   		lStyle = 0, lExStyle = 0;
	int				nRetVal;
	DWORD			dwEdtStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL;
	DWORD			dwEdtExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE;
	DWORD			dwButStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_TEXT;
	WCHAR *			pBtnOk;
	WCHAR *			pBtnCancel;
	NSys::SYS_LANGUAGE	OsLang;

	INPUT_DLG_DATA		DlgData;


	AglinRct.left = nDis;
	AglinRct.top = nDis+2;
	AglinRct.right = nDis;
	AglinRct.bottom = nDis-1;

	int				nEditX, nEditY, nEditW, nEditH;
	int				nButtonX, nButtonY;


	DlgData.hParentWnd = hWnd;
	DlgData.pText = pText;
	DlgData.nTextBufSize = dwBufSize;

	NSys::GetOsLanguage(&OsLang);
	if(OsLang == NSys::Lang_Chinese)
	{
		pBtnOk = L"È·¶¨";
		pBtnCancel = L"È¡Ïû";
	}
	else
	{
		pBtnOk = L"Ok";
		pBtnCancel = L"Cancel";
	}


	pdlgtemplate = p = (PWORD) LocalAlloc (LPTR, 1000);
	///////////////////////   Init for DLGTEMPLATEEX     /////////////////////////////////
	lStyle = DS_MODALFRAME | DS_3DLOOK | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
	*p++ = LOWORD (lStyle);             //style
	*p++ = HIWORD (lStyle);             //style
	*p++ = 0;                           // LOWORD (dwExtendedStyle)
	*p++ = 0;                           // HIWORD (dwExtendedStyle)
	*p++ = 3;                           // NumberOfItems
	*p++ = 0;                           // x
	*p++ = 0;                           // y
	*p++ = nWidth;                      // cx
	*p++ = nHeigh;                      // cy
	*p++ = 0;                           // Menu resource Name
	*p++ = 0;                           // Class Name
	nchar = NStr::Char2WChar(pCapt, (WCHAR *)p);      // title
	p += nchar;                         // title
	p = (WORD *)NData::Align(p);        // Align DWORD boundary 


	nEditX = AglinRct.left;
	nEditY = AglinRct.top;
	nEditW = nWidth - AglinRct.left - AglinRct.right;
	nEditH = nHeigh - AglinRct.top - AglinRct.bottom - nButtonH - nDis;

	///////////////////////   Init for DLGITEMTEMPLATEEX  Edit  ////////////////////////
	*p++ = LOWORD (dwEdtStyle);         //style
	*p++ = HIWORD (dwEdtStyle);		    //style
	*p++ = LOWORD (dwEdtExStyle);       // LOWORD (dwExtendedStyle)
	*p++ = HIWORD (dwEdtExStyle);       // HIWORD (dwExtendedStyle) 
	*p++ = nEditX;                      // x
	*p++ = nEditY;                      // y
	*p++ = nEditW;                      // cx
	*p++ = nEditH;                      // cy
	*p++ = INPUT_ID_EDT1;               // ID
	*p++ = (WORD)0xffff;                //Class Name Or Class Ary
	*p++ = (WORD)0x0081;                //0x0080 Edit 
	nchar = NStr::WcsCopy((WCHAR *)p, L"Edit") + 1;    //title
	p += nchar;                         //title
	*p++ = 0;                           //extraCount
	p = (WORD *)NData::Align(p);        // Align DWORD boundary 


	nButtonX = AglinRct.left;
	nButtonY = nHeigh - AglinRct.bottom - nButtonH;

	///////////////////////   DLGITEMTEMPLATEEX OK Button   ///////////////////////////
	*p++ = LOWORD (dwButStyle);         //style
	*p++ = HIWORD (dwButStyle);         //style
	*p++ = LOWORD (lExStyle);           // LOWORD (dwExtendedStyle)
	*p++ = HIWORD (lExStyle);           // HIWORD (dwExtendedStyle)
	*p++ = nButtonX;				    // x
	*p++ = nButtonY;					// y
	*p++ = nButtonW;                    // cx
	*p++ = nButtonH;                    // cy
	*p++ = IDOK;                        // ID
	*p++ = (WORD)0xffff;                //Class Name Or Class Arry
	*p++ = (WORD)0x0080;                //0x0080 Button 
	nchar = NStr::WcsCopy((WCHAR *)p, pBtnOk) + 1;    //title
	p += nchar;                         //title
	*p++ = 0;                           //extraCount
	p = (WORD *)NData::Align(p);               //Align DWORD boundary 

	nButtonX = nWidth - AglinRct.right - nButtonW;
	nButtonY = nHeigh - AglinRct.bottom - nButtonH;

	///////////////////////   Init for DLGITEMTEMPLATEEX  Cancel  Button  //////////////
	*p++ = LOWORD (dwButStyle);             //style
	*p++ = HIWORD (dwButStyle);             //style
	*p++ = LOWORD (lExStyle);           // LOWORD (dwExtendedStyle)
	*p++ = HIWORD (lExStyle);           // HIWORD (dwExtendedStyle)
	*p++ = nButtonX;                    // x
	*p++ = nButtonY;                    // y
	*p++ = nButtonW;                    // cx
	*p++ = nButtonH;                    // cy
	*p++ = IDCANCEL;                    // ID
	*p++ = (WORD)0xffff;                //Class Name Or Class Arry
	*p++ = (WORD)0x0080;                //0x0080 Button 
	nchar = NStr::WcsCopy((WCHAR *)p, pBtnCancel) + 1;//title
	p += nchar;                         //title
	*p++ = 0;                           //extraCount
	p = (WORD *)NData::Align(p);        //Align DWORD boundary

	nRetVal = (int)DialogBoxIndirectParam((HINSTANCE)::GetWindowLong(hWnd, GWL_HINSTANCE), 
		(LPDLGTEMPLATE)pdlgtemplate, hWnd, (DLGPROC) InputProc, (LPARAM)&DlgData);

	LocalFree (LocalHandle (pdlgtemplate));

	return nRetVal;
}


int		NDlg::GetOneChar()
{
	CHAR		szBuf[1024];

	szBuf[0] = 0;
	GetInput(NULL, szBuf, sizeof(szBuf), "GetOneChar");
	return szBuf[0];
}


__declspec(naked) int	NDlg::InputScanf(const CHAR *format, ...)
{
	//	DEBUG°æ±¾¿ÉÒÔÓÃ _inputº¯Êý, µ«ÊÇRelease CRTÃ»ÓÐµ¼³ö¸Ãº¯Êý
	//	Õ÷ÓÃ·µ»ØµØÖ·×÷Îª sscanf µÄµÚÒ»¸ö²ÎÊý
	//	Òò´ËÔÚ sscanf Ê±¾Ö²¿±äÁ¿»á±»ÆÆ»µ, ËùÒÔ:
	//		1. ²»ÄÜÊ¹ÓÃ¾Ö²¿±äÁ¿, ÓÃ¾²Ì¬±äÁ¿´úÌæ
	//		2. Õ»±»ÆÆ»µ, ÐèÒª×Ô¼º±£´æ¼Ä´æÆ÷, Ê¹ÓÃ naked
	static	CHAR		szBuf[1024];
	static	BYTE **		pReqsAddr;
	static	BYTE *		pOldValue;
	static	BYTE *		OldEsp;
	static	BYTE *		SetEsp;

	static	BYTE *		OldEbx;
	static	BYTE *		OldEcx;
	static	BYTE *		OldEdx;
	static	BYTE *		OldEsi;
	static	BYTE *		OldEdi;
	static	BYTE *		OldEbp;

	static	int			UintSize = sizeof(int);

	__asm	mov OldEbx, ebx;
	__asm	mov OldEcx, ecx;
	__asm	mov OldEdx, edx;
	__asm	mov OldEsi, esi;
	__asm	mov OldEdi, edi;
	__asm	mov OldEbp, ebp;

	__asm	sub esp, UintSize
	__asm	mov ebp, esp
	__asm	sub esp, __LOCAL_SIZE

	pReqsAddr = (BYTE **)(&format - 1);
	SetEsp = (BYTE *)(pReqsAddr);
	pOldValue = *pReqsAddr;

	szBuf[0] = 0;
	GetInput(NULL, szBuf, sizeof(szBuf), szLastPrint, 300);

	*pReqsAddr = (BYTE *)szBuf;

	__asm	mov OldEsp, esp;
	__asm	mov esp, SetEsp;
	__asm	call sscanf
	__asm	mov esp, OldEsp;

	*pReqsAddr = pOldValue;

	__asm	mov ebx, OldEbx;
	__asm	mov ecx, OldEcx;
	__asm	mov edx, OldEdx;
	__asm	mov esi, OldEsi;
	__asm	mov edi, OldEdi;


	__asm	mov esp, ebp
	__asm	mov ebp, OldEbp;
	__asm	add esp, UintSize
	__asm	ret
}


BOOL	SetDlgItemTexts(HWND hDlgWnd, NDlg::ITEMS_DATA &ItemData)
{
	BOOL		bRetVal;

	switch(ItemData.nType)
	{
	case NDlg::Item_CheckBox:
		bRetVal = ::SetDlgItemTextA(hDlgWnd, ItemData.nID, ItemData.pText);
		break;
	default:
		return FALSE;
	}

	return bRetVal;
}


BOOL	GetDlgItemValue(HWND hDlgWnd, NDlg::ITEMS_DATA &ItemData)
{
	BOOL		bRetVal;
	int			nRetVal;
	HWND		hWnd;

	bRetVal = TRUE;

	switch(ItemData.nType)
	{
	case NDlg::Item_CheckBox:
		hWnd = ::GetDlgItem(hDlgWnd, ItemData.nID);
		nRetVal = (int)::SendMessage(hWnd, BM_GETCHECK, 0, 0);
		*(int *)ItemData.pValue = nRetVal;
		break;
	default:
		return FALSE;
	}

	return bRetVal;
}


BOOL	SetDlgItemValue(HWND hDlgWnd, NDlg::ITEMS_DATA &ItemData)
{
	BOOL		bRetVal;
	int			nValue;
	HWND		hWnd;
	CHAR		szOldText[1024];
	CHAR		szBuffer[1024];
	CHAR *		pText;

	bRetVal = TRUE;

	switch(ItemData.nType)
	{
	case NDlg::Item_CheckBox:
		hWnd = ::GetDlgItem(hDlgWnd, ItemData.nID);
		nValue = *(int *)ItemData.pValue;
		::SendMessage(hWnd, BM_SETCHECK, (WPARAM)nValue, 0);
		break;

	case NDlg::Item_Edit:
		hWnd = ::GetDlgItem(hDlgWnd, ItemData.nID);
		if(ItemData.pValue)
		{
			nValue = *(int *)ItemData.pValue;
			itoa(nValue, szBuffer, 10);
			pText = szBuffer;
		}
		else
			pText = ItemData.pText;
		::GetWindowTextA(hWnd, szOldText, sizeof(szOldText));
		if(strcmp(szOldText, pText) == 0)
			break;

		::SendMessage(hWnd, WM_SETTEXT, (WPARAM)NULL, (LPARAM)pText);
		break;

	default:
		return FALSE;
	}

	return bRetVal;
}


//	·µ»ØÑ¡ÔñµÄ ID
int		NDlg::GetRadioChecked(HWND hDlgWnd, int nFirstId, int nLastId)
{
	int		nCurId;
	BOOL	bRetVal;

	for(nCurId=nFirstId; nCurId<=nLastId; nCurId++)
	{
		bRetVal = IsDlgButtonChecked(hDlgWnd, nCurId);
		if(bRetVal)
			return nCurId;
	}

	return 0;
}


//	·µ»ØÑ¡ÔñµÄ ID
int		NDlg::SetRadioChecked(HWND hDlgWnd, int nFirstId, int nLastId, int nSelectId)
{
	int		nCurId;

	for(nCurId=nFirstId; nCurId<=nLastId; nCurId++)
	{
		if(nSelectId == nCurId)
			CheckDlgButton(hDlgWnd, nCurId, BST_CHECKED);
		else
			CheckDlgButton(hDlgWnd, nCurId, BST_UNCHECKED);
	}

	return (nSelectId - nFirstId);
}


BOOL	NDlg::SetDlgItemsText(HWND hDlgWnd, NDlg::ITEMS_DATA *pData)
{
	int				i;

	for(i=0; ; i++)
	{
		if(pData[i].pText == NULL)
			break;
		if(Item_None == pData[i].nType)
			continue;

		SetDlgItemTexts(hDlgWnd, pData[i]);
	}

	return TRUE;
}


BOOL	NDlg::GetDlgItemsValue(HWND hDlgWnd, NDlg::ITEMS_DATA *pData)
{
	int				i;
	int				nRadioFirstId;
	int				nRadioLastId;
	int				nSelectId;
	int				nRadioVal;

	nRadioFirstId = 0;
	for(i=0; ; i++)
	{
		if(Item_Radio == pData[i].nType)
		{
			if(nRadioFirstId == 0)
			{
				nRadioFirstId = pData[i].nID;
			}
			else if(pData[i].pValue != pData[i+1].pValue)
			{
				nRadioLastId = pData[i].nID;
				nSelectId = GetRadioChecked(hDlgWnd, nRadioFirstId, nRadioLastId);
				nRadioVal = nSelectId - nRadioFirstId;
				if(nRadioVal < 0)
					nRadioVal = 0;

				*(int *)pData[i].pValue = nRadioVal;
				nRadioFirstId = 0;
			}
			continue;
		}

		if(pData[i].pValue == NULL)
			break;
		if(Item_None == pData[i].nType)
			continue;


		GetDlgItemValue(hDlgWnd, pData[i]);
	}

	return TRUE;
}
  

BOOL	NDlg::SetDlgItemsValue(HWND hDlgWnd, ITEMS_DATA *pData)
{
	int				i;
	int				nRadioFirstId;
	int				nRadioLastId;
	int				nSelectId;
	int				nRadioVal;

	nRadioFirstId = 0;
	for(i=0; ; i++)
	{
		if(Item_Radio == pData[i].nType)
		{
			if(nRadioFirstId == 0)
			{
				nRadioFirstId = pData[i].nID;
			}
			else if(pData[i].pValue != pData[i+1].pValue)
			{
				nRadioLastId = pData[i].nID;
				nRadioVal = *(int *)pData[i].pValue;
				nSelectId = nRadioFirstId + nRadioVal;
				if(nSelectId > nRadioLastId)
					nSelectId = nRadioFirstId;

				SetRadioChecked(hDlgWnd, nRadioFirstId, nRadioLastId, nSelectId);
				nRadioFirstId = 0;
			}
			continue;
		}

		if(pData[i].pValue == NULL &&
			pData[i].pText == NULL)
			break;
		if(Item_None == pData[i].nType)
			continue;

		SetDlgItemValue(hDlgWnd, pData[i]);
	}

	return TRUE;
}


int		NDlg::ListCtrl_ScrollToIndex(HWND hListWnd, int nIndex)
{
	int			nCurIndex; 
	RECT		ItemRect;
	int			y;
	BOOL		bRetVal;

	nCurIndex =  (int)::SendMessage(hListWnd, LVM_GETTOPINDEX, 0, 0);

	memset(&ItemRect, 0, sizeof(ItemRect));
	ItemRect.left = LVIR_BOUNDS;
	::SendMessage(hListWnd, LVM_GETITEMRECT, (WPARAM)0, (LPARAM)&ItemRect);

	y = (nIndex - nCurIndex) * (ItemRect.bottom - ItemRect.top);

	bRetVal = (BOOL)::SendMessage(hListWnd, LVM_SCROLL, 0, y);
	if(bRetVal == FALSE)
		return 0;

	return y; 
}


int		NDlg::EditOut(HWND hWnd, CONST CHAR * lpszFormat, ...)
{
	CHAR *		buf = NULL;
	int			nRetVal = -1, size = 4096;
	va_list		args;

	if(lpszFormat == NULL || hWnd == NULL)
		return 0;
	va_start(args, lpszFormat);

	for(; nRetVal<0 ; size*=2)
	{
		if(buf)	free(buf);
		buf = (CHAR *)malloc( (size+2)*sizeof(CHAR) );
		nRetVal = _vsnprintf(buf, size, lpszFormat, args);
	}

	va_end(args);
	strcat(buf, "\r\n");
	EditOutString(hWnd, buf);

	if(buf)	
		free(buf);
	return nRetVal;
}


int		NDlg::EditOutString(HWND hWnd, CHAR * pString)
{
	int		nRetVal;
	if(hWnd == NULL)
		return 0;

	if(pString == NULL)
	{
		::SendMessageA(hWnd, WM_SETTEXT, 0, (LPARAM)_T(""));
		return 0;
	}

	::SendMessageA(hWnd, EM_SETSEL, -2, -1);
	nRetVal = (int)::SendMessageA(hWnd, EM_REPLACESEL, 0, (DWORD)pString);

	return nRetVal;
}


BOOL	NDlg::SetWndFocus(HWND hWnd)
{
	DWORD		dwCurTid;
	DWORD		dwWndPid;
	DWORD		dwWndTid;
	HWND		hRetVal;

	dwCurTid = GetCurrentThreadId();
	dwWndTid = GetWindowThreadProcessId(hWnd, &dwWndPid);

	if(dwCurTid == dwWndTid)
	{
		::SetFocus(hWnd);
		return TRUE;
	}

	AttachThreadInput(dwCurTid, dwWndTid, TRUE);
	hRetVal = ::SetFocus(hWnd);
	AttachThreadInput(dwCurTid, dwWndTid, FALSE);

	if(hRetVal)
		return TRUE;
	return FALSE;
}


BOOL	NDlg::SwitchAppWnd(HWND hWnd)
{
	if(::IsIconic(hWnd))
		::ShowWindow(hWnd, SW_RESTORE);

	::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	::SetForegroundWindow(hWnd);

	return TRUE;
}

int		NDlg::ListBox_GetItemCount()
{

	return 0;
}


HTREEITEM	NDlg::TreeCtrl_RmtFindChildItem(HWND hWnd, HTREEITEM hParentItem, CHAR *pItemText, BOOL bNoCase)
{
	HTREEITEM		hCurItem;
	CProcessMem		TextMem;
	CProcessMem		ItemMem;
	TVITEMA			item;
	int				nTextMax;
	CHAR			szItemText[4096];
	int				nCompare;

	nTextMax = 4096;

	if(hParentItem == NULL)
		hParentItem = TreeView_GetRoot(hWnd);

	hCurItem = TreeView_GetChild(hWnd, hParentItem);
	if(hCurItem == NULL)
		return NULL;

	TextMem.Open(hWnd);
	TextMem.Alloc(nTextMax);

	ItemMem.Open(hWnd);
	ItemMem.Alloc(sizeof(item));

	for(;;)
	{
		memset(&item, 0, sizeof(item));
		item.hItem = hCurItem;
		item.mask = TVIF_TEXT;
		item.pszText = (CHAR *)TextMem.GetData();
		item.cchTextMax = nTextMax;

		ItemMem.Wirte(&item, sizeof(item));
		::SendMessage(hWnd, TVM_GETITEMA, 0, (LPARAM)ItemMem.GetData());
		
		TextMem.Read(szItemText, sizeof(szItemText));

		if(bNoCase)
			nCompare = stricmp(szItemText, pItemText);
		else
			nCompare = strcmp(szItemText, pItemText);

		if(nCompare == 0)
			return hCurItem;

		hCurItem = TreeView_GetNextSibling(hWnd, hCurItem);
		if(hCurItem == NULL)
			return NULL;
	}

	return NULL;
}


HTREEITEM	NDlg::TreeCtrl_RmtFindChildItem(HWND hWnd, HTREEITEM hParentItem, int nIndex)
{
	HTREEITEM		hCurItem;
	int				i;

	if(hParentItem == NULL)
		hParentItem = TreeView_GetRoot(hWnd);

	hCurItem = TreeView_GetChild(hWnd, hParentItem);
	if(hCurItem == NULL)
		return NULL;

	for(i=0; ; i++)
	{
		if(nIndex == i)
			return hCurItem;

		hCurItem = TreeView_GetNextSibling(hWnd, hCurItem);
		if(hCurItem == NULL)
			return NULL;
	}

	return NULL;
}

HTREEITEM	NDlg::TreeCtrl_GetNextItem(HWND hWnd, HTREEITEM hCurItem, DWORD *pdwDeep)
{
	HTREEITEM		hRetItem, hFindItem;

	if(hCurItem == NULL) 
	{
		hRetItem = (HTREEITEM)::SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_ROOT, 0);
		(*pdwDeep) = 0;
		return hRetItem;
	}

	hFindItem = (HTREEITEM)::SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hCurItem);
	if(hFindItem)	
	{
		(*pdwDeep) ++;
		return hFindItem;
	}

	hFindItem = (HTREEITEM)::SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hCurItem);
	if(hFindItem)
		return hFindItem;

	hFindItem = hCurItem;
	for(;;)
	{
		hFindItem = (HTREEITEM)::SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hFindItem);
		if(hFindItem == NULL)
			return NULL;
		(*pdwDeep) --;

		hRetItem = (HTREEITEM)::SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hFindItem);
		if(hRetItem)
			return hRetItem;
	}
	return NULL;
}


BOOL		NDlg::TreeCtrl_RmtSelectItem(HWND hWnd, std::vector<std::string> &ItemPath)
{
	std::vector<std::string>::iterator		Iter;
	CHAR *			pItemName;
	DWORD			dwProcId;
	CAutoHandle		hProc;
	HTREEITEM		hChild;
	HTREEITEM		hCurItem;
	int				nIndex;

	GetWindowThreadProcessId(hWnd, &dwProcId);
	hProc = OpenProcess(SYNCHRONIZE, FALSE, dwProcId);
	if(hProc == NULL)
		return FALSE;

	::SetFocus(hWnd);

	hCurItem = TreeView_GetRoot(hWnd);
	for(Iter = ItemPath.begin();
		Iter != ItemPath.end();
		Iter ++)
	{
		pItemName = (CHAR *)Iter->c_str();

		if(pItemName[0] == '\\')
		{
			nIndex = atoi(pItemName+1);
			hChild = TreeCtrl_RmtFindChildItem(hWnd, hCurItem, nIndex);
		}
		else
		{
			hChild = TreeCtrl_RmtFindChildItem(hWnd, hCurItem, pItemName);
		}

		if(hChild == NULL)
			return FALSE;

		TreeView_SelectItem(hWnd, hChild);
		TreeView_Expand(hWnd, hChild, TVE_EXPAND);
		hCurItem = hChild;
	}

	return TRUE;
}


int		NDlg::ListCtrl_InsertItem(HWND hWnd, int nIndex, CHAR *pText, int nImage)
{
	LVITEMA		Item;
	int			nRetIndex;

	memset(&Item, 0, sizeof(Item));
	Item.iIndent = nIndex;
	Item.pszText = pText;
	Item.iImage = nImage;

	if(nImage != -1)
        Item.mask = LVIF_TEXT | LVIF_IMAGE;
	else
		Item.mask = LVIF_TEXT;

	nRetIndex = (int)SendMessage(hWnd, LVM_INSERTITEMA, 0, (LPARAM)&Item);

	return nRetIndex;
}


BOOL	NDlg::ListCtrl_SetItemText(HWND hWnd, int nIndex, int nSubItem, CHAR *pText)
{
	LVITEMA		Item;
	BOOL		bRetVal;

	memset(&Item, 0, sizeof(Item));
	Item.iIndent = nIndex;
	Item.iSubItem = nSubItem;
	Item.pszText = pText;
	bRetVal = (BOOL)SendMessage(hWnd, LVM_SETITEMTEXTA, (WPARAM)nIndex, (LPARAM)&Item);

	return bRetVal;
}

BOOL	NDlg::ListCtrl_DeleteAllItems(HWND hWnd)
{
	BOOL		bRetVal;
	bRetVal = (BOOL)SendMessage(hWnd, LVM_DELETEALLITEMS, (WPARAM)0, (LPARAM)0);
	return bRetVal;
}

int			NDlg::ListCtrl_RmtFindItem(HWND hWnd, CHAR *pItemText, BOOL bNoCase)
{
	CProcessMem		TextMem;
	CProcessMem		ItemMem;
	LVITEMA			item;
	int				nTextMax;
	CHAR			szItemText[4096];
	int				nCompare;
	int				nCount;
	int				i;
	int				nRetVal;

	nTextMax = 4096;
	TextMem.Open(hWnd);
	TextMem.Alloc(nTextMax);

	ItemMem.Open(hWnd);
	ItemMem.Alloc(sizeof(item));

	nCount = ListView_GetItemCount(hWnd);
	for(i=0; i<nCount; i++)
	{
		memset(&item, 0, sizeof(item));
		item.iSubItem = 0;
		item.cchTextMax = nTextMax;
		item.pszText = (CHAR *)TextMem.GetData();

		ItemMem.Wirte(&item, sizeof(item));
		nRetVal = (int)::SendMessage(hWnd, LVM_GETITEMTEXTA, (WPARAM)i, (LPARAM)ItemMem.GetData());
		TextMem.Read(szItemText, sizeof(szItemText));

		if(bNoCase)
			nCompare = stricmp(szItemText, pItemText);
		else
			nCompare = strcmp(szItemText, pItemText);

		if(nCompare == 0)
			return i;
	}

	return -1;
}


BOOL		NDlg::ListCtrl_GetItemCount(HWND hWnd, int *pnRows, int *pnCols)
{
	HWND		hHeadCtrl;
	BOOL		bResult;

	bResult = TRUE;
	*pnRows = (int)::SendMessage(hWnd, LVM_GETITEMCOUNT, 0, 0);

	hHeadCtrl = (HWND)::SendMessage(hWnd, LVM_GETHEADER, 0, 0);
	if(hHeadCtrl == NULL)
		bResult = FALSE;

	*pnCols = (int)::SendMessage(hHeadCtrl, HDM_GETITEMCOUNT, 0, 0);
	return bResult;
}


BOOL		NDlg::ListCtrl_RmtSelectItem(HWND hWnd, CHAR *pItemText, BOOL bNoCase)
{
	int		nIndex;
	BOOL	bRetVal;

	nIndex = ListCtrl_RmtFindItem(hWnd, pItemText, bNoCase);
	if(nIndex == -1)
		return FALSE;

	bRetVal = ListCtrl_RmtSelectItem(hWnd, nIndex);
	bRetVal = ListCtrl_EnsureVisible(hWnd, nIndex, FALSE);

	return bRetVal;
}


BOOL		NDlg::ListCtrl_RmtSelectItem(HWND hWnd, int nIndex)
{
	std::vector<int>			SelList;
	std::vector<int>::iterator	Iter;
	CProcessMem					ItemMem;
	BOOL						bRetVal;
	LVITEMA						Item;
	int							nRetVal;

	bRetVal = ItemMem.Open(hWnd);
	if(bRetVal == FALSE)
		return FALSE;
	bRetVal = ItemMem.Alloc(sizeof(Item));
	if(bRetVal == FALSE)
		return FALSE;

	ListCtrl_GetSelectItems(hWnd, SelList);

	for(Iter = SelList.begin();
		Iter != SelList.end();
		Iter ++)
	{
		memset(&Item, 0, sizeof(Item));
		Item.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
		Item.state = 0;

		ItemMem.Wirte(&Item, sizeof(Item));
		nRetVal = (int)::SendMessage(hWnd, LVM_SETITEMSTATE, (WPARAM)(*Iter), (LPARAM)ItemMem.GetData());
	}

	memset(&Item, 0, sizeof(Item));
	Item.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
	Item.state = LVIS_SELECTED | LVIS_FOCUSED;
	ItemMem.Wirte(&Item, sizeof(Item));
	nRetVal = (int)::SendMessage(hWnd, LVM_SETITEMSTATE, (WPARAM)nIndex, (LPARAM)ItemMem.GetData());

	return (BOOL)nRetVal;
}

BOOL		NDlg::ListCtrl_GetSelectItems(HWND hWnd, std::vector<int> &SelList)
{
	int			nIndex;

	nIndex = -1;
	SelList.clear();

	for(;;)
	{
		nIndex = ListView_GetNextItem(hWnd, nIndex, LVIS_SELECTED);
		if(nIndex == -1)
			return TRUE;

		SelList.push_back(nIndex);
	}

	return TRUE;
}


BOOL	NDlg::ListCtrl_EnsureVisible(HWND hWnd, int nIndex, BOOL bPartialOK)
{
	int		nRetVal;

	nRetVal = (int)::SendMessage(hWnd, LVM_ENSUREVISIBLE, nIndex, MAKELPARAM(bPartialOK, 0));

	return (BOOL)nRetVal;
}


int		NDlg::ListCtrl_GetNextItem(HWND hWnd, int nIndex, UINT uFlag)
{
	nIndex = (int)::SendMessage(hWnd, LVM_GETNEXTITEM, nIndex, MAKELPARAM(uFlag, 0));

	return nIndex;
}



BOOL	NDlg::ListCtrl_CheckItem(HWND hWnd, int nIndex, BOOL bCheck)
{
	int			nRetVal;
	LVITEMA		Item;

	Item.stateMask = LVIS_STATEIMAGEMASK;
	if(bCheck)
		Item.state = 2 << 12;
	else
		Item.state = 1 << 12;
	nRetVal = (int)::SendMessage(hWnd, LVM_SETITEMSTATE, (WPARAM)nIndex, (LPARAM)&Item);

	return TRUE;
}

NDlg::ITEM_TYPE	NDlg::GetControlType(HWND hWnd)
{
	CHAR		szClassName[256];
	int			nRetVal;
	DWORD		dwStyle;

	nRetVal = GetClassNameA(hWnd, szClassName, sizeof(szClassName));
	if(nRetVal == 0)
		return Item_UnKnow;

	if(strcmp(szClassName, "Button") == 0)
	{
		dwStyle = (DWORD)GetWindowLongA(hWnd, GWL_STYLE);
		switch(dwStyle)
		{
		case BS_CHECKBOX:
		case BS_AUTOCHECKBOX:
			return Item_CheckBox;
		case BS_RADIOBUTTON:
		case BS_AUTORADIOBUTTON:
			return Item_Radio;
		case BS_GROUPBOX:
			return Item_Group;
		default:
			return Item_Button;
		}
		return Item_Button;
	}
	if(strcmp(szClassName, "Edit") == 0)
		return Item_Edit;

	if(strcmp(szClassName, "ComboBox") == 0 ||
		strcmp(szClassName, "ComboBoxEx32") == 0 ||
		strcmp(szClassName, "TComboBox") == 0 ||
		strcmp(szClassName, "TComboBoxEx32") == 0 ||
		0)
		return Item_ComBox;
	if(strcmp(szClassName, "ListBox") == 0)
		return Item_ListBox;
	if(strcmp(szClassName, "Static") == 0)
		return Item_Static;
	if(strcmp(szClassName, "ScrollBar") == 0)
	{
		dwStyle = (DWORD)GetWindowLongA(hWnd, GWL_STYLE);
		if(dwStyle & SBS_VERT)
			return Item_V_Scroll;
		else
			return Item_H_Scroll;
	}
	if(strcmp(szClassName, "msctls_trackbar32") == 0)
		return Item_Slider;
	if(strcmp(szClassName, "msctls_updown32") == 0)
		return Item_Spin;
	if(strcmp(szClassName, "msctls_progress32") == 0)
		return Item_Progress;
	if(strcmp(szClassName, "msctls_hotkey32") == 0)
		return Item_HotKey;
	if(strcmp(szClassName, "SysListView32") == 0)
		return Item_ListCtrl;
	if(strcmp(szClassName, "SysTreeView32") == 0 ||
		stricmp(szClassName, "TTreeView") == 0)
		return Item_TreeCtrl;
	if(strcmp(szClassName, "SysTabControl32") == 0)
		return Item_TabCtrl;

	ATLTRACE("Unknow: %s\n", szClassName);

	return Item_UnKnow;
}


BOOL	NDlg::ListBox_GetFullText(HWND hWnd, std::string &sText, CHAR *pItemSplite)
{
	int				nCount;
	int				i;
	CMiniMem		Mem;
	CHAR *			pReadText;
	DWORD			dwAllocLen;
	int				nItemLen;

	sText = "";
	nCount = (int)::SendMessage(hWnd, LB_GETCOUNT, 0, 0);
	if(nCount == 0)
		return TRUE;

	dwAllocLen = 4096;
	pReadText = (CHAR *)Mem.Alloc(dwAllocLen);	//ListBoxÎÞÐèÔ¶Ïß³Ì·ÖÅäÄÚ´æ

	for(i=0; i<nCount; i++)
	{
		nItemLen = (int)::SendMessage(hWnd, LB_GETTEXTLEN, i, 0);
		if(nItemLen >= (int)dwAllocLen)
		{
			Mem.FreeAll();
			dwAllocLen = (DWORD)nItemLen * 3 / 2;
			Mem.Alloc(dwAllocLen);
		}

		strcpy(pReadText, "[Error: LB_GETTEXT]");
		nItemLen = (int)SendMessage(hWnd, LB_GETTEXT, i, (LPARAM)pReadText);

		sText += pReadText;
		sText += pItemSplite;
	}

	return TRUE;
}

//	©¸©À©¦
CHAR *	c_TreeSplite[] = {"©Ç", "©»", "©§", "  ", "\r\n"};

BOOL	NDlg::TreeCtrl_GetFullText(HWND hWnd, std::string &sText, CHAR *ppItemSplite[])
{
	if(ppItemSplite == NULL)
		ppItemSplite = c_TreeSplite;

	int			nCount;
	HTREEITEM	hNextItem;
	int			j;

	TVITEMA		TreeItem;
	CProcessMem	ItemMem;
	CProcessMem	TextMem;

	DWORD		dwDeep;
	DWORD		dwAllocText;
	CHAR *		pReadText;
	CMiniMem	Mem;
	int			nRetVal;

	ItemMem.Open(hWnd);
	TextMem.Open(hWnd);

	dwAllocText = 4096;
	TextMem.Alloc(dwAllocText);
	pReadText = (CHAR *)Mem.Alloc(dwAllocText);

	memset(&TreeItem, 0, sizeof(TreeItem));
	TreeItem.mask = TVIF_TEXT | TVIF_HANDLE;
	TreeItem.pszText = (CHAR *)TextMem.GetData();
	TreeItem.cchTextMax = dwAllocText;
	ItemMem.Alloc(sizeof(TreeItem));

	nCount = (int)::SendMessage(hWnd, TVM_GETCOUNT, 0, 0);

	sText = "";
	hNextItem = NULL;
	for(;;)
	{
		hNextItem = TreeCtrl_GetNextItem(hWnd, hNextItem, &dwDeep);
		if(hNextItem == NULL)
			break;

		TreeItem.hItem = hNextItem;
		ItemMem.Wirte(&TreeItem, sizeof(TreeItem));

		strcpy(pReadText, "[Error: TVM_GETITEM]");
		nRetVal = (int)::SendMessage(hWnd, TVM_GETITEM, 0, (LPARAM)ItemMem.GetData());
		if(nRetVal)
			TextMem.Read(pReadText, dwAllocText);

		std::vector<std::string>		vLines;
		std::string						sLines;
		HTREEITEM						hFind;
		HTREEITEM						hBrother;

		hFind = hNextItem;
		vLines.resize(dwDeep);
		for(j=1; j<=(int)dwDeep; j++)
		{
			hBrother = (HTREEITEM)::SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_NEXT, (int)hFind);
			if(j == 1)
			{
				if(hBrother)
					vLines[dwDeep-j] = ppItemSplite[0];		//"©Ç"
				else
					vLines[dwDeep-j] = ppItemSplite[1];		//"©»"
			}
			else
			{
				if(hBrother)
					vLines[dwDeep-j] = ppItemSplite[2];		//"©§"
				else
					vLines[dwDeep-j] = ppItemSplite[3];		//"  "
			}
			hFind = (HTREEITEM)::SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hFind);
			assert(hFind);
		}
		sLines = "";
		for(j=0; j<(int)dwDeep; j++)
			sLines += vLines[j];

		sText += sLines;
		sText += pReadText;
		sText += ppItemSplite[4];		//"\r\n"
	}

	return TRUE;
}


//
CHAR *	c_ListSplite[] = {"\t", "\r\n"};
BOOL	NDlg::ListCtrl_GetFullText(HWND hWnd, std::string &sText, CHAR *ppItemSplite[])
{
	if(ppItemSplite == NULL)
		ppItemSplite = c_ListSplite;

	int			nRows;
	int			nCols;
	int			i, j;
	LVITEMA		ListItem;
	CProcessMem	ItemMem;
	CProcessMem	TextMem;

	DWORD		dwAllocText;
	CHAR *		pReadText;
	CMiniMem	Mem;
	int			nRetVal;

	ItemMem.Open(hWnd);
	TextMem.Open(hWnd);

	dwAllocText = 4096;
	TextMem.Alloc(dwAllocText);
	pReadText = (CHAR *)Mem.Alloc(dwAllocText);

	memset(&ListItem, 0, sizeof(ListItem));
	ListItem.mask = TVIF_TEXT;
	ListItem.pszText = (CHAR *)TextMem.GetData();
	ListItem.cchTextMax = dwAllocText;
	ItemMem.Alloc(sizeof(ListItem));

	ListCtrl_GetItemCount(hWnd, &nRows, &nCols);

	sText = "";
	for(i=0; i<nRows; i++)
	{
		ListItem.iItem = i;
		for(j=0; j<nCols; j++)
		{
			ListItem.iSubItem = j;
			ItemMem.Wirte(&ListItem, sizeof(ListItem));

			strcpy(pReadText, "[Error: LVM_GETITEMTEXT]");
			nRetVal = (int)::SendMessage(hWnd, LVM_GETITEMTEXT, i, (LPARAM)ItemMem.GetData());
			if(nRetVal)
				TextMem.Read(pReadText, dwAllocText);

			sText += pReadText;
			sText += ppItemSplite[0];		//	"\t"
		}
		sText += ppItemSplite[1];		//	"\r\n"
	}

	return TRUE;
}



BOOL	NDlg::ComBox_GetFullText(HWND hWnd, std::string &sText, CHAR *pItemSplite)
{
	int			nCount;
	int			i;
	CHAR		szBuffer[10];
	CHAR *		pText;
	CMiniMem	Mem;
	int			nAllocSize;
	int			nTextLen;

	sText = "";

	pText = szBuffer;
	nAllocSize = sizeof(szBuffer);
	nCount = (int)::SendMessage(hWnd, CB_GETCOUNT, 0, 0);
	for(i=0; i<nCount; i++)
	{
		nTextLen = (int)::SendMessage(hWnd, CB_GETLBTEXTLEN, i, 0);

		if(nTextLen >= nAllocSize)
		{
			nAllocSize = nTextLen + 2;
			Mem.FreeAll();
			pText = (CHAR *)Mem.Alloc(nAllocSize);
		}

		pText[0] = 0;
		nTextLen = (int)::SendMessage(hWnd, CB_GETLBTEXT, i, (LPARAM)pText);
		sText += pText;
		sText += pItemSplite;
	}

	return TRUE;
}


int		NDlg::GetWindowQText(HWND hWnd, WCHAR *pBuffer, int nBufCount)
{
	int				nRetVal;
	std::wstring	wText;
	DWORD			dwValue;

	pBuffer[0] = 0;
	pBuffer[nBufCount-1] = 0;
	nRetVal = InternalGetWindowText(hWnd, pBuffer, nBufCount);
	if(nRetVal == 0)
	{
		nRetVal = (int)SendMessageTimeoutW(hWnd, WM_GETTEXT, nBufCount, (LPARAM)pBuffer, 
			SMTO_ABORTIFHUNG, 100, &dwValue);
		if(nRetVal != 0)
			nRetVal = dwValue;
	}

	return nRetVal;
}


BOOL	NDlg::GetWindowQText(HWND hWnd, std::string &sText)
{
	WCHAR			szBuffer[4];
	std::wstring	wText;
	std::string		sTextTmp;
	int				nSize;
	int				nRetVal;

	szBuffer[0] = 0;
	nSize = sizeof(szBuffer)/sizeof(WCHAR);
	nRetVal = GetWindowQText(hWnd, szBuffer, nSize);
	if(nRetVal == 0)
		return FALSE;

	wText = szBuffer;
	nRetVal ++;
	while(nRetVal >= nSize)
	{
		nSize *= 2;
		wText.resize(nSize);
		nRetVal = GetWindowQText(hWnd, &wText[0], nSize);
		nRetVal ++;
	}

	nSize = (int)wText.size();
	sTextTmp.resize(nSize*2);

	NStr::WChar2Char((WCHAR *)wText.c_str(), &sTextTmp[0]);
	sText = sTextTmp;

	return TRUE;
}


BOOL	NDlg::GetWindowFullText(HWND hWnd, std::string &sText)
{
	ITEM_TYPE		nType;
	BOOL			bRetVal;

	nType = GetControlType(hWnd);
	switch(nType)
	{
	case Item_ListBox:
		bRetVal = ListBox_GetFullText(hWnd, sText);
		return bRetVal;
	case Item_TreeCtrl:
		bRetVal = TreeCtrl_GetFullText(hWnd, sText);
		return bRetVal;
	case Item_ListCtrl:
		bRetVal = ListCtrl_GetFullText(hWnd, sText);
		return bRetVal;
	case Item_ComBox:
		bRetVal = ComBox_GetFullText(hWnd, sText);
		return bRetVal;
	default:
		bRetVal = GetWindowQText(hWnd, sText);
		return bRetVal;
	}

	return FALSE;
}


BOOL	NDlg::DrawWndRect(HWND hWnd, int nWidth, COLORREF crRect)
{
	HDC			hDC;
	RECT		rct;
	HPEN		hPen;
	HPEN		hOldPen;

	::GetWindowRect(hWnd, &rct);
	hDC=::GetWindowDC(hWnd);
	hPen=::CreatePen(PS_INSIDEFRAME, nWidth, crRect);

	hOldPen = (HPEN)::SelectObject(hDC,hPen);
	::SelectObject(hDC, GetStockObject(NULL_BRUSH));

	::SetROP2(hDC, R2_NOTXORPEN);
	::Rectangle(hDC, 0, 0, rct.right-rct.left, rct.bottom-rct.top);

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);
	::ReleaseDC(hWnd,hDC);

	return TRUE;
}

//	NDlg
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
//	Nntdll


#define NtDllExportFun(FunName)	\
	static Fun_##FunName FunName##_ = NULL;\
	if(FunName##_ == NULL)\
	{\
		FunName##_ = (Fun_##FunName)NSys::GetDllProcAdders("Ntdll.dll", #FunName);\
		if(FunName##_ == NULL)\
		{\
			assert(0);\
			return -1;\
		}\
	}\


Nntdll::NTSTATUS	Nntdll::NtFsControlFile(
									IN HANDLE               FileHandle,
									IN HANDLE               Event,
									IN PIO_APC_ROUTINE      ApcRoutine,
									IN PVOID                ApcContext,
									OUT PIO_STATUS_BLOCK    IoStatusBlock,
									IN ULONG                FsControlCode,
									IN PVOID                InputBuffer,
									IN ULONG                InputBufferLength,
									OUT PVOID               OutputBuffer,
									IN ULONG                OutputBufferLength)
{
	NTSTATUS		nRetVal;
	NtDllExportFun(NtFsControlFile);

	nRetVal = NtFsControlFile_(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, 
		FsControlCode, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength);
	return nRetVal;
}



NTSTATUS	Nntdll::NtSystemDebugControl(
								 IN SYSDBG_COMMAND       Command,
								 IN PVOID                InputBuffer OPTIONAL,
								 IN ULONG                InputBufferLength,
								 OUT PVOID               OutputBuffer OPTIONAL,
								 IN ULONG                OutputBufferLength,
								 OUT PULONG              ReturnLength OPTIONAL )
{
	NTSTATUS		nRetVal;
	NtDllExportFun(NtSystemDebugControl);

	nRetVal = NtSystemDebugControl_(Command, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength, ReturnLength);
	return nRetVal;
}


BOOL	Nntdll::RdMsr(DWORD dwFun, DWORD dwMsr[2])
{
	MSR_STRUCT		Msr;
	NTSTATUS		lRetVal;

	Msr.MsrNum = dwFun;
	Msr.NotUsed = 0;

	lRetVal = NtSystemDebugControl(SysDbgSysReadMsr, &Msr, sizeof(Msr), NULL, 0, NULL);
	if(lRetVal < 0)
		return FALSE;

	dwMsr[0] = Msr.MsrLo;
	dwMsr[1] = Msr.MsrHi;

	return TRUE;
}


NTSTATUS	Nntdll::NtUnmapViewOfSection(
								  IN HANDLE               ProcessHandle,
								  IN PVOID                BaseAddress )
{
	NTSTATUS		nRetVal;
	NtDllExportFun(NtUnmapViewOfSection);

	nRetVal = NtUnmapViewOfSection_(ProcessHandle, BaseAddress);
	return nRetVal;
}


NTSTATUS	Nntdll::NtLoadDriver(
						 PUNICODE_STRING DriverServiceName)
{
	NTSTATUS		nRetVal;
	NtDllExportFun(NtLoadDriver);

	nRetVal = NtLoadDriver_(DriverServiceName);
	return nRetVal;
}

NTSTATUS	Nntdll::NtUnloadDriver(
								 PUNICODE_STRING DriverServiceName)
{
	NTSTATUS		nRetVal;
	NtDllExportFun(NtUnloadDriver);

	nRetVal = NtUnloadDriver_(DriverServiceName);
	return nRetVal;
}


DWORD		Nntdll::RtlNtStatusToDosError(
								  NTSTATUS Status)
{
	DWORD		dwRetVal;
	NtDllExportFun(RtlNtStatusToDosError);

	dwRetVal = RtlNtStatusToDosError_(Status);
	return dwRetVal;
}


NTSTATUS	Nntdll::NtQueryInformationProcess(
									  IN HANDLE               ProcessHandle,
									  IN PROCESS_INFORMATION_CLASS ProcessInformationClass,
									  OUT PVOID               ProcessInformation,
									  IN ULONG                ProcessInformationLength,
									  OUT PULONG              ReturnLength)
{
	DWORD		dwRetVal;
	NtDllExportFun(NtQueryInformationProcess);

	dwRetVal = NtQueryInformationProcess_(ProcessHandle, ProcessInformationClass, ProcessInformation, 
		ProcessInformationLength, ReturnLength);
	return dwRetVal;
}


NTSTATUS	Nntdll::NtQuerySystemInformation(
									 IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
									 OUT PVOID               SystemInformation,
									 IN ULONG                SystemInformationLength,
									 OUT PULONG              ReturnLength OPTIONAL)
{
	DWORD		dwRetVal;
	NtDllExportFun(NtQuerySystemInformation);

	dwRetVal = NtQuerySystemInformation_(SystemInformationClass, SystemInformation, 
		SystemInformationLength, ReturnLength);
	return dwRetVal;
}


NTSTATUS	Nntdll::NtOpenDirectoryObject(
								  OUT PHANDLE             DirectoryObjectHandle,
								  IN ACCESS_MASK          DesiredAccess,
								  IN POBJECT_ATTRIBUTES   ObjectAttributes)
{
	DWORD		dwRetVal;
	NtDllExportFun(NtOpenDirectoryObject);

	dwRetVal = NtOpenDirectoryObject_(DirectoryObjectHandle, DesiredAccess, ObjectAttributes);
	return dwRetVal;
}




NTSTATUS	Nntdll::NtQueryDirectoryObject(
									IN HANDLE               DirectoryObjectHandle,
									OUT POBJDIR_INFORMATION DirObjInformation,
									IN ULONG                BufferLength,
									IN BOOLEAN              GetNextIndex,
									IN BOOLEAN              IgnoreInputIndex,
									IN OUT PULONG           ObjectIndex,
									OUT PULONG              DataWritten)
{
	DWORD		dwRetVal;
	NtDllExportFun(NtQueryDirectoryObject);

	dwRetVal = NtQueryDirectoryObject_(DirectoryObjectHandle, DirObjInformation, BufferLength, 
		GetNextIndex, IgnoreInputIndex, ObjectIndex, DataWritten);
	return dwRetVal;
}


NTSTATUS	Nntdll::NtQueryTimerResolution(
								   OUT PULONG              MinimumResolution,
								   OUT PULONG              MaximumResolution,
								   OUT PULONG              CurrentResolution )
{
	DWORD		dwRetVal;
	NtDllExportFun(NtQueryTimerResolution);

	dwRetVal = NtQueryTimerResolution_(MinimumResolution, MaximumResolution, CurrentResolution);
	return dwRetVal;
}


NTSTATUS	Nntdll::NtSetTimerResolution(
								 IN ULONG                DesiredResolution,
								 IN BOOLEAN              SetResolution,
								 OUT PULONG              CurrentResolution )
{
	DWORD		dwRetVal;
	NtDllExportFun(NtSetTimerResolution);

	dwRetVal = NtSetTimerResolution_(DesiredResolution, SetResolution, CurrentResolution);
	return dwRetVal;
}


NTSTATUS	Nntdll::NtQueryInformationFile(
								   IN HANDLE               FileHandle,
								   OUT PIO_STATUS_BLOCK    IoStatusBlock,
								   OUT PVOID               FileInformation,
								   IN ULONG                Length,
								   IN FILE_INFORMATION_CLASS FileInformationClass)
{
	DWORD		dwRetVal;
	NtDllExportFun(NtQueryInformationFile);

	dwRetVal = NtQueryInformationFile_(FileHandle, IoStatusBlock, FileInformation, Length, FileInformationClass);
	return dwRetVal;
}

NTSTATUS	Nntdll::NtQueryObject(
						  IN HANDLE               ObjectHandle,
						  IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
						  OUT PVOID               ObjectInformation,
						  IN ULONG                Length,
						  OUT PULONG              ResultLength)
{
	DWORD		dwRetVal;
	NtDllExportFun(NtQueryObject);

	dwRetVal = NtQueryObject_(ObjectHandle, ObjectInformationClass, ObjectInformation, Length, ResultLength);
	return dwRetVal;
}


DWORD	Nntdll::GetParentProcId(HANDLE hProc)
{
	PROCESS_BASIC_INFORMATION	pbi;
	DWORD		dwSize;
	DWORD		dwRetVal;

	dwRetVal = Nntdll::NtQueryInformationProcess(hProc, 
		Nntdll::ProcessBasicInformation, &pbi, sizeof(pbi), &dwSize);
	if(dwRetVal != 0)
		return 0;

	return pbi.InheritedFromUniqueProcessId;
}


BOOL	Nntdll::GetSysHandleList(std::vector<SYSTEM_HANDLE_INFORMATION> &vHandleList)
{
	Nntdll::SYSTEM_HANDLE_LIST *		pBuffer;
	DWORD			dwSize;
	LONG			lRetVal;
	DWORD			dwRead;
	DWORD			dwRealSize;
	CMiniMem		Mem;

	vHandleList.clear();
	pBuffer = NULL;
	for(dwSize = 100*1024;
		dwSize < 2*1024*1024;
		dwSize *= 2)
	{
		pBuffer = (Nntdll::SYSTEM_HANDLE_LIST *)Mem.Alloc(dwSize);
		lRetVal = Nntdll::NtQuerySystemInformation(Nntdll::SystemHandleInformation, pBuffer, dwSize, &dwRead);
		if(lRetVal == Nntdll::STATUS_SUCCESS)
			break;
		if(pBuffer)
		{
			Mem.Free(pBuffer);
			pBuffer = NULL;
		}
	}

	if(pBuffer == NULL)
		return FALSE;

	dwRealSize = sizeof(SYSTEM_HANDLE_INFORMATION)*pBuffer->dwCount + sizeof(DWORD);
	if(dwRealSize > dwSize)
	{
		assert(0);
		return FALSE;
	}

	vHandleList.assign(pBuffer->HandleInfo, pBuffer->HandleInfo+pBuffer->dwCount);
	return TRUE;
}


BOOL	Nntdll::GetSysModInfo(std::vector<SYSTEM_MODULE> &vModList)
{
	Nntdll::SYSTEM_MODULE_INFORMATION *		pBuffer;
	DWORD			dwSize;
	LONG			lRetVal;
	DWORD			dwRead;
	DWORD			dwRealSize;
	CMiniMem		Mem;

	vModList.clear();
	pBuffer = NULL;
	for(dwSize = 100*1024;
		dwSize < 2*1024*1024;
		dwSize *= 2)
	{
		pBuffer = (Nntdll::SYSTEM_MODULE_INFORMATION *)Mem.Alloc(dwSize);
		lRetVal = Nntdll::NtQuerySystemInformation(Nntdll::SystemModuleInformation, pBuffer, dwSize, &dwRead);
		if(lRetVal == Nntdll::STATUS_SUCCESS)
			break;
		if(pBuffer)
		{
			Mem.Free(pBuffer);
			pBuffer = NULL;
		}
	}

	if(pBuffer == NULL)
		return FALSE;

	dwRealSize = sizeof(SYSTEM_MODULE)*pBuffer->ModulesCount + sizeof(DWORD);
	if(dwRealSize > dwSize)
	{
		assert(0);
		return FALSE;
	}

	vModList.assign(pBuffer->Modules, pBuffer->Modules+pBuffer->ModulesCount);
	return TRUE;
}


DWORD	Nntdll::ReadKernelMem(DWORD dwAddress, DWORD dwSize, VOID *pBuffer)
{
	MEMORY_CHUNKS	KrnMem;
	LONG			lRetVal;
	DWORD			dwRead;

	KrnMem.Address = dwAddress;
	KrnMem.Data = pBuffer;
	KrnMem.Length = dwSize;

	dwRead = 0;
	lRetVal = NtSystemDebugControl(SysDbgReadCoreMem, &KrnMem, sizeof(KrnMem), NULL, 0, &dwRead);
	if(lRetVal != STATUS_SUCCESS)
		return dwRead;

	return dwRead;
}


DWORD	Nntdll::WriteKernelMem(DWORD dwAddress, DWORD dwSize, VOID *pBuffer)
{
	MEMORY_CHUNKS	KrnMem;
	LONG			lRetVal;
	DWORD			dwWrite;

	KrnMem.Address = dwAddress;
	KrnMem.Data = pBuffer;
	KrnMem.Length = dwSize;

	dwWrite = 0;
	lRetVal = NtSystemDebugControl(SysDbgWriteCoreMem, &KrnMem, sizeof(KrnMem), NULL, 0, &dwWrite);
	if(lRetVal != STATUS_SUCCESS)
		return dwWrite;

	return dwWrite;
}


BOOL	Nntdll::GetDireObjetList(CHAR *pPath, std::vector<OBJDIR_INFORMATION> &vObjList)
{
	Nntdll::UNICODE_STRING		uString;
	Nntdll::OBJECT_ATTRIBUTES	Oa;
	WCHAR				wzPath[MAXPATH];
	LONG				lRetVal;
	CMiniMem			Mem;
	DWORD				dwSize;
	CAutoHandle			hDir;
	DWORD				dwIndex;
	DWORD				dwRead;

	Nntdll::OBJDIR_INFORMATION *		pObjInfo;

	vObjList.clear();
	NStr::Char2WChar(pPath, wzPath, sizeof(wzPath));

	uString.Buffer = wzPath;
	uString.Length = (USHORT)wcslen(wzPath)*sizeof(WCHAR);
	uString.MaximumLength = sizeof(wzPath);

	memset(&Oa, 0, sizeof(Oa));
	Oa.Length = sizeof(Oa);
	Oa.ObjectName = &uString;

	lRetVal = Nntdll::NtOpenDirectoryObject(hDir.GetAddr(), Nntdll::DIRECTORY_QUERY, &Oa);
	if(lRetVal != STATUS_SUCCESS)
		return FALSE;

	dwSize = 100*1024;
	for(;;)
	{
		pObjInfo = (Nntdll::OBJDIR_INFORMATION *)Mem.Alloc(dwSize);
		lRetVal = Nntdll::NtQueryDirectoryObject(hDir, pObjInfo, dwSize, FALSE, TRUE, &dwIndex, &dwRead);
		if(lRetVal == STATUS_SUCCESS)
			break;

		dwSize *= 2;
		if(dwSize > 10*1024*1024)
			return FALSE;

		Mem.Free(pObjInfo);
		pObjInfo = NULL;
	}

	for(;;)
	{
		if(pObjInfo->ObjectName.Buffer == NULL)
			break;

		vObjList.push_back(*pObjInfo);
		pObjInfo = (Nntdll::OBJDIR_INFORMATION *)((BYTE *)pObjInfo + sizeof(Nntdll::UNICODE_STRING)*2);
	}

	return TRUE;
}



DWORD	Nntdll::GetParentProcId(DWORD dwPid)
{
	return 0;
}


//	ÐèÒªfree
Nntdll::SYSTEM_PROCESS_INFORMATION *	Nntdll::GetSysProcessInfo()
{
	SYSTEM_PROCESS_INFORMATION *		pSysProcInfo;
	DWORD			dwAllocSize;
	DWORD			dwReturnLength;
	NTSTATUS		Status;

	Status = STATUS_INFO_LENGTH_MISMATCH;
	pSysProcInfo = NULL;

	for(dwAllocSize = 100*1024;
		Status != STATUS_SUCCESS; 
		dwAllocSize *= 2)
	{
		if(pSysProcInfo)
			free(pSysProcInfo);
		pSysProcInfo = (SYSTEM_PROCESS_INFORMATION *)malloc(dwAllocSize);

		Status = NtQuerySystemInformation(SystemProcessInformation,
			pSysProcInfo, dwAllocSize, &dwReturnLength);

		if(dwAllocSize >= 20 * 1024 * 1024)
		{
			if(pSysProcInfo)
				free(pSysProcInfo);
			return NULL;
		}
	}

	return pSysProcInfo;
}


Nntdll::SYSTEM_PROCESS_INFORMATION *	Nntdll::GetProcessInfo(DWORD dwProcId, CHAR *pBuffer, int nBufSize)
{
	SYSTEM_PROCESS_INFORMATION *	pOneProcInfo;
	SYSTEM_PROCESS_INFORMATION *	pSysProcInfo;
	SYSTEM_PROCESS_INFORMATION *	pInfoNext;
	DWORD		dwCopySize;

	pSysProcInfo = GetSysProcessInfo();
	if(pSysProcInfo == NULL)
		return NULL;

	pOneProcInfo = NULL;
	for(pInfoNext = pSysProcInfo;
		;
		pInfoNext = (SYSTEM_PROCESS_INFORMATION *)((BYTE *)pInfoNext + pInfoNext->NextEntryOffset))
	{
		if(pInfoNext->NextEntryOffset == 0)
			break;
		if(pInfoNext->ProcessId != dwProcId)
			continue;

		dwCopySize = sizeof(SYSTEM_PROCESS_INFORMATION) + (pInfoNext->NumberOfThreads - ANY_SIZE) * sizeof(SYSTEM_THREAD);
		if(nBufSize < (int)dwCopySize)
			break;

		memcpy(pBuffer, pInfoNext, dwCopySize);
		pSysProcInfo = (SYSTEM_PROCESS_INFORMATION *)pBuffer;
		break;
	}

	free(pSysProcInfo);
	return pOneProcInfo;
}

VOID *	Nntdll::GetPEB(HANDLE hProc)
{
	PROCESS_BASIC_INFORMATION	pbi;
	DWORD		dwSize;
	DWORD		dwRetVal;

	if(hProc == NULL)
		hProc = GetCurrentProcess();

	dwRetVal = Nntdll::NtQueryInformationProcess(hProc, 
		Nntdll::ProcessBasicInformation, &pbi, sizeof(pbi), &dwSize);
	if(dwRetVal != 0)
		return 0;

	return pbi.PebBaseAddress;
}


#pragma warning(default: 4311 4312)



