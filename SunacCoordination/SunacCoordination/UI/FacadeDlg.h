#pragma once
#include "GridCtrl_src\GridCtrl.h"

// CFacadeDlg 对话框

class CFacadeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFacadeDlg)

public:
	CFacadeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFacadeDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FACADE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrl m_tFacade;
};
