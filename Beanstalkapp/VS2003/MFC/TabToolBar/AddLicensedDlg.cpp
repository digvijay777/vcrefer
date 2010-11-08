// AddLicensedDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LicenseManager.h"
#include "AddLicensedDlg.h"
#include ".\addlicenseddlg.h"
#include "License2Doc.h"
#include "LicenseManagerDoc.h"

//#include "../License/LicenseData.h"
// CAddLicensedDlg dialog
//static CLicenseData*	s_pLicense = NULL;

IMPLEMENT_DYNCREATE(CAddLicensedDlg, CDialog)

CAddLicensedDlg::CAddLicensedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddLicensedDlg::IDD, pParent)
	, m_szID(_T(""))
	, m_szUser(_T(""))
	, m_szTel(_T(""))
	, m_szClientCount(_T("100"))
	, m_szMatureDate(_T(""))
	, m_szAuthor(_T(""))
	, m_nType(1)
{
	m_hInstLicense = NULL;
}

CAddLicensedDlg::~CAddLicensedDlg()
{
	if(NULL != m_hInstLicense)
	{
		typedef BOOL (*FUN)();
		FUN DeleteLicenseData = (FUN)GetProcAddress(m_hInstLicense, "DeleteLicenseData");
		if(DeleteLicenseData)
			DeleteLicenseData();
		::FreeLibrary(m_hInstLicense);
	}
}

void CAddLicensedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ST_ID, m_szID);
	DDX_Text(pDX, IDC_ET_USER, m_szUser);
	DDX_Text(pDX, IDC_ET_TEL, m_szTel);
	DDX_Text(pDX, IDC_ET_COUNT, m_szClientCount);
	DDX_Text(pDX, IDC_ET_AUTHOR, m_szAuthor);
	DDX_Control(pDX, IDC_CB_GROUP, m_cbGroup);
}

BOOL CAddLicensedDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化License库
// 	TCHAR	szLibPath[MAX_PATH]		= {0};
// 	strcpy(szLibPath, theApp.GetAppPath());
// 	strcat(szLibPath, "License.dll");
// 	m_hInstLicense = ::LoadLibrary(szLibPath);
// 	if(NULL != m_hInstLicense)
// 	{
// 		typedef BOOL (*FUN)(CLicenseData**);
// 		FUN CreateLicenseData = (FUN)GetProcAddress(m_hInstLicense, "CreateLicenseData");
// 		if(CreateLicenseData)
// 		{
// 			CreateLicenseData(&s_pLicense);
// 		}
// 	}

	//添充组数据
	TCHAR		szData[2048]	= {0};
	TCHAR		szKey[200]		= {0};
	TCHAR*		pData			= szData;
	TCHAR*		pChar			= NULL;
	int			iValue			= 0;
	GetPrivateProfileString("AGENT", NULL, "", szData, sizeof(TCHAR) * 2048, theApp.GetIniPath());
	while(strlen(pData))
	{
		szKey[0] = 0;
		strncpy(szKey, pData, 200);
		pData += strlen(pData) + 1;
		iValue = GetPrivateProfileInt("AGENT", szKey, 0, theApp.GetIniPath());
		int nIndex = m_cbGroup.AddString(szKey);
		m_cbGroup.SetItemData(nIndex, iValue);
	}
	m_cbGroup.SetCurSel(0);
	OnCbnSelchangeCbGroup();
	// 设置图标
	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ADD)), FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CAddLicensedDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_CB_GROUP, OnCbnSelchangeCbGroup)
END_MESSAGE_MAP()


void CAddLicensedDlg::OnBnClickedOk()
{
	// 
	UpdateData();
	if(m_szID == "")
	{
		MessageBox("请设置用户ID", "严重错误", MB_OK | MB_ICONERROR);
		return;
	}
	if(m_szClientCount == "")
	{
		MessageBox("授权连接量不能为空", "严重错误", MB_OK | MB_ICONERROR);
		return;
	}
	if(m_szAuthor == "" || m_szTel == "" || m_szUser == "")
	{
		if(MessageBox("输入信息不完整是否断续?", "警告", MB_YESNO | MB_ICONWARNING) != IDYES)
			return ;
	}
	GetDlgItemText(IDC_DT_TIME, m_szMatureDate);
	int nIndex = m_cbGroup.GetItemData(m_cbGroup.GetCurSel());
	if(nIndex > 50) //普通用户
		m_nType = 0;
	OnOK();
}

void CAddLicensedDlg::OnCbnSelchangeCbGroup()
{
	int nIndex = m_cbGroup.GetItemData(m_cbGroup.GetCurSel());
	TCHAR		szProcID[50]	= {0};
	BOOL		bValidate		= FALSE;
	int			i				= 0;
// 	do 
// 	{
// 		if(NULL != s_pLicense)
// 			s_pLicense->CreateLcnKey(szProcID, nIndex);
// 		CLicense2Doc*		pDoc = (CLicense2Doc *)(CLicenseManagerDoc *)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument();
// 		if(NULL != pDoc)
// 			bValidate = pDoc->GetLicenseKeyValidate(szProcID);
// 		i++;
// 	} while ((bValidate == FALSE) && (i < 50));
// 	if(i >= 50)
// 	{
// 		AfxMessageBox("无法生成有效KEY", MB_OK | MB_ICONERROR);
// 		EndDialog(IDCANCEL);
// 	}
	itoa(nIndex, szProcID, 10);
	SetDlgItemText(IDC_ST_ID, szProcID);
}
