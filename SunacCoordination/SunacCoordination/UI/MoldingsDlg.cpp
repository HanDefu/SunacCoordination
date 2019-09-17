// UI\MoldingsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MoldingsDlg.h"
#include "afxdialogex.h"


// CMoldingsDlg 对话框

IMPLEMENT_DYNAMIC(CMoldingsDlg, CAcUiDialog)

CMoldingsDlg::CMoldingsDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CMoldingsDlg::IDD, pParent)
{

}

CMoldingsDlg::~CMoldingsDlg()
{
}

void CMoldingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_MOLDINGS, m_preMoldings);
}


BEGIN_MESSAGE_MAP(CMoldingsDlg, CAcUiDialog)
END_MESSAGE_MAP()


// CMoldingsDlg 消息处理程序


BOOL CMoldingsDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	return TRUE;
}