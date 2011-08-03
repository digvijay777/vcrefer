// EHomeList\EHomeImgList.cpp : implementation file
//

#include "stdafx.h"
#include "EHomeImgList.h"
#include <atlimage.h>

// CEHomeImgList

IMPLEMENT_DYNAMIC(CEHomeImgList, CListCtrl)

CEHomeImgList::CEHomeImgList()
{
	m_hLogoIcon = NULL;
	m_nSelItem = -1;
	m_sizeItem.cx = 80;
	m_bTrackMouse = FALSE;
	m_sizeItem.cy = 110;
}

CEHomeImgList::~CEHomeImgList()
{
}


BEGIN_MESSAGE_MAP(CEHomeImgList, CListCtrl)
	ON_WM_MEASUREITEM_REFLECT()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CEHomeImgList::OnNMCustomdraw)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void CEHomeImgList::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	DWORD		dwStyle		= 0;

	// 更改普通样式
	dwStyle = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
// 	ASSERT(dwStyle & LVS_OWNERDATA);
	dwStyle |= LVS_ICON;				// 列表控件
//	dwStyle |= LVS_OWNERDRAWFIXED;		// 自已绘制 DrawItem() 函数会被调用
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle);

	// 设置高度
	m_imagelist.Create(m_sizeItem.cx, m_sizeItem.cy, ILC_COLOR24, 1, 1);
	SetImageList(&m_imagelist, LVSIL_NORMAL);
}

LRESULT CEHomeImgList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(LVM_INSERTITEM == message)
	{
		LPLVITEM			pItem		= (LPLVITEM)lParam;
		IMAGEITEM			item		= {0};

		item.hBitmap = GetImageFileBitmap(pItem->pszText);
		SplitImageFilePath(pItem->pszText, item.szName, item.szPath);
		m_vctData.push_back(item);
		SetItemCount((int)m_vctData.size());
		//pItem->pszText = item.szName;
		return (LRESULT)m_vctData.size() - 1 ;
	}
	else if(LVM_DELETEALLITEMS == message)
	{
		for(int i = 0; i < (int)m_vctData.size(); i++)
		{
			if(NULL != m_vctData[i].hBitmap)
				DeleteObject(m_vctData[i].hBitmap);
			if(PathFileExists(m_vctData[i].szPath))
				DeleteFile(m_vctData[i].szPath);
		}
		m_vctData.clear();
		return TRUE;
	}
	else if(LVM_DELETEITEM == message)
	{
		int				nItem		= (int)wParam;

		if(nItem < 0 || nItem >= (int)m_vctData.size())
			return FALSE;

		IMAGEITEM		image		= m_vctData.at(nItem);
		
		if(NULL != image.hBitmap)
			DeleteObject(image.hBitmap);
		if(PathFileExists(image.szPath))
			DeleteFile(image.szPath);
		m_vctData.erase(m_vctData.begin() + nItem);
		return TRUE;
	}
	else if(WM_MOUSELEAVE == message)
	{
		m_nSelItem = -1;
		Invalidate(FALSE);
		m_bTrackMouse = FALSE;
	}
	else if(WM_LBUTTONDOWN == message)
	{
		OpenImageFile( HitTest(CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) );
	}

	return CListCtrl::WindowProc(message, wParam, lParam);
}

