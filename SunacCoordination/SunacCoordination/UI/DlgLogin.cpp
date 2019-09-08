// DlgLogin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgLogin.h"
#include "afxdialogex.h"


// DlgLogin dialog

IMPLEMENT_DYNAMIC(DlgLogin, CDialogEx)

DlgLogin::DlgLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgLogin::IDD, pParent)
{

}

DlgLogin::~DlgLogin()
{
}

void DlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_name);
	DDX_Control(pDX, IDC_EDIT2, m_password);
}


BEGIN_MESSAGE_MAP(DlgLogin, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgLogin::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgLogin message handlers


void DlgLogin::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


BOOL DlgLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_name.SetWindowText(L"13621367728");
	m_password.SetWindowText(L"111111");

	return TRUE;  // return TRUE unless you set the focus to a control
}
