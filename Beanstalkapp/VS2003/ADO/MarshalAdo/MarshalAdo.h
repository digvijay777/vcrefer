// MarshalAdo.h : Defines the ATL Server request handler class
//
#pragma once
#include "../../../ExtClass/ado/TransADO.h"
#include "../../../../ExtClass/ado/SQLString.h"
#import "msxml3.dll"

[ request_handler("Default") ]
class CMarshalAdoHandler
{
private:
	// Put private members here

protected:
	// Put protected members here

public:
	// Put public members here

	HTTP_CODE ValidateAndExchange()
	{
		// TODO: Put all initialization and validation code here
		m_HttpResponse.SetContentType("text/data");
		

		try{
			TQuickADO<void>		ado;
			TSQLString<>		sql;
			_RecordsetPtr		spRes;
			_variant_t			vtSave;
			IStreamPtr			spStream;
			CHAR				szBuff[1024]	= {0};
			ULONG				nSize;
			HRESULT				hRes			= E_FAIL;
			ULARGE_INTEGER		seek			= {0};
			LARGE_INTEGER		begin			= {0};
			DWORD				dwWrite			= 0;

			ado.Open(L"Provider=SQLOLEDB.1;User ID=sa;Password=aaBB12,.Xm;Persist Security Info=True"
				L";Initial Catalog=License;Network Address=192.168.1.234");
			sql.format(L"select * from grouptype");
			ado.ExecuteStatic(sql, spRes);
			spStream.CreateInstance(__uuidof(IStream));
			CreateStreamOnHGlobal(NULL, TRUE, &spStream);
			vtSave.vt = VT_UNKNOWN;
			vtSave.punkVal = spStream;
			hRes = spRes->Save(vtSave, adPersistADTG);
			vtSave.punkVal = NULL;
			vtSave.vt = VT_NULL;
			spStream->Seek(begin, STREAM_SEEK_SET, &seek);
			do 
			{
				nSize = sizeof(szBuff);
				spStream->Read(szBuff, nSize, &nSize);
				//AtlHexEncode((BYTE*)szBuff, nSize, szHex, &nHex);
				//m_HttpResponse << szHex;
				m_HttpResponse.WriteStream(szBuff, nSize, &dwWrite);
			} while (sizeof(szBuff) == nSize);
			
		}
		catch(...)
		{

		}


		return HTTP_SUCCESS_NO_PROCESS;
	}
 
protected:
	// Here is an example of how to use a replacement tag with the stencil processor
	[ tag_name(name="Hello") ]
	HTTP_CODE OnHello(void)
	{
		m_HttpResponse << "Hello World!";
		return HTTP_SUCCESS;
	}
}; // class CMarshalAdoHandler
