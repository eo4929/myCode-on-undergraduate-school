// EraserDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "20111688E6_1.h"
#include "EraserDialog.h"
#include "afxdialogex.h"
#include "20111688E6_1View.h"
#include "MainFrm.h"


// CEraserDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEraserDialog, CDialogEx)

CEraserDialog::CEraserDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEraserDialog::IDD, pParent)
	, m_editEraserSize(0)
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CMy20111688E6_1View* pView = (CMy20111688E6_1View*)pFrame->GetActiveView();
	m_editEraserSize = pView->m_nEraserSize;
}

CEraserDialog::~CEraserDialog()
{
}

void CEraserDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ERASER, m_editEraserSize);
	DDX_Control(pDX, IDC_SPIN_ERASER, m_spinEraserSize);
}


BEGIN_MESSAGE_MAP(CEraserDialog, CDialogEx)
END_MESSAGE_MAP()


// CEraserDialog 메시지 처리기입니다.


BOOL CEraserDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_spinEraserSize.SetRange(2,20); // 지우개 사이즈 2~20까지만
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
