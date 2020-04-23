#pragma once


// CWindowTableCheckDlg 对话框

class CWindowTableCheckDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWindowTableCheckDlg)

public:
	CWindowTableCheckDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWindowTableCheckDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WINTABLECHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnCancel();
	afx_msg void OnBnClickedButtonWintableselect();

public:
	void GetWinIdFromWinTableXData(AcDbObjectId p_tableId, vector<AcDbObjectId> &vWinIds);
	void CreateBrightBox(vector<AcDbObjectId> vWinIds); //为门窗编号创建亮框
	void DeleteBrightBox();

protected:
	vector<AcDbObjectId> m_polyBrightBoxId;
};

extern CWindowTableCheckDlg* g_winTableCheckDlg;
BOOL CloseWindowTableCheckDlg();
