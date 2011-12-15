// TransWndDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TransWnd.h"
#include "TransWndDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTransWndDlg �Ի���




CTransWndDlg::CTransWndDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransWndDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pImage = NULL;
}

void CTransWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTransWndDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CTransWndDlg ��Ϣ�������
BOOL CTransWndDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CComPtr<IStream>		spStream;
	HRSRC					hRsrc			= FindResource(NULL, _T("bg.png"), _T("IDR_PNG"));
	HGLOBAL					hGlobal			= NULL;
	DWORD					dwSize			= SizeofResource(NULL, hRsrc);

	hGlobal = LoadResource(NULL, hRsrc);
	if(NULL != hGlobal)
	{
		CreateStreamOnHGlobal(NULL, TRUE, &spStream);
		if(NULL != spStream)
		{
			spStream->Write(LockResource(hGlobal), dwSize, NULL);
			m_pImage = Gdiplus::Image::FromStream(spStream);
			UnlockResource(hGlobal);
		}
		FreeResource(hGlobal);
	}
	if(NULL == m_pImage)
		return	TRUE;
	// �����δ���
	CDC*		pDC		= GetDC();

	UpdateLayeredImageWindow(m_pImage, pDC, this/*&m_dlgShow*/);
	ReleaseDC(pDC);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BOOL CTransWndDlg::UpdateLayeredImageWindow(Gdiplus::Image* pImage, CDC* pDC, CWnd* pWnd)
{
	CRect			rect;
	CDC				memDC;
	CBitmap			bitmap;
	int				nSaveDC;
	DWORD			dwStyleEx;

	if(NULL == pImage || !IsWindow(pWnd->GetSafeHwnd()) || NULL == pDC)
		return FALSE;
	// ��ⴰ������
	dwStyleEx = GetWindowLong(pWnd->GetSafeHwnd(), GWL_EXSTYLE);
	if(WS_EX_LAYERED != (dwStyleEx & WS_EX_LAYERED))
	{
		SetWindowLong(pWnd->GetSafeHwnd(), GWL_EXSTYLE, dwStyleEx | WS_EX_LAYERED);
	}
	// ����ͼƬ���ô����С
	pWnd->SetWindowPos(NULL, 0, 0, pImage->GetWidth(), pImage->GetHeight(), SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
	// ����DC
	pWnd->GetWindowRect(&rect);
	memDC.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	if(NULL == bitmap.GetSafeHandle() || NULL == memDC.GetSafeHdc())
		return FALSE;
	nSaveDC = memDC.SaveDC();
	memDC.SelectObject(&bitmap);
	// ����ͼƬ
	Gdiplus::Graphics		graphics(memDC.GetSafeHdc());

	graphics.DrawImage(pImage, 0, 0, pImage->GetWidth(), pImage->GetHeight());
	graphics.ReleaseHDC(memDC.GetSafeHdc());
	// ����͸������
	BLENDFUNCTION			blend		= {0};
	POINT					ptSrc		= rect.TopLeft();
	SIZE					szSrc		= {rect.Width(), rect.Height()};	// �����Сһ��Ҫ�Ǵ����
	POINT					ptDes		= {0, 0};

	blend.AlphaFormat = AC_SRC_ALPHA;
	blend.BlendFlags = 0;
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	pWnd->UpdateLayeredWindow(pDC, &ptSrc, &szSrc, &memDC, &ptDes, 0, &blend, ULW_ALPHA);
	// �ͷ���Դ
	memDC.RestoreDC(nSaveDC);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}
// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTransWndDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CTransWndDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CTransWndDlg::OnNcHitTest(CPoint point)
{
	return HTCAPTION;

	return CDialog::OnNcHitTest(point);
}

LRESULT CTransWndDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(WM_PAINT == message)
	{
		CPaintDC dc(this);

		dc.FillSolidRect(0, 0, 100, 100, RGB(0xff, 0x0, 0x0));
		return 0;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
