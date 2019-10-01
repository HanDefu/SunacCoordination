// UI\WindowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/WindowDoor/RCWindow.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "../WebIO/WebIO.h"
#include "../GlobalSetting.h"
#include "../WebIO/WindowLocalData.h"

// CWindowDlg 对话框

IMPLEMENT_DYNAMIC(CWindowDlg, CAcUiDialog)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CWindowDlg::IDD, pParent)
	, m_radioDoor(0)
	, m_radioYes(0)
	, m_autoIndex(FALSE)
{

}

CWindowDlg::~CWindowDlg()
{
}

LRESULT CWindowDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

void CWindowDlg::OnOK()
{
	CAcUiDialog::OnOK();
	DestroyWindow();
}

void CWindowDlg::OnCancel()
{
	CAcUiDialog::OnCancel();
	DestroyWindow();
}

void CWindowDlg::PostNcDestroy()
{
	CAcUiDialog::PostNcDestroy();
	delete this;
	g_windowDlg = NULL;
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_WINDOW, m_preWindow);
	DDX_Control(pDX, IDC_COMBO_DOORTYPE, m_doorType);
	DDX_Control(pDX, IDC_COMBO_AREATYPE, m_areaType);
	DDX_Control(pDX, IDC_COMBO_OPENTYPE, m_openType);
	DDX_Control(pDX, IDC_COMBO_OPENAMOUNT, m_openAmount);
	DDX_Control(pDX, IDC_EDIT_VENTILATION, m_ventilation);
	DDX_Control(pDX, IDC_COMBO_OPENWIDTH, m_openWidth);
	DDX_Control(pDX, IDC_COMBO_FIXEDVALUE, m_H2);
	DDX_Control(pDX, IDC_COMBO_DISTANCE, m_distance);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_width);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_height);
	DDX_Control(pDX, IDC_EDIT_NUMBER, m_number);
	DDX_Radio(pDX, IDC_RADIO_DOOR, m_radioDoor);
	DDX_Radio(pDX, IDC_RADIO_YES, m_radioYes);
	DDX_Control(pDX, IDC_EDIT_AREA, m_area);
	DDX_Control(pDX, IDC_COMBO_RATE, m_rate);
	DDX_Check(pDX, IDC_CHECK_AUTOINDEX, m_autoIndex);
	DDX_Control(pDX, IDC_COMBO_VIEWDIR, m_viewDir);
	DDX_Control(pDX, IDC_CHECK_IMAGE, m_isMirror);
}


BEGIN_MESSAGE_MAP(CWindowDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_INSERTWINDOW, &CWindowDlg::OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHWINDOW, &CWindowDlg::OnBnClickedButtonSearchwindow)
	ON_BN_CLICKED(IDC_RADIO_DOOR, &CWindowDlg::OnBnClickedRadioDoor)
	ON_BN_CLICKED(IDC_RADIO_WINDOW, &CWindowDlg::OnBnClickedRadioDoor)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, &CWindowDlg::OnBnClickedCalculate)
	ON_BN_CLICKED(IDC_CHECK_AUTOINDEX, &CWindowDlg::OnBnClickedAutoIndex)
	ON_NOTIFY(GVN_SELCHANGED, IDC_PREVIEW_WINDOW, &CWindowDlg::OnSelChanged)
END_MESSAGE_MAP()


// CWindowDlg 消息处理程序


BOOL CWindowDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_preWindow.LoadDefaltSettings();

	LoadDefaultValue();
	SetRadioDoor(0);

	return TRUE;
}

