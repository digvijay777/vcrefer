#include "stdafx.h"
#include "../../../ExtClass/project/service/ServiceBase.h"

class CMyService : public TServiceBase<CMyService>
{
public:
	CMyService();
	~CMyService();
public:
	virtual void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
};

CMyService::CMyService()
:TServiceBase<CMyService>("TST-x", "TServiceTest", "Service Template test", "test")
{

}

CMyService::~CMyService()
{

}

void CMyService::ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	while(IsServiceRun())
	{
		Sleep(1);
	}
}

class CMyService2 : public TServiceBase<CMyService2>
{
public:
	CMyService2();
	virtual void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
};

CMyService2::CMyService2()
:TServiceBase<CMyService2>("TST-x2", "TServiceTest2", "Service Template test2", "test2")
{

}

void CMyService2::ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	while(IsServiceRun())
	{
		Sleep(1);
	}
}

CMyService		myService;
CMyService2		myService2;

int main(int argc, char* argv[])
{
	if(TRUE == myService.ParseCommand(argc, argv))
		return 0;
	else if(TRUE == myService2.ParseCommand(argc, argv))
		return 0;

	return 0;
}