// DetoursView.h : CDetoursView ��Ľӿ�
//


#pragma once


class CDetoursView : public CHtmlView
{
protected: // �������л�����
	CDetoursView();
	DECLARE_DYNCREATE(CDetoursView)

// ����
public:
	CDetoursDoc* GetDocument() const;

// ����
public:

// ��д
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CDetoursView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DetoursView.cpp �ĵ��԰汾
inline CDetoursDoc* CDetoursView::GetDocument() const
   { return reinterpret_cast<CDetoursDoc*>(m_pDocument); }
#endif

