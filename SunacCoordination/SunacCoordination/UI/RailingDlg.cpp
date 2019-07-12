// UI\RailingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RailingDlg.h"
#include "afxdialogex.h"


// CRailingDlg 对话框

IMPLEMENT_DYNAMIC(CRailingDlg, CDialogEx)

CRailingDlg::CRailingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRailingDlg::IDD, pParent)
{

}

CRailingDlg::~CRailingDlg()
{
}

void CRailingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRailingDlg, CDialogEx)
END_MESSAGE_MAP()


// CRailingDlg 消息处理程序


BOOL CRailingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}
