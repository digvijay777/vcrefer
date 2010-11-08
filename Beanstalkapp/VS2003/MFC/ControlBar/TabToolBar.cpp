// TabToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include "ControlBar.h"
#include "TabToolBar.h"


// CTabToolBar

IMPLEMENT_DYNAMIC(CTabToolBar, CControlBar)
CTabToolBar::CTabToolBar()
{
}

CTabToolBar::~CTabToolBar()
{
}


BEGIN_MESSAGE_MAP(CTabToolBar, CControlBar)
END_MESSAGE_MAP()



// CTabToolBar 消息处理程序

void CTabToolBar::OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL /*bDisableIfNoHndler*/)
{
}
