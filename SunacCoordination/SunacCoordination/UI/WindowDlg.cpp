// UI\WindowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowDlg.h"
#include "afxdialogex.h"


// CWindowDlg 对话框

IMPLEMENT_DYNAMIC(CWindowDlg, CDialogEx)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWindowDlg::IDD, pParent)
{

}

CWindowDlg::~CWindowDlg()
{
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_WINDOW, m_preWindow);
}


BEGIN_MESSAGE_MAP(CWindowDlg, CDialogEx)
END_MESSAGE_MAP()


// CWindowDlg 消息处理程序


BOOL CWindowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_preWindow.LoadDefaltSettings();

	m_preWindow.SetRowCount(4);
	m_preWindow.SetColumnCount(2);
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(2);

	m_preWindow.AddPreview(0, 0, _T("C:\\Daqo\\LogicDwg\\1开口三角.dwg")); //测试用dwg文件
	m_preWindow.SetContentItemText(0, 1, _T("窗类型：双扇单开\n窗户面积：2.1\n通风量：1.6"));
	m_preWindow.ShowPreviews();

	return TRUE;
}