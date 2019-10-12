// UI\RailingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RailingDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
//#include "RailingBaseDlg.h"
#include "../Object/Railing/RCRailing.h"
#include "../Object/Railing/RCRailingTieyi.h"
#include "../Object/Railing/RCRailingBoli.h"
#include "../WebIO/WebIO.h"
#include "../GlobalSetting.h"
#include "GridCellWithPicture.h"

// CRailingDlg 对话框

CRailingDlg* g_railingDlg = NULL;

BOOL CloseRailingDlg()
{
	if (g_railingDlg == NULL)
		return TRUE;
	BOOL ret = g_railingDlg->DestroyWindow();
	if (ret)
		g_railingDlg = NULL;
	return ret;
}


IMPLEMENT_DYNAMIC(CRailingDlg, CAcUiDialog)

CRailingDlg::CRailingDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CRailingDlg::IDD, pParent)
	, m_height(1200)
	, m_heightBase(200)
	, m_width(5400)
{
	m_isMoldless = true;
}

CRailingDlg::~CRailingDlg()
{
}

INT_PTR CRailingDlg::DoModal()
{
	m_isMoldless = false;
	return CAcUiDialog::DoModal();
}

void CRailingDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_RAILING, m_preRailing);
	DDX_Control(pDX, IDC_COMBO_RAILINGTYPE, m_type);
	DDX_Text(pDX, IDC_EDIT_RAILINGHEIGHT, m_height);
	DDV_MinMaxDouble(pDX, m_height, 100, 10000);
	DDX_Text(pDX, IDC_EDIT_REVERSERIDGE_HEIGHT, m_heightBase);
	DDV_MinMaxDouble(pDX, m_heightBase, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_HOLEWIDTH, m_width);
	DDV_MinMaxDouble(pDX, m_width, 100, 100000);
}

BEGIN_MESSAGE_MAP(CRailingDlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON_INSERTRAILING, &CRailingDlg::OnBnClickedInsertToCAD)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_SELECTLINE, &CRailingDlg::OnBnClickedButtonSelectline)
	ON_CBN_SELCHANGE(IDC_COMBO_RAILINGTYPE, &CRailingDlg::OnCbnSelchangeComboRailingtype)
END_MESSAGE_MAP()


// CRailingDlg 消息处理程序


BOOL CRailingDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	vCString allRailingFiles;
	TY_GetAllTieYiLanGanFiles(allRailingFiles);
	m_selectedFile = allRailingFiles[0];

	//m_preStyle.SubclassDlgItem(IDC_STATIC_STYLE, this);
	//m_preStyle.Init(theArxDLL.ModuleResourceInstance(), true);
	//m_preStyle.SetDwgFile(m_selectedFile);

	//m_railingInfo.SetWindowText(_T("栏杆信息说明:\r\n栏杆间距:\r\n单元尺寸:\r\n栏杆类型："));
	m_type.AddString(_T("不限"));
	m_type.AddString(_T("铁艺栏杆"));
	m_type.AddString(_T("玻璃栏杆"));
	m_type.SetCurSel(0);

	UpdateAll();

	//((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_LIB))->SetImage(IDB_BITMAP37);
	//((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_CANCEL))->SetImage(IDB_BITMAP37);
	return TRUE;
}

/*
void CRailingDlg::UpdateSelectFile(CString selectFile)
{
	if (selectFile.GetLength() > 0)
	{
		m_preStyle.SetDwgFile(selectFile);
		m_selectedFile = selectFile;
	}
}
*/

void Test(AttrRailing& railingAtt)
{
	static int n = 0; 
	
	railingAtt.m_railingType = E_RAILING_BOLI;
	if (n==0)
		railingAtt.m_prototypeCode = _T("Railing_B1");
	else if (n == 1)
		railingAtt.m_prototypeCode = _T("Railing_B2");
	else if (n == 2)
		railingAtt.m_prototypeCode = _T("Railing_B3_1");
	else if (n == 3)
		railingAtt.m_prototypeCode = _T("Railing_B3_2");
	else if (n == 4)
		railingAtt.m_prototypeCode = _T("Railing_B4");
	else if (n == 5)
		railingAtt.m_prototypeCode = _T("Railing_B5");
	else if (n == 6)
		railingAtt.m_prototypeCode = _T("Railing_B6");

	n = (n + 1) % 7;
}

