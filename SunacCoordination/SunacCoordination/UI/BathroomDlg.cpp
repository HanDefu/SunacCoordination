// UI\BathroomDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BathroomDlg.h"
#include "../WebIO/WebIO.h"
#include "../Object/Bathroom/BathroomAutoName.h"
#include "../Common/ComFun_ACad.h"

// CBathroomDlg 对话框

CBathroomDlg* g_bathroomDlg = NULL;

BOOL CloseBathroomDlg()
{
	if (g_bathroomDlg == NULL)
		return TRUE;
	BOOL ret = g_bathroomDlg->DestroyWindow();
	if (ret)
		g_bathroomDlg = NULL;
	return ret;
}


IMPLEMENT_DYNAMIC(CBathroomDlg, CAcUiDialog)

CBathroomDlg::CBathroomDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CBathroomDlg::IDD, pParent)
	, m_isStd(0)
{
	m_rect.SetLB(AcGePoint3d(0, 0, 0));
	m_rect.SetRT(AcGePoint3d(0, 0, 0));
	m_doorDir = E_DIR_UNKNOWN;
	m_windowDir = E_DIR_UNKNOWN;
	m_pBathroomGen = NULL;
	m_pCurEdit = NULL;
	bool lockUpdate = false;
}

CBathroomDlg::~CBathroomDlg()
{
	if (m_pBathroomGen != NULL)
	{
		delete m_pBathroomGen;
	}
}

void CBathroomDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_BATHROOM, m_preBathroom);
	DDX_Control(pDX, IDC_COMBO_BATHROOMTYPE, m_bathroomType);
	DDX_Control(pDX, IDC_CHECK_AIROUT, m_noAirOut);
	DDX_Control(pDX, IDC_EDIT_BATHROOMNUMBER, m_number);
	DDX_Control(pDX, IDC_COMBO_BASINWIDTH, m_basinWidth);
	DDX_Control(pDX, IDC_COMBO_TOILET_AREA, m_toiletWidth);
	DDX_Control(pDX, IDC_COMBO_WASHWIDTH, m_washWidth);
	DDX_Control(pDX, IDC_CHECK_IMAGE, m_isMirror);
	DDX_Control(pDX, IDC_CHECK_AUTOINDEX, m_autoIndex);
	DDX_Control(pDX, IDC_COMBO_FLOORRANGE, m_floorRange);
	DDX_Control(pDX, IDC_EDIT_OFFSETX, m_offsetX);
	DDX_Control(pDX, IDC_EDIT_OFFSETY, m_offsetY);
	DDX_Control(pDX, IDC_EDIT_X, m_customX);
	DDX_Control(pDX, IDC_EDIT_Y, m_customY);
	DDX_Radio(pDX, IDC_RADIO_STANDARD, m_isStd);
}


BEGIN_MESSAGE_MAP(CBathroomDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_RANGE, &CBathroomDlg::OnBnClickedButtonRange)
	ON_BN_CLICKED(IDC_BUTTON_DOORDIR, &CBathroomDlg::OnBnClickedButtonDoorDir)
	ON_BN_CLICKED(IDC_BUTTON_WINDOWDIR, &CBathroomDlg::OnBnClickedButtonWindowDir)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CBathroomDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_INSERTBATHROOM, &CBathroomDlg::OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_CHECK_AUTOINDEX, &CBathroomDlg::OnBnClickedAutoIndex)
	ON_NOTIFY(GVN_SELCHANGED, IDC_PREVIEW_BATHROOM, &CBathroomDlg::OnSelChanged)
	ON_CBN_SELCHANGE(IDC_COMBO_BASINWIDTH, &CBathroomDlg::UpdateAttribute)
	ON_CBN_SELCHANGE(IDC_COMBO_TOILET_AREA, &CBathroomDlg::UpdateAttribute)
	ON_CBN_SELCHANGE(IDC_COMBO_WASHWIDTH, &CBathroomDlg::UpdateAttribute)
	ON_CBN_SELCHANGE(IDC_COMBO_FLOORRANGE, &CBathroomDlg::UpdateAttribute)
	ON_BN_CLICKED(IDC_CHECK_IMAGE, &CBathroomDlg::UpdateAttribute)
	ON_BN_CLICKED(IDC_RADIO_STANDARD, &CBathroomDlg::UpdateAttribute)
	ON_BN_CLICKED(IDC_RADIO_CUSTOM, &CBathroomDlg::UpdateAttribute)
	ON_EN_CHANGE(IDC_EDIT_OFFSETX, &CBathroomDlg::UpdateAttribute)
	ON_EN_CHANGE(IDC_EDIT_OFFSETY, &CBathroomDlg::UpdateAttribute)
	ON_EN_CHANGE(IDC_EDIT_X, &CBathroomDlg::UpdateAttribute)
	ON_EN_CHANGE(IDC_EDIT_Y, &CBathroomDlg::UpdateAttribute)
