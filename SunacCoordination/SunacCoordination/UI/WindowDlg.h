#pragma once
#include "GridCtrlWithPreview.h"
#include "afxwin.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "ComFun_MFC.h"
#include "../Common/TYRect.h"

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
	void InitDimComboBoxInt(CComboBox& comboBox, vdouble options, double dimValue);
	void LoadDefaultValue();
	AttrWindow* GetSelWindow();

	bool CheckValueModulo(CComboBox& comboBox, CString p_sType, int p_value); //检查数据是否是50的模数

	void SetEditMode(AcDbBlockReference* pBlock);

	//插入全部，测试用
	void InsertAllWindows_Test();

	double GetArea()const { return (double(m_nWidth*m_nHeight)) / 1e6; }

protected:
	CGridCtrlWithPreview m_preWindow;

	int m_nWidth;
	int m_nHeight;

	CComboBox m_comboAreaType; //功能区类型
	CComboBox m_comboOpenType; //开启类型
	CComboBox m_comboOpenAmount; //开启扇数量
	CEdit m_editVentilation; //通风量

	//CEdit m_area; //面积
	//CComboBox m_rate; //比值

	CEdit m_number; //门窗编号
	CComboBox m_comboW1; //开启扇宽度
	CComboBox m_comboH2; //下固定值
	CComboBox m_comboW3;	//转角宽度
	CComboBox m_comboH3; //窗下墙高
	CComboBox m_comboOutWallDistance; //距外墙距离
	
	int m_radioDoor; //门窗单选
	int m_radioYes; //是否凸窗

	BOOL m_autoIndex;
	CComboBox m_comboViewDir;
	CComboBox m_comboInsertDir;
	CButton m_isMirror;

	vector<AttrWindow> m_allWindows;

	AcDbBlockReference* m_pCurEdit;

	bool m_isMoldless;//是否非模态对话框

	TYRect m_selectRect;

	bool m_bHasInsert; //执行过插入操作
public:
};

extern CWindowDlg* g_windowDlg;
BOOL CloseWindowDlg();
