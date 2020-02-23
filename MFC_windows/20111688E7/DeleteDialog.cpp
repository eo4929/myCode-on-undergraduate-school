// DeleteDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "20111688E7.h"
#include "DeleteDialog.h"
#include "afxdialogex.h"


// CDeleteDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDeleteDialog, CDialogEx)

CDeleteDialog::CDeleteDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeleteDialog::IDD, pParent)
{

}

CDeleteDialog::~CDeleteDialog()
{
}

void CDeleteDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDeleteDialog, CDialogEx)
END_MESSAGE_MAP()


// CDeleteDialog 메시지 처리기입니다.
