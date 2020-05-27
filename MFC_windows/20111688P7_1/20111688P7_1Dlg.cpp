
// 20111688P7_1Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "20111688P7_1.h"
#include "20111688P7_1Dlg.h"
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


// CMy20111688P7_1Dlg 대화 상자




CMy20111688P7_1Dlg::CMy20111688P7_1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy20111688P7_1Dlg::IDD, pParent)
	, m_nSelectedItem(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strDept = _T("");
	m_strID = _T("");
	m_strName = _T("");
	m_strSelectItem = _T("");
}

void CMy20111688P7_1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STUDENT, m_listStudent);
	DDX_Text(pDX, IDC_EDIT_DEPT, m_strDept);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_SELECTED_ITEM, m_strSelectItem);
}

BEGIN_MESSAGE_MAP(CMy20111688P7_1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CMy20111688P7_1Dlg::OnClickedButtonInsert)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STUDENT, &CMy20111688P7_1Dlg::OnItemchangedListStudent)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CMy20111688P7_1Dlg::OnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CMy20111688P7_1Dlg::OnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CMy20111688P7_1Dlg::OnClickedButtonReset)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, &CMy20111688P7_1Dlg::OnSelchangeComboStyle)
END_MESSAGE_MAP()


// CMy20111688P7_1Dlg 메시지 처리기

