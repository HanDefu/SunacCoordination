// UI\FacadeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FacadeDlg.h"
#include "afxdialogex.h"


// CFacadeDlg 对话框

IMPLEMENT_DYNAMIC(CFacadeDlg, CDialogEx)

CFacadeDlg::CFacadeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFacadeDlg::IDD, pParent)
{

}

CFacadeDlg::~CFacadeDlg()
{
}

void CFacadeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABLE_FACADE, m_tFacade);
}


BEGIN_MESSAGE_MAP(CFacadeDlg, CDialogEx)
END_MESSAGE_MAP()


// CFacadeDlg 消息处理程序


BOOL CFacadeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}