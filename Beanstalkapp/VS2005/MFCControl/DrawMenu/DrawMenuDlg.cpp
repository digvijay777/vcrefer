// DrawMenuDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DrawMenu.h"
#include "DrawMenuDlg.h"
#include "SkinMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrawMenuDlg �Ի���




CDrawMenuDlg::CDrawMenuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawMenuDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDrawMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDrawMenuDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CDrawMenuDlg::OnBnClickedButton1)
	ON_WM_INITMENUPOPUP()
END_MESSAGE_MAP()


// CDrawMenuDlg ��Ϣ�������

BOOL CDrawMenuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDrawMenuDlg::OnPaint()
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
HCURSOR CDrawMenuDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDrawMenuDlg::OnBnClickedButton1()
{
	CRect		rect;
	CMenu*		pSysMenu		= GetSystemMenu(FALSE);

	GetDlgItem(IDC_BUTTON1)->GetWindowRect(&rect);
	CSkinMenu		menu;

	menu.CreatePopupMenu();
	menu.LoadImages(IDB_MENU);
	menu.AppendMenu(MF_STRING|MF_OWNERDRAW, 100, _T("����1"));
	menu.AppendMenu(MF_STRING|MF_OWNERDRAW, 1, _T("����2"));
	menu.AppendMenu(MF_SEPARATOR|MF_OWNERDRAW, 0, _T(""));
	menu.AppendMenu(MF_STRING|MF_OWNERDRAW, 2, _T("����3"));
	menu.AppendMenu(MF_STRING|MF_OWNERDRAW, 3, _T("����4"));
	menu.AppendMenu(MF_STRING|MF_OWNERDRAW, 4, _T("����5"));
	//menu.Attach(pSysMenu->GetSafeHmenu());
	menu.TrackPopupMenu(TPM_NOANIMATION, rect.left, rect.bottom, this);
	//menu.Detach();
}

void CDrawMenuDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: Add your message handler code here
}
