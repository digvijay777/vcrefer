#pragma once

class CMemDC : public CDC 
{
private:
	CBitmap*	m_bitmap;
	CBitmap*	m_oldBitmap;
	CDC*		m_pDC;
	CRect		m_rcBounds;
public:

	CMemDC(CDC* pDC, const CRect& rcBounds, BOOL bCopy = FALSE) : CDC()
	{
		CreateCompatibleDC(pDC);
		m_bitmap = new CBitmap;
		m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
		m_oldBitmap = SelectObject(m_bitmap);
		m_pDC = pDC;
		m_rcBounds = rcBounds;

		if(bCopy)
		{
			this->BitBlt(0, 0, m_rcBounds.Width(), m_rcBounds.Height(), 
				pDC, m_rcBounds.left, m_rcBounds.top, SRCCOPY);
		}

	}

	~CMemDC() 
	{
		m_pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(), 
			this, 0, 0, SRCCOPY);

		SelectObject(m_oldBitmap);

		if (m_bitmap != NULL) 
			delete m_bitmap;
	}


//	BOOL	BitltBack(pDc)
//	{
//	}

	CMemDC* operator->() 
	{
		return this;
	}


	BOOL DrawParentBk(CWnd *pWnd)
	{
		CWnd *		pParent;
		CRect		WndRect;
		CRect		ParRect;
		POINT		OffPt;

		pParent = pWnd->GetParent();
		pWnd->GetWindowRect(&WndRect);
		pParent->GetWindowRect(&ParRect);

		OffPt.x = WndRect.left - ParRect.left;
		OffPt.y = WndRect.top - ParRect.top;

		this->SetWindowOrg(OffPt);
		pParent->SendMessage(WM_ERASEBKGND, (WPARAM)this->m_hDC, 0);
		this->SetViewportOrg(OffPt);

		return TRUE;
	}

	BOOL	RoundRect(CRect &Rect, COLORREF crLine, int nRound)
	{
		CPen		Pen;
		CPen *		pOldPen;
		POINT		pt = {nRound, nRound};
		BOOL		bRetVal;
					

		Pen.CreatePen(PS_SOLID, 1, crLine);
		pOldPen = CDC::SelectObject(&Pen);

		bRetVal = CDC::RoundRect(Rect, pt);

		CDC::SelectObject(pOldPen);

		return bRetVal;
	}


	static	BOOL	FillSmoothTop(CDC *pDc, CRect *pRect, COLORREF crTop, COLORREF crBottom)
	{
		CDC			bitmapDC;
		CBitmap		bitmap;
		int			i;
		CBitmap *	pOldBitmap;
		COLORREF	crCalcu;
		int			r, g, b;
		int			nWidth;
		int			nHeight;


		nWidth = pRect->Width();
		nHeight = pRect->Height();
		bitmapDC.CreateCompatibleDC(pDc);
		bitmap.CreateCompatibleBitmap(pDc, 1, nHeight);
		pOldBitmap = bitmapDC.SelectObject(&bitmap);

		for(i=0; i<nHeight; i++)
		{
			r = (GetRValue(crBottom)*(i) + GetRValue(crTop)*(nHeight-i-1)) / (nHeight-1);
			g = (GetGValue(crBottom)*(i) + GetGValue(crTop)*(nHeight-i-1)) / (nHeight-1);
			b = (GetBValue(crBottom)*(i) + GetBValue(crTop)*(nHeight-i-1)) / (nHeight-1);
			crCalcu = RGB(r, g, b);

			bitmapDC.SetPixelV(0, i, crCalcu);
		}

		pDc->StretchBlt(pRect->left, pRect->top, nWidth, nHeight, &bitmapDC, 0, 0, 1, nHeight, SRCCOPY);

		bitmapDC.SelectObject(pOldBitmap);
		bitmap.DeleteObject();
		bitmapDC.DeleteDC();
		return TRUE;
	}



	static BOOL	LoadBitmapFromFile(CBitmap *pBitmap, CHAR *pFileName)
	{
		HBITMAP		hImage;
		BOOL		bRetVal;

		hImage = (HBITMAP)::LoadImageA(NULL, pFileName, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if(hImage == NULL)
			return FALSE;

		bRetVal = pBitmap->Attach(hImage);
		if(bRetVal == FALSE)
			DeleteObject(hImage);

		return bRetVal;
	}




};