
// 20111688P6_2View.cpp : CMy20111688P6_2View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "20111688P6_2.h"
#endif

#include "20111688P6_2Doc.h"
#include "20111688P6_2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy20111688P6_2View

IMPLEMENT_DYNCREATE(CMy20111688P6_2View, CView)

BEGIN_MESSAGE_MAP(CMy20111688P6_2View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy20111688P6_2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_RECTANGLE, &CMy20111688P6_2View::OnRectangle)
	ON_COMMAND(ID_CIRCLE, &CMy20111688P6_2View::OnCircle)
	ON_COMMAND(ID_INCREASE, &CMy20111688P6_2View::OnIncrease)
	ON_COMMAND(ID_DECREASE, &CMy20111688P6_2View::OnDecrease)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMy20111688P6_2View 생성/소멸

CMy20111688P6_2View::CMy20111688P6_2View()
	: m_nDrawMode(0)
	, m_ptStart(0)
	, m_ptPrev(0)
	, m_bLButtonDown(false)
	, m_nResolution(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_bLButtonDown = FALSE;

	m_rtRect = CRect(0,0,0,0);
	m_nResolution = 64;
	for(int i=0; i<1024; i++)
		m_ptCircle[i] = Point(0,0);
}

CMy20111688P6_2View::~CMy20111688P6_2View()
{
}

BOOL CMy20111688P6_2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMy20111688P6_2View 그리기

void CMy20111688P6_2View::OnDraw(CDC* pDC)
{
	CMy20111688P6_2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	HDC hdc = CreateCompatibleDC(pDC->GetSafeHdc()); // 더블 버퍼로 사용할 DC 설정
	CRect ClientRect; 
	int i;
	GetClientRect(&ClientRect); // 현재 클라이언트 영역 크기 구하기
	HBITMAP bitmap = CreateCompatibleBitmap(hdc,ClientRect.Width(),ClientRect.Height()); // 현재 화면 크기의 비트맵 생성

	SelectObject(hdc,bitmap); // 생성한 비트맵을 더블버퍼 DC에 저장
	Rectangle(hdc,ClientRect.left-1,ClientRect.top-1,ClientRect.right+1,ClientRect.bottom+1); // 더블버퍼 DC를 흰색으로 채움
	
	Graphics graphics(hdc); // 더블버퍼로 사용할 dc를 전달인자로 gdi+ 객체 생성
	Pen pen(Color(0,0,0),1); // pen 생성

	switch(m_nDrawMode)
	{
	case RECTANGLE_MODE: // 사각형 그리기 모드일때
		graphics.DrawRectangle(&pen,m_rtRect.left,m_rtRect.top,m_rtRect.Width(),m_rtRect.Height());
		break;
	case CIRCLE_MODE: // 원 그리기 모드일때
		graphics.DrawRectangle(&pen,m_rtRect.left,m_rtRect.top,m_rtRect.Width()-1,m_rtRect.Height()-1);
		pDC->IntersectClipRect(m_rtRect); // 크리핑 영역 설정
		
		computeCircle();
		for(i=1; i< m_nResolution; i++)
		{
			graphics.DrawLine(&pen,m_ptCircle[i-1],m_ptCircle[i]);
		}

		graphics.DrawLine(&pen,m_ptCircle[i-1],m_ptCircle[0]);
	}

	::BitBlt(pDC->GetSafeHdc(),ClientRect.left,ClientRect.top,ClientRect.Width(), // 더블버퍼dc에 그린 것을 원래 dc에 옮겨서 그림
		ClientRect.Height(),hdc,ClientRect.left,ClientRect.top,SRCCOPY);

	DeleteDC(hdc);
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMy20111688P6_2View 인쇄


void CMy20111688P6_2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy20111688P6_2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMy20111688P6_2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMy20111688P6_2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMy20111688P6_2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMy20111688P6_2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMy20111688P6_2View 진단

#ifdef _DEBUG
void CMy20111688P6_2View::AssertValid() const
{
	CView::AssertValid();
}

void CMy20111688P6_2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy20111688P6_2Doc* CMy20111688P6_2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy20111688P6_2Doc)));
	return (CMy20111688P6_2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy20111688P6_2View 메시지 처리기


void CMy20111688P6_2View::OnRectangle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = RECTANGLE_MODE;
}


void CMy20111688P6_2View::OnCircle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = CIRCLE_MODE;
}