BOOL CMy20111688P7_1Dlg::OnInitDialog()
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

	// 리스트 컨트롤 쵝화
	LV_COLUMN lvColumn; // 칼럼 변수
	LPWSTR list[4] = {_T("순번"),_T("학과"),_T("학번"),_T("이름")}; //LPWSTR : w_char
	int nWidth[4] = {50,150,100,100}; // 칼럼 길이 설정

	for(int i=0; i<4; i++)
	{
		lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH; // 칼럼의 마스크 설정
		lvColumn.fmt = LVCFMT_CENTER; // 위치
		lvColumn.pszText = list[i]; // 칼럼의 텍스트
		lvColumn.iSubItem = i; // 칼럼의 부 아이템의 색인
		lvColumn.cx = nWidth[i];//칼럼의 길이 설정
		m_listStudent.InsertColumn(i,&lvColumn); // List컨트롤에 칼럼 삽입
	}

	m_listStudent.SetExtendedStyle(m_listStudent.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); // List컨트롤 스타일 변경 
	((CComboBox*)GetDlgItem(IDC_COMBO_STYLE))->SetCurSel(0); // 콤보 박스의 초기 선택값 지정

	((CButton*)GetDlgItem(IDC_BUTTON_MODIFY))->EnableWindow(FALSE); // 변경 버튼 비활성화
	((CButton*)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(FALSE); // 삭제 버튼 비활성화
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMy20111688P7_1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMy20111688P7_1Dlg::OnPaint()
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
HCURSOR CMy20111688P7_1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy20111688P7_1Dlg::OnClickedButtonInsert()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCount = m_listStudent.GetItemCount(); // List컨트롤에 삽입된 아이템의 수
	LV_ITEM lvItem; // 아이템 변수
	CString strCount; // 아이템에 들어갈 문장

	UpdateData(TRUE);

	if(!m_strDept.IsEmpty() && !m_strID.IsEmpty() && !m_strName.IsEmpty() ) // 학과,학번,이름이 모두 비어있지 않으면
	{
		lvItem.mask = LVIF_TEXT; // 아이템 설정
		lvItem.iItem = nCount; // 아이템의 삽입위치
		lvItem.iSubItem = 0; // 서브 칼럼 인덱스
		strCount.Format(_T("%d"),nCount+1); // 숫자
		lvItem.pszText = (LPWSTR)(LPCTSTR)strCount; // 숫자 삽입
		m_listStudent.InsertItem(&lvItem);

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = nCount;
		lvItem.iSubItem = 1; // 서브 칼럼 인덱스
		lvItem.pszText = (LPWSTR)(LPCTSTR)m_strDept; // 학과
		m_listStudent.SetItem(&lvItem);

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = nCount; 
		lvItem.iSubItem = 2;
		lvItem.pszText = (LPWSTR)(LPCTSTR)m_strID; // 학번
		m_listStudent.SetItem(&lvItem);

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = nCount;
		lvItem.iSubItem = 3;
		lvItem.pszText = (LPWSTR)(LPCTSTR)m_strName; // 이름
		m_listStudent.SetItem(&lvItem);

		m_strDept.Empty(); // 리스트에 삽입 후 에디트 비우기
		m_strID.Empty();
		m_strName.Empty();

		((CButton*)GetDlgItem(IDC_BUTTON_MODIFY))->EnableWindow(FALSE); // 버튼 활성화
		((CButton*)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(FALSE); // 버튼 활성화

		UpdateData(FALSE);
	}
	else
	{
		MessageBox(_T("모든 항목을 입력해 주세요"),_T("잠깐"),MB_OK);
	}
}


void CMy20111688P7_1Dlg::OnItemchangedListStudent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_nSelectedItem = pNMLV->iItem; // 선택된 리스트의 인덱스값 가져옴

	m_strSelectItem.Format(_T("%d"),m_nSelectedItem+1); // 선택된 아이템 번호 출력
	m_strDept = m_listStudent.GetItemText(m_nSelectedItem,1); // 변경될 인덱스의 데이터 가져옴
	m_strID = m_listStudent.GetItemText(m_nSelectedItem,2);
	m_strName = m_listStudent.GetItemText(m_nSelectedItem,3);

	((CButton*)GetDlgItem(IDC_BUTTON_MODIFY))->EnableWindow(TRUE); // 버튼 활성화
	((CButton*)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(TRUE); // 버튼 활성화

	UpdateData(FALSE);
	*pResult = 0;
}


void CMy20111688P7_1Dlg::OnClickedButtonModify()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CString strDept,strID,strName;
	strDept = m_strDept;
	strID = m_strID;
	strName = m_strName;
	CString strIndex;

	if(m_nSelectedItem >= 0) // 선택된 아이템이 있다면
	{
		if(!m_strDept.IsEmpty() && !m_strID.IsEmpty() && !m_strName.IsEmpty() ) // 에디트가 모두 비어있지 않다면
		{
			LV_ITEM lvItem;
			// 리스트에 해당하는 아이템 변경
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = m_nSelectedItem;
			lvItem.iSubItem = 0;
			strIndex.Format(_T("%d"),m_nSelectedItem+1);
			lvItem.pszText = (LPWSTR)(LPCTSTR)strIndex;
			m_listStudent.SetItem(&lvItem);

			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = m_nSelectedItem;
			lvItem.iSubItem = 1;
			lvItem.pszText = (LPWSTR)(LPCTSTR)strDept;
			m_listStudent.SetItem(&lvItem);

			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = m_nSelectedItem;
			lvItem.iSubItem = 2;
			lvItem.pszText = (LPWSTR)(LPCTSTR)strID;
			m_listStudent.SetItem(&lvItem);

			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = m_nSelectedItem;
			lvItem.iSubItem = 3;
			lvItem.pszText = (LPWSTR)(LPCTSTR)strName;
			m_listStudent.SetItem(&lvItem);

			m_strDept.Empty();
			m_strID.Empty();
			m_strName.Empty();

			((CButton*)GetDlgItem(IDC_BUTTON_MODIFY))->EnableWindow(FALSE); // 비활성화
			((CButton*)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(FALSE);

			UpdateData(FALSE);
		}
		else
		{
			MessageBox(_T("모든 항목을 입력해 주세요"),_T("잠깐"),MB_OK);
		}
	}
	else
	{
		MessageBox(_T("아이템을 선택하지 않았습니다."),_T("잠깐"),MB_OK);
	}
	
}


void CMy20111688P7_1Dlg::OnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSelectedItem >= 0) // 선택된 리스트가 있다면
	{
		m_listStudent.DeleteItem(m_nSelectedItem); // 삭제
		
		for(int i = m_nSelectedItem-1; i< m_listStudent.GetItemCount(); i++) // 번호 재배열하기
		{
			CString strIndex;

			strIndex.Format(_T("%d"),i+1);
			m_listStudent.SetItemText(i,0,strIndex);
		}
		
		m_strDept.Empty();
		m_strID.Empty();
		m_strName.Empty();

		((CButton*)GetDlgItem(IDC_BUTTON_MODIFY))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(FALSE);

		UpdateData(FALSE);
	}
	else
	{
		MessageBox(_T("아이템을 선택하지 않았습니다."),_T("잠깐"),MB_OK);
	}
}


void CMy20111688P7_1Dlg::OnClickedButtonReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		m_strDept.Empty();
		m_strID.Empty();
		m_strName.Empty();

		UpdateData(FALSE);
}


void CMy20111688P7_1Dlg::OnSelchangeComboStyle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int numSel = ((CComboBox*)GetDlgItem(IDC_COMBO_STYLE))->GetCurSel(); // 콤보박스에서 선택된 아이템에 맞는 인덱스를 가져옴
	long setStyle;

	setStyle = GetWindowLong(m_listStudent.m_hWnd,GWL_STYLE); // 명시된 윈도우에 대한 정보를 얻음 
	setStyle &= ~LVS_TYPEMASK;

	switch(numSel) //자세히;목록;작은 아이콘;큰 아이콘;
	{
	case 0:
		setStyle |= LVS_REPORT; // 자세히
		break;
	case 1:
		setStyle |= LVS_LIST; // 목록
		break;
	case 2:
		setStyle |= LVS_SMALLICON; // 작은 아이콘
		break;
	case 3:
		setStyle |= LVS_ICON; // 큰 아이콘
		break;
	}
	SetWindowLong(m_listStudent.m_hWnd,GWL_STYLE,setStyle); // 명시된 윈도우의 속성을 변경함
}
