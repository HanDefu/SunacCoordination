// DlgLogin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgLogin.h"
#include "afxdialogex.h"
#include "../WebIO/WebIO.h"
#include "..\Common\TYFormula.h"
#include "..\Common\ComFun_Sunac.h"
#include "..\GlobalSetting.h"


// DlgLogin dialog

IMPLEMENT_DYNAMIC(DlgLogin, CAcUiDialog)

DlgLogin::DlgLogin(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(DlgLogin::IDD, pParent)
	, m_bSavePwd(FALSE)
	, m_sUserName(_T(""))
{

}

DlgLogin::~DlgLogin()
{
}

void DlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_IsSavePassword, m_bSavePwd);
	DDX_Text(pDX, IDC_EDIT1, m_sUserName);
	DDX_Text(pDX, IDC_EDIT2, m_sPassword);
}


BEGIN_MESSAGE_MAP(DlgLogin, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &DlgLogin::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgLogin message handlers


void DlgLogin::OnBnClickedOk()
{	
	UpdateData();

	if (!WebIO::GetInstance()->Login(m_sUserName, m_sPassword))
	{
		AfxMessageBox(L"用户名或密码输入错误！");
		return;
	}

	GlobalSetting::GetInstance()->m_bRememberPwd = m_bSavePwd ? true :false;
	GlobalSetting::GetInstance()->m_userName = m_sUserName;
	GlobalSetting::GetInstance()->m_password = m_sPassword;

	CAcUiDialog::OnOK();
}


BOOL DlgLogin::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	
	m_bSavePwd = GlobalSetting::GetInstance()->m_bRememberPwd;
	m_sUserName = GlobalSetting::GetInstance()->m_userName;
	m_sPassword = GlobalSetting::GetInstance()->m_password;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}