void CWindowDlg::OnBnClickedButtonInsert()
{
	vector<int> sels = m_preWindow.GetSelectedRows();
	if (sels.size() == 0)
	{
		acutPrintf(_T("没有选择原型，请重新选择或者双击原型\n"));
		return;
	}

	ShowWindow(FALSE);

	// TODO: 在此添加控件通知处理程序代码
	AcGePoint3d origin = TY_GetPoint();
	acedPostCommandPrompt();

	double width = TYUI_GetDouble(m_width);
	double height = TYUI_GetDouble(m_height);
	double W1 = TYUI_GetDouble(m_openWidth);
	double H2 = TYUI_GetDouble(m_H2);

	if (sels.size() > 0)
	{
		RCWindow oneWindow;
			
		int sel = m_viewDir.GetCurSel();
		if (sel == 0)
		    oneWindow.Insert(TY_GetLocalFilePath() + m_allWindows[sels[0]].m_name, origin, 0, L"0", 256);
		else if (sel == 1)
            oneWindow.Insert(TY_GetLocalFilePath() + m_allWindows[sels[0]].m_prototypeFlatFile, origin, 0, L"0", 256);
		else
			oneWindow.Insert(TY_GetLocalFilePath() + m_allWindows[sels[0]].m_prototypeTopViewFile, origin, 0, L"0", 256);

		oneWindow.InitParameters();
		oneWindow.SetParameter(L"H", height);
		oneWindow.SetParameter(L"W", width);

		oneWindow.SetParameter(L"W1", W1);
		oneWindow.SetParameter(L"H2", H2);

		oneWindow.RunParameters();

		CString str;
		str.Format(L"%d_%d",(int)(oneWindow.GetW()), (int)(oneWindow.GetH()));
			
		if (m_isMirror.GetCheck())
		    TYCOM_Mirror(oneWindow.m_id, origin, AcGeVector3d(0,1,0));

		//把UI的数据记录在图框的扩展字典中
		AttrWindow * pWindow = new AttrWindow(m_allWindows[sels[0]]);
		oneWindow.AddAttribute(pWindow);
		pWindow->close();

		oneWindow.SetBianHao(m_allWindows[sels[0]].m_yxid + str);
	}
	OnOK();
}


void CWindowDlg::OnBnClickedButtonSearchwindow()
{
	double width = TYUI_GetDouble(m_width);
	double height = TYUI_GetDouble(m_height);
	CString openType = TYUI_GetText(m_openType);
	int openNum = TYUI_GetInt(m_openAmount);
	CString areaType = TYUI_GetText(m_areaType);

	if (m_radioDoor == 0)
	    m_allWindows = WebIO::GetWindowLocalData()->GetDoors(width, openType, openNum, areaType);
	else
		m_allWindows = WebIO::GetWindowLocalData()->GetWindows(width, openType, openNum, areaType);

	m_preWindow.ClearAllPreviews();
	if (m_allWindows.empty())
	{
		acutPrintf(_T("未找到符合条件的记录\n"));
		return;
	}
	m_preWindow.SetRowCount((int)m_allWindows.size());
	m_preWindow.SetColumnCount(1);
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(1);
	for (int i = 0; i < m_allWindows.size(); i++)
	{
		CString str;
		str.Format(_T("原型编号：%s\n窗户面积：%.2lf\n通风量：0.9\n动态类型：动态\n适用范围：集团"), m_allWindows[i].m_yxid, width * height / 1E6);
		m_preWindow.AddPreview(i, 0, TY_GetLocalFilePath() + m_allWindows[i].m_name, str);
	}

	m_preWindow.SelectPreview(0, 0);
}

void CWindowDlg::OnBnClickedRadioDoor()
{
	UpdateData(TRUE);
	SetRadioDoor(m_radioDoor);
}

void CWindowDlg::OnBnClickedCalculate()
{
	double area = TYUI_GetDouble(m_area);

	CString sRate = TYUI_GetText(m_rate);
	double rate = _ttof(sRate);
	int pos = sRate.Find(_T('/'));
	if (pos != -1)
		rate /= _ttof(sRate.Mid(pos + 1));

	TYUI_SetDouble(m_ventilation, area * rate);
}

void CWindowDlg::OnBnClickedAutoIndex()
{
	UpdateData(TRUE);
	if (m_autoIndex)
		m_number.SetReadOnly(TRUE);
	else
		m_number.SetReadOnly(FALSE);
}

