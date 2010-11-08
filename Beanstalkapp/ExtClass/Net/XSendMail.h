#pragma once
#include <Afxsock.h>
#include <vector>

using namespace std;

class CXSendMail
{
public:
	CXSendMail();
	~CXSendMail();
public:
	CString			m_strMailFrom;		
	CString			m_strSendTo;
	CString			m_strSendCC;		// ³­ËÍ
	CString			m_strSubject;
	CString			m_strData;
	CString			m_strFromName;
	CString			m_strSendSvr;
	
	CString			m_strProxyIP;
	int				m_nProxyPort;
	BOOL			m_bUseProxy;

public:
    BOOL	Send();
	BOOL	Send(SOCKET sock);
	BOOL	GetSMTPServer(CString strMailAddr, vector<CString>& vctSMTPSvr);
	BOOL	GetSMTPIP(CString strMailAddr, vector<CString>& vctSMTPIP);
	BOOL	GetDate(CString& strDate);

protected:
	int		GetRequestCode(SOCKET sock, LPSTR lpRecv = NULL, int nLength = 0);
	BOOL	SendToServer(LPCSTR lpData, int nLength, SOCKET sock);
	BOOL	SendHeader(SOCKET sock);
	BOOL	SendBody(SOCKET sock);
	BOOL	SendValidate(SOCKET sock);
private:
	CString			m_strSMTPServer;
};