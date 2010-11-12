// WebTest.h : Defines the ATL Server request handler class
//
#pragma once

namespace WebTestService
{
// all struct, enum, and typedefs for your webservice should go inside the namespace

// IWebTestService - web service interface declaration
//
[
	uuid("C11363AB-ADDB-4443-8E48-1563C6DE3707"), 
	object
]
__interface IWebTestService
{
	// HelloWorld is a sample ATL Server web service method.  It shows how to
	// declare a web service method and its in-parameters and out-parameters
	[id(1)] HRESULT HelloWorld([in] BSTR bstrInput, [out, retval] BSTR *bstrOutput);
	// TODO: Add additional web service methods here
};


// WebTestService - web service implementation
//
[
	request_handler(name="Default", sdl="GenWebTestWSDL"),
	soap_handler(
		name="WebTestService", 
		namespace="urn:WebTestService",
		protocol="soap"
	)
]
class CWebTestService :
	public IWebTestService
{
public:
	// This is a sample web service method that shows how to use the 
	// soap_method attribute to expose a method as a web method
	[ soap_method ]
	HRESULT HelloWorld(/*[in]*/ BSTR bstrInput, /*[out, retval]*/ BSTR *bstrOutput)
	{
		CComBSTR bstrOut(L"服务器名为: ");
		//bstrOut += bstrInput;
		//bstrOut += L"!";
		
		WCHAR	sname[MAX_PATH]		= {0};
		DWORD	dwSize				= sizeof(sname)/sizeof(WCHAR);

		GetComputerNameW(sname, &dwSize);
		bstrOut += sname;

		*bstrOutput = bstrOut.Detach();
		return S_OK;
	}
	// TODO: Add additional web service methods here
}; // class CWebTestService

} // namespace WebTestService
