// ActiveContainer.h : Declaration of the CActiveContainer

#pragma once

#include "resource.h"       // main symbols

#include <atlhost.h>

class CScriptTable;
// CActiveContainer

class CActiveContainer : 
	public CAxDialogImpl<CActiveContainer>
{
public:
	CActiveContainer()
	{
	}

	~CActiveContainer()
	{
	}

	enum { IDD = IDD_ACTIVECONTAINER };

BEGIN_MSG_MAP(CActiveContainer)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	CHAIN_MSG_MAP(CAxDialogImpl<CActiveContainer>)
END_MSG_MAP()

public:
	CScriptTable*		m_pScriptTable;
	CComPtr<IDispatch>	m_spActiveX;
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


