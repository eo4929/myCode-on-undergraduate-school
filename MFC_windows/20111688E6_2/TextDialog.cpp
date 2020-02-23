// TextDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "20111688E6_2.h"
#include "TextDialog.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "20111688E6_2View.h"

// CTextDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTextDialog, CDialogEx)

CTextDialog::CTextDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTextDialog::IDD, pParent)
	, m_strColorOut(_T("000000"))
	, m_strFontOut(_T("system"))
	, m_strEditX(_T(""))
	, m_strEditY(_T(""))
{

}

CTextDialog::~CTextDialog()
{
}

void CTextDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COLOR, m_strColorOut);
	DDX_Text(pDX, IDC_EDIT_FONT, m_strFontOut);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_strTextString);
	DDX_Text(pDX, IDC_EDIT_X, m_strEditX);
	DDX_Text(pDX, IDC_EDIT_Y, m_strEditY);
}


BEGIN_MESSAGE_MAP(CTextDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TEXTOUT, &CTextDialog::OnClickedButtonTextout)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, &CTextDialog::OnClickedButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_FONT, &CTextDialog::OnClickedButtonFont)
	ON_WM_KEYDOWN()
	ON_WM_KEYDOWN()
	ON_EN_CHANGE(IDC_EDIT_X, &CTextDialog::OnEnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, &CTextDialog::OnEnChangeEditY)
END_MESSAGE_MAP()


// CTextDialog 메시지 처리기입니다.


void CTextDialog::OnClickedButtonTextout()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	CString strString;
	m_strTextString.GetWindowText(strString);

	if(!strString.IsEmpty())
	{
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
		CMy20111688E6_2View* pView = (CMy20111688E6_2View*)pFrame->GetActiveView();
				
		pView->m_ptPosition.x = GetDlgItemInt(IDC_EDIT_X); // CString변수이므로 int로 바꾸어서 대입
		pView->m_ptPosition.y = GetDlgItemInt(IDC_EDIT_Y); // CString변수이므로 int로 바꾸어서 대입

		pView->m_strOutText = strString;
		
		
		pView->Invalidate();
	}
	else
	{
		AfxMessageBox(_T("글을 입력하세요"));
	}

	UpdateData(FALSE);
}


void CTextDialog::OnClickedButtonColor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CMy20111688E6_2View* pView = (CMy20111688E6_2View*)pFrame->GetActiveView();

	CColorDialog dlgColor;
	if(dlgColor.DoModal() == IDOK)
	{
		pView->m_colorText = dlgColor.GetColor();
		m_strColorOut.Format(_T("%06X"),pView->m_colorText);

		pView->Invalidate();
	}

	UpdateData(FALSE);
	Invalidate(true);
}


void CTextDialog::OnClickedButtonFont()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CMy20111688E6_2View* pView = (CMy20111688E6_2View*)pFrame->GetActiveView();
	
	CFontDialog fontDlg;
	if(fontDlg.DoModal() == IDOK)
	{
		pView->m_bFont = true;

		fontDlg.GetCurrentFont(&(pView->m_logfont));
		m_strFontOut.Format(_T("%s"),pView->m_logfont.lfFaceName);

		pView->Invalidate();
	}

	UpdateData(FALSE);
	Invalidate(true);
}



void CTextDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CTextDialog::OnEnChangeEditX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTextDialog::OnEnChangeEditY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
