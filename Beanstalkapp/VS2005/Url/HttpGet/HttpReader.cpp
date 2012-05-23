#include "StdAfx.h"
#include "HttpReader.h"

//////////////////////////////////////////////////////////////////////////

void CHttpReaderSock::Close()
{

}

bool CHttpReaderSock::Connect(LPCTSTR szAddr, unsigned short nPort)
{
	return false;
}

bool CHttpReaderSock::Connect(const SOCKADDR* psa, int len)
{
	return false;
}

DWORD CHttpReaderSock::SetSocketTimeout(DWORD dwNewTimeout)
{
	return 0;
}

bool CHttpReaderSock::Write(WSABUF *pBuffers, int nCount, DWORD *pdwSize)
{
	return 0;
}

bool CHttpReaderSock::Write(const unsigned char *pBuffIn, DWORD *pdwSize)
{
	return 0;
}

bool CHttpReaderSock::SupportsScheme(ATL_URL_SCHEME scheme)
{
	return 0;
}

bool CHttpReaderSock::Read(const unsigned char *pBuff, DWORD *pdwSize)
{
	return BuffRead(pBuff, pdwSize);
}

//////////////////////////////////////////////////////////////////////////

CHttpReader::CHttpReader(void)
{
	ClearReader();
}

CHttpReader::~CHttpReader(void)
{
}

/*
 *	初始化HTTP
 */
void CHttpReader::ClearReader(void)
{
	m_state = rs_init;
	m_pHeaderBegin = NULL;
	m_pBodyBegin = NULL;
	memset(&m_navigatedata, 0, sizeof(m_navigatedata));
	ResetRequest();

	ChunkedBody.nChunkBuffCarryOver = 0;
	ChunkedBody.nChunkSize = 0;
	ChunkedBody.nTChunkBuffSize = CHUNK_BUFF_SIZE;
	ChunkedBody.chunk_buffer = NULL;
	ChunkedBody.chunk_buffer_end = NULL;
	ChunkedBody.cstate = READ_CHUNK_SIZE;
	ChunkedBody.cresult = LEX_OK;
	ChunkedBody.result_buffer.Empty();
	if(NULL != ChunkedBody.t_chunk_buffer.m_pData)
	{
		ChunkedBody.t_chunk_buffer.Free();
	}
}

/*
 *	添加接收的字符串
 *  返回值: TRUE表示全部接收完成, FALSE表示HTTP未接收完
 */
bool CHttpReader::AddRecv(const unsigned char *pBuff, long dwSize)
{
	m_pNavData = &m_navigatedata;
	m_pReadData = const_cast<unsigned char *>(pBuff);
	m_dwReadStart = 0;
	m_dwReadLen = dwSize;
	m_bReadFailed = false;

	while( !m_bReadFailed )
	{
		if( ReadHttpResponseEx() )
		{
			return true;
		}
	}

	return false;
}

/*
 *	读取请求
 */
