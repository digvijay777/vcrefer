// SplitterTestDoc.h :  CSplitterTestDoc ��Ľӿ�
//


#pragma once

class CSplitterTestDoc : public CDocument
{
protected: // �������л�����
	CSplitterTestDoc();
	DECLARE_DYNCREATE(CSplitterTestDoc)

// ����
public:

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CSplitterTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


