#pragma once
#include "unzip.h"
#include <map>
#include <vector>
#include <string>

#ifndef byte
typedef unsigned char		byte;
#endif
/*
 *	内存解压ZIP
 */
class CMemUnzip
{
public:
	CMemUnzip();
	~CMemUnzip();

public:
	bool	LoadMemory(byte* pData, uLong uSize, bool bClear = false);
	uLong	GetFileSize(const char* pFileName);
	bool	GetFileData(const char* pFileName, byte* pData, uLong uSize);

	typedef std::string			FileName;
	typedef std::vector<byte>	FileData;
	// 内存数据
	typedef struct _MemoryData{
		byte*		pData;
		uLong		uSize;
		uLong		uCurPos;
	}MEMORYDATA;

protected:
	std::map<FileName, FileData>	m_zipFile;
private:
	static void*	ZipOpenFunc(void* opaque, const char* filename, int mode);
	static uLong	ZipReadFunc(void* opaque, void* stream, void* buf, uLong size);
	static uLong	ZipWriteFunc(void* opaque, void* stream, const void* buf, uLong size);
	static long		ZipSeekFunc (void* opaque, void* stream, uLong offset, int origin);
	static long		ZipTellFunc(void* opaque, void* stream);
	static int		ZipCloseFunc(void* opaque, void* stream);
	static int		ZipErrorFunc(void* opaque, void* stream);
};
