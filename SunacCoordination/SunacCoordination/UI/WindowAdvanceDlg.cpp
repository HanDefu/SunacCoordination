// UI\WindowAdvanceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowAdvanceDlg.h"
#include "../Common/ComFun_Interactive.h"
#include "../Common/ComFun_Sunac.h"
#include "../Common/ComFun_Math.h"
#include "../Object/WindowStatistic/AluminumSeries.h"
#include "../Object/WindowStatistic/GlassSeries.h"
#include "../Object/WindowStatistic/AuxiliaryFrameSeries.h"
#include "ComFun_MFC.h"
#include "algorithm"
#include "iterator"


// CWindowAdvanceDlg 对话框

IMPLEMENT_DYNAMIC(CWindowAdvanceDlg, CAcUiDialog)

CWindowAdvanceDlg::CWindowAdvanceDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CWindowAdvanceDlg::IDD, pParent)
	, m_sCode(_T(""))
	, m_bFuKuang(FALSE)
	, m_fJieNeng(0)
{
}

CWindowAdvanceDlg::~CWindowAdvanceDlg()
{
}

void CWindowAdvanceDlg::OnOK()
{
	UpdateData();

	//节能
	CString sJieNeng;
	m_eidtJieneng.GetWindowText(sJieNeng);
	if (sJieNeng.Find(_T("多种"))<0 && m_fJieNeng>0)
	{
		for (UINT i = 0; i < m_selAttrWindows.size(); i++)
		{
			m_selAttrWindows[i]->m_material.heatCoeff = m_fJieNeng;
		}
	}

	//型材
	if (m_xingCai.GetCurSel() >= 0)
	{
		CString sAlSerial = TYUI_GetComboBoxText(m_xingCai);
		for (UINT i = 0; i < m_selAttrWindows.size(); i++)
		{
			m_selAttrWindows[i]->m_material.sAluminumSerial = sAlSerial;
		}
	}

	//玻璃
	if (m_boLi.GetCurSel() >= 0)
	{
		CString sGlass = TYUI_GetComboBoxText(m_boLi);
		for (UINT i = 0; i < m_selAttrWindows.size(); i++)
		{
			m_selAttrWindows[i]->m_material.sGlassSerial = sGlass;
		}
	}

	
		//塞缝尺寸
	if (m_saiFeng.GetCurSel() >= 0)
	{
		double aValue = TYUI_GetDouble(m_saiFeng);
		for (UINT i = 0; i < m_selAttrWindows.size(); i++)
		{
			m_selAttrWindows[i]->SetA(aValue);
		}
	}

	//是否有塞缝
	if (m_bFuKuang < 2)
	{
		for (UINT i = 0; i < m_selAttrWindows.size(); i++)
		{
			m_selAttrWindows[i]->m_material.bHasAuxiliaryFrame = (m_bFuKuang != FALSE);

			if (false == m_selAttrWindows[i]->m_material.bHasAuxiliaryFrame)
			{
				m_selAttrWindows[i]->m_material.sAuxiliaryFrame.Empty();
			}
			else
			{
				if ((m_fuKuangType.GetCurSel() >= 0))
					m_selAttrWindows[i]->m_material.sAuxiliaryFrame = TYUI_GetComboBoxText(m_fuKuangType);
			}
		}
	}

	////标记为已修改状态 //TODO
	//for (UINT i = 0; i < m_selAttrWindows.size(); i++)
	//{
	//	m_selAttrWindows[i]
	//}

	CAcUiDialog::OnOK();
}

BOOL CWindowAdvanceDlg::PreTranslateMessage(MSG *pMsg)
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
		return TRUE;
	return CAcUiDialog::PreTranslateMessage(pMsg);
}

void CWindowAdvanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CODE, m_sCode);
	DDX_Control(pDX, IDC_COMBO_CAIZHI, m_caiZhi);
	DDX_Control(pDX, IDC_COMBO_XINGCAI, m_xingCai);
	DDX_Control(pDX, IDC_COMBO_BOLI, m_boLi);
	DDX_Check(pDX, IDC_CHECK_FUKUANG, m_bFuKuang);
	DDX_Control(pDX, IDC_COMBO_FUKUANG, m_fuKuangType);
	DDX_Control(pDX, IDC_COMBO_SAIFENG, m_saiFeng);
	DDX_Text(pDX, IDC_EDIT_JIENENG, m_fJieNeng);
	DDX_Control(pDX, IDC_EDIT_JIENENG, m_eidtJieneng);
}

BOOL CWindowAdvanceDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	OnBnClickedSelectOnDwg();

	return TRUE;
}

LRESULT CWindowAdvanceDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

