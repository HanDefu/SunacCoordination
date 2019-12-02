#pragma once
#include "GridCtrlWithPreview.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/Kitchen/KitchenGen.h"
#include "ComFun_MFC.h"
#include "afxwin.h"

// CKitchenDlg 对话框



class CKitchenDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CKitchenDlg)

public:
	CKitchenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKitchenDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_KITCHEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG *pMsg); //不响应回车

	DECLARE_MESSAGE_MAP()

	bool IsKitchRectValid(TYRect rect);
public:
	CGridCtrlWithPreview m_preKitchen;
	CComboBox m_kitchenType;
	CComboBox m_floorRange;
	CComboBox m_basinType;
	CComboBox m_benchWidth;
	CComboBox m_fridgeType;
	CButton m_isMirror;
	CButton m_noAirOut;
	CEdit m_number;
	CEdit m_offsetX;
	CEdit m_offsetY;
	CEdit m_customX;
	CEdit m_customY;
	BOOL m_bAutoIndex;

	//////////////////////////////////////////////////////////////////////////
	TYRect m_rect;
	E_DIRECTION m_doorDir;
	E_DIRECTION m_windowDir;
	int m_isStd; //国标/自定义
	int m_angle; //插入时旋转角，可能取值为0, 90, 180, 270

	vector<AttrKitchen> m_allKitchens;

	CKitchGen* m_pKitchGen;
	
	AcDbBlockReference* m_pCurEdit; //双击编辑时使用

	afx_msg void OnBnClickedOk();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonRange();
	afx_msg void OnBnClickedButtonDoorDir();
	afx_msg void OnBnClickedButtonWindowDir();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAutoIndex();
	afx_msg void OnBnClickedNoAirout();

	void SetEditMode(AcDbBlockReference* pBlock);
	void UpdateAttribute();
	bool lockUpdate;
	void ShowInfo();
	E_DIRECTION GetDir(ads_point pt);
	void LoadDefaultValue();
	void EnableSetProperty(bool bEnable);
	void EnableSetAirout(bool bEnable);
	void ClearPreviews();
	bool CheckValid(CString& errMsg); //插入前检验参数合法性
};

extern CKitchenDlg* g_kitchenDlg;
void OpenKitchenDlg(AcDbBlockReference* pCurEdit = NULL);
BOOL CloseKitchenDlg();