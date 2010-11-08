// License2Doc.cpp :  CLicense2Doc ���ʵ��
//

#include "stdafx.h"
#include "LicenseManager.h"

#include "License2Doc.h"
#include <shlwapi.h>
#include "AddLicensedDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLicense2Doc ����/����

CLicense2Doc::CLicense2Doc()
{
	// TODO: �ڴ����һ���Թ������

}

CLicense2Doc::~CLicense2Doc()
{
}

void CLicense2Doc::Addlicensed()
{
	// ���������Ȩ��ť
	CAddLicensedDlg	dlg;
	if(dlg.DoModal() == IDOK)
	{
		//��ӽ�SQL���ݿ�
		CString strSQL;
		CTime	ttime = CTime::GetCurrentTime();
		strSQL.Format("Insert Into License2 values('%s','%s','%s','%s',%s,0,'%s','%s',1,%d)",
			dlg.m_szID.GetBuffer(), TranslateStrForSQL(dlg.m_szUser), TranslateStrForSQL(dlg.m_szTel), 
			TranslateStrForSQL(dlg.m_szAuthor), dlg.m_szClientCount, 
			ttime.Format("%Y-%m-%d %H:%m:%S"), dlg.m_szMatureDate, dlg.m_nType);
		if(FALSE == ExecuteSQL(strSQL.GetBuffer()))
			return ;
		NotifyView(UM_UPDATESHOW);
	}
}

BOOL CLicense2Doc::DeleteLicensed(LPCSTR szID)
{
	CString		szPropt;
	TCHAR		szSQL[1024]		= {0};

	szPropt.Format("ȷʵҪɾ��IDΪ\"%s\"�ļ�¼��?", szID);
	if(MessageBox(AfxGetMainWnd()->m_hWnd, szPropt.GetBuffer(), "ȷ�ϲ���", MB_YESNO | MB_ICONQUESTION) != IDYES)
		return FALSE;
	// ɾ����
	_stprintf(szSQL, "Delete from License2 Where UserSN='%s'; Insert into InvalidLicense2 values('%s', getdate())", szID, szID);
	if(FALSE == ExecuteSQL(szSQL))
		return FALSE;
	NotifyView(UM_UPDATESHOW);
	return TRUE;
}

// �õ�KEY��Ч��
BOOL CLicense2Doc::GetLicenseKeyValidate(LPCSTR lpKey)
{
	BOOL			bRet	= FALSE;
	CString			strSQL;
// 	_RecordsetPtr	spRes;

// 	strSQL.Format("select count(*) from (select UserSN from InvalidLicense2 " \
// 		"union select UserSN from License2) as License where UserSN='%s'", lpKey);
// 	try
// 	{
// 		spRes = theApp.m_spConnect->Execute(strSQL.GetBuffer(), NULL, 0);
// 		if(spRes == NULL)
// 			throw NULL;
// 		int n = (LONG)spRes->Fields->GetItem(CComVariant(0L))->GetValue();
// 		if(0 == n)
// 			bRet = TRUE;
// 		spRes->Close();
// 	}
// 	catch(_com_error e)
// 	{
// 	}
	return bRet;
}
