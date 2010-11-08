
#include "stdafx.h"
#include "NMfc.h"
#include "NSimple.h"
#include "stdEx.h"


#define SPLITE_INIT_POS		9999999
#define SPLITE_COLOR		RGB(128, 0, 0)

////////////////////////////////////////////////////////////////////////////////
//	CWndAutoPos
CWndAutoPos::CWndAutoPos()
{
	m_nWidth = 0;
	m_nHeigh = 0;
	m_pParent = NULL;
}


CWndAutoPos::~CWndAutoPos()
{

}


BOOL	CWndAutoPos::Clear()
{

	return FALSE;
}


BOOL	CWndAutoPos::AddItem(int nID, BOOL bReDraw, int nPersentX, int nPersentY, int nPersentWidth, int nPersentHeigh)
{
	ITEM_DATA	OneItem;
	CWnd *		pItem;
	CRect		WindowRect;

	pItem = m_pParent->GetDlgItem(nID);
	pItem->GetWindowRect(WindowRect);
	m_pParent->ScreenToClient(WindowRect);

	OneItem.Rect = WindowRect;
	OneItem.nID = nID;
	OneItem.nPersentX = nPersentX;
	OneItem.nPersentY = nPersentY;
	OneItem.nPersentWidth = nPersentWidth;
	OneItem.nPersentHeigh = nPersentHeigh;
	OneItem.bReDraw = bReDraw;

	m_ItemList.push_back(OneItem);

	return TRUE;
}


BOOL	CWndAutoPos::InitOrgSize(CWnd *pParent)
{
	CRect			CltRect;

	m_pParent = pParent;
	pParent->GetClientRect(CltRect);
	m_nWidth = CltRect.Width();
	m_nHeigh = CltRect.Height();

	return TRUE;
}


