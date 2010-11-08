#include "StdAfx.h"
#include "XBase.h"

//////////////////////////////////////////////////////////////////////////
// �Ի������
/*
 *	�����Ի���ģ��
 */
BOOL XBase::GetDialogTemplateInfo(HMODULE hModule, LPCSTR lpTemplate, DLGTEMPLATE* pDlg)
{
	HRSRC			hSrc				= NULL;
	HGLOBAL			hGlobal				= NULL;
	DLGTEMPLATE*	pDlgTemplate		= NULL;
	BOOL			bRet				= FALSE;

	hSrc = FindResource(hModule, lpTemplate, RT_DIALOG);
	if(NULL == hSrc)
		return FALSE;
	hGlobal = LoadResource(hModule, hSrc);
	if(NULL == hGlobal)
		return FALSE;
	pDlgTemplate = (DLGTEMPLATE *)LockResource(hGlobal);
	if(NULL != pDlgTemplate)
	{
		memcpy(pDlg, pDlgTemplate, sizeof(DLGTEMPLATE));
		UnlockResource(hGlobal);
		bRet = TRUE;
	}
	GlobalFree(hGlobal);
	return bRet;
}