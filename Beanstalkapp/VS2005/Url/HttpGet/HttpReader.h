#pragma once
#include <atlbase.h>
#include <atlhttp.h>

class CHttpReaderSock
{
public:
	// SocketClass
	void		Close();
	bool		Connect(LPCTSTR szAddr, unsigned short nPort);
	bool		Connect(const SOCKADDR* psa, int len);
	DWORD		SetSocketTimeout(DWORD dwNewTimeout);
	bool		Write(WSABUF *pBuffers, int nCount, DWORD *pdwSize);
	bool		Write(const unsigned char *pBuffIn, DWORD *pdwSize);
	bool		SupportsScheme(ATL_URL_SCHEME scheme);
	bool		Read(const unsigned char *pBuff, DWORD *pdwSize);

	DWORD					m_dwLastError;

public:
	virtual bool	BuffRead(const unsigned char *pBuff, DWORD *pdwSize) = 0;
};

class CHttpReader : protected CAtlHttpClientT<CHttpReaderSock>
{
public:
	CHttpReader(void);
	~CHttpReader(void);

private:
	readstate				m_state;
	unsigned char *			m_pBodyBegin;
	unsigned char *			m_pHeaderBegin;

	DWORD					m_dwReadStart;
	DWORD					m_dwReadLen;
	unsigned char *			m_pReadData;
	bool					m_bReadFailed;
	ATL_NAVIGATE_DATA		m_navigatedata;

	struct {
		long				nChunkBuffCarryOver;
		long				nChunkSize;
		CHeapPtr<char>		t_chunk_buffer;
		long				nTChunkBuffSize;
		char *				chunk_buffer;
		char *				chunk_buffer_end;
		CHUNK_STATE			cstate;
		CHUNK_LEX_RESULT	cresult;
		CAtlIsapiBuffer<>	result_buffer;
	} ChunkedBody;
private:
	bool			ReadHttpResponseEx();
	bool			ReadChunkedBodyEx();

	virtual bool	BuffRead(const unsigned char *pBuff, DWORD *pdwSize);
public:
	const BYTE*		GetResponse();
	void			ClearReader(void);
	bool			AddRecv(const unsigned char *pBuff, long dwSize);
};
