#pragma once


// CDlgWindowAirCalc 对话框

class CDlgWindowAirCalc : public CAcUiDialog
{
	DECLARE_DYNAMIC(CDlgWindowAirCalc)

public:
	CDlgWindowAirCalc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWindowAirCalc();

// 对话框数据
	enum { IDD = IDD_DIALOG_WIN_AIRCALC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	double m_airQuality; //通风量
protected:
	CEdit m_area; //面积
	CComboBox m_rate; //比值
};
