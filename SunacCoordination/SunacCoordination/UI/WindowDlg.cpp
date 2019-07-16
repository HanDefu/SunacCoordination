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

	m_preWindow.SetRowCount(8);
	m_preWindow.SetColumnCount(2);

	//设置预览图显示区域为3行1列
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(1);

	//设置第一列占60%，其余部分为第二列
	m_preWindow.SetHeaderWidth(_T("60;+"));

	m_preWindow.AddPreview(0, 0, _T("E:\\Sunac_Kitchen_KUq_10_1.dwg")); //测试用dwg文件
	m_preWindow.AddPreview(1, 0, _T("E:\\1.dwg")); //测试用dwg文件
	m_preWindow.AddPreview(2, 0, _T("E:\\2.dwg")); //测试用dwg文件
	m_preWindow.AddPreview(3, 0, _T("E:\\3.dwg")); //测试用dwg文件
	m_preWindow.AddPreview(4, 0, _T("E:\\4.dwg")); //测试用dwg文件
	m_preWindow.AddPreview(5, 0, _T("E:\\5.dwg")); //测试用dwg文件
	m_preWindow.AddPreview(6, 0, _T("E:\\6.dwg")); //测试用dwg文件
	m_preWindow.AddPreview(7, 0, _T("E:\\7.dwg")); //测试用dwg文件
	m_preWindow.AddPreview(8, 0, _T("E:\\8.dwg")); //测试用dwg文件
	m_preWindow.SetContentItemText(0, 1, _T("窗类型：双扇单开\n窗户面积：2.1\n通风量：1.6"));
	m_preWindow.ShowPreviews();

	return TRUE;
}