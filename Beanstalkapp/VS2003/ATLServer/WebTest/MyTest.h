// MyTest.h : Defines the ATL Server request handler class
//
#pragma once



namespace MyTestService
{
// all struct, enum, and typedefs for your webservice should go inside the namespace

// IMyTestService - web service interface declaration
//
[
	uuid("9BC858CF-739F-4D82-8E22-008C29A3FBB4"), 
	object
]
__interface IMyTestService
{
	// HelloWorld is a sample ATL Server web service method.  It shows how to
	// declare a web service method and its in-parameters and out-parameters
	[id(1)] HRESULT HelloWorld([in] BSTR bstrInput, [out, retval] BSTR *bstrOutput);
	// TODO: Add additional web service methods here
};


// MyTestService - web service implementation
//
[
	request_handler(name="MyTestService", sdl="GenMyTestWSDL"),
	soap_handler(
		name="MyTestService", 
		namespace="urn:MyTestService",
		protocol="soap"
	)
]
class CMyTestService :
	public IMyTestService
{
public:
	[ soap_method ]
	HRESULT HelloWorld(/*[in]*/ BSTR bstrInput, /*[out, retval]*/ BSTR *bstrOutput)
	{
		CComBSTR bstrOut(L"Hello ");
		bstrOut += bstrInput;
		bstrOut += L"!";
		*bstrOutput = bstrOut.Detach();
		
		return S_OK;
	}
	// TODO: Add additional web service methods here
}; // class CMyTestService
} // namespace MyTestService
