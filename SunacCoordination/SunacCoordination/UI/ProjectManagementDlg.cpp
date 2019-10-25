// ProjectManagementDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProjectManagementDlg.h"
#include "afxdialogex.h"
#include "../UI/GridCtrl_src/GridCtrlUtil.h"
#include "..\ProjectorFileMrg\ProjectFileMrg.h"


// CProjectManagementDlg �Ի���

IMPLEMENT_DYNAMIC(CProjectManagementDlg, CAcUiDialog)

CProjectManagementDlg::CProjectManagementDlg(CProjectData* pPrjData,CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CProjectManagementDlg::IDD, pParent)
	, m_pPrjData(pPrjData)
{

}

CProjectManagementDlg::~CProjectManagementDlg()
{
}

void CProjectManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DELETEALL, m_BtnDeleteAll);
	DDX_Control(pDX, IDC_BUTTON_DOWNLOADALL, m_BtnDownloadAll);
	DDX_Control(pDX, IDC_BUTTON_UPLOAD, m_BtnUpload);
	DDX_Control(pDX, IDC_STATIC_AREA, m_StcArea);
	DDX_Control(pDX, IDC_STATIC_COMPANY, m_StcCompany);
	DDX_Control(pDX, IDC_STATIC_DOCUMENTNAME, m_StcDocName);
	DDX_Control(pDX, IDC_STATIC_DOCUMENTSIZE, m_StcDocSize);
	DDX_Control(pDX, IDC_STATIC_PROJECTNAME, m_StcPrjName);
	DDX_Control(pDX, IDC_STATIC_UPDATERNAME, m_StcUpdaterName);
	DDX_Control(pDX, IDC_STATIC_UPDATETIME, m_StcUpdateTime);
	DDX_Control(pDX, IDC_STATIC_UPLOADERNAME, m_StcUploaderName);
	DDX_Control(pDX, IDC_STATIC_UPLOADTIME, m_StcUploadTime);
	DDX_Control(pDX, IDC_TREE_PRJDIR, m_TreePrjDir);
	DDX_Control(pDX, IDC_GRIDCTRL_PRJMANAGEMENT, m_PjtManagementGridCtrl);
	DDX_Control(pDX, IDC_STATIC_ROOTNAME, m_StcRootName);
}


BEGIN_MESSAGE_MAP(CProjectManagementDlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CProjectManagementDlg::OnBnClickedButtonUpload)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PRJDIR, &CProjectManagementDlg::OnNMClickTreePrjdir)
END_MESSAGE_MAP()

void CProjectManagementDlg::FillPjtMngTreeCtrl()
{
	HICON Icon[3];
	CImageList *ImageList = new CImageList();
	ImageList->Create(16,16,ILC_COLOR24,12,12);
	Icon[1] = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	Icon[2] = AfxGetApp()->LoadIcon(IDI_ICON_DOCUMENT);

	for (int i = 0; i < 3; i++)
	{
		ImageList->Add(Icon[i]);
	}
	m_TreePrjDir.SetImageList(ImageList, TVSIL_NORMAL);

	//vector <HTREEITEM> Areas;
	HTREEITEM hTreeItem;
	hTreeItem = m_TreePrjDir.InsertItem(L"��Ŀ�ļ���", 3, 3,TVI_ROOT);
	m_TreePrjDir.SetItemHeight(30);
	for (UINT i = 0; i < m_pPrjData->m_rootDir.m_subDirs.size(); i++)
	{
		HTREEITEM hFolder = m_TreePrjDir.InsertItem(m_pPrjData->m_rootDir.m_subDirs[i]->m_sName, hTreeItem);
		for (UINT j = 0; j < m_pPrjData->m_rootDir.m_subDirs[i]->m_subDirs.size(); j++)
		{
			m_TreePrjDir.InsertItem(m_pPrjData->m_rootDir.m_subDirs[i]->m_subDirs[j]->m_sName, 1, 1, hFolder);
		}
	}
	m_TreePrjDir.Expand(hTreeItem, TVE_EXPAND);
}

// CProjectManagementDlg ��Ϣ��������


void CProjectManagementDlg::OnBnClickedButtonUpload()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
}