void CMy20111688P6_2View::OnIncrease()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if(m_nResolution >= 1024)
	{
		AfxMessageBox(_T("현재 최대 해상도인 1024입니다."));
	}
	else
	{
		m_nResolution = m_nResolution * 2;
	}
	Invalidate();

	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CString strResol;
	strResol.Format(_T("해상도 : %d "),m_nResolution);
	pFrame->m_wndStatusBar.SetPaneText(1,strResol);
}


void CMy20111688P6_2View::OnDecrease()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if(m_nResolution <= 2)
	{
		AfxMessageBox(_T("현재 최소 해상도인 2입니다."));
	}
	else
	{
		m_nResolution = m_nResolution / 2;
	}
	
	Invalidate();

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CString strResol;
	strResol.Format(_T("해상도 : %d"),m_nResolution);
	pFrame->m_wndStatusBar.SetPaneText(1,strResol);
}


void CMy20111688P6_2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(m_nDrawMode)
	{
	case RECTANGLE_MODE:
		m_rtRect = CRect(0,0,0,0);

	case CIRCLE_MODE:
		m_ptStart = m_ptPrev = point;
		m_bLButtonDown = true;
		for(int i=0; i<1024; i++)
			m_ptCircle[i] = Point(0,0); // 원 초기화

		Invalidate(TRUE);

		RECT rectClient; // 사각 구조체 변수 선언
		SetCapture(); // 마우스 캡처
		GetClientRect(&rectClient); // 클라이언트 영역 얻음
		ClientToScreen(&rectClient); // 스크린 좌표계로 변환
		::ClipCursor(&rectClient); // 마우스 이동 범위 클라이언트 영역으로 제한
		break;
	}
	CView::OnLButtonDown(nFlags, point);
}


void CMy20111688P6_2View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC dc(this);

	Graphics graphics(dc);
	Pen pen(Color(0,0,0),1); // 현재 도형을 그려줄 객체 생성
	Pen XORpen(Color(255,255,255),1); // 이전 도형을 그려줄 객체 생성

	switch(m_nDrawMode)
	{
	case RECTANGLE_MODE:
		if(m_bLButtonDown)
		{
			CRect PrevRect = CRect(m_ptStart.x,m_ptStart.y,m_ptPrev.x,m_ptPrev.y); // 이전 사각형 좌표 저장
			CRect CurRect = CRect(m_ptStart.x,m_ptStart.y,point.x,point.y); // 현재 사각형 좌표 저장
			
			graphics.DrawRectangle(&XORpen,PrevRect.left,PrevRect.top,PrevRect.Width(),PrevRect.Height()); // 이전 사각형 지움
			graphics.DrawRectangle(&pen,CurRect.left,CurRect.top,CurRect.Width(),CurRect.Height()); // 현재 사각형 지움

			m_ptPrev = point;
		}
		break;
	case CIRCLE_MODE:
		if(m_bLButtonDown)
		{
			m_ptPrev = point;
			Invalidate();
		}
		break;
	}

	CView::OnMouseMove(nFlags, point);
}


void CMy20111688P6_2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(m_nDrawMode)
	{
	case RECTANGLE_MODE:
		if(m_bLButtonDown)
		{
			m_bLButtonDown = false;
			m_rtRect = CRect(m_ptStart.x,m_ptStart.y,point.x,point.y);
		}
		break;
	case CIRCLE_MODE:
		if(m_bLButtonDown)
		{
			m_bLButtonDown = false;
			m_ptPrev = point;
		}
		break;
	}

	ReleaseCapture();
	::ClipCursor(NULL);
	Invalidate(true);

	CView::OnLButtonUp(nFlags, point);
}


void CMy20111688P6_2View::computeCircle(void)
{
	double x,y,r,radian;

	r = sqrt( (double) ( (m_ptStart.x - m_ptPrev.x)*(m_ptStart.x - m_ptPrev.x) + (m_ptStart.y - m_ptPrev.y)*(m_ptStart.y - m_ptPrev.y) ) );
	radian = M_PI * 2 / (double)m_nResolution;

	for(int i=0; i<m_nResolution; i++) // 해상도에 따른 원의 좌표
	{
		x = m_ptStart.x + r*cos(radian*i);
		y = m_ptStart.y + r*sin(radian*i);
		m_ptCircle[i] = Point(x,y);
	}
}