BEGIN_MESSAGE_MAP(CWindowAdvanceDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CWindowAdvanceDlg::OnBnClickedSelectOnDwg)
	ON_BN_CLICKED(IDC_CHECK_FUKUANG, &CWindowAdvanceDlg::OnBnClickedAuxiliaryFrame)
END_MESSAGE_MAP()


// CWindowAdvanceDlg 消息处理程序

void CWindowAdvanceDlg::InitAllOption()
{
	InitAluminumSeries();
	InitHeatCoeffCtrl();
	InitGlassSeries();
	InitMaterialType();
	InitAuxiliaryFrame();
	InitAuxiliaryFrameSeries();
	InitPlugSlotSize();
}

void CWindowAdvanceDlg::OnBnClickedSelectOnDwg()
{
	m_sCode.Empty();
	m_selAttrWindows.clear();

	ShowWindow(SW_HIDE);

	vAcDbObjectId ids;
	JHCOM_SelectEnts(ids);

	for (UINT i = 0; i < ids.size(); i++)
	{
		AcDbObject* pAttr = NULL;
		TY_GetAttributeData(ids[i], pAttr);
		AttrWindow* pAttrWindow = AttrWindow::cast(pAttr);
		if (pAttrWindow == NULL)
			continue;

		m_selAttrWindows.push_back(pAttrWindow);

		if (m_sCode.IsEmpty())
			m_sCode = pAttrWindow->m_instanceCode;
		else if (m_sCode != pAttrWindow->m_instanceCode)
			m_sCode = L"多种";
	}

	ShowWindow(SW_SHOW);
	UpdateData(FALSE);

	InitAllOption(); //根据所选原型初始化型材系列等选项
}

void CWindowAdvanceDlg::OnBnClickedAuxiliaryFrame()
{
	m_bFuKuang = !m_bFuKuang;
	if (m_bFuKuang == FALSE)
	{
		TYUI_Disable(m_fuKuangType);
		m_fuKuangType.SetWindowText(_T(""));
	}
	else
	{
		TYUI_Enable(m_fuKuangType);
	}

	//是否有附框会影响塞缝尺寸选项
	InitPlugSlotSize();
	UpdateData(FALSE);
}

void CWindowAdvanceDlg::InitGlassSeries()
{
	if (m_selAttrWindows.empty())
	{
		m_boLi.ResetContent();
		return;
	}

	vector<CString> glassOptions = CGlassSeries::Instance()->GetAllGlassSerials();
	CString defValue;
	for (UINT i = 0; i < m_selAttrWindows.size(); i++)
	{
		if (m_selAttrWindows[i]->m_material.sGlassSerial != m_selAttrWindows[0]->m_material.sGlassSerial)
		{
			TYUI_InitComboBox(m_boLi, glassOptions, L"");
			TYUI_SetText(m_boLi, L"多种");
			return;
		}
	}
	defValue = m_selAttrWindows[0]->m_material.sGlassSerial;
	TYUI_InitComboBox(m_boLi, glassOptions, defValue);
}
void CWindowAdvanceDlg::InitHeatCoeffCtrl() //初始化传热系数
{
	if (m_selAttrWindows.empty())
		return;

	for (UINT i = 0; i < m_selAttrWindows.size(); i++)
	{
		if (JHCOM_equ(m_selAttrWindows[i]->m_material.heatCoeff, m_selAttrWindows[0]->m_material.heatCoeff, 0.01) == false)
		{
			TYUI_SetText(m_eidtJieneng, L"多种");
			m_fJieNeng = 0; //多种时不处理
			return;
		}
	}

	m_fJieNeng = m_selAttrWindows[0]->m_material.heatCoeff;
	CString str;
	str.Format(_T("%.2f"), m_selAttrWindows[0]->m_material.heatCoeff);
	TYUI_SetText(m_eidtJieneng, str);
}

void CWindowAdvanceDlg::InitAluminumSeries()
{
	if (m_selAttrWindows.empty())
	{
		m_xingCai.ResetContent();
		return;
	}

	vector<CString> options, prevOptions, currOptions;
	CString defValue;
	for (UINT i = 0; i < m_selAttrWindows.size(); i++)
	{
		CString prototype = m_selAttrWindows[i]->GetMainPrototypeCode();
		currOptions = CAluminumSeries::Instance()->GetAluminumSerialsByPrototype(prototype);
		std::sort(currOptions.begin(), currOptions.end());
		if (i == 0)
			options = currOptions;
		else
		{
			prevOptions = options;
			options.clear();
			std::set_intersection(prevOptions.begin(), prevOptions.end(), currOptions.begin(), currOptions.end(), std::back_inserter(options));
		}
	}

	for (UINT i = 0; i < m_selAttrWindows.size(); i++)
	{
		if (m_selAttrWindows[i]->m_material.sAluminumSerial != m_selAttrWindows[0]->m_material.sAluminumSerial)
		{
			TYUI_InitComboBox(m_xingCai, options, L"");
			TYUI_SetText(m_xingCai, L"多种");
			return;
		}
	}
	defValue = m_selAttrWindows[0]->m_material.sAluminumSerial;
	TYUI_InitComboBox(m_xingCai, options, defValue);
}

