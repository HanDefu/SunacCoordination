#pragma once


// CBathroomDlg 对话框

class CBathroomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBathroomDlg)

public:
	CBathroomDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBathroomDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_BATHROOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
