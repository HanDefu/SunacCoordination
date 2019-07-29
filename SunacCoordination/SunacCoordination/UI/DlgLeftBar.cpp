// DlgLeftBar.cpp : implementation file
//

#include "stdafx.h"
#include "DlgLeftBar.h"
#include "afxdialogex.h"


// DlgLeftBar dialog

IMPLEMENT_DYNAMIC(DlgLeftBar, CDialog)

DlgLeftBar::DlgLeftBar(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(DlgLeftBar::IDD, pParent)
{

}

DlgLeftBar::~DlgLeftBar()
{
}

void DlgLeftBar::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgLeftBar, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &DlgLeftBar::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgLeftBar message handlers


void DlgLeftBar::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}
