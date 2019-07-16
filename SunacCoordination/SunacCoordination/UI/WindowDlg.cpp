// UI\WindowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/RCWindow.h"


// CWindowDlg 对话框

IMPLEMENT_DYNAMIC(CWindowDlg, CDialogEx)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWindowDlg::IDD, pParent)
{

}

CWindowDlg::~CWindowDlg()
{
}

LRESULT CWindowDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	//return FALSE;
	return TRUE;
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_WINDOW, m_preWindow);
	DDX_Control(pDX, IDC_COMBO_AREATYPE, m_areaType);
	DDX_Control(pDX, IDC_COMBO_OPENTYPE, m_openType);
	DDX_Control(pDX, IDC_COMBO_OPENAMOUNT, m_openAmount);
	DDX_Control(pDX, IDC_COMBO_OPENWIDTH, m_openWidth);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_width);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_height);
}


BEGIN_MESSAGE_MAP(CWindowDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CWindowDlg::OnBnClickedButtonInsert)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
END_MESSAGE_MAP()


// CWindowDlg 消息处理程序


BOOL CWindowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_preWindow.LoadDefaltSettings();

	m_preWindow.SetRowCount(8);
	m_preWindow.SetColumnCount(2);

	//设置预览图显示区域为3行1列
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(1);

	//设置第一列占60%，其余部分为第二列
	m_preWindow.SetHeaderWidth(_T("60;+"));

	vCString allWindowFiles;
	TY_GetAllWindowFiles(allWindowFiles);
	for (int i = 0; i < allWindowFiles.size(); i++)
	{
		m_preWindow.AddPreview(i, 0, allWindowFiles[i]); 
		m_preWindow.SetContentItemText(i, 1, _T("窗类型：双扇单开\n窗户面积：2.1\n通风量：1.6"));
	}

	
	m_preWindow.ShowPreviews();


	m_areaType.SetCurSel(0);
	m_openType.SetCurSel(0);
	m_openAmount.SetCurSel(0);
	m_openWidth.SetCurSel(0);

	m_width.SetWindowText(L"2000");
	m_height.SetWindowText(L"2000");

	return TRUE;
}

void CWindowDlg::OnBnClickedButtonInsert()
{
	ShowWindow(FALSE);

	// TODO: 在此添加控件通知处理程序代码
	vCString allWindowFiles;
	TY_GetAllWindowFiles(allWindowFiles);
	
	AcGePoint3d origin = TY_GetPoint();

	double width = 0, height = 0;
	CString str;
	m_width.GetWindowText(str);
	width = _wtof(str.GetBuffer());
	m_height.GetWindowText(str);
	height = _wtof(str.GetBuffer());

	vector<int> sels = m_preWindow.GetSelectedRows();
	if (sels.size() > 0)
	{
		RCWindow oneWindow;
		if (sels[0] == 0)
			oneWindow.Insert(allWindowFiles[0], origin, 0, L"0", 256);
		else
			oneWindow.Insert(allWindowFiles[2], origin, 0, L"0", 256);
		oneWindow.InitParametersFromDynamicBlock();
		oneWindow.SetParameter(L"H", height);
		oneWindow.SetParameter(L"W", width);
		oneWindow.RunParameters();
	}
	ShowWindow(true);
}
