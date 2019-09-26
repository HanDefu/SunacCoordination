// UI\BathroomDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BathroomDlg.h"
#include "afxdialogex.h"
#include "../WebIO/WebIO.h"

// CBathroomDlg 对话框

IMPLEMENT_DYNAMIC(CBathroomDlg, CAcUiDialog)

CBathroomDlg::CBathroomDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CBathroomDlg::IDD, pParent)
{
	m_rect.SetLB(AcGePoint3d(0, 0, 0));
	m_rect.SetRT(AcGePoint3d(0, 0, 0));
	m_doorDir = E_DIR_UNKNOWN;
	m_windowDir = E_DIR_UNKNOWN;
}

CBathroomDlg::~CBathroomDlg()
{
	for (UINT i = 0; i < m_allBathrooms.size(); i++)
	{
		delete m_allBathrooms[i];
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
	DDX_Control(pDX, IDC_COMBO_TOILETAREA, m_toiletWidth);
	DDX_Control(pDX, IDC_COMBO_WASHWIDTH, m_washWidth);
	DDX_Control(pDX, IDC_CHECK_IMAGE, m_isMirror);
	DDX_Control(pDX, IDC_CHECK_AUTOINDEX, m_autoIndex);
}


BEGIN_MESSAGE_MAP(CBathroomDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_RANGE, &CBathroomDlg::OnBnClickedButtonRange)
	ON_BN_CLICKED(IDC_BUTTON_DOORDIR, &CBathroomDlg::OnBnClickedButtonDoorDir)
	ON_BN_CLICKED(IDC_BUTTON_WINDOWDIR, &CBathroomDlg::OnBnClickedButtonWindowDir)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CBathroomDlg::OnBnClickedButtonSearch)
	ON_NOTIFY(GVN_SELCHANGED, IDC_PREVIEW_BATHROOM, &CBathroomDlg::OnSelChanged)
END_MESSAGE_MAP()


// CBathroomDlg 消息处理程序


BOOL CBathroomDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_preBathroom.LoadDefaltSettings();
	LoadDefaultValue();

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

void CBathroomDlg::LoadDefaultValue()
{
	const vCString& bathroomTypes = WebIO::GetConfigDict()->Toilet_GetTypes();
	TYUI_InitComboBox(m_bathroomType, bathroomTypes, bathroomTypes.empty() ? _T("") : bathroomTypes[0]);
	m_autoIndex.SetCheck(TRUE);
	m_number.SetReadOnly(TRUE);
	UpdateData(FALSE);
}

vector<AttrToilet*> CBathroomDlg::FilterTI()
{
	vector<AttrToilet*> attrToilet;
	int width = int(m_rect.GetWidth() + 0.5);
	int height = int(m_rect.GetHeight() + 0.5);

	if (width != 1600 && width != 1700 && width != 1850)
		return attrToilet;
	if (height < 2450 || height > 3650)
		return attrToilet;
	if ((height - 2450) % 150 != 0)
		return attrToilet;
	if ((abs(m_windowDir - m_doorDir) % 2) == 1) //只支持对开
		return attrToilet;

	return WebIO::GetInstance()->GetToilets(width, height, _T("对开"), _T("I"), (m_noAirOut.GetCheck() == 0));
}

vector<AttrToilet*> CBathroomDlg::FilterTL()
{
	vector<AttrToilet*> attrToilet;
	int width = int(m_rect.GetWidth() + 0.5);
	int height = int(m_rect.GetHeight() + 0.5);

	if (width < 1700 || width > 2150)
		return attrToilet;
	if (height < 1850 || height > 2750)
		return attrToilet;
	if ((width - 1700) % 150 != 0)
		return attrToilet;
	if ((height - 1850) % 150 != 0)
		return attrToilet;
	//不支持以下尺寸
	if (width == 1700 && height == 1850)
		return attrToilet;
	if (width == 1700 && height == 2000)
		return attrToilet;
	if (width == 2000 && height == 2600)
		return attrToilet;
	if (width == 2000 && height == 2750)
		return attrToilet;
	if (width == 2150 && height == 2600)
		return attrToilet;
	if (width == 2150 && height == 2750)
		return attrToilet;

	//暂不考虑开启方向
	/*if ((height >= 2450) && abs(m_doorDir - m_windowDir) == 2) //四件套垂直开
		return attrToilet;
	if ((height < 2450) && abs(m_doorDir - m_windowDir) != 2) //四件套垂直开
		return attrToilet;*/

	return WebIO::GetInstance()->GetToilets(width, height, _T("对开"), _T("L"), (m_noAirOut.GetCheck() == 0));
}

