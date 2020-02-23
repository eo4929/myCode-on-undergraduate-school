#pragma once
#include "afxcmn.h"


// CEraserDialog 대화 상자입니다.

class CEraserDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CEraserDialog)

public:
	CEraserDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEraserDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ERASER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	UINT m_editEraserSize; // 사이즈 EDIT 컨트롤 변수
	CSpinButtonCtrl m_spinEraserSize; // 스핀 컨트롤 변수
	virtual BOOL OnInitDialog();
};
