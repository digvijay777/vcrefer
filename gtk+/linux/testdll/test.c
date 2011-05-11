#include <stdio.h>
#include <dlfcn.h>

int main()
{
	void 	*handle 	= NULL;
	void 	(*GetString)(char* ppstring, int nSize);
	char	szText[1024]	= {0};

	handle = dlopen("./libtest.so", RTLD_LAZY);
	if(NULL == handle)
	{
		printf("加载动态库失败.\n");
		return 0;
	}
	GetString = (void (*)(char*, int))dlsym(handle, "GetString");

	if(NULL == GetString)
	{
		printf("无法找到GetString函数\n");
		return 0;
	}
	GetString(szText, sizeof(szText));
	printf("szText: %s\n", szText);
	dlclose(handle);
	return 0;
}
