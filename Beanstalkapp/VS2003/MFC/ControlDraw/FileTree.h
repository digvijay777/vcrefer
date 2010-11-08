#pragma once
#include "SkyTreeCtrl.h"
#include "common/NMfc.h"
#include "common/FileInfo.h"

// CFileTree

class CFileTree : public CFolderTree
{
	DECLARE_DYNAMIC(CFileTree)

public:
	CFileTree();
	virtual ~CFileTree();

private:
	CIconList			m_imgIcon;
	CFileIconCache		m_fileIcon;

public:
	void		InitFolder(UINT nState, UINT nICON);
	void		InitImageList(UINT nState = -1, UINT nICON = -1);

	virtual BOOL		HasChildFolder(LPCTSTR lpFolderPath);
	virtual void		OnExpandFolder(HTREEITEM hItem);
protected:
	DECLARE_MESSAGE_MAP()
};


