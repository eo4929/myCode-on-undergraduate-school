
// 2011688P5_1View.cpp : CMy2011688P5_1View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "2011688P5_1.h"
#endif

#include "2011688P5_1Doc.h"
#include "2011688P5_1View.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2011688P5_1View

IMPLEMENT_DYNCREATE(CMy2011688P5_1View, CView)

BEGIN_MESSAGE_MAP(CMy2011688P5_1View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy2011688P5_1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_LINE, &CMy2011688P5_1View::OnLine)
	ON_COMMAND(ID_ELLIPSE, &CMy2011688P5_1View::OnEllipse)
	ON_COMMAND(ID_POLYGON, &CMy2011688P5_1View::OnPolygon)
	ON_COMMAND(ID_LINE_COLOR, &CMy2011688P5_1View::OnLineColor)
	ON_COMMAND(ID_FACE_COLOR, &CMy2011688P5_1View::OnFaceColor)
//	ON_UPDATE_COMMAND_UI(ID_LINE_COLOR, &CMy2011688P5_1View::OnUpdateLineColor)
//	ON_UPDATE_COMMAND_UI(ID_FACE_COLOR, &CMy2011688P5_1View::OnUpdateFaceColor)
	ON_COMMAND(ID_BDIAGONAL, &CMy2011688P5_1View::OnBdiagonal)
	ON_COMMAND(ID_CROSS, &CMy2011688P5_1View::OnCross)
	ON_COMMAND(ID_VERTICAL, &CMy2011688P5_1View::OnVertical)
	ON_COMMAND(ID__SHOW_BITMAP, &CMy2011688P5_1View::OnShowBitmap)
//	ON_COMMAND(AFX_IDC_MAGNIFY, &CMy2011688P5_1View::OnAfxIdcMagnify)
	ON_COMMAND(ID_REDUCE_BITMAP, &CMy2011688P5_1View::OnReduceBitmap)
	ON_COMMAND(ID_ALPHA_BLEND, &CMy2011688P5_1View::OnAlphaBlend)
	ON_COMMAND(ID_MAGNIFY_BITMAP, &CMy2011688P5_1View::OnMagnifyBitmap)
	ON_UPDATE_COMMAND_UI(ID_LINE, &CMy2011688P5_1View::OnUpdateLine)
	ON_UPDATE_COMMAND_UI(ID_ELLIPSE, &CMy2011688P5_1View::OnUpdateEllipse)
	ON_UPDATE_COMMAND_UI(ID_POLYGON, &CMy2011688P5_1View::OnUpdatePolygon)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CMy2011688P5_1View 생성/소멸

CMy2011688P5_1View::CMy2011688P5_1View()
	: m_nDrawMode(0)
	, m_nHatchStyle(0)
	, m_nBitmapMode(0)
	, m_ptStart(0)
	, m_ptPrev(0)
	, m_bFirst(false)
	, m_bLButtonDown(false)
	, m_bContextMenu(false)
	, m_bHatch(false)
	, m_nCount(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_nDrawMode = LINE_MODE; // 그리기 모드 : 펜으로 그리기
	m_bContextMenu = true; 
	m_bFirst = true; 
	m_bLButtonDown = false;
	m_nCount = 0;
	
	for(int i=0; i<50; i++)
	{
		m_ptData[i] = 0; // 배열 초기화
	}
	m_bHatch = false;
	m_nHatchStyle = HS_CROSS; // 해시브러시 십자가 빗금으로 초기화
	m_colorPen = RGB(0,0,0); // 컬러 검은색으로 초기화
	m_colorBrush = RGB(255,255,255); // 컬러 흰색으로 초기화
}

CMy2011688P5_1View::~CMy2011688P5_1View()
{
}

BOOL CMy2011688P5_1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMy2011688P5_1View 그리기

