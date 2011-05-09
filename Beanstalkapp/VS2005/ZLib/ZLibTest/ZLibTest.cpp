// ZLibTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../zlib/zlib.h"
#include "../zlib/zconf.h"
#include "../zlib/memunzip.h"
#include <sys/stat.h>

#pragma comment(lib, "../zlib/zlib.lib")

void testbase();
void testfile();

int _tmain(int argc, _TCHAR* argv[])
{
	// 基本操作
/*
	testbase();
*/

	// 文件操作
	testfile();


	return 0;
}

/*
 *	基本的压缩与解压缩
 */
void testbase()
{
	const unsigned char strSrc[]="hello world!\n\
								 aaaaa bbbbb ccccc ddddd aaaaa bbbbb ccccc ddddd中文测试 中文测试\
								 aaaaa bbbbb ccccc ddddd aaaaa bbbbb ccccc ddddd中文测试 中文测试\
								 aaaaa bbbbb ccccc ddddd aaaaa bbbbb ccccc ddddd中文测试 中文测试\
								 aaaaa bbbbb ccccc ddddd aaaaa bbbbb ccccc ddddd中文测试 中文测试";

	unsigned char buff[1024]={0},strDst[1024]={0};
	unsigned long srcLen=sizeof(strSrc),bufLen=sizeof(buff),dstLen=sizeof(strDst);
	_tprintf(_T("Src string:%s\nLength:%d\n===================\n"),strSrc,srcLen);
	//压缩
	compress(buff,&bufLen,strSrc,srcLen);
	_tprintf(_T("\nAfter Compressed Length:%d\nCompressed String:%s\n==============\n"),bufLen,buff);

	//解压缩
	uncompress(strDst,&dstLen,buff,bufLen);
	_tprintf(_T("\nAfter UnCompressed Length:%d\nUnCompressed String:%s\n"),dstLen,strDst);
}

/*
 *	文件操作
 */
void testfile()
{
	CMemUnzip		zip;
	FILE*			pf				= NULL;
	byte*			pdata			= NULL;
	uLong			uSize			= 0;
	struct stat		st				= {0};
	char			szData[1024]	= {0};

	// step 1: 打开ZIP文件
	pf = fopen("f:\\test.zip", "rb");
	if(NULL == pf)
	{
		printf("can not open f:\\test.zip\n");
		goto end;
	}
	// step 2: 读取ZIP文件
	fstat(pf->_file, &st);
	uSize = st.st_size;
	if(uSize <= 0)
	{
		printf("get file size failed.\n");
		goto end;
	}
	pdata = (byte *)malloc(uSize);
	fread(pdata, uSize, 1, pf);
	// step 3: 解压文件
	zip.LoadMemory(pdata, uSize);
	// step 4: 读文件内容
	zip.GetFileData("test.log", (byte *)szData, sizeof(szData));
	printf("test.log: \n%s\n", szData);
end:
	if(NULL != pf)
		fclose(pf);
	if(NULL != pdata)
		free(pdata);
}
