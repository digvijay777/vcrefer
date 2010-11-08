// DHTMLDialogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DHTMLDialog.h"
#include "DHTMLDialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "../../../Common/html/DHTMLDlg.h"
#include ".\dhtmldialogdlg.h"

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
public:
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CDHTMLDialogDlg 对话框

BEGIN_DHTML_EVENT_MAP(CDHTMLDialogDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
	DHTML_EVENT_ONCLICK(_T("ButtonHideImg"), OnButtonHideImg)
	DHTML_EVENT_ONCLICK(_T("ButtonSelect"), OnButtonSelect)
	DHTML_EVENT_ONCLICK(_T("ButtonCheck"), OnButtonCheck)
	DHTML_EVENT_ONCLICK(_T("ButtonAddSelect"), OnButtonAddSelect)
	DHTML_EVENT_ONCLICK(_T("ButtonDeleteSelect"), OnButtonDeleteSelect)
	DHTML_EVENT_ONCLICK(_T("ButtonClickImg"), OnButtonClickImg)
END_DHTML_EVENT_MAP()


CDHTMLDialogDlg::CDHTMLDialogDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CDHTMLDialogDlg::IDD, CDHTMLDialogDlg::IDH, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	DHTML::g_pDHTMLDialog = this;
}

void CDHTMLDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDHTMLDialogDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
//	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CDHTMLDialogDlg 消息处理程序

BOOL CDHTMLDialogDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

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
	BOOL bDlg1 = DHTML::CheckDHTMLDialogPointer(this);
	BOOL bDlg2 = this->IsKindOf(RUNTIME_CLASS(CDHTMLDialogDlg));
	CRuntimeClass* p1 = this->GetRuntimeClass();
	CRuntimeClass* p2 = RUNTIME_CLASS(CDHtmlDialog);
	CRuntimeClass* p3 = RUNTIME_CLASS(CDHTMLDialogDlg);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CDHTMLDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDHtmlDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDHTMLDialogDlg::OnPaint() 
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
		CDHtmlDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CDHTMLDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
HRESULT CDHTMLDialogDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CDHTMLDialogDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

HRESULT CDHTMLDialogDlg::OnButtonHideImg(IHTMLElement *pElement)
{
	BSTR	bstrText;
	HRESULT hr;

	hr = pElement->get_innerText(&bstrText);
	if(FAILED(hr))
		return E_INVALIDARG;
	if(wcscmp(bstrText, L"隐藏图片") == 0)
	{
		DHTML::SetHTMLElementVisible("idImg", FALSE);
		BSTR  bstrHide = ::SysAllocString(L"显示图片");
		pElement->put_innerText(bstrHide);
		::SysFreeString(bstrHide);
	}
	else
	{
		DHTML::SetHTMLElementVisible("idImg", TRUE);
		BSTR bstrShow = ::SysAllocString(L"隐藏图片");
		pElement->put_innerText(::SysAllocString(L"隐藏图片"));
		::SysFreeString(bstrShow);
	}
	::SysFreeString(bstrText);
	return S_OK;
}

HRESULT CDHTMLDialogDlg::OnButtonSelect(IHTMLElement *pElement)
{
	AfxMessageBox(DHTML::GetHTMLSelectValue("idSel"));
	DHTML::SetHTMLSelectValue("idSel", "3");
	return S_OK;
}

HRESULT CDHTMLDialogDlg::OnButtonCheck(IHTMLElement *pElement)
{
	DHTML::SetHTMLInnerText("idCheck_label", "显示图片");
	DHTML::CopyHTMLOnClicked("idCheck", "idCheck_hidden");
 	HRESULT hr ;
// // 	hr = this->m_spHtmlDoc->QueryInterface(&spScript);
// 	IDispatch* pDisp;
// 	hr = m_spHtmlDoc->get_Script(&pDisp);
// // 	CComQIPtr<IHTMLScriptElement> spScript(pDisp);
// 	//hr = pDisp->QueryInterface(&spScript);
// 	if(FAILED(hr))
// 		return E_NOINTERFACE;
// 	//return E_NOTIMPL
// 	VARIANT			var;
// // 
// 	VariantInit(&var);
// 	var = DHTML::GetHTMLOnClicked("idCheck_hidden");
// 	if(VT_DISPATCH != var.vt)
// 		return E_NOINTERFACE;
// 	IDispatch* pDisp2 = var.pdispVal;
// 	IHTMLEventObj*	pObj = NULL;
// 	IHTMLWindow2 *pWindow2 = NULL;
// 	hr = m_spHtmlDoc->get_parentWindow(&pWindow2);
// 	hr = pWindow2->get_event(&pObj);
	//pObj->
//	hr = pDisp2->QueryInterface(IID_IElementNamespaceFactory, (void **)&pObj);
	//hr = pDisp2->QueryInterface(IID_IElementBehavior, (void **)&pObj);
	IHTMLElement* pIdCheck = NULL;
	hr = GetElement("idCheck", &pIdCheck);
	if(FAILED(hr))
		return E_NOINTERFACE;
//	pIdCheck->put_onclick()

	return S_OK;
}

