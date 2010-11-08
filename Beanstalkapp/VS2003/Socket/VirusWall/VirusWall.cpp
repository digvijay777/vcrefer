// VirusWall.cpp : 定义应用程序的类行为。
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


// CVirusWallApp 构造

CVirusWallApp::CVirusWallApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
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


// 唯一的一个 CVirusWallApp 对象

CVirusWallApp theApp;


// CVirusWallApp 初始化

BOOL CVirusWallApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

//	CVirusWallDlg dlg;
	CUploadFileDlg dlg;

	dlg.m_strIP = m_strIP;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用“确定”来关闭
		//对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	// 而不是启动应用程序的消息泵。
	return FALSE;
}

// 得到防毒墙用户名或密码
BOOL CVirusWallApp::GetVirusWallUserAndPwd(LPSTR lpIP)
{
	// 初始化信息
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

	// 尝度登录
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

// 尝试登录到防毒墙
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

	// 得到用户名和密码
	bRet = GetVirusWallUserAndPwd(lpIP);
	if(FALSE == bRet)
	{
		AfxMessageBox("登录防毒墙失败.", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// 传送文件
	CHAR			szNetPath[MAXPATH]		= {0};
	_bstr_t			bstrErr;

	bRet = NNet::IpcConnect(lpIP, m_strUser.GetBuffer(), m_strPwd.GetBuffer());
	if(FALSE == bRet)
	{
		AfxMessageBox("登录防毒墙失败.", MB_OK | MB_ICONERROR);
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

	// 得到用户名和密码
	bRet = GetVirusWallUserAndPwd(lpIP);
	if(FALSE == bRet)
	{
		AfxMessageBox("登录防毒墙失败.", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// 传送文件
	CHAR			szNetPath[MAXPATH]		= {0};
	_bstr_t			bstrErr;

	bRet = NNet::IpcConnect(lpIP, m_strUser.GetBuffer(), m_strPwd.GetBuffer());
	if(FALSE == bRet)
	{
		AfxMessageBox("登录防毒墙失败.", MB_OK | MB_ICONERROR);
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
		// 判断是否超时
		if((GetTickCount() - dwTick) >= dwMilliseconds)
			break;

		// 处理消息
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(WM_QUIT == msg.message)
				return 0;
			if(WM_CLOSE == msg.message)
			{
				AfxMessageBox("正在执行操作不允许关闭!", MB_OK | MB_ICONERROR);
				continue;
			}

			// MFC分发消息
			if(NULL != AfxGetApp())
			{
				if(AfxGetApp()->PumpMessage())
					continue;
			}

			// 非MFC分发消息
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		// 等待
		dwWaitObj = ::MsgWaitForMultipleObjects(1, &hHandle, FALSE, 
			dwMilliseconds - (GetTickCount() - dwTick),	QS_ALLINPUT);

		// 是消息的到来就处理消息
		if((WAIT_OBJECT_0 + 1) == dwWaitObj)
			continue;

		// 等待结束，退出
		dwRet = dwWaitObj;
		break;
	} while (true);

	return dwRet;
}

// 获取列表
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
		
		// 查找文件
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

// 重启防毒墙服务
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
	// 创建BAT文件
	if(FALSE == GetBATFile(szBatPath))
	{
		AfxMessageBox("创建bat文件失败", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// 复制BAT
	CHAR		szNetPath[MAXPATH]		= {0};

	bRet = NNet::IpcConnect(lpIP, m_strUser.GetBuffer(), m_strPwd.GetBuffer());
	if(FALSE == bRet)
		return FALSE;
	NNet::IpcCreateRmtPath(szNetPath, lpIP, "c:\\skynet.bat");
	bRet = CopyFile(szBatPath, szNetPath, FALSE);
	NNet::DelConnect(lpIP);
	if(FALSE == bRet)
	{
		AfxMessageBox("复制bat文件失败", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	return bRet;
#endif
	return TRUE;
}

// 生成BAT文件
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