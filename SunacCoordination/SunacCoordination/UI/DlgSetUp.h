#pragma once

#include "afxwin.h"
#include "ComFun_MFC.h"
#include "GridCtrlWithPreview.h"
#include "../GlobalSetting.h"

// CDlgSetUp 对话框

class CDlgSetUp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetUp)

public:
	CDlgSetUp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetUp();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	BOOL m_showLimianNumberDlg;	//是否显示立面图的门窗编号
	BOOL m_useAinLimianDlg;		//立面图是否使用塞缝尺寸

	CComboBox m_winNumberTextSizeDlg;//门窗编号字体
	CComboBox m_winDetailDimRateDlg;//门窗编号字体

	CString m_sFrameLayerDlg;	//门窗型材结构框图层
	CString m_sHardWareLayerDlg;	//门窗五金件图层
	CString m_sLayerDlg;			//门窗块图层
	CString m_sNumberLayerLimianDlg;//门窗立面图编号图层
	CString m_sNumberLayerPingmianDlg;//门窗平面图编号图层
	CString m_sOpenLayerDlg;			//门窗开启部分图层
	CString m_sWallLayerDlg;			//门窗外墙的图层

	CWinSetting m_originalWinSetting;


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);///---01.

	void InitWinSetting();

	void UpdateLayer();//更新图层

	void SetDigPos();

public:
	virtual BOOL OnInitDialog();
};