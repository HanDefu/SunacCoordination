#pragma once
#include "afxwin.h"


// DlgLogin dialog

class DlgLogin : public CAcUiDialog
{
	DECLARE_DYNAMIC(DlgLogin)

public:
	DlgLogin(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgLogin();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_sPassword;
	CString m_sUserName;
	BOOL m_bSavePwd;
};
