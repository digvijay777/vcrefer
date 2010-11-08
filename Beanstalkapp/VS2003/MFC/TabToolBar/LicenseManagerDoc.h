#pragma once
#include "License2Doc.h"
#include "LicenseInfoDoc.h"
#include "LicenseInvalidateDoc.h"
// CLicenseManagerDoc document

class CLicenseManagerDoc : public CDocument, 
	public CLicense2Doc, public CLicenseInfoDoc, public CLicenseInvalidateDoc
{
	DECLARE_DYNCREATE(CLicenseManagerDoc)

public:
	CLicenseManagerDoc();
	virtual ~CLicenseManagerDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
private:
	virtual VOID NotifyView(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0); 
public:
	afx_msg void OnRefurbish();
};
