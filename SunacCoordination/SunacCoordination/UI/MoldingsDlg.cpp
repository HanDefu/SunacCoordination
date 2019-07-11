// UI\MoldingsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MoldingsDlg.h"
#include "afxdialogex.h"


// CMoldingsDlg 对话框

IMPLEMENT_DYNAMIC(CMoldingsDlg, CDialogEx)

CMoldingsDlg::CMoldingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMoldingsDlg::IDD, pParent)
{

}

CMoldingsDlg::~CMoldingsDlg()
{
}

void CMoldingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMoldingsDlg, CDialogEx)
END_MESSAGE_MAP()


// CMoldingsDlg 消息处理程序
