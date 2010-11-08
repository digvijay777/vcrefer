// SkyTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SkyTreeCtrl.h"
#include ".\skytreectrl.h"

#define MAXPATH					MAX_PATH*2

/************************************************************************/
/*                                                                      */
/*                        CSkyTreeCtrl                                   */
/*                                                                      */
/************************************************************************/

// CSkyTreeCtrl

IMPLEMENT_DYNAMIC(CSkyTreeCtrl, CTreeCtrl)
CSkyTreeCtrl::CSkyTreeCtrl()
{
}

CSkyTreeCtrl::~CSkyTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkyTreeCtrl, CTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void CSkyTreeCtrl::PreSubclassWindow()
{
	CTreeCtrl::PreSubclassWindow();

	DWORD		dwStyleAdd			= TVS_CHECKBOXES | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	DWORD		dwStyleDel			= TVS_EDITLABELS | TVS_FULLROWSELECT | TVS_DISABLEDRAGDROP | TVS_SHOWSELALWAYS ;

	ModifyStyle(dwStyleDel, dwStyleAdd, 0);
}

// state图片四个状态 无,没选,全选,判选
void CSkyTreeCtrl::InitImageList(UINT nState /* = -1 */, UINT nICON /* = -1 */)
{
	CBitmap		bmp;
	BITMAP		bit;

	if(-1 != nICON)
	{
		if(NULL != m_imgICON.GetSafeHandle())
		{
			m_imgICON.DeleteImageList();
		}

		bmp.LoadBitmap(nICON);
		bmp.GetBitmap(&bit);

		m_imgICON.Create(bit.bmHeight, bit.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
		m_imgICON.Add(&bmp, RGB(0xff, 0x0, 0xff));
		bmp.DeleteObject();
		SetImageList(&m_imgICON, TVSIL_NORMAL);
	}

	if(-1 != nState)
	{
		if (NULL != m_imgState.GetSafeHandle())
		{
			m_imgState.DeleteImageList();
		}

		bmp.LoadBitmap(nState);
		bmp.GetBitmap(&bit);

		m_imgState.Create(bit.bmWidth / 4, bit.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
		m_imgState.Add(&bmp, RGB(0xff, 0x0, 0xff));
		bmp.DeleteObject();
		SetImageList(&m_imgState, TVSIL_STATE); // 注意: 要在TVSIL_NORMAL之后才有效
	}
}

void CSkyTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	HTREEITEM		hItem		= HitTest(point, &nFlags);

	if(NULL == hItem)
		return;
	if(TVHT_ONITEMSTATEICON & nFlags)
	{
		SelectItem(hItem);
		ReverseItemState(hItem);
		return;
	}
	else if(TVHT_ONITEMBUTTON & nFlags)
	{
		SelectItem(hItem);
		Expand(hItem, TVE_TOGGLE);
		return;
	}

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CSkyTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	HTREEITEM		hItem		= HitTest(point, &nFlags);

	if(NULL == hItem)
		return;
	if(TVHT_ONITEMLABEL & nFlags)
	{
		SelectItem(hItem);
		Expand(hItem, TVE_TOGGLE);
		return;
	}

	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}

void CSkyTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if((VK_SPACE == nChar)
		&& (0 == (0xf0 & GetKeyState(VK_CAPITAL))))
	{
		HTREEITEM		hItem	= GetSelectedItem();

		ReverseItemState(hItem);
		return;
	}
	else if(VK_RIGHT == nChar)
	{
		HTREEITEM		hItem	= GetSelectedItem();

		Expand(hItem, TVE_EXPAND);
		return;
	}
	else if(VK_MULTIPLY == nChar)
	{
		return;
	}

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSkyTreeCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 这里没有返回会出现嘟的一声
	if((VK_SPACE == nChar)
		&& (0 == (0xf0 & GetKeyState(VK_CAPITAL))))
	{
		return;
	}

	CTreeCtrl::OnChar(nChar, nRepCnt, nFlags);
}
// 返转一项的状态
void CSkyTreeCtrl::ReverseItemState(HTREEITEM hItem)
{
	if(NULL == hItem)
		return;

	int		nStateIndex		= GetItemStateIndex(hItem);

	if(ITEM_CHECKED_DISABLED == nStateIndex)
		return;	// 禁用时返回

	if(ITEM_CHECKED_EMPTY == nStateIndex || ITEM_CHECKED_HALF == nStateIndex)
	{
		CheckItem(hItem, TRUE);
	}
	else
	{
		CheckItem(hItem, FALSE);
	}
}
// 设置某一项的选择状态
void CSkyTreeCtrl::CheckItem(HTREEITEM hItem, BOOL bChecked /* = TRUE */)
{
	int			nIndex		= (FALSE == bChecked)?1:2;

	// 设置状态
	SetItemStateIndex(hItem, nIndex);
	// 更新父窗体
	UpdateItemParentIndex(hItem);
	// 更新子窗体
	SetItemChildStateIndex(hItem, nIndex);
}
void CSkyTreeCtrl::CheckAll(BOOL bChecked)
{
	HTREEITEM		hItem		= GetRootItem();

	for(; NULL != hItem; hItem = GetNextItem(hItem, TVGN_NEXT))
		CheckItem(hItem, bChecked);
}
// 插入项
HTREEITEM CSkyTreeCtrl::InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage, HTREEITEM hParent)
{
	HTREEITEM			hTree		= CTreeCtrl::InsertItem(lpszItem, nImage, nSelectedImage, hParent);
	int					nIndex		= GetItemStateIndex(hParent);

	if(ITEM_CHECKED_CHEKED == nIndex)
		SetItemStateIndex(hTree, nIndex);
	
	return hTree;
}

