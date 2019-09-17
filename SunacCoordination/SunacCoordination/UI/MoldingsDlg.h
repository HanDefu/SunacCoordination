#pragma once
#include "gridctrlwithpreview.h"


// CMoldingsDlg 对话框

class CMoldingsDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CMoldingsDlg)

public:
	CMoldingsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMoldingsDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MOLDINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrlWithPreview m_preMoldings;
};
