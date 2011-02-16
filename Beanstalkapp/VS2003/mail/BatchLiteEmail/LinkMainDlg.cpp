// LinkMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BatchLiteEmail.h"
#include "LinkMainDlg.h"
#include ".\linkmaindlg.h"
#import "msxml3.dll"

template <typename TDomPtr>
void inline	XmlSetNodeText(TDomPtr& spPtr, _bstr_t bstrPath, _bstr_t bstrText)
{
	try
	{
		MSXML2::IXMLDOMNodePtr		spNode;

		spNode = spPtr->selectSingleNode(bstrPath);
		if(NULL == spNode)
			return;
		spNode->put_text(bstrText);
	}
	catch(_com_error&)
	{
	}
}
// CLinkMainDlg dialog

IMPLEMENT_DYNAMIC(CLinkMainDlg, CDialog)
CLinkMainDlg::CLinkMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLinkMainDlg::IDD, pParent)
{
	m_bModify = FALSE;
}

CLinkMainDlg::~CLinkMainDlg()
{
}

void CLinkMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CLinkMainDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnLvnItemchangedList1)
//	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, OnHdnItemdblclickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
	ON_BN_CLICKED(IDC_SAVEADD, OnBnClickedSaveadd)
	ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
	ON_BN_CLICKED(IDC_SAVEMODIFY, OnBnClickedSavemodify)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CLinkMainDlg message handlers

void CLinkMainDlg::OnBnClickedAdd()
{
	SetDlgItemText(IDC_ET_NAME, _T(""));
	SetDlgItemText(IDC_ET_EMAIL, _T(""));
	SetDlgItemText(IDC_ET_PHONE, _T(""));
	SetDlgItemText(IDC_ET_COMPANY, _T(""));
	GetDlgItem(IDC_SAVEADD)->EnableWindow(TRUE);
	GetDlgItem(IDC_SAVEMODIFY)->EnableWindow(FALSE);
	GetDlgItem(IDC_DELETE)->EnableWindow(FALSE);
	m_list.SetHotItem(-1);
}

//void CLinkMainDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

BOOL CLinkMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0, "姓名", 0, 100);
	m_list.InsertColumn(1, "邮箱", 0, 100);
	m_list.InsertColumn(2, "电话", 0, 100);
	m_list.InsertColumn(3, "公司", 0, 100);

	OnLoadLinkMan();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLinkMainDlg::OnLoadLinkMan()
{
	MSXML2::IXMLDOMDocument2Ptr		spDoc;
	CString							strPath;

	try
	{
		spDoc.CreateInstance("Msxml2.DOMDocument");
		spDoc->put_async(VARIANT_FALSE);
		GetModuleFileName(NULL, strPath.GetBufferSetLength(1024), 1024);
		strPath.ReleaseBuffer();
		strPath.ReleaseBuffer(strPath.ReverseFind(_T('\\')) + 1);
		strPath += "BatchLiteEmail_Data.xml";
		spDoc->load(strPath.GetBuffer());

		MSXML2::IXMLDOMNodeListPtr  spList		= spDoc->selectNodes("/peoples/people");
		LONG						length		= spList->Getlength();
		
		for(long i = 0; i < length; i++)
		{
			MSXML2::IXMLDOMNodePtr		spNode	= spList->Getitem(i);
			int							nIndex;
			_bstr_t						sName, sPhone, sEmail, sCompany;

			spNode->selectSingleNode("name")->get_text(sName.GetAddress());
			spNode->selectSingleNode("email")->get_text(sEmail.GetAddress());
			spNode->selectSingleNode("phone")->get_text(sPhone.GetAddress());
			spNode->selectSingleNode("company")->get_text(sCompany.GetAddress());
			nIndex = m_list.InsertItem(-1, sName, 0);
			m_list.SetItemText(nIndex, 1, sEmail);
			m_list.SetItemText(nIndex, 2, sPhone);
			m_list.SetItemText(nIndex, 3, sCompany);
		}
		
	}
	catch(_com_error& e)
	{
		MessageBox(e.Description());
	}
}
//void CLinkMainDlg::OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

void CLinkMainDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	LPNMLISTVIEW	pLV		= reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	OnSelectItem(pLV->iItem);
}

