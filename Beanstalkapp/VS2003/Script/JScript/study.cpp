
#include "StdAfx.h"
#include "study.h"

CCodeObject::CCodeObject(CScriptingSupportHelper* pScripting, CWnd* pWnd) 
: m_pWnd(pWnd),
m_pScripting(pScripting)

{
	EnableAutomation();
}

CCodeObject::~CCodeObject() 
{
}

BEGIN_DISPATCH_MAP(CCodeObject, CCmdTarget)
	DISP_FUNCTION_ID(CCodeObject, "Line", idLine, Line, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CCodeObject, "Ellipse", idEllipse, Ellipse, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CCodeObject, "DrawText", idDrawText, DrawText, VT_EMPTY, VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CCodeObject, "Alert", idAlert, Alert, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

void CCodeObject::Line(long x1, long y1, long x2, long y2)
{
	CWindowDC dc(m_pWnd);

	dc.MoveTo(x1, y1);
	dc.LineTo(x2, y2);
}

void CCodeObject::Ellipse(long x1, long y1, long x2, long y2)
{
	CWindowDC dc(m_pWnd);
	dc.Ellipse(x1, y1, x2, y2);
}

void CCodeObject::DrawText(LPCTSTR msg, long x, long y, long w, long h)
{
	CWindowDC dc(m_pWnd);
	CRect rect(x, y, x+w, y+h);

	dc.DrawText(msg, rect, 0);
}

void CCodeObject::Alert(LPCTSTR msg)
{
	MessageBox(m_pWnd->m_hWnd, msg, "JScript", MB_OK);
}

void CCodeObject::OnPaint()
{
	COleDispatchDriver disp;
	DISPID dispid;
	if (GetDispatch(L"OnPaint", disp, dispid)) {
		disp.InvokeHelper(dispid, DISPATCH_METHOD, VT_EMPTY, 0, 0);
	}
}

BOOL CCodeObject::GetDispatch(OLECHAR* name, COleDispatchDriver& disp, DISPID& dispid) 
{
	IDispatch* pScriptDispatch = 0;
	m_pScripting->GetActiveScript()->GetScriptDispatch(0, &pScriptDispatch);
	disp.AttachDispatch(pScriptDispatch);
	HRESULT hr = pScriptDispatch->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	return SUCCEEDED(hr);
}

void CCodeObject::OnMouseClick(long x, long y)
{
	COleDispatchDriver disp;
	DISPID dispid;
	if (GetDispatch(L"OnMouseClick", disp, dispid)) {
		disp.InvokeHelper(dispid, DISPATCH_METHOD, VT_EMPTY, 0, (const BYTE*)(VTS_I4 VTS_I4), x, y);
	}
}

//////////////////////////////////////////////////////////////////////////

CScriptingSupportHelper::CScriptingSupportHelper()
: m_pCodeObject(0),
m_pScriptSite(0),
m_pActiveScript(0),
m_pActiveScriptParse(0)
{
}

CScriptingSupportHelper::~CScriptingSupportHelper()
{
	if (m_pActiveScript) 
	{
		m_pActiveScript->Close();
		m_pActiveScriptParse->Release();
		m_pActiveScript->Release();
	}

	delete m_pCodeObject; m_pCodeObject=0;
	delete m_pScriptSite; m_pScriptSite=0;
}

BOOL CScriptingSupportHelper::RunScript(CString strText)
{
	EXCEPINFO ei = {0};
	BSTR bstrText = strText.AllocSysString();
	m_pActiveScriptParse->ParseScriptText(bstrText, NULL, NULL, NULL, 0, 0, 0, NULL, &ei);
	SysFreeString(bstrText);

	m_pActiveScript->SetScriptState(SCRIPTSTATE_CONNECTED);

	return TRUE;
}

BOOL CScriptingSupportHelper::Create(CWnd* pWnd)
{
	m_pCodeObject = new CCodeObject(this, pWnd);
	m_pScriptSite = new CScriptSite(this);

	CLSID clsidJScript;
	CLSIDFromProgID(L"JScript", &clsidJScript);
	CoCreateInstance(clsidJScript, NULL, CLSCTX_INPROC_SERVER, IID_IActiveScript, (void **)&m_pActiveScript);
	m_pActiveScript->QueryInterface(IID_IActiveScriptParse, (void**)&m_pActiveScriptParse);
	m_pActiveScript->SetScriptSite(m_pScriptSite);
	m_pActiveScript->AddNamedItem(L"Code", SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISSOURCE | SCRIPTITEM_GLOBALMEMBERS);
	m_pActiveScriptParse->InitNew();


	return TRUE;
}