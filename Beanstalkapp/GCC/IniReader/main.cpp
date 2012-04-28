#include <stdio.h>
#include <stdlib.h>
extern "C"
{
#include "IniReader.h"
};

int main()
{
	char		szbuf[1024]		= {0};

	ini_get_string("APP", "Test", "hi", szbuf, 
		sizeof(szbuf), "test.ini");
	return 0;
}