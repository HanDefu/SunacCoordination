// UI\KitchenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KitchenDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/RCKitchen.h"
// CKitchenDlg 对话框

IMPLEMENT_DYNAMIC(CKitchenDlg, CDialogEx)

CKitchenDlg::CKitchenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKitchenDlg::IDD, pParent)
{

}

CKitchenDlg::~CKitchenDlg()
{
}

LRESULT CKitchenDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	//return FALSE;
	return TRUE;
}

void CKitchenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_KITCHEN, m_preKitchen);
	DDX_Control(pDX, IDC_COMBO_FLOORRANGE, m_floorRange);
	DDX_Control(pDX, IDC_COMBO_BASINTYPE, m_basinType);
	DDX_Control(pDX, IDC_COMBO_BENCHWIDTH, m_benchWidth);
	DDX_Control(pDX, IDC_COMBO_FRIDGETYPE, m_frigeType);
	DDX_Control(pDX, IDC_COMBO_PROJECTLEVEL, m_projectLevel);
}

BEGIN_MESSAGE_MAP(CKitchenDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CKitchenDlg::OnBnClickedOk)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON1, &CKitchenDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_KITCHENRANGE, &CKitchenDlg::OnBnClickedButtonKitchenrange)
END_MESSAGE_MAP()


// CKitchenDlg 消息处理程序


BOOL CKitchenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_preKitchen.LoadDefaltSettings();

	m_preKitchen.SetRowCount(8);
	m_preKitchen.SetColumnCount(2);

	//设置预览图显示区域为3行1列
	m_preKitchen.SetDisplayRows(3);
	m_preKitchen.SetDisplayColumns(1);

	//设置第一列占60%，其余部分为第二列
	m_preKitchen.SetHeaderWidth(_T("60;+"));

	vCString allFiles;
	TY_GetAllKitchenFiles(allFiles);
	for (int i = 0; i < allFiles.size(); i++)
	{
		m_preKitchen.AddPreview(i, 0, allFiles[i]); 
		m_preKitchen.SetContentItemText(i, 1, _T("厨房类型：KU\n厨房面积：2.1\n通风量：1.6"));
	}


	m_preKitchen.ShowPreviews();

	m_floorRange.SetCurSel(0);
	m_basinType.SetCurSel(0);
	m_benchWidth.SetCurSel(0);
	m_frigeType.SetCurSel(0);
	m_projectLevel.SetCurSel(0);

	return TRUE;
}


void CKitchenDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CKitchenDlg::OnBnClickedButton1()//插入厨房模块
{
	ShowWindow(FALSE);

	// TODO: 在此添加控件通知处理程序代码
	vCString allFiles;
	TY_GetAllKitchenFiles(allFiles);

	TYRect rect = TY_GetOneRect();

	double width = rect.GetWidth(), height = rect.GetHeight();
	AcGePoint3d origin = rect.GetLB();

	vector<int> sels = m_preKitchen.GetSelectedRows();
	if (sels.size() > 0)
	{
		RCKitchen oneKitchen;
		if (sels[0] == 0)
			oneKitchen.Insert(allFiles[0], origin, 0, L"0", 256);
		else
			oneKitchen.Insert(allFiles[1], origin, 0, L"0", 256);
		oneKitchen.InitParametersFromDynamicBlock();
		oneKitchen.SetParameter(L"进深", height);
		oneKitchen.SetParameter(L"开间", width);
		oneKitchen.RunParameters();
	}
	ShowWindow(true);
}

void CKitchenDlg::OnBnClickedButtonKitchenrange()
{
	// TODO: 在此添加控件通知处理程序代码
}
