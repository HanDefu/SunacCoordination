#pragma once
#include "GridCtrlWithPreview.h"
#include "afxwin.h"

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
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	DECLARE_MESSAGE_MAP()

public:
	//afx_msg void OnBnClickedMfcbuttonLib();
	afx_msg void OnBnClickedMfcbuttonSelectline();
	//afx_msg void OnBnClickedMfcbuttonClose();
	//void UpdateSelectFile(CString selectFile);

	//CGsPreviewCtrl m_preStyle; //花样选择预览图
	//CEdit m_railingInfo;
	CGridCtrlWithPreview m_preRailing;
	CEdit m_width; //洞口宽度
	CEdit m_height; //栏杆总高
	CEdit m_reverse; //反坎高度
	CString m_selectedFile;
};
