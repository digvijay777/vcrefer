#include "stdafx.h"
#include "SmallSkin.h"
#include "LunaApp.h"

CLunaApp		gSkinApp;

BOOL __stdcall InstallSmallSkin()
{
	return gSkinApp.Install();
}

BOOL __stdcall UninstallSmallSkin()
{
	return gSkinApp.Uninstall();
}