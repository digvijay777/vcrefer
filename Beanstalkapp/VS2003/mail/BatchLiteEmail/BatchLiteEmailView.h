// BatchLiteEmailView.h : CBatchLiteEmailView ��Ľӿ�
//


#pragma once


class CBatchLiteEmailView : public CHtmlView
{
protected: // �������л�����
	CBatchLiteEmailView();
	DECLARE_DYNCREATE(CBatchLiteEmailView)

// ����
public:
	CBatchLiteEmailDoc* GetDocument() const;

// ����
public:

// ��д
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CBatchLiteEmailView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
//	afx_msg void OnSendmail();
	afx_msg void OnSendmail();
};

#ifndef _DEBUG  // BatchLiteEmailView.cpp �ĵ��԰汾
inline CBatchLiteEmailDoc* CBatchLiteEmailView::GetDocument() const
   { return reinterpret_cast<CBatchLiteEmailDoc*>(m_pDocument); }
#endif