END_MESSAGE_MAP()


// CBathroomDlg 消息处理程序


BOOL CBathroomDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_preBathroom.LoadDefaltSettings();
	LoadDefaultValue();

	EnableSetProperty(false);

	return TRUE;
}

void CBathroomDlg::OnOK()
{
	CAcUiDialog::OnOK();
	DestroyWindow();
}

void CBathroomDlg::OnCancel()
{
	CAcUiDialog::OnCancel();
	DestroyWindow();
}

void CBathroomDlg::PostNcDestroy()
{
	CAcUiDialog::PostNcDestroy();
	delete this;
	g_bathroomDlg = NULL;
}

BOOL CBathroomDlg::PreTranslateMessage(MSG *pMsg)
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
		return TRUE;
	return CAcUiDialog::PreTranslateMessage(pMsg);
}

bool CBathroomDlg::IsBathroomRectValid(TYRect rect)
{
	if (rect.GetWidth() < 1000 || rect.GetHeight() < 1000)
	{
		return false;
	}

	if (rect.GetWidth() > 10000 || rect.GetHeight() > 10000)
	{
		return false;
	}

	return true;
}

E_DIRECTION CBathroomDlg::GetDir(ads_point pt)
{
	double minDis = abs(pt[X] - m_rect.GetLB().x);
	E_DIRECTION dir = E_DIR_LEFT;
	if (abs(pt[X] - m_rect.GetRT().x) < minDis)
	{
		minDis = abs(pt[X] - m_rect.GetRT().x);
		dir = E_DIR_RIGHT;
	}
	if (abs(pt[Y] - m_rect.GetLB().y) < minDis)
	{
		minDis = abs(pt[Y] - m_rect.GetLB().y);
		dir = E_DIR_BOTTOM;
	}
	if (abs(pt[Y] - m_rect.GetRT().y) < minDis)
	{
		minDis = abs(pt[Y] - m_rect.GetRT().y);
		dir = E_DIR_TOP;
	}
	return dir;
}

void CBathroomDlg::EnableSetAirout(bool bEnable)
{
	GetDlgItem(IDC_RADIO_STANDARD)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_CUSTOM)->EnableWindow(bEnable);
	m_floorRange.EnableWindow(bEnable);
	m_offsetX.EnableWindow(bEnable);
	m_offsetY.EnableWindow(bEnable);
	m_customX.EnableWindow(bEnable);
	m_customY.EnableWindow(bEnable);
}

