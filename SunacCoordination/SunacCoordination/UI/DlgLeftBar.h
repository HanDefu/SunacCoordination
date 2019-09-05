#pragma once

#include "../res/resource.h"
#include "afxcmn.h"
// DlgLeftBar dialog

class DlgLeftBar : public CAcUiDialog
{
	DECLARE_DYNAMIC(DlgLeftBar)

public:
	DlgLeftBar(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgLeftBar();

// Dialog Data
	enum { IDD = IDD_DIALOG_LEFTBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	int InitTreeCtrl();
	int FillTreeItem();
	CTreeCtrl m_treeCtrlMenu;


	HTREEITEM m_treeItemWindows; //门窗
	HTREEITEM m_treeItemWindowsQuantity; //门窗算量Quantitysurvey
	HTREEITEM m_treeItemDoor; //门窗
	HTREEITEM m_treeItemDoorQuantity; //门算量
	HTREEITEM m_treeItemWindowTable; //门窗表

	HTREEITEM m_treeItemKitchen; //厨卫
	HTREEITEM m_treeItemKitchenQuantity; //厨卫
	HTREEITEM m_treeItemBathroom; //厨卫
	HTREEITEM m_treeItemBathroomQuantity; //厨卫

	HTREEITEM m_treeItemRailing;//栏杆
	HTREEITEM m_treeItemAircondition; //空调
	HTREEITEM m_treeItemWaterproof; //防水
	HTREEITEM m_treeItemFilling; //填充
	HTREEITEM m_treeItemfacade; //立面


public:
	afx_msg void OnTvnSelchangedTreeMenu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTreeMenu(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};
