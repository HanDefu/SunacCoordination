// UI\RailingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RailingDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
#include "RailingBaseDlg.h"

// CRailingDlg 对话框

IMPLEMENT_DYNAMIC(CRailingDlg, CDialogEx)

CRailingDlg::CRailingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRailingDlg::IDD, pParent)
{

}

CRailingDlg::~CRailingDlg()
{
}

void CRailingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RAILINGINFO, m_railingInfo);
	DDX_Control(pDX, IDC_EDIT_HOLEWIDTH, m_width);
	DDX_Control(pDX, IDC_EDIT_RAILINGHEIGHT, m_height);
	DDX_Control(pDX, IDC_EDIT_REVERSERIDGE_HEIGHT, m_reverse);
}

BEGIN_MESSAGE_MAP(CRailingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON_LIB, &CRailingDlg::OnBnClickedMfcbuttonLib)
	ON_BN_CLICKED(IDC_MFCBUTTON_SELECTLINE, &CRailingDlg::OnBnClickedMfcbuttonSelectline)
	ON_BN_CLICKED(IDC_MFCBUTTON_CLOSE, &CRailingDlg::OnBnClickedMfcbuttonClose)
END_MESSAGE_MAP()


// CRailingDlg 消息处理程序


BOOL CRailingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	vCString allRailingFiles;
	TY_GetAllKitchenFiles(allRailingFiles); //暂时用厨房图纸代替

	m_preStyle.SubclassDlgItem(IDC_STATIC_STYLE, this);
	m_preStyle.Init(theArxDLL.ModuleResourceInstance(), true);
	m_preStyle.SetDwgFile(allRailingFiles[0]);

	m_railingInfo.SetWindowText(_T("栏杆信息说明\r\n栏杆间距\r\n单元尺寸\r\n栏杆类型："));

	return TRUE;
}

void CRailingDlg::OnBnClickedMfcbuttonLib()
{
	CRailingBaseDlg dlg;
	dlg.DoModal();
}


void CRailingDlg::OnBnClickedMfcbuttonSelectline()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CRailingDlg::OnBnClickedMfcbuttonClose()
{
	// TODO: 在此添加控件通知处理程序代码
}
