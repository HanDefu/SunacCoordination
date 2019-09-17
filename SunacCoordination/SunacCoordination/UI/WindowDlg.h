#pragma once
#include "GridCtrlWithPreview.h"
#include "afxwin.h"
#include "../Object/AttrWindow.h"
// CWindowDlg 对话框

class CWindowDlg : public CAcUiDialog
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
	afx_msg void OnBnClickedMfcbuttonInsert();
	afx_msg void OnBnClickedButtonSearchwindow();
	afx_msg void OnBnClickedRadioDoor();

	void SetRadioDoor(int radioDoor);

	CGridCtrlWithPreview m_preWindow;

	CComboBox m_doorType;
	CComboBox m_areaType;
	CComboBox m_openType;
	CComboBox m_openAmount;
	CComboBox m_openWidth;
	CComboBox m_H2;
	CComboBox m_distance;
	CEdit m_width;
	CEdit m_height;
	CEdit m_ventilation;
	CEdit m_number;
	
	vector<AttrWindow *> m_allWindws;

	int m_radioDoor;
	int m_radioYes;
};
