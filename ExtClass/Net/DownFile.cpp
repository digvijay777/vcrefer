#include "stdafx.h"
#include "DownFile.h"
#include <assert.h>

#include <Winnetwk.h>
#pragma comment(lib, "Mpr.lib")
#pragma comment(lib, "Wininet.lib")

CDownFileBase::CDownFileBase()
{
	m_nDownError = DOWNFILE_READY;
	m_bDownCancel = FALSE;
	m_bForceDown = TRUE;
}

CDownFileBase::~CDownFileBase()
{

}

/*
 *	��NetPath����·��
 *
 *  ����: lpNetPath ����·��			lpLocalPath ���ش��Ŀ¼ 
 *        lpUser ��¼������			lpPasswd ��¼��������
 *        bLogout ��������Ƿ��˳���¼
 */
BOOL CDownFileBase::DoFromNetPath(LPCTSTR lpNetPath, LPCTSTR lpLocalPath, LPCTSTR lpUser /* = NULL */, 
								  LPCTSTR lpPasswd /* = NULL */, BOOL bLogout /* = FALSE */)
{
	TCHAR		szNetSvr[MAX_PATH]		= {0};
	TCHAR		szLocalFile[MAX_PATH*2]	= {0};
	TCHAR		szSvrFile[MAX_PATH]		= {0};
	TCHAR*		pStr					= NULL;
	BOOL		bRet					= FALSE;

	_tcscpy(szSvrFile, lpNetPath);
	_tcscpy(szLocalFile, lpLocalPath);
	ReplaceString(szSvrFile, _T('/'), _T('\\'));
	ReplaceString(szLocalFile, _T('/'), _T('\\'));

	if(_T('\\') != szLocalFile[_tcslen(szLocalFile) - 1])
		_tcscat(szLocalFile, _T("\\"));
	pStr = _tcsrchr(szSvrFile, _T('\\'));
	if(NULL == pStr)
		pStr = szSvrFile;
	else 
		pStr++;
	_tcscat(szLocalFile, pStr);

	// ɾ��ԭ�ļ�
	DeleteFile(szLocalFile);

	bRet = CopyFile(szSvrFile, szLocalFile, FALSE);
	if(FALSE == bRet && NULL != lpUser && NULL != lpPasswd)
	{
		NetBIOS_GetPath(lpNetPath, szNetSvr);
		if(FALSE == NetBIOS_Login(szNetSvr, lpUser, lpPasswd))
			return FALSE;
		bRet = CopyFile(szSvrFile, szLocalFile, FALSE);
		if(FALSE != bLogout)
			NetBIOS_Logout(szNetSvr);
	}
	
	return bRet;
}

void CDownFileBase::ReplaceString(LPTSTR lpStr, TCHAR src, TCHAR des)
{
	TCHAR*		p		= lpStr;
	
	for(p = lpStr; 0 != *p; p++)
	{
		if(src == *p)
			*p = des;
	}
}

void CDownFileBase::NetBIOS_GetPath(LPCTSTR lpNetFile, LPSTR lpNet)
{
	TCHAR*		pStr					= NULL;
	int			nIndex					= 0;
	
	// ���ո�
	for(nIndex = 0; nIndex < _tcslen(lpNetFile); nIndex++)
	{
		if(_T('\x20') != nIndex)
			break;
	}
	
	lpNet[0] = 0;
	// �ж�ǰ׺
	if(_T('\\') != lpNetFile[nIndex])
		_tcscat(lpNet, _T("\\"));
	if(_T('\\') != lpNetFile[nIndex+1])
		_tcscat(lpNet, _T("\\"));
	// ���� 
	_tcscat(lpNet, &lpNetFile[nIndex]);
	// ���IPC$
	pStr = _tcschr(&lpNet[2], _T('\\'));
	if(NULL != pStr)
		*pStr = 0;
	_tcscat(lpNet, _T("\\IPC$"));
}
/*
 *	��¼NetBIOS
 */
