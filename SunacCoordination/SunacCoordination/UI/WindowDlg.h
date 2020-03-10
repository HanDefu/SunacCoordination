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

// 对话框数据
	enum { IDD = IDD_DIALOG_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();


	virtual void PostNcDestroy();
	afx_msg void OnClose();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01

	virtual BOOL PreTranslateMessage(MSG *pMsg); //不响应回车

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonSearchwindow();
	afx_msg void OnBnClickedRadioWindowDoor();
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

	void ClearPrototypes();
	void WindowDoorChange();
	void UpdateInstanceCode();
	void UpdatePrototypeAirVolume(); //更新右侧的原型信息中的通风量
	void UpdateDimDataToComboBox(CComboBox& comboBox, const AttrWindow& attrWindow, CString code);
	void InitDimComboBoxInt(CComboBox& comboBox, vdouble options, double dimValue);

	void InitCombOptions();
	AttrWindow* GetSelWindow();

	void SetEditMode(AcDbObjectId editId);

	double GetArea()const { return (double(m_nWidth*m_nHeight)) / 1e6; }
	bool CheckValueModulo(CComboBox& comboBox, CString p_sType, int p_value, int moduloVale = 50); //检查数据是否是50的模数

	void InitPreviewGridByWindowPrototypes();

	//插入全部，测试用
	void InsertAllWindows_Test();
protected:
	CGridCtrlWithPreview m_preWindow;

	int m_radioDoorWindow; //门窗单选项

	int m_nWidth;
	int m_nHeight;
	int m_nThickness;	//墙厚

	int m_radioBayWindow; //是否凸窗

	//筛选部分
	CComboBox m_comboAreaType; //功能区类型
	CComboBox m_comboOpenType; //开启类型
	CComboBox m_comboOpenAmount; //开启扇数量
	CEdit m_editVentilation; //通风量
	BOOL m_isFireproof; //是否防火窗
	
	BOOL m_bAutoNumber; //是否自动门窗编号
	CEdit m_editWinNumber; //门窗编号
	CComboBox m_comboW1; //开启扇宽度
	CComboBox m_comboH2; //下固定值
	CComboBox m_comboW3;	//转角宽度
	CComboBox m_comboH3; //窗下墙高
	CComboBox m_comboOutWallDistance; //距外墙距离	

	CComboBox m_comboViewDir;
	CComboBox m_comboInsertDir;
	CButton m_btnMirror;


	//////////////////////////////////////////////////////////////////////////
	TYRect m_selectRect;
	vector<AttrWindow> m_winPrototypes;  //搜索到符合条件的门窗原型

	bool m_bEditMode;
	AttrWindow m_attBeforeEdit; //编辑前的门窗属性
	AcDbObjectId m_curEditWinId;


	bool m_bHasInsert; //是否已执行过插入操作	
};

extern CWindowDlg* g_windowDlg;
void OpenWindowDlg(AcDbObjectId editId = AcDbObjectId::kNull);
BOOL CloseWindowDlg();
