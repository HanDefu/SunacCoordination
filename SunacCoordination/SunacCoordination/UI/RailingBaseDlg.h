#pragma once
#include "gridctrlwithpreview.h"


// CRailingBaseDlg 对话框

class CRailingBaseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRailingBaseDlg)

public:
	CRailingBaseDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRailingBaseDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RAILINGBASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrlWithPreview m_preRailingBase;
};
