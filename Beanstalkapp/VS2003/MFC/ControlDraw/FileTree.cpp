// FileTree.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "FileTree.h"
#include <shlwapi.h>

#ifndef STR_FOLDERTREE_CTRL
#define STR_DOCUMENT			_T("�ҵ��ĵ�")
#define STR_COMPUTER			_T("�ҵĵ���")
#define STR_MEMORY				_T("��������")
#define STR_VOLUME_UDISK		_T("���ƶ�����")
#define STR_VOLUME_FIXED		_T("���ش���")
#define STR_VOLUME_NETWORK		_T("����������")
#define STR_VOLUME_CDROM		_T("����������")
#define STR_VOLUME_UNKNOWN		_T("δ֪�豸")
#endif

#define TREE_ICON_CLOSEFOLDER			0		// �رյ��ļ���
#define TREE_ICON_OPENFOLDER			1		// �򿪵��ļ���
#define TREE_ICON_CDROM					2		// ����
#define TREE_ICON_FIXED					3		// Ӳ��
#define TREE_ICON_FLOPPY				4		// ����
#define TREE_ICON_UDISK					5		// U��
#define TREE_ICON_COMPUTER				6		// �ҵĵ���
#define TREE_ICON_MEMORY				7		// �ڴ�
#define TREE_ICON_DOCUMENT				8		// �ҵ��ĵ�
#define TREE_ICON_NETDISK				5		// �������
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
	// ��ʼ���ҵĵ���
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

// ������
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

	// ɾ������
	SetItemData(hItem, 1);

	// ���Ŀ¼
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

	// �Ŷ�
	qsort(&*vctFolder.begin(), vctFolder.size(), sizeof(STCHAR), CString_qsort);
	qsort(&*vctFile.begin(), vctFile.size(), sizeof(STCHAR), CString_qsort);
	// ����
	SetRedraw(FALSE);
	vctFolder.insert(vctFolder.end(), vctFile.begin(), vctFile.end());
	// �ļ���Ҫ��ȡ��ͼ��
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
