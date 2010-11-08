#include "StdAfx.h"
#include ".\tabledata.h"

CTableData::CTableData(void)
{
	m_nColumnCount = 0;
}

CTableData::~CTableData(void)
{
}

// 添加列
void CTableData::SetColumnCount(int nColumn /* = 0 */)
{
	m_nColumnCount = nColumn;
}

// 添加行
int CTableData::InsertItem(int nItem, LPCSTR lpText)
{
	if(nItem < 0)
		return -1;
	if(nItem > (int)m_vctData.size())
		nItem = (int)m_vctData.size() - 1;

	vector<TABLEDATA>::iterator		itData		= m_vctData.begin() + nItem;
	TABLEDATA						tb;

	itData = m_vctData.insert(itData, tb);
	itData->vRow[0] = lpText;

	return nItem;
}

// 添加子项
int CTableData::InsertChildItem(int nItem, int nChildItem, LPCSTR lpText)
{
	if((nItem < 0) || (nItem >= (int)m_vctData.size()) || (nChildItem < 0))
		return -1;
	
	if(nChildItem > (int )m_vctData/*[nItem]*/.size())
		nChildItem = (int)m_vctData.size() - 1;

	vector< vector<string> >::iterator	itChild	= m_vctData[nItem].child.begin() + nChildItem;
	
	// itChild = m_vctData[nItem].child.insert();

	return nChildItem;
}