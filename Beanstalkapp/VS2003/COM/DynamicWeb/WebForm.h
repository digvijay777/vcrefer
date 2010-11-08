// WebForm.h : Declaration of the CWebForm

#ifndef __WEBFORM_H_
#define __WEBFORM_H_

#include "resource.h"       // main symbols
#include <atlctl.h>


/////////////////////////////////////////////////////////////////////////////
// CWebForm
class ATL_NO_VTABLE CWebForm : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IWebForm, &IID_IWebForm, &LIBID_DYNAMICWEBLib>,
	public CComCompositeControl<CWebForm>,
	public IPersistStreamInitImpl<CWebForm>,
	public IOleControlImpl<CWebForm>,
	public IOleObjectImpl<CWebForm>,
	public IOleInPlaceActiveObjectImpl<CWebForm>,
	public IViewObjectExImpl<CWebForm>,
	public IOleInPlaceObjectWindowlessImpl<CWebForm>,
	public CComCoClass<CWebForm, &CLSID_WebForm>
{
public:
	CWebForm()
	{
		m_bWindowOnly = TRUE;
		CalcExtent(m_sizeExtent);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WEBFORM)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CWebForm)
	COM_INTERFACE_ENTRY(IWebForm)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
END_COM_MAP()

BEGIN_PROP_MAP(CWebForm)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_MSG_MAP(CWebForm)
	COMMAND_HANDLER(IDC_BUTTON3, BN_CLICKED, OnBnClickedButton3)
	CHAIN_MSG_MAP(CComCompositeControl<CWebForm>)
	COMMAND_HANDLER(IDC_BUTTON1, BN_CLICKED, OnClickedButton1)
	COMMAND_HANDLER(IDC_BUTTON2, BN_CLICKED, OnClickedButton2)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

BEGIN_SINK_MAP(CWebForm)
	//Make sure the Event Handlers have __stdcall calling convention
END_SINK_MAP()

	STDMETHOD(OnAmbientPropertyChange)(DISPID dispid)
	{
		if (dispid == DISPID_AMBIENT_BACKCOLOR)
		{
			SetBackgroundColorFromAmbient();
			FireViewChange();
		}
		return IOleControlImpl<CWebForm>::OnAmbientPropertyChange(dispid);
	}



// IViewObjectEx
	DECLARE_VIEW_STATUS(0)

// IWebForm
public:

	enum { IDD = IDD_WEBFORM };
	LRESULT OnClickedButton1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedButton2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedButton3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

#endif //__WEBFORM_H_
