#pragma once


// CWaterproofDlg 对话框

class CWaterproofDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWaterproofDlg)

public:
	CWaterproofDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWaterproofDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WATERPROOF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
