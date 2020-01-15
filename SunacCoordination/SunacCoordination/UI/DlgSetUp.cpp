// DlgSetUp.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSetUp.h"
#include "afxdialogex.h"
#include "../GlobalSetting.h"
#include <algorithm>
#include <vector>
#include "../UI/GridCtrl_src/GridCtrl.h"

// CDlgSetUp 对话框

IMPLEMENT_DYNAMIC(CDlgSetUp, CDialogEx)

CDlgSetUp::CDlgSetUp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetUp::IDD, pParent)
	, m_showLimianNumber(TRUE)
	, m_useAinLimian(TRUE)
{
	
}

CDlgSetUp::~CDlgSetUp()
{
}

void CDlgSetUp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WINFRAMELAYER, m_winFrameLayer);
	DDX_Control(pDX, IDC_WINHARDWARELAYER, m_winHardwareLayer);
	DDX_Control(pDX, IDC_WINLAYER, m_winLayer);
	DDX_Control(pDX, IDC_WINNUMBERLAYERLIMIAN, m_winNumberLayerLimian);
	DDX_Control(pDX, IDC_WINNUMBERLAYERPINGMIAN, m_winNumberLayerPingmian);
	DDX_Control(pDX, IDC_WINOPENLAYER, m_winOpenLayer);
	DDX_Control(pDX, IDC_WINWALLLAYER, m_winWallLayer);
	DDX_Radio(pDX, IDC_SHOWLIMIANNUMBER, m_showLimianNumber);
	DDX_Radio(pDX, IDC_USEAINLIMIAN, m_useAinLimian);
	DDX_Control(pDX, IDC_WINNUMTEXTSIZE, m_winNumberTextSize);
	DDX_Control(pDX, IDC_WINDETAILDIMRATE, m_winDetailDimRate);
}


BEGIN_MESSAGE_MAP(CDlgSetUp, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSetUp::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSetUp::OnBnClickedCancel)
END_MESSAGE_MAP()


bool CompareWinSetUpLayer(CString& a, CString& b)
{
	return a < b;
}



BOOL CDlgSetUp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadDefaultValue();
	
	return TRUE; 
}

// CDlgSetUp 消息处理程序
void CDlgSetUp::OnBnClickedOk()
{
	UpdateData(TRUE);

	m_winSetUpLayer.clear();

	//门窗图层设置
	WinSetUp(GlobalSetting::GetInstance()->m_winSetting.m_sWinLayer, TYUI_GetText(m_winLayer));
	WinSetUp(GlobalSetting::GetInstance()->m_winSetting.m_sWinFrameLayer, TYUI_GetText(m_winFrameLayer));
	WinSetUp(GlobalSetting::GetInstance()->m_winSetting.m_sWinWallLayer, TYUI_GetText(m_winWallLayer));
	WinSetUp(GlobalSetting::GetInstance()->m_winSetting.m_sWinHardwareLayer, TYUI_GetText(m_winHardwareLayer));
	WinSetUp(GlobalSetting::GetInstance()->m_winSetting.m_sWinOpenLayer, TYUI_GetText(m_winOpenLayer));
	WinSetUp(GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerLimian, TYUI_GetText(m_winNumberLayerLimian));
	WinSetUp(GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerPingmian, TYUI_GetText(m_winNumberLayerPingmian));

	//门窗编号字体设置
	if (TYUI_GetText(m_winNumberTextSize).GetLength() > 0)
	{
		GlobalSetting::GetInstance()->m_winSetting.m_numberTextSize = TYUI_GetInt(m_winNumberTextSize);
	}

	//门窗编号字体设置
	if (TYUI_GetText(m_winDetailDimRate).GetLength() > 0)
	{
		GlobalSetting::GetInstance()->m_winSetting.m_bWinDetailDimRate = TYUI_GetInt(m_winDetailDimRate);
	}

	//其他
	GlobalSetting::GetInstance()->m_winSetting.m_bUseAinLimian = m_useAinLimian ? true : false;
	GlobalSetting::GetInstance()->m_winSetting.m_bShowLimianNumber = m_showLimianNumber ? true : false;
	
	//排序
	std::sort(m_winSetUpLayer.begin(), m_winSetUpLayer.end(), CompareWinSetUpLayer);

	for (int i = 0; i < m_winSetUpLayer.size() - 1; i++)
	{
		if (m_winSetUpLayer[i].CompareNoCase(m_winSetUpLayer[i + 1]) == 0)
		{
			AfxMessageBox(L"图层名称不能相同");
			OnBnClickedCancel();
		}
	}

	GlobalSetting::GetInstance()->UpdateToXml();

	CDialogEx::OnOK();
}

void CDlgSetUp::WinSetUp(CString &p_winSetting, CString p_winSetUp)
{
	if (p_winSetUp.GetLength() > 0)
	{
		p_winSetting = p_winSetUp;
	}
	m_winSetUpLayer.push_back(p_winSetting);
}

void CDlgSetUp::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CDlgSetUp::LoadDefaultValue()
{
	m_winLayer.SetWindowText(GlobalSetting::GetInstance()->m_winSetting.m_sWinLayer);
	m_winFrameLayer.SetWindowText(GlobalSetting::GetInstance()->m_winSetting.m_sWinFrameLayer);
	m_winWallLayer.SetWindowText(GlobalSetting::GetInstance()->m_winSetting.m_sWinWallLayer);
	m_winHardwareLayer.SetWindowText(GlobalSetting::GetInstance()->m_winSetting.m_sWinHardwareLayer);
	m_winOpenLayer.SetWindowText(GlobalSetting::GetInstance()->m_winSetting.m_sWinOpenLayer);
	m_winNumberLayerLimian.SetWindowText(GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerLimian);
	m_winNumberLayerPingmian.SetWindowText(GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerPingmian);

	((CComboBox*)GetDlgItem(IDC_WINNUMTEXTSIZE))->SetCurSel(3);//设置门窗字体编号默认显示100
	((CComboBox*)GetDlgItem(IDC_WINDETAILDIMRATE))->SetCurSel(1);//设置门窗详图标注比例默认显示50
}

CDlgSetUp* g_winSetupDlg = NULL;

void OpenWindowSetUpDlg()
{
	if (g_winSetupDlg == NULL)
	{
		CAcModuleResourceOverride resOverride;
		g_winSetupDlg = new CDlgSetUp(acedGetAcadFrame());
		g_winSetupDlg->Create(IDD_DIALOG_SETUP);
	}
	g_winSetupDlg->ShowWindow(SW_SHOW);
}

BOOL CloseWindowSetUpDlg()
{
	if (g_winSetupDlg == NULL)
		return TRUE;
	BOOL ret = g_winSetupDlg->DestroyWindow();
	if (ret)
	{
		delete g_winSetupDlg;
		g_winSetupDlg = NULL;
	}
	return ret;
}
