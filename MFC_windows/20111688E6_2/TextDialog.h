#pragma once
#include "afxwin.h"


// CTextDialog 대화 상자입니다.

class CTextDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTextDialog)

public:
	CTextDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTextDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TEXT };

public:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strColorOut;
	CString m_strFontOut;
	afx_msg void OnClickedButtonTextout();
	CEdit m_strTextString;
	afx_msg void OnClickedButtonColor();
	afx_msg void OnClickedButtonFont();
	
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEnChangeEditX();
	afx_msg void OnEnChangeEditY();
	CString m_strEditX;
	CString m_strEditY;
};
