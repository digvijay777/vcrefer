#pragma once

class CListHead : public CHeaderCtrl
{
public:
	CListHead();
	virtual ~CListHead();

protected:
	afx_msg void OnPaint();
	afx_msg LRESULT OnLayout( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()

private:
};