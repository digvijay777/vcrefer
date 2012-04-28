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
char* get_file_buff(const char* pFileName)
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
unsigned long ini_get_string(const char* papp, const char* pkey, 
							 const char* pdef, char* pret, 
							 unsigned long len, const char* pfile)
{
	char*			pFileBuf			= NULL;
	int				nLen				= 0;
	// 检查接收缓冲区
	if(NULL == pret)
		return 0;
	// 复制默认字符串
	if(NULL != pdef)
		strncpy(pret, pdef, len);
	else
		memset(pret, 0, len);
	// 检查是否指定文件名
	pFileBuf = get_file_buff(pfile);
	if(NULL == pFileBuf)
		goto end;
	// 分析文件缓冲区, 获取APP的开始位置
	nLen = buff_get_string(papp, pkey, pret
		, len, pFileBuf);
end:
	if(NULL != pFileBuf)
		free(pFileBuf);
	return strlen(pret);
}

int	ini_get_int(const char* papp, const char* pkey, 
				int def, const char* pfile)
{
	char	szInt[128]		= {0};

	if( 0 == ini_get_string(papp, pkey, NULL, 
		szInt, sizeof(szInt), pfile) )
	{
		return def;
	}
	return atoi(szInt);
}

int buff_get_string(const char* papp, const char* pkey, 
					char* pret, unsigned long len, const char* buff)
{
	char*		pStrStart		= NULL;
	char*		pStrEnd			= NULL;

	buff_get_value(papp, pkey, &pStrStart, &pStrEnd, buff);
	if(NULL == pStrStart || NULL == pStrEnd || pStrEnd <= pStrStart)
		return 0;

	len = min(len, pStrEnd - pStrStart);
	strncpy(pret, pStrStart, len);
	return len;
}
/* 获取数据区中的配置项 */
void buff_get_value(const char* papp, const char* pkey, 
					char** pstart, char** pend, const char* buff)
{
	char*		pSecStart			= NULL;
	char*		pSecEnd				= NULL;
	char*		pFindLine			= NULL;
	char*		pFindStart			= NULL;
	int			nLen				= strlen(pkey);

	*pstart = NULL;
	*pend = NULL;
	buff_get_section(papp, &pSecStart, &pSecEnd, buff);
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
		if(0 != strncmp(pFindLine, pkey, nLen))
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
		*pstart = pFindLine;
		*pend = strchr(pFindLine, '\r');
		if(NULL == *pend)
		{
			*pend = strchr(pFindLine, '\n');
			if(NULL == *pend)
				*pend = pSecEnd;
		}
	}
	
}					   
/* 获取数据区的[Section] */
void buff_get_section(const char* papp , char** pstart, 
					  char** pend , const char* buff)
{
	char*		pFindApp	= NULL;
	char*		pFindStart	= (char *)buff;
	int			nLen		= strlen(papp);

	*pstart = NULL;
	*pend = NULL;
	do 
	{
		pFindApp = strstr(pFindStart, papp);
		if(NULL == pFindApp)
			break;
		pFindStart = pFindApp + nLen;
		if(pFindApp == buff)
			continue; /* 首字节时表示不是需要找的Section */
		if('[' != *(pFindApp - 1) || ']' != *(pFindApp + nLen))
			continue; /* 不满足session条件 */
		/* 找到相应的Section */
		*pstart = strchr(pFindApp, '\n');
		if(NULL != *pstart)
			break;
	} while (true);
	/* 查找Seciont的结束点 */
	if(NULL == *pstart)
		return;
	do 
	{
		pFindApp = strchr(pFindStart, '\n');
		if(NULL == pFindApp)
		{
			*pend = pFindStart + strlen(pFindStart);
			break;
		}
		else if('[' == *(pFindApp + 1))
		{
			*pend = pFindApp + 1;
			break;
		}
		pFindStart = pFindApp + 1;
	} while (true);
}

