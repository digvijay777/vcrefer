#include "stdafx.h"
#include <Windows.h>
#include "Resource.h"
#include <gdiplus.h>
#include <Gdiplusinit.h>
#include <tchar.h>
#include "FloatingWnd.h"

#pragma comment(lib, "gdiplus.lib")

void DialogMain(Gdiplus::Image* pImage);

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ULONG_PTR								pGdiToken					= NULL;
	Gdiplus::GdiplusStartupInput			gdiplusStartupInput;
	Gdiplus::Image*							pPngImage					= NULL;
	HRSRC									hrsrc						= NULL;

	AfxWinInit(hInstance, NULL, ::GetCommandLine(), nCmdShow);
	AfxSetResourceHandle(hInstance);

	GdiplusStartup(&pGdiToken, &gdiplusStartupInput, NULL);
	// ����ͼƬ
	hrsrc = FindResource(hInstance, MAKEINTRESOURCE(IDR_PNG_FLOATING), _T("PNG"));
	if(NULL != hrsrc)
	{
		DWORD			dwSize			= 0;
		HGLOBAL			hRes			= NULL;
		BYTE*			pData			= NULL;
		IStream*		pStream			= NULL;

		dwSize = SizeofResource(hInstance, hrsrc);
		hRes = LoadResource(hInstance, hrsrc);
		pData = (BYTE*)LockResource(hRes);
		CreateStreamOnHGlobal(NULL, TRUE, &pStream);
		if(NULL != pStream)
		{
			DWORD		dwWrite			= 0;

			pStream->Write(pData, dwSize, &dwWrite);
			pPngImage = Gdiplus::Image::FromStream(pStream);
			pStream->Release();
		}
		UnlockResource(hRes);
		FreeResource(hRes);
	}
	// �Ի��򹤳�
	DialogMain(pPngImage);
	// ������
	if(NULL != pPngImage)
		delete pPngImage;
	Gdiplus::GdiplusShutdown(pGdiToken);
	return 0;
}

// �Ի���������
void DialogMain(Gdiplus::Image* pImage)
{
	CFloatingWnd		cfb/*(pImage, 3)*/;

	cfb.SetClientRect(3, 0, 220, 27);
	cfb.Create(pImage, 3);
	cfb.ShowWindow(SW_SHOW);
	//cfb.UpdateWindow();
	cfb.SetShowText(L"����ʹ��: %02dʱ%02d��", 2, 5);
	
	// ��Ϣѭ��
	MSG			msg;

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}