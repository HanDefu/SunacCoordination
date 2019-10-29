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
#include "../Common/ComFun_ACad.h"

// CWindowDlg 对话框

IMPLEMENT_DYNAMIC(CWindowDlg, CAcUiDialog)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CWindowDlg::IDD, pParent)
	, m_radioDoor(1)
	, m_radioYes(0)
	, m_autoIndex(FALSE)
{
	m_isMoldless = true;
	m_pCurEdit = NULL;
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
	DDX_Control(pDX, IDC_COMBO_OPENWIDTH, m_W1);
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
	ON_BN_CLICKED(IDC_CHECK_IMAGE, &CWindowDlg::OnBnClickedMirror)
	ON_BN_CLICKED(IDC_RADIO_YES, &CWindowDlg::OnBnClickedBayWindow)
	ON_BN_CLICKED(IDC_RADIO_NO, &CWindowDlg::OnBnClickedBayWindow)
	ON_NOTIFY(GVN_SELCHANGED, IDC_PREVIEW_WINDOW, &CWindowDlg::OnSelChangedPreview)
	ON_CBN_SELCHANGE(IDC_COMBO_OPENWIDTH, &CWindowDlg::OnSelChangedW1)
	ON_CBN_SELCHANGE(IDC_COMBO_FIXEDVALUE, &CWindowDlg::OnSelChangedH2)
	ON_CBN_SELCHANGE(IDC_COMBO_DISTANCE, &CWindowDlg::OnSelChangedWallDis)
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
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	CString sNumber = TYUI_GetText(m_number);
	if (!m_autoIndex && !CWindowAutoName::GetInstance()->IsNameValid(*pSel,sNumber))
	{
		AfxMessageBox(L"此编号已被占用");
		return;
	}
	else
	{
		pSel->SetInstanceCode(sNumber);
		CWindowAutoName::GetInstance()->RenameWindow(*pSel);
	}

	ShowWindow(FALSE);

	AcGePoint3d origin;
	if (m_pCurEdit == NULL)
	{
		origin = TY_GetPoint();
		acedPostCommandPrompt();
	}
	else
	{
		AcDbExtents ext;
		m_pCurEdit->getGeomExtents(ext);
		origin = ext.minPoint();
		JHCOM_DeleteCadObject(m_pCurEdit->objectId());
	}

	double width = TYUI_GetDouble(m_width);
	double height = TYUI_GetDouble(m_height);
	double W1 = TYUI_GetDouble(m_W1);
	double H2 = TYUI_GetDouble(m_H2);

	RCWindow oneWindow;

	int sel = m_viewDir.GetCurSel();
	if (sel == 0)
		oneWindow.Insert(TY_GetPrototypeFilePath() + pSel->m_frontViewFile.fileName, origin, 0, L"0", 256);
	else if (sel == 1)
		oneWindow.Insert(TY_GetPrototypeFilePath() + pSel->m_topViewFile.fileName, origin, 0, L"0", 256);
	else
		oneWindow.Insert(TY_GetPrototypeFilePath() + pSel->m_leftViewFile.fileName, origin, 0, L"0", 256);

	oneWindow.InitParameters();

	oneWindow.SetParameter(L"H", height);
	oneWindow.SetParameter(L"W", width);
	oneWindow.SetParameter(L"W1", W1);
	oneWindow.SetParameter(L"H2", H2);

	oneWindow.RunParameters();

	if (m_isMirror.GetCheck())
	{
		AcGePoint3d basePt(origin.x + width / 2, 0, 0);
		TYCOM_Mirror(oneWindow.m_id, basePt, AcGeVector3d(0, 1, 0));
	}

	CWindowAutoName::GetInstance()->AddWindowType(*pSel);

	//把UI的数据记录在图框的扩展字典中
	AttrWindow * pWindow = new AttrWindow(*pSel);
	oneWindow.AddAttribute(pWindow);
	pWindow->close();

	ShowWindow(TRUE);
	//OnOK();
}

