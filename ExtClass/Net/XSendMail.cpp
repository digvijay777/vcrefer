/*
 *	邮件发送类
 *
 *  Author: xiaxiangjun
 *  Date: 2010-01-12
 */
#include "stdafx.h"
#include "XSendMail.h"
#include "NetAdapter.h"
#include <Windns.h>

#pragma comment(lib, "Dnsapi.lib")

CXSendMail::CXSendMail()
{
	m_bUseProxy = FALSE;
}

CXSendMail::~CXSendMail()
{

}

/*
 *	发送邮件
 */
BOOL CXSendMail::Send()
{
	vector<CString>		vctDNSSvr;
	CString				str;
	CSocket				sock;
	int					TimeOut;

	if(m_strSendSvr.GetLength() > 1)
	{
		vctDNSSvr.push_back(m_strSendSvr);
	}
	else if(FALSE == GetSMTPServer(m_strSendSvr, vctDNSSvr))
	{
		TRACE("Send: GetSMTPServer failed %d\n", GetLastError());
		return FALSE;
	}	// 建立连接
	
	sock.Create();
	TimeOut = 10000;
	sock.SetSockOpt(SO_SNDTIMEO,(char *)&TimeOut,sizeof(TimeOut));
	TimeOut = 3000;//设置接收超时6秒 
	sock.SetSockOpt(SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut));

	for(int i = 0; i < (int)vctDNSSvr.size(); i++)
	{
		if(FALSE == sock.Connect(vctDNSSvr[i].GetBuffer(), 25))
		{
			TRACE("Send: Connect %s filed %d\n", vctDNSSvr[i].GetBuffer(), GetLastError());
			continue;
		}
		
		if(FALSE == Send(sock.m_hSocket))
			continue;

		sock.Close();
		return TRUE;
	}

	sock.Close();

	return FALSE;
}

/*
 *	得到接收的代码
 */
int CXSendMail::GetRequestCode(SOCKET sock, LPSTR lpRecv /* = NULL */, int nLength /* = 0 */)
{
	CHAR			szRecv[1024]		= {0};
	int				nRecv				= 0;
	int				nRet				= -1;
	CSocket			sk;

	if(INVALID_SOCKET == sock)
		return -1;

	sk.m_hSocket = sock;

	nRecv = sk.Receive(szRecv, sizeof(szRecv));
	if(nRecv <= 0)
	{
		TRACE("GetRequestCode: Receive failed %d\n", GetLastError());
		sk.m_hSocket = INVALID_SOCKET;
		return -1;
	}

	// 得到返回头的数字
	nRet = atoi(szRecv);

	// 复制返回串
	if(NULL != lpRecv)
	{
		strncpy(lpRecv, szRecv, min(nLength, (int)strlen(szRecv)));
	}

	TRACE("request: %s\n", szRecv);
	sk.m_hSocket = INVALID_SOCKET;
	return nRet;
}

/*
 *	发送数据到服务器
 */
BOOL CXSendMail::SendToServer(LPCSTR lpData, int nLength, SOCKET sock /*= INVALID_SOCKET*/)
{
	if(INVALID_SOCKET == sock)
		return FALSE;

	CSocket		sk;

	sk.m_hSocket = sock;

	if(SOCKET_ERROR == sk.Send(lpData, nLength))
	{
		TRACE("SendToServer: Send failed %d\n", GetLastError());
		sk.m_hSocket = INVALID_SOCKET;
		return FALSE;
	}

	sk.m_hSocket = INVALID_SOCKET;
	return TRUE;
}
/*
 *	得到邮件的地址
 */
BOOL CXSendMail::GetSMTPServer(CString strMailAddr, vector<CString>& vctSMTPSvr)
{
	vctSMTPSvr.clear();

	DNS_STATUS		status;               //Return value of  DnsQuery_A() function.
	PDNS_RECORD		pDnsRecord;          //Pointer to DNS_RECORD structure.
	PIP4_ARRAY		pSrvList		= NULL;      //Pointer to IP4_ARRAY structure.
	int				nA				= -1;

	nA = strMailAddr.Find('@');
	if(-1 != nA)
		strMailAddr = strMailAddr.Mid(nA+1);

	status = DnsQuery(strMailAddr.GetBuffer(), DNS_TYPE_MX, DNS_QUERY_STANDARD, pSrvList, &pDnsRecord, NULL);
	if(0 == status && NULL != pDnsRecord)
	{
		PDNS_RECORD			pNext	 = NULL;

		for(pNext = pDnsRecord; NULL != pNext; pNext = pNext->pNext)
		{
			vctSMTPSvr.push_back(pNext->pName);
		}
		DnsRecordListFree(pDnsRecord, DnsFreeRecordListDeep);

		if(vctSMTPSvr.size() > 0)
			return TRUE;
	}

	return FALSE;
}
/*
 *	得到服务器IP
 */