// 获取图片信息
BOOL CEHomeImgList::GetImageFileData(LPCTSTR lpPath, BYTE** pData, DWORD* dwSize)
{
	HANDLE			hFile		= NULL;
	DWORD			dwRed		= 0;

	hFile = CreateFile(lpPath, GENERIC_READ, FILE_SHARE_READ
		, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
		return FALSE;
	*dwSize = GetFileSize(hFile, NULL);
	*pData = (BYTE*)malloc(*dwSize);
	ReadFile(hFile, *pData, *dwSize, &dwRed, NULL);
	CloseHandle(hFile);
	return TRUE;
}
// 打开临时文件
void CEHomeImgList::OpenImageFile(int nIndex)
{
	if(-1 == nIndex)
		return;
	if(NULL == m_vctData[nIndex].hBitmap)
	{
		MessageBox(_T("图片文件不存在!"));
		return;
	}

	CImage			img;

	img.Attach(m_vctData[nIndex].hBitmap);
	img.Save(m_vctData[nIndex].szPath);
	img.Detach();

	ShellExecute(NULL, _T("open"), m_vctData[nIndex].szPath, NULL, NULL, SW_SHOWMAXIMIZED);
}
// 获取图版文件的HBITMAP
HBITMAP CEHomeImgList::GetImageFileBitmap(LPCTSTR lpPath)
{
	BYTE*				pData		= NULL;
	DWORD				dwSize		= 0;
	HBITMAP				hBmpRet		= NULL;
	CComPtr<IStream>	spStream;
	CImage				img;
	HGLOBAL				hGlobal		= NULL;

	if(FALSE == GetImageFileData(lpPath, &pData, &dwSize))
		goto end;
	hGlobal = GlobalAlloc(GMEM_FIXED, dwSize);
	if(NULL == hGlobal)
		goto end;
	CreateStreamOnHGlobal(hGlobal, TRUE, &spStream);
	if(NULL == spStream)
	{
		GlobalFree(hGlobal);
		goto end;
	}
	spStream->Write(pData, dwSize, &dwSize);
	img.Load(spStream);
	hBmpRet = img.Detach();
	// 退出
end:
	if(NULL != pData)
		free(pData);
	return hBmpRet;
}

// 分隔文件名
void CEHomeImgList::SplitImageFilePath(LPCTSTR lpPath, LPTSTR pName, LPTSTR pTempPath)
{
	LPCTSTR			pp					= NULL;
	LPCTSTR			p2					= NULL;
	TCHAR			szPath[MAX_PATH]	= {0};

	pp = _tcsrchr(lpPath, _T('\\'));
	if(NULL == pp)
		pp = _tcsrchr(lpPath, _T('/'));
	if(NULL == pp)
		pp = lpPath;
	else
		pp++;
	p2 = _tcsrchr(pp, _T('.'));
	if(NULL == p2)
		_tcscpy(pName, pp);
	else
		_tcsncpy(pName, pp, p2-pp);
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_TEMPLATES, TRUE);
	PathCombine(szPath, szPath, _T("EHome"));
	if(!PathIsDirectory(szPath))
		CreateDirectory(szPath, NULL);
	PathCombine(szPath, szPath, _T("ImageList"));
	if(!PathIsDirectory(szPath))
		CreateDirectory(szPath, NULL);
	PathCombine(szPath, szPath, pp);
	_tcscpy(pTempPath, szPath);
}
// 绘制每一项
void CEHomeImgList::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	INT I = 0;
	// TODO:  Add your code to draw the specified item
}

void CEHomeImgList::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW*		pLVCD			= reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	*pResult = 0;

	if(CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if(CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_SKIPDEFAULT;

		CDC*		pDC			= CDC::FromHandle(pLVCD->nmcd.hdc);
		CRect		rect;
		int			nItem		= pLVCD->nmcd.dwItemSpec;
		int			nSaveDC		= pDC->SaveDC();

		GetItemRect(pLVCD->nmcd.dwItemSpec, &rect, LVIR_BOUNDS);
		pDC->FillSolidRect(&rect, RGB(0xff, 0xff, 0xff));
		MyDrawItem(pDC, rect, m_vctData[nItem].hBitmap, m_vctData[nItem].szName, m_nSelItem == nItem);
		pDC->RestoreDC(nSaveDC);
	}
}

void CEHomeImgList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItem)
{
	int i = 5;
}

