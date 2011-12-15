// ShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TransWnd.h"
#include "ShowDlg.h"


// CShowDlg dialog

IMPLEMENT_DYNAMIC(CShowDlg, CDialog)

CShowDlg::CShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowDlg::IDD, pParent)
{

}

CShowDlg::~CShowDlg()
{
}

void CShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowDlg, CDialog)
END_MESSAGE_MAP()


// CShowDlg message handlers
