// SendMailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SendMail.h"
#include "SendMailDlg.h"
#include ".\sendmaildlg.h"
#include "../../../ExtClass/Net/XSendMail.h"
#include "../../../ExtClass/XCommon.h"
#include "../../../ExtClass/Net/XSocksProxy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSendMailDlg 对话框



CSendMailDlg::CSendMailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendMailDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSendMailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSendMailDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
// 	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CSendMailDlg 消息处理程序

BOOL CSendMailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CSendMailDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSendMailDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CSendMailDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSendMailDlg::OnBnClickedButton2()
{
	// 发送邮件测试
	CXSendMail			mail;
	vector<CString>		vctMailSvr;

	mail.m_strSendTo = "xiaxj@snswall.com";
	mail.m_strMailFrom = "yuanjie.xia@yahoo.cn";
	mail.m_strFromName = "夏祥均";
	mail.m_strSubject = "这是一个测试";
	mail.m_strData = "这是我的一个测试邮件.";
// 	m_strData.GetSMTPServer("yahoo.com.cn", vctMailSvr);
	if(FALSE == mail.Send())
	{
		AfxMessageBox("Send failed!", MB_OK | MB_ICONERROR);
	}
	else
	{
		AfxMessageBox("Send Successful!");
	}
	int i = 0;
}

