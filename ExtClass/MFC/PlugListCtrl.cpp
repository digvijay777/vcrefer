// PlugListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PlugListCtrl.h"
#include ".\pluglistctrl.h"
#include "MemDC.h"
#include "CDC.h"

#define str_PlugCtrl_ItemText				"类型: \t%s\r\n出品公司: \t%s\r\n路径: \t%s"

#define COLOR_ITEM_0		RGB(0xf0, 0xf8, 0xfd)
#define COLOR_ITEM_1		RGB(0xff, 0xff, 0xff)
// CPlugListCtrl

IMPLEMENT_DYNAMIC(CPlugListCtrl, CListBox)
CPlugListCtrl::CPlugListCtrl()
{
	m_nItemHeight = 80;
	m_bShowPlug = TRUE;
}

CPlugListCtrl::~CPlugListCtrl()
{
}


BEGIN_MESSAGE_MAP(CPlugListCtrl, CListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void CPlugListCtrl::PreSubclassWindow()
{
	CListBox::PreSubclassWindow();
	ModifyListStyle();
}

BOOL CPlugListCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL		bRet		= CListBox::PreCreateWindow(cs);

	ModifyListStyle();

	return bRet;
}

void CPlugListCtrl::ModifyListStyle()
{
		
}

// 设置状态图标
void CPlugListCtrl::SetImageList(UINT nIDState /* = -1 */, UINT nIDPrompt /* = -1 */)
{
	CBitmap			bmp;
	BITMAP			bit		= {0};

	if(-1 != nIDState)
	{
		if(NULL != m_imgState.GetSafeHandle())
			m_imgState.DeleteImageList();
		
		bmp.LoadBitmap(nIDState);
		bmp.GetBitmap(&bit);
		
		m_imgState.Create(bit.bmHeight, bit.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
		m_imgState.Add(&bmp, RGB(0xff, 0x0, 0xff));
        
		bmp.DeleteObject();
	}
	if(-1 != nIDPrompt)
	{
		if(NULL != m_imgPrompt.GetSafeHandle())
			m_imgPrompt.DeleteImageList();

		bmp.LoadBitmap(nIDPrompt);
		bmp.GetBitmap(&bit);

		m_imgPrompt.Create(bit.bmHeight, bit.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
		m_imgPrompt.Add(&bmp, RGB(0xff, 0x0, 0xff));

		bmp.DeleteObject();
	}
}

// 选择全部
void CPlugListCtrl::CheckAll(BOOL bChecked /* = TRUE */)
{
	for(int i = 0; i < m_vctPlug.size(); i++)
	{
		m_vctPlug[i].bChecked = bChecked;
	}

	Invalidate();
}

// 得到插件状态
BOOL CPlugListCtrl::GetPlugChecked(int nIndex)
{
	if(nIndex < 0 || nIndex > (m_vctPlug.size() - 1))
		return FALSE;

	return m_vctPlug[nIndex].bChecked;
}
void CPlugListCtrl::AddPlug(LPCSTR lpName, LPCSTR lpPath, LPCSTR lpType, LPCSTR lpCompany)
{
	PLUGDATA		data		= {0};

	if(FALSE == m_bShowPlug)
		ResetContent();

	data.strTitle = lpName;
	data.strCompany = lpCompany;
	data.strPath = lpPath;
	data.strType = lpType;

	m_vctPlug.push_back(data);
	int		nIndex	= AddString("");

	m_bShowPlug = TRUE;
	SetItemHeight(nIndex, m_nItemHeight);
}
// 添加提示
void CPlugListCtrl::AddPrompt(LPCSTR lpText, LPCSTR lpText2 /* = NULL */, int nImg /* = 0 */, COLORREF col /* = RGB */)
{
	m_vctPlug.clear();
	ResetContent();

	m_bShowPlug = FALSE;
	m_pdPrompt.nColor = col;
	m_pdPrompt.nImage = nImg;
	m_pdPrompt.strText = lpText;
	m_pdPrompt.strText2 = (NULL != lpText2)?lpText2:"";

	AddString("");
}

void CPlugListCtrl::ClearPlug()
{
	m_bShowPlug = TRUE;
	m_vctPlug.clear();
	ResetContent();
}

void CPlugListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(FALSE != m_bShowPlug)
	{
		if(lpDrawItemStruct->itemID >= m_vctPlug.size())
			return;

		DrawItem_ShowPlugList(lpDrawItemStruct);
	}
	else
	{
		DrawItem_ShowPrompt(lpDrawItemStruct);
	}
}

// 绘制列表
void CPlugListCtrl::DrawItem_ShowPlugList(LPDRAWITEMSTRUCT pItem)
{
	CRect			rtItem(pItem->rcItem);
	PLUGDATA&		plug							= m_vctPlug[pItem->itemID];
	CDC*			pDC								= CDC::FromHandle(pItem->hDC);
	CMemDC			memDC(pDC, rtItem);
	CRect			rtMem;

	rtMem = rtItem;
	rtMem.MoveToXY(0, 0);

	// 背景
	memDC.FillSolidRect(&rtMem, (pItem->itemID % 2 == 0)?COLOR_ITEM_0:COLOR_ITEM_1);
	// 状态图标
	rtMem.DeflateRect(10, 10, 10, 10);
	if(NULL != m_imgState.GetSafeHandle())
	{
		m_imgState.Draw(&memDC, plug.bChecked, rtMem.TopLeft(), ILD_TRANSPARENT);
	}
	// 标题
	CFont					ftTitle;
	CFont*					pftItem;
	LOGFONT					logFont			= {0};
	CRect					rtText;
	TEXTMETRIC				txtMetric		= {0};
	IMAGEINFO				info			= {0};
	CFont*					pOldDCFont		= NULL;

	m_imgState.GetImageInfo(0, &info);
	rtText = rtMem;
	pftItem = GetFont();
	pftItem->GetLogFont(&logFont);
	logFont.lfWeight = 700;
	ftTitle.CreateFontIndirect(&logFont);
	pOldDCFont = memDC.SelectObject(&ftTitle);
	memDC.GetTextMetrics(&txtMetric);
	rtText.bottom = rtText.top + min(txtMetric.tmHeight, info.rcImage.bottom - info.rcImage.top) + 5;
	rtText.left += info.rcImage.right - info.rcImage.left + 5;
	memDC.DrawText(plug.strTitle, &rtText, DT_LEFT | DT_SINGLELINE);
	// 出品公司
	CString			strText;

	memDC.SelectObject(pftItem);
	rtText.top = rtText.bottom;
	strText.Format(str_PlugCtrl_ItemText, plug.strType.GetBuffer(), 
		plug.strCompany.GetBuffer(), 
		plug.strPath.GetBuffer());
	rtText.bottom = rtMem.bottom;
	// 计算文本是否超出
	CSize			txtSize			= memDC.GetTextExtent(strText);
	if(txtSize.cx > GetHorizontalExtent())
		SetHorizontalExtent(txtSize.cx+1);

	rtText.right = rtText.left + GetHorizontalExtent();
	memDC.DrawText(strText, &rtText, DT_LEFT);

	memDC.SelectObject(pOldDCFont);
}

// 绘制提示
void CPlugListCtrl::DrawItem_ShowPrompt(LPDRAWITEMSTRUCT pItem)
{
	CDC*			pDC		= CDC::FromHandle(pItem->hDC);
	CRect			rtClient;
	GetClientRect(&rtClient);
	CMemDC			memDC(pDC, rtClient);
	CFont*			pOldFont;
	CFont			ftText;
	LOGFONT			logFont;
	CRect			rtIcon;
	CRect			rtText;
	CSize			szTxt;
	int				nTop		= 50;

	memDC.FillSolidRect(&rtClient, RGB(0xff, 0xff, 0xff));
	GetFont()->GetLogFont(&logFont);
	logFont.lfWeight = 700;
	logFont.lfHeight = -MulDiv(18, GetDeviceCaps(pItem->hDC, LOGPIXELSY), 72);
	ftText.CreateFontIndirect(&logFont);
	pOldFont = memDC.SelectObject(&ftText);
	memDC.SetTextColor(m_pdPrompt.nColor);
	memDC.SetBkMode(TRANSPARENT);
	szTxt = memDC.GetTextExtent(m_pdPrompt.strText);
	rtText = rtClient;
	rtText.top = rtClient.top + nTop - szTxt.cy / 2;
	if(NULL != m_imgPrompt.GetSafeHandle())
	{
		IMAGEINFO			info;

		m_imgPrompt.GetImageInfo(0, &info);
		rtIcon = info.rcImage;
		rtIcon.MoveToX(rtClient.left + (rtClient.Width() - rtIcon.Width() - szTxt.cx) / 2);
		rtIcon.MoveToY(rtClient.top + nTop - rtIcon.Height() / 2);
		rtText.left = rtIcon.right;

		m_imgPrompt.Draw(&memDC, m_pdPrompt.nImage, rtIcon.TopLeft(), ILD_TRANSPARENT);
	}
	else
	{
		rtText.left = rtClient.left + (rtClient.Width() - szTxt.cx) / 2;
	}
	memDC.DrawText(m_pdPrompt.strText, &rtText, DT_SINGLELINE | DT_LEFT | DT_TOP);
	// 辅助标题
	if("" != m_pdPrompt.strText2)
	{
		rtText.top += szTxt.cy+5;
		memDC.SelectObject(GetFont());
		memDC.SetTextColor(0);
		memDC.DrawText(m_pdPrompt.strText2, &rtText, DT_SINGLELINE | DT_LEFT | DT_TOP);
	}

	memDC.SelectObject(pOldFont);
}

// 得到状态图标位置
CRect CPlugListCtrl::CalCheckBoxRect(CRect& itRect)
{
	CRect		rect;
	IMAGEINFO	info			= {0};

	m_imgState.GetImageInfo(0, &info);
	rect = info.rcImage;
	rect.MoveToXY(itRect.left+10, itRect.top+10);
	return rect;
}

// 左键接下
void CPlugListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL	bBOOL;
	int nIndex = ItemFromPoint(point,bBOOL);
	if(nIndex == 0xFFFF)
		return ;

// 	m_bMouseDown = TRUE;
	CRect	rect;
	GetItemRect(nIndex,rect);

	CRect rectCheckIcon = CalCheckBoxRect(rect);
	
	if(rectCheckIcon.PtInRect(point))
	{
		PLUGDATA&		plug		= m_vctPlug[nIndex];

		plug.bChecked = !plug.bChecked;
		Invalidate();
	}

	SetFocus();
	// CListBox::OnLButtonDown(nFlags, point);
}

BOOL CPlugListCtrl::OnEraseBkgnd(CDC* pDC)
{
	if(FALSE == m_bShowPlug)
		return FALSE;

	CRect		rtItems;
	CRect		rtClient;

	GetClientRect(&rtClient);
	if(LB_ERR == GetItemRect(GetCount() - 1, &rtItems))
		return FALSE;
	if(rtItems.bottom >= rtClient.bottom)
		return FALSE;

	rtClient.top = rtItems.bottom;
	pDC->FillSolidRect(&rtClient, RGB(0xff, 0xff, 0xff));
 	return TRUE;

	return CListBox::OnEraseBkgnd(pDC);
}

void CPlugListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	
	OnLButtonDown(nFlags, point);
	// CListBox::OnLButtonDblClk(nFlags, point);
}
