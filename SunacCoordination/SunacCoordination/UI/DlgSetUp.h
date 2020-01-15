#pragma once

#include "afxwin.h"
#include "ComFun_MFC.h"
#include "GridCtrlWithPreview.h"
#include "D:\vs2010\VC\atlmfc\include\afxwin.h"

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
	CEdit m_winFrameLayer;		//门窗型材结构框图层
	CEdit m_winHardwareLayer;	//门窗五金件图层
	CEdit m_winLayer;			 //门窗块图层
	CEdit m_winNumberLayerLimian;//门窗立面图编号图层
	CEdit m_winNumberLayerPingmian;//门窗平面图编号图层
	CEdit m_winOpenLayer;		//门窗开启部分图层
	CEdit m_winWallLayer;		//门窗外墙的图层
	BOOL m_showLimianNumber;	//是否显示立面图的门窗编号
	BOOL m_useAinLimian;		//立面图是否使用塞缝尺寸
	CComboBox m_winNumberTextSize;//门窗编号字体

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();


	//判断设置是否为空，不为空才将设置对话框中的值赋给GlobalSetting,为空则使用默认值
	void WinSetUp(CString& p_winSetting, CString p_winSetUp);

	void LoadDefaultValue();

private:
	vector<CString> m_winSetUpLayer;

public:
	virtual BOOL OnInitDialog();
};

extern CDlgSetUp* g_winSetupDlg;
void OpenWindowSetUpDlg();
BOOL CloseWindowSetUpDlg();