void CWindowDlg::OnBnClickedButtonSearchwindow()
{
	double width = TYUI_GetDouble(m_width);
	double height = TYUI_GetDouble(m_height);
	CString openType = TYUI_GetComboBoxText(m_openType);
	int openNum = _ttoi(TYUI_GetComboBoxText(m_openAmount));
	CString areaType = TYUI_GetComboBoxText(m_areaType);

	if (m_radioDoor == 0)
	{
		openType = TYUI_GetText(m_doorType);
	    m_allWindows = WebIO::GetInstance()->GetDoors(width, height, openType, openNum, areaType);
	}
	else
		m_allWindows = WebIO::GetInstance()->GetWindows(width, height, openType, openNum, areaType);

	m_preWindow.ClearAllPreviews();

	for (UINT i = 0; i < m_allWindows.size(); i++)
	{
		CString dwgPath = TY_GetPrototypeFilePath() + m_allWindows[i].GetFileName();
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
		CString str;
		str.Format(L"原型编号：\n%s\n窗户面积：%.2lf\n通风量：%.2lf\n动态类型：%s\n适用范围：集团", m_allWindows[i].m_prototypeCode, width * height / 1E6, m_allWindows[i].GetTongFengQty(false), m_allWindows[i].m_isDynamic ? L"动态" : L"静态");

		CString dwgPath = TY_GetPrototypeFilePath() + m_allWindows[i].GetFileName();
		CString pngPath = TY_GetPrototypeImagePath_Local() + m_allWindows[i].GetFileName(); //门窗原型优先使用内部的图片
		pngPath.Replace(L".dwg", L".png");
		if (PathFileExists(pngPath))
			m_preWindow.AddPreview(i, 0, pngPath, str);
		else
			m_preWindow.AddPreview(i, 0, dwgPath, str);
	}

	m_preWindow.SelectPreview(0, 0);
}

void CWindowDlg::OnBnClickedRadioDoor()
{
	int preVal = m_radioDoor;
	UpdateData(TRUE);
	if (m_radioDoor != preVal)
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
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	UpdateData(TRUE);
	if (m_autoIndex)
	{
		m_number.SetReadOnly(TRUE);
		CString newName = CWindowAutoName::GetInstance()->GetWindowName(*pSel);
		TYUI_SetText(m_number, newName);
	}
	else
		m_number.SetReadOnly(FALSE);
}

void CWindowDlg::OnBnClickedSelOnDwg()
{
	ShowWindow(false);
	TYRect rect = TY_GetOneRect();
	ShowWindow(true);
	
	int width = int(rect.GetWidth() + 0.5);
	int height = int(rect.GetHeight() + 0.5);

	TYUI_SetInt(m_width, width);
	TYUI_SetInt(m_height, height);
}

void CWindowDlg::OnBnClickedMirror()
{
	UpdateData(TRUE);
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;
	pSel->m_isMirror = (m_isMirror.GetCheck() != FALSE);
	UpdateInstanceCode();
}

void CWindowDlg::OnSelChangedPreview(NMHDR *pNMHDR, LRESULT *pResult)
{
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	const CWindowsDimData* dimW1 = pSel->GetDimData(L"W1");
	if (dimW1 != NULL)
	{
		if ((dimW1->type == SINGLE) || (dimW1->type == MULTI))
		{
			TYUI_Enable(m_W1);
			TYUI_InitComboBox(m_W1, dimW1->valueOptions, dimW1->value);
		}
		else
			TYUI_Disable(m_W1);
	}

	const CWindowsDimData* dimH2 = pSel->GetDimData(L"H2");
	if (dimH2 != NULL)
	{
		if ((dimH2->type == SINGLE) || (dimH2->type == MULTI))
		{
			TYUI_Enable(m_H2);
			TYUI_InitComboBox(m_H2, dimH2->valueOptions, dimH2->value);
		}
		else
			TYUI_Disable(m_H2);
	}

	pSel->m_instanceCode = CWindowAutoName::GetInstance()->GetWindowName(*pSel);
	TYUI_SetText(m_number, pSel->m_instanceCode);
	m_radioYes = (pSel->m_isBayWindow ? 0 : 1);
	m_isMirror.SetCheck(pSel->m_isMirror);
	TYUI_SetInt(m_distance, int(pSel->m_wallDis));

	m_viewDir.ResetContent();
	if ((!pSel->m_frontViewFile.fileName.IsEmpty()) && PathFileExists(TY_GetPrototypeFilePath() + pSel->m_frontViewFile.fileName))
		m_viewDir.AddString(L"立面");
	if ((!pSel->m_topViewFile.fileName.IsEmpty()) && PathFileExists(TY_GetPrototypeFilePath() + pSel->m_topViewFile.fileName))
		m_viewDir.AddString(L"平面");
	if ((!pSel->m_leftViewFile.fileName.IsEmpty()) && PathFileExists(TY_GetPrototypeFilePath() + pSel->m_leftViewFile.fileName))
		m_viewDir.AddString(L"侧视");
	if (m_viewDir.GetCount() > 0)
		m_viewDir.SetCurSel(0);

	UpdateData(FALSE);
}

