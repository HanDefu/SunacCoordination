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
}


BEGIN_MESSAGE_MAP(CKitchenDlg, CDialogEx)
END_MESSAGE_MAP()


// CKitchenDlg 消息处理程序
