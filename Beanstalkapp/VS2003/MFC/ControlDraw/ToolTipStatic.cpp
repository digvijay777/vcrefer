// ToolTipStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "ToolTipStatic.h"
#include ".\tooltipstatic.h"


// CToolTipStatic

IMPLEMENT_DYNAMIC(CToolTipStatic, CStatic)
CToolTipStatic::CToolTipStatic()
{
}

CToolTipStatic::~CToolTipStatic()
{
}


BEGIN_MESSAGE_MAP(CToolTipStatic, CStatic)
END_MESSAGE_MAP()



// CToolTipStatic message handlers


void CToolTipStatic::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CStatic::PreSubclassWindow();

	ModifyStyle(0, SS_NOTIFY);
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	m_tooltip.AddTool(this, "当前分数 98分");
}

LRESULT CToolTipStatic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	MSG		msg;

	PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
	if(NULL != m_tooltip.GetSafeHwnd())
		m_tooltip.RelayEvent(&msg);

	return CStatic::WindowProc(message, wParam, lParam);
}
