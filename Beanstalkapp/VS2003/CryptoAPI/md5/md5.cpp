// md5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <Windows.h>
#include <Wincrypt.h>

#pragma comment(lib, "Advapi32.lib")

void DataToHex(BYTE* pData, DWORD dwData, LPSTR lpOut, DWORD dwStr);
std::string md5(LPVOID lpData, DWORD dwSize);

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 1)
	{
		printf("参数不正确.\n");
		system("pause");
		return -1;
	}

	for(int i = 1; i < argc; i++)
	{
		printf("'%s'的MD5是: %s\r\n", argv[i], md5(argv[i], strlen(argv[i])).c_str());
	}

	system("pause");
	return 0;
}

std::string md5(LPVOID lpData, DWORD dwSize)
{
	HCRYPTPROV		prov		= NULL;
	HCRYPTHASH		hash		= NULL;
	CHAR			szMd5[64]	= {0};
	BOOL			bRes		= FALSE;
	BYTE			btMd5[64]	= {0};
	DWORD			dwMd5		= sizeof(btMd5);

	bRes = CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
	if(FALSE == bRes)
		goto md5_End;

	bRes = CryptCreateHash(prov, CALG_MD5, 0, 0, &hash);
	if(FALSE == bRes)
		goto md5_End;

	bRes = CryptHashData(hash, (BYTE*)lpData, dwSize, 0);
	if(FALSE == bRes)
		goto md5_End;

	bRes = CryptGetHashParam(hash, HP_HASHVAL, btMd5, &dwMd5, 0);
	if(FALSE == bRes)
		goto md5_End;

	DataToHex(btMd5, dwMd5, szMd5, sizeof(szMd5));
md5_End:
	if(NULL != hash)
		CryptDestroyHash(hash);
	if(NULL != prov)
		CryptReleaseContext(hash, 0);

	return szMd5;
}

#define HEX			"0123456789ABCDEF"

void DataToHex(BYTE* pData, DWORD dwData, LPSTR lpOut, DWORD dwStr)
{
	int		nIndexD		= 0;
	int		nIndexS		= 0;

	for(; nIndexD < dwData && nIndexS < (dwStr-1); nIndexD++)
	{
		CHAR	ch1			= HEX[pData[nIndexD]>>4];
		CHAR	ch2			= HEX[pData[nIndexD]&0xF];

		lpOut[nIndexS++] = ch1;
		if(nIndexS < (dwStr-1))
			lpOut[nIndexS++] = ch2;
	}

	lpOut[nIndexS] = 0;
}