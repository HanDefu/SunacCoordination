// UI\AirconditionerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AirconditionerDlg.h"
#include "afxdialogex.h"


// CAirconditionerDlg 对话框

IMPLEMENT_DYNAMIC(CAirconditionerDlg, CAcUiDialog)

CAirconditionerDlg::CAirconditionerDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CAirconditionerDlg::IDD, pParent)
{

}

CAirconditionerDlg::~CAirconditionerDlg()
{
}

void CAirconditionerDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAirconditionerDlg, CAcUiDialog)
END_MESSAGE_MAP()


// CAirconditionerDlg 消息处理程序


BOOL CAirconditionerDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	//测试用
	m_preAC.SubclassDlgItem(IDC_STATIC_AC, this);
	m_preAC.SetLayoutMode(PREVIEW_LAYOUT_HORIZONTAL);
	m_preAC.SetPreview(_T("D:\\Projects\\Sunac\\Support\\Sunac2019\\LocalMode\\Window_N_7_0.dwg"));
	m_preAC.SetText(_T("预览图"));
	m_preAC.Invalidate();
	return TRUE;
}