BOOL CProjectManagementDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	CFont Font;
	Font.CreatePointFont(160, L"");
	m_StcRootName.SetFont(&Font);
	Font.Detach();

	InitGridCtrl();
	FillPjtMngTreeCtrl();

	m_StcPrjName.SetWindowTextW(m_pPrjData->m_prjInfo.m_sName);
	m_StcArea.SetWindowTextW(m_pPrjData->m_prjInfo.m_sArea);
	m_StcCompany.SetWindowTextW(m_pPrjData->m_prjInfo.m_sCityCompany);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CProjectManagementDlg::InitGridCtrl()
{
	m_PjtManagementGridCtrl.DeleteAllItems();
	
	m_PjtManagementGridCtrl.SetRowCount(1);
	m_PjtManagementGridCtrl.SetColumnCount(9);
	m_PjtManagementGridCtrl.SetItemText(0, 1, L"�ļ�����");
	m_PjtManagementGridCtrl.SetItemText(0, 2, L"������");
	m_PjtManagementGridCtrl.SetItemText(0, 3, L"����ʱ��");
	m_PjtManagementGridCtrl.SetItemText(0, 4, L"������Ա");
	m_PjtManagementGridCtrl.SetItemText(0, 5, L"����ʱ��");
	m_PjtManagementGridCtrl.SetItemText(0, 6, L"�ļ���С");

	m_PjtManagementGridCtrl.SetColumnWidth(0, 30);
	m_PjtManagementGridCtrl.SetColumnWidth(1, 140);
	m_PjtManagementGridCtrl.SetColumnWidth(2, 60);
	m_PjtManagementGridCtrl.SetColumnWidth(3, 150);
	m_PjtManagementGridCtrl.SetColumnWidth(4, 60);
	m_PjtManagementGridCtrl.SetColumnWidth(5, 140);
	m_PjtManagementGridCtrl.SetColumnWidth(6, 80);
	m_PjtManagementGridCtrl.SetColumnWidth(7, 40);
	m_PjtManagementGridCtrl.SetColumnWidth(8, 40);

	m_PjtManagementGridCtrl.SetSingleColSelection(TRUE);

	COLORREF color=RGB(220,220,220);
	for (UINT i = 0; i < 9; i++)
	{
		m_PjtManagementGridCtrl.SetItemBkColour(0, i, color);
	}	

}

void CProjectManagementDlg::FillPjtGridCtrl(CProjectDir* SelectedDir)
{	
	InitGridCtrl();
	if (SelectedDir->m_subFiles.empty())
	{
		return;
	}
	m_PjtManagementGridCtrl.SetRowCount((int)SelectedDir->m_subFiles.size() + 1);
	for (UINT i = 1; i <= SelectedDir->m_subFiles.size(); i++)
	{
		m_PjtManagementGridCtrl.SetItemText(i, 1, SelectedDir->m_subFiles[i - 1].m_sName);
		m_PjtManagementGridCtrl.SetItemText(i, 2, SelectedDir->m_subFiles[i - 1].m_sCreator);
		m_PjtManagementGridCtrl.SetItemText(i, 3, SelectedDir->m_subFiles[i - 1].m_sCreateTime);
		m_PjtManagementGridCtrl.SetItemText(i, 4, SelectedDir->m_subFiles[i - 1].m_sUpdator);
		m_PjtManagementGridCtrl.SetItemText(i, 5, SelectedDir->m_subFiles[i - 1].m_sUpdateTime);
		m_PjtManagementGridCtrl.SetItemText(i, 6, SelectedDir->m_subFiles[i - 1].m_sFileSize);
		m_PjtManagementGridCtrl.SetItemText(i, 7, L"����");
		CGridCtrlUtil::SetCellButtonType(m_PjtManagementGridCtrl, i, 7);
		m_PjtManagementGridCtrl.SetItemState(i, 7, GVIS_READONLY);
		m_PjtManagementGridCtrl.SetItemText(i, 8, L"ɾ��");
		CGridCtrlUtil::SetCellButtonType(m_PjtManagementGridCtrl, i, 8);
		m_PjtManagementGridCtrl.SetItemState(i, 8, GVIS_READONLY);
		
	}
	UpdateData();
}

CProjectDir* CProjectManagementDlg::FindClkDir(HTREEITEM CurClkItem)
{
	if (CurClkItem == m_TreePrjDir.GetRootItem())
	{
		return &m_pPrjData->m_rootDir;
	}

	CProjectDir* pParentDir = FindClkDir(m_TreePrjDir.GetParentItem(CurClkItem));
	assert(pParentDir);
	CProjectDir* pDir = pParentDir->GetFolder(m_TreePrjDir.GetItemText(CurClkItem));
	//assert(pDir);
	return pDir;
}


void CProjectManagementDlg::OnNMClickTreePrjdir(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	*pResult = 0;
	CPoint CurClkPoint;
	GetCursorPos(&CurClkPoint);
	m_TreePrjDir.ScreenToClient(&CurClkPoint);
	HTREEITEM CurClkItem;
	/*UINT flag;
	CurClkItem = m_treeProjectManagement.HitTest(CurClkPoint, &flag);*/
	CurClkItem = m_TreePrjDir.GetSelectedItem();
	if (m_TreePrjDir.ItemHasChildren(CurClkItem))
	{
		return;
	}
	m_selectedDir = FindClkDir(CurClkItem);
	if (m_selectedDir == nullptr)
	{
		return;
	}
	else if (m_selectedDir->m_subFiles.empty())
	{
		InitGridCtrl();
		return;
	}
	FillPjtGridCtrl(m_selectedDir);
}