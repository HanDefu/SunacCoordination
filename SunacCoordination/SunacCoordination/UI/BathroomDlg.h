#pragma once
#include "GridCtrlWithPreview.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/Bathroom/BathroomGen.h"
#include "ComFun_MFC.h"
#include "afxwin.h"

// CBathroomDlg 对话框


class CBathroomDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CBathroomDlg)

public:
	CBathroomDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBathroomDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_BATHROOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy(); //释放非模态对话框内存

	DECLARE_MESSAGE_MAP()

	bool IsBathroomRectValid(TYRect rect);

public:
	CGridCtrlWithPreview m_preBathroom;
	CComboBox m_bathroomType;
	CComboBox m_basinWidth;
	CComboBox m_toiletWidth;
	CComboBox m_washWidth;
	CComboBox m_floorRange;
	CEdit m_number;
	CEdit m_offsetX;
	CEdit m_offsetY;
	CEdit m_customX;
	CEdit m_customY;
	CButton m_isMirror;
	CButton m_autoIndex;
	CButton m_noAirOut;

	TYRect m_rect;
	E_DIRECTION m_doorDir;
	E_DIRECTION m_windowDir;
	int m_isStd;

	vector<AttrBathroom*> m_allBathrooms;
	CBathroomGen* m_pBathroomGen;

	void LoadDefaultValue();
	E_DIRECTION GetDir(ads_point pt);
	void EnableSetAirout(bool bEnable);
	void EnableSetProperty(bool bEnable);

	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01
	afx_msg void OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonRange();
	afx_msg void OnBnClickedButtonDoorDir();
	afx_msg void OnBnClickedButtonWindowDir();
	afx_msg void OnBnClickedButtonSearch();
};

extern CBathroomDlg* g_bathroomDlg;
BOOL CloseBathroomDlg();