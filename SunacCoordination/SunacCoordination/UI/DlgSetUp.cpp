// DlgSetUp.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSetUp.h"
#include "afxdialogex.h"
#include "../GlobalSetting.h"
#include <algorithm>
#include <vector>
#include "../UI/GridCtrl_src/GridCtrl.h"
#include "../Common/ComFun_Layer.h"

// CDlgSetUp 对话框

IMPLEMENT_DYNAMIC(CDlgSetUp, CDialogEx)

CDlgSetUp::CDlgSetUp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetUp::IDD, pParent)
	, m_showLimianNumber(TRUE)
	, m_useAinLimian(TRUE)
	, m_sFrameLayerDlg(_T(""))
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
	DDX_Text(pDX, IDC_WINFRAMELAYER, m_sFrameLayerDlg);
}


BEGIN_MESSAGE_MAP(CDlgSetUp, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSetUp::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSetUp::OnBnClickedCancel)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
END_MESSAGE_MAP()

LRESULT CDlgSetUp::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

bool CompareWinSetUpLayer(CString& a, CString& b)
{
	return a < b;
}



BOOL CDlgSetUp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitWinSetting();
	
	return TRUE; 
}

void CDlgSetUp::InitWinSetting()
{
	m_originalWinSetting = GlobalSetting::GetInstance()->m_winSetting;

	m_winLayer.SetWindowText(GlobalSetting::GetInstance()->m_winSetting.m_sWinLayer);
	m_winFrameLayer.SetWindowText(GlobalSetting::GetInstance()->m_winSetting.m_sWinFrameLayer);
	m_winWallLayer.SetWindowText(GlobalSetting::GetInstance()->m_winSetting.m_sWinWallLayer);
	m_winHardwareLayer.SetWindowText(GlobalSetting::GetInstance()->m_winSetting.m_sWinHardwareLayer);
	m_winOpenLayer.SetWindowText(GlobalSetting::GetInstance()->m_winSetting.m_sWinOpenLayer);
	m_winNumberLayerLimian.SetWindowText(GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerLimian);
	m_winNumberLayerPingmian.SetWindowText(GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerPingmian);

	((CComboBox*)GetDlgItem(IDC_WINNUMTEXTSIZE))->SetCurSel(3);//设置门窗字体编号默认显示100
}

// CDlgSetUp 消息处理程序
void CDlgSetUp::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_sFrameLayerDlg.IsEmpty())// TODO||
	{
		AfxMessageBox(_T("图层不能为空"));
		return;
	}


	//判断各个图层不能相同
	vector<CString> sLayers;




	//对当前文档图层转换图层


	//保存到setting

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
			return;
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

void CDlgSetUp::UpdateLayer()
{
	//TODO 叶明远
	UpdateData(TRUE);
	if (m_originalWinSetting.m_sWinFrameLayer != m_sFrameLayerDlg)
	{
		ChangeLayer(m_originalWinSetting.m_sWinFrameLayer, m_sFrameLayerDlg);
		ChangeLayer(GSINST->m_winSetting.GetWinFrameLayerDefault(), m_sFrameLayerDlg);
	}
	if (m_originalWinSetting.m_sWinHardwareLayer != TYUI_GetText(m_winHardwareLayer))
	{
		ChangeLayer(m_originalWinSetting.m_sWinHardwareLayer, TYUI_GetText(m_winHardwareLayer));
		ChangeLayer(GSINST->m_winSetting.GetWinHardwareLayerDefault(), TYUI_GetText(m_winHardwareLayer));
	}
	if (m_originalWinSetting.m_sWinLayer != TYUI_GetText(m_winLayer))
	{
		ChangeLayer(m_originalWinSetting.m_sWinLayer, TYUI_GetText(m_winLayer));
		ChangeLayer(GSINST->m_winSetting.GetWinLayerDefault(), TYUI_GetText(m_winLayer));
	}
	if (m_originalWinSetting.m_sWinNumberLayerLimian != TYUI_GetText(m_winNumberLayerLimian))
	{
		ChangeLayer(m_originalWinSetting.m_sWinNumberLayerLimian, TYUI_GetText(m_winNumberLayerLimian));
		ChangeLayer(GSINST->m_winSetting.GetWinNumberLayerLimianDefault(), TYUI_GetText(m_winNumberLayerLimian));
	}
	if (m_originalWinSetting.m_sWinNumberLayerPingmian != TYUI_GetText(m_winNumberLayerPingmian))
	{
		ChangeLayer(m_originalWinSetting.m_sWinNumberLayerPingmian, TYUI_GetText(m_winNumberLayerPingmian));
		ChangeLayer(GSINST->m_winSetting.GetWinNumberLayerPingmianDefault(), TYUI_GetText(m_winNumberLayerPingmian));
	}
	if (m_originalWinSetting.m_sWinOpenLayer != TYUI_GetText(m_winOpenLayer))
	{
		ChangeLayer(m_originalWinSetting.m_sWinOpenLayer, TYUI_GetText(m_winOpenLayer));
		ChangeLayer(GSINST->m_winSetting.GetWinOpenLayerDefault(), TYUI_GetText(m_winOpenLayer));
	}
	if (m_originalWinSetting.m_sWinWallLayer != TYUI_GetText(m_winWallLayer))
	{
		ChangeLayer(m_originalWinSetting.m_sWinWallLayer, TYUI_GetText(m_winWallLayer));
		ChangeLayer(GSINST->m_winSetting.GetWallLayerDefault(), TYUI_GetText(m_winWallLayer));
	}
}

void CDlgSetUp::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
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