HRESULT CDHTMLDialogDlg::OnButtonAddSelect(IHTMLElement *pElement)
{
// 	IHTMLSelectElement *pSel;
// 	IHTMLOptionElement *pOpt;
// 	IHTMLElement *pElem;
// 	HRESULT hr;
// 
// 	hr = GetElement("idSel", &pElem);
// 	hr = pElem->QueryInterface(IID_IHTMLSelectElement, (VOID **)&pSel);
// //	hr = pSel->remove(1);
// // 	IDispatch *pdisp;
// // 	pSel->get_options(&pdisp);
// // 	pdisp->QueryInterface(IID_IHTMLOptionElement, (void **)&pOpt);
// 	IHTMLElement *pNew = NULL;
// 	hr = m_spHtmlDoc->createElement(::SysAllocString(L"OPTION"), (IHTMLElement**)&pNew);
// // 	hr = pNew->QueryInterface(IID_IHTMLOptionElement, (void **)&pOpt);
// //	hr = pNew->setAttribute(L"value", CComVariant(L"5"));
// //	pNew->put_outerText(L"星期五");
// 	hr = pNew->QueryInterface(IID_IHTMLOptionElement, (void **)&pOpt);
// 	if(FAILED(hr))
// 		return hr;
// 	pOpt->put_value(::SysAllocString(L"5"));
// 	pOpt->put_text(::SysAllocString(L"星期五"));
// //	hr = pNew->put_innerText(::SysAllocString(L"星期五"));
// //	pNew->put_outerHTML(L"<option value='5'>星期五</option>");
// //	pNew->put_outerText(L"星期五");
// //	pNew->put_value(L"5");
// //	pNew->put_text(L"星期五");
// //	pNew->QueryInterface(&spOpt);
// 	VARIANT  vt;
// 	VariantInit(&vt);
// 	vt.vt = VT_I4;
// 	vt.lVal = 4;
// //	hr = pSel->add(pNew, vt);
// 	BSTR str;
// 	pNew->get_outerHTML(&str);
// 	hr = pSel->add(pNew, vt);
// //	hr = pElem->put_innerHTML(::SysAllocString(str));
// //	hr = pElem->get_outerHTML(&str);
// 	//pOpt->
	DHTML::HTMLSelectOptionAdd("idSel", "星期五", "5");
	return S_OK;
}

HRESULT CDHTMLDialogDlg::OnButtonDeleteSelect(IHTMLElement *pElement)
{
	DHTML::HTMLSelectOptionDelete("idSel", "5");
	return S_OK;
}

HRESULT CDHTMLDialogDlg::OnButtonClickImg(IHTMLElement *pElement)
{
	IHTMLScriptElement*		pScript			= NULL;
	IHTMLElement*			pNewElement		= NULL;
	IHTMLScriptElement*		pDocScript		= NULL;
	HRESULT					hr;
	IDispatch*				pDispatch		= NULL;
	IHTMLDOMNode*			pNode			= NULL;
	IHTMLDOMNode*			pNewNode		= NULL;
	IHTMLDOMNode*			pNewNode2		= NULL;
	IHTMLElement*			pTbElement		= NULL;

// 	hr = m_spHtmlDoc->get_Script(&pDispatch);
// 	if(FAILED(hr))
// 		return S_OK;
// 	hr = pDispatch->QueryInterface(IID_IHTMLScriptElement, (void **)&pDocScript);
// 	pDocScript->Release();
// 	if(FAILED(hr))
// 		return S_OK;
	//m_spHtmlDoc->get_
	hr = GetElement("tbImg", &pTbElement);
	hr = pTbElement->QueryInterface(IID_IHTMLDOMNode, (void **)&pNode);
	if(FAILED(hr))
		return S_OK;
	hr = m_spHtmlDoc->createElement(CComBSTR("SCRIPT"), &pNewElement);
	hr = pNewElement->QueryInterface(IID_IHTMLScriptElement, (void **)&pScript);
	if(FAILED(hr))
		return S_OK;
	hr = pScript->put_event(CComBSTR("onclick()"));
	hr = pScript->put_type(CComBSTR("text/javascript"));
	hr = pScript->put_htmlFor(CComBSTR("idImg"));
	hr = pScript->put_text(CComBSTR("alert('单击图片');"));
	pScript->Release();
	hr = pNewElement->QueryInterface(IID_IHTMLDOMNode, (void **)&pNewNode);
	pNewElement->Release();
// 	pNode->insertBefore(pNewNode, CComVariant(), &pNewNode2);
	pNode->appendChild(pNewNode, &pNewNode2);
	pNewNode->Release();
	//m_spHtmlDoc->execCommand()
	//m_spHtmlDoc->
	return S_OK;
}
//void CAboutDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
//{
//	// TODO: 在此处添加消息处理程序代码
//}

//void CDHTMLDialogDlg::OnRButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CDHtmlDialog::OnRButtonDown(nFlags, point);
//}
