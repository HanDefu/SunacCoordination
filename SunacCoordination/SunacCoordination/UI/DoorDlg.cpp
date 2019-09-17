// UI\DoorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DoorDlg.h"
#include "afxdialogex.h"


// CDoorDlg 对话框

IMPLEMENT_DYNAMIC(CDoorDlg, CAcUiDialog)

CDoorDlg::CDoorDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CDoorDlg::IDD, pParent)
{

}

CDoorDlg::~CDoorDlg()
{
}

void CDoorDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDoorDlg, CAcUiDialog)
END_MESSAGE_MAP()


// CDoorDlg 消息处理程序


BOOL CDoorDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	return TRUE;
}