void CRailingDlg::OnBnClickedInsertToCAD()
{	
	UpdateData();

	vector<CCellID> selCells = m_preRailing.GetSelectedCells();
	if (selCells.empty())
		return;
	m_selectedRow = selCells[0].row;
	m_selectedColoum = selCells[0].col;

	CGridCellForPreview* pCell = m_preRailing.GetPreviewCell(m_selectedRow, m_selectedColoum);
	if (pCell==NULL)
	{
		acutPrintf(_T("没有选择原型，请重新选择或者双击原型\n"));
		return;
	}

	CString sPrototypeName = pCell->GetName();

	//检查数据
	if (m_width<1500)
	{
		AfxMessageBox(_T("栏杆长度太短"));
		return;
	}

	ShowWindow(FALSE);

	CString path;
	AttrRailing railingAtt;
	railingAtt.m_height = m_height;
	railingAtt.m_length = m_width;
	railingAtt.m_prototypeCode = sPrototypeName;
	railingAtt.m_railingType = sPrototypeName.Find(_T("_T"))>0 ? E_RAILING_TIEYI : E_RAILING_BOLI;

	//生成
	
	//Test(railingAtt);

	CRCRailing* pRailing = CreateRailing(railingAtt);

	//选择插入点
	ShowWindow(FALSE);
	AcGePoint3d pnt;
	pnt = TY_GetPoint();

	AcDbObjectId railingId;
	pRailing->GenerateRailing(pnt, railingId);

	delete pRailing;

	//ShowWindow(TRUE);
	OnOK();
}



LRESULT CRailingDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}
void CRailingDlg::OnOK()
{
	CAcUiDialog::OnOK();
	if (m_isMoldless)
		DestroyWindow();
}

void CRailingDlg::OnCancel()
{
	CAcUiDialog::OnCancel();
	if (m_isMoldless)
		DestroyWindow();
}

void CRailingDlg::PostNcDestroy()
{
	CAcUiDialog::PostNcDestroy();
	if (m_isMoldless)
	{
		delete this;
		g_railingDlg = NULL;
	}
}

void CRailingDlg::OnBnClickedButtonSelectline()
{
	ShowWindow(FALSE);
	AcGePoint3d pnt1, pnt2;
	TY_GetTwoPoints(pnt1, pnt2);

	//TODO 选线

	ShowWindow(TRUE);

	double len = pnt2.x - pnt1.x;
	m_width = len;
	UpdateData(FALSE);
}


void CRailingDlg::OnCbnSelchangeComboRailingtype()
{
	UpdateData(TRUE);

	//获取控件ComboBox中选的值
	CString type = TYUI_GetComboBoxText(m_type);

	if (type == _T("不限"))
	{
		UpdateAll();
	}

	if (type == _T("铁艺栏杆"))
	{
		UpdateTY();
	}

	if (type == _T("玻璃栏杆"))
	{
		UpdateBL();
	}

	UpdateData(FALSE);
}

void CRailingDlg::UpdateTY()
{
	AttrRailing railingAtt;
	railingAtt.m_height = m_height;
	railingAtt.m_length = m_width;

	m_preRailing.ClearAllPreviews();
	m_preRailing.SetRowCount(4);
	m_preRailing.SetColumnCount(2);
	m_preRailing.SetDisplayRows(2);
	m_preRailing.SetDisplayColumns(2);

	for (UINT i = 0; i < 7; i++)
	{
		CString str;
		CString path;
		sPrototypeName.Format(_T("原型编号：Railing_T%d"),i+1);
		str.Format(_T("原型编号：%s\n栏杆类型：铁艺栏杆\n栏杆高度：%d\n栏杆宽度：%d\n"),sPrototypeName, railingAtt.m_height, railingAtt.m_length);
		path.Format(TY_GetLocalImagePath() + ("Railing_T%d.png"),i+1);
		CGridCellForPreview* pCell = m_preRailing.AddPreviewPng(i/2, i%2, path, str);
		pCell->SetName(sPrototypeName);
	}

	m_preRailing.SelectPreview(0, 0);
}