void CMy2011688P5_1View::OnDraw(CDC* pDC)
{
	CMy2011688P5_1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CPen pen,*oldpen; // pen 객체 생성
	pen.CreatePen(PS_SOLID,1,m_colorPen); 
	oldpen = pDC->SelectObject(&pen); // pen 객체 등록 및 oldpen 포인터 저장
	//pDC->SetROP2(R2_COPYPEN);
	pDC->SetROP2(R2_NOTXORPEN);
	CBrush brush, *oldbrush;
	
	if(m_bHatch)
		brush.CreateHatchBrush(m_nHatchStyle,m_colorBrush);
	else
		brush.CreateSolidBrush(m_colorBrush);
	oldbrush = pDC->SelectObject(&brush);

	switch(m_nDrawMode)
	{
	case LINE_MODE: 
		pDC->MoveTo(m_ptStart); 
		pDC->LineTo(m_ptPrev); 
		break;
	case ELLIPSE_MODE:
		pDC->Ellipse(m_ptStart.x,m_ptStart.y,m_ptPrev.x,m_ptPrev.y);
		break;
	case POLYGON_MODE:
		pDC->Polygon(m_ptData,m_nCount);
		break;
	}

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	pen.DeleteObject();
	brush.DeleteObject();

	//
	CDC memdc; // 메모리 DC 생성
	memdc.CreateCompatibleDC(pDC); // 호환성이 있는 메모리 생성

	CBitmap bitmap, *oldbitmap; // 비트맵의 인스턴스 생성
	bitmap.LoadBitmap(IDB_WINDOWS_LOGO); // 리소스에서 비트맵 읽기
	oldbitmap = memdc.SelectObject(&bitmap); // 메모리DC에서 비트맵 선택

	BITMAP bmpinfo; // 비트맵 정보
	bitmap.GetBitmap(&bmpinfo); // 리소스에서 비트맵 가져오기
	memdc.SelectObject(&bitmap); 
	
	BLENDFUNCTION bf; // 투명도 설정
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;

	switch(m_nBitmapMode)
	{
	case 1: // 비트맵 출력
		pDC->BitBlt(0,0,bmpinfo.bmWidth,bmpinfo.bmHeight,&memdc,0,0,SRCCOPY);
		break;
	case 2: // 비트맵 2배 확대 출력
		pDC->StretchBlt(0,0,bmpinfo.bmWidth*2,bmpinfo.bmHeight*2,&memdc,0,0,
			bmpinfo.bmWidth, bmpinfo.bmHeight,SRCCOPY);
		break;
	case 3: // 비트맵 2배 축소 출력
		pDC->StretchBlt(0,0,bmpinfo.bmWidth/2,bmpinfo.bmHeight/2,&memdc,0,0,
			bmpinfo.bmWidth, bmpinfo.bmHeight,SRCCOPY);
		break;
	case 4:
		bf.SourceConstantAlpha = 128; // 투명도 설정
		
		pDC->AlphaBlend(0,0,bmpinfo.bmWidth,bmpinfo.bmHeight,&memdc,0,0,bmpinfo.bmWidth,bmpinfo.bmHeight,bf);
		break;
	}
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	memdc.SelectObject(oldbitmap);
}


// CMy2011688P5_1View 인쇄


void CMy2011688P5_1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy2011688P5_1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMy2011688P5_1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMy2011688P5_1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMy2011688P5_1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMy2011688P5_1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	if(m_bContextMenu == true)
	{
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
	}
	m_bContextMenu = true;
#endif
}


// CMy2011688P5_1View 진단

#ifdef _DEBUG
void CMy2011688P5_1View::AssertValid() const
{
	CView::AssertValid();
}

void CMy2011688P5_1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2011688P5_1Doc* CMy2011688P5_1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2011688P5_1Doc)));
	return (CMy2011688P5_1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy2011688P5_1View 메시지 처리기


void CMy2011688P5_1View::OnLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = LINE_MODE;
	m_nBitmapMode = 0;

	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	
	CString strDrawMode = (_T("직선 그리기"));
	
	pFrame->m_wndStatusBar.SetWindowText(strDrawMode);
}


void CMy2011688P5_1View::OnEllipse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = ELLIPSE_MODE;
	m_nBitmapMode = 0;

	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	
	CString strDrawMode = (_T("원 그리기"));
	
	pFrame->m_wndStatusBar.SetWindowText(strDrawMode);
}


