// DES.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <Windows.h>
#include <Wincrypt.h>
#include <assert.h>

BOOL DES(LPCSTR lpPwd, LPCBYTE lpData, DWORD nDataLen, LPBYTE pOut, BOOL bDecode = FALSE);
DWORD	MsCrypt_DES(LPCTSTR lpPwd, BYTE *pBuffer, DWORD dwDatSize, DWORD dwBufferSize, BOOL bEncrypt = TRUE);

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 2)
	{
		printf("please input string parameter.\n");
		return -1;
	}
	DWORD		dwLen		= (DWORD)strlen(argv[1])-8;

	//DES("1234567890", (BYTE*)argv[1], dwLen, NULL);
	dwLen = MsCrypt_DES("1234567890", (BYTE*)argv[1], dwLen, dwLen+8, TRUE);
	printf("des encode: %s\n", argv[1]);
	//DES("1234567890", (BYTE*)argv[1], dwLen, NULL, TRUE);
	MsCrypt_DES("1234567890", (BYTE*)argv[1], dwLen, dwLen+8, false);
	printf("des decode: %s\n", argv[1]);
	return 0;
}

DWORD	MsCrypt_DES(LPCTSTR lpPwd, BYTE *pBuffer, DWORD dwDatSize, DWORD dwBufferSize, BOOL bEncrypt /*= TRUE*/)
{
	HCRYPTPROV		hCryptProv; 
	HCRYPTKEY		hKey; 
	HCRYPTHASH		hHash;
	BOOL			bRetVal;

	bRetVal = CryptAcquireContextA(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
	if(bRetVal == FALSE)
		assert(0);

	bRetVal = CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash);
	if(bRetVal == FALSE)
		assert(0);

	bRetVal = CryptHashData(hHash, (BYTE *)lpPwd, strlen(lpPwd), 0);
	if(bRetVal == FALSE)
		assert(0);

	bRetVal = CryptDeriveKey(hCryptProv, CALG_3DES, hHash, 0, &hKey);
	if(bRetVal == FALSE)
		assert(0);

	if(bEncrypt)
	{
		bRetVal = CryptEncrypt(hKey, NULL, TRUE, 0, pBuffer, &dwDatSize, dwBufferSize);
	}
	else
	{
		bRetVal = CryptDecrypt(hKey, NULL, TRUE, 0, pBuffer, &dwDatSize);
	}
	if(bRetVal == FALSE)
		assert(0);

	if(hKey) 
		CryptDestroyKey(hKey); 
	if(hHash) 
		CryptDestroyHash(hHash); 
	if(hCryptProv) 
		CryptReleaseContext(hCryptProv, 0);

	return dwDatSize;
} 


BOOL DES(LPCSTR lpPwd, LPCBYTE lpData, DWORD nDataLen, LPBYTE pOut, BOOL bDecode /*= FALSE*/)
{
	HCRYPTPROV		prov		= NULL;
	HCRYPTHASH		hash		= NULL;
	CHAR			szMd5[64]	= {0};
	BOOL			bRes		= FALSE;
	BYTE			btMd5[64]	= {0};
	DWORD			dwMd5		= sizeof(btMd5);
	HCRYPTKEY		key			= NULL;

	bRes = CryptAcquireContext(&prov, NULL, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
	if(FALSE == bRes)
		goto md5_End;

	struct
	{
		BLOBHEADER hdr;
		DWORD cbKeySize;
		BYTE rgbKeyData[128];
	} keyBlob;    //½á¹¹²Î¿¼MSDN - CryptImportKey

	keyBlob.hdr.bType = PLAINTEXTKEYBLOB;
	keyBlob.hdr.bVersion = CUR_BLOB_VERSION;
	keyBlob.hdr.reserved = 0;
	keyBlob.hdr.aiKeyAlg = CALG_3DES;
	keyBlob.cbKeySize = (int)strlen(lpPwd);
	ZeroMemory(keyBlob.rgbKeyData, sizeof(keyBlob.rgbKeyData));
	strcpy((LPSTR)keyBlob.rgbKeyData, lpPwd);
	//CopyMemory(keyBlob.rgbKeyData, lpPwd, cbKeyLen > 8 ? 8 : cbKeyLen);
	bRes = CryptImportKey(prov, (BYTE*)&keyBlob, sizeof(keyBlob), NULL, 0, &key);
	if(FALSE == bRes)
		goto md5_End;
// 	bRes = CryptCreateHash(prov, CALG_3DES, 0, 0, &hash);
// 	if(FALSE == bRes)
// 		goto md5_End;

// 	bRes = CryptCreateHash(prov, CALG_MD5, 0, 0, &hash);
// 	if(FALSE == bRes)
// 		goto md5_End;
// 
// 	bRes = CryptHashData(hash, (BYTE*)lpPwd, (DWORD)strlen(lpPwd), 0);
// 	if(FALSE == bRes)
// 		goto md5_End;
// 
// 	bRes = CryptDeriveKey(prov, CALG_DES, hash, 0, &key);
// 	if(FALSE == bRes)
// 		goto md5_End;

	nDataLen = (DWORD)strlen(lpPwd);
	if(FALSE == bDecode)
		bRes = CryptEncrypt(key, NULL, TRUE, 0, (BYTE*)lpData, &nDataLen, nDataLen);
	else
		bRes = CryptDecrypt(key, NULL, TRUE, 0, (BYTE*)lpData, &nDataLen);
// 	bRes = CryptHashData(hash, (BYTE*)lpData, nDataLen, 0);
// 	if(FALSE == bRes)
// 		goto md5_End;
// 
// 	bRes = CryptGetHashParam(hash, HP_HASHVAL, btMd5, &dwMd5, 0);
// 	if(FALSE == bRes)
// 		goto md5_End;
// 
// 	DataToHex(btMd5, dwMd5, szMd5, sizeof(szMd5));
md5_End:
	if(NULL != hash)
		CryptDestroyHash(hash);
	if(NULL != key)
		CryptDestroyKey(key);
	if(NULL != prov)
		CryptReleaseContext(hash, 0);

	return bRes;
}