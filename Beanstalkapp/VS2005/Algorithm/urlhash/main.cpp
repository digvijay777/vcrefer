#include <windows.h>
#include <stdio.h>

// BKDR Hash Function
unsigned int BKDRHash(char *str)
{
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;
	while (*str)
	{
		hash = hash * seed + (*str++);
	}
	return (hash & 0x7FFFFFFF);
}

// ELF Hash Function
unsigned int ELFHash(char *str)
{
	unsigned int hash = 0;
	unsigned int x = 0;
	while (*str)
	{
		hash = (hash << 4) + (*str++);
		if ((x = hash & 0xF0000000L) != 0)
		{
			hash ^= (x >> 24);
			hash &= ~x;
		}
	}
	return (hash & 0x7FFFFFFF);
}


void main(void)
{
	DWORD		dwTickCount		= 0;
	char		szUrl[]			= {"www.*baidu.com"};

	dwTickCount = GetTickCount();
	for(__int64 i = 0; i < 10000 * 10000 ; i++)
	{
		szUrl[4] = 'a' + (char)i % 26;
		BKDRHash(szUrl); // 这个比较快
	}
// 	dwTickCount = GetTickCount() - dwTickCount;
// 	dwTickCount /= 10000;
// 	dwTickCount /= 1000;
	printf("BKDRHash: %d\n", dwTickCount);

	dwTickCount = GetTickCount();
	for(__int64 i = 0; i < 10000 * 10000; i++)
	{
		szUrl[4] = 'a' + (char)i % 26;
		ELFHash(szUrl);
	}
// 	dwTickCount = GetTickCount() - dwTickCount;
// 	dwTickCount /= 10000;
// 	dwTickCount /= 1000;
	printf("ELFHash: %d\n", dwTickCount);

	system("pause");
}