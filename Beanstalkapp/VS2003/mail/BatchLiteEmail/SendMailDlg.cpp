// SendMailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BatchLiteEmail.h"
#include "SendMailDlg.h"
#include ".\sendmaildlg.h"
#include "LinkMainDlg.h"
#include "../SendMail/SendOneMail.h"
#include <sys/stat.h>

#import "msxml3.dll"
// CSendMailDlg dialog

IMPLEMENT_DYNAMIC(CSendMailDlg, CDialog)
CSendMailDlg::CSendMailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendMailDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strEmail(_T(""))
	, m_strSvr(_T(""))
	, m_strUser(_T(""))
	, m_strPwd(_T(""))
	, m_strTitle(_T(""))
{
	m_hSendThread = NULL;
	m_bCancel = FALSE;
}

CSendMailDlg::~CSendMailDlg()
{
}

void CSendMailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LT_ATTACH, m_ltAttachment);
	DDX_Control(pDX, IDC_LT_LOG, m_ltLog);
	DDX_Text(pDX, IDC_ET_NAME, m_strName);
	DDX_Text(pDX, IDC_ET_EMAIL, m_strEmail);
	DDX_Text(pDX, IDC_ET_SERVER, m_strSvr);
	DDX_Text(pDX, IDC_ET_USER, m_strUser);
	DDX_Text(pDX, IDC_ET_PWD, m_strPwd);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctlPos);
	DDX_Text(pDX, IDC_ET_TITLE, m_strTitle);
}


BEGIN_MESSAGE_MAP(CSendMailDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_LINKMANAGER, OnBnClickedLinkmanager)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BT_ADD, OnBnClickedBtAdd)
	ON_BN_CLICKED(IDC_BT_DELETE, OnBnClickedBtDelete)
	ON_BN_CLICKED(IDC_BT_SEND, OnBnClickedBtSend)
	ON_BN_CLICKED(IDC_BT_HIDE, OnBnClickedBtHide)
	ON_MESSAGE(UM_TRAYNOTIFY, OnTrayNotify)
	ON_BN_CLICKED(IDC_BT_TEST, OnBnClickedBtTest)
END_MESSAGE_MAP()


// CSendMailDlg message handlers

void CSendMailDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CSendMailDlg::OnBnClickedCancel()
{
	m_bCancel = TRUE;
}



void CSendMailDlg::OnBnClickedLinkmanager()
{
	CLinkMainDlg		dlg;

	dlg.DoModal();
}

void CSendMailDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(WaitForSingleObject(m_hSendThread, 0) == WAIT_TIMEOUT)
	{
		return;
	}

	OnCancel();
}

void CSendMailDlg::OnBnClickedBtAdd()
{
	CFileDialog			fd(TRUE, "*.*", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "所有文件 (*.*)|*.*||", this);

	if(IDOK == fd.DoModal())
	{
		m_ltAttachment.AddString(fd.GetPathName());
	}
}

void CSendMailDlg::OnBnClickedBtDelete()
{
	for(int i = m_ltAttachment.GetCount() - 1; i >= 0; i--)
	{ 
		if(m_ltAttachment.GetSel(i) <= 0)
			continue;
		m_ltAttachment.DeleteString(i);
	}
}

void CSendMailDlg::OnBnClickedBtSend()
{
	if(WAIT_TIMEOUT == WaitForSingleObject(m_hSendThread, 0))
	{
		return;
	}
	UpdateData();
	if(m_strTitle.GetLength() < 1)
	{
		MessageBox("请输入邮件标题");
		return;
	}
	if(m_strName.GetLength() < 1 || m_strEmail.GetLength() < 1 || m_strSvr.GetLength() < 1
		|| m_strUser.GetLength() < 1 || m_strPwd.GetLength() < 1)
	{
		MessageBox("请输入完整的发件人信息");
		return;
	}

	SetHistory("Name", m_strName);
	SetHistory("Email", m_strEmail);
	SetHistory("Server", m_strSvr);
	SetHistory("User", m_strUser);
	SetHistory("Password", m_strPwd);
	SetHistory("MailTitle", m_strTitle);

	m_hSendThread = CreateThread(NULL, NULL, _ThreadProc, this, 0, NULL);
}