BOOL CDownFileBase::NetBIOS_Login(LPCTSTR lpNet, LPCTSTR lpUser, LPCTSTR lpPasswd)
{
	NETRESOURCEA	NetRes;
	DWORD			dwRetVal;

	memset(&NetRes, 0, sizeof(NetRes));
	NetRes.dwType = RESOURCETYPE_DISK;
	NetRes.lpRemoteName = (CHAR *)lpNet;
	dwRetVal = WNetAddConnection2A(&NetRes, lpPasswd, lpUser, 0);
	if(dwRetVal == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

/*
 *	�ǳ�NetBIOS
 */
BOOL CDownFileBase::NetBIOS_Logout(LPCTSTR lpNet)
{
	TCHAR		szSvr[MAX_PATH]		= {0};
	TCHAR*		pStr				= NULL;

	_tcscpy(szSvr, lpNet);
	pStr = _tcschr(&szSvr[2], _T('\\'));
	if(NULL != pStr)
		*pStr = 0;
	WNetCancelConnection2A(szSvr, 0, TRUE);
	return TRUE;
}

void CDownFileBase::NotifyUI(int nType, LPVOID lpParameter)
{
	// -----  nType  -----+-----  lParameter -----
	// NUIE_SetPorcess     int ����
	// NUIE_BeginDown      LPCTSTR ·��
	// NUIE_EndDown        int ������
}
/*
 *	������·������
 */
BOOL CDownFileBase::DoFromHTMLServer(LPCTSTR lpURL, LPCTSTR lpLocalPath)
{
	m_bDownCancel = FALSE;
	m_nDownError = DOWNFILE_READY;

	NotifyUI(NUIE_SetPorcess, 0);	// ֪ͨ����
	
	HINTERNET		hINetOpen				= NULL;
	HINTERNET		hINetConnect			= NULL;
	HINTERNET		hINetRequest			= NULL;
	BOOL			bRet;
	TCHAR			szSvrName[1024]			= {0};
	TCHAR			szObjPath[1024]			= {0};
	DWORD			dwFileSize				= 0;
	TCHAR			szLocalFile[MAX_PATH*2]	= {0};
	DWORD			dwDownSize				= NULL;
	HANDLE			hDownFile				= NULL;

	do 
	{
		// ��ʼ������
		Http_SplitURL(lpURL, szSvrName, szObjPath);
		Http_GetLocalPath(lpLocalPath, szObjPath, szLocalFile);
		bRet = Http_Init(hINetOpen, hINetConnect, hINetRequest, szSvrName, szObjPath);
		if(FALSE == bRet)
		{
			m_nDownError = DOWNLOAD_CONNECT_FAILED;
			break;
		}
		// �õ��ļ���С
		dwFileSize = Http_GetFileSize(hINetRequest);
		if(-1 == dwFileSize)
		{
			m_nDownError = DOWNLOAD_GETFILESIZE_FAILED;
			break;
		}
		// �Ƿ���Ҫ����
		bRet = Http_IsNeedDown(szLocalFile, dwFileSize);
		if(FALSE == bRet)
		{
			NotifyUI(NUIE_SetPorcess, (void *)100);
			m_nDownError = DOWNLOAD_ALL_SUCCESS;
			break;
		}
		// �����ļ�
		hDownFile = CreateFile(szLocalFile, GENERIC_WRITE, 
			0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(INVALID_HANDLE_VALUE == hDownFile)
		{
			m_nDownError = DOWNLOAD_CREATEFILE_FAILED;
			break;
		}
		// ��ʼ����
		dwDownSize = Http_DownFile(hINetRequest, hDownFile, dwFileSize);
		if(dwDownSize != dwFileSize)
		{
			if(FALSE != m_bDownCancel)
				m_nDownError = DOWNLOAD_EXIT_FAILED;
			else
				m_nDownError = DOWNLOAD_DISCONNECT_FAILED;
		}
		else
		{
			m_nDownError = DOWNLOAD_ALL_SUCCESS;
		}
		CloseHandle(hDownFile);
	} while (false);

	if(NULL != hINetRequest)
		InternetCloseHandle(hINetRequest);
	if(NULL != hINetConnect)
		InternetCloseHandle(hINetConnect);
	if(NULL != hINetOpen)
		InternetCloseHandle(hINetOpen);

	return (DOWNLOAD_ALL_SUCCESS == m_nDownError);
}

BOOL CDownFileBase::Http_Init(HINTERNET& hOpen, HINTERNET& hConnect, HINTERNET& hRequest, 
							 LPCTSTR lpSvrName, LPCTSTR lpObjectName)
{
	DWORD	dwTime = 15000;
	CHAR * 	pAcceptTypes[] = 
	{
		"*/*",
		NULL
	};

	hOpen = InternetOpen( "Mozilla/4.0 (compatible)", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
	InternetSetOption( hOpen, INTERNET_OPTION_CONNECT_TIMEOUT, (void*)&dwTime, 4 );
	hConnect = InternetConnect( hOpen, lpSvrName, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, 
		INTERNET_SERVICE_HTTP, 0, 0 );
	hRequest = HttpOpenRequestA( hConnect, "GET", lpObjectName, HTTP_VERSIONA, NULL, 
		(LPCSTR *)&pAcceptTypes[0], INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, 0);

	DWORD	dwReq = 0;
	DWORD	dwBufferLength = 4;
	if( NULL != hRequest )
	{
		if( HttpSendRequest(hRequest, NULL, 0, NULL, 0) )
		{
			if( !HttpQueryInfo(hRequest, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_STATUS_CODE,
				&dwReq, &dwBufferLength, NULL) || dwReq == HTTP_STATUS_OK	)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CDownFileBase::Http_SplitURL(LPCTSTR lpURL, LPTSTR lpSvrName, LPTSTR lpObjName)
{
	if(_tcsnicmp(_T("http://"), lpURL, 7) != 0)
	{
		assert(0);
		return;
	}
	
	TCHAR*		pStr		= NULL;

	_tcscpy(lpSvrName, &lpURL[7]);
	ReplaceString(lpSvrName, _T('\\'), _T('/'));
	pStr = _tcschr(lpSvrName, _T('/'));
	if(NULL == pStr)
	{
		assert(false);
		lpSvrName[0] = 0;
		return;
	}
	_tcscpy(lpObjName, (TCHAR *)(pStr+1));
	*pStr = 0;
}
void CDownFileBase::Http_GetLocalPath(LPCTSTR lpPrePath, LPCTSTR lpURL, LPTSTR lpPath)
{
	TCHAR*		pStr		= NULL;

	_tcscpy(lpPath, lpPrePath);
	if(_T('\\') != lpPath[_tcslen(lpPath) - 1])
		_tcscat(lpPath, _T("\\"));
	pStr = _tcsrchr(lpURL, _T('/'));
	if(NULL == pStr)
		pStr = (TCHAR *)lpURL;
	else
		pStr++;
	_tcscat(lpPath, pStr);
}

BOOL CDownFileBase::Http_IsNeedDown(LPCTSTR lpPath, DWORD dwSize)
{
	if(FALSE != m_bForceDown) // ǿ������
		return TRUE;

	HANDLE		hFile	= CreateFile(lpPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD		dwFile	= 0;

	if(INVALID_HANDLE_VALUE == hFile)
		return TRUE;
	dwFile = GetFileSize(hFile, NULL);
	CloseHandle(hFile);
	if( dwSize == dwFile)
		return FALSE;

	return TRUE;
}

DWORD CDownFileBase::Http_GetFileSize(HINTERNET& hRequest)
{
	DWORD		dwBufSize;
	BOOL		bRetVal;
	DWORD		dwSize;

	dwBufSize = sizeof(dwSize);
	bRetVal = HttpQueryInfoA(hRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
		&dwSize, &dwBufSize, NULL);
	if(bRetVal == FALSE)
		dwSize = -1; 

	return dwSize;
}

DWORD CDownFileBase::Http_DownFile(HINTERNET& hRequest, HANDLE hFile, DWORD dwSize)
{
	DWORD		dwComplate			= 0;
	BYTE		szRequest[8192]		= {0};

	while (TRUE)
	{
		if( FALSE != m_bDownCancel )
			return dwComplate;

		//////////////////////////////////////////////////////////////////////////
		DWORD	dwNumberOfBytesWritten = 0;
		
		memset(szRequest, 0, sizeof(szRequest));
		if( !InternetReadFile(hRequest, (void*)szRequest, sizeof(szRequest)-1, &dwNumberOfBytesWritten))
		{
			return dwComplate;
		}

		if( 0 == dwNumberOfBytesWritten )
			return dwComplate;

		if(FALSE == WriteFile(hFile, szRequest, dwNumberOfBytesWritten, &dwNumberOfBytesWritten, NULL))
			return dwComplate;
		dwComplate += dwNumberOfBytesWritten;

		int		nProg = 0;
		
		nProg = (int)((float)dwComplate / (float)dwSize * (float)100);
		NotifyUI(NUIE_SetPorcess, (void *)nProg);
	}

	return dwComplate;
}