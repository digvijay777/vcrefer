#pragma once


// CImgTabCtrl

class CImgTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CImgTabCtrl)

public:
	CImgTabCtrl();
	virtual ~CImgTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
private:
	CBitmap		m_bmpItemSel;
	CBitmap		m_bmpItemNormal;
	CBitmap		m_bmpBk;
public:
	BOOL	InitTabCtrl(SIZE *siItem, UINT nIDNorBmp = -1, UINT nIDSelBmp = -1, UINT nIDBkBmp = -1);

};


