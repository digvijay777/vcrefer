/*
 * Base64����
 *
 * Base64�������Base64�����ַ����ȱ�Ϊ4:3
 */

namespace Encrypt
{
	DWORD		EncodeBase64(IN BYTE* lpSrcData, IN LONG lSrcLen, OUT CHAR* lpDstBase64, IN OUT LONG* pDstLen);
	DWORD		DecodeBase64(IN CHAR* lpSrcBase64, OUT BYTE* lpDstData, IN OUT LONG* pDstLen);
};