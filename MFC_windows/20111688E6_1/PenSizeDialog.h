#pragma once
#include "afxcmn.h"


// CPenSizeDialog 대화 상자입니다.

class CPenSizeDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPenSizeDialog)

public:
	CPenSizeDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPenSizeDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	UINT m_editPenSize; // 사이즈 컨트롤 변수
	CSpinButtonCtrl m_spinPenSize; // 스핀 컨트롤 변수
	virtual BOOL OnInitDialog();
};
