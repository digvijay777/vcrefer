// ZLibTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../zlib/zlib.h"
#include "../zlib/zconf.h"
#include "../zlib/memunzip.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>
#include <fcntl.h>

#pragma comment(lib, "../zlib/zlib.lib")

void testbase();
void testfile();
void testfile2(void);

int _tmain(int argc, _TCHAR* argv[])
{
	// ��������
/*
	testbase();
*/

	// �ļ�����
/*
	testfile();
*/

	testfile2();

	return 0;
}

/*
 *	������ѹ�����ѹ��
 */
void testbase(void)
{
	const unsigned char strSrc[]="hello world!\n\
								 aaaaa bbbbb ccccc ddddd aaaaa bbbbb ccccc ddddd���Ĳ��� ���Ĳ���\
								 aaaaa bbbbb ccccc ddddd aaaaa bbbbb ccccc ddddd���Ĳ��� ���Ĳ���\
								 aaaaa bbbbb ccccc ddddd aaaaa bbbbb ccccc ddddd���Ĳ��� ���Ĳ���\
								 aaaaa bbbbb ccccc ddddd aaaaa bbbbb ccccc ddddd���Ĳ��� ���Ĳ���";

	unsigned char buff[1024]={0},strDst[1024]={0};
	unsigned long srcLen=sizeof(strSrc),bufLen=sizeof(buff),dstLen=sizeof(strDst);
	_tprintf(_T("Src string:%s\nLength:%d\n===================\n"),strSrc,srcLen);
	//ѹ��
	compress(buff,&bufLen,strSrc,srcLen);
	_tprintf(_T("\nAfter Compressed Length:%d\nCompressed String:%s\n==============\n"),bufLen,buff);

	//��ѹ��
	uncompress(strDst,&dstLen,buff,bufLen);
	_tprintf(_T("\nAfter UnCompressed Length:%d\nUnCompressed String:%s\n"),dstLen,strDst);
}

/*
 *	�ļ�����
 */
void testfile()
{
	CMemUnzip		zip;
	FILE*			pf				= NULL;
	byte*			pdata			= NULL;
	uLong			uSize			= 0;
	struct stat		st				= {0};
	char			szData[1024]	= {0};

	// step 1: ��ZIP�ļ�
	pf = fopen("c:\\test.zip", "rb");
	if(NULL == pf)
	{
		printf("can not open f:\\test.zip\n");
		goto end;
	}
	// step 2: ��ȡZIP�ļ�
	fstat(pf->_file, &st);
	uSize = st.st_size;
	if(uSize <= 0)
	{
		printf("get file size failed.\n");
		goto end;
	}
	pdata = (byte *)malloc(uSize);
	fread(pdata, uSize, 1, pf);
	// step 3: ��ѹ�ļ�
	zip.LoadMemory(pdata, uSize);
	// ��ѹ����
	// step 4: ���ļ�����
	zip.GetFileData("test.log", (byte *)szData, sizeof(szData));
	printf("test.log: \n%s\n", szData);
end:
	if(NULL != pf)
		fclose(pf);
	if(NULL != pdata)
		free(pdata);
}

void testfile2(void)
{
	int				fd				= open("c:\\test.zip", O_RDONLY);
	char			szSrc[1024]		= {0};
	char			szDes[10240]	= {0};
	struct stat		st				= {0};
	uLong			nLen			= sizeof(szDes);

	if(-1 == fd)
	{
		return;
	}
	stat("c:\\test.zip", &st);
	if(st.st_size > sizeof(szSrc))
	{
		return;
	}

	read(fd, szSrc, sizeof(szSrc));
	close(fd);
	
	//��ѹ��
	uncompress((Bytef *)szSrc, &nLen, (Bytef *)szDes, strlen(szSrc));

	return;
}