// 更新父节点
void CSkyTreeCtrl::UpdateItemParentIndex(HTREEITEM hItem)
{
	HTREEITEM		hParent			= GetParentItem(hItem);

	if(NULL == hParent)
		return; // 没有父节点
	
	int				nIndex			= GetItemStateIndex(hParent);
	int				nOldIndex		= 0;

	if(0 == nIndex)
		return;	// 是终点状态就不用操作了

    for(HTREEITEM hChild = GetChildItem(hParent)
		; NULL != hChild
		; hChild = GetNextItem(hChild, TVGN_NEXT))
	{
		int			nChild		= GetItemStateIndex(hChild);

		// 只要有一个是半选就半选
		if(ITEM_CHECKED_HALF == nChild)
		{
			nIndex = ITEM_CHECKED_HALF;
			break;
		}
		if(0 == nOldIndex)
			nOldIndex = nChild;
		// 只要有两个子项不相等就是半选
		if(nOldIndex != nChild)	
		{
			nIndex = ITEM_CHECKED_HALF;
			break;
		}
	}
	if(NULL == hChild)
		nIndex = nOldIndex;

	SetItemStateIndex(hParent, nIndex);
	// 递归父项
	UpdateItemParentIndex(hParent);
}

// 设置子节点
void CSkyTreeCtrl::SetItemChildStateIndex(HTREEITEM hItem, int nIndex)
{
	for(HTREEITEM hChild = GetChildItem(hItem)
		; NULL != hChild
		; hChild = GetNextItem(hChild, TVGN_NEXT))
	{
		SetItemStateIndex(hChild, nIndex);
		// 递归子项
		SetItemChildStateIndex(hChild, nIndex);
	}
}

// 设置一项的状态
void CSkyTreeCtrl::SetItemStateIndex(HTREEITEM hItem, int nIndex)
{
	if(NULL == hItem)
		return;

	int		nState		= GetItemState(hItem, TVIS_STATEIMAGEMASK);

	nState &= ~TVIS_STATEIMAGEMASK;
	nState |= nIndex << 12;

	SetItemState(hItem, nState, TVIS_STATEIMAGEMASK);
}

// 得到一项的状态 
int CSkyTreeCtrl::GetItemStateIndex(HTREEITEM hItem)
{
	if(NULL == hItem)
		return 0;

	int		nState		= GetItemState(hItem, TVIS_STATEIMAGEMASK);

	nState &= TVIS_STATEIMAGEMASK;
	nState >>= 12;

	return nState;
}


/************************************************************************/
/*                                                                      */
/*                        CFolderTree                                   */
/*                                                                      */
/************************************************************************/

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

IMPLEMENT_DYNAMIC(CFolderTree, CSkyTreeCtrl)
CFolderTree::CFolderTree()
{
	m_hComputer = NULL;
	m_hDocument = NULL;
	m_hMemory = NULL;
}

CFolderTree::~CFolderTree()
{
	
}

