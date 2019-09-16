#pragma once
/*
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

	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01

	afx_msg void OnBnClickedMfcbuttonOK();
	CString GetSelectedFile(){return m_selectedFile;}
	void SetParent(CDialog * parent){m_parent = parent;}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	int InitTree();
	int FillTreeItem();
	int InitLib();
	void RefreshPreviewFiles();
	DECLARE_MESSAGE_MAP()
	
private:

	CGridCtrlWithPreview m_libPreview;
	CTreeCtrl m_category;
	CImageList m_treeImages;
	HTREEITEM m_hroot;
	HTREEITEM m_hlibTieYi;
	HTREEITEM m_hlibBoLi;

	HTREEITEM m_selItem;
	
	CString m_selectedFile;
	CDialog * m_parent;

	vCString m_allFilePathNames;
	
public:
	afx_msg void OnNMClickTreeRailingbase(NMHDR *pNMHDR, LRESULT *pResult);
};
*/
