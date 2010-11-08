// IStreamWrite.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	IStorage*		pStg		= NULL;
	IStream*		pStream		= NULL;
	LONG			l			= 0;
	HRESULT			hRes		= 0;
	ULONG			cb			= 0;

	l = ::StgOpenStorage(L"C:\\MyStorageFile", NULL, STGM_READWRITE | STGM_DIRECT | STGM_SHARE_EXCLUSIVE, NULL, 0, &pStg);
	if(FAILED(l))
	{
		cout<<"StgOpenStorage() failed. \n";
		exit(0);
	}
	hRes = pStg->OpenStream(L"MyStream", NULL, STGM_READWRITE | STGM_DIRECT | STGM_SHARE_EXCLUSIVE, 0, &pStream);
	if(FAILED(hRes))
	{
		pStg->Release();
		cout<<"OpenStream() failed."<<endl;
		exit(0);
	}
	hRes = pStream->Write(L"Hello", 12, &cb);
	if(FAILED(hRes))
		cout<<"Write() failed"<<endl;
	pStream->Release();
	pStg->Release();
	return 0;
}

