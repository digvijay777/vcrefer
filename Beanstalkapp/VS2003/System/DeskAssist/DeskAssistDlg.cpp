// DeskAssistDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DeskAssist.h"
#include "DeskAssistDlg.h"
#include ".\deskassistdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDeskAssistDlg �Ի���



CDeskAssistDlg::CDeskAssistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeskAssistDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDeskAssistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_ctlTab);
}

BEGIN_MESSAGE_MAP(CDeskAssistDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CDeskAssistDlg ��Ϣ�������

BOOL CDeskAssistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ctlTab.InsertItem(0, "����һ");
	m_ctlTab.InsertItem(1, "�����");
	m_ctlTab.InsertItem(2, "������");

// 	m_ctlTab.ModifyStyle(0, TCS_VERTICAL);
	/*m_ctlTab.SendMessage(LB_SETHORIZONTALEXTENT, 1000);*/
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDeskAssistDlg::OnPaint() 
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
HCURSOR CDeskAssistDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDeskAssistDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}

void CDeskAssistDlg::OnBnClickedButton1()
{

}
