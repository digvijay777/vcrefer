#pragma once

#define UM_MYTHREADBASE		(WM_USER+500)
#define UM_TEST				UM_MYTHREADBASE+1


// CMyTestThread

class CMyTestThread : public CWinThread
{
	DECLARE_DYNCREATE(CMyTestThread)

public:
	CMyTestThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMyTestThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void		OnTest(WPARAM wParam, LPARAM lParam);
};


