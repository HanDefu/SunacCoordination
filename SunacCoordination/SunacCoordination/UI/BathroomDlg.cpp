// UI\BathroomDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BathroomDlg.h"
#include "afxdialogex.h"


// CBathroomDlg 对话框

IMPLEMENT_DYNAMIC(CBathroomDlg, CAcUiDialog)

CBathroomDlg::CBathroomDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CBathroomDlg::IDD, pParent)
{

}

CBathroomDlg::~CBathroomDlg()
{
}

void CBathroomDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_BATHROOM, m_preBathroom);
}


BEGIN_MESSAGE_MAP(CBathroomDlg, CAcUiDialog)
END_MESSAGE_MAP()


// CBathroomDlg 消息处理程序


BOOL CBathroomDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	return TRUE;
}