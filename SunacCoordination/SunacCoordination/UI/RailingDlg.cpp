// UI\RailingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RailingDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
//#include "RailingBaseDlg.h"
#include "../Object/RCRailing.h"


// CRailingDlg 对话框

IMPLEMENT_DYNAMIC(CRailingDlg, CAcUiDialog)

CRailingDlg::CRailingDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CRailingDlg::IDD, pParent)
{

}

CRailingDlg::~CRailingDlg()
{
}

void CRailingDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT_RAILINGINFO, m_railingInfo);
	DDX_Control(pDX, IDC_EDIT_HOLEWIDTH, m_width);
	DDX_Control(pDX, IDC_EDIT_RAILINGHEIGHT, m_height);
	DDX_Control(pDX, IDC_EDIT_REVERSERIDGE_HEIGHT, m_reverse);
	DDX_Control(pDX, IDC_PREVIEW_RAILING, m_preRailing);
	DDX_Control(pDX, IDC_COMBO_RAILINGTYPE, m_type);
}

BEGIN_MESSAGE_MAP(CRailingDlg, CAcUiDialog)
	//ON_BN_CLICKED(IDC_MFCBUTTON_LIB, &CRailingDlg::OnBnClickedMfcbuttonLib)
	ON_BN_CLICKED(IDC_BUTTON_INSERTRAILING, &CRailingDlg::OnBnClickedMfcbuttonSelectline)
	//ON_BN_CLICKED(IDC_MFCBUTTON_CLOSE, &CRailingDlg::OnBnClickedMfcbuttonClose)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
END_MESSAGE_MAP()


// CRailingDlg 消息处理程序

LRESULT CRailingDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	//return FALSE;
	return TRUE;
}

BOOL CRailingDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	vCString allRailingFiles;
	TY_GetAllTieYiLanGanFiles(allRailingFiles);
	m_selectedFile = allRailingFiles[0];

	//m_preStyle.SubclassDlgItem(IDC_STATIC_STYLE, this);
	//m_preStyle.Init(theArxDLL.ModuleResourceInstance(), true);
	//m_preStyle.SetDwgFile(m_selectedFile);

	//m_railingInfo.SetWindowText(_T("栏杆信息说明:\r\n栏杆间距:\r\n单元尺寸:\r\n栏杆类型："));

	m_width.SetWindowTextW(_T("2500"));
	m_height.SetWindowTextW(_T("1100"));
	m_reverse.SetWindowTextW(_T("200"));
	m_type.SetCurSel(0);

	//((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_LIB))->SetImage(IDB_BITMAP37);
	//((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_CANCEL))->SetImage(IDB_BITMAP37);
	return TRUE;
}

/*
void CRailingDlg::OnBnClickedMfcbuttonLib()
{
	CAcModuleResourceOverride resOverride;

	//Memory freed on PostNcDestroy(call delete this;) or cancel function.
	CRailingBaseDlg * pDlg = new CRailingBaseDlg(acedGetAcadFrame());
	pDlg->Create(IDD_DIALOG_RAILINGBASE);
	pDlg->SetParent(this);
	pDlg->ShowWindow(SW_SHOW);
}*/
/*

void CRailingDlg::UpdateSelectFile(CString selectFile)
{
	if (selectFile.GetLength() > 0)
	{
		m_preStyle.SetDwgFile(selectFile);
		m_selectedFile = selectFile;
	}
}
*/

void CRailingDlg::OnBnClickedMfcbuttonSelectline()
{
	ShowWindow(FALSE);

	AcGePoint3d pnt1, pnt2;
	TY_GetTwoPoints(pnt1, pnt2);
	if (m_selectedFile.GetLength() > 0)
	{
		RCRailing oneRailing;
		oneRailing.InsertRailing(pnt1, pnt2, m_selectedFile);
	}
	ShowWindow(true);
	OnOK();
}


/*
void CRailingDlg::OnBnClickedMfcbuttonClose()
{
	OnOK();
}*/