void CMy2011688P5_1View::OnPolygon()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nDrawMode = POLYGON_MODE;
	m_nBitmapMode = 0;

	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	
	CString strDrawMode = (_T("다각형 그리기"));
	
	pFrame->m_wndStatusBar.SetWindowText(strDrawMode);
}


void CMy2011688P5_1View::OnLineColor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorDialog dlgColor;

	if(dlgColor.DoModal() == IDOK)
	{
		m_colorPen = dlgColor.GetColor();
	}
	Invalidate(true);
}


void CMy2011688P5_1View::OnFaceColor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorDialog dlgColor;

	if(dlgColor.DoModal() == IDOK)
	{
		m_colorBrush = dlgColor.GetColor();
	}

	m_bHatch = false;
	Invalidate(true);
}



//void CMy2011688P5_1View::OnUpdateLineColor(CCmdUI *pCmdUI)
//{
//	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
//	AfxMessageBox(_T("선의 색상을 변경합니다."));
//}


//void CMy2011688P5_1View::OnUpdateFaceColor(CCmdUI *pCmdUI)
//{
//	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
//	AfxMessageBox(_T("면의 색상을 변경합니다."));
//}


void CMy2011688P5_1View::OnBdiagonal()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nHatchStyle = HS_BDIAGONAL;

	m_bHatch = true;
	Invalidate(true);

}


void CMy2011688P5_1View::OnCross()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nHatchStyle = HS_CROSS;

	m_bHatch = true;
	Invalidate(true);
}


void CMy2011688P5_1View::OnVertical()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nHatchStyle = HS_VERTICAL;

	m_bHatch = true;
	Invalidate(true);
}


void CMy2011688P5_1View::OnShowBitmap()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nBitmapMode = 1;
	m_nDrawMode = DRAW_NONE;
	Invalidate();
}


//void CMy2011688P5_1View::OnAfxIdcMagnify()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	AfxMessageBox(_T("비트맵을 2배 확대하여 출력합니다."));
//}


void CMy2011688P5_1View::OnReduceBitmap()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nBitmapMode = 3;
	m_nDrawMode = DRAW_NONE;
	Invalidate();
}


void CMy2011688P5_1View::OnAlphaBlend()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nBitmapMode = 4;
	m_nDrawMode = DRAW_NONE;
	Invalidate();
}


void CMy2011688P5_1View::OnMagnifyBitmap()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nBitmapMode = 2;
	m_nDrawMode = DRAW_NONE;
	Invalidate();
}


void CMy2011688P5_1View::OnUpdateLine(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_nDrawMode == LINE_MODE ? 1 : 0);
}


void CMy2011688P5_1View::OnUpdateEllipse(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_nDrawMode == ELLIPSE_MODE ? 1 : 0);

}


void CMy2011688P5_1View::OnUpdatePolygon(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_nDrawMode == POLYGON_MODE ? 1 : 0);
}


