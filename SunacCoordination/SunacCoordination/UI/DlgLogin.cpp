// DlgLogin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgLogin.h"
#include "afxdialogex.h"
#include "../WebIO/WebIO.h"


// DlgLogin dialog

IMPLEMENT_DYNAMIC(DlgLogin, CAcUiDialog)

DlgLogin::DlgLogin(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(DlgLogin::IDD, pParent)
{

}

DlgLogin::~DlgLogin()
{
}

void DlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_name);
	DDX_Control(pDX, IDC_EDIT2, m_password);
}


BEGIN_MESSAGE_MAP(DlgLogin, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &DlgLogin::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgLogin message handlers


void DlgLogin::OnBnClickedOk()
{
	//Acad::ErrorStatus es = acDocManager->sendStringToExecute(curDoc(), L"REMOVEBLOCKREFDOUBLECLICK\n");
	
	m_name.GetWindowTextW(m_sName);
	m_password.GetWindowTextW(m_sPassword);
	if (!WebIO::GetInstance()->Login(m_sName, m_sPassword))
	{
		AfxMessageBox(L"用户名或密码输入错误！");
		return;
	}
	CAcUiDialog::OnOK();
}


BOOL DlgLogin::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	/*m_name.SetWindowText(L"13621367728");
	m_password.SetWindowText(L"111111");*/

	m_sName = L"LeavE";
	m_sPassword = L"123456";

	m_name.SetWindowText(m_sName);
	m_password.SetWindowText(m_sPassword);

	return TRUE;  // return TRUE unless you set the focus to a control
}
