// ImgToolBarDoc.h :  CImgToolBarDoc ��Ľӿ�
//


#pragma once

class CImgToolBarDoc : public CDocument
{
protected: // �������л�����
	CImgToolBarDoc();
	DECLARE_DYNCREATE(CImgToolBarDoc)

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
	virtual ~CImgToolBarDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


