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

/* 匹配一个字符的宏 */
#define MATCH_CHAR(c1,c2,ignore_case)  ( (c1==c2) || ((ignore_case==1) && (tolower(c1) == tolower(c2))) )

/*  通配符匹配算法
*        src      字符串
*        pattern  含有通配符( * 或 ? 号)的字符串
*        ignore_case 是否区分大小写，1 表示不区分,  0 表示区分
*
*  返回1表示 src 匹配 pattern，返回0表示不匹配
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
	// 分配内存
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
	// 构建dfa表格
	(*mem)[0] = 1;
	(*mem)[1] = 1;
	last_row = 0;
	for(c = 1; c < pat_len; c++)
	{
		int			rs		= last_row;

		last_row = src_len;
		// 分析每一列
		for(r = rs + 1; r < src_len; r++)
		{
			// 左上格为0立即跳出
			if( 0 == (*mem)[(r - 1) * pat_len + (c - 1)] )
			{
				if( '*' != pattern[c - 1] 
					|| 0 == (*mem)[(r - 1) * pat_len + c] )
				{
					continue;
				}				
			}
			// 判断匹配
			if('?' != pattern[c - 1]
				&& '*' != pattern[c - 1]
				&& 0 == MATCH_CHAR(pattern[c - 1], src[r - 1], ignore_case) )
			{
				continue;
			}
			
			// 找到匹配
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
// 		{   /* 如果 pattern 的当前字符是 '*' */
// 			/* 如果后续有多个 '*', 跳过 */
// 			while ((*pattern == '*') || (*pattern == '?'))
// 				pattern++;
// 
// 			/* 如果 '*" 后没有字符了，则正确匹配 */
// 			if (!*pattern) return MATCH;
// 
// 			/* 在 src 中查找一个与 pattern中'*"后的一个字符相同的字符*/
// 			while (*src && (!MATCH_CHAR(*src,*pattern,ignore_case)))
// 				src++;
// 
// 			/* 如果找不到，则匹配失败 */        
// 			if (!*src) return NOT_MATCH;
// 
// 			/* 如果找到了，匹配剩下的字符串*/
// 			result = WildcardMatch (src, pattern, ignore_case);
// 			/* 如果剩下的字符串匹配不上，但src后一个字符等于pattern中'*"后的一个字符 */
// 			/* src前进一位，继续匹配 */
// 			while ( (!result) && (*(src+1)) && MATCH_CHAR(*(src+1),*pattern,ignore_case) )
// 				result = WildcardMatch (++src, pattern, ignore_case);
// 
// 			return result;
// 
// 		}
// 		else
// 		{
// 			/* 如果pattern中当前字符不是 '*' */
// 			/* 匹配当前字符*/
// 			if ( MATCH_CHAR(*src,*pattern,ignore_case) || ('?' == *pattern))
// 			{ 
// 				/* src,pattern分别前进一位，继续匹配 */
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
// 	/* 如果src结束了，看pattern有否结束*/       
// 	if (*pattern)  
// 	{ 
// 		/* pattern没有结束*/          
// 		if ( (*pattern=='*') && (*(pattern+1)==0) ) /* 如果pattern有最后一位字符且是'*' */
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
// 	n = WildcardMatch("Aaa.txt","*.txt",1);    //返回1
// 	printf("result: %d\n",n);
// 
// 	n = WildcardMatch("Aaaa","a*a",1);   //返回1，这个看似简单，试了别的算法，不一定行的
// 	printf("result: %d\n",n);
// 
// 	n = WildcardMatch("Aaaa","a*a",0);  //返回0
// 	printf("result: %d\n",n);
// 
// 	n = WildcardMatch("ABA","a?a",1);  //返回1
// 	printf("result: %d\n",n);
// 
// 	n = WildcardMatch("ABAd","a?a",1); //返回0
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