void CLinkMainDlg::OnSelectItem(int nItem)
{
	if(0 > nItem)
	{
		OnBnClickedAdd();
		return;
	}
	m_list.SetHotItem(nItem);
	SetDlgItemText(IDC_ET_NAME, m_list.GetItemText(nItem, 0));
	SetDlgItemText(IDC_ET_EMAIL, m_list.GetItemText(nItem, 1));
	SetDlgItemText(IDC_ET_PHONE, m_list.GetItemText(nItem, 2));
	SetDlgItemText(IDC_ET_COMPANY, m_list.GetItemText(nItem, 3));
	GetDlgItem(IDC_SAVEADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_SAVEMODIFY)->EnableWindow(TRUE);
	GetDlgItem(IDC_DELETE)->EnableWindow(TRUE);
}

void CLinkMainDlg::OnBnClickedSaveadd()
{
	CString		sName, sPhone, sEmail, sCompany;
	int			nIndex	= 0;

	GetDlgItemText(IDC_ET_NAME, sName);
	GetDlgItemText(IDC_ET_EMAIL, sEmail);
	GetDlgItemText(IDC_ET_PHONE, sPhone);
	GetDlgItemText(IDC_ET_COMPANY, sCompany);

	nIndex = m_list.InsertItem(-1, sName, 0);
	m_list.SetItemText(nIndex, 1, sEmail);
	m_list.SetItemText(nIndex, 2, sPhone);
	m_list.SetItemText(nIndex, 3, sCompany);
	m_bModify = TRUE;

	OnSelectItem(nIndex);
}

void CLinkMainDlg::OnBnClickedDelete()
{
	m_list.DeleteItem(m_list.GetHotItem());
	m_bModify = TRUE;
	OnSelectItem(-1);
}

void CLinkMainDlg::OnBnClickedSavemodify()
{
	CString		sName, sPhone, sEmail, sCompany;
	int			nIndex	= m_list.GetHotItem();

	GetDlgItemText(IDC_ET_NAME, sName);
	GetDlgItemText(IDC_ET_EMAIL, sEmail);
	GetDlgItemText(IDC_ET_PHONE, sPhone);
	GetDlgItemText(IDC_ET_COMPANY, sCompany);

	m_list.SetItemText(nIndex, 0, sName);
	m_list.SetItemText(nIndex, 1, sEmail);
	m_list.SetItemText(nIndex, 2, sPhone);
	m_list.SetItemText(nIndex, 3, sCompany);
	m_bModify = TRUE;

	OnSelectItem(nIndex);
}

void CLinkMainDlg::OnBnClickedOk()
{
	if(FALSE == m_bModify)
	{
		OnOK();
		return;
	}
	// 保存操作
	try
	{
		MSXML2::IXMLDOMDocument2Ptr		spDoc;
		MSXML2::IXMLDOMNodePtr			spPeople;
		MSXML2::IXMLDOMNodePtr			spPeoples;

		spDoc.CreateInstance("Msxml2.DOMDocument");
		spDoc->put_async(VARIANT_FALSE);
		spDoc->loadXML(L"<?xml version=\"1.0\" ?>\n<peoples><people><name></name><email></email><phone></phone>"
			L"<company></company></people></peoples>");
		spPeople = spDoc->selectSingleNode("/peoples/people");
		spPeoples = spDoc->selectSingleNode("/peoples");
		for(int i = 0; i < m_list.GetItemCount(); i++)
		{
			MSXML2::IXMLDOMNodePtr		spNode;

			spNode = spPeople->cloneNode(VARIANT_TRUE);
			XmlSetNodeText(spNode, "name", m_list.GetItemText(i, 0).GetBuffer());
			XmlSetNodeText(spNode, "email", m_list.GetItemText(i, 1).GetBuffer());
			XmlSetNodeText(spNode, "phone", m_list.GetItemText(i, 2).GetBuffer());
			XmlSetNodeText(spNode, "company", m_list.GetItemText(i, 3).GetBuffer());
			spPeoples->appendChild(spNode);
		}
		spPeoples->removeChild(spPeople);

		// save
		CString		strPath;

		GetModuleFileName(NULL, strPath.GetBufferSetLength(1024), 1024);
		strPath.ReleaseBuffer();
		strPath.ReleaseBuffer(strPath.ReverseFind(_T('\\')) + 1);
		strPath += "BatchLiteEmail_Data.xml";
		spDoc->save(strPath.GetBuffer());
	}
	catch(_com_error& e)
	{
		MessageBox(e.Description(), NULL, MB_OK|MB_ICONERROR);
		return;
	}
	// 退出
	OnOK();
}

void CLinkMainDlg::OnBnClickedCancel()
{
	if(FALSE != m_bModify)
	{
		if(IDYES != MessageBox("您的修改操作还没有保存, 是否确定不保存退出?", NULL, MB_YESNO|MB_ICONQUESTION))
			return;
	}
	OnCancel();
}
