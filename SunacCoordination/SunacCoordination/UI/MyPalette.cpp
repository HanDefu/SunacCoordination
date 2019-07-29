#include "StdAfx.h"
#include "MyPalette.h"

IMPLEMENT_DYNAMIC(CMyPalette,CAdUiPalette)

CMyPalette::CMyPalette(void)
{
}

CMyPalette::~CMyPalette(void)
{
}
BEGIN_MESSAGE_MAP(CMyPalette, CAdUiPalette)
	ON_WM_CREATE()
	//ON_BN_CLICKED(121,OnClick1)
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CMyPalette::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CAdUiPalette::OnCreate(lpCreateStruct) == -1)
		return -1;

	//自定义的按钮控件，可以响应
	//谢刚
	//CButton *pButton = new CButton;
	//CRect rect(30,30,100,60);
	//pButton->Create(_T("Hello"),WS_VISIBLE|WS_CHILD,rect,this,121);

	CAcModuleResourceOverride myResource;
	m_leftBar.Create(IDD_DIALOG_LEFTBAR,this);

	CRect rect;
	GetWindowRect(rect);

	m_leftBar.ShowWindow(SW_SHOW);
	m_leftBar.MoveWindow(0,0,rect.Width(),rect.Height(),TRUE);

	return 0;
}

void CMyPalette::OnClick1( void )
{
	MessageBox(_T("Hello,welcome to test!"),_T("提示"),MB_OK);
}

void CMyPalette::OnSize(UINT nType, int cx, int cy)
{
	CAdUiPalette::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (::IsWindow(m_leftBar.GetSafeHwnd()))
	{
		m_leftBar.MoveWindow(0,0,cx,cy,TRUE);
	}
}
