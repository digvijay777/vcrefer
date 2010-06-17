#pragma once
#include <comdef.h>
#include <Wbemidl.h>

class CWMIBase
{
public:
	CWMIBase();
	~CWMIBase();
protected:
	BOOL						m_bInit;
	CComPtr<IWbemLocator>		m_spWbemLocator;
	CComPtr<IWbemServices>		m_spWbemServices;
	CHAR						m_szError[1024];
public:
	BOOL						Initialize();
	BOOL						ConnectServer(LPCTSTR lpNamespace = NULL, LPCTSTR lpUser = NULL, LPCTSTR lpPassword = NULL);
	BOOL						ExecuteWQL(LPCTSTR lpWQL, IEnumWbemClassObject** ppEnum);
	VARIANT						ExecMethod(IWbemClassObject* pInstance, LPCWSTR lpMethodName, ...);
	IWbemServices*				GetServices();

	CHAR*						GetLastErr();
	void						SetLastErr(LPCSTR lpFmt, ...);
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