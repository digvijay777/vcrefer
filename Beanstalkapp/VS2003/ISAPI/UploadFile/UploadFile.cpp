// UploadFile.cpp - Implementation file for ISAPI
//    UploadFile Extension

#include "stdafx.h"
#include "UploadFile.h"
#include ".\uploadfile.h"


// The one and only CWinApp object
// NOTE: You may remove this object if you alter your project to no
// longer use MFC in a DLL.

CWinApp theApp;


// command-parsing map

BEGIN_PARSE_MAP(CUploadFileExtension, CHttpServer)
	// TODO: insert your ON_PARSE_COMMAND() and 
	// ON_PARSE_COMMAND_PARAMS() here to hook up your commands.
	// For example:

	ON_PARSE_COMMAND(Default, CUploadFileExtension, ITS_EMPTY)
	ON_PARSE_COMMAND(Upload, CUploadFileExtension, ITS_PSTR	ITS_I2)
	ON_PARSE_COMMAND_PARAMS("name age")
	DEFAULT_PARSE_COMMAND(Default, CUploadFileExtension)
END_PARSE_MAP(CUploadFileExtension)



// The one and only CUploadFileExtension object

CUploadFileExtension theExtension;



// CUploadFileExtension implementation

CUploadFileExtension::CUploadFileExtension()
{
}

CUploadFileExtension::~CUploadFileExtension()
{
}

BOOL CUploadFileExtension::GetExtensionVersion(HSE_VERSION_INFO* pVer)
{
	// Call default implementation for initialization
	CHttpServer::GetExtensionVersion(pVer);

	// Load description string
	TCHAR sz[HSE_MAX_EXT_DLL_NAME_LEN+1];
	ISAPIVERIFY(::LoadString(AfxGetResourceHandle(),
			IDS_SERVER, sz, HSE_MAX_EXT_DLL_NAME_LEN));
	_tcscpy(pVer->lpszExtensionDesc, sz);
	return TRUE;
}

BOOL CUploadFileExtension::TerminateExtension(DWORD dwFlags)
{
	// extension is being terminated
	//TODO: Clean up any per-instance resources
	return TRUE;
}


// CUploadFileExtension command handlers

void CUploadFileExtension::Default(CHttpServerContext* pCtxt)
{
	StartContent(pCtxt);
	WriteTitle(pCtxt);

	*pCtxt << _T("This default message was produced by the Internet");
	*pCtxt << _T(" Server DLL Wizard. Edit your CUploadFileExtension::Default()");
	*pCtxt << _T(" implementation to change it.\r\n");

	EndContent(pCtxt);
}

void CUploadFileExtension::Upload(CHttpServerContext* pCtxt, LPSTR lpText, int nNumber)
{
	CHAR			szTmp[1024];

	_asm int 3

	StartContent(pCtxt);
	WriteTitle(pCtxt);

	*pCtxt << "这是我的一个测试" << "<br>";
	*pCtxt << "lpText: "<< lpText << "<br>";
	*pCtxt << "nNumber: "<< itoa(nNumber, szTmp, 10) << "<br>";
	EndContent(pCtxt);
}


// If your extension will not use MFC, you'll need this code to make
// sure the extension objects can find the resource handle for the
// module.  If you convert your extension to not be dependent on MFC,
// remove the comments around the following AfxGetResourceHandle()
// and DllMain() functions, as well as the g_hInstance global.

/****

static HINSTANCE g_hInstance;

HINSTANCE AFXISAPI AfxGetResourceHandle()
{
	return g_hInstance;
}

BOOL WINAPI DllMain(HINSTANCE hInst, ULONG ulReason,
					LPVOID lpReserved)
{
	if (ulReason == DLL_PROCESS_ATTACH)
	{
		g_hInstance = hInst;
	}

	return TRUE;
}

****/

DWORD CUploadFileExtension::HttpExtensionProc(EXTENSION_CONTROL_BLOCK *pECB)
{
	// TODO: Add your specialized code here and/or call the base class

	return CHttpServer::HttpExtensionProc(pECB);
}
