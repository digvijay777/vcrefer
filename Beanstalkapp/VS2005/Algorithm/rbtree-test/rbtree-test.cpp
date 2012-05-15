// rbtree-test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
extern "C"
{
#include "../lib/rbtree.h"
};

struct mynode{
	struct rb_node		node;
	int					id;
};

void insert(struct rb_root* proot, struct mynode *pnode)
{
	struct rb_node		**ppnew		= &(proot->rb_node);
	struct rb_node		*parent		= NULL;
	int					result		= 0;
	struct mynode*		pthis;

	while(*ppnew)
	{
		pthis = (struct mynode*)(*ppnew);
		result = pnode->id - pthis->id;
		parent = *ppnew;
		if(result < 0)
		{
			ppnew = &((*ppnew)->rb_left);
		}
		else if(result > 0)
		{
			ppnew = &((*ppnew)->rb_right);
		}
		else
		{
			return;
		}
	}

	rb_init_node(&(pnode->node));
	rb_link_node(&(pnode->node), parent, ppnew);
	rb_insert_color(&(pnode->node), proot);
}

int _tmain(int argc, _TCHAR* argv[])
{
	rb_root			root		= {0};
	struct mynode	nodes[6]	= {0};
	
	nodes[0].id = 1;
	nodes[1].id = 3;
	nodes[2].id = 15;
	nodes[3].id = 13;
	nodes[4].id = 10;
	nodes[5].id = 9;

	for(int i = 0; i < 6; i++)
	{
		insert(&root, &nodes[i]);
		int j = 0;
	}

	return 0;
}

