#pragma once
#include "gridctrlwithpreview.h"
#include "afxwin.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/RCKitchen.h"
#include "../Object/AttrKitchen.h"

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
	CComboBox m_floorRange;
	CComboBox m_basinType;
	CComboBox m_benchWidth;
	CComboBox m_frigeType;
	CComboBox m_projectLevel;

	TYRect m_rect;
	DIR doorDir;
	DIR windowDir;

	afx_msg void OnBnClickedOk();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	afx_msg void OnBnClickedMfcbuttonInsert();
	afx_msg void OnBnClickedMfcbuttonRange();
	afx_msg void OnBnClickedMfcbuttonDoorDir();
	afx_msg void OnBnClickedMfcbuttonWindowDir();
	afx_msg void OnBnClickedButtonSearch();

	DIR GetDir(ads_point pt);
	vector<AttrKitchen*> FilterKUq();
};
