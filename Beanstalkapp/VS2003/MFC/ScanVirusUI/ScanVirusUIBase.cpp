// ScanVirusUIBase.cpp : implementation file
//

#include "stdafx.h"
#include "ScanVirusUI.h"
#include "ScanVirusUIBase.h"
#include ".\scanvirusuibase.h"

#define IMAGE_TITLEBK_HEIGHT		27	//图片高度为27像素
#define TITLE_BUTTON_Y				4 //关闭、最大化按钮的上边界
#define TITLE_BUTTON_X				7 //关闭、最大化按钮的右边界
#define FRAME_BORDER_BUTTON			2 //下边界
#define FRAME_BORDER_LEFT			2 //左边界
#define FRAME_BORDER_RIGHT			2 //右边界

#define STRING_WNDPROCPROP			_T("SkyWnd.IFancyWnd.WindowProc")	//窗口属性
#define STRING_WNDSTYLEPROP			_T("SkyWnd.IFancyWnd.Style")		//窗口原来属性
#define STRING_BUTTONPROCPROP		_T("SkyWnd.IFancyButton.WindowProc")	//按钮是否子类化

#define UM_ATTACHWINDOW				WM_APP - 100	//用户使用Attach
#define UM_DETACHWINDOW				WM_APP - 99		//用户使用Detach

#define IDM_MINIMIZE			0x100			//最小化
#define IDM_REBACK				0x101			//还原
#define IDM_CLOSE				0x102			//关闭
#define IDM_ABOUT				0x103			//关于
#define IDM_MOVE				0x104			//移动
// CScanVirusUIBase dialog

IMPLEMENT_DYNAMIC(CScanVirusUIBase, CDialog)
CScanVirusUIBase::CScanVirusUIBase(UINT nIDTemplate, CWnd* pParent /* = NULL */)
	: CDialog(nIDTemplate, pParent)
{
}

CScanVirusUIBase::~CScanVirusUIBase()
{
}

void CScanVirusUIBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScanVirusUIBase, CDialog)
END_MESSAGE_MAP()


LRESULT CScanVirusUIBase::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 美化界面
	switch(message)
	{
	case WM_NCPAINT:
	case WM_NCACTIVATE:
		if(DrawTitleBar())
			return 1;
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

bool CScanVirusUIBase::DrawTitleBar()
{
	CDC		*pdc	= GetWindowDC();

	if(NULL == pDC)
		return false;
	//////////////////////////////////////////////////////////////////////////////////////
	//开始绘制标题栏
	CRect mainRect;
	GetWindowRect(&mainRect);	
	RECT	tlRect			= {0, 0, mainRect.Width(), IMAGE_TITLEBK_HEIGHT};
	RECT	tlTxtRect		= {28, 0, mainRect.Width() - 80, IMAGE_TITLEBK_HEIGHT};
	CDC		tlmemdc;
	CBitmap tlbit, *ptlbit;
	tlmemdc.CreateCompatibleDC(pdc);
	//绘制背景
	tlbit.LoadBitmap(m_nIDTitleBarBkimg);
	ptlbit = tlmemdc.SelectObject(&tlbit);
	//绘制图标
	::DrawIconEx(tlmemdc.m_hDC, 9, (IMAGE_TITLEBK_HEIGHT - 16) / 2, pWnd->GetIcon(false), 16, 16, 0,NULL,DI_NORMAL); 
	tlmemdc.SetBkMode(TRANSPARENT);
	tlmemdc.SetTextColor(RGB(0xff, 0xff, 0xff));
	CString title;
	GetWindowText(title);
	if(title.GetLength() == 0)
		GetWindowText(title);
	//绘制标题
	CFont		font;
	CFont		*pOldFont	= NULL;
	font.CreatePointFont(110, _T("宋体"), &tlmemdc);
	pOldFont = tlmemdc.SelectObject(&font);
	tlmemdc.DrawText(title, &tlTxtRect, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS); 
	tlmemdc.SelectObject(pOldFont);
	font.DeleteObject();
	//绘制系统按钮
	DrawSystemButton(3, 0, &tlmemdc, tlRect);
	//完成标题栏绘制
	pdc->BitBlt(0, 0, tlRect.right - tlRect.left, tlRect.bottom - tlRect.top, &tlmemdc, 0, 0, SRCCOPY);
	tlmemdc.SelectObject(ptlbit);
	tlbit.DeleteObject();
	tlmemdc.DeleteDC();
	///////////////////////////////////////////////////////////////////////////////////////
	//绘制左边
	CBrush brush;
	brush.CreateSolidBrush(RGB(0x99, 0x99, 0x99));
	RECT lfRect = {0, IMAGE_TITLEBK_HEIGHT, FRAME_BORDER_LEFT, mainRect.Height()};
	pdc->FillRect(&lfRect, &brush);
	//绘制下边
	RECT btRect = {0, mainRect.Height() - FRAME_BORDER_BUTTON, mainRect.Width(), mainRect.Height()};
	pdc->FillRect(&btRect, &brush);
	//绘制右边
	RECT rtRect = {mainRect.Width() - FRAME_BORDER_RIGHT, IMAGE_TITLEBK_HEIGHT, mainRect.Width(), mainRect.Height()};
	pdc->FillRect(&rtRect, &brush);
	brush.DeleteObject();

	::ReleaseDC(pWnd->m_hWnd, pdc->m_hDC);
	return true;
}
