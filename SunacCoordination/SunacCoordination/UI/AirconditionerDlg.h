#pragma once
#include "PreviewWithDetail.h"

// CAirconditionerDlg 对话框

class CAirconditionerDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CAirconditionerDlg)

public:
	CAirconditionerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAirconditionerDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_AIRCONDITIONER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	CPreviewWithDetail m_preAC;
};
