// MathTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>

#define PI						(3.1415926)
#define AngleToRadian(x)		(x)*PI/180.0		// �Ƕȵ�����
#define RadianToAngle(x)		(x)*180.0/PI		// ���ȵ��Ƕ�

int _tmain(int argc, _TCHAR* argv[])
{
	int		f;
	
	f = int(RadianToAngle(atan(29.0/133.0))+360) % 360;
	return 0;
}