BOOL CSendMailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strName = GetHistory("Name");
	m_strEmail = GetHistory("Email");
	m_strSvr = GetHistory("Server");
	m_strUser = GetHistory("User");
	m_strPwd = GetHistory("Password");
	m_strTitle = GetHistory("MailTitle");

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSendMailDlg::EnableWorkItem(BOOL bWork)
{
	GetDlgItem(IDC_ET_NAME)->EnableWindow(bWork);
	GetDlgItem(IDC_ET_EMAIL)->EnableWindow(bWork);
	GetDlgItem(IDC_ET_SERVER)->EnableWindow(bWork);
	GetDlgItem(IDC_ET_USER)->EnableWindow(bWork);
	GetDlgItem(IDC_ET_TITLE)->EnableWindow(bWork);
	GetDlgItem(IDC_ET_PWD)->EnableWindow(bWork);
	GetDlgItem(IDC_BT_SEND)->EnableWindow(bWork);
	GetDlgItem(IDC_BT_TEST)->EnableWindow(bWork);
	GetDlgItem(IDC_BT_ADD)->EnableWindow(bWork);
	GetDlgItem(IDC_BT_DELETE)->EnableWindow(bWork);
	GetDlgItem(IDC_LINKMANAGER)->EnableWindow(bWork);
	GetDlgItem(IDCANCEL)->EnableWindow(!bWork);
	GetDlgItem(IDC_BT_HIDE)->EnableWindow(!bWork);
}

void CSendMailDlg::DoSendMail()
{
	EnableWorkItem(FALSE);

	FILE*		pFile		= fopen(m_strTemplateUrl, "rb+");

	if(NULL == pFile)
	{
		MessageBox("打开文件" + m_strTemplateUrl + "失败");
		return;
	}
	struct stat		st		= {0};

	fstat(pFile->_file, &st);
	BYTE*	pData		= new BYTE[st.st_size+1];

	memset(pData, 0, st.st_size+1);
	fread(pData, 1, st.st_size, pFile);
	fclose(pFile);

	try
	{
		CString							strPath;
		MSXML2::IXMLDOMDocument2Ptr		spDoc;

		spDoc.CreateInstance("Msxml2.DOMDocument");
		spDoc->put_async(VARIANT_FALSE);
		GetModuleFileName(NULL, strPath.GetBufferSetLength(1024), 1024);
		strPath.ReleaseBuffer();
		strPath.ReleaseBuffer(strPath.ReverseFind(_T('\\')) + 1);
		strPath += "BatchLiteEmail_Data.xml";
		spDoc->load(strPath.GetBuffer());

		MSXML2::IXMLDOMNodeListPtr  spList		= spDoc->selectNodes("/peoples/people");
		LONG						length		= spList->Getlength();

		m_ctlPos.SetRange(0, length);
		for(long i = 0; i < length && !m_bCancel; i++)
		{
			if(0 != i)
				Sleep(10 * 1000);	// 暂停

			MSXML2::IXMLDOMNodePtr		spNode	= spList->Getitem(i);
			int							nIndex;
			_bstr_t						sName, sPhone, sEmail, sCompany;

			spNode->selectSingleNode("name")->get_text(sName.GetAddress());
			spNode->selectSingleNode("email")->get_text(sEmail.GetAddress());
			spNode->selectSingleNode("phone")->get_text(sPhone.GetAddress());
			spNode->selectSingleNode("company")->get_text(sCompany.GetAddress());
		
			// 发送邮件
			CSendOneMail		mail;

			mail.SetDirectory(m_strTemplateUrl);
			mail.SetMailServer(m_strSvr, m_strUser, m_strPwd);
			mail.SetMailFrom(m_strEmail, m_strName);
			mail.SetMailTo(sEmail, sName, m_strTitle);
			mail.SetTemplate(pData, st.st_size+1);
			mail.SetVariant("Name", sName);

			if(mail.Send())
			{
				m_ltLog.AddString("发送邮件 " + sName + ":" + sEmail + "成功!");
			}
			else
			{
				m_ltLog.AddString("发送邮件 " + sName + ":" + sEmail + "失败!");
			}
			m_ctlPos.SetPos(i+1);
		}
	}
	catch(_com_error& e)
	{
		m_ltLog.AddString(e.Description());
		MessageBox(e.Description(), NULL, MB_OK|MB_ICONERROR);
	}

	delete [] pData;
	EnableWorkItem(TRUE);

	// 退出托盘
	//ShowDlg();
	PostMessage(UM_TRAYNOTIFY, 0, WM_LBUTTONDBLCLK);
}

