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
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CMyPalette::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CAdUiPalette::OnCreate(lpCreateStruct) == -1)
		return -1;

	CAcModuleResourceOverride myResource;
	m_leftBar.Create(IDD_DIALOG_LEFTBAR,this);

	CRect rect;
	GetWindowRect(rect);

	m_leftBar.ShowWindow(SW_SHOW);
	m_leftBar.MoveWindow(0,0,rect.Width(),rect.Height(),TRUE);

	return 0;
}

void CMyPalette::OnSize(UINT nType, int cx, int cy)
{
	CAdUiPalette::OnSize(nType, cx, cy);

	if (::IsWindow(m_leftBar.GetSafeHwnd()))
	{
		m_leftBar.MoveWindow(0,0,cx,cy,TRUE);
	}
}
