// PlayGifStatic.cpp : implementation file
//

#include "stdafx.h"
#include "PlayGifStatic.h"
#include ".\playgifstatic.h"


// CPlayGifStatic

IMPLEMENT_DYNAMIC(CPlayGifStatic, CStatic)
CPlayGifStatic::CPlayGifStatic()
{
	m_pImage = NULL;
	m_pDimensionIDs = NULL;
	m_FrameCount = 0;
	m_pItem = NULL;
	m_iCurrentFrame = 0;
}

CPlayGifStatic::~CPlayGifStatic()
{
	Clear();
}


BEGIN_MESSAGE_MAP(CPlayGifStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CPlayGifStatic message handlers
BOOL CPlayGifStatic::Load(LPCTSTR lpStr)
{
	Clear();

	m_pImage = new Gdiplus::Image(CT2W(lpStr));
	
	return Load(m_pImage);
}

BOOL CPlayGifStatic::Load(HINSTANCE hInst, LPCSTR lpName, LPCSTR lpType)
{
	HRSRC				hRsrc;
	HGLOBAL				hResorce;
	DWORD				dwSize;
	HGLOBAL				hStream;
	CComPtr<IStream>	spStream;

	Clear();

	hRsrc = FindResourceA(hInst, lpName, lpType);
	if(NULL == hRsrc)
		return FALSE;
	hResorce = LoadResource(hInst, hRsrc);
	if(NULL == hResorce)
		return FALSE;
	// 构造Stream
	dwSize = SizeofResource(hInst, hRsrc);
	hStream = GlobalAlloc(GMEM_FIXED, dwSize);
	CreateStreamOnHGlobal(hStream, TRUE, &spStream);
	if(spStream == NULL)
	{
		GlobalFree(hStream);
		return FALSE;
	}
	spStream->Write(LockResource(hResorce), dwSize, &dwSize);
	UnlockResource(hResorce);
	FreeResource(hResorce);
	// 创建图片
	m_pImage = new Gdiplus::Image(spStream);

	return Load(m_pImage);
}

BOOL CPlayGifStatic::Load(Gdiplus::Image* pImage)
{
	if(NULL == pImage)
		return FALSE;

	m_pImage = pImage;
	UINT count = m_pImage->GetFrameDimensionsCount();
	m_pDimensionIDs = new GUID[count];
	m_pImage->GetFrameDimensionsList(m_pDimensionIDs, count);
	m_FrameCount = m_pImage->GetFrameCount(&m_pDimensionIDs[0]);
	//PropertyTagFrameDelay是GDI+中预定义的一个GIG属性ID值，表示标签帧数据的延迟时间
	UINT TotalBuffer = m_pImage->GetPropertyItemSize(PropertyTagFrameDelay);
	m_pItem = (Gdiplus::PropertyItem *)malloc(TotalBuffer);
	m_pImage->GetPropertyItem(PropertyTagFrameDelay, TotalBuffer, m_pItem);

	return TRUE;
}

BOOL CPlayGifStatic::Play()
{
	if(NULL == m_pItem)
		return FALSE;

	m_iCurrentFrame = 0;
	SetTimer(1, ((UINT *)m_pItem[0].value)[m_iCurrentFrame] * 10, NULL);
	return TRUE;
}

BOOL CPlayGifStatic::Stop()
{
	KillTimer(1);
	return TRUE;
}

void CPlayGifStatic::Clear()
{
	if(NULL != m_pImage)
		delete m_pImage;
	if(NULL != m_pDimensionIDs)
		delete [] m_pDimensionIDs;
	if(NULL != m_pItem)
		free(m_pItem);
}

void CPlayGifStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	if(NULL == m_pImage)
	{
		return;
	}

	Gdiplus::Graphics		g(dc.GetSafeHdc());
	CRect					rtClient;
	int						nWidth			= 0;
	int						nHeight			= 0;
	Gdiplus::Color			col(255, 0, 0, 0);
	HDC						hDC			= g.GetHDC();

	SendMessage(WM_ERASEBKGND, (WPARAM)hDC);
	g.ReleaseHDC(hDC);
	GetClientRect(&rtClient);
	nWidth = m_pImage->GetWidth();
	nHeight = m_pImage->GetHeight();
	g.DrawImage(m_pImage
		, rtClient.left + (rtClient.Width() - nWidth) / 2
		, rtClient.top + (rtClient.Height() - nHeight) / 2
		, nWidth, nHeight);
}

void CPlayGifStatic::OnTimer(UINT nIDEvent)
{
	KillTimer(nIDEvent);

	GUID	Guid		= Gdiplus::FrameDimensionTime;
	
	if(NULL == m_pImage || NULL == m_pItem)
		return;
	m_pImage->SelectActiveFrame(&Guid, m_iCurrentFrame);
	SetTimer(1, ((UINT *)m_pItem[0].value)[m_iCurrentFrame] * 10, NULL);
	m_iCurrentFrame = (++m_iCurrentFrame) % m_FrameCount;
	Invalidate();
}


BOOL CPlayGifStatic::OnEraseBkgnd(CDC* pDC)
{
	POINT		offPt;
	RECT		rtWnd;
	RECT		rtPre;
	CRgn		rgn;
	int			nSave;

	GetWindowRect(&rtWnd);
	GetParent()->GetWindowRect(&rtPre);
	offPt.x = rtWnd.left - rtPre.left;
	offPt.y = rtWnd.top - rtPre.top;

	nSave = pDC->SaveDC();

	rgn.CreateRectRgn(0, 0, rtWnd.right-rtWnd.left, rtWnd.bottom-rtWnd.top);
	pDC->SetWindowOrg(offPt);
	pDC->SelectClipRgn(&rgn);
	GetParent()->SendMessage(WM_ERASEBKGND, (WPARAM)pDC->GetSafeHdc(), 0);

	pDC->RestoreDC(nSave);
	return TRUE;
}
