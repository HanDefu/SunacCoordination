// UI\WindowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/RCWindow.h"
#include "../WebIO/WebIO.h"
#include "../GlobalSetting.h"
#include "../WebIO/WindowLocalData.h"
#include "../Object/AttrWindow.h"

// CWindowDlg 对话框

IMPLEMENT_DYNAMIC(CWindowDlg, CAcUiDialog)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CWindowDlg::IDD, pParent)
	, m_radioDoor(0)
	, m_radioYes(0)
{

}

CWindowDlg::~CWindowDlg()
{
}

LRESULT CWindowDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	//return FALSE;
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
}


BEGIN_MESSAGE_MAP(CWindowDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_INSERTWINDOW, &CWindowDlg::OnBnClickedMfcbuttonInsert)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHWINDOW, &CWindowDlg::OnBnClickedButtonSearchwindow)
	ON_BN_CLICKED(IDC_RADIO_DOOR, &CWindowDlg::OnBnClickedRadioDoor)
	ON_BN_CLICKED(IDC_RADIO_WINDOW, &CWindowDlg::OnBnClickedRadioDoor)
END_MESSAGE_MAP()


// CWindowDlg 消息处理程序


BOOL CWindowDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_preWindow.LoadDefaltSettings();

	m_areaType.SetCurSel(0);
	m_openType.SetCurSel(0);
	m_openAmount.SetCurSel(0);
	m_openWidth.SetCurSel(0);

	m_width.SetWindowText(L"2000");
	m_height.SetWindowText(L"2000");
	m_H2.SetWindowText(L"450");

	SetRadioDoor(0);

	return TRUE;
}

void CWindowDlg::OnBnClickedMfcbuttonInsert()
{
	ShowWindow(FALSE);

	// TODO: 在此添加控件通知处理程序代码
	AcGePoint3d origin = TY_GetPoint();

	double width = 0, height = 0;
	CString str;
	m_width.GetWindowText(str);
	width = _wtof(str.GetBuffer());
	m_height.GetWindowText(str);
	height = _wtof(str.GetBuffer());

	vector<int> sels = m_preWindow.GetSelectedRows();

	m_openWidth.GetWindowText(str);
	double W1 = _wtof(str.GetBuffer());

	m_H2.GetWindowText(str);
	double H2 = _wtof(str.GetBuffer());

	if (sels.size() > 0)
	{
		RCWindow oneWindow;
			
		oneWindow.Insert(m_allWindws[sels[0]]->m_filePathName, origin, 0, L"0", 256);

		oneWindow.InitParameters();
		oneWindow.SetParameter(L"H", height);
		oneWindow.SetParameter(L"W", width);

		oneWindow.SetParameter(L"W1", W1);
		oneWindow.SetParameter(L"H2", H2);

		oneWindow.RunParameters();

		str.Format(L"%d_%d",(int)(oneWindow.GetW()), (int)(oneWindow.GetH()));
			
		//把UI的数据记录在图框的扩展字典中
		AttrWindow * pWindow = new AttrWindow(*m_allWindws[sels[0]]);
		oneWindow.AddAttribute(pWindow);
		pWindow->close();

		oneWindow.SetBianHao(m_allWindws[sels[0]]->m_yxid + str);
	}
	OnOK();
}


void CWindowDlg::OnBnClickedButtonSearchwindow()
{
	m_allWindws = WebIO::GetAllWindows();
	m_preWindow.ClearAllPreviews();
	m_preWindow.SetRowCount((int)m_allWindws.size());
	m_preWindow.SetColumnCount(1);
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(1);
	for (int i = 0; i < m_allWindws.size(); i++)
		m_preWindow.AddPreview(i, 0, m_allWindws[i]->m_filePathName, _T("窗类型:双扇单开\n窗户面积:2.1\n通风量:1.6"));
}

void CWindowDlg::OnBnClickedRadioDoor()
{
	UpdateData(TRUE);
	SetRadioDoor(m_radioDoor);
}

void CWindowDlg::SetRadioDoor(int radioDoor)
{
	if (radioDoor < 0 || radioDoor > 1)
		return;
	m_radioDoor = radioDoor;

	if (radioDoor == 0)
	{
		m_doorType.EnableWindow(TRUE);
		m_openType.EnableWindow(FALSE);
		m_openAmount.EnableWindow(FALSE);
		m_openWidth.EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_YES)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_NO)->EnableWindow(FALSE);
	}
	else
	{
		m_openType.EnableWindow(TRUE);
		m_openAmount.EnableWindow(TRUE);
		m_openWidth.EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_YES)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_NO)->EnableWindow(TRUE);
		m_doorType.EnableWindow(FALSE);
	}
}

