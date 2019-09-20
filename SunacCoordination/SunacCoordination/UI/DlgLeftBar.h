#pragma once

#include "../res/resource.h"
#include "afxcmn.h"
#include "TreeCtrlEx.h"
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

	int FillTreeItem();
	CTreeCtrlEx m_treeCtrlMenu;

public:
	afx_msg LRESULT OnClickedPopupMenu(WPARAM mID, LPARAM notUsed);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTabSelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	CTabCtrl m_tab;
};