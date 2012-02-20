#pragma once
#include <vector>
#include <string>
#include <map>
#include "ListHead.h"
#include <atlimage.h>

#define UM_SETITEMICONHANDER			(WM_USER+1)
#define UM_SETITEMICONFILE				(WM_USER+2)
#define UM_SETITEMSWITCHBUTTON			(WM_USER+3)
#define UM_EHOMELISTCLICKITEM			(WM_USER+4)			// 单击其中一个项目: wParam: HWND指向LIST句柄, lParam: MAKELONG(iItem, iSubItem)
#define UM_EHOMELISTCLICKBUTTON			(WM_USER+5)			// 单击其中一个项目: wParam: HWND指向LIST句柄, lParam: MAKELONG(iItem, iButtonMask)

#ifndef String
#ifdef UNICODE
#define String std::wstring
#else
#define String std::string
#endif
#endif

typedef enum _EHomeListFormat{
	EHLF_Normal	= 0x0			// 普通样式: 值为一行字符串
	, EHLF_Title				// 标题样式: 值第一行为标题，值第二行为说明文本(文本以CRLF换行)
	, EHLF_SwitchButton			// 交换按钮样式: 由UM_SETITEMSWITCHBUTTON消息设置交换按钮列和交换按钮图片, 值为0(未先中态)或1(选中态)
	, EHLF_Progress				// 进度条样式: 值为0-100的数字
	, EHLF_Link					// 超连接样式: 值为一行字符串, 此样式会在列文本上加一下划线
	, EHLF_IconList				// 五角星样式: 值为0-10的数字
	, EHLF_MuiltButton			// 多按钮样式: 值为数字表示要显示按钮的图标索引MASK值， 图标是四个为一组, 多按钮只能有一列
}EHOMELISTFORMAT;
// 列数据
typedef struct _EHomeListColumn{
	EHOMELISTFORMAT			fmt;		// 样式类型
	HIMAGELIST				hImgList;	// 图标
}EHOMELISTCOLUMN;
// 行数据
typedef struct _EHomeListItem{
	std::vector<String>			vctstr;
	int							nImage;
	std::map<int, RECT>			mapMouse;
	LPARAM						lpData;
}EHOMELISTITEM;
// CEHomeList
class CEHomeList : public CListCtrl
{
	DECLARE_DYNAMIC(CEHomeList)

public:
	CEHomeList();
	virtual ~CEHomeList();

private:
	std::vector<EHOMELISTCOLUMN>		m_vctColumnFmt;
	CListHead							m_ctrlHeader;
	std::vector<EHOMELISTITEM>			m_vctData;
	int									m_nItemHeight;			// 行高
	CImageList*							m_pImageList[3];
	CFont								m_FontBold;
	CFont								m_FontLink;
	CPen								m_PenGridlines;
	BOOL								m_bTrackMouseEvent;
	DWORD								m_dwMouseItem;
	DWORD								m_dwMuiltIndex;
	HCURSOR								m_hCursor[2];
	CImageList							m_imagelist;
	ULONG								m_nLoadingIndex;
	Gdiplus::Image*						m_pLoadingImg;
	ULONG								m_nLoadingImageCount;
	BOOL								m_bEnableWindow;

protected:
	void				SetColumnFormat(int nColumn, EHOMELISTFORMAT fmt, HIMAGELIST hImgList);
	int					MuiltButtonHitTest(POINT pt, CRect &rect, HIMAGELIST hImage);
	int					RealGetItemText(int nItem, int nSubItem, TCHAR *pBuffer, int nSize);

public:
	// 扩展List控件操作方法
	void				SetColumnFormat(int nColumn, EHOMELISTFORMAT fmt);
	EHOMELISTFORMAT		GetColumnFormat(int nColumn);
	BOOL				SetItemText(int nItem, int nSubItem, LPCTSTR lpszText );
	void				SetItemHeight(int nHeight);
	void				SetSwitchImageList(int nSubItem, HIMAGELIST hImageList);
	void				SetIconImageList(int nSubItem, HIMAGELIST hImageList);
	void				SetMuiltButtonImageList(int nSubItem, HIMAGELIST hImageList);
	void				SetEmptyString(LPCTSTR lpEmptyString);
	void				SetLoadingImage(Gdiplus::Image* pImage, ULONG nSplitCount);

protected:
	// 绘制集合
	virtual	void		Draw_NormalItem(CDC* pDC, RECT rect, LPCTSTR lpText, UINT nColumnFmt);
	virtual void		Draw_TitleItem(CDC* pDC, RECT rect, LPCTSTR lpText);
	virtual void		Draw_SwitchButtonItem(CDC* pDC, RECT& rect, int nIndex, HIMAGELIST hImageList, BOOL bMouseOver);
	virtual void		Draw_ProgressItem(CDC* pDC, RECT rect, int nProg, int nColor);
	virtual void		Draw_LinkItem(CDC* pDC, RECT& rect, LPCTSTR lpLink, BOOL bMouseOver);
	virtual void		Draw_IconListItem(CDC* pDC, HIMAGELIST hImageList, RECT rect, int nIndex);
	virtual void		Draw_MuiltButtonItem(CDC* pDC, RECT& rect, HIMAGELIST hImageList, DWORD dwMask, int nIndex);

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void DrawLoading(CDC* pDC, CRect rect);
private:
#if defined(UNICODE) || defined(_UNICODE)
	std::wstring							m_strEmpty;
#else
	std::string								m_strEmpty;
#endif
protected:
	DECLARE_MESSAGE_MAP()

	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItem);
public:
// 	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