BOOL CXSendMail::GetSMTPIP(CString strMailAddr, vector<CString>& vctSMTPIP)
{
	vctSMTPIP.clear();

	DNS_STATUS		status;               //Return value of  DnsQuery_A() function.
	PDNS_RECORD		pDnsRecord;          //Pointer to DNS_RECORD structure.
	PIP4_ARRAY		pSrvList		= NULL;      //Pointer to IP4_ARRAY structure.
	int				nA				= -1;

	nA = strMailAddr.Find('@');
	if(-1 != nA)
		strMailAddr = strMailAddr.Mid(nA+1);

	status = DnsQuery(strMailAddr.GetBuffer(), DNS_TYPE_MX, DNS_QUERY_STANDARD, pSrvList, &pDnsRecord, NULL);
	if(0 == status && NULL != pDnsRecord)
	{
		PDNS_RECORD			pNext	 = NULL;

		for(pNext = pDnsRecord; NULL != pNext; pNext = pNext->pNext)
		{
			hostent*	phost	= NULL;
			char*		pIP		= NULL;

			phost = gethostbyname(pNext->pName);
			if(NULL == phost)
				continue;
			pIP	= inet_ntoa(*(struct   in_addr   *)(phost->h_addr_list[0]));  
			if(NULL == pIP)
				continue;
			vctSMTPIP.push_back(pIP);
		}
		DnsRecordListFree(pDnsRecord, DnsFreeRecordListDeep);

		if(vctSMTPIP.size() > 0)
			return TRUE;
	}

	return FALSE;
}
/*
 *	发送验证
 */
BOOL CXSendMail::SendValidate(SOCKET sock /*= INVALID_SOCKET*/)
{
	return TRUE;
}
/*
 *	发送头部信息
 */
BOOL CXSendMail::SendHeader(SOCKET sock /* = INVALID_SOCKET */)
{
	CString			strTmp;
	int				nCode			= 0;

	// 建立会话
	strTmp.Format("helo CXSendMail\r\n");
	if(FALSE == SendToServer(strTmp.GetBuffer(), strTmp.GetLength(), sock))
		return FALSE;
	nCode = GetRequestCode(sock);
	if(250 != nCode)
		return FALSE;

	// 验证用户
	if(FALSE == SendValidate(sock))
		return FALSE;

	// 邮件发送人
	strTmp.Format("mail from: <%s>\r\n", m_strMailFrom.GetBuffer());
	if(FALSE == SendToServer(strTmp.GetBuffer(), strTmp.GetLength(), sock))
		return FALSE;
	nCode =  GetRequestCode(sock);
	if(250 != nCode)
		return FALSE;

	// 收件人
	strTmp.Format("rcpt to: <%s>\r\n", m_strSendTo.GetBuffer());
	if(FALSE == SendToServer(strTmp.GetBuffer(), strTmp.GetLength(), sock))
		return FALSE;
	nCode =  GetRequestCode(sock);
	if(250 != nCode)
		return FALSE;

	// 发送DATA
	strTmp.Format("data\r\n");
	if(FALSE == SendToServer(strTmp.GetBuffer(), strTmp.GetLength(), sock))
		return FALSE;
	nCode =  GetRequestCode(sock);
	if(354 != nCode)
		return FALSE;

	return TRUE;
}

/*
 *	发送邮件体
 */
BOOL CXSendMail::SendBody(SOCKET sock /* = INVALID_SOCKET */)
{
	CString		strBody;
	int			nCode;
	CString		strDate;

	GetDate(strDate);
	strBody.Format("From: \"%s\" <%s>\r\n"
		"Date: %s\r\n"
		"To: <%s>\r\n"
		"Subject: %s\r\n"
		"\r\n%s"
		"\r\n\r\n.\r\n",
		m_strFromName.GetBuffer(),
		m_strMailFrom.GetBuffer(),
		strDate.GetBuffer(),
		m_strSendTo.GetBuffer(),
		m_strSubject.GetBuffer(),
		m_strData.GetBuffer());

	if(FALSE == SendToServer(strBody.GetBuffer(), strBody.GetLength(), sock))
		return FALSE;

	nCode =  GetRequestCode(sock);
	if(250 != nCode)
		return FALSE;

	return TRUE;
}

/*
 *	得到日期
 */
BOOL CXSendMail::GetDate(CString& strDate)
{
	__time64_t		time		= _time64(NULL);
	tm*				pToday		= _localtime64(&time);

	strftime(strDate.GetBufferSetLength(100), 100, "%a, %#d %b %Y %H:%M:%S +0800", pToday);
	strDate.ReleaseBuffer();

	return TRUE;
}

/*
 *	发送数据
 */
BOOL CXSendMail::Send(SOCKET sock)
{
	CString			str;
	int				nCode;

	nCode =  GetRequestCode(sock);

	if(220 != nCode)
		return FALSE;
	// 发送数据
	if(FALSE == SendHeader(sock))
	{
		return FALSE;
	}

	// 发送主体
	if(FALSE == SendBody(sock))
	{
		return FALSE;
	}

	// 结束发送
	str.Format("quit\r\n");
	if(FALSE == SendToServer(str.GetBuffer(), str.GetLength(), sock))
	{
		return FALSE;
	}
	nCode =  GetRequestCode(sock);
	if(221 != nCode)
	{
		return FALSE;
	}

	return TRUE;
}