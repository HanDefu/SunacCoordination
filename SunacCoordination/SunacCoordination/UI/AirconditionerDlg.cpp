// UI\AirconditionerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AirconditionerDlg.h"
#include "afxdialogex.h"


// CAirconditionerDlg 对话框

IMPLEMENT_DYNAMIC(CAirconditionerDlg, CDialogEx)

CAirconditionerDlg::CAirconditionerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAirconditionerDlg::IDD, pParent)
{

}

CAirconditionerDlg::~CAirconditionerDlg()
{
}

void CAirconditionerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAirconditionerDlg, CDialogEx)
END_MESSAGE_MAP()


// CAirconditionerDlg 消息处理程序
