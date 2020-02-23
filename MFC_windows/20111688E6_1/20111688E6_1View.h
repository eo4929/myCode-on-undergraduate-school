
// 20111688E6_1View.h : CMy20111688E6_1View 클래스의 인터페이스
//

#pragma once
#include "atltypes.h"
#include "20111688E6_1Doc.h"


class CMy20111688E6_1View : public CView
{
protected: // serialization에서만 만들어집니다.
	CMy20111688E6_1View();
	DECLARE_DYNCREATE(CMy20111688E6_1View)

// 특성입니다.
public:
	CMy20111688E6_1Doc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMy20111688E6_1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	int m_nPenSize;
	int m_nEraserSize;
	CPoint m_ptPrev;
	COLORREF m_colorPen;
	afx_msg void OnPenSize();
	afx_msg void OnPenColor();
	afx_msg void OnEraserSize();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	bool m_nDrawMode;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	bool m_nEraserMode;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	bool m_nClickAndMove;
};

#ifndef _DEBUG  // 20111688E6_1View.cpp의 디버그 버전
inline CMy20111688E6_1Doc* CMy20111688E6_1View::GetDocument() const
   { return reinterpret_cast<CMy20111688E6_1Doc*>(m_pDocument); }
#endif

