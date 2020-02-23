
// 20111688P7_2Dlg.h : 헤더 파일
//

#pragma once


// CMy20111688P7_2Dlg 대화 상자
class CMy20111688P7_2Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMy20111688P7_2Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MY20111688P7_2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelchangedTreeControl(NMHDR *pNMHDR, LRESULT *pResult);
	CTreeCtrl m_treeControl; // 트리 컨트롤 변수
	CString m_strSelectedNode; // 선택된 노드 문자열 출력
	CString m_strNodeText; //  입력할 노드 텍스트
	HTREEITEM m_hRoot; // 루트에 해당하는 노드
	HTREEITEM m_hSelectedNode; // 선택된 노드
	afx_msg void OnBegindragTreeControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedButtonInsert();
	afx_msg void OnClickedButtonModify();
	afx_msg void OnClickedButtonDelete();
};