BOOL	CWndAutoPos::FindItem(int nID, ITEM_DATA **ppItemData)
{
	std::list<ITEM_DATA>::iterator Iter;
	ITEM_DATA *		pItem;

	*ppItemData = NULL;

	for(Iter = m_ItemList.begin();
		Iter != m_ItemList.end();
		Iter ++)
	{
		pItem = &(*Iter);
		if(pItem->nID == nID)
		{
			*ppItemData = pItem;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL	CWndAutoPos::SaveItemChange(int nID, int nPersentX, int nPersentY, int nPersentWidth, int nPersentHeigh)
{
	ITEM_DATA	*pItem;
	BOOL		bRetVal;
	CRect		ParRect;
	CRect		ItemRect;
	int			dltx, dlty, dltw, dlth;

	bRetVal = FindItem(nID, &pItem);
	if(bRetVal == FALSE)
		return FALSE;

	m_pParent->GetWindowRect(ParRect);
	m_pParent->GetDlgItem(nID)->GetWindowRect(ItemRect);
	m_pParent->ScreenToClient(ItemRect);

	pItem->nPersentX = nPersentX;
	pItem->nPersentY = nPersentY;
	pItem->nPersentWidth = nPersentWidth;
	pItem->nPersentHeigh = nPersentHeigh;

//	TRACE("OLD: %3d, %3d, %3d, %3d\r\n", pItem->Rect.left, pItem->Rect.top, pItem->Rect.right, pItem->Rect.bottom);

	dltx = (ParRect.Width() - m_nWidth) * pItem->nPersentX / 100;
	dlty = (ParRect.Height() - m_nHeigh) * pItem->nPersentY / 100;
	dltw = (ParRect.Width() - m_nWidth) * pItem->nPersentWidth / 100;
	dlth = (ParRect.Height() - m_nHeigh) * pItem->nPersentHeigh / 100;

	pItem->Rect.left = ItemRect.left - dltx;
	pItem->Rect.top = ItemRect.top - dlty;
	pItem->Rect.right = pItem->Rect.left + (ItemRect.Width() - dltw);
	pItem->Rect.bottom = pItem->Rect.top + (ItemRect.Height() - dlth);

//	TRACE("NEW: %3d, %3d, %3d, %3d\r\n", pItem->Rect.left, pItem->Rect.top, pItem->Rect.right, pItem->Rect.bottom);

	return TRUE;
}


BOOL	CWndAutoPos::SaveItemsPos()
{
	std::list<ITEM_DATA>::iterator		Iter;
	ITEM_DATA *		pItem;
	CWnd *			pItemWnd;
	int				dltx, dlty, dltw, dlth;
	CRect			ParRect;
	CRect			ItemRect;

	m_pParent->GetWindowRect(ParRect);

	for(Iter = m_ItemList.begin();
		Iter != m_ItemList.end();
		Iter ++)
	{
		pItem = &(*Iter);
		pItemWnd = m_pParent->GetDlgItem(pItem->nID);


		m_pParent->GetDlgItem(pItem->nID)->GetWindowRect(ItemRect);
		m_pParent->ScreenToClient(ItemRect);

		dltx = (ParRect.Width() - m_nWidth) * pItem->nPersentX / 100;
		dlty = (ParRect.Height() - m_nHeigh) * pItem->nPersentY / 100;
		dltw = (ParRect.Width() - m_nWidth) * pItem->nPersentWidth / 100;
		dlth = (ParRect.Height() - m_nHeigh) * pItem->nPersentHeigh / 100;

		pItem->Rect.left = ItemRect.left - dltx;
		pItem->Rect.top = ItemRect.top - dlty;
		pItem->Rect.right = pItem->Rect.left + (ItemRect.Width() - dltw);
		pItem->Rect.bottom = pItem->Rect.top + (ItemRect.Height() - dlth);
	}

	return TRUE;
}

BOOL	CWndAutoPos::InflateItemRect(int nID, CRect &IftRect, BOOL bRePaint)
{
	CWnd *			pItemWnd;
	ITEM_DATA *		pItemData;
	CRect			ItemRect;

	pItemWnd = m_pParent->GetDlgItem(nID);

	FindItem(nID, &pItemData);
	if(pItemData == NULL)
		return FALSE;


	pItemWnd->GetWindowRect(ItemRect);
	m_pParent->ScreenToClient(ItemRect);
	ItemRect.InflateRect(IftRect);
	pItemWnd->MoveWindow(ItemRect, bRePaint);

	pItemData->Rect.InflateRect(IftRect);

	return TRUE;
}


BOOL	CWndAutoPos::DeflateItemRect(int nID, CRect &DftRect, BOOL bRePaint)
{
	CWnd *			pItemWnd;
	ITEM_DATA *		pItemData;
	CRect			ItemRect;

	pItemWnd = m_pParent->GetDlgItem(nID);

	FindItem(nID, &pItemData);
	if(pItemData == NULL)
		return FALSE;


	pItemWnd->GetWindowRect(ItemRect);
	m_pParent->ScreenToClient(ItemRect);
	ItemRect.DeflateRect(DftRect);
	pItemWnd->MoveWindow(ItemRect, bRePaint);

	pItemData->Rect.DeflateRect(DftRect);

	return TRUE;
}

void CWndAutoPos::OnParentSize(int nWidth, int nHeigh, BOOL bRepaint)
{
	std::list<ITEM_DATA>::iterator		Iter;
	ITEM_DATA *		pItem;
	CRect			FinRect;
	int				x, y, w, h;
	CWnd *			pItemWnd;
	HDWP			hWinPosInfo;
	int				nCount;
	int				dltx, dlty, dltw, dlth;

	nCount = (int)m_ItemList.size();
	hWinPosInfo = BeginDeferWindowPos(nCount);

	for(Iter = m_ItemList.begin();
		Iter != m_ItemList.end();
		Iter ++)
	{
		pItem = &(*Iter);
		pItemWnd = m_pParent->GetDlgItem(pItem->nID);
		FinRect = pItem->Rect;

		dltx = (nWidth - m_nWidth) * pItem->nPersentX / 100;
		dlty = (nHeigh - m_nHeigh) * pItem->nPersentY / 100;
		dltw = (nWidth - m_nWidth) * pItem->nPersentWidth / 100;
		dlth = (nHeigh - m_nHeigh) * pItem->nPersentHeigh / 100;

		x = pItem->Rect.left + dltx;
		y = pItem->Rect.top + dlty;
		w = pItem->Rect.Width() + dltw;
		h = pItem->Rect.Height() + dlth;

		if(bRepaint || pItem->bReDraw)
			DeferWindowPos(hWinPosInfo, pItemWnd->GetSafeHwnd(), HWND_TOP, x, y, w, h, SWP_NOZORDER);
		else
			DeferWindowPos(hWinPosInfo, pItemWnd->GetSafeHwnd(), HWND_TOP, x, y, w, h, SWP_NOZORDER | SWP_NOREDRAW);

	}

	EndDeferWindowPos(hWinPosInfo);

//	NMfc::GetDlgBkRgn(m_pParent, &BkRgn);
//	m_pParent->InvalidateRgn(&BkRgn, TRUE);

	return;
}
//	CWndAutoPos
////////////////////////////////////////////////////////////////////////////////





DWORD	WINAPI NMfc::ThreadRunClass_Proc(VOID *pParam)
{
	CCmdTarget *		pTarget;
	AFX_PMSG			pFun;
	THD_RUN_DAT *		pData;

	pData = (THD_RUN_DAT *)pParam;
	pTarget = pData->pTarget;
	pFun = pData->pFun;

	(pTarget->*pFun)();

	return 0;
}


HANDLE	NMfc::ThreadRunClass(CCmdTarget *pTarget, AFX_PMSG pFun)
{
	HANDLE					hThread;
	static	THD_RUN_DAT		Data = {NULL, NULL};

	Data.pTarget = pTarget;
	Data.pFun = pFun;

	hThread = CreateThread(NULL, 0, ThreadRunClass_Proc, &Data, 0, NULL);
	if(hThread == NULL)
		return NULL;

	return hThread;
}


BOOL	NMfc::AppInitialize(HINSTANCE hInstance, TCHAR *pCmdLine, int nCmdShow)
{

	AfxWinInit(hInstance, NULL, pCmdLine, nCmdShow);
	AfxSetResourceHandle(hInstance);

	return TRUE;
}


BOOL	NMfc::GetWndBkRgn(CWnd *pWnd, CRgn *pBkRgn)
{
	CWnd *		pChildWnd;
	DWORD		dwStyle;
	CRect		MainRect;
	CRect		OneRect;
	CRgn		MainRgn;
	CRgn		AllRgn;
	CRgn		OneRgn;

	pWnd->GetWindowRect(MainRect);
	pWnd->ScreenToClient(MainRect);
	MainRgn.CreateRectRgnIndirect(MainRect);

	AllRgn.CreateRectRgn(0, 0, 0, 0);
	for(pChildWnd = pWnd->GetTopWindow();
		pChildWnd;
		pChildWnd = pChildWnd->GetNextWindow(GW_HWNDNEXT))
	{
		dwStyle = pChildWnd->GetStyle();
		if((dwStyle & WS_VISIBLE) == 0)	
			continue;

		pChildWnd->GetWindowRect(OneRect);
		pWnd->ScreenToClient(OneRect);
		OneRgn.CreateRectRgnIndirect(OneRect);

		AllRgn.CombineRgn(&AllRgn, &OneRgn, RGN_OR);
		OneRgn.DeleteObject();
	}

	pBkRgn->CreateRectRgn(0, 0, 0, 0);
	pBkRgn->CombineRgn(&MainRgn, &AllRgn, RGN_XOR);

	MainRgn.DeleteObject();
	AllRgn.DeleteObject();

	return TRUE;
}


BOOL	NMfc::ListCtrlGetBkDrawRgn(CListCtrl *pList, CRgn *pBkRgn)
{
	CRect	CltRect;
	CRect	HeadCtrlRect;
	CRect	ItemsRect;
	CRect	OneRect;
	int		nLastItem;
	CRgn	OneRgn;


	pList->GetClientRect(CltRect);
	pBkRgn->CreateRectRgn(CltRect.left, CltRect.top, CltRect.right, CltRect.bottom);

	nLastItem = pList->GetItemCount() - 1;
	if(nLastItem < 0)
		goto GetListCtrlBkRgn_ItemDone;

	pList->GetItemRect(0, ItemsRect, LVIR_BOUNDS);
	pList->GetItemRect(0, OneRect, LVIR_ICON);
	ItemsRect.left = OneRect.right;

	pList->GetItemRect(nLastItem, OneRect, LVIR_BOUNDS);
	ItemsRect.bottom = OneRect.bottom;

	
	OneRgn.CreateRectRgnIndirect(ItemsRect);
	pBkRgn->CombineRgn(pBkRgn, &OneRgn, RGN_XOR);
	OneRgn.DeleteObject();

GetListCtrlBkRgn_ItemDone:

	return TRUE;
}


BOOL	NMfc::ListCtrlGetAllItemsRect(CListCtrl *pList, RECT *pRect, UINT uCode)
{
	CRect		ItemsRect;
	CRect		OneRect;
	int			nLastItem;

	pList->GetItemRect(0, ItemsRect, uCode);

	nLastItem = pList->GetItemCount() - 1;
	if(nLastItem < 0)
		goto ListCtrlGetAllItemsRect_Done;

	pList->GetItemRect(nLastItem, OneRect, uCode);
	ItemsRect.bottom = OneRect.bottom;

ListCtrlGetAllItemsRect_Done:
	*pRect = ItemsRect;

	return TRUE;
}


int		NMfc::ListCtrlGetFirtSelect(CListCtrl *pList)
{
	POSITION		ps;
	int				nIndex;

	ps = pList->GetFirstSelectedItemPosition();
	nIndex = pList->GetNextSelectedItem(ps);

	return nIndex;
}


BOOL	NMfc::TranslateKeyDown(CHAR *pOneKey, CHAR *pBuffer)
{
	CHAR	szKeyText[1024];
	BOOL	bCtrl, bAlt, bShift;

	bCtrl	= GetKeyState(VK_CONTROL)	< 0;
	bAlt	= GetKeyState(VK_MENU)		< 0;
	bShift	= GetKeyState(VK_SHIFT)		< 0;

	if(strcmp(pOneKey, "Ctrl") == 0)
		bCtrl = FALSE;
	if(strcmp(pOneKey, "Alt") == 0)
		bAlt = FALSE;
	if(strcmp(pOneKey, "Shift") == 0)
		bShift = FALSE;

	szKeyText[0] = 0;
	if(bCtrl)	
		strcat(szKeyText, "Ctrl + ");
	if(bAlt)	
		strcat(szKeyText, "Alt + ");
	if(bShift)	
		strcat(szKeyText, "Shift + ");
	strcat(szKeyText, pOneKey);

	strcpy(pBuffer, szKeyText);
	return TRUE;
}

BOOL	NMfc::TranslateKeyDown(DWORD dwLParam, CHAR *pBuffer)
{
	CHAR	szOneKey[1024];
	BOOL	bRetVal;

	szOneKey[0] = 0;
	GetKeyNameTextA(dwLParam, szOneKey, sizeof(szOneKey));
	bRetVal = TranslateKeyDown(szOneKey, pBuffer);

	return bRetVal;
}


BOOL	NMfc::TranslateKeyDown(UINT uVirKey, BOOL bExt, CHAR *pBuffer)
{
	CHAR	szOneKey[1024];
	BOOL	bRetVal;

	szOneKey[0] = 0;
	NSys::GetNameByVKey(uVirKey, bExt, szOneKey);
	bRetVal = TranslateKeyDown(szOneKey, pBuffer);

	return bRetVal;
}


BOOL	NMfc::TranslateKeyDown(MSG * pMsg, CHAR *pBuffer)
{
	BOOL	bRetVal;

	if(pMsg->message != WM_KEYDOWN)
		return FALSE;

	bRetVal = TranslateKeyDown((DWORD)pMsg->lParam, pBuffer);
	return bRetVal;
}


BOOL	NMfc::TranslateKeyDown(NMLVKEYDOWN * pLVKeyDow, CHAR *pBuffer)
{
	BOOL	bRetVal;

	bRetVal =TranslateKeyDown((UINT)pLVKeyDow->wVKey, FALSE, pBuffer);

	return bRetVal;
}


BOOL	NMfc::TreeCtrl_InsertItems(CTreeCtrl *pTree, std::vector<std::string> &ItemList, HTREEITEM hParent)
{
	std::vector<std::string>::iterator		Iter;

	for(Iter = ItemList.begin();
		Iter != ItemList.end();
		Iter ++)
	{
#ifndef	UNICODE
		pTree->InsertItem(Iter->c_str(), 0, 0, hParent);
#endif
	}

	return TRUE;

}

BOOL	NMfc::MenuCheckItems(CMenu *pMenu, MenuItemList *pItemList)
{
	int				i;
	DWORD			dwFlag;
				

	for(i=0; pItemList[i].uId; i++)
	{
		if(pItemList[i].bChecked)
            dwFlag = MF_CHECKED;
		else
			dwFlag = MF_UNCHECKED;

		if(pItemList[i].bUsePostion)
			dwFlag |= MF_BYPOSITION;

		pMenu->CheckMenuItem(pItemList[i].uId, dwFlag);
	}

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//	CMenuBtn

BEGIN_MESSAGE_MAP(CMenuBtn, CButton)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CMenuBtn::CMenuBtn()
{

}

CMenuBtn::~CMenuBtn()
{

}


BOOL	CMenuBtn::SetPopMenu(CMenu *pMenu)
{
	return TRUE;
}

BOOL	CMenuBtn::LoadPopMenu(UINT uMenu)
{

	return TRUE;
}

void CMenuBtn::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CButton::OnLButtonDown(nFlags, point);
}
//	CMenuBtn
////////////////////////////////////////////////////////////////////////////////




IMPLEMENT_DYNAMIC(CSpliteLine, CWnd)
CSpliteLine::CSpliteLine()
{
	m_bMousDown = FALSE;
	m_nNewPos = SPLITE_INIT_POS;
	m_pParent = NULL;
	m_nType = 0;
}

CSpliteLine::~CSpliteLine()
{
}


BEGIN_MESSAGE_MAP(CSpliteLine, CWnd)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//	ON_WM_MOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgSplite message handlers


#define _AFX_NO_TRACKER_RESOURCES

BOOL	CSpliteLine::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	if(m_nType == 1)
        m_hCursor = LoadCursor(NULL, IDC_SIZEWE);
	else
        m_hCursor = LoadCursor(NULL, IDC_SIZENS);

	::SetCursor(m_hCursor);

	// AfxGetApp()->LoadCursor(AFX_IDC_VSPLITBAR));

	return TRUE;
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


void	CSpliteLine::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (::GetCapture() != NULL) return;

	SetCapture();

	m_bMousDown = TRUE;

	CWnd::OnLButtonDown(nFlags, point);
}


BOOL	CSpliteLine::DrawSplite(int ny)
{
	CWnd *		pDlgWnd;
	CDC *		pDc;
	CPen		SplitePen;
	CPen *		pOldPen;
	CRect		Rect;
	CPoint		ps;
	int			h;


	pDlgWnd = GetParent();
	pDc = pDlgWnd->GetDC();

	this->GetWindowRect(Rect);
	pDlgWnd->ScreenToClient(Rect);

	h = Rect.Height();
	Rect.top = ny - h/2;
	Rect.bottom = Rect.top + h;

	SplitePen.CreatePen(PS_INSIDEFRAME, 5, SPLITE_COLOR);
	pDc->SetROP2(R2_NOTXORPEN);

	pOldPen = pDc->SelectObject(&SplitePen);

	pDc->Rectangle(Rect);

	pDc->SelectObject(pOldPen);
	pDlgWnd->ReleaseDC(pDc);

	return TRUE;
}


void CSpliteLine::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bMousDown == FALSE)
	{
		CWnd::OnMouseMove(nFlags, point);
		return;
	}

	CPoint		pt;

	GetCursorPos(&pt);
	GetParent()->ScreenToClient(&pt);

	if(m_nType == 1)
        m_nNewPos = pt.x;
	else
		m_nNewPos = pt.y;

	UpdateItemPos();
}


