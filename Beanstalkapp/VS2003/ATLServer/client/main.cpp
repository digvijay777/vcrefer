#include <WinSock2.h>
#include <Windows.h>
#include <comutil.h>
#include <iostream>

#include "WebTestService.h"

#pragma comment(lib, "comsupp.lib")

using namespace std;
int main()
{
	CoInitialize(NULL);

	WebTestService::CWebTestService		myServer;
	_bstr_t				bstr;

	myServer.HelloWorld(L"", bstr.GetAddress());
	cout << (char *)bstr << endl;

	myServer.Release();
	//CoUninitialize();
	system("pause");
	return 0;
}