vector<AttrToilet*> CBathroomDlg::FilterTU()
{
	vector<AttrToilet*> attrToilet;
	int width = int(m_rect.GetWidth() + 0.5);
	int height = int(m_rect.GetHeight() + 0.5);

	//U型尺寸固定且不重复，暂时只考虑尺寸
	if (width == 1850 && height == 2000)
		return WebIO::GetInstance()->GetToilets(width, height, _T("对开"), _T("U"), (m_noAirOut.GetCheck() == 0));
	if (width == 1600 && height == 2450)
		return WebIO::GetInstance()->GetToilets(width, height, _T("对开"), _T("U"), (m_noAirOut.GetCheck() == 0));
	if (width == 1850 && height == 2750)
		return WebIO::GetInstance()->GetToilets(width, height, _T("对开"), _T("U"), (m_noAirOut.GetCheck() == 0));
	if (width == 2000 && height == 2750)
		return WebIO::GetInstance()->GetToilets(width, height, _T("对开"), _T("U"), (m_noAirOut.GetCheck() == 0));

	return attrToilet;
}

LRESULT CBathroomDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

void CBathroomDlg::OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GRIDVIEW* pView = (NM_GRIDVIEW*)pNMHDR;
	int nSel = pView->iRow;

	int width = int(m_rect.GetWidth() + 0.5);
	int height = int(m_rect.GetHeight() + 0.5);

	if (nSel >= 0 && nSel < m_allBathrooms.size())
	{
		CString type = m_allBathrooms[nSel]->m_toiletType;
		bool isG = (m_allBathrooms[nSel]->m_name.Right(6) == _T("_g.dwg"));

		TYUI_InitComboBox(m_basinWidth, _T("650;750;800;900;1000"), _T("650"));
		TYUI_InitComboBox(m_toiletWidth, _T("750;800"), _T("750"));
		if (isG)
		{
			TYUI_Enable(m_washWidth);
			TYUI_InitComboBox(m_washWidth, _T("900;1050"), _T("900"));
		}
		else
		{
			TYUI_Disable(m_washWidth);
		}
	}
}

void CBathroomDlg::OnBnClickedButtonRange()
{
	ShowWindow(false);
	TYRect rect = TY_GetOneRect();
	ShowWindow(true);

	if (m_rect.IsSame(rect, 1E-4))
		return;
	if (rect.GetWidth() < 1E-4 || rect.GetHeight() < 1E-4)
	{
		acutPrintf(_T("所选卫生间范围无效\n"));
		return;
	}
	m_rect = rect;

	//更新范围后清空原有搜索列表
	m_preBathroom.ClearAllPreviews();
}

void CBathroomDlg::OnBnClickedButtonDoorDir()
{
	if (m_rect.GetWidth() < 1E-4 || m_rect.GetHeight() < 1E-4)
	{
		acutPrintf(_T("请先选择卫生间范围\n"));
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
	if ((abs(m_windowDir - m_doorDir) % 2) == 0)
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("门窗位置关系：门窗对开"));
	else
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("门窗位置关系：门窗垂直开"));
}

void CBathroomDlg::OnBnClickedButtonWindowDir()
{
	if (m_rect.GetWidth() < 1E-4 || m_rect.GetHeight() < 1E-4)
	{
		acutPrintf(_T("请先选择卫生间范围\n"));
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
	if ((abs(m_windowDir - m_doorDir) % 2) == 0)
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("门窗位置关系：门窗对开"));
	else
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("门窗位置关系：门窗垂直开"));
}

void CBathroomDlg::OnBnClickedButtonSearch()
{
	if (m_rect.GetWidth() < 1E-4 || m_rect.GetHeight() < 1E-4)
	{
		acutPrintf(_T("请先选择卫生间范围\n"));
		return;
	}
	if (m_doorDir == E_DIR_UNKNOWN || m_windowDir == E_DIR_UNKNOWN)
	{
		acutPrintf(_T("请先选择门窗方向\n"));
		return;
	}
	CString type = TYUI_GetText(m_bathroomType);
	if (type == _T("U型"))
		m_allBathrooms = FilterTU();
	else if (type == _T("L型"))
		m_allBathrooms = FilterTL();
	else if (type == _T("I型"))
		m_allBathrooms = FilterTI();

	m_preBathroom.ClearAllPreviews();
	if (m_allBathrooms.empty())
	{
		acutPrintf(_T("未找到符合条件的记录\n"));
		return;
	}
	m_preBathroom.SetRowCount((int)m_allBathrooms.size());
	m_preBathroom.SetColumnCount(1);
	m_preBathroom.SetDisplayRows(3);
	m_preBathroom.SetDisplayColumns(1);
	for (int i = 0; i < m_allBathrooms.size(); i++)
	{
		CString str;
		str.Format(_T("原型编号：%s\n卫生间面积：%.2lf\n通风量要求：0.8\n动态类型：动态\n适用范围：集团"), m_allBathrooms[i]->m_yxid, m_rect.GetWidth() * m_rect.GetHeight() / 1E6);
		m_preBathroom.AddPreview(i, 0, TY_GetLocalFilePath() + m_allBathrooms[i]->m_name, str);
	}

	m_preBathroom.SelectPreview(0, 0);
}
