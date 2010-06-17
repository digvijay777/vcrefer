#pragma once
#include <Wininet.h>

class CDownFileBase
{
public:
	CDownFileBase();
	virtual ~CDownFileBase();

	enum NotifyUIEnum{NUIE_SetPorcess, NUIE_BeginDown, NUIE_EndDown};
public:
	virtual void NotifyUI(int nType, LPVOID lpParameter);
public:
	int		m_nDownError;
	BOOL	m_bDownCancel;
	BOOL	m_bForceDown;
public:
	BOOL	DoFromNetPath(LPCTSTR lpNetPath, LPCTSTR lpLocalPath, LPCTSTR lpUser = NULL, LPCTSTR lpPasswd = NULL, BOOL bLogout = FALSE);
	BOOL	DoFromHTMLServer(LPCTSTR lpURL, LPCTSTR lpLocalPath);

public:
	void	ReplaceString(LPTSTR lpStr, TCHAR src, TCHAR des);
	void	NetBIOS_GetPath(LPCTSTR lpNetFile, LPSTR lpNet);
	BOOL	NetBIOS_Login(LPCTSTR lpNet, LPCTSTR lpUser, LPCTSTR lpPasswd);
	BOOL	NetBIOS_Logout(LPCTSTR lpNet);

	BOOL	Http_Init(HINTERNET& hOpen, HINTERNET& hConnect, HINTERNET& hRequest, LPCTSTR lpSvrName, LPCTSTR lpObjectName);
	void	Http_SplitURL(LPCTSTR lpURL, LPTSTR lpSvrName, LPTSTR lpObjName);
	void	Http_GetLocalPath(LPCTSTR lpPrePath, LPCTSTR lpURL, LPTSTR lpPath);
	DWORD	Http_GetFileSize(HINTERNET& hRequest);
	BOOL	Http_IsNeedDown(LPCTSTR lpPath, DWORD dwSize);
	DWORD	Http_DownFile(HINTERNET& hRequest, HANDLE hFile, DWORD dwSize);
};

#define DOWNFILE_READY				-1		// 未操作
#define DOWNFILE_SUCCESS			0		// 下载成功

//  0 : 下载成功  1 :连接网络失败 2 :取得文件长度失败 3: 创建文件失败 4 .强制退出 . 5 网络断开
#define DOWNLOAD_ALL_SUCCESS			0
#define DOWNLOAD_CONNECT_FAILED			1
#define DOWNLOAD_GETFILESIZE_FAILED		2
#define DOWNLOAD_CREATEFILE_FAILED		3
#define DOWNLOAD_EXIT_FAILED			4
#define DOWNLOAD_DISCONNECT_FAILED		5