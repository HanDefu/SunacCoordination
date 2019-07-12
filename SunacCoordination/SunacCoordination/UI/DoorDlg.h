#pragma once


// CDoorDlg 对话框

class CDoorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDoorDlg)

public:
	CDoorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDoorDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
