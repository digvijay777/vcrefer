// SendMail.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SendOneMail.h"
#include <sys/stat.h>

int _tmain(int argc, _TCHAR* argv[])
{
	CSendOneMail		mail;
	CHAR				szModName[512]		= {0};
	CHAR				szDir[512]			= {0};
	CHAR*				pName				= 0;

	GetModuleFileName(NULL, szModName, sizeof(szModName));
	strcpy(szDir, szModName);
	pName = strrchr(szDir, '\\');
	if(NULL == pName)
		pName = strrchr(szDir, '/');
	if(NULL != pName)
		*pName++ = 0;
	SetCurrentDirectoryA(szDir);

	FILE*		pFile		= fopen("mail/mail.html", "r+");
	if(NULL == pFile)
		return -1;
	struct stat		st		= {0};

	fstat(pFile->_file, &st);
	BYTE*	pData		= new BYTE[st.st_size+1];

	memset(pData, 0, st.st_size+1);
	fread(pData, 1, st.st_size, pFile);
	fclose(pFile);


	mail.SetDirectory("mail/mail.html");
	mail.SetMailServer("mail.txtws.com", "xiaxj@txtws.com", "xxj!@#06");
	mail.SetMailFrom("xiaxj@txtws.com", "txtw");
	mail.SetMailTo("xiaxj@txtws.com;", "xiaxiangjun", "test");
	mail.SetTemplate(pData, st.st_size+1);

	mail.Send();

	delete [] pData;
	return 0;
}

