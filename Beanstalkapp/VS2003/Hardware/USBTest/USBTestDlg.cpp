// USBTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "USBTest.h"
#include "USBTestDlg.h"
#include ".\usbtestdlg.h"

// http://msdn.krugle.com/kse/entfiles?query=HidD_GetHidGuid#3
DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE, 0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, \
			0xC0, 0x4F, 0xB9, 0x51, 0xED);
#define GUID_CLASS_USB_DEVICE           GUID_DEVINTERFACE_USB_DEVICE

#include <Dbt.h>
#include <Setupapi.h>

#pragma comment(lib, "Setupapi.lib")

#include "USBControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CUSBTestDlg 对话框



CUSBTestDlg::CUSBTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUSBTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUSBTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUSBTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CUSBTestDlg 消息处理程序

BOOL CUSBTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CUSBTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUSBTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CUSBTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
bool   InitUSB()  
{  

	USHORT VendorID         =   0x0000;  
	USHORT ProductID		= 0x0000;
	DWORD dwProductID         =   0x0000;  


	//   查找设备  
	//   本例程使用HID设备的API，它查找HID设备列表，找出与Vendor   ID   和   Product   ID匹配的设备  

// 	ClassDeviceData                         =NULL;  
// 	HIDDevice                                 =NULL;  
	SP_DEVICE_INTERFACE_DATA		deviceInfoData = {0};
 	deviceInfoData.cbSize         =sizeof(deviceInfoData);  
	GUID						hidGuid = {0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED};
	//   从操作系统获取HIDs   的GUID 
// 	HidD_GetHidGuid(&hidGuid);  
	/*GUID_CLASS_USB_DEVICE*/
	//   获取所有HIDs的设备信息  
	HDEVINFO  AllHIDDeviceInfo = SetupDiGetClassDevs(&hidGuid,NULL,NULL,  
		DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);  

	DWORD 	HIDCounter=0;  

	while   (TRUE)  
	{  

		//   这个API将发现的设备信息写入   deviceInfoData  
		//   HIDCounter   允许这个API重复调用所有HID设备  
		//   如果API调用返回0,没有更多的HID设备发现  
		if   (!SetupDiEnumDeviceInterfaces(AllHIDDeviceInfo,0,&hidGuid,  
			HIDCounter,&deviceInfoData))  
		{  
			//   没有发现与Vendor   ID   和   Product   ID匹配的HID设备  

			SetupDiDestroyDeviceInfoList(AllHIDDeviceInfo);          
			return   FALSE;  
		}  
		else    
		{  
			
			//   发现一个HID设备,获取设备的详细信息  
			//   第一次调用SetupDiGetDeviceInterfaceDetail得到ClassDeviceData  
			//   的大小,但返回错误  
			DWORD requiredLength;
			SetupDiGetDeviceInterfaceDetail(AllHIDDeviceInfo,&deviceInfoData,  
				NULL,0,&requiredLength,NULL);  

			DWORD neededLength                         =requiredLength;  
			PSP_DEVICE_INTERFACE_DETAIL_DATA ClassDeviceData                         =(PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(neededLength);  
			ClassDeviceData->cbSize   =sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);  

			//第二次调用SetupDiGetDeviceInterfaceDetail    
			//   使用   合适的neededLength.  
// 			if   (!SetupDiGetDeviceInterfaceDetail(AllHIDDeviceInfo,&deviceInfoData,  
// 				ClassDeviceData,neededLength,&requiredLength,NULL))  
// 			{  
// 				free(ClassDeviceData);  
// 				SetupDiDestroyDeviceInfoList(AllHIDDeviceInfo);          
// 				return   FALSE;  
// 			}  
// 			free(ClassDeviceData);  

// 			SP_DEVINFO_DATA deviceInfo;
// 			SetupDiGetDeviceRegistryProperty(AllHIDDeviceInfo, &deviceInfo, 0, NULL, NULL, 0, NULL);

// 			SP_DEVICE_INTERFACE_DATA	info;
// 			SetupDiOpenDeviceInterfaceRegKey(AllHIDDeviceInfo, &info, 0, 0);

			BYTE			bzProp[1024];
			DWORD			dwPropSize;
			DWORD			dwType;
			SP_DEVINFO_DATA DevInfoData;
			memset( &DevInfoData, 0, sizeof(DevInfoData));
			DevInfoData.cbSize = sizeof(DevInfoData);

			dwPropSize = sizeof(bzProp);
			BOOL bRetVal = SetupDiGetDeviceRegistryProperty(AllHIDDeviceInfo, &DevInfoData, SPDRP_DEVICEDESC,
				&dwType, bzProp, dwPropSize, &dwPropSize);
			if(bRetVal == FALSE)
			{
				
			}

			//   建立HID设备的句柄  
// 			HANDLE HIDDevice=CreateFile(ClassDeviceData->DevicePath,  
// 				GENERIC_READ|GENERIC_WRITE,  
// 				FILE_SHARE_READ|FILE_SHARE_WRITE,  
// 				NULL,OPEN_EXISTING,0,NULL);  

			//   获取   attributes   以便得到Vendor   ID   和   Product   ID  
// 			HIDD_ATTRIBUTES Attributes;
// 			HidD_GetAttributes(HIDDevice,&Attributes);  

// 			if   ((Attributes.VendorID   ==   VendorID)   &&  
// 				(Attributes.ProductID   ==   ProductID))  
// 			{  
// 
// 				//   找到了匹配的Vendor   ID   和   Product   ID的HID设备  
// 				//   建立ReadHIDDevice设备的句柄以便读取信息  
// 
// 				HANDLE ReadHIDDevice=CreateFile(ClassDeviceData->DevicePath,  
// 					GENERIC_READ|GENERIC_WRITE,  
// 					FILE_SHARE_READ|FILE_SHARE_WRITE,  
// 					NULL,OPEN_EXISTING,0,NULL);  
// 				free(ClassDeviceData);  
// 				SetupDiDestroyDeviceInfoList(AllHIDDeviceInfo);  

				//初始化成功  
// 				return   TRUE;                          
// 			}  
// 			else  
// 			{  
// 				CloseHandle(HIDDevice);          
// 			}  
			HIDCounter   =   HIDCounter+1;  

		}  
	}  

}   

