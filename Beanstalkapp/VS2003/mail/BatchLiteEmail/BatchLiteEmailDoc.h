// BatchLiteEmailDoc.h :  CBatchLiteEmailDoc ��Ľӿ�
//


#pragma once

class CBatchLiteEmailDoc : public CDocument
{
protected: // �������л�����
	CBatchLiteEmailDoc();
	DECLARE_DYNCREATE(CBatchLiteEmailDoc)

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
	virtual ~CBatchLiteEmailDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnLinkmain();
};


