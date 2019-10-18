// UI\WindowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowDlg.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/WindowDoor/RCWindow.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "../WebIO/WebIO.h"
#include "../GlobalSetting.h"
#include "../Object/WindowDoor/WindowAutoName.h"

// CWindowDlg 对话框

IMPLEMENT_DYNAMIC(CWindowDlg, CAcUiDialog)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CWindowDlg::IDD, pParent)
	, m_radioDoor(1)
	, m_radioYes(0)
	, m_autoIndex(FALSE)
{
	m_isMoldless = true;
}

CWindowDlg::~CWindowDlg()
{
}

INT_PTR CWindowDlg::DoModal()
{
	m_isMoldless = false;
	return CAcUiDialog::DoModal();
}

LRESULT CWindowDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

void CWindowDlg::OnOK()
{
	CAcUiDialog::OnOK();
	if (m_isMoldless)
		DestroyWindow();
}

void CWindowDlg::OnCancel()
{
	CAcUiDialog::OnCancel();
	if (m_isMoldless)
	    DestroyWindow();
}

void CWindowDlg::PostNcDestroy()
{
	CAcUiDialog::PostNcDestroy();
	if (m_isMoldless)
	{
		delete this;
		g_windowDlg = NULL;
	}
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
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CWindowDlg::OnBnClickedSelOnDwg)
	ON_NOTIFY(GVN_SELCHANGED, IDC_PREVIEW_WINDOW, &CWindowDlg::OnSelChangedPreview)
	ON_CBN_SELCHANGE(IDC_COMBO_OPENWIDTH, &CWindowDlg::OnSelChangedW1)
	ON_CBN_SELCHANGE(IDC_COMBO_FIXEDVALUE, &CWindowDlg::OnSelChangedH2)
END_MESSAGE_MAP()


// CWindowDlg 消息处理程序


BOOL CWindowDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_preWindow.LoadDefaltSettings();

	LoadDefaultValue();
	UpdateEnable();

	return TRUE;
}

void CWindowDlg::OnBnClickedButtonInsert()
{
	vector<int> sels = m_preWindow.GetSelectedRows();
	if (sels.size() == 0)
	{
		AfxMessageBox(L"没有选择原型，请重新选择或者双击原型\n");
		return;
	}
	AttrWindow &selWindow = m_allWindows[sels[0]];

	ShowWindow(FALSE);

	AcGePoint3d origin = TY_GetPoint();
	acedPostCommandPrompt();

	double width = TYUI_GetDouble(m_width);
	double height = TYUI_GetDouble(m_height);
	double W1 = TYUI_GetDouble(m_openWidth);
	double H2 = TYUI_GetDouble(m_H2);

	selWindow.SetW1(W1);
	selWindow.SetH2(H2);

	RCWindow oneWindow;

	int sel = m_viewDir.GetCurSel();
	if (sel == 0)
		oneWindow.Insert(TY_GetLocalFilePath() + selWindow.m_frontViewFile.fileName, origin, 0, L"0", 256);
	else if (sel == 1)
		oneWindow.Insert(TY_GetLocalFilePath() + selWindow.m_topViewFile.fileName, origin, 0, L"0", 256);
	else
		oneWindow.Insert(TY_GetLocalFilePath() + selWindow.m_leftViewFile.fileName, origin, 0, L"0", 256);

	oneWindow.InitParameters();

	oneWindow.SetParameter(L"H", height);
	oneWindow.SetParameter(L"W", width);
	oneWindow.SetParameter(L"W1", W1);
	oneWindow.SetParameter(L"H2", H2);

	oneWindow.RunParameters();

	if (m_isMirror.GetCheck())
		TYCOM_Mirror(oneWindow.m_id, origin, AcGeVector3d(0,1,0));

	CWindowAutoName::GetInstance()->AddWindowType(selWindow);

	//把UI的数据记录在图框的扩展字典中
	AttrWindow * pWindow = new AttrWindow(m_allWindows[sels[0]]);
	oneWindow.AddAttribute(pWindow);
	pWindow->close();

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
	    m_allWindows = WebIO::GetInstance()->GetDoors(width, height, openType, openNum, areaType);
	else
		m_allWindows = WebIO::GetInstance()->GetWindows(width, height, openType, openNum, areaType);

	m_preWindow.ClearAllPreviews();

	for (UINT i = 0; i < m_allWindows.size(); i++)
	{
		CString dwgPath = TY_GetLocalFilePath() + m_allWindows[i].GetFileName();
		if (!PathFileExists(dwgPath))
		{
			acutPrintf(L"\n原型文件" + m_allWindows[i].GetFileName() + L"未找到\n");
			m_allWindows.erase(m_allWindows.begin() + i--);
		}
	}
	if (m_allWindows.empty())
	{
		AfxMessageBox(L"未找到符合条件的记录\n");
		return;
	}

	m_preWindow.SetRowCount((int)m_allWindows.size());
	m_preWindow.SetColumnCount(1);
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(1);

	for (UINT i = 0; i < m_allWindows.size(); i++)
	{
		m_allWindows[i].SetW(width);
		m_allWindows[i].SetH(height);
		m_allWindows[i].m_instanceCode = CWindowAutoName::GetInstance()->GetWindowName(m_allWindows[i]);
		CString str;
		str.Format(L"原型编号：%s\n窗户面积：%.2lf\n通风量：%.2lf\n动态类型：动态\n适用范围：集团", m_allWindows[i].m_prototypeCode, width * height / 1E6, m_allWindows[i].GetTongFengQty(false));
		CString dwgPath = TY_GetLocalFilePath() + m_allWindows[i].GetFileName();
		CString pngPath = dwgPath;
		pngPath.Replace(L"\\LocalMode", L"\\Image");
		pngPath.Replace(L".dwg", L".png");
		if (PathFileExists(pngPath))
			m_preWindow.AddPreviewPng(i, 0, pngPath, str);
		else
			m_preWindow.AddPreview(i, 0, dwgPath, str);
	}

	m_preWindow.SelectPreview(0, 0);
}

