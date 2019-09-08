#pragma once
#include "GridCtrlWithPreview.h"
#include "afxwin.h"
#include "../Object/AttrWindow.h"
// CWindowDlg 对话框

class CWindowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWindowDlg)

public:
	CWindowDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWindowDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy(); //释放非模态对话框内存

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrlWithPreview m_preWindow;
	CComboBox m_areaType;
	CComboBox m_openType;
	CComboBox m_openAmount;
	CComboBox m_openWidth;
	CEdit m_width;
	CEdit m_height;
	afx_msg void OnBnClickedMfcbuttonInsert();

	CEdit m_H2;
	CEdit m_cengShu;
	CEdit m_cengGao;
	afx_msg void OnBnClickedButtonSearchwindow();
	std::vector<AttrWindow *> m_allWindws;
};
