// AtlHttpClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <atlhttp.h>

const char* pTestSend =	"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" \
xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" \
xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
<SOAP-ENV:Body> \
<m:HelloWorld xmlns:m=\"urn:WebTestService\" SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"> \
<bstrInput xsi:type=\"xsd:string\">String</bstrInput> \
</m:HelloWorld> \
</SOAP-ENV:Body> \
</SOAP-ENV:Envelope>";


int _tmain(int argc, _TCHAR* argv[])
{
	CAtlHttpClient		http;
	CAtlNavigateData	data;
	const BYTE*			pData;

	data.SetMethod(ATL_HTTP_METHOD_POST);
	data.SetPostData((BYTE *)pTestSend, (DWORD)strlen(pTestSend), "application/soap+xml; charset=utf-8");
// 	data.SetExtraHeaders("Content-Type: application/soap+xml; charset=utf-8\r\n");
	http.Navigate("http://192.168.1.234/WebTest/WebTest.dll", &data);
	pData = http.GetPostData();
	return 0;
}

