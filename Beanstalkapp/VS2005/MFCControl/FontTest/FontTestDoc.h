// FontTestDoc.h : CFontTestDoc ��Ľӿ�
//


#pragma once


class CFontTestDoc : public CDocument
{
protected: // �������л�����
	CFontTestDoc();
	DECLARE_DYNCREATE(CFontTestDoc)

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
	virtual ~CFontTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


