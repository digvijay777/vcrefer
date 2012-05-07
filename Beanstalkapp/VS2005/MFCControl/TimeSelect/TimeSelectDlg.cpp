// TimeSelectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TimeSelect.h"
#include "TimeSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTimeSelectDlg �Ի���




CTimeSelectDlg::CTimeSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeSelectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTimeSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_2, m_ctlRichTest);
}

BEGIN_MESSAGE_MAP(CTimeSelectDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CTimeSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTimeSelectDlg ��Ϣ�������

BOOL CTimeSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	BYTE		szWeekFlag[21]		= {0};
	BYTE		szWeekFlagEx[84]	= {0};

	szWeekFlag[1] = 0xff;
	szWeekFlag[4] = 0xff;
	szWeekFlag[7] = 0xff;
	szWeekFlag[10] = 0xff;
	szWeekFlag[13] = 0xff;
	szWeekFlag[16] = 0xff;
	szWeekFlag[19] = 0xff;
	m_timesel.SetWeekFlag(szWeekFlag, 21);
	szWeekFlagEx[12 * 0 + 8 / 2] = 0xa;
	szWeekFlagEx[12 * 1 + 8 / 2] = 0xa;
	szWeekFlagEx[12 * 2 + 8 / 2] = 0xa;
	szWeekFlagEx[12 * 3 + 8 / 2] = 0xa;
	szWeekFlagEx[12 * 4 + 8 / 2] = 0xa;
	szWeekFlagEx[12 * 5 + 8 / 2] = 0xa;
	szWeekFlagEx[12 * 6 + 8 / 2] = 0xa;
	m_ctlTime.SetWeekFlagEx(szWeekFlagEx, 84);
	m_timesel.SubclassWindow(::GetDlgItem(GetSafeHwnd(), IDC_STATIC_1));
	m_ctlTime.SubclassWindow(::GetDlgItem(GetSafeHwnd(), IDC_STATIC_3));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTimeSelectDlg::OnPaint()
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
HCURSOR CTimeSelectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTimeSelectDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	static	int		i = 0;

	m_timesel.EnableWindow(i);
	i = (i + 1) % 2;
}
