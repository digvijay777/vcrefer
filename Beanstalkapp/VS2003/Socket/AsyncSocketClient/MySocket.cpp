#include "StdAfx.h"
#include ".\mysocket.h"
#include "Resource.h"

CMySocket* g_pSocket = NULL;

CMySocket::CMySocket(HWND hDlg)
{
	m_hDlg = hDlg;
}

CMySocket::~CMySocket(void)
{
}

void CMySocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	char	buff[100]	= {0};
	if(Receive(buff, 100, 0))
	{
		SendDlgItemMessage(m_hDlg, IDC_LT_MSG, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)buff);
		SendDlgItemMessage(m_hDlg, IDC_LT_MSG, WM_VSCROLL, SB_BOTTOM, 0);
	}
	//CAsyncSocket::OnReceive(nErrorCode);
}

void CMySocket::OnClose(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	this->Close();
	EnableWindow(GetDlgItem(m_hDlg, IDC_BT_CLOSE), FALSE);
	EnableWindow(GetDlgItem(m_hDlg, IDC_BT_SEND), FALSE);
	EnableWindow(GetDlgItem(m_hDlg, IDC_BT_CONNECT), TRUE);
	delete this;
	g_pSocket = NULL;
}

void CMySocket::OnConnect(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(0 != nErrorCode)
	{
		delete this;
		g_pSocket = NULL;
		AfxMessageBox("��������ʧ��.");
		return;
	}
	EnableWindow(GetDlgItem(m_hDlg, IDC_BT_CLOSE), TRUE);
	EnableWindow(GetDlgItem(m_hDlg, IDC_BT_SEND), TRUE);
	EnableWindow(GetDlgItem(m_hDlg, IDC_BT_CONNECT), FALSE);
	CAsyncSocket::OnConnect(nErrorCode);
}
