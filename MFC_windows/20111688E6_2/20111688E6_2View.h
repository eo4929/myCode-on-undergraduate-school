
// 20111688E6_2View.h : CMy20111688E6_2View 클래스의 인터페이스
//

#pragma once
#include "atltypes.h"
#include "20111688E6_2Doc.h"

class CMy20111688E6_2View : public CView
{
protected: // serialization에서만 만들어집니다.
	CMy20111688E6_2View();
	DECLARE_DYNCREATE(CMy20111688E6_2View)

// 특성입니다.
public:
	CMy20111688E6_2Doc* GetDocument() const;

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
	virtual ~CMy20111688E6_2View();
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
	CString m_strOutText; // 문자열 저장 변수
	BOOL m_bFont; // 폰트가 대화상자에 지정되었는지 나타냄
	LOGFONT m_logfont; // 폰트를 저장할 변수
	COLORREF m_colorText; // 문자열 색상 저장 변수
	CPoint m_ptPosition; // 문자열 출력 위치 저장 변수
	int m_nTextHeight; // 출력된 문자열의 높이 저장 변수
	int m_nTextWidth; // 출력된 문자열 너비 저장 변수
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CString strX;
	CString strY;
	afx_msg void OnDestroy();
	afx_msg void OnEnChangeEditX();
};

#ifndef _DEBUG  // 20111688E6_2View.cpp의 디버그 버전
inline CMy20111688E6_2Doc* CMy20111688E6_2View::GetDocument() const
   { return reinterpret_cast<CMy20111688E6_2Doc*>(m_pDocument); }
#endif

