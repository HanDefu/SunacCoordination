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


	HTREEITEM m_treeItemWindows; //√≈¥∞
	HTREEITEM m_treeItemWindowsQuantity; //√≈¥∞À„¡øQuantitysurvey
	HTREEITEM m_treeItemDoor; //√≈¥∞
	HTREEITEM m_treeItemDoorQuantity; //√≈¥∞

	HTREEITEM m_treeItemKitchen; //≥¯Œ¿
	HTREEITEM m_treeItemKitchenQuantity; //≥¯Œ¿
	HTREEITEM m_treeItemBathroom; //≥¯Œ¿
	HTREEITEM m_treeItemBathroomQuantity; //≥¯Œ¿

	HTREEITEM m_treeItemRailing;//¿∏∏À
	HTREEITEM m_treeItemAircondition; //ø’µ˜
	HTREEITEM m_treeItemWaterproof; //∑¿ÀÆ
	HTREEITEM m_treeItemFilling; //ÃÓ≥‰
	HTREEITEM m_treeItemfacade; //¡¢√Ê


public:
	afx_msg void OnTvnSelchangedTreeMenu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTreeMenu(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};
