// ProcessAsUserDlg.h : 头文件
//

#pragma once


// CProcessAsUserDlg 对话框
class CProcessAsUserDlg : public CDialog
{
// 构造
public:
	CProcessAsUserDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PROCESSASUSER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	HANDLE OpenSystemProcess() ;
public:
	afx_msg void OnBnClickedButton1();
	BOOL	EnableProcPrivilege(DWORD PID);
	BOOL	EnablePrivilege(TCHAR * name, BOOL fEnable = TRUE, HANDLE hToken = NULL);
};