void CRailingDlg::UpdateBL()
{
	AttrRailing railingAtt;
	railingAtt.m_height = m_height;
	railingAtt.m_length = m_width;

	m_preRailing.ClearAllPreviews();
	m_preRailing.SetRowCount(4);
	m_preRailing.SetColumnCount(2);
	m_preRailing.SetDisplayRows(2);
	m_preRailing.SetDisplayColumns(2);

	for (UINT i = 0; i < 7; i++)
	{
		CString str;
		CString path;
		if (i <= 1)
		{
			sPrototypeName.Format(_T("原型编号：Railing_B%d"),i+1);
			str.Format(_T("原型编号：%s\n栏杆类型：玻璃栏杆\n栏杆高度：%d\n栏杆宽度：%d\n"),sPrototypeName, railingAtt.m_height, railingAtt.m_length);
			path.Format(TY_GetLocalImagePath() + ("Railing_B%d.png"),i+1);
			CGridCellForPreview* pCell = m_preRailing.AddPreviewPng(i/2, i%2, path, str);
			pCell->SetName(sPrototypeName);
		}
		else if (i > 1 && i < 4)
		{
			sPrototypeName.Format(_T("原型编号：Railing_B3_%d"),i-1);
			str.Format(_T("原型编号：%s\n栏杆类型：玻璃栏杆\n栏杆高度：%d\n栏杆宽度：%d\n"),sPrototypeName, railingAtt.m_height, railingAtt.m_length);
			path.Format(TY_GetLocalImagePath() + ("Railing_B3_%d.png"),i-1);
			CGridCellForPreview* pCell = m_preRailing.AddPreviewPng(i/2, i%2, path, str);	
			pCell->SetName(sPrototypeName);
		}
		else
		{
			sPrototypeName.Format(_T("原型编号：Railing_B%d"),i);
			str.Format(_T("原型编号：%s\n栏杆类型：玻璃栏杆\n栏杆高度：%d\n栏杆宽度：%d\n"),sPrototypeName, railingAtt.m_height, railingAtt.m_length);
			path.Format(TY_GetLocalImagePath() + ("Railing_B%d.png"),i);
			CGridCellForPreview* pCell = m_preRailing.AddPreviewPng(i/2, i%2, path, str);
			pCell->SetName(sPrototypeName);
		}
	}

	m_preRailing.SelectPreview(0, 0);
}

void CRailingDlg::UpdateAll()
{
	AttrRailing railingAtt;
	railingAtt.m_height = m_height;
	railingAtt.m_length = m_width;

	m_preRailing.ClearAllPreviews();
	m_preRailing.SetRowCount(7);
	m_preRailing.SetColumnCount(2);
	m_preRailing.SetDisplayRows(2);
	m_preRailing.SetDisplayColumns(2);

	for (UINT i = 0; i < 14; i++)
	{
		CString str;
		CString path;
		if (i < 7)
		{
			if (i <= 1)
			{
				sPrototypeName.Format(_T("Railing_B%d"),i+1);
				str.Format(_T("原型编号：%s\n栏杆类型：铁艺栏杆\n栏杆高度：%d\n栏杆宽度：%d\n"),sPrototypeName, railingAtt.m_height, railingAtt.m_length);
				path.Format(TY_GetLocalImagePath() + ("Railing_B%d.png"),i+1);
				CGridCellForPreview* pCell = m_preRailing.AddPreviewPng(i/2, i%2, path, str);
				pCell->SetName(sPrototypeName);
			}
			else if (i > 1 && i < 4)
			{
				sPrototypeName.Format(_T("原型编号：Railing_B3_%d"),i-1);
				str.Format(_T("原型编号：%s\n栏杆类型：铁艺栏杆\n栏杆高度：%d\n栏杆宽度：%d\n"),sPrototypeName, railingAtt.m_height, railingAtt.m_length);
				path.Format(TY_GetLocalImagePath() + ("Railing_B3_%d.png"),i-1);
				CGridCellForPreview* pCell = m_preRailing.AddPreviewPng(i/2, i%2, path, str);	
				pCell->SetName(sPrototypeName);
			}
			else
			{
				sPrototypeName.Format(_T("原型编号：Railing_B%d"),i);
				str.Format(_T("原型编号：%s\n栏杆类型：铁艺栏杆\n栏杆高度：%d\n栏杆宽度：%d\n"),sPrototypeName, railingAtt.m_height, railingAtt.m_length);
				path.Format(TY_GetLocalImagePath() + ("Railing_B%d.png"),i);
				CGridCellForPreview* pCell = m_preRailing.AddPreviewPng(i/2, i%2, path, str);
				pCell->SetName(sPrototypeName);
			}
		}
		else
		{
			sPrototypeName.Format(_T("原型编号：Railing_T%d"),i-6);
			str.Format(_T("原型编号：%s\n栏杆类型：铁艺栏杆\n栏杆高度：%d\n栏杆宽度：%d\n"),sPrototypeName, railingAtt.m_height, railingAtt.m_length);
			path.Format(TY_GetLocalImagePath() + ("Railing_T%d.png"),i-6);
			CGridCellForPreview* pCell = m_preRailing.AddPreviewPng(i/2, i%2, path, str);
			pCell->SetName(sPrototypeName);
		}
	}

	m_preRailing.SelectPreview(0, 0);
}