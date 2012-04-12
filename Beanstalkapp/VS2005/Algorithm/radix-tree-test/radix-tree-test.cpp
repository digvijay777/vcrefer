// radix-tree-test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <atlbase.h>
#include <atltrace.h>
#include <map>
extern "C"
{
#include "../lib/radix-tree.h"
};

int _tmain(int argc, _TCHAR* argv[])
{
	// 初始化
	radix_tree_init();
	// 插入操作
	radix_tree_root		root			= {0};
	radix_tree_root		root2			= {0};
	char				szPath1[256]	= {0};
	char				szPath2[256]	= {0};	
	
	DWORD		dwTickCount		= GetTickCount();
	
	for(unsigned long i = 0; i < (65535 * 100); i++)
	{
		radix_tree_insert(&root, i, (void *)i);
		radix_tree_insert(&root2, i, (void *)i);
	}
	ATLTRACE("radix_tree_insert use time: %d\n", GetTickCount() - dwTickCount);

	// 与rb-tree比较
	std::map<unsigned long, void *>		mroot;
	std::map<unsigned long, void *>		mroot2;

	dwTickCount		= GetTickCount();
	for(unsigned long i = 0; i < (65535 * 100); i++)
	{
		mroot[i] = (void *)i;
		mroot2[i] = (void *)i;
	}
	ATLTRACE("rbtree_insert use time: %d\n", GetTickCount() - dwTickCount);

	void *p1 = radix_tree_lookup(&root, 12400);
	void *p2 = radix_tree_lookup(&root2, 12400);
	void *p3 = mroot[12401];
	void *p4 = mroot2[12401];

	return 0;
}

