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

// stateͼƬ�ĸ�״̬ ��,ûѡ,ȫѡ,��ѡ
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
		SetImageList(&m_imgState, TVSIL_STATE); // ע��: Ҫ��TVSIL_NORMAL֮�����Ч
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
	// ����û�з��ػ����ཱུ�һ��
	if((VK_SPACE == nChar)
		&& (0 == (0xf0 & GetKeyState(VK_CAPITAL))))
	{
		return;
	}

	CTreeCtrl::OnChar(nChar, nRepCnt, nFlags);
}
// ��תһ���״̬
void CSkyTreeCtrl::ReverseItemState(HTREEITEM hItem)
{
	if(NULL == hItem)
		return;

	int		nStateIndex		= GetItemStateIndex(hItem);

	if(ITEM_CHECKED_DISABLED == nStateIndex)
		return;	// ����ʱ����

	if(ITEM_CHECKED_EMPTY == nStateIndex || ITEM_CHECKED_HALF == nStateIndex)
	{
		CheckItem(hItem, TRUE);
	}
	else
	{
		CheckItem(hItem, FALSE);
	}
}
// ����ĳһ���ѡ��״̬
void CSkyTreeCtrl::CheckItem(HTREEITEM hItem, BOOL bChecked /* = TRUE */)
{
	int			nIndex		= (FALSE == bChecked)?1:2;

	// ����״̬
	SetItemStateIndex(hItem, nIndex);
	// ���¸�����
	UpdateItemParentIndex(hItem);
	// �����Ӵ���
	SetItemChildStateIndex(hItem, nIndex);
}
void CSkyTreeCtrl::CheckAll(BOOL bChecked)
{
	HTREEITEM		hItem		= GetRootItem();

	for(; NULL != hItem; hItem = GetNextItem(hItem, TVGN_NEXT))
		CheckItem(hItem, bChecked);
}
// ������
HTREEITEM CSkyTreeCtrl::InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage, HTREEITEM hParent)
{
	HTREEITEM			hTree		= CTreeCtrl::InsertItem(lpszItem, nImage, nSelectedImage, hParent);
	int					nIndex		= GetItemStateIndex(hParent);

	if(ITEM_CHECKED_CHEKED == nIndex)
		SetItemStateIndex(hTree, nIndex);
	
	return hTree;
}

// ���¸��ڵ�
void CSkyTreeCtrl::UpdateItemParentIndex(HTREEITEM hItem)
{
	HTREEITEM		hParent			= GetParentItem(hItem);

	if(NULL == hParent)
		return; // û�и��ڵ�
	
	int				nIndex			= GetItemStateIndex(hParent);
	int				nOldIndex		= 0;

	if(0 == nIndex)
		return;	// ���յ�״̬�Ͳ��ò�����

    for(HTREEITEM hChild = GetChildItem(hParent)
		; NULL != hChild
		; hChild = GetNextItem(hChild, TVGN_NEXT))
	{
		int			nChild		= GetItemStateIndex(hChild);

		// ֻҪ��һ���ǰ�ѡ�Ͱ�ѡ
		if(ITEM_CHECKED_HALF == nChild)
		{
			nIndex = ITEM_CHECKED_HALF;
			break;
		}
		if(0 == nOldIndex)
			nOldIndex = nChild;
		// ֻҪ�����������Ⱦ��ǰ�ѡ
		if(nOldIndex != nChild)	
		{
			nIndex = ITEM_CHECKED_HALF;
			break;
		}
	}
	if(NULL == hChild)
		nIndex = nOldIndex;

	SetItemStateIndex(hParent, nIndex);
	// �ݹ鸸��
	UpdateItemParentIndex(hParent);
}

// �����ӽڵ�
void CSkyTreeCtrl::SetItemChildStateIndex(HTREEITEM hItem, int nIndex)
{
	for(HTREEITEM hChild = GetChildItem(hItem)
		; NULL != hChild
		; hChild = GetNextItem(hChild, TVGN_NEXT))
	{
		SetItemStateIndex(hChild, nIndex);
		// �ݹ�����
		SetItemChildStateIndex(hChild, nIndex);
	}
}

// ����һ���״̬
void CSkyTreeCtrl::SetItemStateIndex(HTREEITEM hItem, int nIndex)
{
	if(NULL == hItem)
		return;

	int		nState		= GetItemState(hItem, TVIS_STATEIMAGEMASK);

	nState &= ~TVIS_STATEIMAGEMASK;
	nState |= nIndex << 12;

	SetItemState(hItem, nState, TVIS_STATEIMAGEMASK);
}

// �õ�һ���״̬ 
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
// չ��Ŀ¼
void CFolderTree::OnExpandFolder(HTREEITEM hItem)
{
	if(NULL != GetItemData(hItem))
		return;

	// ɾ������
	SetItemData(hItem, 1);

	// ���Ŀ¼
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
 *	��ʼ���б�
 *
 *  nState:  1 δѡ;2 ȫѡ;3 ��ѡ 
 *  nICON:	1 �ر��ļ���;2 ���ļ���;3 ����;4 Ӳ��;5 �����;6 U��;7 �ҵĵ���;8 �ڴ�;9 �ҵ��ĵ�
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
//ѡ����
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
// �õ�ѡ��Ŀ¼
int CFolderTree::GetSelectFolder(std::vector<String>& vctSel)
{
	vctSel.clear();
	TCHAR		szPath[MAXPATH]		= {0};
	// �ҵ��ĵ�
	if(ITEM_CHECKED_CHEKED == GetItemStateIndex(m_hDocument))
	{
		memset(szPath, 0, sizeof(szPath));
		SHGetSpecialFolderPath(NULL, szPath, CSIDL_PERSONAL, FALSE);
		// _tcscat(szPath, _T("\\"));
		vctSel.push_back(szPath);
	}
	// �ҵĵ���
	if(ITEM_CHECKED_EMPTY == GetItemStateIndex(m_hComputer))
		return vctSel.size();	// û��ѡ��
	
	GetFullCheckedFolder(vctSel, m_hComputer);
	return vctSel.size();
}
// ���üӺ�
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
// �õ�ȫѡĿ¼
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
// ��Ӵ���
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
// ���Ŀ¼
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

// �õ�������������ͼ��
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
// �õ����
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

// �Ƿ����ӽڵ� 
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

// �õ�·��
CString CFolderTree::GetFolderPath(HTREEITEM hItem)
{
	CString			strItem;
	CString			strPath;
	HTREEITEM		hParent;
	BOOL			bFixed		= FALSE;

	if(m_hComputer == hItem || m_hDocument == hItem || m_hMemory == hItem)
		return _T("");

	strItem = GetItemText(hItem);
	if(strItem.Find(_T(':')) > 0)	// �Ƿ�Ϊ����������
	{
		strPath = strItem.Left(1) + _T(":\\");
		return strPath;
	}

	strPath = strItem + _T("\\");
	// �õ���·��
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
