
// 20111688E7Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "20111688E7.h"
#include "20111688E7Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy20111688E7Dlg 대화 상자




CMy20111688E7Dlg::CMy20111688E7Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy20111688E7Dlg::IDD, pParent)
	, m_nCount(0)
	, m_nSelectedItem(0)
	, m_selectTree(false)
	, m_selectList(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy20111688E7Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CONTROL, m_treeControl);
	DDX_Control(pDX, IDC_LIST_CONTROL, m_listControl);
}

BEGIN_MESSAGE_MAP(CMy20111688E7Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CMy20111688E7Dlg::OnClickedButtonInsert)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CONTROL, &CMy20111688E7Dlg::OnSelchangedTreeControl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CONTROL, &CMy20111688E7Dlg::OnItemchangedListControl)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CMy20111688E7Dlg::OnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CMy20111688E7Dlg::OnClickedButtonDelete)
END_MESSAGE_MAP()


// CMy20111688E7Dlg 메시지 처리기

BOOL CMy20111688E7Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	// 트리 초기화
	TV_INSERTSTRUCT tvStruct1; //가상대학교 -> 루트 노드
	tvStruct1.hParent = 0;
	tvStruct1.hInsertAfter = TVI_LAST;
	tvStruct1.item.mask = TVIF_TEXT;
	tvStruct1.item.pszText = _T("가상대학교");
	m_hRoot = m_treeControl.InsertItem(&tvStruct1);

	TV_INSERTSTRUCT tvStruct2; // 국어국문학과
	tvStruct2.hParent = m_hRoot;
	tvStruct2.hInsertAfter = TVI_LAST;
	tvStruct2.item.mask = TVIF_TEXT;
	tvStruct2.item.pszText = _T("국어국문학과");
	m_hKorean = m_treeControl.InsertItem(&tvStruct2);
	m_treeControl.Expand(m_hRoot,TVE_EXPAND);

	TV_INSERTSTRUCT tvStruct3; // 컴퓨터공학과
	tvStruct3.hParent = m_hRoot;
	tvStruct3.hInsertAfter = TVI_LAST;
	tvStruct3.item.mask = TVIF_TEXT;
	tvStruct3.item.pszText = _T("컴퓨터공학과");
	m_hComputer = m_treeControl.InsertItem(&tvStruct3);
	m_treeControl.Expand(m_hRoot,TVE_EXPAND);

	TV_INSERTSTRUCT tvStruct4; // 물리학과
	tvStruct4.hParent = m_hRoot;
	tvStruct4.hInsertAfter = TVI_LAST;
	tvStruct4.item.mask = TVIF_TEXT;
	tvStruct4.item.pszText = _T("물리학과");
	m_hPhysics = m_treeControl.InsertItem(&tvStruct4);
	m_treeControl.Expand(m_hRoot,TVE_EXPAND);

	// 리스트 초기화
	LV_COLUMN lvColumn;
	LPWSTR list[5] = {_T("학번"),_T("이름"),_T("학년"),_T("성별"),_T("주소")};
	int nWidth[5] = {40,90,60,40,240};

	for(int i=0; i<5; i++)
	{
		lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_CENTER;
		lvColumn.pszText = list[i];
		lvColumn.iSubItem = i;
		lvColumn.cx = nWidth[i];
		m_listControl.InsertColumn(i,&lvColumn);
	}
	m_listControl.SetExtendedStyle(m_listControl.GetExtendedStyle() | 
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	// 버튼 초기화
	((CButton*)GetDlgItem(IDC_BUTTON_INSERT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_MODIFY))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMy20111688E7Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMy20111688E7Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMy20111688E7Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy20111688E7Dlg::OnClickedButtonInsert()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CStudentDialog studentDialog;
	int itemNum = m_listControl.GetItemCount();
	BOOL bInsert = TRUE;
	
	if(m_nSelectedNode == m_hRoot)
	{
		AfxMessageBox(_T("학과를 선택하세요"));
		return;
	}

	if(studentDialog.DoModal() == IDOK)
	{
		for(int i=0; i<itemNum; i++)
		{
			CString strNum = m_listControl.GetItemText(i,0); // 학번 가져오기

			if(strNum == studentDialog.m_strNumber) // 삽입되는 학생의 학번이 같다면
			{
				bInsert = FALSE;
			}
		}
			if(studentDialog.m_strName.IsEmpty() || studentDialog.m_strNumber.IsEmpty() || studentDialog.m_strAddress.IsEmpty() )
			{
				AfxMessageBox(_T("모든 항목을 입력하세요"));

				((CButton*)GetDlgItem(IDC_BUTTON_INSERT))->EnableWindow(FALSE);
				((CButton*)GetDlgItem(IDC_BUTTON_MODIFY))->EnableWindow(FALSE);
				((CButton*)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(FALSE);

				return;
			}
			if(bInsert)
			{
				CString strLevel; // 학년 문자열
				if(studentDialog.m_intLevel==0)
				{
					strLevel.Format(_T("1학년"));
				}
				else if(studentDialog.m_intLevel==1)
				{
					strLevel.Format(_T("2학년"));
				}
				else if(studentDialog.m_intLevel==2)
				{
					strLevel.Format(_T("3학년"));
				}
				else if(studentDialog.m_intLevel==3)
				{
					strLevel.Format(_T("4학년"));
				}
				
				CString strSex; // 성별 문자열
				if(studentDialog.m_intSex==0)
				{
					strSex.Format(_T("남자"));
				}
				else if(studentDialog.m_intSex==1)
				{
					strSex.Format(_T("여자"));
				}
				
				LV_ITEM lvItem1;	
				lvItem1.mask = LVIF_TEXT;
				lvItem1.iItem = itemNum;
				lvItem1.iSubItem = 0;
				lvItem1.pszText = (LPWSTR)(LPCTSTR)studentDialog.m_strNumber;
				m_listControl.InsertItem(&lvItem1); // 학번 삽입
				
				LV_ITEM lvItem2;
				lvItem2.mask = LVIF_TEXT;
				lvItem2.iItem = itemNum;
				lvItem2.iSubItem = 1;
				lvItem2.pszText = (LPWSTR)(LPCTSTR)studentDialog.m_strName;
				m_listControl.SetItem(&lvItem2); // 이름 삽입

				LV_ITEM lvItem3;
				lvItem3.mask = LVIF_TEXT;
				lvItem3.iItem = itemNum;
				lvItem3.iSubItem = 2;
				lvItem3.pszText = (LPWSTR)(LPCTSTR)strLevel;
				m_listControl.SetItem(&lvItem3); // 학년 삽입

				LV_ITEM lvItem4;
				lvItem4.mask = LVIF_TEXT;
				lvItem4.iItem = itemNum;
				lvItem4.iSubItem = 3;
				lvItem4.pszText = (LPWSTR)(LPCTSTR)strSex;
				m_listControl.SetItem(&lvItem4); // 성별 삽입

				LV_ITEM lvItem5;
				lvItem5.mask = LVIF_TEXT;
				lvItem5.iItem = itemNum;
				lvItem5.iSubItem = 4;
				lvItem5.pszText = (LPWSTR)(LPCTSTR)studentDialog.m_strAddress;
				m_listControl.SetItem(&lvItem5); // 주소 삽입
				
				m_studentInfo[m_nCount].strDept = m_treeControl.GetItemText(m_nSelectedNode); // 학생 구조체 배열 초기화
				m_studentInfo[m_nCount].strID = studentDialog.m_strNumber;
				m_studentInfo[m_nCount].strName = studentDialog.m_strName;
				m_studentInfo[m_nCount].nGrade = studentDialog.m_intLevel;
				m_studentInfo[m_nCount].nSex = studentDialog.m_intSex;
				m_studentInfo[m_nCount].strAddress = studentDialog.m_strAddress;
				m_nCount++;
			}
			else
			{
				AfxMessageBox(_T("이미 등록된 학번입니다."));
			}
		
	}

	((CButton*)GetDlgItem(IDC_BUTTON_INSERT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_MODIFY))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(FALSE);

	UpdateData(FALSE);
}


void CMy20111688E7Dlg::OnSelchangedTreeControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	m_selectTree =true; // 트리에 접근

	m_nSelectedNode = pNMTreeView->itemNew.hItem; // 선택된 노드 체크
	int itemCount = m_nCount; // 배열에 저장된 학생 수
	int nDept = 0; // 학과마다 학생 수

	m_listControl.DeleteAllItems(); // 리스트 초기화

	for(int i=0; i<itemCount; i++) // 리스트 개수까지
	{
				CString strLevel;
				if(m_studentInfo[i].nGrade==0)
				{
					strLevel.Format(_T("1학년"));
				}
				else if(m_studentInfo[i].nGrade==1)
				{
					strLevel.Format(_T("2학년"));
				}
				else if(m_studentInfo[i].nGrade==2)
				{
					strLevel.Format(_T("3학년"));
				}
				else if(m_studentInfo[i].nGrade==3)
				{
					strLevel.Format(_T("4학년"));
				}
				
				CString strSex;
				if(m_studentInfo[i].nSex==0)
				{
					strSex.Format(_T("남자"));
				}
				else if(m_studentInfo[i].nSex==1)
				{
					strSex.Format(_T("여자"));
				}

		if(m_nSelectedNode == m_hRoot) // 루트노드 선택되었다면 학생 배열에 있는 모든 정보 -> 리스트에 삽입
		{
				LV_ITEM lvItem1;	
				lvItem1.mask = LVIF_TEXT;
				lvItem1.iItem = i;
				lvItem1.iSubItem = 0;
				lvItem1.pszText = (LPWSTR)(LPCTSTR)m_studentInfo[i].strID;
				m_listControl.InsertItem(&lvItem1); //
				
				LV_ITEM lvItem2;
				lvItem2.mask = LVIF_TEXT;
				lvItem2.iItem = i;
				lvItem2.iSubItem = 1;
				lvItem2.pszText = (LPWSTR)(LPCTSTR)m_studentInfo[i].strName;
				m_listControl.SetItem(&lvItem2); //

				LV_ITEM lvItem3;
				lvItem3.mask = LVIF_TEXT;
				lvItem3.iItem = i;
				lvItem3.iSubItem = 2;
				lvItem3.pszText = (LPWSTR)(LPCTSTR)strLevel;
				m_listControl.SetItem(&lvItem3); //

				LV_ITEM lvItem4;
				lvItem4.mask = LVIF_TEXT;
				lvItem4.iItem = i;
				lvItem4.iSubItem = 3;
				lvItem4.pszText = (LPWSTR)(LPCTSTR)strSex;
				m_listControl.SetItem(&lvItem4); //

				LV_ITEM lvItem5;
				lvItem5.mask = LVIF_TEXT;
				lvItem5.iItem = i;
				lvItem5.iSubItem = 4;
				lvItem5.pszText = (LPWSTR)(LPCTSTR)m_studentInfo[i].strAddress;
				m_listControl.SetItem(&lvItem5); //
		}
		else if(m_nSelectedNode == m_hKorean) // 국어국문학과 선택되었다면
		{
			if(m_studentInfo[i].strDept == "국어국문학과")
			{
				LV_ITEM lvItem;	
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept;
				lvItem.iSubItem = 0;
				lvItem.pszText = (LPWSTR)(LPCTSTR)m_studentInfo[i].strID;
				m_listControl.InsertItem(&lvItem); //
				
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept;
				lvItem.iSubItem = 1;
				lvItem.pszText = (LPWSTR)(LPCTSTR)m_studentInfo[i].strName;
				m_listControl.SetItem(&lvItem); //

				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept;
				lvItem.iSubItem = 2;
				lvItem.pszText = (LPWSTR)(LPCTSTR)strLevel;
				m_listControl.SetItem(&lvItem); //

				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept;
				lvItem.iSubItem = 3;
				lvItem.pszText = (LPWSTR)(LPCTSTR)strSex;
				m_listControl.SetItem(&lvItem); //

				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept;
				lvItem.iSubItem = 4;
				lvItem.pszText = (LPWSTR)(LPCTSTR)m_studentInfo[i].strAddress;
				m_listControl.SetItem(&lvItem); //

				nDept++;
			}
		}
		else if(m_nSelectedNode == m_hComputer) // 컴퓨터공학과 선택되었다면
		{
			if(m_studentInfo[i].strDept == "컴퓨터공학과") 
			{
				LV_ITEM lvItem;	
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept;
				lvItem.iSubItem = 0;
				lvItem.pszText = (LPWSTR)(LPCTSTR)m_studentInfo[i].strID;
				m_listControl.InsertItem(&lvItem); //
				
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept;
				lvItem.iSubItem = 1;
				lvItem.pszText = (LPWSTR)(LPCTSTR)m_studentInfo[i].strName;
				m_listControl.SetItem(&lvItem); //

				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept;
				lvItem.iSubItem = 2;
				lvItem.pszText = (LPWSTR)(LPCTSTR)strLevel;
				m_listControl.SetItem(&lvItem); //

				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept;
				lvItem.iSubItem = 3;
				lvItem.pszText = (LPWSTR)(LPCTSTR)strSex;
				m_listControl.SetItem(&lvItem); //

				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept;
				lvItem.iSubItem = 4;
				lvItem.pszText = (LPWSTR)(LPCTSTR)m_studentInfo[i].strAddress;
				m_listControl.SetItem(&lvItem); //

				nDept++;
			}
		}
		else if(m_nSelectedNode == m_hPhysics) // 물리학과 선택되었다면
		{
			if(m_studentInfo[i].strDept == "물리학과")
			{
				LV_ITEM lvItem;	
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept; // ...
				lvItem.iSubItem = 0;
				lvItem.pszText = (LPWSTR)(LPCTSTR)m_studentInfo[i].strID;
				m_listControl.InsertItem(&lvItem); //
				
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept;
				lvItem.iSubItem = 1;
				lvItem.pszText = (LPWSTR)(LPCTSTR)m_studentInfo[i].strName;
				m_listControl.SetItem(&lvItem); //

				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept;
				lvItem.iSubItem = 2;
				lvItem.pszText = (LPWSTR)(LPCTSTR)strLevel;
				m_listControl.SetItem(&lvItem); //

				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept;
				lvItem.iSubItem = 3;
				lvItem.pszText = (LPWSTR)(LPCTSTR)strSex;
				m_listControl.SetItem(&lvItem); //

				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = nDept;
				lvItem.iSubItem = 4;
				lvItem.pszText = (LPWSTR)(LPCTSTR)m_studentInfo[i].strAddress;
				m_listControl.SetItem(&lvItem); //

				nDept++;
			}
		}
	}

	((CButton*)GetDlgItem(IDC_BUTTON_INSERT))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_MODIFY))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(FALSE);


	UpdateData(FALSE);
	*pResult = 0;
}


