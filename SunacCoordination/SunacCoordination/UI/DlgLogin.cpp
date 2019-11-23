// DlgLogin.cpp : implementation file
//

#include "stdafx.h"
#include "DlgLogin.h"
#include "afxdialogex.h"
#include "../WebIO/WebIO.h"
#include "..\Common\TYFormula.h"
#include "..\Common\ComFun_Sunac.h"


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
	DDX_Control(pDX, IDC_CHECK_IsSavePassword, m_IsSavePassword);
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

	CString sSaveUserInfoFilePath = TY_GetDataFilePath() + _T("账号密码表.xlsx");
	Excel::CExcelUtil xls;
	bool bSuc = xls.OpenExcel(sSaveUserInfoFilePath); //打开表格
	if (bSuc==false)
	{
		AfxMessageBox(_T("无法打开 账号密码表.xlsx"));
		return;
	}

	xls.SetVisible(false); 
	xls.SetCellValue(2, 1, m_sName);
	if (m_IsSavePassword.GetCheck())
	{
		xls.SetCellValue(2, 2, m_sPassword);
	}
	else xls.SetCellValue(2, 2, L"");
	xls.SaveExcel();

	CAcUiDialog::OnOK();
}


BOOL DlgLogin::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	/*m_name.SetWindowText(L"13621367728");
	m_password.SetWindowText(L"111111");*/

	CString sSaveUserInfoFilePath = TY_GetDataFilePath() + _T("账号密码表.xlsx");
	Excel::CExcelUtil xls;
	bool bSuc = xls.OpenExcel(sSaveUserInfoFilePath); //打开表格
	if (bSuc==false)
	{
		AfxMessageBox(_T("无法打开 账号密码表.xlsx"));
		return FALSE;
	}

	xls.SetVisible(false); 
	m_sName = xls.GetCellValue(2, 1);
	m_sPassword = xls.GetCellValue(2, 2);
	xls.CloseExcel();

	m_name.SetWindowText(m_sName);
	m_password.SetWindowText(m_sPassword);

	return TRUE;  // return TRUE unless you set the focus to a control
}