void CSendMailDlg::ShowDlg()
{
	if(IsWindowVisible())
	{
		return;
	}

	NOTIFYICONDATA		notify		= {0};

	notify.cbSize = sizeof(notify);
	notify.uID = 1;
	notify.hWnd = GetSafeHwnd();

	Shell_NotifyIcon(NIM_DELETE, &notify);
	// 显示窗体
	ShowWindow(SW_SHOW);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
}

DWORD CALLBACK CSendMailDlg::_ThreadProc(LPVOID lpParameter)
{
	CSendMailDlg*	pThis		= (CSendMailDlg *)lpParameter;

	CoInitialize(NULL);
	pThis->m_bCancel = FALSE;
	pThis->DoSendMail();
	return 0;
}

void CSendMailDlg::OnBnClickedBtHide()
{
	AfxGetMainWnd()->ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);

	// 显示图标
	NOTIFYICONDATA		notify		= {0};

	notify.cbSize = sizeof(notify);
	notify.uID = 1;
	notify.hWnd = GetSafeHwnd();


	// 确定消息
	notify.uCallbackMessage = UM_TRAYNOTIFY;
	notify.uFlags |= NIF_MESSAGE;
	// 确定图标
	notify.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	notify.uFlags |= NIF_ICON;
	// 确定提示文本
	notify.uFlags |= NIF_TIP;
	lstrcpyn(notify.szTip, "正在批量发送邮件", 128);

	Shell_NotifyIcon(NIM_ADD, &notify);
}

LRESULT CSendMailDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case WM_LBUTTONDBLCLK:
		ShowDlg();
		break;
	}

	return 0;
}

void CSendMailDlg::OnBnClickedBtTest()
{
	if(WAIT_TIMEOUT == WaitForSingleObject(m_hSendThread, 0))
	{
		return;
	}
	UpdateData();
	if(m_strTitle.GetLength() < 1)
	{
		MessageBox("请输入邮件标题");
		return;
	}
	if(m_strName.GetLength() < 1 || m_strEmail.GetLength() < 1 || m_strSvr.GetLength() < 1
		|| m_strUser.GetLength() < 1 || m_strPwd.GetLength() < 1)
	{
		MessageBox("请输入完整的发件人信息");
		return;
	}

	SetHistory("Name", m_strName);
	SetHistory("Email", m_strEmail);
	SetHistory("Server", m_strSvr);
	SetHistory("User", m_strUser);
	SetHistory("Password", m_strPwd);
	SetHistory("MailTitle", m_strTitle);

	// 发送邮件
	FILE*		pFile		= fopen(m_strTemplateUrl, "rb+");

	if(NULL == pFile)
	{
		MessageBox("打开文件" + m_strTemplateUrl + "失败");
		return;
	}
	struct stat		st		= {0};

	fstat(pFile->_file, &st);
	BYTE*	pData		= new BYTE[st.st_size+1];

	memset(pData, 0, st.st_size+1);
	fread(pData, 1, st.st_size, pFile);
	fclose(pFile);


	// 发送邮件
	CSendOneMail		mail;

	mail.SetDirectory(m_strTemplateUrl);
	mail.SetMailServer(m_strSvr, m_strUser, m_strPwd);
	mail.SetMailFrom(m_strEmail, m_strName);
	mail.SetMailTo(m_strEmail, m_strName, m_strTitle);
	mail.SetTemplate(pData, st.st_size+1);
	mail.SetVariant("Name", m_strName);

	if(mail.Send())
	{
		MessageBox("发送邮件 " + m_strName + ":" + m_strEmail + "成功!", NULL, MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox("发送邮件 " + m_strName + ":" + m_strEmail + "失败!", NULL, MB_OK|MB_ICONERROR);
	}

	delete [] pData;
}
