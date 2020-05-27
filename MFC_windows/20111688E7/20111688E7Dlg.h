
// 20111688E7Dlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "studentdialog.h"
#include "deletedialog.h"

typedef struct
{
	CString strDept;
	CString strID;
	CString strName;
	int nGrade;
	int nSex;
	CString strAddress;
}Student;


// CMy20111688E7Dlg 대화 상자
class CMy20111688E7Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMy20111688E7Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MY20111688E7_DIALOG };

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
	CTreeCtrl m_treeControl;
	CListCtrl m_listControl;
	HTREEITEM m_hRoot; // 루트노드
	CStudentDialog m_CStudentDialog; // 학생 정보 다이얼로그 
	HTREEITEM m_hKorean; // 국어국문학과
	HTREEITEM m_hComputer; // 컴퓨터학과
	HTREEITEM m_hPhysics; // 물리학과
	Student m_studentInfo[50]; // 학생 정보 저장할 배열
	int m_nCount; // 배열에 저장된 학생 수
	int m_nSelectedItem; // 선택된 리스트 인덱스
	HTREEITEM m_nSelectedNode; // 선택된 노드
	afx_msg void OnClickedButtonInsert();
	afx_msg void OnSelchangedTreeControl(NMHDR *pNMHDR, LRESULT *pResult);
	bool m_selectTree; // 트리가 선택되었다면
	bool m_selectList; // 리스트가 선택되었다면
	afx_msg void OnItemchangedListControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedButtonModify();
	afx_msg void OnClickedButtonDelete();
	CDeleteDialog m_deleteDialog; // 삭제 다이얼로그
};