void CWindowDlg::OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GRIDVIEW* pView = (NM_GRIDVIEW*)pNMHDR;
	int nSel = pView->iRow;
	if (nSel >= 0 && nSel < m_allWindows.size())
	{
		vSRCDimData& vDimData = m_allWindows[nSel].m_dimData;
		for (UINT i = 0; i < vDimData.size(); i++)
		{
			if (vDimData[i].sCodeName == _T("W1"))
			{
				if ((vDimData[i].type == SINGLE) || (vDimData[i].type == MULTI))
				{
					TYUI_Enable(m_openWidth);
					TYUI_InitComboBox(m_openWidth, vDimData[i].values, vDimData[i].defaultValue);
				}
				else
					TYUI_Disable(m_openWidth);
			}
			if (vDimData[i].sCodeName == _T("H2"))
			{
				if ((vDimData[i].type == SINGLE) || (vDimData[i].type == MULTI))
				{
					TYUI_Enable(m_H2);
					TYUI_InitComboBox(m_H2, vDimData[i].values, vDimData[i].defaultValue);
				}
				else
					TYUI_Disable(m_H2);
			}
		}

		TYUI_SetText(m_number, m_allWindows[nSel].m_yxid);
		m_radioYes = (m_allWindows[nSel].m_isBayWindow ? 0 : 1);
		UpdateData(FALSE);
	}
}

void CWindowDlg::SetRadioDoor(int radioDoor)
{
	if (radioDoor < 0 || radioDoor > 1)
		return;
	m_radioDoor = radioDoor;

	if (radioDoor == 0)
	{
		TYUI_Enable(m_doorType);

		TYUI_Disable(m_openType);
		TYUI_Disable(m_openAmount);
		TYUI_Disable(m_openWidth);
		TYUI_Disable(*GetDlgItem(IDC_RADIO_YES));
		TYUI_Disable(*GetDlgItem(IDC_RADIO_NO));
	}
	else
	{
		TYUI_Enable(m_openType);
		TYUI_Enable(m_openAmount);
		TYUI_Enable(m_openWidth);
		TYUI_Enable(*GetDlgItem(IDC_RADIO_YES));
		TYUI_Enable(*GetDlgItem(IDC_RADIO_NO));

		TYUI_Disable(m_doorType);
	}
}

void CWindowDlg::LoadDefaultValue()
{
	const vCString& doorTypes = WebIO::GetConfigDict()->Door_GetTypes();
	const vCString& openTypes = WebIO::GetConfigDict()->Window_GetOpenTypes();
	const vCString& areaTypes = WebIO::GetConfigDict()->GetGongNengQus();
	const vCString& openAmount = WebIO::GetConfigDict()->Window_GetOpenAmount();
	const vCString& rate = WebIO::GetConfigDict()->Window_GetRate();
	const vCString& wallDis = WebIO::GetConfigDict()->Window_GetWallDis();

	TYUI_SetInt(m_width, 1500);
	TYUI_SetInt(m_height, 1200);
	TYUI_SetInt(m_ventilation, 2);
	TYUI_SetInt(m_area, 8);
	
	TYUI_InitComboBox(m_doorType, doorTypes, doorTypes.empty() ? _T("") : doorTypes[0]);
	TYUI_InitComboBox(m_openType, openTypes, openTypes.empty()? _T("") : openTypes[0]);
	TYUI_InitComboBox(m_areaType, areaTypes, areaTypes.empty() ? _T("") : areaTypes[0]);
	TYUI_InitComboBox(m_openAmount, openAmount, openAmount.empty() ? _T("") : openAmount[0]);
	TYUI_InitComboBox(m_rate, rate, rate.empty() ? _T("") : rate[0]);
	TYUI_InitComboBox(m_distance, wallDis, wallDis.empty() ? _T("") : wallDis[0]);

	m_viewDir.SetCurSel(0);
	m_autoIndex = TRUE;
	m_number.SetReadOnly(TRUE);
	UpdateData(FALSE);
}

CWindowDlg* g_windowDlg = NULL;

BOOL CloseWindowDlg()
{
	if (g_windowDlg == NULL)
		return TRUE;
	BOOL ret = g_windowDlg->DestroyWindow();
	if (ret)
		g_windowDlg = NULL;
	return ret;
}
