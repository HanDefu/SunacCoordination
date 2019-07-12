// UI\DoorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DoorDlg.h"
#include "afxdialogex.h"


// CDoorDlg 对话框

IMPLEMENT_DYNAMIC(CDoorDlg, CDialogEx)

CDoorDlg::CDoorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDoorDlg::IDD, pParent)
{

}

CDoorDlg::~CDoorDlg()
{
}

void CDoorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDoorDlg, CDialogEx)
END_MESSAGE_MAP()


// CDoorDlg 消息处理程序


BOOL CDoorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}