bool CHttpReader::ReadHttpResponseEx()
{
	switch(m_state)
	{
	case rs_init:
		m_HeaderMap.RemoveAll();
		m_nStatus = ATL_INVALID_STATUS;
		m_dwHeaderLen = 0;
		m_dwBodyLen = 0;
		m_state = rs_readheader;
		// fall through

	case rs_readheader:

		// read from the socket until we have a complete set of headers.
		m_pBodyBegin = FindHeaderEnd(&m_pHeaderBegin);
		if (!m_pBodyBegin)
		{
			if (!ReadSocket())
			{
				// Either reading from the socket failed, or there
				// was not data to read. Set the nav status to error
				// and change the state to complete.
// 				if( !m_bReadFailed )
// 				{
					m_state = rs_complete;
// 				}
// 				result = RR_READSOCKET_FAILED;
				break;
			}
			else
				break; // loop back and FindHeaderEnd again.
		}
		// we have a complete set of headers
		m_dwHeaderLen = (DWORD)(m_pBodyBegin - m_pHeaderBegin);
		m_dwHeaderStart = (DWORD)(m_pHeaderBegin - (BYTE*)(LPCSTR)m_current);
		// fall through
		m_state = rs_scanheader;

	case rs_scanheader:
		// set m_nStatus and check for valid status
		ParseStatusLine(m_pHeaderBegin);
		// failed to set m_nStatus;
		if (m_nStatus == ATL_INVALID_STATUS)
		{
			m_state = rs_complete;
// 			result = RR_STATUS_INVALID;
			break;
		}

		else if (m_nStatus == 100) // continue
		{
			m_state = rs_init;
			break;
		}

		// crack all the headers and put them into a header map. We've already
		// done the check to make sure we have a complete set of headers in 
		// rs_readheader above
		if (ATL_HEADER_PARSE_COMPLETE != CrackResponseHeader((LPCSTR)m_pHeaderBegin, 
			(LPCSTR*)&m_pBodyBegin))
		{
			// something bad happened while parsing the headers!
			m_state = rs_complete;
// 			result = RR_PARSEHEADERS_FAILED;
			break;
		}
		m_state = rs_readbody;
		// fall through

	case rs_readbody:
		// headers are parsed and cracked, we're ready to read the rest
		// of the response. 
		if (IsMsgBodyChunked())
		{
			if (!ReadChunkedBodyEx())
			{
// 				result = RR_READCHUNKEDBODY_FAILED;
				if( !m_bReadFailed )
				{
					m_state = rs_complete;
				}
				break;
			}
		}
		else
			if (!ReadBody(GetContentLength(), m_current.GetLength()-(m_dwHeaderStart+m_dwHeaderLen)))
			{
// 				result = RR_READBODY_FAILED;
				break;
			}
		m_state = rs_complete;
		//fall through

	case rs_complete:
		// clean up the connection if the server requested a close;
		DisconnectIfRequired();
		break;
	}

	return ( rs_complete == m_state );
}

