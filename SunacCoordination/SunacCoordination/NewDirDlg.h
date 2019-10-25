#pragma once


// CNewDirDlg 对话框

class CNewDirDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CNewDirDlg)

public:
	CNewDirDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewDirDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NewDir };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_sNewDir;
	afx_msg void OnBnClickedOk();
	CEdit m_NewDirEdit;
};
