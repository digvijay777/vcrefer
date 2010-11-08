// ImgTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ImgTabCtrl.h"
#include ".\imgtabctrl.h"


// CImgTabCtrl

IMPLEMENT_DYNAMIC(CImgTabCtrl, CTabCtrl)
CImgTabCtrl::CImgTabCtrl()
{
}

CImgTabCtrl::~CImgTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CImgTabCtrl, CTabCtrl)
END_MESSAGE_MAP()


// ��ʼ��TAB�ؼ�
BOOL CImgTabCtrl::InitTabCtrl(SIZE *siItem, UINT nIDNorBmp /* = -1 */, UINT nIDSelBmp /* = -1 */, UINT nIDBkBmp /* = -1 */)
{
	// �ı���ʽ
	ModifyStyle(TCS_MULTILINE | WS_TABSTOP, TCS_FIXEDWIDTH | TCS_BUTTONS | TCS_SINGLELINE , 0);
	// ����ͼƬ
	if(-1 != nIDNorBmp)
		m_bmpItemNormal.LoadBitmap(nIDNorBmp);
	if(-1 != nIDSelBmp)
		m_bmpItemSel.LoadBitmap(nIDSelBmp);
	if(-1 != nIDBkBmp)
		m_bmpBk.LoadBitmap(nIDBkBmp);
	return TRUE;
}

