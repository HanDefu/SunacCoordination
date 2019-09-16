// UI\RailingBaseDlg.cpp : 实现文件
//

#include "stdafx.h"
/*
#include "RailingBaseDlg.h"
#include "afxdialogex.h"
#include "../Common/ComFun_Sunac.h"
#include "RailingDlg.h"
// CRailingBaseDlg 对话框

IMPLEMENT_DYNAMIC(CRailingBaseDlg, CDialogEx)

CRailingBaseDlg::CRailingBaseDlg(CWnd* pParent / *=NULL* /)
	: CDialogEx(CRailingBaseDlg::IDD, pParent)
{
	m_parent = 0;
}

CRailingBaseDlg::~CRailingBaseDlg()
{
}

void CRailingBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW_RAILINGBASE, m_libPreview);
	DDX_Control(pDX, IDC_TREE_RAILINGBASE, m_category);
}

LRESULT CRailingBaseDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	//return FALSE;
	return TRUE;
}

BEGIN_MESSAGE_MAP(CRailingBaseDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON_LIB, &CRailingBaseDlg::OnBnClickedMfcbuttonOK)
	ON_NOTIFY(NM_CLICK, IDC_TREE_RAILINGBASE, &CRailingBaseDlg::OnNMClickTreeRailingbase)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
END_MESSAGE_MAP()


// CRailingBaseDlg 消息处理程序


BOOL CRailingBaseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitTree();
	FillTreeItem();
	InitLib();
	return TRUE;
}

int CRailingBaseDlg::InitLib()
{
	m_libPreview.LoadDefaltSettings();

	m_libPreview.SetRowCount(4);
	m_libPreview.SetColumnCount(2);

	//设置预览图显示区域为3行1列
	m_libPreview.SetDisplayRows(3);
	m_libPreview.SetDisplayColumns(2);

	//设置第一列占60%，其余部分为第二列
	//m_libPreview.SetHeaderWidth(_T("60;+"));

	TY_GetAllTieYiLanGanFiles(m_allFilePathNames);//临时测试

	RefreshPreviewFiles();

	return 0;
}

void CRailingBaseDlg::RefreshPreviewFiles()
{
	m_libPreview.ClearAllPreviews();
	for (int i = 0; i < m_allFilePathNames.size(); i++)
	{
		m_libPreview.AddPreview(i/2, i%2, m_allFilePathNames[i]);
	}
	m_libPreview.ShowPreviews();
}

int CRailingBaseDlg::InitTree()
{
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
	m_category.ModifyStyle(1,dwViewStyle);

	UINT uiBmpId = IDB_BITMAP1;
	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return false;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	m_treeImages.Create(16, bmpObj.bmHeight, ILC_MASK | ILC_COLOR24, 0, 0);
	m_treeImages.Add(&bmp, RGB(255, 0, 255));
	m_category.SetImageList(&m_treeImages, TVSIL_NORMAL);

	return true;
}

int CRailingBaseDlg::FillTreeItem()
{
	m_hroot = m_category.InsertItem(_T("栏杆库"), 0, 0);
	m_category.SetItemState(m_hroot, TVIS_BOLD, TVIS_BOLD);

	m_hlibTieYi = m_category.InsertItem(_T("铁艺栏杆"), 0, 0, m_hroot);
	m_hlibBoLi = m_category.InsertItem(_T("玻璃栏杆"), 0, 0, m_hroot);

	m_category.InsertItem(_T("TY-11B-10"), 1, 1, m_hlibTieYi);
	m_category.InsertItem(_T("TY-11B-11"), 1, 1, m_hlibTieYi);
	m_category.InsertItem(_T("TY-11B-12"), 1, 1, m_hlibTieYi);
	m_category.InsertItem(_T("TY-11B-13"), 1, 1, m_hlibTieYi);
	m_category.InsertItem(_T("TY-11B-14"), 1, 1, m_hlibTieYi);
	m_category.InsertItem(_T("TY-11B-15"), 1, 1, m_hlibTieYi);
	m_category.InsertItem(_T("TY-11B-16"), 1, 1, m_hlibTieYi);

	m_category.InsertItem(_T("子类型1"), 1, 1, m_hlibBoLi);
	m_category.InsertItem(_T("子类型2"), 1, 1, m_hlibBoLi);
	m_category.InsertItem(_T("子类型3"), 1, 1, m_hlibBoLi);
	m_category.InsertItem(_T("子类型4"), 1, 1, m_hlibBoLi);
	m_category.InsertItem(_T("子类型5"), 1, 1, m_hlibBoLi);
	m_category.InsertItem(_T("子类型6"), 1, 1, m_hlibBoLi);

	m_category.Expand(m_hroot, TVE_EXPAND);
	m_category.Expand(m_hlibBoLi, TVE_EXPAND);
	m_category.Expand(m_hlibTieYi, TVE_EXPAND);

	return 0;
}


void CRailingBaseDlg::OnBnClickedMfcbuttonOK()//
{
	CCellRange range = m_libPreview.GetSelectedCellRange();
	int row = range.GetMaxRow();
	int col = range.GetMaxCol();

	int index = row * 2 + col;

	if (index < m_allFilePathNames.size())
	{
	    m_selectedFile = m_allFilePathNames[index];
		if (m_parent != 0)
		{
			CRailingDlg * pRailingDlg = (CRailingDlg *)m_parent;
			pRailingDlg->UpdateSelectFile(m_selectedFile);
		}
	}
	OnOK();
}


void CRailingBaseDlg::OnNMClickTreeRailingbase(NMHDR *pNMHDR, LRESULT *pResult)
{
	DWORD dw = GetMessagePos();
	CPoint pt(LOWORD(dw),HIWORD(dw)); //鼠标的屏幕坐标
	CPoint ptClient;
	CRect rcPart;
	m_category.GetWindowRect(rcPart);
	ptClient.x = pt.x - rcPart.left;
	ptClient.y = pt.y - rcPart.top;
	UINT uFlags;
	m_selItem = m_category.HitTest(ptClient, &uFlags);
	if ((m_selItem != NULL))
	{
		if (m_category.ItemHasChildren(m_selItem))
		{
			 CString folderName = m_category.GetItemText(m_selItem);
			 if (folderName == L"铁艺栏杆")
			 {
				 TY_GetAllTieYiLanGanFiles(m_allFilePathNames);
				 RefreshPreviewFiles();
			 }
			 else if(folderName == L"玻璃栏杆")
			 {
				 TY_GetAllBoLiLanGanFiles(m_allFilePathNames);
				 RefreshPreviewFiles();
			 }
		}
		m_category.Select(m_selItem, TVGN_CARET);
		*pResult = 0;
		return;
	}
	*pResult = 0;
}
*/
