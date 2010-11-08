#include   "stdafx.h "  
#include     <tchar.h>                    
#include     <windows.h>            
#include     <setupapi.h>  

#pragma   comment   (lib, "setupapi.lib")  

#define   UnknownDevice   TEXT( " ")  

HDEVINFO   hDevInfo   =   0;  


BOOL   StateChange(DWORD   NewState,   DWORD   SelectedItem,HDEVINFO   hDevInfo)  
{  
	SP_PROPCHANGE_PARAMS   PropChangeParams   =   {sizeof(SP_CLASSINSTALL_HEADER)};  
	SP_DEVINFO_DATA   DeviceInfoData   =   {sizeof(SP_DEVINFO_DATA)};  
	HCURSOR   hCursor;  

	//  
	//   This   may   take   a   while   :^(  
	//  
	hCursor   =   SetCursor(LoadCursor(NULL,   IDC_WAIT));  

	//  
	//   Get   a   handle   to   the   Selected   Item.  
	//  
	if   (!SetupDiEnumDeviceInfo(hDevInfo,SelectedItem,&DeviceInfoData))  
	{  
		printf( "EnumDeviceInfo ");  
		return   FALSE;  
	}  

	//  
	//   Set   the   PropChangeParams   structure.  
	//  
	PropChangeParams.ClassInstallHeader.InstallFunction   =   DIF_PROPERTYCHANGE;  
	PropChangeParams.Scope   =   DICS_FLAG_GLOBAL;  
	PropChangeParams.StateChange   =   NewState;  

	if   (!SetupDiSetClassInstallParams(hDevInfo,  
		&DeviceInfoData,  
		(SP_CLASSINSTALL_HEADER   *)&PropChangeParams,  
		sizeof(PropChangeParams)))  
	{  
		printf( "SetClassInstallParams ");  
		SetCursor(hCursor);  
		return   FALSE;  
	}  

	//  
	//   Call   the   ClassInstaller   and   perform   the   change.  
	//  
	if   (!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE,  
		hDevInfo,  
		&DeviceInfoData))  
	{  
		printf( "SetClassInstallParams ");  
		SetCursor(hCursor);  
		return   TRUE;  
	}  

	SetCursor(hCursor);  
	return   TRUE;  
}  


BOOL   IsClassNet(   GUID   *   ClassGuid   )  
{  
#define   MAX_NUM   50  

	HKEY   hKeyClass;  
	LONG   lRet;  
	char   ClassType[MAX_NUM];  
	char   NetClass[MAX_NUM]   =   "Net ";  
	DWORD   dwLength   =   MAX_NUM,dwType   =   REG_SZ;  

	if   (hKeyClass   =   SetupDiOpenClassRegKey(ClassGuid,KEY_READ))  
	{  
		lRet   =   RegQueryValueEx(hKeyClass,  
			TEXT( "Class "),  
			NULL,   &dwType,   LPBYTE(ClassType),   &dwLength);  
		RegCloseKey(hKeyClass);  

		if   (lRet   !=   ERROR_SUCCESS)  
			return   FALSE;  

		if   (!strcmp(ClassType,NetClass))  
			return   TRUE;  
	}  

	return   FALSE;  
}  

int   netadapter()  
{  
	if   (INVALID_HANDLE_VALUE   ==   (hDevInfo   =  
		SetupDiGetClassDevs(NULL,NULL,0,  
		DIGCF_PRESENT   |DIGCF_ALLCLASSES)))  
	{  
		printf( "GetClassDevs ");  
		return   0;  
	}  

	DWORD   i,   Status,   Problem;  
	SP_DEVINFO_DATA   DeviceInfoData   =   {sizeof(SP_DEVINFO_DATA)};  

	HKEY   hKeyClass;  
	char   DeviceName[200];  
	for   (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,&DeviceInfoData);i++)  
	{  
		if   (IsClassNet(&DeviceInfoData.ClassGuid))  
		{  
			printf( "   the   class 's   index   is   %d\n ",i);  
			//  
			//DICS_DISABLE--> ½ûÓÃÍøÂç  
			//DICS_ENABLE---> »Ö¸´ÍøÂç  
			//  
			if   (StateChange(DICS_ENABLE,i,hDevInfo))  
				printf( "NetWork   stopped ");  

		}  
	}  

	return   0;  
}   