void CSpliteLine::OnLButtonUp(UINT nFlags, CPoint point)
{
	UpdateItemPos();
	m_nNewPos = SPLITE_INIT_POS;
	m_bMousDown = FALSE;
	ReleaseCapture();

	CWnd::OnLButtonUp(nFlags, point);
}


BOOL	CSpliteLine::ChangePos(int nNewPos)
{
	m_nNewPos = nNewPos;
	UpdateItemPos();

	m_pParent->PostMessage(WM_SPLITE_UPDATE);

	return TRUE;
}


BOOL	CSpliteLine::AddAssItem(int nId, int x, int y, int w, int h, BOOL bRedraw)
{
	ITEM_DATA		Item;
	CWnd *			pItem;
	CRect			WindowRect;

	pItem = m_pParent->GetDlgItem(nId);
	pItem->GetWindowRect(WindowRect);
	m_pParent->ScreenToClient(WindowRect);

	Item.nID = nId;
	Item.bReDraw = bRedraw;
	Item.nPersentX = x;
	Item.nPersentY = y;
	Item.nPersentW = w;
	Item.nPersentH = h;
	Item.InitRect = WindowRect;

	m_ItemList.push_back(Item);

	if(m_nType == 0)
	{
		if(w)
			m_nType = 1;
		if(h)
			m_nType = 2;
	}

	return TRUE;
}


