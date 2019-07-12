#pragma once
#include "gridctrlwithpreview.h"


// CFillingDlg 对话框

class CFillingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFillingDlg)

public:
	CFillingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFillingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FILLING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrlWithPreview m_preFilling;
};