void CEHomeImgList::MyDrawItem(CDC* pDC, CRect& rect, HBITMAP hBitmap, LPCTSTR lpName, BOOL bSel)
{
	CImage			img;
	CRect			rtIcon;
	CRect			rtImg;
	CRect			rtBorder;
	CRect			rtText;

	rtBorder = rect;
	rtBorder.DeflateRect(5, 5, 5, 25);
	rtIcon.left = rtBorder.left + (rtBorder.Width() - 100) / 2;
	rtIcon.top = rtBorder.top + (rtBorder.Height() - 76) / 2;
	rtIcon.right = rtIcon.left + 100;
	rtIcon.bottom = rtIcon.top + 76;
	// 绘制边框
	if(NULL == m_penBorder.GetSafeHandle())
	{
		m_penBorder.CreatePen(PS_SOLID, 1, RGB(0xEC, 0xEC, 0xEC));
	}
	if(NULL == m_penBorder2.GetSafeHandle())
	{
		m_penBorder2.CreatePen(PS_SOLID, 1, RGB(0xCE, 0xCE, 0xCE));
	}
	if(NULL == m_penSelBorder.GetSafeHandle())
	{
		m_penSelBorder.CreatePen(PS_SOLID, 2, RGB(0x67, 0x01, 0x01));
	}
	if(bSel)
		pDC->SelectObject(&m_penSelBorder);
	else
		pDC->SelectObject(&m_penBorder);
	pDC->MoveTo(rtBorder.left, rtBorder.top);
	pDC->LineTo(rtBorder.right, rtBorder.top);
	pDC->LineTo(rtBorder.right, rtBorder.bottom);
	pDC->LineTo(rtBorder.left, rtBorder.bottom);
	if(bSel)
		pDC->LineTo(rtBorder.left, rtBorder.top-1);
	else
		pDC->LineTo(rtBorder.left, rtBorder.top);
	pDC->SelectObject(&m_penBorder2);
	pDC->MoveTo(rtIcon.left, rtIcon.top);
	pDC->LineTo(rtIcon.right, rtIcon.top);
	pDC->LineTo(rtIcon.right, rtIcon.bottom);
	pDC->LineTo(rtIcon.left, rtIcon.bottom);
	pDC->LineTo(rtIcon.left, rtIcon.top);
	rtIcon.DeflateRect(3, 3, 3, 3);
	// 绘制图片
	if(NULL != hBitmap)
	{
		img.Attach(hBitmap);
		rtImg.SetRectEmpty();
		rtImg.right = img.GetWidth();
		rtImg.bottom = img.GetHeight();
		pDC->SetStretchBltMode(COLORONCOLOR);
		img.Draw(pDC->GetSafeHdc(), rtIcon, rtImg);
		img.Detach();
	}
	else
	{
		pDC->FillSolidRect(&rtIcon, RGB(0xCC, 0xCC, 0xCC));
		pDC->SetTextColor(RGB(0x66, 0x66, 0x66));
		pDC->DrawText(_T("图片不存在"), 5, &rtIcon, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		pDC->SetTextColor(RGB(0x0, 0x0, 0x0));
	}
	// 绘制微标
	if(NULL != m_hLogoIcon)
	{
		DrawIconEx(pDC->GetSafeHdc(), rtIcon.right-5, rtIcon.top - 11, m_hLogoIcon
			, 16, 16, 0, NULL, DI_NORMAL);
	}
	// 绘制文本
	if(NULL == m_fontBold.GetSafeHandle())
	{
		LOGFONT		logFont		= {0};

		GetFont()->GetLogFont(&logFont);
		logFont.lfWeight = FW_BOLD;
		m_fontBold.CreateFontIndirect(&logFont);
	}
	if(bSel)
	{
		pDC->SelectObject(&m_fontBold);
		pDC->SetTextColor(RGB(0x67, 0x01, 0x01));
	}
	rtText = rect;
	rtText.top = rtText.bottom - 25;
	pDC->DrawText(lpName, _tcslen(lpName), &rtText, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
}

//////////////////////////////////////////////////////////////////////////
// 设置徽标
void CEHomeImgList::SetLogoIcon(HICON hIcon)
{
	m_hLogoIcon = hIcon;
}
void CEHomeImgList::OnMouseMove(UINT nFlags, CPoint point)
{
	if(FALSE == m_bTrackMouse)
	{
		TRACKMOUSEEVENT			track		= {0};

		track.cbSize = sizeof(TRACKMOUSEEVENT);
		track.dwFlags = TME_LEAVE;
		track.dwHoverTime = 1;
		track.hwndTrack = GetSafeHwnd();

		m_bTrackMouse = TrackMouseEvent(&track);
	}

	int			nOld		= m_nSelItem;
	int			nNew		= HitTest(point);

	if(nOld != nNew)
	{
		CRect		rect;

		m_nSelItem = nNew;
		if(-1 != nOld)
		{
			GetItemRect(nOld, &rect, LVIR_BOUNDS);
			InvalidateRect(&rect, FALSE);
		}
		if(-1 != nNew)
		{
			GetItemRect(nNew, &rect, LVIR_BOUNDS);
			InvalidateRect(&rect, FALSE);
		}
	}

	CListCtrl::OnMouseMove(nFlags, point);
}
