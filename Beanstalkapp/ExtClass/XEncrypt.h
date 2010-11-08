#pragma once
#include <string>

#define C1 1986
#define C2 316
// 加密
CString CCloudCenter::Encrypt(CString str, WORD Key) // 加密函数
{
	CString			Result;

	Result = str; // 初始化结果字符串
	for(int i = 0; i < str.GetLength(); i++) // 依次对字符串中各字符进行操作
	{
		Result.SetAt(i, str.GetAt(i)^(Key >> 8)); // 将密钥移位后与字符异或
		Key = ((BYTE)Result.GetAt(i) + Key) * C1 + C2; // 产生下一个密钥
	}
	str = Result; // 保存结果
	Result.Empty(); // 清除结果
	for(int i = 0; i < str.GetLength(); i++) // 对加密结果进行转换
	{
		int				j			= (BYTE)str.GetAt(i); // 提取字符
		CHAR			sz[3]		= {0}; // 设置str长度为2

		sz[0] = 65 + j / 26;//这里将65改大点的数例如256，密文就会变乱码，效果更好，相应的，解密处要改为相同的数
		sz[1] = 65 + j % 26;
		Result += sz;
	}

	return Result;
}
// 解密
CString CCloudCenter::Decrypt(CString str, WORD Key) // 解密函数
{
	CString			Result;

	Result.Empty(); // 清除结果
	for(int i = 0; i < str.GetLength() / 2; i++) // 将字符串两个字母一组进行处理
	{
		int			j		= 0;
		CHAR		sz[2]	= {0};
		
		j = (BYTE)(str.GetAt(2 * i) - 65) * 26;//相应的，解密处要改为相同的数
		j += (BYTE)str.GetAt(2 * i + 1) - 65;
		sz[0] = j;
		Result += sz; // 追加字符，还原字符串
	}

	str = Result; // 保存中间结果
	for(int i = 0; i < str.GetLength(); i++) // 依次对字符串中各字符进行操作
	{
		Result.SetAt(i, (BYTE)str.GetAt(i)^(Key>>8)); // 将密钥移位后与字符异或
		Key = ((BYTE)str.GetAt(i)+Key)*C1+C2; // 产生下一个密钥
	}

	return Result;
}