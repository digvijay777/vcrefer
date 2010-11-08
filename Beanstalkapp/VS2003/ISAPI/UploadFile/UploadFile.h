#pragma once


// UploadFile.h - Header file for your Internet Information Server
//    UploadFile Extension

#include "resource.h"

class CUploadFileExtension : public CHttpServer
{
public:
	CUploadFileExtension();
	~CUploadFileExtension();

	// Overrides
public:
	virtual BOOL GetExtensionVersion(HSE_VERSION_INFO* pVer);
	virtual BOOL TerminateExtension(DWORD dwFlags);

	// TODO: Add handlers for your commands here.
	// For example:

	void Default(CHttpServerContext* pCtxt);
	void Upload(CHttpServerContext* pCtxt, LPSTR lpText, int nNumber);

	DECLARE_PARSE_MAP()
	virtual DWORD HttpExtensionProc(EXTENSION_CONTROL_BLOCK *pECB);
};