void CBathroomDlg::SetEditMode(AcDbBlockReference* pBlock)
{
	m_pCurEdit = pBlock;
	if (m_pCurEdit == NULL)
		return;

	AcDbObject* pAtt = NULL;
	TY_GetAttributeData(pBlock->objectId(), pAtt);
	AttrBathroom *pBathroom = dynamic_cast<AttrBathroom *>(pAtt);
	if (pBathroom == NULL)
		return;

	AcDbExtents ext;
	pBlock->getGeomExtents(ext);
	AcGePoint3d ptLB = ext.minPoint();

	//还原卫生间范围和门窗位置
	m_doorDir = E_DIRECTION((pBathroom->m_angle / 90 + pBathroom->m_prop.m_doorPos) % 4);
	m_windowDir = E_DIRECTION((pBathroom->m_angle / 90 + pBathroom->m_prop.m_windowPos) % 4);
	AcGePoint3d ptRT = ptLB;
	if (m_doorDir == E_DIR_BOTTOM || m_doorDir == E_DIR_TOP)
	{
		ptRT.x += pBathroom->m_width;
		ptRT.y += pBathroom->m_height;
	}
	else
	{
		ptRT.x += pBathroom->m_height;
		ptRT.y += pBathroom->m_width;
	}
	m_rect.SetLB(ptLB);
	m_rect.SetRT(ptRT);

	ShowInfo();
	m_noAirOut.SetCheck(!pBathroom->m_hasPaiQiDao);

	m_allBathrooms.clear();
	m_allBathrooms.push_back(*pBathroom);

	//////////////////////////////////////////////////////////////////////////
	//3. 显示原型
	m_preBathroom.ClearAllPreviews();
	m_preBathroom.SetRowCount(1);
	m_preBathroom.SetColumnCount(1);
	m_preBathroom.SetDisplayRows(3);
	m_preBathroom.SetDisplayColumns(1);

	CString str;
	str.Format(_T("原型编号：%s\n卫生间面积：%.2lf\n通风量要求：1.5\n动态类型：%s\n适用范围：集团"), m_allBathrooms[0].m_prototypeCode, m_rect.GetWidth() * m_rect.GetHeight() / 1E6, m_allBathrooms[0].m_isDynamic ? _T("动态") : _T("静态"));
	m_preBathroom.AddPreview(0, 0, TY_GetPrototypeFilePath() + m_allBathrooms[0].GetFileName(), str);

	m_preBathroom.SelectPreview(0, 0);

	TYUI_SetText(*GetDlgItem(IDC_BUTTON_INSERTBATHROOM), L"确定");
}

void CBathroomDlg::UpdateAttribute()
{
	if (lockUpdate)
		return;

	UpdateData(TRUE);

	if (m_pBathroomGen == NULL)
		return;

	AttrBathroom* pAtt = m_pBathroomGen->GetBathroomAtt();
	assert(pAtt);

	pAtt->m_taipenWidth = TYUI_GetComboBoxText(m_basinWidth);
	pAtt->m_matongWidth = TYUI_GetComboBoxText(m_toiletWidth);
	pAtt->m_guanXiWidth = _ttof(TYUI_GetComboBoxText(m_washWidth));

	pAtt->m_isGuoBiao = (m_isStd == 0);
	pAtt->m_floorRange = (E_BATHROOM_FLOOR_RANGE)m_floorRange.GetCurSel();
	pAtt->m_airVentOffsetX = TYUI_GetInt(m_offsetX);
	pAtt->m_airVentOffsetY = TYUI_GetInt(m_offsetY);
	pAtt->m_airVentW = TYUI_GetInt(m_customX);
	pAtt->m_airVentH = TYUI_GetInt(m_customY);

	pAtt->m_isMirror = m_isMirror.GetCheck() ? true : false;

	if (m_autoIndex.GetCheck())
	{
		pAtt->m_instanceCode = CBathroomAutoName::GetInstance()->GetBathroomName(*pAtt);
		TYUI_SetText(m_number, pAtt->m_instanceCode);
	}
}

void CBathroomDlg::LoadDefaultValue()
{
	lockUpdate = true;
	const vCString& bathroomTypes = WebIO::GetInstance()->GetConfigDict()->Bathroom_GetTypes();
	TYUI_InitComboBox(m_bathroomType, bathroomTypes, bathroomTypes.empty() ? _T("") : bathroomTypes[0]);
	m_autoIndex.SetCheck(TRUE);
	m_number.SetReadOnly(TRUE);
	m_floorRange.SetCurSel(0);
	TYUI_SetInt(m_offsetX, 0);
	TYUI_SetInt(m_offsetY, 0);
	TYUI_SetInt(m_customX, 450);
	TYUI_SetInt(m_customY, 350);
	UpdateData(FALSE);
	lockUpdate = false;
}

