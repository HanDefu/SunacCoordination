// UI\BathroomDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BathroomDlg.h"
#include "afxdialogex.h"


// CBathroomDlg 对话框

IMPLEMENT_DYNAMIC(CBathroomDlg, CDialogEx)

CBathroomDlg::CBathroomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBathroomDlg::IDD, pParent)
{

}

CBathroomDlg::~CBathroomDlg()
{
}

void CBathroomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBathroomDlg, CDialogEx)
END_MESSAGE_MAP()


// CBathroomDlg 消息处理程序
