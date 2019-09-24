// UI\KitchenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KitchenDlg.h"
#include "afxdialogex.h"
// CKitchenDlg 对话框

IMPLEMENT_DYNAMIC(CKitchenDlg, CAcUiDialog)

CKitchenDlg::CKitchenDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CKitchenDlg::IDD, pParent)
{
	m_rect.SetLB(AcGePoint3d(0, 0, 0));
	m_rect.SetRT(AcGePoint3d(0, 0, 0));
	doorDir = DIR_UNKNOWN;
	windowDir = DIR_UNKNOWN;
}

CKitchenDlg::~CKitchenDlg()
{
}

LRESULT CKitchenDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	//return FALSE;
	return TRUE;
}

void CKitchenDlg::OnOK()
{
	CAcUiDialog::OnOK();
	DestroyWindow();
}

void CKitchenDlg::OnCancel()
{
	CAcUiDialog::OnCancel();
	DestroyWindow();
}

void CKitchenDlg::PostNcDestroy()
{
	CAcUiDialog::PostNcDestroy();
	delete this;
}

void CKitchenDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_KITCHEN, m_preKitchen);
	DDX_Control(pDX, IDC_COMBO_FLOORRANGE, m_floorRange);
	DDX_Control(pDX, IDC_COMBO_BASINTYPE, m_basinType);
	DDX_Control(pDX, IDC_COMBO_BENCHWIDTH, m_benchWidth);
	DDX_Control(pDX, IDC_COMBO_FRIDGETYPE, m_frigeType);
}

BEGIN_MESSAGE_MAP(CKitchenDlg, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CKitchenDlg::OnBnClickedOk)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_INSERTKITCHEN, &CKitchenDlg::OnBnClickedMfcbuttonInsert)
	ON_BN_CLICKED(IDC_BUTTON_RANGE, &CKitchenDlg::OnBnClickedMfcbuttonRange)
	ON_BN_CLICKED(IDC_BUTTON_DOORDIR, &CKitchenDlg::OnBnClickedMfcbuttonDoorDir)
	ON_BN_CLICKED(IDC_BUTTON_WINDOWDIR, &CKitchenDlg::OnBnClickedMfcbuttonWindowDir)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CKitchenDlg::OnBnClickedButtonSearch)
END_MESSAGE_MAP()


// CKitchenDlg 消息处理程序


BOOL CKitchenDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_preKitchen.LoadDefaltSettings();

	m_preKitchen.SetRowCount(8);
	m_preKitchen.SetColumnCount(2);

	//设置预览图显示区域为3行1列
	m_preKitchen.SetDisplayRows(3);
	m_preKitchen.SetDisplayColumns(1);

	//设置第一列占60%，其余部分为第二列
	m_preKitchen.SetHeaderWidth(_T("60;+"));

	
	m_floorRange.SetCurSel(0);
	m_basinType.SetCurSel(0);
	m_benchWidth.SetCurSel(0);
	m_frigeType.SetCurSel(0);
	m_projectLevel.SetCurSel(0);

	return TRUE;
}


void CKitchenDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CAcUiDialog::OnOK();
}

void CKitchenDlg::OnBnClickedMfcbuttonInsert()
{
	ShowWindow(FALSE);

	// TODO: 在此添加控件通知处理程序代码
	vCString allFiles;
	TY_GetAllKitchenFiles(allFiles);

	double width = m_rect.GetWidth(), height = m_rect.GetHeight();
	AcGePoint3d origin = m_rect.GetLB();

	vector<int> sels = m_preKitchen.GetSelectedRows();
	if (sels.size() > 0)
	{
		RCKitchen oneKitchen;
		if (sels[0] == 0)
			oneKitchen.Insert(allFiles[0], origin, 0, L"0", 256);
		else
			oneKitchen.Insert(allFiles[1], origin, 0, L"0", 256);
		oneKitchen.InitParameters();
		oneKitchen.SetParameter(L"进深", height);
		oneKitchen.SetParameter(L"开间", width);
		oneKitchen.RunParameters();

		//把UI的数据记录在图框的扩展字典中
		AttrKitchen *pAttribute = new AttrKitchen();
		if (sels[0] == 0)
			pAttribute->m_yxid = L"KUA";
		else if (sels[0] == 1)
			pAttribute->m_yxid = L"KUB";
		else
			pAttribute->m_yxid = L"KUC";

		oneKitchen.AddAttribute(pAttribute);
		pAttribute->close();
	}
	ShowWindow(true);
	OnOK();
}


