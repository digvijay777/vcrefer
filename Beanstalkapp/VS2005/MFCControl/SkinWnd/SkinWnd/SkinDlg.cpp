// SkinWnd\SkinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SkinDlg.h"


// CSkinDlg dialog

IMPLEMENT_DYNAMIC(CSkinDlg, CDialog)

CSkinDlg::CSkinDlg(UINT nIDTemplate, CWnd* pParent /* = NULL */)
	: CDialog(nIDTemplate, pParent)
{
	m_bTrackMouseEvent = FALSE;
	m_colorTitle = 0x0;
	m_bNcCapture = FALSE;
}

CSkinDlg::~CSkinDlg()
{
	for(int i = 0; i < (int)m_SkinControl.size(); i++)
	{
		if(NULL != m_SkinControl[i])
			delete m_SkinControl[i];
	}
}

BEGIN_MESSAGE_MAP(CSkinDlg, CDialog)
	ON_WM_NCPAINT()
	ON_WM_NCHITTEST()
	ON_WM_NCACTIVATE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_SIZE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_NCMOUSELEAVE, OnNcMouseLeave)
	ON_MESSAGE(0x313, On0x313)
	ON_WM_NCRBUTTONUP()
	ON_WM_NCRBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CSkinDlg message handlers

void CSkinDlg::OnNcPaint()
{
	// ʹ��ԭ����
	if(NULL == m_bmpBk.GetSafeHandle())
	{
		CDialog::OnNcPaint();
		return;
	}
	// ��ʼ����
	CDC*			pDC			= GetWindowDC();
	CRect			rect;
	CRect			rtBk;
	CDC				memDC;
	CBitmap			memBmp;
	int				nMemSaveID;
	CSize			sizeBk;
	CBitmapDC		bkDC((HBITMAP)m_bmpBk.GetSafeHandle());
	CRect			rtClient;
	BITMAP			bInfo;
	
	m_bmpBk.GetBitmap(&bInfo);
	rtBk.SetRect(0, 0, bInfo.bmWidth, bInfo.bmHeight);
	sizeBk.SetSize(rtBk.Width() / 3, rtBk.Height() / 3);
	GetWindowRect(&rect);
	GetClientRect(&rtClient);
	ClientToScreen(&rtClient);

	memDC.CreateCompatibleDC(pDC);
	memBmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	nMemSaveID = memDC.SaveDC();
	memDC.SelectObject(&memBmp);
	// ��ʼ���Ʊ߿�
	for(int i = rtBk.Width() / 3; i < (rect.Width() - rtBk.Width() / 3); i += rtBk.Width() / 3)
	{
		// ��
		memDC.BitBlt(i, 0, sizeBk.cx, sizeBk.cy
			, &bkDC, rtBk.left + sizeBk.cx, rtBk.top, SRCCOPY);
		// ��
		for(int k = rtBk.Height() / 3; k < (rect.Height() - rtBk.Height() / 3); k += rtBk.Height() / 3)
		{
			memDC.BitBlt(i, k, sizeBk.cx, sizeBk.cy
				, &bkDC, rtBk.left + sizeBk.cx, rtBk.top + sizeBk.cy, SRCCOPY);
		}
		// ��
		memDC.BitBlt(i, rect.Height() - sizeBk.cy, sizeBk.cx, sizeBk.cy
			, &bkDC, rtBk.left + sizeBk.cx, rtBk.top + sizeBk.cy * 2, SRCCOPY);
	}
	for(int i = rtBk.Height() / 3; i < (rect.Height() - rtBk.Height() / 3); i += rtBk.Height() / 3)
	{
		// ��
		memDC.BitBlt(0, i, sizeBk.cx, sizeBk.cy
			, &bkDC, rtBk.left, rtBk.top + sizeBk.cy, SRCCOPY);
		// ��
		memDC.BitBlt(rect.Width() - sizeBk.cx, i, sizeBk.cx, sizeBk.cy
			, &bkDC, rtBk.left + sizeBk.cx * 2, rtBk.top + sizeBk.cy, SRCCOPY);
	}
	// �ĽǵĻ���
	memDC.BitBlt(0, 0, sizeBk.cx, sizeBk.cy, &bkDC, rtBk.left, rtBk.top, SRCCOPY);
	memDC.BitBlt(rect.Width() - sizeBk.cx, 0, sizeBk.cx, sizeBk.cy
		, &bkDC, rtBk.left + sizeBk.cx * 2, rtBk.top, SRCCOPY);
	memDC.BitBlt(rect.Width() - sizeBk.cx, rect.Height() - sizeBk.cy, sizeBk.cx, sizeBk.cy
		, &bkDC, rtBk.left + sizeBk.cx * 2, rtBk.top + sizeBk.cy * 2, SRCCOPY);
	memDC.BitBlt(0, rect.Height() - sizeBk.cy, sizeBk.cx, sizeBk.cy
		, &bkDC, rtBk.left, rtBk.top + sizeBk.cy * 2, SRCCOPY);
	// ���ƿ��ư�ť
	CRect		rtTitleBar(0, 0, rect.Width(), rtClient.top - rect.top);

	Draw_ControlBox(&memDC, rtTitleBar);
	Draw_TitleBar(&memDC, rtTitleBar);
	// ���ƴ���-��ֹ���ƿͻ���
	CRgn		rgn, rgn2;
	INT			nSaveDC			= pDC->SaveDC();

	rtClient.MoveToXY(rtClient.left - rect.left, rtClient.top - rect.top);
	rect.MoveToXY(0, 0);
	rgn.CreateRectRgnIndirect(&rect);
	rgn2.CreateRectRgnIndirect(&rtClient);
	rgn.CombineRgn(&rgn, &rgn2, RGN_DIFF);
	pDC->SelectClipRgn(&rgn);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	pDC->RestoreDC(nSaveDC);

	memDC.RestoreDC(nMemSaveID);

	ReleaseDC(pDC);
}
// ���Ʊ�����
void CSkinDlg::Draw_TitleBar(CDC* pDC, CRect& rect)
{
	HICON		hIcon				= GetIcon(FALSE);
	CRect		rt					= rect;
	TCHAR		szTitle[1024]		= {0};

	rt.left += 5;
	GetWindowText(szTitle, 1024);
	if(NULL != hIcon)
	{
		// ����ͼ��
		DrawIconEx(pDC->GetSafeHdc(), rt.left, rt.Height() / 2 - 8, hIcon, 16, 16, 0, NULL, DI_NORMAL);
		rt.left += 20;
	}
	// ���Ʊ���
	pDC->SelectObject(GetFont());
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_colorTitle);
	pDC->DrawText(szTitle, _tcslen(szTitle), &rt, DT_SINGLELINE|DT_VCENTER|DT_WORDBREAK);
}
// ���ƿ��ư�
void CSkinDlg::Draw_ControlBox(CDC* pDC, CRect& rect)
{
	for(int i = 0; i < (int)m_SkinControl.size(); i++)
	{
		if(NULL == m_SkinControl[i])
			continue;
		
		m_SkinControl[i]->CalcSize(rect);
		m_SkinControl[i]->Draw(pDC);
	}
}

