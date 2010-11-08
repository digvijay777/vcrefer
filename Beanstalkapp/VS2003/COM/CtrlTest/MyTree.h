// MyTree.h : CMyTree 的声明
#pragma once
#include "resource.h"       // 主符号
#include <atlctl.h>
#include <commctrl.h>

// IMyTree
[
	object,
	uuid(D547D663-850A-4B83-9FE5-F73C59B26812),
	dual,
	helpstring("IMyTree 接口"),
	pointer_default(unique)
]
__interface IMyTree : public IDispatch
{
};


// CMyTree
[
	coclass,
	threading("apartment"),
	vi_progid("CtrlTest.MyTree"),
	progid("CtrlTest.MyTree.1"),
	version(1.0),
	uuid("87997FC7-8A7A-4354-9712-F6F4C6100E69"),
	helpstring("MyTree Class"),
	support_error_info(IMyTree),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CMyTree : 
	public IMyTree,
	public IPersistStreamInitImpl<CMyTree>,
	public IOleControlImpl<CMyTree>,
	public IOleObjectImpl<CMyTree>,
	public IOleInPlaceActiveObjectImpl<CMyTree>,
	public IViewObjectExImpl<CMyTree>,
	public IOleInPlaceObjectWindowlessImpl<CMyTree>,
	public IPersistStorageImpl<CMyTree>,
	public ISpecifyPropertyPagesImpl<CMyTree>,
	public IQuickActivateImpl<CMyTree>,
	public IDataObjectImpl<CMyTree>,
	public IProvideClassInfo2Impl<&__uuidof(CMyTree), NULL>,
	public CComControl<CMyTree>
{
public:
	CContainedWindow m_ctlSysTreeView32;

#pragma warning(push)
#pragma warning(disable: 4355) // “this”: 用于基成员初始值设定项列表

	CMyTree()
		: m_ctlSysTreeView32(_T("SysTreeView32"), this, 1)
	{
		m_bWindowOnly = TRUE;
	}

#pragma warning(pop)

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CMyTree)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// 示例项
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CMyTree)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
	CHAIN_MSG_MAP(CComControl<CMyTree>)
ALT_MSG_MAP(1)
	// 将此替换为超类 SysTreeView32 的消息映射项
END_MSG_MAP()
// 处理程序原型: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	BOOL PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet)
	{
		if(pMsg->message == WM_KEYDOWN)
		{
			switch(pMsg->wParam)
			{
			case VK_LEFT:
			case VK_RIGHT:
			case VK_UP:
			case VK_DOWN:
			case VK_HOME:
			case VK_END:
			case VK_NEXT:
			case VK_PRIOR:
				hRet = S_FALSE;
				return TRUE;
			}
		}
		//TODO: 在此添加其他快捷键处理代码
		return FALSE;
	}

	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRes = CComControl<CMyTree>::OnSetFocus(uMsg, wParam, lParam, bHandled);
		if (m_bInPlaceActive)
		{
			if(!IsChild(::GetFocus()))
				m_ctlSysTreeView32.SetFocus();
		}
		return lRes;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		RECT rc;
		GetWindowRect(&rc);
		rc.right -= rc.left;
		rc.bottom -= rc.top;
		rc.top = rc.left = 0;
		InitCommonControls();
		m_ctlSysTreeView32.Create(m_hWnd, rc);
		return 0;
	}

	STDMETHOD(SetObjectRects)(LPCRECT prcPos,LPCRECT prcClip)
	{
		IOleInPlaceObjectWindowlessImpl<CMyTree>::SetObjectRects(prcPos, prcClip);
		int cx, cy;
		cx = prcPos->right - prcPos->left;
		cy = prcPos->bottom - prcPos->top;
		::SetWindowPos(m_ctlSysTreeView32.m_hWnd, NULL, 0,
			0, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE);
		return S_OK;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IMyTree

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}
};