BOOL	CSpliteLine::InitItemsData(int nType)
{
	CRect		rect;

	m_pParent = GetParent();

	GetWindowRect(rect);
	m_pParent->ScreenToClient(rect);
	m_nType = nType;

	m_nInitPos = rect.top + rect.Height()/2;

	return TRUE;
}


BOOL	CSpliteLine::UpdateItemPos()
{
	std::list<ITEM_DATA>::iterator	Iter;
	ITEM_DATA *		pItem;
	int				x, y;
	int				w, h;
	CWnd *			pItemWnd;
	int				nCount;
	HDWP			hWinPosInfo;
	CRgn			BkRgn;
	CWnd *			pParent;
	int				dltx, dlty, dltw, dlth;
	CRect			OrgRect;
	CRect			rect;
	CRect			PrntRect;
	int				Newy, Oldy;

	if(m_nNewPos == SPLITE_INIT_POS)
		return TRUE;

	pParent = GetParent();
	pParent->GetClientRect(PrntRect);
	this->GetWindowRect(rect);
	pParent->ScreenToClient(rect);

	if(m_nNewPos < 0)
		m_nNewPos = 0;

	if(m_nType == 1)
	{
		if(m_nNewPos + rect.Width() > PrntRect.Width())
			m_nNewPos = PrntRect.Width() - rect.Width()/2;
	}
	else
	{
		if(m_nNewPos + rect.Height() > PrntRect.Height())
			m_nNewPos = PrntRect.Height() - rect.Height()/2;
	}

	//	TRACE("%d\n", m_nNewPos);

	Oldy = m_nInitPos;
	Newy = m_nNewPos;

	nCount = (int)m_ItemList.size();
	if(nCount == 0)
		return TRUE;

	hWinPosInfo = BeginDeferWindowPos(nCount);
	for(Iter = m_ItemList.begin();
		Iter != m_ItemList.end();
		Iter ++)
	{
		pItem = &(*Iter);

		pItemWnd = m_pParent->GetDlgItem(pItem->nID);
		OrgRect = pItem->InitRect;

		dltx = (Newy - Oldy) * pItem->nPersentX / 100;
		dlty = (Newy - Oldy) * pItem->nPersentY / 100;
		dltw = (Newy - Oldy) * pItem->nPersentW / 100;
		dlth = (Newy - Oldy) * pItem->nPersentH / 100;

		x = OrgRect.left + dltx;
		y = OrgRect.top + dlty;
		w = OrgRect.Width() + dltw;
		h = OrgRect.Height() + dlth;

		if(pItem->bReDraw)
			DeferWindowPos(hWinPosInfo, pItemWnd->GetSafeHwnd(), NULL, x, y, w, h, SWP_NOZORDER);
		else
			DeferWindowPos(hWinPosInfo, pItemWnd->GetSafeHwnd(), NULL, x, y, w, h, SWP_NOZORDER | SWP_NOREDRAW);
	}
	EndDeferWindowPos(hWinPosInfo);


	if(m_nType == 1)
	{
		x = m_nNewPos - rect.Width()/2;
		this->SetWindowPos(NULL, x, rect.top, 0, 0, SWP_NOSIZE | SWP_NOREDRAW);
	}
	else
	{
		y = m_nNewPos - rect.Height()/2;
		this->SetWindowPos(NULL, rect.left, y, 0, 0, SWP_NOSIZE | SWP_NOREDRAW);
	}
	this->InvalidateRect(NULL, FALSE);

	return TRUE;
}


BOOL	CSpliteLine::OnParentSize(int cx, int cy)
{
	std::list<ITEM_DATA>::iterator	Iter;
	ITEM_DATA *		pItem;
	CWnd *			pItemWnd;
	CRgn			BkRgn;
	CRect			rect;
	CRect			WindowRect;

	if(GetSafeHwnd() == NULL)
		return FALSE;

	if(this == NULL)
		return FALSE;

	if(m_pParent == NULL)
		m_pParent = GetParent();

	GetWindowRect(rect);
	m_pParent->ScreenToClient(rect);

	if(m_nType == 1)
        m_nInitPos = rect.CenterPoint().x;
	else
		m_nInitPos = rect.CenterPoint().y;


	for(Iter = m_ItemList.begin();
		Iter != m_ItemList.end();
		Iter ++)
	{
		pItem = &(*Iter);

		pItemWnd = m_pParent->GetDlgItem(pItem->nID);
		pItemWnd->GetWindowRect(WindowRect);
		m_pParent->ScreenToClient(WindowRect);

		pItem->InitRect = WindowRect;
	}

	return TRUE;
}


void CSpliteLine::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd::PreSubclassWindow();

	this->ModifyStyle(0, SS_NOTIFY);
}