void CMy2011688P5_1View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC dc(this); // 클라이언트 DC 객체 얻음
	CPen pen, *oldpen;
	pen.CreatePen(PS_SOLID,1,m_colorPen); // 펜 생성
	oldpen = dc.SelectObject(&pen); // pen 등록

	dc.SetROP2(R2_NOTXORPEN); // 이전 그림 지워지고 다시 시작점과 현재 점으로 그림 그리면 러버밴드로 그림 그려짐
	//dc.SetROP2(R2_COPYPEN);
	//dc.SetROP2(R2_XORPEN);

	CBrush brush, *oldbrush;
	if(m_bHatch)
		brush.CreateHatchBrush(m_nHatchStyle,m_colorBrush);
	else
		brush.CreateSolidBrush(m_colorBrush);

	oldbrush = dc.SelectObject(&brush);

	switch(m_nDrawMode)
	{
	case LINE_MODE:
		if(m_bLButtonDown)
		{
			dc.MoveTo(m_ptStart);
			dc.LineTo(m_ptPrev); // 이전 직선 지움

			dc.MoveTo(m_ptStart);
			dc.LineTo(point); // 현재 직선 그림

			m_ptPrev = point; // 이전 점에 현재 점 저장
		}
		break;
	case ELLIPSE_MODE:
		if(m_bLButtonDown)
		{
			dc.Ellipse(m_ptStart.x,m_ptStart.y,m_ptPrev.x,m_ptPrev.y); // 이전 원 지움

			dc.Ellipse(m_ptStart.x,m_ptStart.y,point.x,point.y); // 현재 원 그리기
			
			m_ptPrev = point; // 이전 점에 현재 점 저장
		}
		break;
	case POLYGON_MODE:
		if(!m_bFirst)
		{
			dc.MoveTo(m_ptStart);
			dc.LineTo(m_ptPrev);
			
			dc.MoveTo(m_ptStart);
			dc.LineTo(point);
			m_ptPrev = point;
		}
		break;
	}

	dc.SelectObject(oldpen);
	dc.SelectObject(oldbrush);
	pen.DeleteObject();
	brush.DeleteObject();
	//
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	
	CString strPoint;
	strPoint.Format(_T("마우스 위치 x : %d, y : %d"),point.x,point.y);
	
	pFrame->m_wndStatusBar.SetPaneText(1,strPoint);

	CView::OnMouseMove(nFlags, point);
}


void CMy2011688P5_1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(m_bFirst) // 전에 있던 그림 지울 수 있게 하는 부분
	{
		m_ptStart = m_ptPrev = 0;
		m_bLButtonDown = false;
		m_bFirst = true;
		m_nCount = 0;
		for(int i=0; i<50; i++)
		{
			m_ptData[i] = 0;
		}
		Invalidate(true);
	}

	switch(m_nDrawMode)
	{
	case LINE_MODE:
	case ELLIPSE_MODE:
		m_bLButtonDown = true; // 왼쪽 버튼 눌림
		m_ptStart = m_ptPrev = point; // 시작 점과 이전 점에 현재 점을 저장
		m_bFirst = false;
		break;
	case POLYGON_MODE:
		if(m_bFirst == true)
			m_bFirst = false;
		m_ptStart = m_ptPrev = point; // 시작 점과 이전 점에 현재 점을 저장
		m_ptData[m_nCount] = point; // 현재 점 저장
		m_nCount++;
		break;
	}

	if(m_nBitmapMode == 0) // 모드가 없다면
	{
		RECT rectClient;
		SetCapture(); // 마우스 캡쳐 (러버밴드 사용 시 왼쪽버튼 누를 때 유용) -> 마우스에 대한 모든 이벤트 가져옴
		GetClientRect(&rectClient);
		ClientToScreen(&rectClient); // 스크린 좌표계 반환
		::ClipCursor(&rectClient); // 마우스 이동범위를 얻어온 구조체 영역(클라이언트 영역)으로 제한
	}
	CView::OnLButtonDown(nFlags, point);
}


void CMy2011688P5_1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(m_bLButtonDown) // 왼쪽 버튼이 눌린 상태였다면
	{
		if(m_nDrawMode == LINE_MODE || m_nDrawMode == ELLIPSE_MODE)
		{
			m_bLButtonDown = false;
			m_bFirst = true;

			ReleaseCapture();
			::ClipCursor(NULL); // 마우스 이동범위 제한 해제
			Invalidate(true);
		}
	}
	CView::OnLButtonUp(nFlags, point);
}


void CMy2011688P5_1View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if(m_nDrawMode == POLYGON_MODE)
	{
		if(!m_bFirst)
		{
			m_bContextMenu = FALSE;
			m_ptData[m_nCount] = point; // 배열에 현재 점 저장
			m_nCount++; // 카운트 증가
			m_bFirst = TRUE;

			//ReleaseCapture(); // 마우스 캡쳐 해제
			//::ClipCursor(NULL);
			Invalidate(TRUE);
		}
	}

	CView::OnRButtonDown(nFlags, point);
}
