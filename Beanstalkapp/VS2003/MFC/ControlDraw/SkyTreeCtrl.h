#pragma once
#include <vector>
#include <string>

#if defined(UNICODE) || defined(_UNICODE)
	#define String			std::wstring
#else
	#define String			std::string
#endif
// CSkyTreeCtrl
#define ITEM_CHECKED_EMPTY			1
#define ITEM_CHECKED_CHEKED			2
#define ITEM_CHECKED_HALF			3
#define ITEM_CHECKED_DISABLED		0

class CSkyTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CSkyTreeCtrl)

public:
	CSkyTreeCtrl();
	virtual ~CSkyTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
private:
	CImageList		m_imgState;
	CImageList		m_imgICON;
public:
	void		InitImageList(UINT nState = -1, UINT nICON = -1);
	void		CheckItem(HTREEITEM hItem, BOOL bChecked = TRUE);
	void		CheckAll(BOOL bChecked);
	HTREEITEM	InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage, HTREEITEM hParent);
protected:
	void		ReverseItemState(HTREEITEM hItem);
	void		SetItemStateIndex(HTREEITEM hItem, int nIndex);
	int			GetItemStateIndex(HTREEITEM hItem);
	void		UpdateItemParentIndex(HTREEITEM hItem);
	void		SetItemChildStateIndex(HTREEITEM hItem, int nIndex);
	
private:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
};

//////////////////////////////////////////////////////////////////////////
// 文件浏览树形控件
class CFolderTree : public CSkyTreeCtrl
{
	DECLARE_DYNAMIC(CFolderTree)
public:
	CFolderTree();
	virtual ~CFolderTree();

protected:
	HTREEITEM	m_hComputer;
	HTREEITEM	m_hDocument;
	HTREEITEM	m_hMemory;

public:
	void		InitFolder(UINT nState, UINT nICON);
	void		CheckMyDocument(BOOL bChecked = TRUE);
	void		CheckComputer(BOOL bChecked = TRUE);
	void		CheckMemory(BOOL bChecked = TRUE);
// 	void		CheckAll(BOOL bChecked = TRUE);
	int			GetSelectFolder(std::vector<String>& vctSel);
	BOOL		IsSelectMemory();

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual void		OnExpandFolder(HTREEITEM hItem);
	virtual BOOL		HasChildFolder(LPCTSTR lpFolderPath);

protected:
	void		SetHasChild(HTREEITEM hItem);
	void		GetFullCheckedFolder(std::vector<String>& vctSel, HTREEITEM hParent);
	void		AddDrive(LPCTSTR lpDrive);
	HTREEITEM	AddFolder(LPCTSTR lpFolder, HTREEITEM hParent);
	int			GetVolumeName(LPCTSTR lpDrive, LPTSTR lpVolumeName);
	int			GetDriveImage(LPCTSTR lpDrive);
	CString		GetFolderPath(HTREEITEM hItem);
};
