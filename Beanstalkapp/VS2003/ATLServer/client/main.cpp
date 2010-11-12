#include <WinSock2.h>
#include <Windows.h>
#include <comutil.h>
#include <iostream>

#include "WebTestService.h"

#pragma comment(lib, "comsupp.lib")

// sproxy.exe

using namespace std;
int main()
{
	CoInitialize(NULL);

	WebTestService::CWebTestService		myServer;
	_bstr_t				bstr;

	myServer.SetUrl("http://192.168.1.61:80/WebTest/WebTest.dll?Handler=Default");
	myServer.HelloWorld(L"", bstr.GetAddress());
	cout << (char *)bstr << endl;

	myServer.Release();
	//CoUninitialize();
	system("pause");
	return 0;
}