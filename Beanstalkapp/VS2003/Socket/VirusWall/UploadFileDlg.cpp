// UploadFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VirusWall.h"
#include "UploadFileDlg.h"
#include ".\uploadfiledlg.h"
#include "../../common/NSimple.h"
#include "WMIBase.h"


#include <lm.h>
#include <LMat.h>
#pragma comment(lib, "Netapi32.lib")
// CUploadFileDlg dialog

IMPLEMENT_DYNAMIC(CUploadFileDlg, CDialog)
CUploadFileDlg::CUploadFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUploadFileDlg::IDD, pParent)
{
	m_nHeight = 0;
	m_strIP = "";
	m_strSvrPath = "C:\\Inetpub\\wwwroot\\vc\\";
}

CUploadFileDlg::~CUploadFileDlg()
{
}

void CUploadFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG, m_ctlLog);
	DDX_Control(pDX, IDC_LISTVIRUS, m_ctlVirus);
}


BEGIN_MESSAGE_MAP(CUploadFileDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_LOGIN, OnBnClickedLogin)
	ON_BN_CLICKED(IDC_FINDVIRUS, OnBnClickedFindvirus)
	ON_BN_CLICKED(IDC_RESTARTSERVICE, OnBnClickedRestartservice)
END_MESSAGE_MAP()


// CUploadFileDlg message handlers

void CUploadFileDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	HANDLE			hThread;
	DWORD			dwExit			= 0;

	hThread = CreateThread(NULL, 0, _UploadFileThread, this, 0, NULL);
	EnableOptWindow(FALSE);
	GetDlgItem(IDC_LOGIN)->EnableWindow(FALSE);
	MessageWorkLoop(hThread, INFINITE);
	GetExitCodeThread(hThread, &dwExit);
	CloseHandle(hThread);
	EnableOptWindow(TRUE);
	GetDlgItem(IDC_LOGIN)->EnableWindow(TRUE);
}