LRESULT CFolderTree::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(TVM_EXPAND == message)
	{
		OnExpandFolder((HTREEITEM)lParam);
	}

	return CSkyTreeCtrl::WindowProc(message, wParam, lParam);
}
// 展开目录
void CFolderTree::OnExpandFolder(HTREEITEM hItem)
{
	if(NULL != GetItemData(hItem))
		return;

	// 删除空项
	SetItemData(hItem, 1);

	// 添加目录
	CString					strPath		= GetFolderPath(hItem);
	WIN32_FIND_DATA			findData	= {0};
	HANDLE					hFind		= NULL;

	strPath += "*";
	hFind = FindFirstFile(strPath.GetBuffer(), &findData);
	if(INVALID_HANDLE_VALUE == hFind)
		return;
	SetRedraw(FALSE);
	do 
	{
		if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			&& (_tcscmp(findData.cFileName, _T(".")) != 0)
			&& (_tcscmp(findData.cFileName, _T("..")) != 0))
		{
			AddFolder(findData.cFileName, hItem);
		}
	} while (FindNextFile(hFind, &findData));
	FindClose(hFind);
	SetRedraw(TRUE);
}

/*
 *	初始化列表
 *
 *  nState:  1 未选;2 全选;3 半选 
 *  nICON:	1 关闭文件夹;2 打开文件夹;3 光驱;4 硬盘;5 软件盘;6 U盘;7 我的电脑;8 内存;9 我的文档
 */
void CFolderTree::InitFolder(UINT nState, UINT nICON)
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
//选择项
// void CFolderTree::CheckAll(BOOL bChecked /* = TRUE */)
// {
// 	CheckItem(m_hDocument, bChecked);
// 	CheckItem(m_hComputer, bChecked);
// }
void CFolderTree::CheckMemory(BOOL bChecked /* = TRUE */)
{
	CheckItem(m_hMemory, bChecked);
}
void CFolderTree::CheckMyDocument(BOOL bChecked /* = TRUE */)
{
	CheckItem(m_hDocument, bChecked);
}
void CFolderTree::CheckComputer(BOOL bChecked /* = TRUE */)
{
	CheckItem(m_hComputer, bChecked);
}
BOOL CFolderTree::IsSelectMemory()
{
	if(2 == GetItemStateIndex(m_hMemory))
		return TRUE;

	return FALSE;
}
// 得到选择目录
int CFolderTree::GetSelectFolder(std::vector<String>& vctSel)
{
	vctSel.clear();
	TCHAR		szPath[MAXPATH]		= {0};
	// 我的文档
	if(ITEM_CHECKED_CHEKED == GetItemStateIndex(m_hDocument))
	{
		memset(szPath, 0, sizeof(szPath));
		SHGetSpecialFolderPath(NULL, szPath, CSIDL_PERSONAL, FALSE);
		// _tcscat(szPath, _T("\\"));
		vctSel.push_back(szPath);
	}
	// 我的电脑
	if(ITEM_CHECKED_EMPTY == GetItemStateIndex(m_hComputer))
		return vctSel.size();	// 没有选择
	
	GetFullCheckedFolder(vctSel, m_hComputer);
	return vctSel.size();
}
// 设置加号
void CFolderTree::SetHasChild(HTREEITEM hItem)
{
	if(NULL == hItem)
		return;

	TVITEM		tvItem;

	memset(&tvItem, 0, sizeof(tvItem));
	tvItem.mask = TVIF_CHILDREN;
	tvItem.cChildren = 1;
	tvItem.hItem = hItem;
	SetItem(&tvItem);
}
// 得到全选目录
void CFolderTree::GetFullCheckedFolder(std::vector<String>& vctSel, HTREEITEM hParent)
{
	if(ITEM_CHECKED_EMPTY == GetItemStateIndex(hParent))
		return;

	HTREEITEM			hChild		= NULL;

	for(hChild = GetChildItem(hParent)
		; NULL != hChild
		; hChild = GetNextItem(hChild, TVGN_NEXT))
	{
		if(m_hMemory == hChild)
			continue;

		if(ITEM_CHECKED_HALF == GetItemStateIndex(hChild))
		{
			GetFullCheckedFolder(vctSel, hChild);
		}
		else if(ITEM_CHECKED_CHEKED == GetItemStateIndex(hChild))
		{
			CString		strChild	= GetFolderPath(hChild);

			strChild.ReleaseBuffer(strChild.GetLength() - 1);
			vctSel.push_back(strChild.GetBuffer());
		}
	}
}
// 添加磁盘
void CFolderTree::AddDrive(LPCTSTR lpDrive)
{
	TCHAR newDrive[100] = {0}, volumeName[100] = {0};

	GetVolumeName(lpDrive, volumeName);
	_stprintf(newDrive, _T("%c:[%s]"), lpDrive[0], volumeName);

	HTREEITEM		hItem		= InsertItem(newDrive, GetDriveImage(lpDrive), GetDriveImage(lpDrive), m_hComputer);
	
	if(HasChildFolder(lpDrive))
	{
		SetHasChild(hItem);
	}
	else
	{
		SetItemData(hItem, 1);
	}
}
// 添加目录
HTREEITEM CFolderTree::AddFolder(LPCTSTR lpFolder, HTREEITEM hParent)
{
	HTREEITEM		hItem		= InsertItem(lpFolder, TREE_ICON_CLOSEFOLDER, TREE_ICON_CLOSEFOLDER, hParent);

	if(HasChildFolder(GetFolderPath(hParent) + lpFolder))
	{
		SetHasChild(hItem);
	}
	else
	{
		SetItemData(hItem, 1);
	}

	return hItem;
}