LRESULT CSkinDlg::OnNcHitTest(CPoint point)
{
	CRect		rect;

	GetWindowRect(rect);
	rect.top += GetSystemMetrics(SM_CYFRAME);
	rect.bottom = rect.top + GetSystemMetrics(SM_CYCAPTION);
	rect.DeflateRect(GetSystemMetrics(SM_CYFRAME), 0, GetSystemMetrics(SM_CYFRAME), 0);
	if(rect.PtInRect(point))
		return HTCAPTION;

	return CDialog::OnNcHitTest(point);
}

BOOL CSkinDlg::OnNcActivate(BOOL bActive)
{
	OnNcPaint();

	return TRUE;
	// return CDialog::OnNcActivate(bActive);
}

void CSkinDlg::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CSkinDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(HTCAPTION != nHitTest)
	{
		CDialog::OnNcLButtonDown(nHitTest, point);
		return;
	}
	
	BOOL		bInline		= FALSE;
	CPoint		ptSave		= point;

	ScreenToWindow(&ptSave);
	for(int i = 0; i < (int)m_SkinControl.size(); i++)
	{
		if(m_SkinControl[i]->OnLButtonDown(ptSave))
		{
			bInline = TRUE;
			m_bNcCapture = TRUE;
		}
	}
	
	if(bInline)
	{
		OnNcPaint();
		return;
	}
	CDialog::OnNcLButtonDown(nHitTest, point);
}

