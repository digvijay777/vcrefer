/*
 *	MFCGDIP ����ͷ�ļ�
 */
#pragma once

#ifndef _MFCGDIP_HEADER		// �����ظ�����
#define _MFCGDIP_HEADER

//////////////////////////////////////////////////////////////////////////
// ͷ�ļ�����
// math
#include <math.h>
// GDI+
#include <gdiplus.h>
#include <Gdiplusinit.h>
#include <Gdiplusgraphics.h>

using namespace Gdiplus;
//////////////////////////////////////////////////////////////////////////
// ��Ϣ����
#define UM_BASE					(WM_USER+5120)

#define UM_DLGBASE				(UM_BASE+1)
#define UM_GETBKGNDBRUSH		UM_DLGBASE+1		// �õ�������HBRUSH

//////////////////////////////////////////////////////////////////////////
// ���ú�������

namespace MfcGdip
{
	// ͸������
	BOOL DrawTranspBkgnd(HWND hWnd, HWND hParent, Graphics* const pGraphics = NULL);
	// ��������ת��
	void ScreenToClient(HWND hWnd, LPRECT lpRect);
}

//////////////////////////////////////////////////////////////////////////
// ������

// GDI+������
class CGdipConfig
{
public:
	CGdipConfig();
	~CGdipConfig();
private:
	ULONG_PTR		m_pGdiToken;
};

extern CGdipConfig		gdipConfig;
// ͨ��RECT��
class XRect : public CRect
{
public:
	XRect();
	~XRect();
public:
	Gdiplus::Rect		Rect();
	Gdiplus::RectF		RectF();

	void operator=(Gdiplus::Rect& rt);
	void operator=(Gdiplus::RectF& rt);
};



#endif
