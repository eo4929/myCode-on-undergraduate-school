
// 20111688E6_2View.cpp : CMy20111688E6_2View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "20111688E6_2.h"
#endif

#include "20111688E6_2Doc.h"
#include "20111688E6_2View.h"
#include "MainFrm.h"
#include "TextDokingPane.h"
#include "TextDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy20111688E6_2View

IMPLEMENT_DYNCREATE(CMy20111688E6_2View, CView)

BEGIN_MESSAGE_MAP(CMy20111688E6_2View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy20111688E6_2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_X, &CMy20111688E6_2View::OnEnChangeEditX)
END_MESSAGE_MAP()

// CMy20111688E6_2View 생성/소멸

CMy20111688E6_2View::CMy20111688E6_2View()
	: m_strOutText(_T(""))
	, m_bFont(FALSE)
	, m_ptPosition(0)
	, m_nTextHeight(0)
	, m_nTextWidth(0)
	, strX(_T(""))
	, strY(_T(""))
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMy20111688E6_2View::~CMy20111688E6_2View()
{
}

BOOL CMy20111688E6_2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMy20111688E6_2View 그리기

void CMy20111688E6_2View::OnDraw(CDC* pDC)
{
	CMy20111688E6_2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	UpdateData(TRUE);

	CFont *oldFont, *currentFont, newFont;

	if(m_bFont) // 폰트 선택
	{
		newFont.CreateFontIndirect(&m_logfont);
		oldFont = pDC->SelectObject(&newFont);
	}
	else
	{
		currentFont = pDC->GetCurrentFont();
		oldFont = pDC->SelectObject(currentFont);
	}

	CSize sizeText; // 문자열 너비 , 높이 구하기
	sizeText = pDC->GetTextExtent(m_strOutText);
	m_nTextWidth = sizeText.cx;
	m_nTextHeight = sizeText.cy;


	pDC->SetTextColor(m_colorText); // 색상 대입
	pDC->TextOut(m_ptPosition.x,m_ptPosition.y,m_strOutText); // 문자열 출력

	UpdateData(FALSE);

	pDC->SelectObject(oldFont); // 폰트 삭제
	if(m_bFont)
	{
		newFont.DeleteObject();
	}
	else
	{
		currentFont->DeleteObject();
	}

	
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMy20111688E6_2View 인쇄


void CMy20111688E6_2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy20111688E6_2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMy20111688E6_2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMy20111688E6_2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMy20111688E6_2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMy20111688E6_2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMy20111688E6_2View 진단

#ifdef _DEBUG
void CMy20111688E6_2View::AssertValid() const
{
	CView::AssertValid();
}

void CMy20111688E6_2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy20111688E6_2Doc* CMy20111688E6_2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy20111688E6_2Doc)));
	return (CMy20111688E6_2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy20111688E6_2View 메시지 처리기


void CMy20111688E6_2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	UpdateData(TRUE);
	
	switch(nChar)
	{
	case VK_LEFT:
		m_ptPosition.x-=10;
		break;
	case VK_RIGHT:
		m_ptPosition.x+=10;
		break;
	case VK_UP:
		m_ptPosition.y-=10;
		break;
	case VK_DOWN:
		m_ptPosition.y+=10;
		break;
	case VK_HOME:
		m_ptPosition = CPoint(0,0);
		break;
	}

	RECT rect;
	GetClientRect(&rect);

	if(m_ptPosition.x > rect.right - m_nTextWidth)
	{
		m_ptPosition.x = rect.right - m_nTextWidth;
		AfxMessageBox(_T("오른쪽으로 더 이상 갈 수 없습니다."));
	}
	if(m_ptPosition.x < 0)
	{
		m_ptPosition.x = 0;
		AfxMessageBox(_T("왼쪽으로 더 이상 갈 수 없습니다."));
	}
	if(m_ptPosition.y > rect.bottom - m_nTextHeight)
	{
		m_ptPosition.y = rect.bottom - m_nTextHeight;
		AfxMessageBox(_T("아래쪽으로 더 이상 갈 수 없습니다."));
	}
	if(m_ptPosition.y < 0)
	{
		m_ptPosition.y = 0;
		AfxMessageBox(_T("위쪽으로 더 이상 갈 수 없습니다."));
	}

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	pFrame->m_textDokingPane.m_textDialog.SetDlgItemInt(IDC_EDIT_X,m_ptPosition.x);
	pFrame->m_textDokingPane.m_textDialog.SetDlgItemInt(IDC_EDIT_Y,m_ptPosition.y);


	Invalidate();
	
	UpdateData(FALSE);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMy20111688E6_2View::OnDestroy()
{
	CView::OnDestroy();

	

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMy20111688E6_2View::OnEnChangeEditX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