// 得到磁盘驱动器的图标
int CFolderTree::GetDriveImage(LPCTSTR lpDrive)
{
	int			nRet		= TREE_ICON_FIXED;
	UINT		nType		= GetDriveType(lpDrive);

	switch(nType)
	{
	case DRIVE_REMOVABLE:
		nRet = TREE_ICON_UDISK;
		break;
	case DRIVE_FIXED:
		nRet = TREE_ICON_FIXED;
		break;
	case DRIVE_REMOTE:
		nRet = TREE_ICON_NETDISK;
		break;
	case DRIVE_CDROM:
		nRet = TREE_ICON_CDROM;
		break;
	default:
		break;
	}

	return nRet;
}
// 得到卷标
int CFolderTree::GetVolumeName(LPCTSTR lpDrive, LPTSTR lpVolumeName)
{
	int		nRet = GetVolumeInformation(lpDrive, lpVolumeName, MAXPATH, NULL, NULL, NULL, NULL, 0);

	if(_tcslen(lpVolumeName) > 0)
		return nRet;

	UINT	nType = GetDriveType(lpDrive);
	switch (nType)
	{
	case DRIVE_REMOVABLE:
		_tcscpy(lpVolumeName, STR_VOLUME_UDISK);
		break;
	case DRIVE_FIXED:
		_tcscpy(lpVolumeName, STR_VOLUME_FIXED);
		break;
	case DRIVE_REMOTE:
		_tcscpy(lpVolumeName, STR_VOLUME_NETWORK);
		break;
	case DRIVE_CDROM:
		_tcscpy(lpVolumeName, STR_VOLUME_CDROM);
		break;
	default:
		_tcscpy(lpVolumeName, STR_VOLUME_UNKNOWN);
		break;
	}

	return  _tcslen(lpVolumeName);
}

// 是否有子节点 
BOOL CFolderTree::HasChildFolder(LPCTSTR lpFolderPath)
{
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
		if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
			&& (_tcscmp(findData.cFileName, _T(".")) != 0)
			&& (_tcscmp(findData.cFileName, _T("..")) != 0))
		{
			bRet = TRUE;
			break;
		}
	} while (FindNextFile(hFind, &findData));

	FindClose(hFind);
	return bRet;
}

// 得到路径
CString CFolderTree::GetFolderPath(HTREEITEM hItem)
{
	CString			strItem;
	CString			strPath;
	HTREEITEM		hParent;
	BOOL			bFixed		= FALSE;

	if(m_hComputer == hItem || m_hDocument == hItem || m_hMemory == hItem)
		return _T("");

	strItem = GetItemText(hItem);
	if(strItem.Find(_T(':')) > 0)	// 是否为磁盘驱动器
	{
		strPath = strItem.Left(1) + _T(":\\");
		return strPath;
	}

	strPath = strItem + _T("\\");
	// 得到父路径
	for(hParent = GetParentItem(hItem)
		; NULL != hItem && m_hComputer != hParent
		; hParent = GetParentItem(hParent))
	{
		strItem = GetItemText(hParent);
		if(strItem.Find(_T(':')) > 0)
		{
			bFixed = TRUE;
			strItem = strItem.Left(1) + _T(":");
		}
		strPath = strItem + _T("\\") + strPath;

		if(FALSE != bFixed)
			break;
	}

	return strPath;
}
