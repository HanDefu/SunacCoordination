// UI\WindowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/RCWindow.h"
#include "../WebIO.h"
#include "../GlobalSetting.h"

// CWindowDlg 对话框

IMPLEMENT_DYNAMIC(CWindowDlg, CDialogEx)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWindowDlg::IDD, pParent)
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
	CDialogEx::OnOK();
	DestroyWindow();
}

void CWindowDlg::OnCancel()
{
	CDialogEx::OnCancel();
	DestroyWindow();
}

void CWindowDlg::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
	delete this;
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_WINDOW, m_preWindow);
	DDX_Control(pDX, IDC_COMBO_AREATYPE, m_areaType);
	DDX_Control(pDX, IDC_COMBO_OPENTYPE, m_openType);
	DDX_Control(pDX, IDC_COMBO_OPENAMOUNT, m_openAmount);
	DDX_Control(pDX, IDC_COMBO_OPENWIDTH, m_openWidth);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_width);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_height);
	DDX_Control(pDX, IDC_EDIT_FIXEDVALUE, m_H2);
	DDX_Control(pDX, IDC_EDIT_FIXEDVALUE2, m_cengShu);
	DDX_Control(pDX, IDC_EDIT_FIXEDVALUE3, m_cengGao);
}


BEGIN_MESSAGE_MAP(CWindowDlg, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_MFCBUTTON_SELECTLINE, &CWindowDlg::OnBnClickedMfcbuttonInsert)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHWINDOW, &CWindowDlg::OnBnClickedButtonSearchwindow)
END_MESSAGE_MAP()


// CWindowDlg 消息处理程序


BOOL CWindowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_preWindow.LoadDefaltSettings();

	m_preWindow.SetRowCount(8);
	m_preWindow.SetColumnCount(2);

	//设置预览图显示区域为3行1列
	m_preWindow.SetDisplayRows(3);
	m_preWindow.SetDisplayColumns(1);

	//设置第一列占60%，其余部分为第二列
	m_preWindow.SetHeaderWidth(_T("60;+"));



	m_areaType.SetCurSel(0);
	m_openType.SetCurSel(0);
	m_openAmount.SetCurSel(0);
	m_openWidth.SetCurSel(0);

	m_width.SetWindowText(L"2000");
	m_height.SetWindowText(L"2000");
	m_H2.SetWindowText(L"450");

	m_cengShu.SetWindowText(L"1");
	m_cengGao.SetWindowText(L"3000");

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

	m_cengShu.GetWindowText(str);
	int cengShu = _wtoi(str.GetBuffer());

	m_cengGao.GetWindowText(str);
	double cengGao = _wtof(str.GetBuffer());

	if (sels.size() > 0)
	{
		for (int i = 0; i < cengShu; i++)
		{
			RCWindow oneWindow;
			
			oneWindow.Insert(m_allWindws[sels[0]]->m_filePathName, origin, 0, L"0", 256);

			oneWindow.InitParameters();
			oneWindow.SetParameter(L"H", height);
			oneWindow.SetParameter(L"W", width);

			oneWindow.SetParameter(L"W1", W1);
			oneWindow.SetParameter(L"H2", H2);

			oneWindow.RunParameters();
			origin.y += cengGao;

			str.Format(L"%d_%d",(int)(oneWindow.GetW()), (int)(oneWindow.GetH()));
			
			//把UI的数据记录在图框的扩展字典中
			AttrWindow * pWindow = new AttrWindow(*m_allWindws[sels[0]]);
			oneWindow.AddAttribute(pWindow);
			pWindow->close();

			oneWindow.SetBianHao(m_allWindws[sels[0]]->m_yxid + str); 
		}
		
	}
	ShowWindow(true);
	OnOK();
}


void CWindowDlg::OnBnClickedButtonSearchwindow()
{
	m_allWindws = WebIO::GetAllWindows();

	for (int i = 0; i < m_allWindws.size(); i++)
	{
		m_preWindow.AddPreview(i, 0, m_allWindws[i]->m_filePathName); 
		m_preWindow.SetContentItemText(i, 1, _T("窗类型:双扇单开\n窗户面积:2.1\n通风量:1.6"));
	}


	m_preWindow.ShowPreviews();
}