void CSendMailDlg::OnBnClickedButton1()
{
	// 发送邮件
	CXSendMail			mail;
	HANDLE				hThread;
	DWORD				dwCode		= 0;
	CString				strdate;

	GetDlgItemText(IDC_TITLE, mail.m_strSubject);
	GetDlgItemText(IDC_SENDTO, mail.m_strSendTo);
	GetDlgItemText(IDC_SENDSVR, mail.m_strSendSvr);
	GetDlgItemText(IDC_FROMNAME, mail.m_strFromName);
	GetDlgItemText(IDC_FROMMAIL, mail.m_strMailFrom);
	GetDlgItemText(IDC_DATA, mail.m_strData);
	GetDlgItemText(IDC_PROXYSVR, mail.m_strProxyIP);
	mail.m_nProxyPort = GetDlgItemInt(IDC_PROXY_PORT);
	mail.m_bUseProxy = ((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck();

	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	hThread = CreateThread(NULL, 0, SendMailThread, (void *)&mail, 0, NULL);
	if(NULL != hThread)
	{
		ext::WaitObjectWithMsgLoop(hThread, INFINITE);
		GetExitCodeThread(hThread, &dwCode);
		if(0 == dwCode)
		{
			AfxMessageBox("Send failed!", MB_OK | MB_ICONERROR);
		}
		else
		{
			AfxMessageBox("Send Successful!");
		}
		CloseHandle(hThread);
	}
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
}

/*
 *	发送线程
 */
#include "../../../ExtClass/MAPI32.h"
#import "jmail.dll"

DWORD CSendMailDlg::SendMailThread(LPVOID lpParamter)
{
// 	CXSendMail*			pMail = (CXSendMail *)lpParamter;
// 	CXSocksProxy		proxy;
// 	LHANDLE				hSession		= NULL;
// 	ULONG				ulRet			= 0;
// 
// 	ulRet = mapi32::MAPILogon(NULL, "xiaxj@txtws.com", "xxj!@#06", MAPI_LOGON_UI, NULL, &hSession);
// 	if(SUCCESS_SUCCESS != ulRet)  
// 		return 0;
// 	MapiMessage			msg				= {0};
// 	MapiRecipDesc		recip;
// 
// 	recip.lpszAddress = "yuanjie.xia@yahoo.com.cn";
// 	recip.ulRecipClass = MAPI_TO;
// 
// 	msg.nFileCount = 0;
// 	msg.lpFiles = NULL;
// 	msg.nRecipCount = 1;
// 	msg.lpRecips = &recip;
// 	msg.lpszSubject = "hello mapi mail";
// 	msg.lpszNoteText = "this is mapi mail body";
// 
// 	ulRet = mapi32::MAPISendMail(hSession, NULL, &msg
// 		, MAPI_LOGON_UI|MAPI_DIALOG
// 		, 0);
// 
// 	ulRet = mapi32::MAPILogoff(hSession, NULL, 0, 0);
	CoInitialize(NULL);

	jmail::IMessagePtr		pMessage("JMail.Message");
	try
	{

		// 添加发件人
		pMessage->From = "xiaxj@txtws.com";
		// 发件人名字
		pMessage->FromName = "夏祥均";
		// 添加收件人 (addr, name, pgpkey)
		pMessage->AddRecipient("yuanjie.xia@yahoo.com.cn", "夏先生", "");
		// 优先级
		pMessage->Priority = 3;
		// 编码
		pMessage->Charset = "GB2312";
		// 主题
		pMessage->Subject = "JMAIL test";
		// 正文
		//pMessage->Body = "这里是邮件正文\r\n";

		// 符加正文
		//pMessage->AppendText("this is append text");

		// 附件
		_bstr_t  strID = pMessage->AddAttachment("m:\\baidu.mht", VARIANT_TRUE, "application/octet-stream");


		//  发送验证
		//pMessage->Send("mail.txtws.com", VARIANT_FALSE);
		pMessage->MailServerUserName = "xiaxj@txtws.com";
		pMessage->MailServerPassWord = "xxj!@#06";
		pMessage->Send("mail.txtws.com", VARIANT_FALSE);
	
		pMessage.Release();
		CoUninitialize();
		return 1;
	}
	catch( _com_error & E)
	{
		TRACE("Error 0x%08X: %s\n", E.Error(), (LPCSTR)E.Description());
	}

	pMessage.Release();
	CoUninitialize();
	return 0;
}
DWORD SendMailThread(LPVOID lpParamter)
{
	CXSendMail*			pMail = (CXSendMail *)lpParamter;
	CXSocksProxy		proxy;

	if(FALSE != pMail->m_bUseProxy)
	{
		vector<CString>		vctIP;
		CString				strSendTo;
		int					i = -1;

		if(pMail->m_strSendSvr.GetLength() > 0)
			strSendTo = pMail->m_strSendSvr;
		else
			strSendTo = pMail->m_strSendTo;

		if(FALSE == pMail->GetSMTPIP(strSendTo, vctIP))
		{
			TRACE("GetSMTPIP failed.\n");
			return 0;
		}
		for(i = 0; i < (int)vctIP.size(); i++)
		{
			if(FALSE == proxy.Connect(pMail->m_strProxyIP.GetBuffer(), pMail->m_nProxyPort))
			{
				TRACE("#Error connect %s\n", pMail->m_strProxyIP.GetBuffer());
				continue;
			}
			if(FALSE != proxy.ProxySocks5(vctIP[i].GetBuffer(), 25))
			{
				break;
			}

			TRACE("Proxy: can not connect %s\n", vctIP[i].GetBuffer());
		}
		if(i >= (int)vctIP.size())
		{
			TRACE("Proxy: %d ip can not use connect. ", vctIP.size());
			return 0;
		}

		TRACE("SendMailThread: begin send with proxy mail.\n");
		return pMail->Send(proxy.m_hSocket);
	}

	TRACE("SendMailThread: begin send mail.\n");
	return pMail->Send();
}
void CSendMailDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(FALSE == GetDlgItem(IDC_BUTTON1)->IsWindowEnabled())
	{
		AfxMessageBox("正在发送邮件");	
		return;
	}

	CDialog::OnCancel();
}

void CSendMailDlg::OnBnClickedButton3()
{
	// 代理测试
// 	CXSocksProxy		proxy;
// 	SOCKET				sockProxy;
// 
//	proxy->ConnectSocks4()
// 	proxy.Connect("202.102.72.38", 1080);
// 
// 	proxy.ConnectSocks5(NULL, NULL, "218.106.248.58", 25);

	CXSendMail			mail;
	vector<CString>		vctIP;

	mail.GetSMTPIP("163.com", vctIP);
	
	int i = 0;
}
