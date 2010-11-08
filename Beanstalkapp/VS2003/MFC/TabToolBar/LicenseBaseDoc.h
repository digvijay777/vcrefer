#pragma once



class CLicenseBaseDoc
{
public:
	CLicenseBaseDoc(void);
	virtual ~CLicenseBaseDoc(void);
public:
	BOOL		ExecuteSQL(LPCSTR lpSQL);
	LPCSTR		TranslateStrForSQL(CString& str);
	BOOL		FillListFromSQL(LPCSTR sql, CListCtrl* list);
	BOOL		ModifyLicensed(LPCSTR szID);
public:
	virtual VOID NotifyView(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
	//////////////////////////////////////////////////////////////////////////
	// 以下是与界面交互
public:
	BOOL		SetStatusBarText(int nIndex, LPCTSTR lpText);
};

#define TRYCOMEXCUTE(cmd) \
	try\
{\
	(cmd);\
}\
	catch(_com_error e)\
{}