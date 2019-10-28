// UI\WindowAdvanceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowAdvanceDlg.h"
#include "../Common/ComFun_Interactive.h"
#include "../Common/ComFun_Sunac.h"
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
{
	m_isMoldless = true;
}

CWindowAdvanceDlg::~CWindowAdvanceDlg()
{
}

INT_PTR CWindowAdvanceDlg::DoModal()
{
	m_isMoldless = false;
	return CAcUiDialog::DoModal();
}

void CWindowAdvanceDlg::OnOK()
{
	for (UINT i = 0; i < m_selAttrWindows.size(); i++)
	{
		if (m_xingCai.GetCurSel() >= 0)
			m_selAttrWindows[i]->m_material.sAluminumSerial = TYUI_GetComboBoxText(m_xingCai);
		if (m_boLi.GetCurSel() >= 0)
			m_selAttrWindows[i]->m_material.sGlassSerial = TYUI_GetComboBoxText(m_boLi);
		if (m_saiFeng.GetCurSel() >= 0)
			m_selAttrWindows[i]->SetA(TYUI_GetDouble(m_saiFeng));
		if (m_bFuKuang < 2)
			m_selAttrWindows[i]->m_material.bHasAuxiliaryFrame = (m_bFuKuang != FALSE);
		if (m_selAttrWindows[i]->m_material.bHasAuxiliaryFrame && (m_fuKuangType.GetCurSel() >= 0))
			m_selAttrWindows[i]->m_material.sAuxiliaryFrame = TYUI_GetComboBoxText(m_fuKuangType);
	}

	CAcUiDialog::OnOK();
	if (m_isMoldless)
		DestroyWindow();
}

void CWindowAdvanceDlg::OnCancel()
{
	CAcUiDialog::OnCancel();
	if (m_isMoldless)
		DestroyWindow();
}

void CWindowAdvanceDlg::PostNcDestroy()
{
	CAcUiDialog::PostNcDestroy();
	if (m_isMoldless)
	{
		delete this;
		g_windowAdvanceDlg = NULL;
	}
}

void CWindowAdvanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CODE, m_sCode);
	DDX_Control(pDX, IDC_COMBO_JIENENG, m_jieNeng);
	DDX_Control(pDX, IDC_COMBO_CAIZHI, m_caiZhi);
	DDX_Control(pDX, IDC_COMBO_XINGCAI, m_xingCai);
	DDX_Control(pDX, IDC_COMBO_BOLI, m_boLi);
	DDX_Check(pDX, IDC_CHECK_FUKUANG, m_bFuKuang);
	DDX_Control(pDX, IDC_COMBO_FUKUANG, m_fuKuangType);
	DDX_Control(pDX, IDC_COMBO_SAIFENG, m_saiFeng);
}

BOOL CWindowAdvanceDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	OnBnClickedSelectOnDwg();
	InitAluminumSeries();
	InitGlassSeries();
	InitMaterialType();
	InitAuxiliaryFrame();
	InitAuxiliaryFrameSeries();
	InitPlugSlotSize();
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
}

void CWindowAdvanceDlg::OnBnClickedAuxiliaryFrame()
{
	m_bFuKuang = !m_bFuKuang;
	if (m_bFuKuang == FALSE)
		TYUI_Disable(m_fuKuangType);
	else
		TYUI_Enable(m_fuKuangType);
	//是否有附框会影响塞缝尺寸选项
	InitPlugSlotSize();
	UpdateData(FALSE);
}

void CWindowAdvanceDlg::InitGlassSeries()
{
	if (m_selAttrWindows.empty())
	{
		TYUI_InitComboBox(m_boLi, L"", L"");
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

void CWindowAdvanceDlg::InitAluminumSeries()
{
	if (m_selAttrWindows.empty())
	{
		TYUI_InitComboBox(m_xingCai, L"", L"");
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
		TYUI_InitComboBox(m_caiZhi, L"", L"");
		return;
	}

	TYUI_InitComboBox(m_caiZhi, L"铝型材", L"铝型材");
}

void CWindowAdvanceDlg::InitPlugSlotSize()
{
	if (m_selAttrWindows.empty())
	{
		TYUI_InitComboBox(m_saiFeng, L"", L"");
		return;
	}
	if (m_bFuKuang == 2)
	{
		TYUI_InitComboBox(m_saiFeng, L"", L""); //有/无附框时的选项无交集
		TYUI_SetText(m_saiFeng, L"多种");
		return;
	}

	vector<double> temp = CAuxiliaryFrameSeries::Instance()->GetPlugSlotSize(m_bFuKuang != FALSE);
	vector<int> options;
	for (UINT i = 0; i < temp.size(); i++)
		options.push_back((int)temp[i]);

	int defValue = -1;

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

	defValue = _ttoi(m_selAttrWindows[0]->m_material.sAuxiliaryFrame);
	TYUI_InitComboBox(m_saiFeng, options, defValue);
}

void CWindowAdvanceDlg::InitAuxiliaryFrame()
{
	if (m_selAttrWindows.empty())
		return;

	for (UINT i = 0; i < m_selAttrWindows.size(); i++)
	{
		if (m_selAttrWindows[i]->m_material.bHasAuxiliaryFrame != m_selAttrWindows[0]->m_material.bHasAuxiliaryFrame)
		{
			m_bFuKuang = 2;
			break;
		}
	}
	m_bFuKuang = m_selAttrWindows[0]->m_material.bHasAuxiliaryFrame;
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
		TYUI_InitComboBox(m_fuKuangType, L"", L"");
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

BOOL CloseWindowAdvanceDlg()
{
	if (g_windowAdvanceDlg == NULL)
		return TRUE;
	BOOL ret = g_windowAdvanceDlg->DestroyWindow();
	if (ret)
		g_windowAdvanceDlg = NULL;
	return ret;
}
