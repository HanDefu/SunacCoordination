// UI\RailingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RailingDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
//#include "RailingBaseDlg.h"
#include "../Object/Railing/RCRailing.h"


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
	, m_height(1100)
	, m_heightBase(200)
	, m_width(5400)
{

}

CRailingDlg::~CRailingDlg()
{
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
END_MESSAGE_MAP()


// CRailingDlg 消息处理程序

LRESULT CRailingDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	//return FALSE;
	return TRUE;
}

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

	m_type.AddString(_T("铁艺栏杆"));
	m_type.AddString(_T("玻璃栏杆"));
	m_type.SetCurSel(0);

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

void CRailingDlg::OnBnClickedInsertToCAD()
{
	UpdateData();

	//检查数据
	if (m_width<1500)
	{
		AfxMessageBox(_T("栏杆长度太短"));
		return;
	}

	//TODO 必须选择栏杆类型，必须选择栏杆原型

	if (m_selectedFile.GetLength() > 0)
	{
		//CRCRailing* pRailing = CreateRailing();
		//pRailing->InsertRailing(pnt1, pnt2, m_selectedFile);
	}

	//生成

	AttrRailing railingAtt;
	railingAtt.m_height = m_height;
	railingAtt.m_length = m_width;
	railingAtt.m_prototypeCode = _T("Railing_T1"); //TODO 支持其他类型
	railingAtt.m_railingType = E_RAILING_TIEYI;

	CRCRailing* pRailing = CreateRailing(railingAtt);

	pRailing->GenerateRailing(AcGePoint3d(0, 0, 0), AcGePoint3d(5400, 0, 0));

	delete pRailing;

	OnOK();
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
