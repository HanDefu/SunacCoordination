#pragma once
#include "gridctrlwithpreview.h"
#include "afxcmn.h"


// CRailingBaseDlg 对话框

class CRailingBaseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRailingBaseDlg)

public:
	CRailingBaseDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRailingBaseDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RAILINGBASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	int InitTree();
	int FillTreeItem();
	int InitLib();

	DECLARE_MESSAGE_MAP()

private:

	CGridCtrlWithPreview m_libPreview;
	CTreeCtrl m_category;
	CImageList m_treeImages;
	HTREEITEM m_hroot;
	HTREEITEM m_hlibTieYi;
	HTREEITEM m_hlibBoLi;

	
};
