// UI\FacadeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FacadeDlg.h"
#include "afxdialogex.h"


// CFacadeDlg 对话框

IMPLEMENT_DYNAMIC(CFacadeDlg, CAcUiDialog)

CFacadeDlg::CFacadeDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CFacadeDlg::IDD, pParent)
{

}

CFacadeDlg::~CFacadeDlg()
{
}

void CFacadeDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABLE_FACADE, m_tFacade);
}


BEGIN_MESSAGE_MAP(CFacadeDlg, CAcUiDialog)
END_MESSAGE_MAP()


// CFacadeDlg 消息处理程序


BOOL CFacadeDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	return TRUE;
}