BOOL	EnableDeviceSmp(GUID &DeviceId, BOOL bEnable)
{
	DWORD			NewState;
	DWORD			i;
	HDEVINFO		hDevInfo;
	SP_DEVINFO_DATA DevInfoData;
	BOOL			bRetVal;
	int				nError;

	SP_PROPCHANGE_PARAMS	PropChangeParams;

	nError = ERROR_SUCCESS;

	if(bEnable == FALSE)
	{
		NewState = DICS_DISABLE;//禁用
	}
	else
	{
		NewState = DICS_ENABLE;//启用
	}

	//获取对象集合
	hDevInfo = SetupDiGetClassDevs(&DeviceId, NULL, NULL, DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
	if(hDevInfo == INVALID_HANDLE_VALUE)
	{
		goto EnableDevice_Done;
	}

	//枚举接口
	memset( &DevInfoData, 0, sizeof(DevInfoData));
	DevInfoData.cbSize = sizeof(DevInfoData);

	SP_DEVICE_INTERFACE_DATA		deviceInfoData = {0};
	deviceInfoData.cbSize         =sizeof(deviceInfoData);  

	for(i=0; 
		SetupDiEnumDeviceInfo(hDevInfo, i, &DevInfoData);
// 	SetupDiEnumDeviceInterfaces(hDevInfo,0,&DeviceId,  
// 								i,&deviceInfoData);
		i++)
	{
		BYTE			bzProp[1024];
		DWORD			dwPropSize;
		DWORD			dwType;

		dwPropSize = sizeof(bzProp);
		bRetVal = SetupDiGetDeviceRegistryProperty(hDevInfo, &DevInfoData, SPDRP_DEVICEDESC,
			&dwType, bzProp, dwPropSize, &dwPropSize);
		if(bRetVal == FALSE)
		{
			nError = GetLastError();
			goto EnableDevice_Done;
		}

		//禁用该设备
		PropChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
		PropChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
		PropChangeParams.Scope = DICS_FLAG_GLOBAL;
		PropChangeParams.StateChange = NewState;//禁用：DICS_DISABLE，DICS_ENABLE启用

		bRetVal = SetupDiSetClassInstallParams(hDevInfo, &DevInfoData,
			(SP_CLASSINSTALL_HEADER *)&PropChangeParams, sizeof(PropChangeParams));
		if(bRetVal == FALSE)
		{
			nError = GetLastError();
			goto EnableDevice_Done;
		}

		bRetVal = SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, hDevInfo, &DevInfoData);
		if(bRetVal == FALSE)
		{
			nError = GetLastError();
			goto EnableDevice_Done;
		}

	}


EnableDevice_Done:
	SetupDiDestroyDeviceInfoList(hDevInfo);

	if(nError == ERROR_SUCCESS)
		return TRUE;

	SetLastError(nError);
	return FALSE;
}

void TestUsb()
{
	GUID			usb[5];
	DWORD			dwSize		= 5;


}

GUID guidUsb = {0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED};
HDEVNOTIFY   gNotidy		= NULL;
void CUSBTestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
// 	GUID  USB = {0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED};
// 	EnableDeviceSmp(USB, TRUE);
// 	TestUsb();
// 	CUSBControl		usb;
// 
// 	usb.EnableUSBStorage(TRUE);
	DEV_BROADCAST_DEVICEINTERFACE		di;

	memset(&di, 0, sizeof(di));
	di.dbcc_size = sizeof(di);
	di.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	di.dbcc_reserved = 0;
	di.dbcc_classguid = guidUsb;
	gNotidy =  RegisterDeviceNotification((HANDLE)m_hWnd, &di, DEVICE_NOTIFY_WINDOW_HANDLE);
}

void CUSBTestDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	UnregisterDeviceNotification(gNotidy);
	CDialog::OnClose();
}

LRESULT CUSBTestDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_DEVICECHANGE == message)
	{
		if(DBT_DEVICEARRIVAL == wParam || DBT_DEVICEREMOVECOMPLETE == wParam)
		{

		}
		return 0;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CUSBTestDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	UnregisterDeviceNotification(gNotidy);
}
