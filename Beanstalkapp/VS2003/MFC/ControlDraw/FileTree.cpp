// FileTree.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "FileTree.h"
#include <shlwapi.h>

#ifndef STR_FOLDERTREE_CTRL
#define STR_DOCUMENT			_T("我的文档")
#define STR_COMPUTER			_T("我的电脑")
#define STR_MEMORY				_T("核心区域")
#define STR_VOLUME_UDISK		_T("可移动磁盘")
#define STR_VOLUME_FIXED		_T("本地磁盘")
#define STR_VOLUME_NETWORK		_T("网络驱动器")
#define STR_VOLUME_CDROM		_T("光盘驱动器")
#define STR_VOLUME_UNKNOWN		_T("未知设备")
#endif

#define TREE_ICON_CLOSEFOLDER			0		// 关闭的文件夹
#define TREE_ICON_OPENFOLDER			1		// 打开的文件夹
#define TREE_ICON_CDROM					2		// 光驱
#define TREE_ICON_FIXED					3		// 硬盘
#define TREE_ICON_FLOPPY				4		// 软驱
#define TREE_ICON_UDISK					5		// U盘
#define TREE_ICON_COMPUTER				6		// 我的电脑
#define TREE_ICON_MEMORY				7		// 内存
#define TREE_ICON_DOCUMENT				8		// 我的文档
#define TREE_ICON_NETDISK				5		// 网络磁盘
// CFileTree

IMPLEMENT_DYNAMIC(CFileTree, CFolderTree)
CFileTree::CFileTree()
{
}

CFileTree::~CFileTree()
{
}


BEGIN_MESSAGE_MAP(CFileTree, CFolderTree)
END_MESSAGE_MAP()

void CFileTree::InitFolder(UINT nState, UINT nICON)
{
	HTREEITEM		hItem			= NULL;

	DeleteAllItems();
	InitImageList(nState, nICON);

	SetRedraw(FALSE);
	m_hDocument = InsertItem(STR_DOCUMENT, TREE_ICON_DOCUMENT, TREE_ICON_DOCUMENT, NULL);
	SetItemData(m_hDocument, 1);
	m_hComputer = InsertItem(STR_COMPUTER, TREE_ICON_COMPUTER, TREE_ICON_COMPUTER, NULL);
	SetItemData(m_hComputer, 1);
	// 初始化我的电脑
	m_hMemory = InsertItem(STR_MEMORY, TREE_ICON_MEMORY, TREE_ICON_MEMORY, m_hComputer);
	SetItemData(m_hMemory, 1);

	DWORD		dwDrives		= GetLogicalDrives();
	TCHAR		szDriveName[]	= _T("\x20:\\");

	for(int i = 2; i < 32; i++)
	{
		if((0x1 << i) & dwDrives)
		{
			szDriveName[0] = _T('A') + i;
			AddDrive(szDriveName);
		}
	}
	SetRedraw(TRUE);
	Expand(m_hComputer, TVE_EXPAND);
}

void CFileTree::InitImageList(UINT nState /* = -1 */, UINT nICON /* = -1 */)
{
	if(-1 != nICON)
	{
		CBitmap		bmp;
		BITMAP		bit		= {0};

		if (NULL != m_imgIcon.GetSafeHandle())
		{
			m_imgIcon.DeleteImageList();
		}

		bmp.LoadBitmap(nICON);
		bmp.GetBitmap(&bit);

		m_imgIcon.Create(16, 16, ILC_COLOR24 | ILC_MASK, 0, 0);
		m_imgIcon.SetBkColor(RGB(0xff, 0xff, 0xff));
		ImageList_AddMasked(m_imgIcon, (HBITMAP)bmp.GetSafeHandle(), RGB(0xff, 0x0, 0xff));
		bmp.DeleteObject();
		SetImageList(&m_imgIcon, TVSIL_NORMAL); 
	}

	__super::InitImageList(nState, -1);
}

BOOL CFileTree::HasChildFolder(LPCTSTR lpFolderPath)
{
	if(!PathIsDirectory(lpFolderPath))
		return FALSE;

	WIN32_FIND_DATA			findData		= {0};
	HANDLE					hFind			= NULL;
	CString					strFind			= lpFolderPath;
	BOOL					bRet			= FALSE;

	if(strFind.GetAt(strFind.GetLength() - 1) != _T('\\'))
		strFind += _T("\\");
	strFind += "*";
	hFind = FindFirstFile(strFind.GetBuffer(), &findData);
	if(INVALID_HANDLE_VALUE == hFind)
		return FALSE;
	do 
	{
		if((_tcscmp(findData.cFileName, _T(".")) != 0)
			&& (_tcscmp(findData.cFileName, _T("..")) != 0))
		{
			bRet = TRUE;
			break;
		}
	} while (FindNextFile(hFind, &findData));

	FindClose(hFind);
	return bRet;
}

// 排序函数
int CString_qsort(const void *str1, const void *str2)
{
	TCHAR*	s1	= (TCHAR *)str1;
	TCHAR*	s2	= (TCHAR *)str2;

	return _tcsicmp(s1, s2);
}
typedef struct tagSTCHAR{ 
	TCHAR	szPath[MAX_PATH]; 
} STCHAR;
void CFileTree::OnExpandFolder(HTREEITEM hItem)
{
	if(NULL != GetItemData(hItem))
		return;

	// 删除空项
	SetItemData(hItem, 1);

	// 添加目录
	CString					strPath		= GetFolderPath(hItem);
	WIN32_FIND_DATA			findData	= {0};
	HANDLE					hFind		= NULL;
	std::vector< STCHAR >	vctFolder;
	std::vector< STCHAR >	vctFile;

	strPath += "*";
	hFind = FindFirstFile(strPath.GetBuffer(), &findData);
	if(INVALID_HANDLE_VALUE == hFind)
		return;
	do 
	{
		if((_tcscmp(findData.cFileName, _T(".")) != 0)
			&& (_tcscmp(findData.cFileName, _T("..")) != 0))
		{
			if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				vctFolder.push_back((STCHAR&)findData.cFileName);
			else
				vctFile.push_back((STCHAR&)findData.cFileName);
		}
	} while (FindNextFile(hFind, &findData));
	FindClose(hFind);

	// 排度
	qsort(&*vctFolder.begin(), vctFolder.size(), sizeof(STCHAR), CString_qsort);
	qsort(&*vctFile.begin(), vctFile.size(), sizeof(STCHAR), CString_qsort);
	// 更新
	SetRedraw(FALSE);
	vctFolder.insert(vctFolder.end(), vctFile.begin(), vctFile.end());
	// 文件就要获取其图标
	for(int i = 0; i < vctFolder.size(); i++)
	{
		HTREEITEM		hChildItem;

		hChildItem = AddFolder(vctFolder[i].szPath, hItem);

		int			nIcon	= TREE_ICON_CLOSEFOLDER;
		CString		strFile	= GetFolderPath(hItem);
		BOOL		bAdd;

		strFile += vctFolder[i].szPath;
		nIcon = m_imgIcon.Add(m_fileIcon.GetFileIcon(strFile.GetBuffer()), bAdd);
		SetItemImage(hChildItem, nIcon, nIcon);
	}

	SetRedraw(TRUE);
}
