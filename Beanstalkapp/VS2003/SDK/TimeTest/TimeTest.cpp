// TimeTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Atlbase.h>
#include <math.h>

#define TIME_MyBirthday				"1986-04-24 22:00:00"
#define TIME_MyBirthdayW			L"1986-04-24 22:00:00"

#define TICKSPERSEC        10000000
#define TICKSPERMSEC       10000
#define SECSPERDAY         86400
#define SECSPERHOUR        3600
#define SECSPERMIN         60
#define MINSPERHOUR        60
#define HOURSPERDAY        24

int _tmain(int argc, _TCHAR* argv[])
{
	VARIANT		varSrcTime;
	VARIANT		varDesTime;
	VARIANT		varLongTime;
	VARIANT		varSysTime;
	BSTR		spTime		= ::SysAllocString(TIME_MyBirthdayW);
	SYSTEMTIME	sysTime;
	FILETIME	fileTime;
	unsigned int days;

	VariantInit(&varSysTime);
	VariantInit(&varSrcTime);
	VariantInit(&varDesTime);
	VariantInit(&varLongTime);
	memset(&sysTime, 0, sizeof(sysTime));
	memset(&fileTime, 0, sizeof(fileTime));
	sscanf(TIME_MyBirthday, "%hd-%hd-%hd %hd:%hd:%hd", 
		&sysTime.wYear, &sysTime.wMonth, &sysTime.wDay, 
		&sysTime.wHour, &sysTime.wMinute, &sysTime.wSecond);
	// sysTime.wMonth += 24;
	int				a;


	a = pow(10, 5);
	SystemTimeToFileTime(&sysTime, &fileTime);
	//fileTime += Int32x32To64(SECSPERDAY, 10 ^ 5);
	memset(&sysTime, 0, sizeof(sysTime));
	FileTimeToSystemTime(&fileTime, &sysTime);
	// days = (unsigned __int64)*((unsigned __int64 *)&fileTime) / 1000 / 10000 / 60 / 60 / 24;
	varSysTime.vt = VT_R8;
	SystemTimeToVariantTime(&sysTime, &varSysTime.dblVal);
	varSrcTime.vt = VT_BSTR;
	varSrcTime.bstrVal = spTime;
	VariantChangeType(&varDesTime, &varSrcTime, 0, VT_DATE);
	VariantChangeType(&varLongTime, &varDesTime, 0, VT_R8);
	

// 	CTime			tt1;
// 	COleDateTime	tt2;
	return 0;
}

