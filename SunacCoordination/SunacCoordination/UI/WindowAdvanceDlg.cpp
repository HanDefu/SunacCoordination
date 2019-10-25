// UI\WindowAdvanceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowAdvanceDlg.h"
#include "../Common/ComFun_Interactive.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/WindowStatistic/AluminumSeries.h"
#include "ComFun_MFC.h"


// CWindowAdvanceDlg 对话框

IMPLEMENT_DYNAMIC(CWindowAdvanceDlg, CAcUiDialog)

CWindowAdvanceDlg::CWindowAdvanceDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CWindowAdvanceDlg::IDD, pParent)
	, m_sCode(_T(""))
{
	m_isMoldless = true;
	m_pAttrWindow = NULL;
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
	if (m_pAttrWindow != NULL)
	{
		m_pAttrWindow->m_material.sAluminumSerial = TYUI_GetComboBoxText(m_xingCai);
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
	ShowWindow(SW_HIDE);
	AcDbObjectId id = JHCOM_SelectJHSolidOne();
	AcDbObject* pAttr = NULL;
	TY_GetAttributeData(id, pAttr);
	AttrWindow* pAttrWindow = AttrWindow::cast(pAttr);
	if (pAttrWindow == NULL)
	{
		ShowWindow(SW_SHOW);
		return;
	}
	m_pAttrWindow = pAttrWindow;
	m_sCode = pAttrWindow->m_instanceCode;

	CString prototype = pAttrWindow->GetMainPrototypeCode();
	vector<CString> options = CAluminumSeries::Instance()->GetAluminumSerialsByPrototype(prototype);
	CString defValue = pAttrWindow->m_material.sAluminumSerial;
	TYUI_InitComboBox(m_xingCai, options, defValue);

	ShowWindow(SW_SHOW);
	UpdateData(FALSE);
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