void CMy20111688E7Dlg::OnItemchangedListControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nSelectedItem = pNMLV->iItem;

	m_selectList = true;

		((CButton*)GetDlgItem(IDC_BUTTON_INSERT))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_MODIFY))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(TRUE);

	*pResult = 0;
}


void CMy20111688E7Dlg::OnClickedButtonModify()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	CStudentDialog studentDialog;
	// 1. 리스트에 있는 값 -> 학생 수정 다이얼로그에 저장 
	studentDialog.m_strNumber = m_listControl.GetItemText(m_nSelectedItem,0); // 학번
	studentDialog.m_strName = m_listControl.GetItemText(m_nSelectedItem,1); // 이름
	if(m_listControl.GetItemText(m_nSelectedItem,2) == "1학년") //1학년 인경우
	{
		studentDialog.m_intLevel = 0;
	}
	else if(m_listControl.GetItemText(m_nSelectedItem,2) == "2학년") //2학년 인경우
	{
		studentDialog.m_intLevel = 1;
	}
	else if(m_listControl.GetItemText(m_nSelectedItem,2) == "3학년") //1학년 인경우
	{
		studentDialog.m_intLevel = 2;
	}
	else // 4학년인경우
	{
		studentDialog.m_intLevel = 3;
	}
	if(m_listControl.GetItemText(m_nSelectedItem,3) == "남자") // 성별
	{
		studentDialog.m_intSex = 0;
	}
	else if(m_listControl.GetItemText(m_nSelectedItem,3) == "여자") 
	{
		studentDialog.m_intSex = 1;
	}
	studentDialog.m_strAddress = m_listControl.GetItemText(m_nSelectedItem,4); // 주소

