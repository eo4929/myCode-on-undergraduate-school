#pragma once


// CDeleteDialog 대화 상자입니다.

class CDeleteDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDeleteDialog)

public:
	CDeleteDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDeleteDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DELETE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
