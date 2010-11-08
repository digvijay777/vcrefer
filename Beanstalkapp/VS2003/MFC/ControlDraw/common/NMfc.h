#pragma once

#define NMfc_ThreadRunThis(_Fun_)\
	NMfc::ThreadRunClass(this, static_cast<AFX_PMSG>(_Fun_))
#define NMfc_ThreadRun(_pTarget_, _Fun_)\
	NMfc::ThreadRunClass(_pTarget_, static_cast<AFX_PMSG>(_Fun_))

#include <afxtempl.h>
#include <list>
#include <map>
#include <vector>
#include <string>

#include <afxcmn.h>

#define WM_YMSG				(WM_USER+0x3200)
#define WM_SPLITE_UPDATE	(WM_YMSG+0x0001)


class CWndAutoPos
{
	struct ITEM_DATA
	{
		CRect	Rect;
		int     nID;
		int     nPersentX;
		int     nPersentY;
		int     nPersentWidth;
		int     nPersentHeigh;
		BOOL	bReDraw;
	};

public:
	CWndAutoPos();
	~CWndAutoPos();


	BOOL	Clear();
	BOOL	AddItem(int nID, BOOL bReDraw = FALSE, int nPersentX = 0, int nPersentY = 0, int nPersentWidth = 0, int nPersentHeigh = 0);
	BOOL	InitOrgSize(CWnd *pParent);

	BOOL	InflateItemRect(int nID, CRect &IftRect, BOOL bRePaint = TRUE);
	BOOL	DeflateItemRect(int nID, CRect &DftRect, BOOL bRePaint = TRUE);
	BOOL	FindItem(int nID, ITEM_DATA **ppItemData);

	BOOL	SaveItemChange(int nID, int nPersentX = 0, int nPersentY = 0, int nPersentWidth = 0, int nPersentHeigh = 0);
	
	BOOL	SaveItemsPos();

	virtual	void OnParentSize(int nWidth, int nHeigh, BOOL bRepaint = FALSE);


//	
	CWnd *					m_pParent;
	int						m_nWidth;
	int						m_nHeigh;
	std::list<ITEM_DATA>	m_ItemList;
};




class CIconList : public CImageList
{
public:
	CIconList()
	{
	}

	virtual	~CIconList()
	{
		__super::DeleteImageList();
	}

	BOOL DeleteImageList()
	{
		m_IconMap.clear();
		return __super::DeleteImageList();
	}

	int Add(HICON hIcon, BOOL &bRealAdd)
	{
		std::map<HICON, INT>::iterator	IconIter;
		int		nIndex;

		bRealAdd = FALSE;
		if(hIcon == NULL)
			return -1;

		IconIter = m_IconMap.find(hIcon);
		if(IconIter != m_IconMap.end())
		{
			return IconIter->second;
		}

		nIndex = __super::Add(hIcon);
		bRealAdd = TRUE;
		m_IconMap[hIcon] = nIndex;

		return nIndex;
	}

	std::map<HICON, int>		m_IconMap;
};





namespace NMfc
{
	struct THD_RUN_DAT 
	{
		CCmdTarget *		pTarget;
		AFX_PMSG			pFun;
	};

	DWORD	WINAPI ThreadRunClass_Proc(VOID *pParam);

	//	static_cast<AFX_PMSG>(XXX)	typedef void (AFX_MSG_CALL CCmdTarget::*AFX_PMSG)(void)
	HANDLE	ThreadRunClass(CCmdTarget *pTarget, AFX_PMSG pFun);


	BOOL	AppInitialize(HINSTANCE hInstance, TCHAR *pCmdLine = NULL, int nCmdShow = 0);

	BOOL	GetWndBkRgn(CWnd *pWnd, CRgn *pBkRgn);

	BOOL	ListCtrlGetBkDrawRgn(CListCtrl *pList, CRgn *pBkRgn);
	BOOL	ListCtrlGetAllItemsRect(CListCtrl *pList, RECT *pRect, UINT uCode = LVIR_BOUNDS);
	int		ListCtrlGetFirtSelect(CListCtrl *pList);