if(studentDialog.DoModal() == IDOK) // 2. 수정한 학생 수정 다이얼로그의 값 선택한 리스트에 대입
{
		LV_ITEM lvItem;

				CString strLevel;
				if(studentDialog.m_intLevel==0)
				{
					strLevel.Format(_T("1학년"));
				}
				else if(studentDialog.m_intLevel==1)
				{
					strLevel.Format(_T("2학년"));
				}
				else if(studentDialog.m_intLevel==2)
				{
					strLevel.Format(_T("3학년"));
				}
				else if(studentDialog.m_intLevel==3)
				{
					strLevel.Format(_T("4학년"));
				}
				
				CString strSex;
				if(studentDialog.m_intSex==0)
				{
					strSex.Format(_T("남자"));
				}
				else if(studentDialog.m_intSex==1)
				{
					strSex.Format(_T("여자"));
				}

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = m_nSelectedItem;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR)(LPCTSTR)studentDialog.m_strNumber;
		m_listControl.SetItem(&lvItem);

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = m_nSelectedItem;
		lvItem.iSubItem = 1;
		lvItem.pszText = (LPWSTR)(LPCTSTR)studentDialog.m_strName;
		m_listControl.SetItem(&lvItem);

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = m_nSelectedItem;
		lvItem.iSubItem = 2;
		lvItem.pszText = (LPWSTR)(LPCTSTR)strLevel;
		m_listControl.SetItem(&lvItem);

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = m_nSelectedItem;
		lvItem.iSubItem = 3;
		lvItem.pszText = (LPWSTR)(LPCTSTR)strSex;
		m_listControl.SetItem(&lvItem);

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = m_nSelectedItem;
		lvItem.iSubItem = 4;
		lvItem.pszText = (LPWSTR)(LPCTSTR)studentDialog.m_strAddress;
		m_listControl.SetItem(&lvItem);

	if(!studentDialog.m_strNumber.IsEmpty() && !studentDialog.m_strName.IsEmpty() && !studentDialog.m_strAddress)
	{
		AfxMessageBox(_T("알맞은 데이터를 입력하세요"));
		return;
	}

}
	for(int i=0; i<m_nCount; i++) // 3. 수정한 리스트 정보 -> 학생 배열 구조체에도 적용( if 학번 이용 ) 
	{
		if(m_listControl.GetItemText(m_nSelectedItem,0) == m_studentInfo[i].strID) // 학번이 같다면
		{
			m_studentInfo[i].strName = studentDialog.m_strName;
			m_studentInfo[i].nGrade = studentDialog.m_intLevel;
			m_studentInfo[i].nSex = studentDialog.m_intSex;
			m_studentInfo[i].strAddress = studentDialog.m_strAddress;
		}
	}

	((CButton*)GetDlgItem(IDC_BUTTON_INSERT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_MODIFY))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(FALSE);

	m_selectTree = false;
	m_selectList = false;

	UpdateData(FALSE);
}


