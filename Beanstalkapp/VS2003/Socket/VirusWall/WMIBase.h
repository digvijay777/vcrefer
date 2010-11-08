#pragma once
#include <comdef.h>
#include <Wbemidl.h>

class CWMIBase
{
public:
	CWMIBase();
	~CWMIBase();
protected:
	_bstr_t						m_bstrErr;
	BOOL						m_bInit;
	CComPtr<IWbemLocator>		m_spWbemLocator;
	CComPtr<IWbemServices>		m_spWbemServices;
public:
	BOOL		Initialize();
	BOOL		ConnectServer(LPCTSTR lpNamespace = NULL, LPCTSTR lpUser = NULL, LPCTSTR lpPassword = NULL);
	BOOL		ExecuteWQL(LPCTSTR lpWQL, IEnumWbemClassObject** ppEnum);
	VARIANT		ExecMethod(IWbemClassObject* pInstance, LPCWSTR lpMethodName, ...);
	IWbemServices*	GetServices();
};

class CWMIClassObjects
{
public:
	CWMIClassObjects();
	~CWMIClassObjects();
private:
	CComPtr<IEnumWbemClassObject>		m_spEnumWbem;
	IWbemClassObject*					m_pCurWbemObject;
public:
	IEnumWbemClassObject** operator&();
	BOOL	MoveFirst();
	BOOL	MoveNext();
};