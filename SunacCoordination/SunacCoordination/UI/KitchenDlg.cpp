// UI\KitchenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KitchenDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/RCKitchen.h"
#include "../Object/AttrKitchen.h"
// CKitchenDlg 对话框

IMPLEMENT_DYNAMIC(CKitchenDlg, CAcUiDialog)

CKitchenDlg::CKitchenDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CKitchenDlg::IDD, pParent)
{

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

TYRect rect;

void CKitchenDlg::OnBnClickedMfcbuttonInsert()
{
	ShowWindow(FALSE);

	// TODO: 在此添加控件通知处理程序代码
	vCString allFiles;
	TY_GetAllKitchenFiles(allFiles);

	double width = rect.GetWidth(), height = rect.GetHeight();
	AcGePoint3d origin = rect.GetLB();

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
	rect = TY_GetOneRect();
	ShowWindow(true);
}


void CKitchenDlg::OnBnClickedMfcbuttonDoorDir()//门方向
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(false);
	ads_point pt;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"\n选择门的位置\n",pt)!=RTNORM) //第一角点选择
	{
		return;
	}
	ShowWindow(true);
}

void CKitchenDlg::OnBnClickedMfcbuttonWindowDir()//窗方向
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(false);
	ads_point pt;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"\n选择窗的位置\n",pt)!=RTNORM) //第一角点选择
	{
		return;
	}
	ShowWindow(true);
}


void CKitchenDlg::OnBnClickedButtonSearch()
{
	vCString allFiles;
	TY_GetAllKitchenFiles(allFiles);
	for (int i = 0; i < allFiles.size(); i++)
		m_preKitchen.AddPreview(i, 0, allFiles[i], _T("厨房类型：KU\n厨房面积：2.1\n通风量：1.6"));

	//m_preKitchen.ShowPreviews();

}
