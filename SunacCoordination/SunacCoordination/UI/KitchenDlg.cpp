// UI\KitchenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KitchenDlg.h"
#include "afxdialogex.h"


// CKitchenDlg 对话框

IMPLEMENT_DYNAMIC(CKitchenDlg, CDialogEx)

CKitchenDlg::CKitchenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKitchenDlg::IDD, pParent)
{

}

CKitchenDlg::~CKitchenDlg()
{
}

void CKitchenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_KITCHEN, m_preKitchen);
}

BEGIN_MESSAGE_MAP(CKitchenDlg, CDialogEx)
END_MESSAGE_MAP()


// CKitchenDlg 消息处理程序


BOOL CKitchenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}
