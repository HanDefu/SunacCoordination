#pragma once


// CMoldingsDlg 对话框

class CMoldingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMoldingsDlg)

public:
	CMoldingsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMoldingsDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MOLDINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
