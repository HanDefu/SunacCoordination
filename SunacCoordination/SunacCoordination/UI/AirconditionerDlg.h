#pragma once
#include "PreviewWithDetail.h"
#include "GsPreviewCtrl.h"
#include "afxwin.h"
#include "../Object/AttrAirCon.h"
#include "ComFun_MFC.h"

// CAirconditionerDlg 对话框

class CAirconditionerDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CAirconditionerDlg)

public:
	CAirconditionerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAirconditionerDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_AIRCONDITIONER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	CPreviewWithDetail m_preAC;

	CComboBox m_pNum;		//匹数
	CComboBox m_lNTubePos;	//冷凝水管位置
	CButton m_hasRainTube;	//是否有雨水立管穿过
	CComboBox m_rainTubePos;//雨水管位置

	vector<AttrAirCon> m_allAirCons;

public:

	CGsPreviewCtrl m_preAirCon;
	CButton m_btnInbsert;

	void LoadDefaultValue();
//	afx_msg void OnCbnSelchangeComboPnum();
	afx_msg void OnBnClickedCheckHasraintube();
	afx_msg void OnCbnSelchangeComboPnum();
	void UpdatePreview();
	
	afx_msg void OnCbnSelchangeComboLntubepos();
	afx_msg void OnCbnSelchangeComboRaintubepos();
	afx_msg void OnBnClickedButtonInsertac();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);

	CString m_filePathName;
	CButton m_upDownImage;
	CButton m_leftRightImage;
	CStatic m_room;	//房间面积(静态文本框)
	afx_msg void OnBnClickedButtonCalculate(); 
	CEdit m_roomSize; //房间面积(编辑框)
	bool m_flag;  //判断是否显示空调编辑框
	double m_rSize; //房间面积，用于计算空调匹数
};

extern CAirconditionerDlg* g_airconditionerDlg;
BOOL CloseAirconditionerDlg();