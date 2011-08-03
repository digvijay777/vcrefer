#pragma once
#include <vector>

// CEHomeImgList

class CEHomeImgList : public CListCtrl
{
	DECLARE_DYNAMIC(CEHomeImgList)

private:
	// 项数据结构
	typedef struct _ImageItem{
		HBITMAP			hBitmap;
		TCHAR			szName[64];
		TCHAR			szPath[MAX_PATH];
	}IMAGEITEM, *LPIMAGEITEM;

public:
	CEHomeImgList();
	virtual ~CEHomeImgList();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual	BOOL	GetImageFileData(LPCTSTR lpPath, BYTE** pData, DWORD* dwSize);
	virtual void	OpenImageFile(int nIndex);

public:
	// 新增的方法
	void			SetLogoIcon(HICON hIcon);

private:
	std::vector<IMAGEITEM>			m_vctData;
	CImageList						m_imagelist;
	CPen							m_penBorder;
	CPen							m_penBorder2;
	CPen							m_penSelBorder;
	HICON							m_hLogoIcon;
	int								m_nSelItem;
	CSize							m_sizeItem;
	CFont							m_fontBold;
	BOOL							m_bTrackMouse;

	void			MyDrawItem(CDC* pDC, CRect& rect, HBITMAP hBitmap, LPCTSTR lpName, BOOL bSel);

protected:
	HBITMAP			GetImageFileBitmap(LPCTSTR lpPath);
	void			SplitImageFilePath(LPCTSTR lpPath, LPTSTR pName, LPTSTR pTempPath);

protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItem);
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