void CSkinDlg::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	BOOL		bInline		= FALSE;
	CPoint		ptSave		= point;

	ScreenToWindow(&ptSave);
	for(int i = 0; i < (int)m_SkinControl.size(); i++)
	{
		if(m_SkinControl[i]->OnLButtonUp(ptSave))
			bInline = TRUE;
	}

	m_bNcCapture = FALSE;
	if(bInline)
	{
		OnNcPaint();
		return;
	}
	CDialog::OnNcLButtonUp(nHitTest, point);
}

void CSkinDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(FALSE != m_bNcCapture)
	{
		// �ǿͻ���
		ClientToScreen(&point);
		CSkinDlg::OnNcLButtonUp(CSkinDlg::OnNcHitTest(point), point);
		return;
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CSkinDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(HTCAPTION != nHitTest)
	{
		CDialog::OnNcMouseMove(nHitTest, point);
		return;
	}
	// ע���˳�ʱ�¼�
	if(FALSE == m_bTrackMouseEvent)
	{
		TRACKMOUSEEVENT		event;

		event.cbSize = sizeof(event);
		event.dwFlags = TME_NONCLIENT | TME_LEAVE;
		event.hwndTrack = GetSafeHwnd();
		event.dwHoverTime = 1000;
		m_bTrackMouseEvent = TrackMouseEvent(&event);
	}

	BOOL		bInvalude		= FALSE;

	ScreenToWindow(&point);
	for(int i = 0; i < (int)m_SkinControl.size(); i++)
	{
		if(m_SkinControl[i]->OnMouseMove(point))
			bInvalude = TRUE;
	}

	if(bInvalude)
		OnNcPaint();
}

void CSkinDlg::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
	if(HTCAPTION != nHitTest)
	{
		CDialog::OnNcRButtonDown(nHitTest, point);
		return;
	}

	CPoint		ptSave		= point;

	ScreenToWindow(&ptSave);
	for(int i = 0; i < (int)m_SkinControl.size(); i++)
	{
		if(m_SkinControl[i]->m_rawRect.PtInRect(ptSave))
		{
			CDialog::OnNcRButtonDown(nHitTest, point);
			return;
		}
	}
}

void CSkinDlg::OnNcRButtonUp(UINT nHitTest, CPoint point)
{
	// CDialog::OnNcRButtonUp(nHitTest, point);

	if(HTCAPTION != nHitTest)
		return;
	// �����Ҽ��˵�
	CMenu*		pMenu	= GetSystemMenu(FALSE);
	DWORD		dwRet;

	if(NULL != pMenu)
	{
		DWORD			dwFlag		= 0;
		DWORD			dwDisable	= MF_BYCOMMAND|MF_DISABLED|MF_GRAYED;
		DWORD			dwEnable	= MF_BYCOMMAND|MF_ENABLED;
		// ���ò˵���
		if(IsIconic())
		{
			dwFlag |= 0x1;
		}
		else if(IsZoomed())
		{
			dwFlag |= 0x2;
		}

		pMenu->EnableMenuItem(SC_MINIMIZE, (0x1 & dwFlag)?dwDisable:dwEnable);
		pMenu->EnableMenuItem(SC_MAXIMIZE, ((0x2 & dwFlag) == 0x2)?dwDisable:dwEnable);
		pMenu->EnableMenuItem(SC_MOVE, (0x3 & dwFlag)?dwDisable:dwEnable);
		pMenu->EnableMenuItem(SC_SIZE, (0x3 & dwFlag)?dwDisable:dwEnable);
		pMenu->EnableMenuItem(SC_RESTORE, (0 == dwFlag)?dwDisable:dwEnable);
		// ��ʾ�˵�
		OnNcPaint();
		dwRet = pMenu->TrackPopupMenu(TPM_RETURNCMD | TPM_NONOTIFY, point.x, point.y, this);
		if(0 != dwRet)
			PostMessage(WM_SYSCOMMAND, dwRet, 0);
	}
}

LRESULT CSkinDlg::On0x313(WPARAM wParam, LPARAM lParam)
{
	CPoint		point;

	GetCursorPos(&point);
	CSkinDlg::OnNcRButtonUp(HTCAPTION, point);
	return 0;
}

void CSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	DoCalcWindowRect();

}