void CBathroomDlg::EnableSetProperty(bool bEnable)
{
	m_number.EnableWindow(bEnable);
	m_basinWidth.EnableWindow(bEnable);
	m_toiletWidth.EnableWindow(bEnable);
	m_washWidth.EnableWindow(bEnable);
	m_isMirror.EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_AUTOINDEX)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_INSERTBATHROOM)->EnableWindow(bEnable);
	EnableSetAirout(bEnable && (m_noAirOut.GetCheck() == FALSE));
}

void CBathroomDlg::EnableDynamic(bool bEnable)
{
	m_basinWidth.EnableWindow(bEnable);
	m_toiletWidth.EnableWindow(bEnable);
	m_washWidth.EnableWindow(bEnable && (m_washWidth.GetCount() > 0));
	EnableSetAirout(bEnable && (m_noAirOut.GetCheck() == FALSE));
}

void CBathroomDlg::ClearPreviews()
{
	m_allBathrooms.clear();
	m_preBathroom.ClearAllPreviews();
	EnableSetProperty(false);
}

LRESULT CBathroomDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

void CBathroomDlg::OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GRIDVIEW* pView = (NM_GRIDVIEW*)pNMHDR;
	int nSel = pView->iRow;

	if (nSel < 0 || nSel >= (int)m_allBathrooms.size())
	{
		EnableSetProperty(false);
		return;
	}

	if (m_pBathroomGen != NULL)
	{
		delete m_pBathroomGen;
	}

	m_pBathroomGen = CBathroomMrg::CreateBathroomByAttribute(&m_allBathrooms[nSel]);
	if (m_pBathroomGen == NULL)
	{
		AfxMessageBox(_T("原型无法创建BathroomGen"));
		EnableSetProperty(false);
		return;
	}

	AttrBathroom* pCurSelBathroom = m_pBathroomGen->GetBathroomAtt();
	CKitchenBathroomProp* pCurSelPrototype = &pCurSelBathroom->m_prop;

	lockUpdate = true;
	bool needMirror = false;
	//pCurSelPrototype->GetRotateAngle(m_doorDir, m_windowDir, m_angle, needMirror);
	int width = int(m_rect.GetWidth() + 0.5);
	int height = int(m_rect.GetHeight() + 0.5);
	pCurSelPrototype->GetRotateAngle(width, height, m_angle);
	pCurSelBathroom->m_angle = m_angle;
	if (pCurSelPrototype->GetWindowDoorPos() == CHUIZHIKAI)
	{
		TYUI_Disable(m_isMirror);
		pCurSelBathroom->m_isMirror = needMirror;
	}
	else
		TYUI_Enable(m_isMirror);
	m_isMirror.SetCheck(pCurSelBathroom->m_isMirror);

	//设置属性区选项
	EnableSetProperty(true);
	m_pBathroomGen->InitBathroomByDefault();
	CString sGuanxiWidth;
	sGuanxiWidth.Format(L"%.0lf", pCurSelBathroom->m_guanXiWidth);
	TYUI_InitComboBox(m_basinWidth, m_pBathroomGen->GetTaipenOptions(), pCurSelBathroom->m_taipenWidth, true);
	TYUI_InitComboBox(m_toiletWidth, m_pBathroomGen->GetMatongOptions(), pCurSelBathroom->m_matongWidth, true);
	TYUI_InitComboBox(m_washWidth, m_pBathroomGen->GetGuanxiquOptions(), sGuanxiWidth, true);
	m_isStd = pCurSelBathroom->m_isGuoBiao ? 0 : 1;
	m_floorRange.SetCurSel((int)pCurSelBathroom->m_floorRange);
	TYUI_SetInt(m_offsetX, (int)pCurSelBathroom->m_airVentOffsetX);
	TYUI_SetInt(m_offsetY, (int)pCurSelBathroom->m_airVentOffsetY);
	TYUI_SetInt(m_customX, (int)pCurSelBathroom->m_airVentW);
	TYUI_SetInt(m_customY, (int)pCurSelBathroom->m_airVentH);

	m_autoIndex.SetCheck(TRUE);
	TYUI_SetText(m_number, pCurSelBathroom->m_instanceCode);

	UpdateData(FALSE);
	lockUpdate = false;
}

