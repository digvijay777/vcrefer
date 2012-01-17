/*
 *	Ini�ļ�������
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
/* VC�Ͳ�ִ���������� */
#elif defined(__GNUC__)
/* GCC������ */
#include <unistd.h>
#else
#error "Not supported"
#endif // defined(_MSC_VER)

#if !defined(min)
#define min(x, y)		( ((x) < (y))?(x):(y) )
#endif

/* ��ȡ�ļ� */
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

/*Ini��������*/
unsigned long GetConfigProfileString(const char* pAppName
									  , const char* pKeyName, const char* pDefault, char* pReturnedString
									  , unsigned long nSize, const char* pFileName)
{
	char*			pFileBuf			= NULL;
	int				nLen				= 0;
	// �����ջ�����
	if(NULL == pReturnedString)
		return 0;
	// ����Ĭ���ַ���
	if(NULL != pDefault)
		strncpy(pReturnedString, pDefault, nSize);
	else
		memset(pReturnedString, 0, nSize);
	// ����Ƿ�ָ���ļ���
	pFileBuf = GetConfigProfileBuff(pFileName);
	if(NULL == pFileBuf)
		goto end;
	// �����ļ�������, ��ȡAPP�Ŀ�ʼλ��
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
/* ��ȡ�������е������� */
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
	/* ��ʼ����Key��ֵ */
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
		/* �ҵ�����Ӧ�������� */
		pFindLine++;
		while('\t' == *pFindLine || '\x20' == *pFindLine)
			pFindLine++;
		*pStrStart = pFindLine;
		*pStrEnd = strchr(pFindLine, '\n');
		if(NULL == *pStrEnd)
			*pStrEnd = pSecEnd;
	}
	
}					   
/* ��ȡ��������[Section] */
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
			continue; /* ���ֽ�ʱ��ʾ������Ҫ�ҵ�Section */
		if('[' != *(pFindApp - 1) || ']' != *(pFindApp + nLen))
			continue; /* ������session���� */
		/* �ҵ���Ӧ��Section */
		*pSecStart = strchr(pFindApp, '\n');
		if(NULL != *pSecStart)
			break;
	} while (true);
	/* ����Seciont�Ľ����� */
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

/* д�����ļ����� */
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
