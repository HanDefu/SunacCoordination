// UI\MoldingsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MoldingsDlg.h"
#include "afxdialogex.h"


// CMoldingsDlg �Ի���

IMPLEMENT_DYNAMIC(CMoldingsDlg, CDialogEx)

CMoldingsDlg::CMoldingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMoldingsDlg::IDD, pParent)
{

}

CMoldingsDlg::~CMoldingsDlg()
{
}

void CMoldingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_MOLDINGS, m_preMoldings);
}


BEGIN_MESSAGE_MAP(CMoldingsDlg, CDialogEx)
END_MESSAGE_MAP()


// CMoldingsDlg ��Ϣ��������


BOOL CMoldingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}