void CWindowDlg::OnBnClickedRadioDoor()
{
	UpdateData(TRUE);
	UpdateEnable();
}

void CWindowDlg::OnBnClickedCalculate()
{
	double area = TYUI_GetDouble(m_area);

	CString sRate = TYUI_GetText(m_rate);
	double rate = _ttof(sRate);
	int pos = sRate.Find(L'/');
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

void CWindowDlg::OnBnClickedSelOnDwg()
{
	ShowWindow(false);
	TYRect rect = TY_GetOneRect();
	ShowWindow(true);

	TYUI_SetDouble(m_width, rect.GetWidth());
	TYUI_SetDouble(m_height, rect.GetHeight());
}

void CWindowDlg::OnSelChangedPreview(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GRIDVIEW* pView = (NM_GRIDVIEW*)pNMHDR;
	int nSel = pView->iRow;
	if (nSel >= 0 && nSel < (int)m_allWindows.size())
	{
		const CWindowsDimData* dimW1 = m_allWindows[nSel].GetDimData(L"W1");
		if (dimW1!=NULL)
		{
			if ((dimW1->type == SINGLE) || (dimW1->type == MULTI))
			{
				TYUI_Enable(m_openWidth);
				TYUI_InitComboBox(m_openWidth, dimW1->valueOptions, dimW1->defaultValue);
			}
			else
				TYUI_Disable(m_openWidth);
		}

		const CWindowsDimData* dimH2 = m_allWindows[nSel].GetDimData(L"H2");
		if (dimH2 != NULL)
		{
			if ((dimH2->type == SINGLE) || (dimH2->type == MULTI))
			{
				TYUI_Enable(m_H2);
				TYUI_InitComboBox(m_H2, dimH2->valueOptions, dimH2->defaultValue);
			}
			else
				TYUI_Disable(m_H2);
		}

		TYUI_SetText(m_number, m_allWindows[nSel].m_instanceCode);
		m_radioYes = (m_allWindows[nSel].m_isBayWindow ? 0 : 1);

		m_viewDir.ResetContent();
		if (PathFileExists(TY_GetLocalFilePath() + m_allWindows[nSel].m_frontViewFile.fileName))
			m_viewDir.AddString(L"立面");
		if (PathFileExists(TY_GetLocalFilePath() + m_allWindows[nSel].m_topViewFile.fileName))
			m_viewDir.AddString(L"平面");
		if (PathFileExists(TY_GetLocalFilePath() + m_allWindows[nSel].m_leftViewFile.fileName))
			m_viewDir.AddString(L"侧视");
		if (m_viewDir.GetCount() > 0)
			m_viewDir.SetCurSel(0);

		UpdateData(FALSE);
	}
}

void CWindowDlg::OnSelChangedW1()
{
	vector<int> sels = m_preWindow.GetSelectedRows();
	if (sels.empty())
		return;
	int nSel = sels[0];

	CString sSel = TYUI_GetComboBoxText(m_openWidth);
	m_allWindows[nSel].SetW1(_ttoi(sSel));
	m_allWindows[nSel].m_instanceCode = CWindowAutoName::GetInstance()->GetWindowName(m_allWindows[nSel]);
	TYUI_SetText(m_number, m_allWindows[nSel].m_instanceCode);
}

void CWindowDlg::OnSelChangedH2()
{
	vector<int> sels = m_preWindow.GetSelectedRows();
	if (sels.empty())
		return;
	int nSel = sels[0];

	CString sSel = TYUI_GetComboBoxText(m_H2);
	m_allWindows[nSel].SetH2(_ttoi(sSel));
	m_allWindows[nSel].m_instanceCode = CWindowAutoName::GetInstance()->GetWindowName(m_allWindows[nSel]);
	TYUI_SetText(m_number, m_allWindows[nSel].m_instanceCode);
}

void CWindowDlg::UpdateEnable()
{
	if (m_radioDoor == 0)
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
	const vCString& doorTypes = WebIO::GetInstance()->GetConfigDict()->Door_GetTypes();
	const vCString& openTypes = WebIO::GetInstance()->GetConfigDict()->Window_GetOpenTypes();
	const vCString& areaTypes = WebIO::GetInstance()->GetConfigDict()->GetGongNengQus();
	const vCString& openAmount = WebIO::GetInstance()->GetConfigDict()->Window_GetOpenAmount();
	const vCString& rate = WebIO::GetInstance()->GetConfigDict()->Window_GetRate();
	const vCString& wallDis = WebIO::GetInstance()->GetConfigDict()->Window_GetWallDis();

	TYUI_SetInt(m_width, 1500);
	TYUI_SetInt(m_height, 1800);
	TYUI_SetInt(m_ventilation, 0);
	TYUI_SetInt(m_area, 0);
	
	TYUI_InitComboBox(m_doorType, doorTypes, doorTypes.empty() ? L"" : doorTypes[0]);
	TYUI_InitComboBox(m_openType, openTypes, openTypes.empty()? L"" : openTypes[0]);
	TYUI_InitComboBox(m_areaType, areaTypes, areaTypes.empty() ? L"" : areaTypes[0]);
	TYUI_InitComboBox(m_openAmount, openAmount, openAmount.empty() ? L"" : openAmount[0]);
	TYUI_InitComboBox(m_rate, rate, rate.empty() ? L"" : rate[0]);
	TYUI_InitComboBox(m_distance, wallDis, wallDis.empty() ? L"" : wallDis[0]);

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
