// UI\WindowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowDlg.h"
#include "afxdialogex.h"


// CWindowDlg 对话框

IMPLEMENT_DYNAMIC(CWindowDlg, CDialogEx)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWindowDlg::IDD, pParent)
{

}

CWindowDlg::~CWindowDlg()
{
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWindowDlg, CDialogEx)
END_MESSAGE_MAP()


// CWindowDlg 消息处理程序
