#pragma once
#include "textdialog.h"


// CTextDokingPane

class CTextDokingPane : public CDockablePane
{
	DECLARE_DYNAMIC(CTextDokingPane)

public:
	CTextDokingPane();
	virtual ~CTextDokingPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CTextDialog m_textDialog;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