void CMy20111688E7Dlg::OnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	

if(m_deleteDialog.DoModal() == IDOK)
{
	for(int i=0; i<m_nCount; i++)
	{
		if(m_listControl.GetItemText(m_nSelectedItem,0) == m_studentInfo[i].strID) // 학번이 같다면 student 삭제
		{
			int j;
			for(j=i; j<m_nCount-1; j++)
			{
				m_studentInfo[j].strDept = m_studentInfo[j+1].strDept;
				m_studentInfo[j].strID = m_studentInfo[j+1].strID;
				m_studentInfo[j].strName = m_studentInfo[j+1].strName;
				m_studentInfo[j].nGrade = m_studentInfo[j+1].nGrade;
				m_studentInfo[j].nSex = m_studentInfo[j+1].nSex;
				m_studentInfo[j].strAddress = m_studentInfo[j+1].strAddress;
			}
			m_studentInfo[j].strDept.Empty();
			m_studentInfo[j].strID.Empty(); 
			m_studentInfo[j].strName.Empty();
			m_studentInfo[j].nGrade = 0;
			m_studentInfo[j].nSex = 0;
			m_studentInfo[j].strAddress.Empty();

			m_nCount--;
		}
	}
	
	m_listControl.DeleteItem(m_nSelectedItem);
	 // 리스트에서 삭제
}
	((CButton*)GetDlgItem(IDC_BUTTON_INSERT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_MODIFY))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(FALSE);
	
	UpdateData(FALSE);

}
