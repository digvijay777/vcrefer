// AddUpnpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UpnpManger.h"
#include "AddUpnpDlg.h"
#include ".\addupnpdlg.h"


// CAddUpnpDlg dialog

IMPLEMENT_DYNAMIC(CAddUpnpDlg, CDialog)
CAddUpnpDlg::CAddUpnpDlg(MyUPnP* pUpnp, CWnd* pParent /* = NULL */)
	: CDialog(CAddUpnpDlg::IDD, pParent)
{
	m_upnp = pUpnp;
}

CAddUpnpDlg::~CAddUpnpDlg()
{
}

void CAddUpnpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddUpnpDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CAddUpnpDlg message handlers

void CAddUpnpDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// OnOK();
	MyUPnP::UPNPNAT_MAPPING			map		= {0};
	CString							str;

	GetDlgItemText(IDC_CB_PROTOCOL, str);
	map.protocol = (str.CompareNoCase("TCP") == 0)?MyUPnP::UNAT_TCP:MyUPnP::UNAT_UDP;
	map.internalPort = GetDlgItemInt(IDC_ET_INPORT);
	map.externalPort = GetDlgItemInt(IDC_ET_OUTPORT);
	GetDlgItemText(IDC_ET_DESCRIB, map.description);

	if(MyUPnP::UNAT_OK == m_upnp->AddNATPortMapping(&map))
	{
		OnOK();
	}
	else
	{
		MessageBox(m_upnp->GetLastError(), NULL, MB_OK|MB_ICONERROR);
	}
}

BOOL CAddUpnpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CComboBox *)GetDlgItem(IDC_CB_PROTOCOL))->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