BOOL CUploadFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	EnableOptWindow(FALSE);


	SetDlgItemText(IDC_IP, m_strIP);
	if("" != m_strIP)
	{
		OnBnClickedLogin();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// ���ò�����ť
void CUploadFileDlg::EnableOptWindow(BOOL bEnble)
{
	GetDlgItem(IDC_FINDVIRUS)->EnableWindow(bEnble);
	GetDlgItem(IDC_LISTVIRUS)->EnableWindow(bEnble);
	GetDlgItem(IDOK)->EnableWindow(bEnble);
	GetDlgItem(IDC_RESTARTSERVICE)->EnableWindow(bEnble);

	if(FALSE == bEnble)
		SetDlgItemText(IDC_LOGIN, "���ӷ���ǽ");
}

// �����־
void CUploadFileDlg::AddLog(LPSTR lpLog, BOOL bMsg /* = FALSE */)
{
	CString				str(lpLog);

	str.Replace(_T("\r"), _T(""));	//ȥ��ԭ�еĻس�����
	str.Replace(_T("\n"), _T(""));
	str += _T("\r\n");				//��ӻس�����
	CTime time = CTime::GetCurrentTime();
	str = time.Format(_T("[%H:%M:%S]  ")) + str;

	m_ctlLog.SetSel(-2, -1);
	m_ctlLog.ReplaceSel(str.GetBuffer());
	
	// ������ʾ
	if(FALSE == bMsg)
		return;
	if(!IsWindow(m_hWnd))
	{
		AfxMessageBox(lpLog, MB_OK | MB_ICONERROR);
	}
	else
	{
		MessageBox(lpLog, "�ϴ�������", MB_OK | MB_ICONERROR);
	}
}

// ��Ӵ�����־
void CUploadFileDlg::AddErrLog(LPSTR lpPreLog, BOOL bMsg)
{
	CString			str;
	_bstr_t			bstrErr;

	NSys::GetErrorString(&bstrErr);
	str.Format("%s: %s", lpPreLog, (LPSTR)bstrErr);
	AddLog(str.GetBuffer(), bMsg);
}

DWORD CUploadFileDlg::_UploadFileThread(LPVOID lpParameter)
{
	CUploadFileDlg*		pThis		= (CUploadFileDlg *)lpParameter;

	return pThis->UploadFile();
}

// �ϴ��ļ�
BOOL CUploadFileDlg::UploadFile()
{
	CString			strLocal;
	CString			strSvr;
	CHAR			sz7za[MAXPATH]		= {0};
	CHAR			szTmpPath[MAXPATH]	= {0};
	
	// �ҵ�7za
	NFile::GetFileInModulePath(sz7za, "7za.exe");
	if(!PathFileExists(sz7za))
	{
		AddLog("�Ҳ���7za�ļ�!", TRUE);
		return FALSE;
	}
	// ������ʱĿ¼
	NFile::GetModuleDir(szTmpPath);
	strcat(szTmpPath, "\\Temp");
	NFile::DeleteDirTree(szTmpPath);
	if(FALSE == CreateDirectory(szTmpPath, NULL))
	{
		AddLog("������ʱĿ¼ʧ��!", TRUE);
		return FALSE;
	}
	// ��ѹ������
	CString					strCmd;
	STARTUPINFO				si				= {0};
	PROCESS_INFORMATION		pi				= {0};
	BOOL					bRet			= 0;
	DWORD					dwCode			= 0;

	GetDlgItemText(IDC_LOCALPATH, strLocal);
	if(!PathFileExists(strLocal))
	{
		AfxMessageBox("����Ĳ�����·��!", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	AddLog("��ʼ��ѹ������");
	strCmd.Format("\"%s\" x -y -o\"%s\" \"%s\"", sz7za, szTmpPath, strLocal);
	bRet = NSys::Execute(NULL, strCmd.GetBuffer(), TRUE, TRUE, &dwCode);//CreateProcess(NULL, strCmd.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(FALSE == bRet || 0 != dwCode)
	{
		AddErrLog("������ѹ����ʧ��");
		return FALSE;
	}
	
	// ��֤�������ļ�
	CHAR			szVDB[MAXPATH]		= {0};
	CHAR			szVDBFind[MAXPATH]	= {0};
	CHAR			szVersion[50]		= {0};

	AddLog("���ڷ���������");
	strcpy(szVDBFind, szTmpPath);
	strcat(szVDBFind, "\\V*");
	if(FALSE == NFile::GetFirstFile(szVDBFind, szVDB))
	{
		AddLog("������Ч�Ĳ������ļ�.", TRUE);
		return FALSE;
	}
	strcpy(szVDBFind, szVDB);
	strcat(szVDBFind, "\\filesys.htm");
	if(!PathFileExists(szVDBFind))
	{
		AddLog("������Ч�Ĳ������ļ�.", TRUE);
		return FALSE;
	}
	NFile::SpliteFilePath(szVDB, NULL, szVersion);
	strcpy(szVDBFind, m_strSvrPath.GetBuffer());
	strcat(szVDBFind, szVersion);

	// �ϴ�
	AddLog("��ʼ�ϴ�");
	bRet = theApp.UploadDir(m_strIP.GetBuffer(), szVDB, szVDBFind);
	if(FALSE != bRet)
	{
		AddLog("�ϴ��ɹ�");
	}
	else
	{
		AddLog("�ϴ����, �в����ļ�û���ϴ��ɹ�.");
	}
	
	// ɾ����ʱĿ¼
	NFile::DeleteDirTree(szTmpPath);

	InitVirusList();

	// ��������
	OnBnClickedRestartservice();
	AfxMessageBox("�ϴ�������ɹ�\r\n\r\n1����֮�󽫻��Զ���������ǽ����.", MB_OK);
	return bRet;
}



void CUploadFileDlg::OnBnClickedLogin()
{
	CString		strBtn;

	GetDlgItemText(IDC_LOGIN, strBtn);
	if("���ӷ���ǽ" == strBtn)
	{
		GetDlgItemText(IDC_IP, m_strIP);
		if("" == m_strIP)
		{
			AfxMessageBox("���������ǽIP", MB_OK |MB_ICONINFORMATION);
			return;
		}
		if(FALSE != theApp.GetVirusWallUserAndPwd(m_strIP.GetBuffer()))
		{
			AddLog(CString("�Ѿ����ӵ�: " + m_strIP).GetBuffer());
			EnableOptWindow(TRUE);
			SetDlgItemText(IDC_LOGIN, "��������");
			InitVirusList();
		}
		else
		{
			AddLog(CString("�޷����ӵ�: " + m_strIP).GetBuffer(), TRUE);
		}
	}
	else
	{
		EnableOptWindow(FALSE);
		SetDlgItemText(IDC_LOGIN, "���ӷ���ǽ");
		SendDlgItemMessage(IDC_IP, EM_SETSEL, 0, -1);
	}
}

void CUploadFileDlg::OnBnClickedFindvirus()
{
	CFileDialog			fDlg(TRUE, "*.zip", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "����ǽ�������ļ� (*.zip)|*.zip||", this);

	if(IDOK == fDlg.DoModal())
	{
		SetDlgItemText(IDC_LOCALPATH, fDlg.GetPathName());
	}
}

// ��ʼ���б�
void CUploadFileDlg::InitVirusList()
{
	std::vector<std::string>		vList;
	BOOL							bRet			= FALSE;

	m_ctlVirus.ResetContent();
	bRet = theApp.GetVirusList(m_strIP.GetBuffer(), vList);
	if(FALSE == bRet)
	{
		AddLog("��ȡ����ǽ�������б�ʧ��");
		EnableOptWindow(FALSE);
	}

	for(int i = 0; i < vList.size(); i++)
	{
		m_ctlVirus.AddString(vList[i].c_str());
	}
}

void CUploadFileDlg::OnBnClickedRestartservice()
{
// 	CSystemInfo		wmi;
// 	pServer = "192.168.1.201";
// 	pSorDir = "C: \\Inetpub\\wwwroot\\vc\\V32615";
// 
// 
// 	wmi.Initialize("Yofoo", "zzz", pServer);
// 	wmi.CreateProces("C: \\Windows\\system32\\cmd.exe /c dir c:\ > c:\\1.txt ");
// 	wmi.GetService("SkynetMgr", &pSvrClassObj);
// 
// 	pSvrClassObj->Release();
// 	return;
// 	wmi.CallMethod(pSvrClassObj, "StopService", NULL, vtRet.GetAddress());
	CHAR			szBatPath[MAXPATH]	= {0};
	BOOL			bRet				= FALSE;

	bRet = theApp.GetVirusWallUserAndPwd(m_strIP.GetBuffer());
	if(FALSE == bRet)
	{
		AddLog("�޷���֤�û�", TRUE);
		return ;
	}
	// ����BAT�ļ�
	if(FALSE == theApp.GetBATFile(szBatPath))
	{
		AddErrLog("��ʼ������ʧ��", TRUE);
		return ;
	}

	// ����BAT
	CHAR		szNetPath[MAXPATH]		= {0};

	bRet = NNet::IpcConnect(m_strIP.GetBuffer(), theApp.m_strUser.GetBuffer(), theApp.m_strPwd.GetBuffer());
	if(FALSE == bRet)
	{
		AddErrLog("���ӷ���ǽ����", TRUE);
		return ;
	}
	NNet::IpcCreateRmtPath(szNetPath, m_strIP.GetBuffer(), "c:\\skynet.bat");
	bRet = CopyFile(szBatPath, szNetPath, FALSE);
	NNet::DelConnect(m_strIP.GetBuffer());
	DeleteFile(szBatPath);
	if(FALSE == bRet)
	{
		AddErrLog("����ǽ�ܾ�����", TRUE);
		return ;
	}
	
	// ����IPC$����
	CHAR		szIPCPath[100]		= {0};
	
	strcpy(szIPCPath, m_strIP.GetBuffer());
	strcat(szIPCPath, "\\IPC$");
	bRet = NNet::IpcConnect(szIPCPath, theApp.m_strUser.GetBuffer(), theApp.m_strPwd.GetBuffer());
	if(FALSE == bRet)
	{
		AddErrLog("���ӷ���ǽ����", TRUE);
		return ;
	}
	// IPC$����
	SendAtCommand(m_strIP.GetBuffer());

	NNet::DelConnect(szIPCPath);
	return ;
}

// ����BAT�ļ�
void CUploadFileDlg::SendAtCommand(LPSTR lpIP)
{
	LPTIME_OF_DAY_INFO		pBuf			= NULL;
	NET_API_STATUS			nStatus;
	CHAR					szCmd[2048]		= {0};
	CStringW				wstrIP;
	tm						svrTm;
	time_t					locTm			= time(NULL);
	AT_INFO					atInfo			= {0};
	DWORD					jobTime;
	int						nMin			= 1;

	atInfo.Command = L"c:\\skynet.bat";
	atInfo.Flags = JOB_ADD_CURRENT_DATE;
	atInfo.DaysOfMonth = 0;
	atInfo.DaysOfWeek = 0;
	// ��ȡʱ��
	wstrIP.Format(L"\\\\%S", lpIP);
	nStatus = NetRemoteTOD(wstrIP.GetBuffer(), (LPBYTE *)&pBuf);
	if(NERR_Success != nStatus || 0 == pBuf)
	{
		nMin = 5;
		memcpy(&svrTm, localtime(&locTm), sizeof(tm));
	}
	else
	{
		memcpy(&svrTm, localtime((time_t *)&pBuf->tod_elapsedt), sizeof(tm));
		NetApiBufferFree(pBuf);
	}
	// ��������

	atInfo.JobTime = (svrTm.tm_hour*3600 + svrTm.tm_min*60 + svrTm.tm_sec + 60 * nMin) * 1000;
	nStatus = NetScheduleJobAdd(wstrIP.GetBuffer(), (LPBYTE)&atInfo, (LPDWORD)&jobTime);
	if(NERR_Success != nStatus)
	{
		AddErrLog("��������ǽ����ʧ��", TRUE);
	}
	else
	{
		AddLog("��������ǽ����ɹ�");
	}
	return;
}
