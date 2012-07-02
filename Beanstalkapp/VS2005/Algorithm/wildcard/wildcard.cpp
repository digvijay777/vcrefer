// wildcard.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <ctype.h>

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
int WildcardMatch(char *src, char *pattern, int ignore_case)
{
	int result;

	while (*src)
	{
		if (*pattern == '*')
		{   /* ��� pattern �ĵ�ǰ�ַ��� '*' */
			/* ��������ж�� '*', ���� */
			while ((*pattern == '*') || (*pattern == '?'))
				pattern++;

			/* ��� '*" ��û���ַ��ˣ�����ȷƥ�� */
			if (!*pattern) return MATCH;

			/* �� src �в���һ���� pattern��'*"���һ���ַ���ͬ���ַ�*/
			while (*src && (!MATCH_CHAR(*src,*pattern,ignore_case)))
				src++;

			/* ����Ҳ�������ƥ��ʧ�� */        
			if (!*src) return NOT_MATCH;

			/* ����ҵ��ˣ�ƥ��ʣ�µ��ַ���*/
			result = WildcardMatch (src, pattern, ignore_case);
			/* ���ʣ�µ��ַ���ƥ�䲻�ϣ���src��һ���ַ�����pattern��'*"���һ���ַ� */
			/* srcǰ��һλ������ƥ�� */
			while ( (!result) && (*(src+1)) && MATCH_CHAR(*(src+1),*pattern,ignore_case) )
				result = WildcardMatch (++src, pattern, ignore_case);

			return result;

		}
		else
		{
			/* ���pattern�е�ǰ�ַ����� '*' */
			/* ƥ�䵱ǰ�ַ�*/
			if ( MATCH_CHAR(*src,*pattern,ignore_case) || ('?' == *pattern))
			{ 
				/* src,pattern�ֱ�ǰ��һλ������ƥ�� */
				return WildcardMatch (++src, ++pattern, ignore_case);
			}
			else
			{
				return NOT_MATCH;
			}
		}
	}


	/* ���src�����ˣ���pattern�з����*/       
	if (*pattern)  
	{ 
		/* patternû�н���*/          
		if ( (*pattern=='*') && (*(pattern+1)==0) ) /* ���pattern�����һλ�ַ�����'*' */
			return MATCH;
		else
			return NOT_MATCH;
	}
	else
		return MATCH;
}


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

	n = WildcardMatch("AcaBAdcb","a*b",1);
	
	return 0;
}

