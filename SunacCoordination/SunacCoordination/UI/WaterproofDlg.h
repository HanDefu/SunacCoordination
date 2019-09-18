#pragma once


// CWaterproofDlg 对话框

class CWaterproofDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CWaterproofDlg)

public:
	CWaterproofDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWaterproofDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WATERPROOF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
