// HttpGet.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HttpReader.h"
#include "HttpRequest.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CHttpReader		reader;
	ZEvtSyncSocket	sock;
	CAtlHttpClient	client;

// 	CHttpRequest::StartHttp();
// 	char*			pData		= "GET / HTTP/1.1\r\n"
// 		"Host: baidu.com\r\n"
// 		"\r\n\r\n";
	char*			pData		= "GET /v?fr=bk&ct=301989888&rn=20&pn=0&db=0&s=24&word=df HTTP/1.1\r\n"
		"Host: video.baidu.com\r\n"
		"Connection: keep-alive\r\n"
		"Cache-Control: max-age=0\r\n"
		"User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/535.19 (KHTML, like Gecko) Chrome/18.0.1025.152 Safari/535.19\r\n"
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
		"Accept-Encoding: gzip,deflate,sdch\r\n"
		"Accept-Language: zh-CN,zh;q=0.8\r\n"
		"Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3\r\n"
		"Cookie: BAIDUID=73FC12F40C9B7F187EC47DB8382040C4:FG=1; BDUSS=HR3RGRYOXlTNmtFRUsyWmlQYTkzNXhUVS1OMko5NmFBd0R4fmtsZGFoc2RncEJRQVFBQUFBJCQAAAAAAAAAAAokNxJTGu4JeXVhbmppZV94aWEAAAAAAAAAAAAAAAAAAAAAAAAAAADg2gV8AAAAAODaBXwAAAAA-WZCAAAAAAAxMC42NS4yNB00o08dNKNPM; USERID=ed8591ab5729c6927d6b3038a4054d; MCITY=-194%3A; BDUT=ty6fD05106B8F897F6CFAD42DFFDB3EAC4A01373a45df6c1; BBX=close\r\n"
		"\r\n\r\n";
	DWORD			dwSend		= strlen(pData);
	CAtlNavigateData	nData;

	nData.SetExtraHeaders("User-Agent: Mozilla/5.0 (Windows NT 5.1; rv:5.0) Gecko/20100101 Firefox/5.0\r\n"
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
		"Accept-Language: zh-cn,zh;q=0.5\r\n"
		"Accept-Encoding: gzip, deflate\r\n"
		"Accept-Charset: GB2312,utf-8;q=0.7,*;q=0.7\r\n"
		"Connection: keep-alive\r\n"
		"Cookie: BAIDUID=D05106B8F897F6CFAD42DFFDB3EAC4A0:FG=1; BDUT=ty6fD05106B8F897F6CFAD42DFFDB3EAC4A01373a45df6c1\r\n");
	nData.SetFlags(ATL_HTTP_FLAG_PROCESS_RESULT);
	client.Navigate("http://video.baidu.com/v?ct=301989888&rn=20&pn=0&db=0&s=8&word=%C9%AB%C7%E9&fr=ala0", &nData);
// 	client.NavigateChunked("http://video.baidu.com/v?ct=301989888&rn=20&pn=0&db=0&s=8&word=%C9%AB%C7%E9&fr=ala0", &nData);

	int s = client.GetStatus();
	char* r = (char *)client.GetBody();
	int		nLen = client.GetBodyLength();


	sock.Connect("video.baidu.com", 80);
	sock.Write((const unsigned char *)pData, &dwSend);

	while(true)
	{
		char		szBuf[200]		= {0};
		DWORD		dwSize			= sizeof(szBuf);

		if( !sock.Read((const unsigned char *)szBuf, &dwSize) )
		{
			break;
		}

		if( reader.AddRecv((const unsigned char *)szBuf, dwSize) )
		{
			char *p = (char *)reader.GetResponse();

			int b = 0;
			break;
		}
	}
	
	return 0;
}

