/*
 *	�ʼ�������
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
 *	�����ʼ�
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
	}	// ��������
	
	sock.Create();
	TimeOut = 10000;
	sock.SetSockOpt(SO_SNDTIMEO,(char *)&TimeOut,sizeof(TimeOut));
	TimeOut = 3000;//���ý��ճ�ʱ6�� 
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
 *	�õ����յĴ���
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

	// �õ�����ͷ������
	nRet = atoi(szRecv);

	// ���Ʒ��ش�
	if(NULL != lpRecv)
	{
		strncpy(lpRecv, szRecv, min(nLength, (int)strlen(szRecv)));
	}

	TRACE("request: %s\n", szRecv);
	sk.m_hSocket = INVALID_SOCKET;
	return nRet;
}

/*
 *	�������ݵ�������
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
 *	�õ��ʼ��ĵ�ַ
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
 *	�õ�������IP
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
 *	������֤
 */
BOOL CXSendMail::SendValidate(SOCKET sock /*= INVALID_SOCKET*/)
{
	return TRUE;
}
/*
 *	����ͷ����Ϣ
 */
BOOL CXSendMail::SendHeader(SOCKET sock /* = INVALID_SOCKET */)
{
	CString			strTmp;
	int				nCode			= 0;

	// �����Ự
	strTmp.Format("helo CXSendMail\r\n");
	if(FALSE == SendToServer(strTmp.GetBuffer(), strTmp.GetLength(), sock))
		return FALSE;
	nCode = GetRequestCode(sock);
	if(250 != nCode)
		return FALSE;

	// ��֤�û�
	if(FALSE == SendValidate(sock))
		return FALSE;

	// �ʼ�������
	strTmp.Format("mail from: <%s>\r\n", m_strMailFrom.GetBuffer());
	if(FALSE == SendToServer(strTmp.GetBuffer(), strTmp.GetLength(), sock))
		return FALSE;
	nCode =  GetRequestCode(sock);
	if(250 != nCode)
		return FALSE;

	// �ռ���
	strTmp.Format("rcpt to: <%s>\r\n", m_strSendTo.GetBuffer());
	if(FALSE == SendToServer(strTmp.GetBuffer(), strTmp.GetLength(), sock))
		return FALSE;
	nCode =  GetRequestCode(sock);
	if(250 != nCode)
		return FALSE;

	// ����DATA
	strTmp.Format("data\r\n");
	if(FALSE == SendToServer(strTmp.GetBuffer(), strTmp.GetLength(), sock))
		return FALSE;
	nCode =  GetRequestCode(sock);
	if(354 != nCode)
		return FALSE;

	return TRUE;
}

/*
 *	�����ʼ���
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
 *	�õ�����
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
 *	��������
 */
BOOL CXSendMail::Send(SOCKET sock)
{
	CString			str;
	int				nCode;

	nCode =  GetRequestCode(sock);

	if(220 != nCode)
		return FALSE;
	// ��������
	if(FALSE == SendHeader(sock))
	{
		return FALSE;
	}

	// ��������
	if(FALSE == SendBody(sock))
	{
		return FALSE;
	}

	// ��������
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