// VirusWall.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "VirusWall.h"
#include "VirusWallDlg.h"

#include "UploadFileDlg.h"
#include "WMIBase.h"

#include "../../common/NSimple.h"
#include "../../common/ObjInfo/SystemInfo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVirusWallApp

BEGIN_MESSAGE_MAP(CVirusWallApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVirusWallApp ����

CVirusWallApp::CVirusWallApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	CHAR		szINI[MAXPATH]		= {0};

	m_strPwd = "";
	m_strUser = "";
	m_strVdbPath = "C:\\Inetpub\\wwwroot\\vc\\";

	NFile::GetFileInModulePath(szINI, "VirusWall.ini");
	m_nIDUser = GetPrivateProfileInt("INFO", "data1", -1, szINI);
	m_nIDPwd = GetPrivateProfileInt("INFO", "data2", -1, szINI);
	GetPrivateProfileString("INFO", "data3", "", m_strIP.GetBufferSetLength(MAX_PATH), MAX_PATH, szINI);
	m_strIP.ReleaseBuffer();
}


// Ψһ��һ�� CVirusWallApp ����

CVirusWallApp theApp;


// CVirusWallApp ��ʼ��

BOOL CVirusWallApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

//	CVirusWallDlg dlg;
	CUploadFileDlg dlg;

	dlg.m_strIP = m_strIP;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

// �õ�����ǽ�û���������
BOOL CVirusWallApp::GetVirusWallUserAndPwd(LPSTR lpIP)
{
	// ��ʼ����Ϣ
	CHAR*		arrUser[]		= {"root", NULL};
	CHAR*		arrPwd[]		= {"www.Tisson.cn", "www.sns.net.cn", NULL};

	if((-1 < m_nIDUser) && 
		(-1 < m_nIDPwd) && 
		("" != m_strIP))
	{
		if((m_nIDPwd < (sizeof(arrPwd) / sizeof(CHAR*))) && 
			(m_nIDUser < (sizeof(arrUser) / sizeof(CHAR*))))
		{
			m_strPwd = arrPwd[m_nIDPwd];
			m_strUser = arrUser[m_nIDUser];
			m_nIDUser = -1;
			m_nIDPwd = -1;
		}
	}

	if(m_strIP == lpIP)
	{
		if("" != m_strUser && "" != m_strPwd)
			return TRUE;
	}


	m_strUser = "";
	m_strPwd = "";
	m_strIP = lpIP;

	// ���ȵ�¼
	for(int iUser = 0; NULL != arrUser[iUser]; iUser++)
	{
		for(int iPwd = 0; NULL != arrPwd[iPwd]; iPwd++)
		{
			if(FALSE != TestLoginVirusWall(lpIP, arrUser[iUser], arrPwd[iPwd]))
			{
				m_strUser = arrUser[iUser];
				m_strPwd = arrPwd[iPwd];

				CHAR		szINI[MAXPATH]		= {0};
				CHAR		szN[10]				= {0};

				NFile::GetFileInModulePath(szINI, "VirusWall.ini");
				WritePrivateProfileString("INFO", "data1", itoa(iUser, szN, 10), szINI);
				WritePrivateProfileString("INFO", "data2", itoa(iPwd, szN, 10), szINI);
				WritePrivateProfileString("INFO", "data3", m_strIP.GetBuffer(), szINI);
				return TRUE;
			}
		}
	}

	return FALSE;
}

// ���Ե�¼������ǽ
BOOL CVirusWallApp::TestLoginVirusWall(LPSTR lpIP, LPSTR lpUser, LPSTR lpPwd)
{
	BOOL		bRet		= FALSE;

	bRet = NNet::IpcConnect(lpIP, lpUser, lpPwd);
	if(FALSE != bRet)
	{
		NNet::DelConnect(lpIP);
		return TRUE;
	}

	return FALSE;
}

