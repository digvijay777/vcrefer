#pragma once

typedef struct {
	WORD dlgVer;
	WORD signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
/*
	sz_Or_Ord menu;
	sz_Or_Ord windowClass;
	WCHAR title[titleLen];
	WORD pointsize;
	WORD weight;
	BYTE italic;
	BYTE charset;
	WCHAR typeface[stringLen];
*/
} DLGTEMPLATEEX;

class CDlgTemplate
{
private:
	BYTE				m_data[512];
	int					m_nPos;
public:
	DLGTEMPLATEEX		m_dlg;
	
public:
	CDlgTemplate(){
		ClearTemplate();
	}

	// 清空
	void ClearTemplate(){
		memset(&m_dlg, 0, sizeof(m_dlg));
		memset(m_data, 0, sizeof(m_data));
		m_dlg.dlgVer = 1;		// 必须是1
	}

	// 设置菜单

};