void CBathroomDlg::OnBnClickedButtonInsert()
{
	UpdateData(TRUE);
	if (m_pBathroomGen == NULL)
	{
		AfxMessageBox(_T("请选择原型"));
		return;
	}

	CString newInstanceCode = TYUI_GetText(m_number);
	if (!CBathroomAutoName::GetInstance()->IsNameValid(*m_pBathroomGen->GetBathroomAtt(), newInstanceCode))
	{
		AfxMessageBox(L"此编号已被占用");
		return;
	}

	UpdateAttribute();
	//检查参数合法性
	CString errMsg;
	if (!m_pBathroomGen->CheckParameter(errMsg))
	{
		AfxMessageBox(errMsg);
		return;
	}

	if (!m_autoIndex.GetCheck())
	{
		m_pBathroomGen->GetBathroomAtt()->SetInstanceCode(newInstanceCode);
		CBathroomAutoName::GetInstance()->RenameBathroom(*m_pBathroomGen->GetBathroomAtt());
	}

	AcGePoint3d origin = m_rect.GetLB();

	//生成
	if (m_pCurEdit != NULL)
	{
		JHCOM_DeleteCadObject(m_pCurEdit->objectId());
	}
	m_pBathroomGen->GenBathroom(origin, m_angle);
	CBathroomAutoName::GetInstance()->AddBathroomType(*m_pBathroomGen->GetBathroomAtt());

	OnOK();
	//ShowWindow(TRUE);
}

void CBathroomDlg::OnBnClickedButtonRange()
{
	ShowWindow(false);
	TYRect rect;
	bool bSuc = TY_GetOneRect(rect);
	if (bSuc==false)
	{
		ShowWindow(SW_SHOW);
		return;
	}

	if (IsBathroomRectValid(rect)==false)
	{
		AfxMessageBox(_T("所选卫生间范围无效\n"));
		ShowWindow(true);
		return;
	}

	m_rect = rect;

	/*if (m_doorDir == E_DIR_UNKNOWN)
	{
		ads_point pt;
		acedInitGet(32,NULL);
		if(acedGetPoint(NULL,L"\n选择门的位置\n",pt)!=RTNORM) //第一角点选择
		{
			ShowWindow(true);
			return;
		}
		m_doorDir = GetDir(pt);
	}

	if (m_windowDir == E_DIR_UNKNOWN)
	{
		do
		{
			ads_point pt;
			acedInitGet(32,NULL);
			if(acedGetPoint(NULL,L"\n选择窗的位置\n",pt)!=RTNORM) //第一角点选择
			{
				ShowWindow(true);
				return;
			}
			m_windowDir = GetDir(pt);
			if (m_doorDir == m_windowDir)
				AfxMessageBox(_T("门窗方向不能相同\n"));
		}
		while (m_doorDir == m_windowDir);

		ShowInfo();
	}*/
	ShowWindow(true);
	ShowInfo();
	ClearPreviews();
}

void CBathroomDlg::ShowInfo()
{
	CString sInfo;
	if (IsBathroomRectValid(m_rect))
		sInfo.Format(_T("卫生间信息：%.0lf x %.0lf"), m_rect.GetWidth(), m_rect.GetHeight());
	else
		sInfo = L"卫生间信息：未知尺寸";

	if ((m_windowDir != E_DIR_UNKNOWN) && (m_doorDir != E_DIR_UNKNOWN))
	{
		if ((abs(m_windowDir - m_doorDir) % 2) == 0)
			sInfo += L"，门窗对开";
		else
			sInfo += L"，门窗垂直开";
	}
	
	GetDlgItem(IDC_STATIC_DIR)->SetWindowText(sInfo);
}

void CBathroomDlg::OnBnClickedButtonDoorDir()
{
	if (IsBathroomRectValid(m_rect) == false)
	{
		AfxMessageBox(_T("请先选择卫生间范围\n"));
		return;
	}

	ShowWindow(false);
	ads_point pt;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"\n选择门的位置\n",pt)!=RTNORM) //第一角点选择
	{
		ShowWindow(true);
		return;
	}
	ShowWindow(true);

	E_DIRECTION temp = GetDir(pt);
	if (m_doorDir == temp) //未修改，直接跳过
		return;
	if (m_windowDir == temp)
	{
		acutPrintf(_T("门窗方向不能相同\n"));
		return;
	}
	m_doorDir = temp;
	if (m_windowDir == E_DIR_UNKNOWN)
		return;
	//更新方向后清空原有搜索列表
	m_preBathroom.ClearAllPreviews();
	ShowInfo();
}

