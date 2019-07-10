#pragma once


// CKitchenDlg 对话框

class CKitchenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKitchenDlg)

public:
	CKitchenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKitchenDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_KITCHEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
