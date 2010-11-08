#include "StdAfx.h"
#include "LicenseManager.h"
#include "LicenseBaseDoc.h"
#include "ModifyLicenseDlg.h"

CLicenseBaseDoc::CLicenseBaseDoc(void)
{
}

CLicenseBaseDoc::~CLicenseBaseDoc(void)
{
}

// 执行SQL
BOOL CLicenseBaseDoc::ExecuteSQL(LPCSTR lpSQL)
{
// 	try
// 	{
// 		theApp.m_spConnect->Execute(lpSQL, NULL, 0);
// 	}
// 	catch(_com_error e)
// 	{
// 		MessageBox(AfxGetMainWnd()->m_hWnd, e.Description(), "操作数据错误", MB_OK | MB_ICONERROR);
// 		return FALSE;
// 	}
	return TRUE;
}

// 转换SQL代码 
LPCSTR CLicenseBaseDoc::TranslateStrForSQL(CString& str)
{
	str.Replace("\'", "\'\'");
	return str.GetBuffer();
}
// CLicense2Doc 命令
BOOL CLicenseBaseDoc::FillListFromSQL(LPCSTR sql, CListCtrl* list)
{
	if(NULL == sql || strlen(sql) < 15)
		return FALSE;
// 	_RecordsetPtr	pRecod;
// 	try
// 	{
// 		pRecod = theApp.m_spConnect->Execute(sql, NULL, 0);
// 	}
// 	catch(_com_error e)
// 	{
// 		AfxMessageBox(e.Description(), MB_OK | MB_ICONERROR);
// 		return FALSE;
// 	}
// 
// 	list->LockWindowUpdate();
// 	list->SetRedraw(FALSE);
// 	list->DeleteAllItems();
// 	TCHAR	szStr[1024]		= {0};
// 	for(int i = 0; !pRecod->adoEOF; i++, pRecod->MoveNext())
// 	{
// 		itoa(i+1, szStr, 10);
// 		list->InsertItem(i, szStr, 0);
// 		for(int j = pRecod->Fields->Count - 1; j >= 0; j--)
// 		{
// 			szStr[0] = 0;
// 			TRYCOMEXCUTE(strcpy(szStr, (LPCTSTR)(_bstr_t)pRecod->Fields->GetItem(_variant_t((LONG)j))->GetValue()));
// 			StrTrim(szStr, " ");
// 			list->SetItemText(i, j + 1, szStr);
// 		}
// 	}
// 	list->SetRedraw(TRUE);
// 	list->UnlockWindowUpdate();
// 	pRecod->Close();
	return TRUE;
}

// 设置状态栏指示器
BOOL CLicenseBaseDoc::SetStatusBarText(int nIndex, LPCTSTR lpText)
{
	CWnd*	pWnd = AfxGetMainWnd();
	if(NULL == pWnd)
		return FALSE;
	return pWnd->SendMessage(WM_USER+1, (WPARAM)nIndex, (LPARAM)lpText);
}

BOOL CLicenseBaseDoc::ModifyLicensed(LPCSTR szID)
{
	CModifyLicenseDlg		dlg;

	dlg.m_szID = szID;

	UINT	nRet = dlg.DoModal();
	CString	szSQL;
	if(IDMODIFY == nRet)
	{
		// 修改操作
		szSQL.Format("Update License2 Set CustName='%s', Tel='%s', PermitClient=%s " \
			", MatureDate='%s', ChangeNum=%s, AttachName='%s' Where UserSN='%s'", 
			TranslateStrForSQL(dlg.m_szUser), TranslateStrForSQL(dlg.m_szTel), dlg.m_szCount,
			dlg.m_szTime, dlg.m_szChange,
			TranslateStrForSQL(dlg.m_szAuthor), dlg.m_szID);
		if(FALSE == ExecuteSQL(szSQL))
			return FALSE;
		NotifyView(UM_UPDATESHOW);
	}
	else if(IDDELETE == nRet)
	{
		// 删除操作
		szSQL.Format("Delete From License2 Where UserSN = '%s'", dlg.m_szID.GetBuffer());
		if(FALSE == ExecuteSQL(szSQL))
			return FALSE;
		NotifyView(UM_UPDATESHOW);
	}
	return TRUE;
}