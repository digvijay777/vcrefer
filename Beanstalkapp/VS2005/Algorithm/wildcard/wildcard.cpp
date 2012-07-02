// wildcard.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>


#define min(x, y)		( ((x) < (y))?(x):(y) )

/** Defines and Macros */
#define MATCH      1
#define NOT_MATCH  0

/* ƥ��һ���ַ��ĺ� */
#define MATCH_CHAR(c1,c2,ignore_case)  ( (c1==c2) || ((ignore_case==1) && (tolower(c1) == tolower(c2))) )

/*  ͨ���ƥ���㷨
*        src      �ַ���
*        pattern  ����ͨ���( * �� ? ��)���ַ���
*        ignore_case �Ƿ����ִ�Сд��1 ��ʾ������,  0 ��ʾ����
*
*  ����1��ʾ src ƥ�� pattern������0��ʾ��ƥ��
*/
int wildcardmatch_r(char *src, char *pattern, int ignore_case, 
					unsigned char** mem, unsigned int* len)
{
	size_t		tmplen;
	int			r, c;
	size_t		src_len, pat_len;
	int			last_row;

	src_len = strlen(src) + 1;
	pat_len = strlen(pattern) + 1;
	if(1 == src_len || 1 == pat_len)
	{
		return NOT_MATCH;
	}
	tmplen = src_len * pat_len;
	// �����ڴ�
	if(tmplen > *len)
	{
		if(NULL != *mem)
		{
			free(*mem);
		}
		
		*mem = (unsigned char*)malloc(tmplen);
		if(NULL == *mem)
		{
			*len = 0;
			return NOT_MATCH;
		}
		*len = (unsigned int)tmplen;
	}
	memset(*mem, 0, *len);
	// ����dfa���
	(*mem)[0] = 1;
	(*mem)[1] = 1;
	last_row = 0;
	for(c = 1; c < pat_len; c++)
	{
		int			rs		= last_row;

		last_row = src_len;
		// ����ÿһ��
		for(r = rs + 1; r < src_len; r++)
		{
			// ���ϸ�Ϊ0��������
			if( 0 == (*mem)[(r - 1) * pat_len + (c - 1)] )
			{
				if( '*' != pattern[c - 1] 
					|| 0 == (*mem)[(r - 1) * pat_len + c] )
				{
					continue;
				}				
			}
			// �ж�ƥ��
			if('?' != pattern[c - 1]
				&& '*' != pattern[c - 1]
				&& 0 == MATCH_CHAR(pattern[c - 1], src[r - 1], ignore_case) )
			{
				continue;
			}
			
			// �ҵ�ƥ��
			(*mem)[ r * pat_len + c ] = 1;
			last_row = min(last_row, r);
		}
	}

	if(1 == (*mem)[src_len * pat_len - 1])
	{
		return MATCH;
	}

	return NOT_MATCH;
}

int wildcardmatch(char *src, char *pattern, int ignore_case)
{
	static unsigned char*	mem		= NULL;
	static unsigned int		len		= 0;

	return wildcardmatch_r(src, pattern, ignore_case, &mem, &len);
}

// int WildcardMatch(char *src, char *pattern, int ignore_case)
// {
// 	int result;
// 
// 	while (*src)
// 	{
// 		if (*pattern == '*')
// 		{   /* ��� pattern �ĵ�ǰ�ַ��� '*' */
// 			/* ��������ж�� '*', ���� */
// 			while ((*pattern == '*') || (*pattern == '?'))
// 				pattern++;
// 
// 			/* ��� '*" ��û���ַ��ˣ�����ȷƥ�� */
// 			if (!*pattern) return MATCH;
// 
// 			/* �� src �в���һ���� pattern��'*"���һ���ַ���ͬ���ַ�*/
// 			while (*src && (!MATCH_CHAR(*src,*pattern,ignore_case)))
// 				src++;
// 
// 			/* ����Ҳ�������ƥ��ʧ�� */        
// 			if (!*src) return NOT_MATCH;
// 
// 			/* ����ҵ��ˣ�ƥ��ʣ�µ��ַ���*/
// 			result = WildcardMatch (src, pattern, ignore_case);
// 			/* ���ʣ�µ��ַ���ƥ�䲻�ϣ���src��һ���ַ�����pattern��'*"���һ���ַ� */
// 			/* srcǰ��һλ������ƥ�� */
// 			while ( (!result) && (*(src+1)) && MATCH_CHAR(*(src+1),*pattern,ignore_case) )
// 				result = WildcardMatch (++src, pattern, ignore_case);
// 
// 			return result;
// 
// 		}
// 		else
// 		{
// 			/* ���pattern�е�ǰ�ַ����� '*' */
// 			/* ƥ�䵱ǰ�ַ�*/
// 			if ( MATCH_CHAR(*src,*pattern,ignore_case) || ('?' == *pattern))
// 			{ 
// 				/* src,pattern�ֱ�ǰ��һλ������ƥ�� */
// 				return WildcardMatch (++src, ++pattern, ignore_case);
// 			}
// 			else
// 			{
// 				return NOT_MATCH;
// 			}
// 		}
// 	}
// 
// 
// 	/* ���src�����ˣ���pattern�з����*/       
// 	if (*pattern)  
// 	{ 
// 		/* patternû�н���*/          
// 		if ( (*pattern=='*') && (*(pattern+1)==0) ) /* ���pattern�����һλ�ַ�����'*' */
// 			return MATCH;
// 		else
// 			return NOT_MATCH;
// 	}
// 	else
// 		return MATCH;
// }



int _tmain(int argc, _TCHAR* argv[])
{
 	int				n;
// 
// 	n = WildcardMatch("Aaa.txt","*.txt",1);    //����1
// 	printf("result: %d\n",n);
// 
// 	n = WildcardMatch("Aaaa","a*a",1);   //����1��������Ƽ򵥣����˱���㷨����һ���е�
// 	printf("result: %d\n",n);
// 
// 	n = WildcardMatch("Aaaa","a*a",0);  //����0
// 	printf("result: %d\n",n);
// 
// 	n = WildcardMatch("ABA","a?a",1);  //����1
// 	printf("result: %d\n",n);
// 
// 	n = WildcardMatch("ABAd","a?a",1); //����0
// 	printf("result: %d\n",n);
// 
// 	n = WildcardMatch("ABAdc","a*a*b",1); 
// 	printf("result: %d\n",n);

// 	n = wildcardmatch("AcaBAdcb","a*b",1);
	
	BOOL		bRet				= 0;
	DWORD		dwTickcount			= 0;

	//bRet = NStr::WildcardCompare("AbcdabAbcdbc", "a*a*b", TRUE);
	bRet = wildcardmatch("http://www.baidu.com/test/index.html",
		"http://www.baidu.com/*/index.html", TRUE);
	dwTickcount = GetTickCount();
	for(int i = 0; i < 100000; i++)
	{
		wildcardmatch("http://www.baidu.com/test/index.html",
			"http://www.baidu.com/*/index.html", TRUE);
	}
	dwTickcount = GetTickCount() - dwTickcount;
	printf("wildcard 10000 use time: %d\n", dwTickcount);
	int i = 0;
	return 0;
}

