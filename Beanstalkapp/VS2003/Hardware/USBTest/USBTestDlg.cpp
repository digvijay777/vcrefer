// USBTestDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CUSBTestDlg �Ի���



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


// CUSBTestDlg ��Ϣ�������

BOOL CUSBTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUSBTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CUSBTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
bool   InitUSB()  
{  

	USHORT VendorID         =   0x0000;  
	USHORT ProductID		= 0x0000;
	DWORD dwProductID         =   0x0000;  


	//   �����豸  
	//   ������ʹ��HID�豸��API��������HID�豸�б��ҳ���Vendor   ID   ��   Product   IDƥ����豸  

// 	ClassDeviceData                         =NULL;  
// 	HIDDevice                                 =NULL;  
	SP_DEVICE_INTERFACE_DATA		deviceInfoData = {0};
 	deviceInfoData.cbSize         =sizeof(deviceInfoData);  
	GUID						hidGuid = {0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED};
	//   �Ӳ���ϵͳ��ȡHIDs   ��GUID 
// 	HidD_GetHidGuid(&hidGuid);  
	/*GUID_CLASS_USB_DEVICE*/
	//   ��ȡ����HIDs���豸��Ϣ  
	HDEVINFO  AllHIDDeviceInfo = SetupDiGetClassDevs(&hidGuid,NULL,NULL,  
		DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);  

	DWORD 	HIDCounter=0;  

	while   (TRUE)  
	{  

		//   ���API�����ֵ��豸��Ϣд��   deviceInfoData  
		//   HIDCounter   �������API�ظ���������HID�豸  
		//   ���API���÷���0,û�и����HID�豸����  
		if   (!SetupDiEnumDeviceInterfaces(AllHIDDeviceInfo,0,&hidGuid,  
			HIDCounter,&deviceInfoData))  
		{  
			//   û�з�����Vendor   ID   ��   Product   IDƥ���HID�豸  

			SetupDiDestroyDeviceInfoList(AllHIDDeviceInfo);          
			return   FALSE;  
		}  
		else    
		{  
			
			//   ����һ��HID�豸,��ȡ�豸����ϸ��Ϣ  
			//   ��һ�ε���SetupDiGetDeviceInterfaceDetail�õ�ClassDeviceData  
			//   �Ĵ�С,�����ش���  
			DWORD requiredLength;
			SetupDiGetDeviceInterfaceDetail(AllHIDDeviceInfo,&deviceInfoData,  
				NULL,0,&requiredLength,NULL);  

			DWORD neededLength                         =requiredLength;  
			PSP_DEVICE_INTERFACE_DETAIL_DATA ClassDeviceData                         =(PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(neededLength);  
			ClassDeviceData->cbSize   =sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);  

			//�ڶ��ε���SetupDiGetDeviceInterfaceDetail    
			//   ʹ��   ���ʵ�neededLength.  
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

			//   ����HID�豸�ľ��  
// 			HANDLE HIDDevice=CreateFile(ClassDeviceData->DevicePath,  
// 				GENERIC_READ|GENERIC_WRITE,  
// 				FILE_SHARE_READ|FILE_SHARE_WRITE,  
// 				NULL,OPEN_EXISTING,0,NULL);  

			//   ��ȡ   attributes   �Ա�õ�Vendor   ID   ��   Product   ID  
// 			HIDD_ATTRIBUTES Attributes;
// 			HidD_GetAttributes(HIDDevice,&Attributes);  

// 			if   ((Attributes.VendorID   ==   VendorID)   &&  
// 				(Attributes.ProductID   ==   ProductID))  
// 			{  
// 
// 				//   �ҵ���ƥ���Vendor   ID   ��   Product   ID��HID�豸  
// 				//   ����ReadHIDDevice�豸�ľ���Ա��ȡ��Ϣ  
// 
// 				HANDLE ReadHIDDevice=CreateFile(ClassDeviceData->DevicePath,  
// 					GENERIC_READ|GENERIC_WRITE,  
// 					FILE_SHARE_READ|FILE_SHARE_WRITE,  
// 					NULL,OPEN_EXISTING,0,NULL);  
// 				free(ClassDeviceData);  
// 				SetupDiDestroyDeviceInfoList(AllHIDDeviceInfo);  

				//��ʼ���ɹ�  
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
		NewState = DICS_DISABLE;//����
	}
	else
	{
		NewState = DICS_ENABLE;//����
	}

	//��ȡ���󼯺�
	hDevInfo = SetupDiGetClassDevs(&DeviceId, NULL, NULL, DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
	if(hDevInfo == INVALID_HANDLE_VALUE)
	{
		goto EnableDevice_Done;
	}

	//ö�ٽӿ�
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

		//���ø��豸
		PropChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
		PropChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
		PropChangeParams.Scope = DICS_FLAG_GLOBAL;
		PropChangeParams.StateChange = NewState;//���ã�DICS_DISABLE��DICS_ENABLE����

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
