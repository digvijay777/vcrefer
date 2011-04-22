// NPWebPlugin.cpp : Implementation of CNPWebPlugin
#include "stdafx.h"
#include "NPWebPlugin.h"


// CNPWebPlugin

LRESULT CNPWebPlugin::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// gIEPlugFuncs->newp();
	GetHtmlDocument(&m_spHtmlDoc);

	return 0;
}

LRESULT CNPWebPlugin::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default

	return 0;
}
