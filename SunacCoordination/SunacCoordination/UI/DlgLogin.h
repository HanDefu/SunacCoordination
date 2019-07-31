#pragma once


// DlgLogin dialog

class DlgLogin : public CDialogEx
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
};
