// License2Doc.h :  CLicense2Doc 类的接口
//


#pragma once
#include "LicenseBaseDoc.h"


class CLicense2Doc : virtual public CLicenseBaseDoc
{
protected: 
	CLicense2Doc();
public:
	virtual ~CLicense2Doc();
public:
	BOOL DeleteLicensed(LPCSTR szID);
	BOOL GetLicenseKeyValidate(LPCSTR lpKey);
	void Addlicensed();
};


