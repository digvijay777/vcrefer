// EHomeListsView.h : CEHomeListsView ��Ľӿ�
//


#pragma once
#include "afxcmn.h"
#include "EHomeList/EHomeList.h"

class CEHomeListsView : public CFormView
{
protected: // �������л�����
	CEHomeListsView();
	DECLARE_DYNCREATE(CEHomeListsView)

public:
	enum{ IDD = IDD_EHOMELISTS_FORM };

// ����
public:
	CEHomeListsDoc* GetDocument() const;

// ����
public:
	CImageList			m_imagelist;
	CImageList			m_imagelist2;
	CImageList			m_imagelist3;
	CImageList			m_imagelist4;
	CImageList			m_imagelistscore;
	CImageList			m_imagemuiltbutton;
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���
	
// ʵ��
public:
	virtual ~CEHomeListsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnUmEHomeListClickItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmEHomeListClickMuiltButton(WPARAM wParam, LPARAM lParam);
public:
	CEHomeList	 m_listsoft;
	CEHomeList	 m_listurl;
	CEHomeList	 m_listtime;
	CEHomeList	 m_listtype;
	CEHomeList	 m_listsoftuse;
};

#ifndef _DEBUG  // EHomeListsView.cpp �еĵ��԰汾
inline CEHomeListsDoc* CEHomeListsView::GetDocument() const
   { return reinterpret_cast<CEHomeListsDoc*>(m_pDocument); }
#endif