/* 写配置文件操作 */
int ini_set_section(const char* papp , const char* psection, 
					const char* pfile)
{
	char*		pFileBuff		= NULL;
	char*		pSecStart		= NULL;
	char*		pSecEnd			= NULL;
	int			nFile			= 0;

	pFileBuff = get_file_buff(papp);
	if(NULL == pFileBuff)
		goto write_append;
	buff_get_section(papp, &pSecStart, &pSecEnd, pFileBuff);
	if(NULL == pSecStart || NULL == pSecEnd || pSecEnd < pSecStart)
		goto write_append;

write_over: /* 覆盖式操作 */
	nFile = open(pfile, O_RDWR);
	if(-1 == nFile)
		goto end;
	lseek(nFile, pSecStart - pFileBuff, SEEK_SET);
	write(nFile, psection, strlen(psection));
	write(nFile, pSecEnd, strlen(pSecEnd));
	if('\n' != psection[strlen(psection) - 1]) 
		write(nFile, "\r\n", 2);
	close(nFile);
	goto end;

write_append: /* 追加式操作 */
	nFile = open(pfile, O_WRONLY | O_APPEND);
	if(-1 == nFile)
		goto end;
	write(nFile, "\r\n[", 3);
	write(nFile, papp, strlen(papp));
	write(nFile, "]\r\n", 3);
	write(nFile, psection, strlen(psection));
	close(nFile);

end:
	if(NULL != pFileBuff)
		free(pFileBuff);
	return 0;
}

/* 写一项配置 */
int ini_set_string(const char* papp, const char* pkey, 
				   const char* pvalue, const char* pfile)
{
	char*		pStrStart		= NULL;
	char*		pStrEnd			= NULL;
	char*		pSecStart		= NULL;
	char*		pSecEnd			= NULL;
	char*		pFileBuff		= NULL;
	int			nFile			= 0;

	pFileBuff = get_file_buff(pfile);
	if(NULL == pFileBuff)
		goto write_new;
	/* 获取原Section */
	buff_get_section(papp, &pSecStart, &pSecEnd, pFileBuff);
	if(NULL == pSecStart || NULL == pSecEnd || pSecEnd < pSecStart)
		goto write_new;
	/* 获取原值 */
	buff_get_value(papp, pkey, &pStrStart, &pStrEnd, pFileBuff);
	if(NULL == pStrStart || NULL == pStrEnd || pStrEnd < pStrStart)
		goto write_append;

write_over: /* 覆盖操作 */
	nFile = open(pfile, O_WRONLY);
	if(-1 == nFile)
		goto end;
	lseek(nFile, pStrStart - pFileBuff, SEEK_SET);
	write(nFile, pvalue, strlen(pvalue));
	write(nFile, pStrEnd, strlen(pStrEnd));
	close(nFile);
write_new: /* 写入一个新项 */
	nFile = open(pfile, O_WRONLY|O_APPEND);
	if(-1 == nFile)
		goto end;
	write(nFile, "\r\n[", 3);
	write(nFile, papp, strlen(papp));
	write(nFile, "]\r\n", 3);
	write(nFile, pkey, strlen(pkey));
	write(nFile, " = ", 3);
	write(nFile, pvalue, strlen(pvalue));
	write(nFile, "\r\n", 2);
	close(nFile);
write_append: /* 追回操作 */
	nFile = open(pfile, O_WRONLY);
	if(-1 == nFile)
		goto end;
	lseek(nFile, pSecEnd - pFileBuff, SEEK_SET);
	if('\n' != *(pSecEnd - 1))
		write(nFile, "\r\n", 2);
	write(nFile, pkey, strlen(pkey));
	write(nFile, " = ", 3);
	write(nFile, pvalue, strlen(pvalue));
	write(nFile, "\r\n", 2);
	write(nFile, pSecEnd, strlen(pSecEnd));
	close(nFile);
end:
	if(NULL != pFileBuff)
		free(pFileBuff);

	return 0;
}
