// UI\WindowAdvanceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowAdvanceDlg.h"
#include "../Common/ComFun_Interactive.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/WindowStatistic/AluminumSeries.h"
#include "../Object/WindowStatistic/GlassSeries.h"
#include "ComFun_MFC.h"
#include "algorithm"
#include "iterator"


// CWindowAdvanceDlg 对话框

IMPLEMENT_DYNAMIC(CWindowAdvanceDlg, CAcUiDialog)

CWindowAdvanceDlg::CWindowAdvanceDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CWindowAdvanceDlg::IDD, pParent)
	, m_sCode(_T(""))
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
}

BOOL CWindowAdvanceDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	OnBnClickedSelectOnDwg();
	InitAluminumSeries();
	InitGlassSeries();
	InitMaterialType();
	return TRUE;
}

LRESULT CWindowAdvanceDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

BEGIN_MESSAGE_MAP(CWindowAdvanceDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CWindowAdvanceDlg::OnBnClickedSelectOnDwg)
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

void CWindowAdvanceDlg::InitGlassSeries()
{
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
	if (!m_selAttrWindows.empty())
		defValue = m_selAttrWindows[0]->m_material.sGlassSerial;
	TYUI_InitComboBox(m_boLi, glassOptions, defValue);
}

void CWindowAdvanceDlg::InitAluminumSeries()
{
	vector<CString> options, prevOptions, currOptions;
	CString defValue;

	for (UINT i = 0; i < m_selAttrWindows.size(); i++)
	{
		AttrWindow* pAttrWindow = m_selAttrWindows[i];
		CString prototype = pAttrWindow->GetMainPrototypeCode();
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
	if (!m_selAttrWindows.empty())
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
