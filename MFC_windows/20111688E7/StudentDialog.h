#pragma once
#include "afxwin.h"


// CStudentDialog 대화 상자입니다.

class CStudentDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CStudentDialog)

public:
	CStudentDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStudentDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_STUDENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNumber; // 학번
	CString m_strName; // 이름
	int m_intLevel; // 학년
	int m_intSex; // 성별
	CString m_strAddress; // 주소
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboLevel();
	CComboBox m_cbLevel; // 학년
	CComboBox m_cbSex; // 성별
	afx_msg void OnCbnSelchangeComboSex();
};
