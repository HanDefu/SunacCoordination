#pragma once
#include "GridCtrlWithPreview.h"
#include "afxwin.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "ComFun_MFC.h"

// CWindowDlg 对话框

class CWindowDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CWindowDlg)

public:
	CWindowDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWindowDlg();

	virtual INT_PTR DoModal();

// 对话框数据
	enum { IDD = IDD_DIALOG_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy(); //释放非模态对话框内存
	virtual BOOL PreTranslateMessage(MSG *pMsg); //不响应回车

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonSearchwindow();
	afx_msg void OnBnClickedRadioDoor();
	afx_msg void OnBnClickedCalculate();
	afx_msg void OnBnClickedAutoIndex();
	afx_msg void OnBnClickedSelOnDwg();
	afx_msg void OnBnClickedMirror();
	afx_msg void OnSelChangedPreview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelChangedW1();
	afx_msg void OnSelChangedH2();
	afx_msg void OnSelChangedW3();
	afx_msg void OnSelChangedH3();
	afx_msg void OnSelChangedView();
	afx_msg void OnBnClickedBayWindow();
	afx_msg void OnSelChangedWallDis();
	void UpdateEnable();
	void UpdateInstanceCode();
	void UpdateVent();
	void UpdateDimDataToComboBox(CComboBox& comboBox, const AttrWindow& attrWindow, CString code);
	void LoadDefaultValue();
	AttrWindow* GetSelWindow();

	void SetEditMode(AcDbBlockReference* pBlock);

	//插入全部，测试用
	void InsertAllWindows();

protected:

	CGridCtrlWithPreview m_preWindow;

	CEdit m_width; //宽度
	CEdit m_height; //高度
	CComboBox m_areaType; //功能区类型
	CComboBox m_openType; //开启类型
	CComboBox m_openAmount; //开启扇数量
	CEdit m_ventilation; //通风量
	CEdit m_area; //面积
	CComboBox m_rate; //比值

	CEdit m_number; //门窗编号
	CComboBox m_W1; //开启扇宽度
	CComboBox m_H2; //下固定值
	CComboBox m_distance; //距外墙距离
	CComboBox m_W3;
	CComboBox m_H3;
	CComboBox m_insertDir;
	
	int m_radioDoor; //门窗单选
	int m_radioYes; //是否凸窗

	BOOL m_autoIndex;
	CComboBox m_viewDir;
	CButton m_isMirror;

	vector<AttrWindow> m_allWindows;
	double m_selWidth; //选中原型的开间
	double m_selHeight;//选中原型的进深

	AcDbBlockReference* m_pCurEdit;

	bool m_isMoldless;//是否非模态对话框
};

extern CWindowDlg* g_windowDlg;
BOOL CloseWindowDlg();
