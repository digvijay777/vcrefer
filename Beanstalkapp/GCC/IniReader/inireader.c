/*
 *	Ini文件操作类
 */
#include "inireader.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef true
#define true	1
#endif
#ifndef false
#define false	0
#endif

#if defined(_MSC_VER)
/* VC就不执行下面的语句 */
#elif defined(__GNUC__)
/* GCC编译器 */
#include <unistd.h>
#else
#error "Not supported"
#endif // defined(_MSC_VER)

#if !defined(min)
#define min(x, y)		( ((x) < (y))?(x):(y) )
#endif

/* 读取文件 */
char* GetConfigProfileBuff(const char* pFileName)
{
	int				nFile				= -1;
	struct stat		filestat			= {0};
	char*			pRetBuff			= NULL;

	if(NULL == pFileName)
		return NULL;
	nFile = open(pFileName, O_RDONLY);
	if(-1 == nFile)
		goto end;
	if(-1 == fstat(nFile, &filestat))
		goto end;
	pRetBuff = malloc(filestat.st_size + 1);
	if(NULL == pRetBuff)
		goto end;
	memset(pRetBuff, 0, filestat.st_size + 1);
	read(nFile, pRetBuff, filestat.st_size);
end:
	if(-1 != nFile)
		close(nFile);
	return pRetBuff;
}

/*Ini操作函数*/
unsigned long GetConfigProfileString(const char* pAppName
									  , const char* pKeyName, const char* pDefault, char* pReturnedString
									  , unsigned long nSize, const char* pFileName)
{
	char*			pFileBuf			= NULL;
	int				nLen				= 0;
	// 检查接收缓冲区
	if(NULL == pReturnedString)
		return 0;
	// 复制默认字符串
	if(NULL != pDefault)
		strncpy(pReturnedString, pDefault, nSize);
	else
		memset(pReturnedString, 0, nSize);
	// 检查是否指定文件名
	pFileBuf = GetConfigProfileBuff(pFileName);
	if(NULL == pFileBuf)
		goto end;
	// 分析文件缓冲区, 获取APP的开始位置
	nLen = GetConfigBufferString(pAppName, pKeyName, pReturnedString
		, nSize, pFileBuf);
end:
	if(NULL != pFileBuf)
		free(pFileBuf);
	return strlen(pReturnedString);
}

int	GetConfigProfileInt(const char* pAppName
						 , const char* pKeyName, int nDef, const char* pFileName)
{
	char	szInt[128]		= {0};

	if( 0 == GetConfigProfileString(pAppName, pKeyName, NULL
		, szInt, sizeof(szInt), pFileName) )
		return nDef;
	return atoi(szInt);
}

int GetConfigBufferString(const char* pAppName, const char* pKeyName
							, char* pReturnedString, unsigned long nSize
							, const char* pBuff)
{
	char*		pStrStart		= NULL;
	char*		pStrEnd			= NULL;

	GetConfigBufferValue(pAppName, pKeyName, &pStrStart, &pStrEnd, pBuff);
	if(NULL == pStrStart || NULL == pStrEnd || pStrEnd <= pStrStart)
		return 0;

	nSize = min(nSize, pStrEnd - pStrStart);
	strncpy(pReturnedString, pStrStart, nSize);
	return nSize;
}
/* 获取数据区中的配置项 */
void GetConfigBufferValue(const char* pAppName, const char* pKeyName 
					   , char** pStrStart, char** pStrEnd , const char* pBuff)
{
	char*		pSecStart			= NULL;
	char*		pSecEnd				= NULL;
	char*		pFindLine			= NULL;
	char*		pFindStart			= NULL;
	int			nLen				= strlen(pKeyName);

	*pStrStart = NULL;
	*pStrEnd = NULL;
	GetConfigBufferSection(pAppName, &pSecStart, &pSecEnd, pBuff);
	if(NULL == pSecStart || NULL == pSecEnd || pSecEnd <= pSecStart)
		return;
	/* 开始分析Key的值 */
	pFindStart = pSecStart;
	pFindLine = pSecStart;
	for(; pFindLine < pSecEnd; pFindLine = strchr(pFindLine, '\n'))
	{
		if(NULL == pFindLine)
			break;
		if('\n' == *pFindLine)
			pFindLine++;
		while('\t' == *pFindLine || '\x20' == *pFindLine)
			pFindLine++;
		if(0 != strncmp(pFindLine, pKeyName, nLen))
			continue;
		pFindLine += nLen;
		while('\t' == *pFindLine || '\x20' == *pFindLine)
			pFindLine++;
		if('=' != *pFindLine)
			continue;
		/* 找到了相应的行数据 */
		pFindLine++;
		while('\t' == *pFindLine || '\x20' == *pFindLine)
			pFindLine++;
		*pStrStart = pFindLine;
		*pStrEnd = strchr(pFindLine, '\n');
		if(NULL == *pStrEnd)
			*pStrEnd = pSecEnd;
	}
	
}					   
/* 获取数据区的[Section] */
void GetConfigBufferSection(const char* pAppName , char** pSecStart
						 , char** pSecEnd , const char* pBuff)
{
	char*		pFindApp	= NULL;
	char*		pFindStart	= (char *)pBuff;
	int			nLen		= strlen(pAppName);

	*pSecStart = NULL;
	*pSecEnd = NULL;
	do 
	{
		pFindApp = strstr(pFindStart, pAppName);
		if(NULL == pFindApp)
			break;
		pFindStart = pFindApp + nLen;
		if(pFindApp == pBuff)
			continue; /* 首字节时表示不是需要找的Section */
		if('[' != *(pFindApp - 1) || ']' != *(pFindApp + nLen))
			continue; /* 不满足session条件 */
		/* 找到相应的Section */
		*pSecStart = strchr(pFindApp, '\n');
		if(NULL != *pSecStart)
			break;
	} while (true);
	/* 查找Seciont的结束点 */
	if(NULL == *pSecStart)
		return;
	do 
	{
		pFindApp = strchr(pFindStart, '\n');
		if(NULL == pFindApp)
		{
			*pSecEnd = pFindStart + strlen(pFindStart);
			break;
		}
		else if('[' == *(pFindApp + 1))
		{
			*pSecEnd = pFindApp + 1;
			break;
		}
		pFindStart = pFindApp + 1;
	} while (true);
}

/* 写配置文件操作 */
int WriteConfigProfileSection(const char* pAppName , const char* pSection , const char* pFileName)
{
	char*		pFileBuff		= NULL;

	pFileBuff = GetConfigProfileBuff(pAppName);
	if(NULL == pFileBuff)
		return -1;

end:
	if(NULL != pFileBuff)
		free(pFileBuff);
	return 0;
}
