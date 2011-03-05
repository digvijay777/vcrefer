// ActiveContainer.cpp : Implementation of CActiveContainer

#include "stdafx.h"
#include "ActiveContainer.h"
#include "ScriptTable.h"
#include <comutil.h>


#pragma comment(lib, "comsuppw.lib")
// CActiveContainer
LRESULT CActiveContainer::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxDialogImpl<CActiveContainer>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	bHandled = TRUE;

	m_pScriptTable = (CScriptTable *)lParam;
	// �������
	HRESULT		hres				= E_FAIL;;
	CLSID		clsid				= {0};
	TCHAR		szText[1024]		= {0};

	// �õ���ʶ��
	hres = CLSIDFromString(_bstr_t(m_pScriptTable->m_clsid.c_str()), &clsid);
	if(FAILED(hres))
	{
		SetDlgItemText(IDC_ST_INFO, _T("��Ч��ע�����"));
		return 1;
	}
	// �������
	hres = m_spActiveX.CoCreateInstance(clsid);
	if(FAILED(hres))
	{
		_sntprintf(szText, sizeof(szText)/sizeof(TCHAR), _T("��ʼ�����ʧ��: 0x%08X"), hres);
		SetDlgItemText(IDC_ST_INFO, szText);
		return 1;
	}
	GetDlgItem(IDC_ST_INFO).ShowWindow(SW_HIDE);
	// �Ƿ񴰿����
	
	return 1;  // Let the system set the focus
}

LRESULT CActiveContainer::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT		rect	= {0, GET_Y_LPARAM(lParam) / 2 - 20, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) / 2 + 20};

	if(GetDlgItem(IDC_ST_INFO).IsWindowVisible())
		GetDlgItem(IDC_ST_INFO).MoveWindow(&rect);
	return 0;
}

LRESULT CActiveContainer::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}