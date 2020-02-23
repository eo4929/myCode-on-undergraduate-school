
// 20111688P6_2View.h : CMy20111688P6_2View 클래스의 인터페이스
//
#include "MainFrm.h"
#include "atltypes.h"
#include "gdiplustypes.h"
#define RECTANGLE_MODE		1
#define CIRCLE_MODE			2

#pragma once

#include "math.h"
#define M_PI	3.141592649

class CMy20111688P6_2View : public CView
{
protected: // serialization에서만 만들어집니다.
	CMy20111688P6_2View();
	DECLARE_DYNCREATE(CMy20111688P6_2View)

// 특성입니다.
public:
	CMy20111688P6_2Doc* GetDocument() const;

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
	virtual ~CMy20111688P6_2View();
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
	int m_nDrawMode;
	afx_msg void OnRectangle();
	afx_msg void OnCircle();
	afx_msg void OnIncrease();
	afx_msg void OnDecrease();
	CPoint m_ptStart; // 시작점 저장
	CPoint m_ptPrev; // 이전점 저장
	bool m_bLButtonDown; // 왼쪽 버튼 눌렸는지
	CRect m_rtRect; // 사각형 좌표 저장
	Point m_ptCircle[1025]; // 원 좌표 저장
	int m_nResolution; // 해상도 저장
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void computeCircle(void);
};

#ifndef _DEBUG  // 20111688P6_2View.cpp의 디버그 버전
inline CMy20111688P6_2Doc* CMy20111688P6_2View::GetDocument() const
   { return reinterpret_cast<CMy20111688P6_2Doc*>(m_pDocument); }
#endif

