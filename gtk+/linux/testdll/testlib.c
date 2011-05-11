#include <stdio.h>

void GetString(char* pstr, int nSize)
{
	strncpy(pstr, "这是动态库test里的文本。", nSize);
}

