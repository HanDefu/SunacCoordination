#pragma once
#include "afxwin.h"
#include "ComFun_MFC.h"
#include "GridCtrlWithPreview.h"
#include "../Object/Railing/AttrRailing.h"
#include "D:\vs2010\VC\atlmfc\include\afxwin.h"
#include "D:\vs2010\VC\atlmfc\include\afxwin.h"

// CRailingDlg 对话框

class CRailingDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CRailingDlg)

public:
	CRailingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRailingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RAILING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	//非模态对话框相关函数
	virtual void PostNcDestroy();
	afx_msg void OnClose();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01

	virtual BOOL PreTranslateMessage(MSG *pMsg); //不响应回车

public:
	afx_msg void OnBnClickedInsertToCAD();
	afx_msg void OnBnClickedButtonSelectline();
	afx_msg void OnCbnSelchangeComboRailingtype();
	afx_msg void OnBnClickedCheckAutoindex();
	afx_msg void OnSelChangedPreview(NMHDR *pNMHDR, LRESULT *pResult);
	void UpdateRailingToGrid(eRailingType p_railetype);
	CString RailingSize(int i);

	void SetEditMode(AcDbBlockReference* pBlock);

	CGridCtrlWithPreview m_preRailing;
	CComboBox m_comboRailingType;
	int m_height;// 栏杆高度	
	int m_width;// 栏杆长度
	CEdit m_editRailingID;
	CString m_sRailingId;
	BOOL m_bRailingAutoName;// 自动编号

	AcDbBlockReference* m_pCurEdit;
	CComboBox m_comboViewDir;

	afx_msg void OnCbnSelchangeComboViewdir();
};

extern CRailingDlg* g_railingDlg;
void OpenRailingDlg(AcDbBlockReference* pCurEdit = NULL);
BOOL CloseRailingDlg();