void CKitchenDlg::OnBnClickedMfcbuttonRange()
{
	ShowWindow(false);
	m_rect = TY_GetOneRect();
	if (m_rect.GetWidth() < 1E-4 || m_rect.GetHeight() < 1E-4)
	{
		acutPrintf(_T("所选厨房范围无效\n"));
		ShowWindow(true);
		return;
	}
	ShowWindow(true);
}


void CKitchenDlg::OnBnClickedMfcbuttonDoorDir()//门方向
{
	if (m_rect.GetWidth() < 1E-4 || m_rect.GetHeight() < 1E-4)
	{
		acutPrintf(_T("请先选择厨房范围\n"));
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
	doorDir = GetDir(pt);
	if (windowDir == DIR_UNKNOWN)
		return;
	if (windowDir == doorDir)
	{
		doorDir = DIR_UNKNOWN;
		acutPrintf(_T("门窗方向不能相同\n"));
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("门窗位置关系："));
		return;
	}
	if (abs(windowDir - doorDir) == 2)
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("门窗位置关系：门窗对开"));
	else
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("门窗位置关系：门窗垂直开"));
}

void CKitchenDlg::OnBnClickedMfcbuttonWindowDir()//窗方向
{
	if (m_rect.GetWidth() < 1E-4 || m_rect.GetHeight() < 1E-4)
	{
		acutPrintf(_T("请先选择厨房范围\n"));
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
	windowDir = GetDir(pt);
	if (doorDir == DIR_UNKNOWN)
		return;
	if (windowDir == doorDir)
	{
		windowDir = DIR_UNKNOWN;
		acutPrintf(_T("门窗方向不能相同\n"));
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("门窗位置关系："));
		return;
	}
	if (abs(windowDir - doorDir) == 2)
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("门窗位置关系：门窗对开"));
	else
		GetDlgItem(IDC_STATIC_DIR)->SetWindowText(_T("门窗位置关系：门窗垂直开"));
}


void CKitchenDlg::OnBnClickedButtonSearch()
{
	vCString allFiles;
	TY_GetAllKitchenFiles(allFiles);
	for (int i = 0; i < allFiles.size(); i++)
		m_preKitchen.AddPreview(i, 0, allFiles[i], _T("厨房类型：KU\n厨房面积：2.1\n通风量：1.6"));

	//m_preKitchen.ShowPreviews();

}

DIR CKitchenDlg::GetDir(ads_point pt)
{
	double minDis = abs(pt[X] - m_rect.GetLB().x);
	DIR dir = DIR_LEFT;
	if (abs(pt[X] - m_rect.GetRT().x) < minDis)
	{
		minDis = abs(pt[X] - m_rect.GetRT().x);
		dir = DIR_RIGHT;
	}
	if (abs(pt[Y] - m_rect.GetLB().y) < minDis)
	{
		minDis = abs(pt[Y] - m_rect.GetLB().y);
		dir = DIR_BOTTOM;
	}
	if (abs(pt[Y] - m_rect.GetRT().y) < minDis)
	{
		minDis = abs(pt[Y] - m_rect.GetRT().y);
		dir = DIR_TOP;
	}
	return dir;
}

vector<AttrKitchen*> CKitchenDlg::FilterKUq()
{
	vector<AttrKitchen*> attrKitchen;
	int width = int(m_rect.GetWidth() + 0.5);
	int height = int(m_rect.GetHeight() + 0.5);

	//限定开间与进深的范围
	if (width < 2450 || width > 3500)
		return attrKitchen;
	if (height < 1700 || height > 2600)
		return attrKitchen;
	if (width < height)
		return attrKitchen;

	//不支持2450*1700
	if (width == 2450 && height == 1700)
		return attrKitchen;

	//以150递增尺寸
	if ((width - 2450) % 150 != 0)
		return attrKitchen;
	if ((height - 1700) % 150 != 0)
		return attrKitchen;

	//进深>=2000时包含转角橱柜
	//if (height >= 2000)

}
