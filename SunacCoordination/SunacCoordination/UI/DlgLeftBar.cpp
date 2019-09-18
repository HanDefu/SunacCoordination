// DlgLeftBar.cpp : implementation file
//

#include "stdafx.h"
#include "../Command/Command.h"
#include "../Command/CommandWindowDetail.h"
#include "../Command/CommandWindowTable.h"
#include "DlgLeftBar.h"
#include "afxdialogex.h"


// DlgLeftBar dialog

IMPLEMENT_DYNAMIC(DlgLeftBar, CAcUiDialog)

DlgLeftBar::DlgLeftBar(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(DlgLeftBar::IDD, pParent)
{

}

DlgLeftBar::~DlgLeftBar()
{
}

void DlgLeftBar::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_MENU, m_treeCtrlMenu);
	DDX_Control(pDX, IDC_TAB_LEFTBAR, m_tab);
}


BEGIN_MESSAGE_MAP(DlgLeftBar, CAcUiDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_MENU_CLICKED, OnClickedPopupMenu)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LEFTBAR, OnTabSelChanged)
END_MESSAGE_MAP()


// DlgLeftBar message handlers


int DlgLeftBar::FillTreeItem()
{
	m_treeCtrlMenu.InsertItem(_T("标准化平面库"));
	m_treeCtrlMenu.InsertItem(_T("标准化立面库"));

	HTREEITEM hItem = m_treeCtrlMenu.InsertItem(_T("标准部品库"));
	m_treeCtrlMenu.InsertItem(_T("门窗"), hItem);
	m_treeCtrlMenu.InsertItem(_T("栏杆"), hItem);
	m_treeCtrlMenu.InsertItem(_T("外墙涂料"), hItem);
	m_treeCtrlMenu.InsertItem(_T("屋面瓦"), hItem);

	hItem = m_treeCtrlMenu.InsertItem(_T("标准空间模块库"));
	m_treeCtrlMenu.InsertItem(_T("厨卫"), hItem);
	m_treeCtrlMenu.InsertItem(_T("空调"), hItem);
	m_treeCtrlMenu.InsertItem(_T("大堂"), hItem);
	m_treeCtrlMenu.InsertItem(_T("玄关"), hItem);

	m_treeCtrlMenu.InsertItem(_T("标准构造"));
	m_treeCtrlMenu.InsertItem(_T("标准审图"));

	//绑定弹出菜单
	m_treeCtrlMenu.SetMenuID(IDR_LEFTBAR_MENU);
	m_treeCtrlMenu.SetMenuWidth(150);

	return 0;
}

LRESULT DlgLeftBar::OnClickedPopupMenu(WPARAM mID, LPARAM notUsed)
{
	switch (mID)
	{
	case ID_COMMAND_WINDOW:
		CMD_SUNACWINDOW();
		break;
	case ID_COMMAND_WINDOWQUANTITY:
		CMD_SUNACSTATISTICS();
		break;
	case ID_COMMAND_WINDOWTABLE:
		CMD_SUNACWINDOWTable();
		break;
	case ID_COMMAND_WINDOWDETAIL:
		CMD_SUNACWINDOWDetail();
		break;
	case ID_COMMAND_BATHROOM:
		CMD_SUNACBATHROOM();
		break;
	case ID_COMMAND_KITCHEN:
		CMD_SUNACKITCHEN();
		break;
	case ID_COMMAND_KITCHENQUANTITY:
		CMD_SUNACSTATISTICS();
		break;
	case ID_COMMAND_RAILING:
		CMD_SUNACRAILING();
		break;
	case ID_COMMAND_AIRCONDITIONER:
		CMD_SUNACAIRCONDITIONER();
		break;
	default:
		break;
	}

	return 0;
}

void DlgLeftBar::OnSize(UINT nType, int cx, int cy)
{
	CAcUiDialog::OnSize(nType, cx, cy);
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);
	rect.DeflateRect(5, 5);
	m_tab.SetItemSize(CSize(rect.Width()/2 - 2, 30));
	m_tab.MoveWindow(rect.left, rect.top, rect.Width(), 30);
	rect.top += 30;
	m_treeCtrlMenu.MoveWindow(rect);
}

void DlgLeftBar::OnTabSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	if (m_tab.GetCurSel() == 0)
		m_treeCtrlMenu.ShowWindow(SW_SHOW);
	else
		m_treeCtrlMenu.ShowWindow(SW_HIDE);
}

BOOL DlgLeftBar::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	m_tab.InsertItem(0, _T("标准库"));
	m_tab.InsertItem(1, _T("项目管理"));
	m_tab.SetCurSel(0);
	m_treeCtrlMenu.SetItemHeight(30);
	FillTreeItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