void CWindowAdvanceDlg::InitMaterialType()
{
	if (m_selAttrWindows.empty())
	{
		m_caiZhi.ResetContent();
		return;
	}

	TYUI_InitComboBox(m_caiZhi, L"铝型材", L"铝型材");
}

void CWindowAdvanceDlg::InitPlugSlotSize()
{
	if (m_selAttrWindows.empty())
	{
		m_saiFeng.ResetContent();
		return;
	}

	if (m_bFuKuang == 2)
	{
		m_saiFeng.ResetContent(); //有/无附框时的选项无交集
		TYUI_SetText(m_saiFeng, L"多种");
		return;
	}

	vector<double> temp = CAuxiliaryFrameSeries::Instance()->GetPlugSlotSize(m_bFuKuang != FALSE);
	vector<int> options;
	for (UINT i = 0; i < temp.size(); i++)
		options.push_back((int)temp[i]);
	
	for (UINT i = 0; i < m_selAttrWindows.size(); i++)
	{
		if (m_selAttrWindows[i]->m_material.bHasAuxiliaryFrame != (m_bFuKuang != FALSE))
		{
			TYUI_InitComboBox(m_saiFeng, options, options.empty() ? -1 : options[0]);
			return;
		}
	}

	for (UINT i = 0; i < m_selAttrWindows.size(); i++)
	{
		if (m_selAttrWindows[i]->GetA() != m_selAttrWindows[0]->GetA())
		{
			TYUI_InitComboBox(m_saiFeng, options, -1);
			TYUI_SetText(m_saiFeng, L"多种");
			return;
		}
	}

	int defValue = (int)m_selAttrWindows[0]->GetA();
	TYUI_InitComboBox(m_saiFeng, options, defValue);
}

void CWindowAdvanceDlg::InitAuxiliaryFrame()
{
	if (m_selAttrWindows.empty())
		return;

	m_bFuKuang = m_selAttrWindows[0]->m_material.bHasAuxiliaryFrame;

	for (UINT i = 0; i < m_selAttrWindows.size(); i++)
	{
		if (m_selAttrWindows[i]->m_material.bHasAuxiliaryFrame != m_selAttrWindows[0]->m_material.bHasAuxiliaryFrame)
		{
			m_bFuKuang = 2;
			break;
		}
	}

	if (m_bFuKuang == FALSE)
		TYUI_Disable(m_fuKuangType);
	else
		TYUI_Enable(m_fuKuangType);

	UpdateData(FALSE);
}

void CWindowAdvanceDlg::InitAuxiliaryFrameSeries()
{
	if (m_selAttrWindows.empty())
	{
		m_fuKuangType.ResetContent();
		return;
	}

	vector<CString> options = CAuxiliaryFrameSeries::Instance()->GetAllAuxiliaryFrameSerials();
	for (UINT i = 0; i < m_selAttrWindows.size(); i++)
	{
		if (m_selAttrWindows[i]->m_material.sAuxiliaryFrame != m_selAttrWindows[0]->m_material.sAuxiliaryFrame)
		{
			TYUI_InitComboBox(m_fuKuangType, options, L"");
			TYUI_SetText(m_fuKuangType, L"多种");
			return;
		}
	}
	CString defValue = m_selAttrWindows[0]->m_material.sAuxiliaryFrame;
	TYUI_InitComboBox(m_fuKuangType, options, defValue);
}

CWindowAdvanceDlg* g_windowAdvanceDlg = NULL;

void OpenWindowAdvanceDlg()
{
	if (g_windowAdvanceDlg == NULL)
	{
		g_windowAdvanceDlg = new CWindowAdvanceDlg(acedGetAcadFrame());
		g_windowAdvanceDlg->Create(IDD_DIALOG_WINDOW_ADVANCE);
	}
	g_windowAdvanceDlg->ShowWindow(SW_SHOW);
}

BOOL CloseWindowAdvanceDlg()
{
	if (g_windowAdvanceDlg == NULL)
		return TRUE;
	BOOL ret = g_windowAdvanceDlg->DestroyWindow();
	if (ret)
	{
		delete g_windowAdvanceDlg;
		g_windowAdvanceDlg = NULL;
	}
	return ret;
}

