// classFun.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

template <typename T>
class TDestructure
{
public:
	TDestructure(T* pT){
		m_pClass = pT;
	}
	~TDestructure(){
		
	}
public:
	T*		m_pClass;
};

class TestA
{
public:
	int		nPrint;
public:
	void Print(){cout<<"This is TestA's Print"<<endl;};
};

void TestFun(TestA* tt)
{
	TDestructure<TestA>	a(tt);

	if(tt->nPrint == 1)
	{
		cout<<"TestFun case 1"<<endl;
		return;
	}

	cout<<"TestFun case 2"<<endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestA		tt;
	tt.nPrint = 1;
	TestFun(&tt);
	return 0;
}

