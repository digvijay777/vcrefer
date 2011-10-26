// SkinWndDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SkinWnd.h"
#include "SkinWndDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSkinWndDlg �Ի���




CSkinWndDlg::CSkinWndDlg(CWnd* pParent /*=NULL*/)
	: CSkinDlg(CSkinWndDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSkinWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSkinWndDlg, CSkinDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CSkinWndDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSkinWndDlg ��Ϣ�������

BOOL CSkinWndDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//CWnd::SubclassWindow()
	CSkinDlg::LoadImages(IDB_BITMAP1);
	CSkinDlg::AddContorlButton(SC_CLOSE, IDB_BITMAP2, 45, 0);
	CSkinDlg::AddContorlButton(SC_MAXIMIZE, IDB_BITMAP3, 45 + 34, 0);
	CSkinDlg::AddContorlButton(SC_MINIMIZE, IDB_BITMAP4, 45 + 34 + 34, 0);

	CSkinDlg::SetTextColor(RGB(0xff, 0xff, 0xff));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSkinWndDlg::OnPaint()
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
		CSkinDlg::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CSkinWndDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSkinWndDlg::OnBnClickedButton1()
{
	MessageBox(_T("Hello"));
}
