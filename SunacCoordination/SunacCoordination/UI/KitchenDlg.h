#pragma once
#include "gridctrlwithpreview.h"
#include "afxwin.h"
#include "../Object/AttrKitchen.h"
#include "../Common/ComFun_Sunac.h"
#include "ComFun_MFC.h"

// CKitchenDlg 对话框

enum DIR
{
	DIR_TOP,
	DIR_LEFT,
	DIR_BOTTOM,
	DIR_RIGHT,
	DIR_UNKNOWN
};

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
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy(); //释放非模态对话框内存

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrlWithPreview m_preKitchen;
	CComboBox m_kitchenType;
	CComboBox m_floorRange;
	CComboBox m_basinType;
	CComboBox m_benchWidth;
	CComboBox m_fridgeType;
	CEdit m_number;
	BOOL m_bHasAirOut;
	BOOL m_bAutoIndex;

	TYRect m_rect;
	DIR m_doorDir;
	DIR m_windowDir;
	vector<AttrKitchen*> m_allKitchens;

	afx_msg void OnBnClickedOk();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonRange();
	afx_msg void OnBnClickedButtonDoorDir();
	afx_msg void OnBnClickedButtonWindowDir();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAirOut();
	afx_msg void OnBnClickedAutoIndex();

	DIR GetDir(ads_point pt);
	void LoadDefaultValue();
	vector<AttrKitchen*> FilterKU();
	vector<AttrKitchen*> FilterKUq();
	vector<AttrKitchen*> FilterKUqc();
	vector<AttrKitchen*> FilterKUs();
	vector<AttrKitchen*> FilterKL();
	vector<AttrKitchen*> FilterKI();
};