void CBathroomDlg::OnBnClickedButtonWindowDir()
{
	if (IsBathroomRectValid(m_rect) == false)
	{
		AfxMessageBox(_T("请先选择卫生间范围\n"));
		return;
	}

	ShowWindow(false);
	ads_point pt;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"\n选择窗的位置\n",pt)!=RTNORM) //第一角点选择
	{
		ShowWindow(true);
		return;
	}
	ShowWindow(true);

	E_DIRECTION temp = GetDir(pt);
	if (m_windowDir == temp) //未修改，直接跳过
		return;
	if (m_doorDir == temp)
	{
		acutPrintf(_T("门窗方向不能相同\n"));
		return;
	}
	m_windowDir = temp;
	if (m_doorDir == E_DIR_UNKNOWN)
		return;
	//更新方向后清空原有搜索列表
	m_preBathroom.ClearAllPreviews();
	ShowInfo();
}

void CBathroomDlg::OnBnClickedButtonSearch()
{
	//1.检查数据
	if (IsBathroomRectValid(m_rect) == false)
	{
		AfxMessageBox(_T("请先选择卫生间范围\n"));
		return;
	}
	/*if (m_doorDir == E_DIR_UNKNOWN || m_windowDir == E_DIR_UNKNOWN)
	{
		AfxMessageBox(_T("请先选择门窗方向\n"));
		return;
	}
	if (m_windowDir == m_doorDir)
	{
		AfxMessageBox(_T("门窗方向不能相同\n"));
		return;
	}*/

	//////////////////////////////////////////////////////////////////////////
	//2. 搜索原型
	EBathroomType bathroomType = E_BATHROOM_ALL;
	CString type = TYUI_GetText(m_bathroomType);
	if (type == _T("U型"))
		bathroomType = E_BATHROOM_U;
	else if (type == _T("L型"))
		bathroomType = E_BATHROOM_L;
	else if (type == _T("I型"))
		bathroomType = E_BATHROOM_I;

	double width = m_rect.GetWidth();
	double height = m_rect.GetHeight();

	m_allBathrooms = WebIO::GetInstance()->GetBathrooms(bathroomType, width, height, m_doorDir, m_windowDir);
	
	//////////////////////////////////////////////////////////////////////////
	//3. 显示原型
	m_preBathroom.ClearAllPreviews();
	if (m_allBathrooms.empty())
	{
		AfxMessageBox(_T("未找到符合条件的记录,请确保面宽进深为150递增\n"));
		return;
	}
	m_preBathroom.SetRowCount((int)m_allBathrooms.size());
	m_preBathroom.SetColumnCount(1);
	m_preBathroom.SetDisplayRows(3);
	m_preBathroom.SetDisplayColumns(1);
	for (UINT i = 0; i < m_allBathrooms.size(); i++)
	{
		CString str;
		str.Format(_T("原型编号：%s\n厨房面积：%.2lf\n通风量要求：1.5\n动态类型：%s\n适用范围：集团"), m_allBathrooms[i].GetPrototypeCode(), m_rect.GetWidth() * m_rect.GetHeight() / 1E6, m_allBathrooms[i].m_isDynamic ? _T("动态") : _T("静态"));
		CString dwgPath = TY_GetPrototypeFilePath() + m_allBathrooms[i].GetFileName();
		CString pngPath = TY_GetPrototypeImagePath_Local() + m_allBathrooms[i].GetFileName();
		pngPath.Replace(L".dwg", L".png");
		if (PathFileExists(pngPath))
			m_preBathroom.AddPreview(i, 0, pngPath, str);
		else
			m_preBathroom.AddPreview(i, 0, dwgPath, str);
	}

	m_preBathroom.SelectPreview(0, 0);
}

void CBathroomDlg::OnBnClickedAutoIndex()
{
	m_number.SetReadOnly(m_autoIndex.GetCheck());
}