bool CHttpReader::ReadChunkedBodyEx()
{
	// At this point, m_current contains the headers, up to and including the \r\n\r\n,
	// plus any additional data that might have been read off the socket. So, we need
	// to copy off the additional data into our read buffer before we start parsing the
	// chunks.
#ifdef _DEBUG
	// nReadCount, keeps track of how many socket reads we do.
	int nReadCount = 0;
#endif

	// nChunkBuffCarryOver
	// When we run out of data in the input buffer, this is the
	// count of bytes that are left in the input that could not
	// be lexed into anything useful. We copy this many bytes to
	// the top of the input buffer before we fill the input buffer
	// with more bytes from the socket
// 	long nChunkBuffCarryOver = 0;
// 
// 	// nChunkSize
// 	// The size of the next chunk to be read from the input buffer.
// 	long nChunkSize = 0;
// 
// 	// t_chunk_buffer
// 	// The heap allocated buffer that we holds data
// 	// read from the socket. We will increase the size
// 	// of this buffer to 2 times the max chunk size we
// 	// need to read if we have to.
// 	CHeapPtr<char> t_chunk_buffer;
// 
// 	// nTChunkBuffSize
// 	// Keeps track of the allocated size of t_chunk_buffer.
// 	// This size will change if we need to read chunks bigger
// 	// than the currently allocated size of t_chunk_buffer.
// 	long nTChunkBuffSize = CHUNK_BUFF_SIZE;
// 
// 	// chunk_buffer & chunk_buffer_end
// 	// Keeps track of the current location
// 	// in t_chunk_buffer that we are lexing input from.
// 	// chunk_buffer_end is the end of the input buffer we
// 	// are lexing from. chunk_buffer_end is used as a marker
// 	// to make sure we don't read past the end of our input buffer
// 	char *chunk_buffer, *chunk_buffer_end;
// 
// 	// cstate
// 	// The current state of the chunk parsing state machine. We
// 	// start out reading the size of the first chunk.
// 	CHUNK_STATE cstate = READ_CHUNK_SIZE;
// 
// 	// cresult
// 	// Holds the value of the result of a lexing operation performed
// 	// on the input buffer.
// 	CHUNK_LEX_RESULT cresult = LEX_OK;
// 
// 	CAtlIsapiBuffer<> result_buffer;

	// Initialize pointers and allocate the chunk buffer.
	ChunkedBody.chunk_buffer = ChunkedBody.chunk_buffer_end = NULL;
	if( !ChunkedBody.t_chunk_buffer.Allocate(ChunkedBody.nTChunkBuffSize) )
		return false;

	// copy the headers into a temporary buffer.
	ChunkedBody.result_buffer.Append(m_current + m_dwHeaderStart, m_dwHeaderLen);

	// calculate number of bytes left in m_current past the headers
	long leftover_in_m_current = m_current.GetLength() - (m_dwHeaderStart + m_dwHeaderLen);

	// copy the extra bytes that might have been read into m_current into the chunk buffer
	if (leftover_in_m_current > 0)
	{
		if (leftover_in_m_current > ChunkedBody.nTChunkBuffSize)
		{
			if( ! ChunkedBody.t_chunk_buffer.Reallocate(leftover_in_m_current) )
				return false;
		}

		ChunkedBody.chunk_buffer = (char*)ChunkedBody.t_chunk_buffer;
		Checked::memcpy_s(ChunkedBody.chunk_buffer, leftover_in_m_current, ((LPCSTR)m_current)+ m_dwHeaderStart + m_dwHeaderLen, leftover_in_m_current);
		ChunkedBody.chunk_buffer_end = ChunkedBody.chunk_buffer + leftover_in_m_current;
	}

	m_current.Empty();
	m_dwBodyLen = 0;
	m_dwHeaderStart = 0;

	// as we start the state machine, we should be either pointing at the first
	// byte of chunked response or nothing, in which case we will need to get 
	// more data from the socket.
	ChunkedBody.nChunkSize = 0;

	bool bDone = false;

	while(!bDone)
	{
		// if we run out of data during processing, chunk_buffer
		// get set to null
		if (!ChunkedBody.chunk_buffer ||
			ChunkedBody.chunk_buffer >= ChunkedBody.chunk_buffer_end)
		{
			// we ran out of data in our input buffer, we need
			// to read more from the socket.
			DWORD dwReadBuffSize = ChunkedBody.nTChunkBuffSize - ChunkedBody.nChunkBuffCarryOver;
			ChunkedBody.chunk_buffer = ChunkedBody.t_chunk_buffer;
			if (!BuffRead((const unsigned char*)(ChunkedBody.chunk_buffer+ChunkedBody.nChunkBuffCarryOver), &dwReadBuffSize))
			{
				ATLTRACE("ReadChunkedBody: Error reading from socket (%d)\n", GetLastError());
				return false;
			}
			else if(dwReadBuffSize == 0)
			{
				ATLTRACE("ReadChunkedBody: The socket read timed out and no bytes were read from the socket.\n");
				return false;
			}
#ifdef _DEBUG
			ATLTRACE("ReadChunkedBody read %d bytes from socket. Reads %d \n", dwReadBuffSize, ++nReadCount);
#endif
			ChunkedBody.chunk_buffer_end = ChunkedBody.chunk_buffer + ChunkedBody.nChunkBuffCarryOver + dwReadBuffSize;
			ChunkedBody.nChunkBuffCarryOver = 0;
		}

		switch(ChunkedBody.cstate)
		{
		case READ_CHUNK_SIZE:
			{
				ChunkedBody.cresult = get_chunked_size(ChunkedBody.chunk_buffer, ChunkedBody.chunk_buffer_end, &ChunkedBody.nChunkSize);
				switch(ChunkedBody.cresult)
				{
				case LEX_ERROR:
					ATLTRACE("ReadChunkedBody Failed retrieving chunk size\n");
					return false;
					break;
				case LEX_OUTOFDATA:
					ChunkedBody.nChunkBuffCarryOver = (long)(ChunkedBody.chunk_buffer_end - ChunkedBody.chunk_buffer);
					if (!move_leftover_bytes((char*)ChunkedBody.t_chunk_buffer, ChunkedBody.nChunkBuffCarryOver, 
						ChunkedBody.chunk_buffer, ChunkedBody.chunk_buffer_end))
					{
						ATLTRACE("failed to move leftover chunk data to head of buffer\n");
						return false;
					}
					ChunkedBody.chunk_buffer = ChunkedBody.chunk_buffer_end = NULL;
					break;
				case LEX_OK:
					if (ChunkedBody.nChunkSize == 0)
					{
						ChunkedBody.cstate = CHUNK_READ_DATA_COMPLETE;
					}
					else if (ChunkedBody.nChunkSize + 2 > ChunkedBody.nTChunkBuffSize)
					{
						char *pBuffStart = (char*)ChunkedBody.t_chunk_buffer;
						int nReadSoFar = (int)(ChunkedBody.chunk_buffer - pBuffStart);
						int nTotal = (int)(ChunkedBody.chunk_buffer_end - pBuffStart);			
						if( FAILED(::ATL::AtlMultiply(&ChunkedBody.nTChunkBuffSize, ChunkedBody.nChunkSize, 2L)))
						{
							return false;
						}
						ChunkedBody.t_chunk_buffer.Reallocate(ChunkedBody.nTChunkBuffSize);
						pBuffStart = (char*)ChunkedBody.t_chunk_buffer;
						ChunkedBody.chunk_buffer = pBuffStart + nReadSoFar;
						ChunkedBody.chunk_buffer_end = pBuffStart + nTotal;
						ChunkedBody.cstate = READ_CHUNK_SIZE_FOOTER;
						m_dwBodyLen += ChunkedBody.nChunkSize;
					}
					else
					{
						// everything is OK. move to next state
						ChunkedBody.cstate = READ_CHUNK_SIZE_FOOTER;
						m_dwBodyLen += ChunkedBody.nChunkSize;
					}
					break;
				default:
					ATLASSERT(0);
					return false;
					break;
				}
			}
			break;
		case READ_CHUNK_DATA:
			{
				char *pDataStart = NULL;
				long nDataLen = 0;
				ChunkedBody.cresult = LEX_OK;
				ChunkedBody.cresult = get_chunked_data(ChunkedBody.chunk_buffer, ChunkedBody.chunk_buffer_end,
					ChunkedBody.nChunkSize, &pDataStart, &nDataLen);
				switch(ChunkedBody.cresult)
				{
				case LEX_ERROR:
					ATLTRACE("ReadChunkedBody failed to retrieve chunk data\n");
					return false;
					break;
				case LEX_OUTOFDATA:
					ChunkedBody.nChunkBuffCarryOver = (long)(ChunkedBody.chunk_buffer_end - ChunkedBody.chunk_buffer);
					if (!move_leftover_bytes((char*)ChunkedBody.t_chunk_buffer, ChunkedBody.nChunkBuffCarryOver, 
						ChunkedBody.chunk_buffer, ChunkedBody.chunk_buffer_end))
					{
						ATLTRACE("failed to move leftover chunk data to head of buffer\n");
						return false;
					}
					ChunkedBody.chunk_buffer = ChunkedBody.chunk_buffer_end = NULL;
					break;
				case LEX_OK:
					ChunkedBody.result_buffer.Append(pDataStart, nDataLen);
					ChunkedBody.cstate = READ_CHUNK_DATA_FOOTER;
					break;
				default:
					ATLASSERT(0);
					return false;
				}
			}
			break;
		case READ_CHUNK_SIZE_FOOTER:
		case READ_CHUNK_DATA_FOOTER:
			{
				ChunkedBody.cresult = consume_chunk_footer(ChunkedBody.chunk_buffer, ChunkedBody.chunk_buffer_end);
				switch(ChunkedBody.cresult)
				{
				case LEX_OK:
					ChunkedBody.cstate = (ChunkedBody.cstate == READ_CHUNK_SIZE_FOOTER) ? READ_CHUNK_DATA : READ_CHUNK_SIZE;
					break;
				case LEX_ERROR:
					ATLTRACE("Error consuming chunk footer!\n");
					return false;
					break;
				case LEX_OUTOFDATA:
					ChunkedBody.nChunkBuffCarryOver = (long)(ChunkedBody.chunk_buffer_end - ChunkedBody.chunk_buffer);
					if (!move_leftover_bytes((char*)ChunkedBody.t_chunk_buffer, ChunkedBody.nChunkBuffCarryOver, 
						ChunkedBody.chunk_buffer, ChunkedBody.chunk_buffer_end))
					{
						ATLTRACE("failed to move leftover chunk data to head of buffer\n");
						return false;
					}
					ChunkedBody.chunk_buffer = ChunkedBody.chunk_buffer_end = NULL;
					break;
				default:
					ATLASSERT(0);
					return false;

				}
			}
			break;
		case CHUNK_READ_DATA_COMPLETE:
			{
				// We read the chunk of size 0
				// consume the chunk footer.
				DWORD dwLen = 0;
				ChunkedBody.cresult = consume_chunk_footer(ChunkedBody.chunk_buffer, ChunkedBody.chunk_buffer_end);
				if (GetHeaderValue((_T("Trailer")), NULL, &dwLen))
				{
					ChunkedBody.cstate = READ_CHUNK_TRAILER; // start reading trailer headers
					break;
				}
				else
					bDone = true;
			}
			break;
		case READ_CHUNK_TRAILER:
			ChunkedBody.cresult = consume_chunk_trailer(ChunkedBody.chunk_buffer, ChunkedBody.chunk_buffer_end);
			switch(ChunkedBody.cresult)
			{
			case LEX_OK:
				ChunkedBody.cstate = READ_CHUNK_TRAILER; // keep reading
				break;
			case LEX_ERROR:
				ATLTRACE("Error consuming chunk trailers!\n");
				return false;
				break;
			case LEX_OUTOFDATA:
				ChunkedBody.nChunkBuffCarryOver = (long)(ChunkedBody.chunk_buffer_end - ChunkedBody.chunk_buffer);
				if (!move_leftover_bytes((char*)ChunkedBody.t_chunk_buffer, ChunkedBody.nChunkBuffCarryOver, 
					ChunkedBody.chunk_buffer, ChunkedBody.chunk_buffer_end))
				{
					ATLTRACE("failed to move leftover chunk data to head of buffer\n");
					return false;
				}
				ChunkedBody.chunk_buffer = ChunkedBody.chunk_buffer_end = NULL;
				break;
			case LEX_TRAILER_COMPLETE:
				return true;
				break;
			default:
				ATLASSERT(0);
				return false;



			}
			break;

		}
	}
	if (!m_current.Append((LPCSTR)ChunkedBody.result_buffer))
		return false;

	m_pEnd = ((BYTE*)(LPCSTR)m_current) + m_current.GetLength();

	return true;
}

bool CHttpReader::BuffRead(const unsigned char *pBuff, DWORD *pdwSize)
{
	DWORD		dwLen		= min(*pdwSize, m_dwReadLen - m_dwReadStart);

	if(NULL == m_pReadData
		|| m_dwReadStart >= m_dwReadLen)
	{
		m_bReadFailed = true;
		return false;
	}

	memcpy((char *)pBuff, m_pReadData + m_dwReadStart, dwLen);
	m_dwReadStart += dwLen;
	*pdwSize = dwLen;
	return true;
}

const BYTE* CHttpReader::GetResponse()
{
	return __super::GetResponse();
}
