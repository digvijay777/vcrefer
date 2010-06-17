/*
 * Base64实现代码
 */

#include "stdafx.h"
#include "Base64.h"

#define AVal(x)		Base64ValTab[x];
#define BVal(x)		Base64IdxTab[x];

const BYTE Base64ValTab[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const BYTE Base64IdxTab[128] =
{
		255,255,255,255,  255,255,255,255,  255,255,255,255,  255,255,255,255,
		255,255,255,255,  255,255,255,255,  255,255,255,255,  255,255,255,255,
		255,255,255,255,  255,255,255,255,  255,255,255,62,   255,255,255,63,
		52,53,54,55,      56,57,58,59,      60,61,255,255,    255,255,255,255,
		255,0,1,2,        3,4,5,6,          7,8,9,10,         11,12,13,14,
		15,16,17,18,      19,20,21,22,      23,24,25,255,     255,255,255,255,
		255,26,27,28,     29,30,31,32,      33,34,35,36,      37,38,39,40,
		41,42,43,44,      45,46,47,48,      49,50,51,255,     255,255,255,255
};

DWORD Encrypt::EncodeBase64(IN BYTE* lpSrcData, IN LONG lSrcLen, OUT CHAR* lpDstBase64, IN OUT LONG* pDstLen)
{
	DWORD		dwRet		= 0;
	LONG		loop		= 0;
	LONG		remain		= 0;
	LONG		lDstLen		= 0;

	loop = lSrcLen / 3;
	remain = lSrcLen % 3;
	lDstLen = (lSrcLen + 3) / 3 * 4;
	if(lDstLen > *pDstLen)
		return -1; // 接收数据块空间不足
	for(int i = 0; i < loop; i++)
	{
		BYTE a1 = (lpSrcData[i * 3] >> 2);
		BYTE a2 = (((lpSrcData[i * 3] & 0x03) << 4) | (lpSrcData[i * 3 + 1] >> 4));
		BYTE a3 = (((lpSrcData[i * 3 + 1] & 0x0F) << 2) | ((lpSrcData[i * 3 + 2] & 0xC0) >> 6));
		BYTE a4 = (lpSrcData[i * 3 + 2] & 0x3F);

		lpDstBase64[i * 4] = AVal(a1);
		lpDstBase64[i * 4 + 1] = AVal(a2);
		lpDstBase64[i * 4 + 2] = AVal(a3);
		lpDstBase64[i * 4 + 3] = AVal(a4);
	}

	*pDstLen = i * 4;
	if(1 == remain)
	{
		i = lSrcLen - 1;
		BYTE a1 = (lpSrcData[i] >> 2);
		BYTE a2 = ((lpSrcData[i] & 0x3) << 4);

		lpDstBase64[(*pDstLen)++] = AVal(a1);
		lpDstBase64[(*pDstLen)++] = AVal(a2);
		lpDstBase64[(*pDstLen)++] = '=';
		lpDstBase64[(*pDstLen)++] = '=';
		lpDstBase64[(*pDstLen)]	= 0x0;
	}
	else if(2 == remain)
	{
		i = lSrcLen - 2;
		BYTE a1 = (lpSrcData[i] >> 2);
		BYTE a2 = (((lpSrcData[i] & 0x3) << 4) | (lpSrcData[i + 1] >> 4));
		BYTE a3 = ((lpSrcData[i+1] & 0x0F) << 2);
		
		lpDstBase64[(*pDstLen)++] = AVal(a1);
		lpDstBase64[(*pDstLen)++] = AVal(a2);
		lpDstBase64[(*pDstLen)++] = AVal(a3);
		lpDstBase64[(*pDstLen)++] = '=';
		lpDstBase64[(*pDstLen)] = 0x0;
	}
	else
	{
		lpDstBase64[(*pDstLen)] = 0x0;
	}

	return dwRet;
}

DWORD Encrypt::DecodeBase64(IN CHAR* lpSrcBase64, OUT BYTE* lpDstData, IN OUT LONG* pDstLen)
{
	DWORD		dwRet		= 0;
	LONG		lSrcLen		= 0;
	LONG		lDstLen		= 0;
	BYTE*		pByte		= NULL;
	SHORT		nCut		= 0;

	lSrcLen = strlen(lpSrcBase64);
	lDstLen = (lSrcLen + 4) / 4 * 3;
	if(lDstLen > *pDstLen)
		return -1; // 接收数据块空间不足
	pByte = lpDstData;
	for(int i = 0; i < lSrcLen; i++)
	{
		if(lpSrcBase64[i] > 127)
			continue;
		if(lpSrcBase64[i] == '=')
		{
			*pDstLen = pByte - lpDstData + 1;
			return dwRet;
		}
		
		BYTE	a = BVal(lpSrcBase64[i]);
		if(255 == a)	// 非Base64字符
			continue;
		switch(nCut++)
		{
		case 0:
			*pByte = a << 2;
			break;
		case 1:
			*pByte++ |= a >> 4;
			*pByte = a << 4;
			break;
		case 2:
			*pByte++ |= a >> 2;
			*pByte = a << 6;
			break;
		case 3:
			*pByte++ |= a;
			nCut = 0;
			break;
		}
	}

	*pByte = 0x0;
	*pDstLen = pByte - lpDstData;
	return dwRet;
}