void CWindowDlg::OnSelChangedW1()
{
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	CString sSel = TYUI_GetComboBoxText(m_W1);
	pSel->SetW1(_ttoi(sSel));
	//更改参数会引起实例编号变化，需更新
	UpdateInstanceCode();
}

void CWindowDlg::OnSelChangedH2()
{
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	CString sSel = TYUI_GetComboBoxText(m_H2);
	pSel->SetH2(_ttoi(sSel));
	//更改参数会引起实例编号变化，需更新
	UpdateInstanceCode();
}

void CWindowDlg::OnBnClickedBayWindow()
{
	UpdateData(TRUE);
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	pSel->m_isBayWindow = (m_radioYes == 0);
}

void CWindowDlg::OnSelChangedWallDis()
{
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;

	pSel->m_wallDis = _ttof(TYUI_GetComboBoxText(m_distance));
}

void CWindowDlg::UpdateEnable()
{
	//切换后清除列表，避免插入时错误
	m_allWindows.clear();
	m_preWindow.ClearAllPreviews();

	if (m_radioDoor == 0)
	{
		TYUI_Enable(m_doorType);

		TYUI_Disable(m_openType);
		TYUI_Disable(m_openAmount);
		TYUI_Disable(m_W1);
		TYUI_Disable(*GetDlgItem(IDC_RADIO_YES));
		TYUI_Disable(*GetDlgItem(IDC_RADIO_NO));
	}
	else
	{
		TYUI_Enable(m_openType);
		TYUI_Enable(m_openAmount);
		TYUI_Enable(m_W1);
		TYUI_Enable(*GetDlgItem(IDC_RADIO_YES));
		TYUI_Enable(*GetDlgItem(IDC_RADIO_NO));

		TYUI_Disable(m_doorType);
	}
	LoadDefaultValue();
}

void CWindowDlg::UpdateInstanceCode()
{
	AttrWindow* pSel = GetSelWindow();
	if (pSel == NULL)
		return;
	if (m_autoIndex)
	{
		pSel->m_instanceCode = CWindowAutoName::GetInstance()->GetWindowName(*pSel);
		TYUI_SetText(m_number, pSel->m_instanceCode);
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
	TYUI_SetText(m_number, L"");
	
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

AttrWindow* CWindowDlg::GetSelWindow()
{
	CCellRange sels = m_preWindow.GetSelectedCellRange();
	if (sels.Count() == 0)
		return NULL;
	return &m_allWindows[sels.GetMinRow()];
}

void CWindowDlg::SetEditMode(AcDbBlockReference* pBlock)
{
	m_pCurEdit = pBlock;
	if (m_pCurEdit == NULL)
		return;

	AcDbObject* pAtt = NULL;
	TY_GetAttributeData(pBlock->objectId(), pAtt);
	AttrWindow *pWindow = dynamic_cast<AttrWindow *>(pAtt);
	if (pWindow == NULL)
		return;

	m_radioDoor = (pWindow->GetType() == DOOR) ? 0 : 1;
	UpdateEnable();

	//还原门窗尺寸
	double width = pWindow->GetW();
	double height =  pWindow->GetH();
	TYUI_SetInt(m_width, int(width));
	TYUI_SetInt(m_height, int(height));

	m_allWindows.clear();
	m_allWindows.push_back(*pWindow);

	//////////////////////////////////////////////////////////////////////////
	//3. 显示原型
	m_preWindow.ClearAllPreviews();
	m_preWindow.SetRowCount(1);
	m_preWindow.SetColumnCount(1);
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(1);

	CString str;
	str.Format(L"原型编号：\n%s\n窗户面积：%.2lf\n通风量：%.2lf\n动态类型：%s\n适用范围：集团", m_allWindows[0].m_prototypeCode, width * height / 1E6, m_allWindows[0].GetTongFengQty(false), m_allWindows[0].m_isDynamic ? L"动态" : L"静态");

	CString dwgPath = TY_GetPrototypeFilePath() + m_allWindows[0].GetFileName();
	CString pngPath = TY_GetPrototypeImagePath_Local() + m_allWindows[0].GetFileName(); //门窗原型优先使用内部的图片
	pngPath.Replace(L".dwg", L".png");
	if (PathFileExists(pngPath))
		m_preWindow.AddPreview(0, 0, pngPath, str);
	else
		m_preWindow.AddPreview(0, 0, dwgPath, str);

	m_preWindow.SelectPreview(0, 0);

	TYUI_SetText(*GetDlgItem(IDC_BUTTON_INSERTWINDOW), L"确定");
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
