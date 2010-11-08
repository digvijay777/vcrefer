// ProcessListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CreateChildProcess.h"
#include "ProcessListDlg.h"
#include ".\processlistdlg.h"
#include <tlhelp32.h>

// CProcessListDlg �Ի���

IMPLEMENT_DYNAMIC(CProcessListDlg, CDialog)
CProcessListDlg::CProcessListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessListDlg::IDD, pParent)
{
	m_dwProcessID = 0;
}

CProcessListDlg::~CProcessListDlg()
{
}

void CProcessListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
}


BEGIN_MESSAGE_MAP(CProcessListDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CProcessListDlg ��Ϣ�������

void CProcessListDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int			nItem	= -1;
	CString		strItem;

	nItem = m_list.GetNextItem(-1, LVNI_SELECTED);
	if(-1 != nItem)
	{
		strItem = m_list.GetItemText(nItem, 0);
		m_dwProcessID = atoi(strItem.GetBuffer());
		OnOK();
	}
}


BOOL CProcessListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��ʼ���б��
	m_list.InsertColumn(0, "PID", LVCFMT_LEFT, 50);
	m_list.InsertColumn(1, "����", LVCFMT_LEFT, 150);
	m_list.InsertColumn(2, "�û���", LVCFMT_LEFT, 100);

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	FillList();
	return TRUE;
}

/*
 *	�������б�
 *
 *  ���ʱ��:2009-11-8
 */
void CProcessListDlg::FillList()
{
	m_list.DeleteAllItems();

	// �оٽ���
	HANDLE			hProcessList		= NULL;
	PROCESSENTRY32	process				= {0};
	BOOL			bRes				= FALSE;
	CString			strItem				= 0;
	int				nCurItem			= 0;

	hProcessList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(INVALID_HANDLE_VALUE == hProcessList)
		return;
	process.dwSize = sizeof(PROCESSENTRY32);
	bRes = Process32First(hProcessList, &process);
	while(FALSE != bRes)
	{
		// ����ö�ٵĽ���
		strItem.Format("%d", process.th32ProcessID);
		nCurItem = m_list.InsertItem(nCurItem, strItem);
		m_list.SetItemText(nCurItem, 1, process.szExeFile);
		// �õ������û���
		HANDLE		hProcess		= NULL;
		HANDLE		hToken			= NULL;
		DWORD		dwRes			= 0;
		SID_AND_ATTRIBUTES		sid[10]	= {0};
		SID_NAME_USE			snu;
		CString					strTemp;

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, process.th32ProcessID);
		if(NULL != hProcess)
		{
			bRes = OpenProcessToken(hProcess, TOKEN_QUERY, &hToken);
			if(TRUE == bRes)
			{
				bRes = GetTokenInformation(hToken, TokenUser, sid, sizeof(sid) * 10, &dwRes);
				if(TRUE == bRes)
				{
					bRes = ::LookupAccountSid(NULL, sid[0].Sid, strItem.GetBufferSetLength(1024), &dwRes, strTemp.GetBufferSetLength(1024), &dwRes, &snu);
					if(bRes == TRUE)
					{
						strItem.ReleaseBuffer();
						m_list.SetItemText(nCurItem, 2, strItem);
					}
				}
				CloseHandle(hToken);
			}
			CloseHandle(hProcess);
		}
		nCurItem++;
		// ������һ��
		bRes = Process32Next(hProcessList, &process);
	}
	CloseHandle(hProcessList);
}