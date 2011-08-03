// EHomeListsView.h : CEHomeListsView 类的接口
//


#pragma once
#include "afxcmn.h"
#include "EHomeList/EHomeList.h"

class CEHomeListsView : public CFormView
{
protected: // 仅从序列化创建
	CEHomeListsView();
	DECLARE_DYNCREATE(CEHomeListsView)

public:
	enum{ IDD = IDD_EHOMELISTS_FORM };

// 属性
public:
	CEHomeListsDoc* GetDocument() const;

// 操作
public:
	CImageList			m_imagelist;
	CImageList			m_imagelist2;
	CImageList			m_imagelist3;
	CImageList			m_imagelist4;
	CImageList			m_imagelistscore;
	CImageList			m_imagemuiltbutton;
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用
	
// 实现
public:
	virtual ~CEHomeListsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // EHomeListsView.cpp 中的调试版本
inline CEHomeListsDoc* CEHomeListsView::GetDocument() const
   { return reinterpret_cast<CEHomeListsDoc*>(m_pDocument); }
#endif

