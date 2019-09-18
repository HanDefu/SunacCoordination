// UI\FillingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FillingDlg.h"
#include "afxdialogex.h"


// CFillingDlg 对话框

IMPLEMENT_DYNAMIC(CFillingDlg, CAcUiDialog)

CFillingDlg::CFillingDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CFillingDlg::IDD, pParent)
{

}

CFillingDlg::~CFillingDlg()
{
}

void CFillingDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_FILLING, m_preFilling);
}


BEGIN_MESSAGE_MAP(CFillingDlg, CAcUiDialog)
END_MESSAGE_MAP()


// CFillingDlg 消息处理程序


BOOL CFillingDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	return TRUE;
}