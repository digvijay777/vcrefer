// StudyGDI+Doc.h :  CStudyGDIDoc ��Ľӿ�
//


#pragma once

class CStudyGDIDoc : public CDocument
{
protected: // �������л�����
	CStudyGDIDoc();
	DECLARE_DYNCREATE(CStudyGDIDoc)

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
	virtual ~CStudyGDIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


