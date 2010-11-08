// ModifyLicenseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LicenseManager.h"
#include "ModifyLicenseDlg.h"
#include ".\modifylicensedlg.h"


// CModifyLicenseDlg dialog

IMPLEMENT_DYNAMIC(CModifyLicenseDlg, CDialog)
CModifyLicenseDlg::CModifyLicenseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyLicenseDlg::IDD, pParent)
	, m_szID(_T(""))
	, m_szUser(_T(""))
	, m_szTel(_T(""))
	, m_szCount(_T(""))
	, m_szCurCount(_T(""))
	, m_szTime(_T(""))
	, m_szChange(_T(""))
	, m_szAuthor(_T(""))
	, m_szAddTime(_T(""))
	, m_szTitle(_T("修改操作"))
{
}

CModifyLicenseDlg::~CModifyLicenseDlg()
{
}

void CModifyLicenseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ST_ID, m_szID);
	DDX_Text(pDX, IDC_ET_USER, m_szUser);
	DDX_Text(pDX, IDC_ET_TEL, m_szTel);
	DDX_Text(pDX, IDC_ET_COUNT, m_szCount);
	DDX_Text(pDX, IDC_ET_CHANGE, m_szChange);
	DDX_Text(pDX, IDC_ET_CURCOUNT, m_szCurCount);
	DDX_Text(pDX, IDC_ET_AUTHOR, m_szAuthor);
	DDX_Text(pDX, IDC_ST_ADDTIME, m_szAddTime);
}


BEGIN_MESSAGE_MAP(CModifyLicenseDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_MODIFY, OnBnClickedModify)
	ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
END_MESSAGE_MAP()


// CModifyLicenseDlg message handlers

void CModifyLicenseDlg::OnBnClickedOk()
{
	// 修改
	//OnOK();
}

void CModifyLicenseDlg::OnBnClickedModify()
{
	// 添加代码
	UpdateData();
	GetDlgItemText(IDC_DT_TIME, m_szTime);
	CString		szPrompt;
	szPrompt.Format("确实要修改ID为\"%s\"的项吗?", m_szID);
	if(AfxMessageBox(szPrompt, MB_YESNO | MB_ICONQUESTION) != IDYES)
		return ;
	//////////////////////////////////////////////////////////////////////////
	// 修改受理次数
	if(m_szTime != m_szOldTime || m_szCount != m_szOldCount)
	{
		int n = atoi(m_szChange.GetBuffer());
		n++;
		m_szChange.Format("%d", n);
	}
	EndDialog(IDMODIFY);
}

void CModifyLicenseDlg::OnBnClickedDelete()
{
	// 删除
	CString		szPrompt;
	szPrompt.Format("确实要删除ID为\"%s\"的项吗?", m_szID);
	if(AfxMessageBox(szPrompt, MB_YESNO | MB_ICONQUESTION) != IDYES)
		return ;
	EndDialog(IDDELETE);
}

BOOL CModifyLicenseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 从数据库里初始化代码
	CString		szSQL;
	BOOL		bInvalidate		= FALSE;
	szSQL.Format("Select * from License2 Where UserSN='%s'", m_szID.GetBuffer());
// 	try
// 	{
// 		_RecordsetPtr	spRes = theApp.m_spConnect->Execute(szSQL.GetBuffer(), 0, 0);
// 		if(spRes == NULL)
// 			throw  NULL;
// 		if(spRes->adoEOF)
// 			throw true;
// 		m_szAddTime		= (LPCSTR)(_bstr_t)spRes->Fields->GetItem(_variant_t("AuthorizationDate"))->GetValue();
// 		m_szAuthor		= (LPCSTR)(_bstr_t)spRes->Fields->GetItem(_variant_t("AttachName"))->GetValue();
// 		m_szChange		= (LPCSTR)(_bstr_t)spRes->Fields->GetItem(_variant_t("ChangeNum"))->GetValue();
// 		m_szCount		= (LPCSTR)(_bstr_t)spRes->Fields->GetItem(_variant_t("PermitClient"))->GetValue();
// 		m_szCurCount	= (LPCSTR)(_bstr_t)spRes->Fields->GetItem(_variant_t("CurrClient"))->GetValue();
// 		m_szTime		= (LPCSTR)(_bstr_t)spRes->Fields->GetItem(_variant_t("MatureDate"))->GetValue();
// 		m_szUser		= (LPCSTR)(_bstr_t)spRes->Fields->GetItem(_variant_t("CustName"))->GetValue();
// 		m_szTel			= (LPCSTR)(_bstr_t)spRes->Fields->GetItem(_variant_t("Tel"))->GetValue();
// 		spRes->Close();
// 		m_szAddTime	= "添加时间:" + m_szAddTime;
// 		bInvalidate = TRUE;
// 	}
// 	catch(_com_error e)
// 	{
// 		::MessageBox(AfxGetMainWnd()->m_hWnd, e.Description(), "操作提示",MB_OK | MB_ICONERROR);
// 		return FALSE;
// 	}
// 	catch(bool b)
// 	{
// 
// 	}
// 	catch(...)
// 	{
// 		EndDialog(IDINVALID);
// 		return FALSE;
// 	}
// 	if(FALSE == bInvalidate)
// 	{
// 		//查看是否禁用？
// 		szSQL.Format("Select * from InvalidLicense2 Where UserSN='%s'", m_szID.GetBuffer());
// 		try
// 		{
// 			_RecordsetPtr	spRes = theApp.m_spConnect->Execute(szSQL.GetBuffer(), 0, 0);
// 			if(spRes == NULL)
// 				throw  NULL;
// 			if(spRes->adoEOF)
// 				throw NULL;
// 			m_szAddTime		= (LPCSTR)(_bstr_t)spRes->Fields->GetItem(_variant_t("LastTime"))->GetValue();
// 			spRes->Close();
// 			bInvalidate = TRUE;
// 			m_szAddTime = "Key:" + m_szID + "被禁用\n禁用时间:" + m_szAddTime;
// 			::MessageBox(AfxGetMainWnd()->m_hWnd, m_szAddTime, "操作提示",MB_OK | MB_ICONQUESTION);
// 			EndDialog(IDINVALID);
// 			return FALSE;
// 		}
// 		catch(_com_error e)
// 		{
// 			::MessageBox(AfxGetMainWnd()->m_hWnd, e.Description(), "操作提示",MB_OK | MB_ICONERROR);
// 			return FALSE;
// 		}
// 		catch(...)
// 		{
// 			EndDialog(IDINVALID);
// 			return FALSE;
// 		}
// 	}

	UpdateData(FALSE);
	// TODO:  在此添加额外的初始化
	m_szOldCount = m_szCount;
	m_szOldTime = m_szTime;
	//设置时间
	if(m_szTime != "")
	{
		int		nYear = 2009, nMonth = 1, nDay = 1, nHour = 0, nMin = 0, nSec = 0;
		sscanf(m_szTime.GetBuffer(), "%d-%d-%d %02d:%02d:%02d", &nYear, &nMonth, &nDay, &nHour, &nMin, &nSec);
		CTime  ttime(nYear, nMonth, nDay, nHour, nMin, nSec);
		((CDateTimeCtrl *)GetDlgItem(IDC_DT_TIME))->SetTime(&ttime);
	}
	// 标题
	SetWindowText(m_szTitle);
	// 设置图标
	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MODIFY)), FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
