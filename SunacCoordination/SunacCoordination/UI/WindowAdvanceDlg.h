#pragma once
#include "afxwin.h"
#include "../Object/WindowDoor/AttrWindow.h"

// CWindowAdvanceDlg 对话框

class CWindowAdvanceDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CWindowAdvanceDlg)

public:
	CWindowAdvanceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWindowAdvanceDlg();

	virtual INT_PTR DoModal();

// 对话框数据
	enum { IDD = IDD_DIALOG_WINDOW_ADVANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy(); //释放非模态对话框内存

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedSelectOnDwg();

	bool m_isMoldless;	//是否非模态对话框

	AttrWindow* m_pAttrWindow;

	CString m_sCode;
	CComboBox m_jieNeng;
	CComboBox m_caiZhi;
	CComboBox m_xingCai;
	CComboBox m_boLi;
};

extern CWindowAdvanceDlg* g_windowAdvanceDlg;
BOOL CloseWindowAdvanceDlg();