LRESULT CSkinDlg::OnNcMouseLeave(WPARAM wParam, LPARAM lParam)
{
	BOOL		bInvalude		= FALSE;
	CPoint		point(-1, -1);

	for(int i = 0; i < (int)m_SkinControl.size(); i++)
	{
		if(m_SkinControl[i]->OnMouseMove(point))
			bInvalude = TRUE;
	}

	if(bInvalude)
		OnNcPaint();
	m_bTrackMouseEvent = FALSE;
	return 0;
}

BOOL CSkinDlg::OnEraseBkgnd(CDC* pDC)
{
	// ʹ��ԭ����
	if(NULL == m_bmpBk.GetSafeHandle())
	{
		CDialog::OnNcPaint();
		return CDialog::OnEraseBkgnd(pDC);
	}
	// ��ʼ����
	CRect			rect;
	CDC				memDC;
	CBitmap			memBmp;
	int				nMemSaveID;
	CSize			sizeBk;
	CBitmapDC		bkDC((HBITMAP)m_bmpBk.GetSafeHandle());
	BITMAP			bInfo;

	m_bmpBk.GetBitmap(&bInfo);
	sizeBk.SetSize(bInfo.bmWidth / 3, bInfo.bmHeight / 3);
	GetClientRect(&rect);

	memDC.CreateCompatibleDC(pDC);
	memBmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	nMemSaveID = memDC.SaveDC();
	memDC.SelectObject(&memBmp);
	// ��ʼ����
	for(int i = 0; i < (rect.Width() + sizeBk.cx - 1); i += sizeBk.cx)
	{
		for(int k = 0; k < (rect.Height() + sizeBk.cy - 1); k += sizeBk.cy)
		{
			memDC.BitBlt(i, k, sizeBk.cx, sizeBk.cy, &bkDC, sizeBk.cx, sizeBk.cy, SRCCOPY);
		}
	}
	// ���ƴ���-��ֹ���ƿͻ���
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.RestoreDC(nMemSaveID);
	return FALSE;/*CDialog::OnEraseBkgnd(pDC)*/;
}


