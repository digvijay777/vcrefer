#pragma once

class CUSBControl
{
public:
	CUSBControl(void);
	virtual ~CUSBControl(void);
private:
	typedef BOOL (CUSBControl::*_CtrlFUN)(LPVOID);
	BOOL	EnableUSB(GUID& guid, BOOL bEnable, _CtrlFUN fnCtrl);
	BOOL	IsUSBStorage(VOID* pDisplay);
public:
	BOOL	EnableUSBStorage(BOOL bEnable);
};
