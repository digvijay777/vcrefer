#pragma once
#include <string>

#define C1 1986
#define C2 316
// ����
CString CCloudCenter::Encrypt(CString str, WORD Key) // ���ܺ���
{
	CString			Result;

	Result = str; // ��ʼ������ַ���
	for(int i = 0; i < str.GetLength(); i++) // ���ζ��ַ����и��ַ����в���
	{
		Result.SetAt(i, str.GetAt(i)^(Key >> 8)); // ����Կ��λ�����ַ����
		Key = ((BYTE)Result.GetAt(i) + Key) * C1 + C2; // ������һ����Կ
	}
	str = Result; // ������
	Result.Empty(); // ������
	for(int i = 0; i < str.GetLength(); i++) // �Լ��ܽ������ת��
	{
		int				j			= (BYTE)str.GetAt(i); // ��ȡ�ַ�
		CHAR			sz[3]		= {0}; // ����str����Ϊ2

		sz[0] = 65 + j / 26;//���ｫ65�Ĵ���������256�����ľͻ�����룬Ч�����ã���Ӧ�ģ����ܴ�Ҫ��Ϊ��ͬ����
		sz[1] = 65 + j % 26;
		Result += sz;
	}

	return Result;
}
// ����
CString CCloudCenter::Decrypt(CString str, WORD Key) // ���ܺ���
{
	CString			Result;

	Result.Empty(); // ������
	for(int i = 0; i < str.GetLength() / 2; i++) // ���ַ���������ĸһ����д���
	{
		int			j		= 0;
		CHAR		sz[2]	= {0};
		
		j = (BYTE)(str.GetAt(2 * i) - 65) * 26;//��Ӧ�ģ����ܴ�Ҫ��Ϊ��ͬ����
		j += (BYTE)str.GetAt(2 * i + 1) - 65;
		sz[0] = j;
		Result += sz; // ׷���ַ�����ԭ�ַ���
	}

	str = Result; // �����м���
	for(int i = 0; i < str.GetLength(); i++) // ���ζ��ַ����и��ַ����в���
	{
		Result.SetAt(i, (BYTE)str.GetAt(i)^(Key>>8)); // ����Կ��λ�����ַ����
		Key = ((BYTE)str.GetAt(i)+Key)*C1+C2; // ������һ����Կ
	}

	return Result;
}