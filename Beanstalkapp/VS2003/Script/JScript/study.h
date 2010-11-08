#pragma once

#include <afxdisp.h>
#include <activscp.h>

class CCodeObject;
class CScriptSite;

class CScriptingSupportHelper
{
public:
	CScriptingSupportHelper();
	~CScriptingSupportHelper();

	BOOL Create(CWnd* pWnd);
	BOOL RunScript(CString str);

	CCodeObject* GetCodeObject() const { return m_pCodeObject; }
	CScriptSite* GetScriptSite() const { return m_pScriptSite; }
	IActiveScript* GetActiveScript() const { return m_pActiveScript; }

public:
	CCodeObject* m_pCodeObject;
	CScriptSite* m_pScriptSite;

	IActiveScript* m_pActiveScript;   
	IActiveScriptParse* m_pActiveScriptParse;
};

class CCodeObject : public CCmdTarget
{
public:
	CCodeObject(CScriptingSupportHelper* pScripting, CWnd* pWnd);
	virtual ~CCodeObject();

	void Line(long, long, long, long);
	void Ellipse(long, long, long, long);
	void DrawText(LPCTSTR msg, long x, long y, long w, long h);
	void Alert(LPCTSTR msg);

	void OnPaint();
	void OnMouseClick(long x, long y);
	
private:
	CWnd* m_pWnd;
	CScriptingSupportHelper* m_pScripting;
	BOOL GetDispatch(OLECHAR* name, COleDispatchDriver& disp, DISPID& dispid);

	enum 
	{
		idLine = 1,
		idEllipse,
		idDrawText,
		idAlert
	};

	DECLARE_DISPATCH_MAP()
};

class CScriptSite : public IActiveScriptSite
{
public:
	CScriptSite(CScriptingSupportHelper* pScripting)   
	{ 
		m_pScripting = pScripting;
	};

	~CScriptSite()  
	{
	};

	virtual ULONG STDMETHODCALLTYPE AddRef() 
	{    
		return InterlockedIncrement(&m_nRefCount); 
	}

	virtual ULONG STDMETHODCALLTYPE Release() 
	{     
		return InterlockedDecrement(&m_nRefCount);
	};

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppObj)
	{
		*ppObj = NULL;

		if ((iid == IID_IUnknown) || (iid == IID_IActiveScriptSite))
		{
			*ppObj= (IActiveScriptSite*)this;
			AddRef();
			return S_OK;
		} 

		return E_NOINTERFACE;
	}

	virtual HRESULT STDMETHODCALLTYPE GetLCID(LCID __RPC_FAR *) 
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetItemInfo(LPCOLESTR, DWORD, IUnknown __RPC_FAR *__RPC_FAR * pObj, ITypeInfo __RPC_FAR *__RPC_FAR *)
	{
		ASSERT(m_pScripting);
		ASSERT(m_pScripting->GetCodeObject());

		*pObj = m_pScripting->GetCodeObject()->GetIDispatch(TRUE);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetDocVersionString(BSTR __RPC_FAR *)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE OnScriptTerminate(const VARIANT __RPC_FAR * ,const EXCEPINFO __RPC_FAR *) 
	{
		return E_NOTIMPL;
	}


	virtual HRESULT STDMETHODCALLTYPE OnStateChange(SCRIPTSTATE)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE OnScriptError(IActiveScriptError __RPC_FAR * pScriptError)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE OnEnterScript()
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE OnLeaveScript()
	{
		return E_NOTIMPL;
	}

private:
	long m_nRefCount;
	CScriptingSupportHelper* m_pScripting;
};
