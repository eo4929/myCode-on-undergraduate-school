// PenSizeDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "20111688E6_1.h"
#include "PenSizeDialog.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "20111688E6_1View.h"

// CPenSizeDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPenSizeDialog, CDialogEx)

CPenSizeDialog::CPenSizeDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPenSizeDialog::IDD, pParent)
	, m_editPenSize(0)
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CMy20111688E6_1View* pView = (CMy20111688E6_1View*)pFrame->GetActiveView();
	m_editPenSize = pView->m_nPenSize;
}

CPenSizeDialog::~CPenSizeDialog()
{
}

void CPenSizeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PEN, m_editPenSize);
	DDX_Control(pDX, IDC_SPIN_PEN, m_spinPenSize);
}


BEGIN_MESSAGE_MAP(CPenSizeDialog, CDialogEx)
END_MESSAGE_MAP()


// CPenSizeDialog 메시지 처리기입니다.


BOOL CPenSizeDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_spinPenSize.SetRange(1,10); // 펜 사이즈 1~10까지만

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
