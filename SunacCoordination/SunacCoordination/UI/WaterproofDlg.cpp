// UI\WaterproofDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WaterproofDlg.h"
#include "afxdialogex.h"


// CWaterproofDlg 对话框

IMPLEMENT_DYNAMIC(CWaterproofDlg, CAcUiDialog)

CWaterproofDlg::CWaterproofDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CWaterproofDlg::IDD, pParent)
{

}

CWaterproofDlg::~CWaterproofDlg()
{
}

void CWaterproofDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaterproofDlg, CAcUiDialog)
END_MESSAGE_MAP()


// CWaterproofDlg 消息处理程序


BOOL CWaterproofDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	return TRUE;
}