BOOL CSkinDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ȥ��ϵͳ�˵�ѡ��
	ModifyStyle(WS_SYSMENU, 0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CSkinDlg::ScreenToWindow(CPoint* pPoint)
{
	CRect		rect;

	GetWindowRect(&rect);
	pPoint->x = pPoint->x - rect.left;
	pPoint->y = pPoint->y - rect.top;
}

// ����ͼƬ
BOOL CSkinDlg::LoadImages(INT nIDBk)
{
	CBitmap			bmp;

	if(FALSE == bmp.LoadBitmap(nIDBk))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return LoadBitmaps((HBITMAP)bmp.Detach());
}
BOOL CSkinDlg::LoadBitmaps(HBITMAP hBmpBk)
{
	ASSERT(NULL != hBmpBk);
	// ɾ��ԭͼƬ
	if(NULL != m_bmpBk.GetSafeHandle())
		m_bmpBk.DeleteObject();
	// ����ͼƬ
	if(FALSE == m_bmpBk.Attach(hBmpBk))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	// ����߿�
	DoCalcWindowRect();

	return TRUE;
}
// ���㴰���С
BOOL CSkinDlg::DoCalcWindowRect()
{
	if(NULL == m_bmpBk.GetSafeHandle())
		return FALSE;
	if(NULL == GetSafeHwnd())
		return FALSE;

	BITMAP			bInfo		= {0};
	BYTE*			pBit		= NULL;
	DWORD			dwBits		= 0;
	int				nStep		= 4;
	CRect			rtBk;

	m_bmpBk.GetBitmap(&bInfo);
	rtBk.SetRect(0, 0, bInfo.bmWidth, bInfo.bmHeight);
	dwBits = bInfo.bmWidthBytes * bInfo.bmHeight;
	pBit = (BYTE *)malloc(dwBits);
	if(NULL == pBit)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(dwBits != m_bmpBk.GetBitmapBits(dwBits, pBit))
	{
		ASSERT(FALSE);
		free(pBit);
		return FALSE;
	}
	nStep = bInfo.bmWidthBytes / bInfo.bmWidth;
	// ��ʼ�ĽǴ���
	CRgn		rgn;
	CRect		rect;

	GetWindowRect(&rect);
	if(FALSE == rgn.CreateRectRgn(0, 0, rect.Width(), rect.Height()))
	{
		ASSERT(FALSE);
		free(pBit);
		return FALSE;
	}
	for(int t = 0; t < rtBk.Height(); t++)
	{
		int		h = rtBk.Height() - t - 1;
		// ��ʼ����
		for(int l = 0; l < rtBk.Width(); l++)
		{
			CRect		rtTran;
			DWORD		dwCol			= 0;

			dwCol = *((DWORD *)(pBit + (rtBk.top + t) * bInfo.bmWidthBytes + (rtBk.left + l) * nStep));
			dwCol = dwCol & 0x00FFFFFF;
			if(0xff00ff != dwCol)
				continue;
			if(h < (rtBk.Height() / 3))
			{
				// �ϱ�
				if(l < (rtBk.Width() / 3))
				{
					// ��-��
					rtTran.SetRect(l, h, l + 1, h + 1);
				}
				else if(l > (rtBk.Width() * 2 / 3))
				{
					// ��-��
					rtTran.SetRect(rect.Width() - rtBk.Width() + l, h
						, rect.Width() - rtBk.Width() + l + 1, h + 1);
				}
				else
				{
					continue;
				}
			}
			else if(h > (rtBk.Height() * 2 / 3))
			{
				// �±�
				if(l < (rtBk.Width() / 3))
				{
					// ��-��
					rtTran.SetRect(l, rect.Height() - rtBk.Height() + h
						, l + 1, rect.Height() - rtBk.Height() + h + 1);
				}
				else if(l >(rtBk.Width() * 2 / 3))
				{
					// ��-��
					rtTran.SetRect(rect.Width() - rtBk.Width() + l
						, rect.Height() - rtBk.Height() + h
						, rect.Width() - rtBk.Width() + l + 1
						, rect.Height() - rtBk.Height() + h + 1);
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}
			// ��������
			CRgn	rgnTmp;

			if(rgnTmp.CreateRectRgnIndirect(&rtTran))
				rgn.CombineRgn(&rgn, &rgnTmp, RGN_DIFF);
		}
	}
	SetWindowRgn((HRGN)rgn.GetSafeHandle(), TRUE);
	free(pBit);
	return TRUE;
}
// ���ư�ť
BOOL CSkinDlg::ContorlButton(INT nID, HBITMAP hBitmap, int nRight, int nTop, BOOL bAdd)
{
	if(NULL == hBitmap && bAdd != FALSE)
		return FALSE;

	std::vector<CSkinNcControl*>::iterator		it;
	for(it = m_SkinControl.begin(); it != m_SkinControl.end(); it++)
	{
		if(nID == (*it)->m_ID)
		{
			if(bAdd)
			{
				ASSERT(FALSE);
				return FALSE;
			}

			delete *it;
			m_SkinControl.erase(it);
			return TRUE;
		}
	}
	// �������
	CSkinNcButton*		pBtn		= new CSkinNcButton();

	pBtn->Create(GetSafeHwnd(), nID, -nRight, nTop, 0, 0);
	pBtn->SetBitmap(hBitmap);
	m_SkinControl.push_back(pBtn);

	return FALSE;
}
BOOL CSkinDlg::AddContorlButton(INT nID, INT nBtnBmpID, int nRight, int nTop)
{
	CBitmap		bmp;

	bmp.LoadBitmap(nBtnBmpID);
	
	return ContorlButton(nID, (HBITMAP)bmp.Detach(), nRight, nTop, TRUE);
}
BOOL CSkinDlg::DelContorlButton(int nID)
{
	return ContorlButton(nID, NULL, 0, 0, FALSE);
}
void CSkinDlg::SetTextColor(COLORREF col)
{
	m_colorTitle = col;
}
//////////////////////////////////////////////////////////////////////////
// 
CBitmapDC::CBitmapDC(HBITMAP hBitmap)
{
	m_hBmp = NULL;
	m_nSaveID = 0;
	CreateFromBitmap(hBitmap);
}

CBitmapDC::~CBitmapDC()
{
	if(GetSafeHdc() && NULL != m_hBmp)
	{
		SelectObject(m_hBmp);
		RestoreDC(m_nSaveID);
		DeleteDC();
	}
}

BOOL CBitmapDC::CreateFromBitmap(HBITMAP hBitmap)
{
	CDC*		pDC	= CDC::FromHandle(GetDC(NULL));

	CreateCompatibleDC(pDC);
	m_hBmp = (HBITMAP)SelectObject((HGDIOBJ)hBitmap);
	m_nSaveID = SaveDC();
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//
CSkinNcControl::CSkinNcControl()
{
	m_rawRect.SetRect(0, 0, 0, 0);
	m_ID = 0;
	m_lpData = NULL;
	m_bMouseOver = FALSE;
	m_bCapture = FALSE;
	m_bDisabled = FALSE;
	m_hWnd = NULL;
}

CSkinNcControl::~CSkinNcControl()
{

}
/*
 *	�����ؼ�
 *  ����˵��: nID, �ؼ�ID
 *           rl, ��Ժ���λ�ã� ��N��ʾ�������N������, ��N��ʾ�����ұ�N������, 0��ʾ�����λ�ã������
 *           rt, �������λ�ã� ��N��ʾ�����ϱ�N�����أ���N��ʾ�����ұ�N������, 0��ʾ�����λ�ã����ϱ�
 *			 nWidth, ���
 *           nHeight, �߶�
 */
BOOL CSkinNcControl::Create(HWND hWnd, DWORD nID, int rl, int rt, int nWidth, int nHeight)
{
	m_ID = nID;
	m_relPos.SetPoint(rl, rt);
	m_relSize.SetSize(nWidth, nHeight);
	m_hWnd = hWnd;

	return TRUE;
}

BOOL CSkinNcControl::CalcSize(CRect rtParent)
{
	m_rawRect.SetRect(0, 0, m_relSize.cx, m_relSize.cy);

	if(m_relPos.x < 0)
		m_rawRect.MoveToX(rtParent.right + m_relPos.x);
	else if(m_relPos.x > 0)
		m_rawRect.MoveToX(rtParent.left + m_relPos.x);
	if(m_relPos.y < 0)
		m_rawRect.MoveToY(rtParent.bottom + m_relPos.y);
	else if(m_relPos.y > 0)
		m_rawRect.MoveToY(rtParent.top + m_relPos.y);

	return FALSE;
}

BOOL CSkinNcControl::OnLButtonDown(CPoint point)
{
	m_bCapture = FALSE;
	if(m_rawRect.PtInRect(point))
	{
		m_bCapture = TRUE;
		SetCapture(m_hWnd);
		return TRUE;
	}

	return FALSE;
}

BOOL CSkinNcControl::OnLButtonUp(CPoint point)
{
	if(FALSE == m_bCapture)
		return FALSE;
	
	m_bCapture = FALSE;
	ReleaseCapture();
	if(m_rawRect.PtInRect(point))
	{
		if(SC_MAXIMIZE == m_ID && IsZoomed(m_hWnd))
		{
			PostMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		else
		{
			PostMessage(m_hWnd, WM_SYSCOMMAND, m_ID, 0);
		}
	}

	return TRUE;
}

BOOL CSkinNcControl::OnMouseMove(CPoint point)
{
	BOOL		bMouseOver		= FALSE;

	if(m_rawRect.PtInRect(point))
		bMouseOver = TRUE;
	if(bMouseOver != m_bMouseOver)
	{
		m_bMouseOver = bMouseOver;
		return TRUE;
	}
	return FALSE;
}

void CSkinNcControl::Draw(CDC* pDC)
{
	
}

//////////////////////////////////////////////////////////////////////////
// ��ť��
CSkinNcButton::CSkinNcButton()
{
}

CSkinNcButton::~CSkinNcButton()
{

}

BOOL CSkinNcButton::SetBitmap(HBITMAP hBitmap)
{
	CBitmap		bmp;
	BITMAP		bInfo;

	bmp.Attach(hBitmap);
	bmp.GetBitmap(&bInfo);
	if(NULL != m_imagelist.GetSafeHandle())
		m_imagelist.DeleteImageList();
	
	m_imagelist.Create(bInfo.bmWidth / 4, bInfo.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
	m_imagelist.Add(&bmp, RGB(0xff, 0x0, 0xff));
	m_relSize.SetSize(bInfo.bmWidth / 4, bInfo.bmHeight);

	return TRUE;
}

void CSkinNcButton::Draw(CDC* pDC)
{
	if(NULL == m_imagelist.GetSafeHandle())
		return ;

	int		nIndex		= 0;

	if(m_bMouseOver)
		nIndex++;
	if(m_bCapture)
		nIndex = 2;
	if(m_bDisabled)
		nIndex = 3;
	m_imagelist.Draw(pDC, nIndex, m_rawRect.TopLeft(), ILD_TRANSPARENT);
}

