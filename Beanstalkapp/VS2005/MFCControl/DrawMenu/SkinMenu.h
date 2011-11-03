#pragma once


// CSkinMenu

class CSkinMenu : public CMenu
{
	DECLARE_DYNAMIC(CSkinMenu)

public:
	CSkinMenu();
	virtual ~CSkinMenu();

public:
	virtual void		DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void		DrawFrame(HDC hDC);
	virtual void		MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

private:
	CImageList		m_imgList;
	CSize			m_sizeItem;
	COLORREF		m_colFrames[3];

private:
	void	SetFrameColor(HBITMAP hImage);
public:
	BOOL	SetImages(HBITMAP hItem);
	BOOL	LoadImages(int nItem);
};