BOOL CVirusWallApp::UploadFile(LPSTR lpIP, LPSTR lpLocalPath, LPSTR lpSvrPath)
{
	BOOL			bRet;

	// �õ��û���������
	bRet = GetVirusWallUserAndPwd(lpIP);
	if(FALSE == bRet)
	{
		AfxMessageBox("��¼����ǽʧ��.", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// �����ļ�
	CHAR			szNetPath[MAXPATH]		= {0};
	_bstr_t			bstrErr;

	bRet = NNet::IpcConnect(lpIP, m_strUser.GetBuffer(), m_strPwd.GetBuffer());
	if(FALSE == bRet)
	{
		AfxMessageBox("��¼����ǽʧ��.", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	do 
	{
		bRet = NNet::IpcCreateRmtPath(szNetPath, lpIP, lpSvrPath);
		if(FALSE == bRet)
		{
			break;
		}
		bRet = CopyFile(lpLocalPath, szNetPath, TRUE);
	} while (false);

	if(FALSE == bRet)
	{
		NSys::GetErrorString(&bstrErr);
		AfxMessageBox((LPSTR)bstrErr, MB_OK | MB_ICONERROR);
	}
	NNet::DelConnect(lpIP);

	return bRet;
}

BOOL CVirusWallApp::UploadDir(LPSTR lpIP, LPSTR lpLocalPath, LPSTR lpSvrPath)
{
	BOOL			bRet;

	// �õ��û���������
	bRet = GetVirusWallUserAndPwd(lpIP);
	if(FALSE == bRet)
	{
		AfxMessageBox("��¼����ǽʧ��.", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// �����ļ�
	CHAR			szNetPath[MAXPATH]		= {0};
	_bstr_t			bstrErr;

	bRet = NNet::IpcConnect(lpIP, m_strUser.GetBuffer(), m_strPwd.GetBuffer());
	if(FALSE == bRet)
	{
		AfxMessageBox("��¼����ǽʧ��.", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	do 
	{
		bRet = NNet::IpcCreateRmtPath(szNetPath, lpIP, lpSvrPath);
		if(FALSE == bRet)
		{
			break;
		}
		bRet = NFile::CopyDirTree(lpLocalPath, szNetPath, FALSE);
	} while (false);

	if(FALSE == bRet)
	{
		NSys::GetErrorString(&bstrErr);
		AfxMessageBox((LPSTR)bstrErr, MB_OK | MB_ICONERROR);
	}
	NNet::DelConnect(lpIP);

	return bRet;
}

DWORD MessageWorkLoop(HANDLE hHandle, DWORD dwMilliseconds)
{
	DWORD			dwRet		= WAIT_TIMEOUT;
	MSG				msg;
	DWORD			dwTick		= GetTickCount();
	DWORD			dwWaitObj	= 0;

	do 
	{
		// �ж��Ƿ�ʱ
		if((GetTickCount() - dwTick) >= dwMilliseconds)
			break;

		// ������Ϣ
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(WM_QUIT == msg.message)
				return 0;
			if(WM_CLOSE == msg.message)
			{
				AfxMessageBox("����ִ�в���������ر�!", MB_OK | MB_ICONERROR);
				continue;
			}

			// MFC�ַ���Ϣ
			if(NULL != AfxGetApp())
			{
				if(AfxGetApp()->PumpMessage())
					continue;
			}

			// ��MFC�ַ���Ϣ
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		// �ȴ�
		dwWaitObj = ::MsgWaitForMultipleObjects(1, &hHandle, FALSE, 
			dwMilliseconds - (GetTickCount() - dwTick),	QS_ALLINPUT);

		// ����Ϣ�ĵ����ʹ�����Ϣ
		if((WAIT_OBJECT_0 + 1) == dwWaitObj)
			continue;

		// �ȴ��������˳�
		dwRet = dwWaitObj;
		break;
	} while (true);

	return dwRet;
}

// ��ȡ�б�
BOOL CVirusWallApp::GetVirusList(LPSTR lpIP, std::vector<std::string>& vList)
{
	BOOL		bRet			= FALSE;

	bRet = GetVirusWallUserAndPwd(lpIP);
	if(FALSE == bRet)
	{
		return FALSE;
	}

	bRet = NNet::IpcConnect(lpIP, m_strUser.GetBuffer(), m_strPwd.GetBuffer());
	if(FALSE == bRet)
		return FALSE;
	do 
	{
		CHAR		szNetPath[MAXPATH]		= {0};

		bRet = NNet::IpcCreateRmtPath(szNetPath, lpIP, m_strVdbPath.GetBuffer());
		if(FALSE == bRet)
			break;
		
		// �����ļ�
		HANDLE		hFind;
		WIN32_FIND_DATA		wfd;

		strcat(szNetPath, "*");
		memset(&wfd, 0, sizeof(wfd));
		hFind = FindFirstFile(szNetPath, &wfd);
		if(INVALID_HANDLE_VALUE == hFind)
		{
			bRet = FALSE;
			break;
		}
		while(FindNextFile(hFind, &wfd))
		{
			if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if('v' == wfd.cFileName[0] || 'V' == wfd.cFileName[0])
					vList.push_back(wfd.cFileName);
			}
		}

		FindClose(hFind);
		bRet = TRUE;
	} while (false);

	NNet::DelConnect(lpIP);
	return bRet;
}

// ��������ǽ����
BOOL CVirusWallApp::RestartVirusWallService(LPSTR lpIP)
{
#if 0
	CSystemInfo		wmi;
	BOOL			bRet				= FALSE;

	bRet = GetVirusWallUserAndPwd(lpIP);
	if(FALSE == bRet)
	{
		return FALSE;
	}

	wmi.m_bChgAuth = TRUE;
	wmi.Initialize(m_strUser.GetBuffer(), m_strPwd.GetBuffer(), lpIP);
	wmi.CreateProces("C: \\Windows\\system32\\cmd.exe /c dir c:\ > c:\\1.txt "); 

	return TRUE;
#endif
#if 1
	CHAR			szBatPath[MAXPATH]	= {0};
	BOOL			bRet				= FALSE;

	bRet = GetVirusWallUserAndPwd(lpIP);
	if(FALSE == bRet)
	{
		return FALSE;
	}
	// ����BAT�ļ�
	if(FALSE == GetBATFile(szBatPath))
	{
		AfxMessageBox("����bat�ļ�ʧ��", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// ����BAT
	CHAR		szNetPath[MAXPATH]		= {0};

	bRet = NNet::IpcConnect(lpIP, m_strUser.GetBuffer(), m_strPwd.GetBuffer());
	if(FALSE == bRet)
		return FALSE;
	NNet::IpcCreateRmtPath(szNetPath, lpIP, "c:\\skynet.bat");
	bRet = CopyFile(szBatPath, szNetPath, FALSE);
	NNet::DelConnect(lpIP);
	if(FALSE == bRet)
	{
		AfxMessageBox("����bat�ļ�ʧ��", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	return bRet;
#endif
	return TRUE;
}

// ����BAT�ļ�
BOOL CVirusWallApp::GetBATFile(LPSTR lpPath)
{
	HANDLE			hFile;
	CHAR			szIni[MAXPATH]		= {0};
	CHAR			szService[500]		= {0};
	CString			strLine;

	NFile::GetFileInModulePath(szIni, "VirusWall.ini");
	NFile::GetFileInModulePath(lpPath, "skynet.bat");
	hFile = CreateFile(lpPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{
		lpPath[0] = 0;
		return FALSE;
	}
	GetPrivateProfileString("VirusWall", "Service", "Virus Chaser", szService, sizeof(szService), szIni);
	WritePrivateProfileString("VirusWall", "Service", szService, szIni);
	strLine.Format("net stop \"%s\"\r\n", szService);
	NFile::WriteFile(hFile, strLine.GetBuffer());
	strLine.Format("net start \"%s\"\r\n", szService);
	NFile::WriteFile(hFile, strLine.GetBuffer());
	NFile::WriteFile(hFile, "del c:\\skynet.bat\r\n");
	CloseHandle(hFile);
	return TRUE;
}