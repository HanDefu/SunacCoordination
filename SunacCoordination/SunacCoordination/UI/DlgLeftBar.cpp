// DlgLeftBar.cpp : implementation file
//

#include "stdafx.h"
#include "../Command/Command.h"
#include "../Command/CommandWindowDetail.h"
#include "../Command/CommandWindowTable.h"
#include "DlgLeftBar.h"
#include "afxdialogex.h"


// DlgLeftBar dialog

IMPLEMENT_DYNAMIC(DlgLeftBar, CDialog)

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
}


BEGIN_MESSAGE_MAP(DlgLeftBar, CAcUiDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_MENU_CLICKED, OnClickedPopupMenu)
END_MESSAGE_MAP()


// DlgLeftBar message handlers


int DlgLeftBar::FillTreeItem()
{
	/*HTREEITEM  hItem= m_treeCtrlMenu.InsertItem(_T("门窗"));

	m_treeItemWindows = m_treeCtrlMenu.InsertItem(_T("外窗"), hItem);
	m_treeItemWindowsQuantity = m_treeCtrlMenu.InsertItem(_T("外窗算量"), hItem);

	m_treeItemDoor = m_treeCtrlMenu.InsertItem(_T("门"), hItem);
	m_treeItemDoorQuantity = m_treeCtrlMenu.InsertItem(_T("门算量"), hItem);
	m_treeItemWindowTable = m_treeCtrlMenu.InsertItem(_T("门窗表"), hItem);
	m_treeItemWindowDetail = m_treeCtrlMenu.InsertItem(_T("门窗详图"), hItem);


	//////////////////////////////////////////////////////////////////////////
	hItem = m_treeCtrlMenu.InsertItem(_T("厨卫"));

	m_treeItemKitchen = m_treeCtrlMenu.InsertItem(_T("厨房"), hItem);
	m_treeItemKitchenQuantity = m_treeCtrlMenu.InsertItem(_T("厨房算量"), hItem);
	m_treeItemBathroom = m_treeCtrlMenu.InsertItem(_T("卫生间"), hItem);
	m_treeItemBathroomQuantity = m_treeCtrlMenu.InsertItem(_T("卫生间算量"), hItem);

	//////////////////////////////////////////////////////////////////////////
	m_treeItemRailing = m_treeCtrlMenu.InsertItem(_T("栏杆"));

	hItem = m_treeCtrlMenu.InsertItem(_T("空调"));

	hItem = m_treeCtrlMenu.InsertItem(_T("防水"));

	hItem = m_treeCtrlMenu.InsertItem(_T("填充"));

	hItem = m_treeCtrlMenu.InsertItem(_T("立面"));*/

	m_treeCtrlMenu.SetItemHeight(30);
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


/*

void DlgLeftBar::OnTvnSelchangedTreeMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	HTREEITEM curHItem = pNMTreeView->itemNew.hItem;

	if (m_treeItemWindows==curHItem)
	{
		CMD_SUNACWINDOW();
	}
	else if (m_treeItemWindowsQuantity==curHItem)
	{
		CMD_SUNACSTATISTICS();
	}
	else if (m_treeItemKitchen == curHItem)
	{
		CMD_SUNACKITCHEN();
	}
	else if (m_treeItemKitchenQuantity == curHItem)
	{
		CMD_SUNACSTATISTICS();
	}
	else if (m_treeItemRailing == curHItem)
	{
		CMD_SUNACRAILING();
	}
	else if (m_treeItemWindowTable == curHItem)
	{
		CMD_SUNACWINDOWTable();
	}
	else if (m_treeItemWindowDetail == curHItem)
	{
		CMD_SUNACWINDOWDetail();
	}


	*pResult = 0;
}
*/

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
	case ID_COMMAND_KITCHEN:
		CMD_SUNACKITCHEN();
		break;
	case ID_COMMAND_KITCHENQUANTITY:
		CMD_SUNACSTATISTICS();
		break;
	case ID_COMMAND_RAILING:
		CMD_SUNACRAILING();
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
	m_treeCtrlMenu.MoveWindow(rect);
}

BOOL DlgLeftBar::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_treeCtrlMenu.SetItemHeight(40);

	FillTreeItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
