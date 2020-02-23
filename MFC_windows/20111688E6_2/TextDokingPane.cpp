// TextDokingPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "20111688E6_2.h"
#include "TextDokingPane.h"
#include "TextDialog.h"

// CTextDokingPane

IMPLEMENT_DYNAMIC(CTextDokingPane, CDockablePane)

CTextDokingPane::CTextDokingPane()
{

}

CTextDokingPane::~CTextDokingPane()
{
}


BEGIN_MESSAGE_MAP(CTextDokingPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CTextDokingPane 메시지 처리기입니다.




int CTextDokingPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if(!m_textDialog.Create(IDD_DIALOG_TEXT,this))
	{
		TRACE0("키패드 윈도우를 만들지 못하였습니다.\n");
		return -1;
	}

	m_textDialog.ShowWindow(SW_SHOW);
	
	return 0;
}


void CTextDokingPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if(m_textDialog.GetSafeHwnd())
	{
		m_textDialog.MoveWindow(0,0,cx,cy);
		m_textDialog.SetFocus();
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
