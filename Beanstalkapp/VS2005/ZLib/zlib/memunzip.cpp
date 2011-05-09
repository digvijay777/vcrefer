#include "memunzip.h"
#include <stdio.h>

#ifndef FILE_BEGIN
#define FILE_BEGIN           0
#define FILE_CURRENT         1
#define FILE_END             2
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

CMemUnzip::CMemUnzip()
{

}

CMemUnzip::~CMemUnzip()
{

}
/*
 *	加载内存文件
 */
bool CMemUnzip::LoadMemory(byte* pData, uLong uSize, bool bClear /* = false */)
{
	// step 1: 构建内存数据
	MEMORYDATA			memData		= {0};

	memData.pData = pData;
	memData.uSize = uSize;
	if(bClear)
		m_zipFile.clear();
	// step 2: 加解压zip内存
	zlib_filefunc_def		zip_funcs		= {0};
	unzFile					puzFile			= NULL;;

	zip_funcs.zopen_file = &CMemUnzip::ZipOpenFunc;
	zip_funcs.zread_file = &CMemUnzip::ZipReadFunc;
	zip_funcs.zwrite_file = &CMemUnzip::ZipWriteFunc;
	zip_funcs.ztell_file = &CMemUnzip::ZipTellFunc;
	zip_funcs.zseek_file = &CMemUnzip::ZipSeekFunc;
	zip_funcs.zclose_file = &CMemUnzip::ZipCloseFunc;
	zip_funcs.zerror_file = &CMemUnzip::ZipErrorFunc;
	zip_funcs.opaque=NULL;
	puzFile = unzOpen2((const char*)&memData, &zip_funcs);
	if(NULL == puzFile)
		return false;
	// step 3: 分离内存文件
	int						nuzCode			= 0;

	nuzCode = unzGoToFirstFile(puzFile);
	for(; UNZ_OK == nuzCode
		; nuzCode = unzGoToNextFile(puzFile))
	{
		char				szCurrentFile[256]		= {0};
		unz_file_info		fileInfo				= {0};
		uLong				uSeekPos;
		uLong				uSize;
		byte*				pFileData				= NULL;
		uLong				uRead					= 0;

		nuzCode = unzGetCurrentFileInfo(puzFile
			, &fileInfo
			, szCurrentFile
			, sizeof(szCurrentFile)
			, NULL
			, 0
			, NULL
			, 0);
		if(UNZ_OK != nuzCode)
			break;

		uSeekPos = unzGetOffset(puzFile);
		uSize = fileInfo.uncompressed_size;
		// 初始化文件
		strupr(szCurrentFile);
		m_zipFile[szCurrentFile].resize(uSize, 0);
		pFileData = &m_zipFile[szCurrentFile].at(0);
		// 读取压缩文件
		nuzCode = unzOpenCurrentFile(puzFile);
		if(UNZ_OK != nuzCode)
		{
			m_zipFile.erase(szCurrentFile);
			continue;
		}
		uSize = unzReadCurrentFile(puzFile, pFileData, uSize);
		if(0 == uSize)
		{
			m_zipFile.erase(szCurrentFile);
		}
		unzCloseCurrentFile(puzFile);
	}
	// step 4: 关闭ZIP文件
	unzClose(puzFile);

	return m_zipFile.size() > 0;
}
/*
 *	得到文件大小
 */
uLong CMemUnzip::GetFileSize(const char* pFileName)
{
	char		szFileName[256]			= {0};
	std::map<FileName, FileData>::iterator		iFind;

	strncpy(szFileName, pFileName, sizeof(szFileName));
	strupr(szFileName);
	iFind = m_zipFile.find(szFileName);
	if(m_zipFile.end() == iFind)
		return 0;

	return (uLong)iFind->second.size();
}
/*
 *	得到文件内容
 */
bool CMemUnzip::GetFileData(const char* pFileName, byte* pData, uLong uSize)
{	
	char		szFileName[256]			= {0};
	std::map<FileName, FileData>::iterator		iFind;

	strncpy(szFileName, pFileName, sizeof(szFileName));
	strupr(szFileName);
	iFind = m_zipFile.find(szFileName);
	if(m_zipFile.end() == iFind)
		return false;

	memcpy(pData, &iFind->second.at(0), min(uSize, iFind->second.size()));
	return true;
}

void* CMemUnzip::ZipOpenFunc(void* opaque, const char* filename, int mode) 
{
	return (void*)filename;
}

uLong CMemUnzip::ZipReadFunc(void* opaque, void* stream, void* buf, uLong size)
{
	uLong			ret			= 0;
	MEMORYDATA*		pMemFile	= (MEMORYDATA *)stream;

	if (!pMemFile)
		goto clean0;

	if(pMemFile->uCurPos >= pMemFile->uSize)
		goto clean0;

	if((pMemFile->uSize - pMemFile->uCurPos) < size)
		size = pMemFile->uSize - pMemFile->uCurPos;
	memcpy(buf, pMemFile->pData+pMemFile->uCurPos, size);
	pMemFile->uCurPos += size;
	ret = size;
clean0:
	return ret;
}

uLong CMemUnzip::ZipWriteFunc(void* opaque, void* stream, const void* buf, uLong size)
{
	uLong			ret			= 0;
	MEMORYDATA*		pMemFile	= (MEMORYDATA*)stream;

	if (!pMemFile)
		goto clean0;

	if(pMemFile->uCurPos >= pMemFile->uSize)
		goto clean0;
	if((pMemFile->uSize - pMemFile->uCurPos) < size)
		size = pMemFile->uSize - pMemFile->uCurPos;
	memcpy(pMemFile->pData+pMemFile->uCurPos, buf, size);
	pMemFile->uCurPos += size;
	ret = size;
clean0:
	return ret;
}

long CMemUnzip::ZipSeekFunc (void* opaque, void* stream, uLong offset, int origin)
{
	uLong			ret = -1;
	MEMORYDATA*		pMemFile = (MEMORYDATA*)stream;

	if (!pMemFile)
		goto clean0;

	if (FILE_CURRENT == origin)
	{
		if(offset > 0 && (pMemFile->uCurPos+offset) >= pMemFile->uSize)
			goto clean0;
		if(offset < 0 && (pMemFile->uCurPos+offset) < 0)
			goto clean0;

		pMemFile->uCurPos += offset;
	}
	else if (FILE_END == origin)
	{
		if(offset > 0)
			goto clean0;
		if((pMemFile->uSize+offset) < 0)
			goto clean0;

		pMemFile->uCurPos = pMemFile->uSize + offset;
	}
	else if (FILE_BEGIN == origin)
	{
		if(offset < 0)
			goto clean0;
		if(offset > pMemFile->uSize)
			goto clean0;

		pMemFile->uCurPos = offset;
	}

	ret = 0;

clean0:
	return ret;
}

long CMemUnzip::ZipTellFunc(void* opaque, void* stream)
{
	long			ret			=-1;
	MEMORYDATA*		pMemFile	= (MEMORYDATA*)stream;

	if (!pMemFile)
		goto clean0;

	if(pMemFile->uCurPos > pMemFile->uSize)
		goto clean0;

	ret = pMemFile->uCurPos;

clean0:
	return ret;
}

int CMemUnzip::ZipCloseFunc(void* opaque, void* stream)
{
	return 0;
}

int CMemUnzip::ZipErrorFunc(void* opaque, void* stream)
{
	return 0;
}