	BOOL	TranslateKeyDown(CHAR *pOneKey, CHAR *pBuffer);
	BOOL	TranslateKeyDown(DWORD dwLParam, CHAR *pBuffer);
	BOOL	TranslateKeyDown(UINT uVirKey, BOOL bExt, CHAR *pBuffer);
	BOOL	TranslateKeyDown(MSG * pMsg, CHAR *pBuffer);
	BOOL	TranslateKeyDown(NMLVKEYDOWN * pLVKeyDow, CHAR *pBuffer);

	BOOL	TreeCtrl_InsertItems(CTreeCtrl *pTree, std::vector<std::string> &ItemList, HTREEITEM hParent = TVI_ROOT);


	typedef	struct	_MenuItemList
	{
		UINT		uId;
		BOOL		bChecked;
		BOOL		bUsePostion;
	}MenuItemList;

	BOOL	MenuCheckItems(CMenu *pMenu, MenuItemList *pItemList);
}

#include "MemDc.h"

namespace	NDC
{
	template<class TYPE>
	BOOL	DrawWaveData(CDC *pDc, CRect Rect, TYPE *pData, int nDataLen, int nMax, int nMin = 0, COLORREF crLine = RGB(255, 255, 255))
	{
		int			x, y;
		int			nIndex;
		CPen		Pen;
		CPen *		pOldPen;
		int			nSave;

		nSave = pDc->SaveDC();
		pDc->IntersectClipRect(Rect);
		Pen.CreatePen(PS_SOLID, 1, crLine);
		pOldPen = pDc->SelectObject(&Pen);

		y = (int)(Rect.Height() * (pData[0]-nMin) / (nMax-nMin));
		pDc->MoveTo(Rect.left, Rect.bottom-y);
		for(x=1; x<Rect.Width(); x++)
		{
			nIndex = (int)((double)x * nDataLen / Rect.Width() + 0.5);
			y = (int)(Rect.Height() * (pData[nIndex]-nMin) / (nMax-nMin));
			pDc->LineTo(x+Rect.left, Rect.bottom-y);
		}

		pDc->SelectObject(pOldPen);
		pDc->RestoreDC(nSave);

		return TRUE;
	}


	template<class TYPE>
	BOOL	WndDrawWave(HWND hWnd, TYPE *pData, int nDataLen, int nMax, int nMin = 0)
	{
		CWnd *		pWnd;
		CDC *		pDC;
		CRect		Rct;
		BOOL		bRetVal;

		pWnd = CWnd::FromHandle(hWnd);
		if(pWnd == NULL)
			return FALSE;

		pDC = pWnd->GetDC();
		pWnd->GetClientRect(Rct);

		{
			CMemDC	MemDc(pDC, Rct);

			MemDc.FillSolidRect(Rct, RGB(0, 0, 0));
			bRetVal = DrawWaveData(&MemDc, Rct, pData, nDataLen, nMax, nMin);
		}

		pWnd->ReleaseDC(pDC);
		return bRetVal;
	}
}



class CMenuBtn : public CButton
{
public:
	CMenuBtn();
	~CMenuBtn();


	BOOL	SetPopMenu(CMenu *pMenu);
	BOOL	LoadPopMenu(UINT uMenu);


	CMenu	m_PopMenu;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};




class CSpliteLine : public CWnd
{
	DECLARE_DYNAMIC(CSpliteLine)
public:
	struct ITEM_DATA
	{
		int     nID;
		CRect	InitRect;
		int     nPersentX;
		int     nPersentY;
		int     nPersentW;
		int     nPersentH;
		BOOL	bReDraw;
	};

	CSpliteLine();
	virtual ~CSpliteLine();

	//	1: X	2: Y
	BOOL	InitItemsData(int nType = 0);
	BOOL	AddAssItem(int nId, int x, int y, int w, int h, BOOL bRedraw = TRUE);


	BOOL	UpdateItemPos();
	BOOL	DrawSplite(int ny);
	BOOL	ChangePos(int nNewPos);
	BOOL	OnParentSize(int cx, int cy);


	HCURSOR		m_hCursor;

	std::list<ITEM_DATA>	m_ItemList;
	BOOL					m_bMousDown;
	int						m_nNewPos;
	int						m_nInitPos;
	CWnd *					m_pParent;
	int						m_nType;	//0: ц╩сп	1: X	2: Y

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
protected:
	virtual void PreSubclassWindow();
};













