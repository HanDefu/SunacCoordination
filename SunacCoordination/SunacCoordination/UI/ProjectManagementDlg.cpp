// ProjectManagementDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProjectManagementDlg.h"
#include "NewDirDlg.h"
#include "afxdialogex.h"
#include "../UI/GridCtrl_src/GridCtrlUtil.h"
#include "..\ProjectorFileMrg\ProjectFileMrg.h"
#include "..\ProjectorFileMrg\ProjectInfo.h"
#include "..\ProjectorFileMrg\FileUploadDownload.h"
#include "..\ProjectorFileMrg\ProjectFile.h"
#include "../Common/ComFun_String.h"


// CProjectManagementDlg 对话框

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
	//DDX_Control(pDX, IDC_STATIC_ROOTNAME, m_StcRootName);
}


LRESULT CProjectManagementDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

BEGIN_MESSAGE_MAP(CProjectManagementDlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CProjectManagementDlg::OnBnClickedButtonUpload)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PRJDIR, &CProjectManagementDlg::OnNMClickTreePrjdir)
	ON_NOTIFY(NM_CLICK, IDC_GRIDCTRL_PRJMANAGEMENT, OnGridClick)
	ON_BN_CLICKED(IDC_BUTTON_NewDir, &CProjectManagementDlg::OnBnClickedButtonNewdir)
	ON_BN_CLICKED(IDC_BUTTON_DeleteDir, &CProjectManagementDlg::OnBnClickedButtonDeletedir)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOADALL, &CProjectManagementDlg::OnBnClickedButtonDownloadall)
	ON_BN_CLICKED(IDC_BUTTON_DELETEALL, &CProjectManagementDlg::OnBnClickedButtonDeleteall)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
END_MESSAGE_MAP()

CString SelFilePath()
{
	TCHAR           szFolderPath[MAX_PATH] = {0};  
	CString         strFolderPath = TEXT("");  

	BROWSEINFO      sInfo;  
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	
	
	sInfo.pidlRoot   = NULL;  
	sInfo.lpszTitle   = _T("请选择文件存储路径");  
	sInfo.ulFlags   = BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_DONTGOBELOWDOMAIN|BIF_USENEWUI;  
	sInfo.lpfn     = NULL;  

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
	if (lpidlBrowse != NULL)  
	{  
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))    
		{  
			strFolderPath = szFolderPath;  
		}
		else
		{
			return _T("");
		}
	}  
	if(lpidlBrowse != NULL)  
	{  
		::CoTaskMemFree(lpidlBrowse);  
	}  

	return strFolderPath;  
}

void CProjectManagementDlg::FillPjtMngTreeCtrl()
{
	m_TreePrjDir.DeleteAllItems();
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

	HTREEITEM hTreeItem;
	hTreeItem = m_TreePrjDir.InsertItem(L"项目文件夹", 3, 3,TVI_ROOT);
	m_TreePrjDir.SetItemHeight(30);

	vector<const CProjectDir*> allDirs;
	vector<HTREEITEM> allItems;
	allDirs.push_back(m_pPrjData->GetRootDir());
	allItems.push_back(hTreeItem);

	for (UINT i = 0; i < allDirs.size(); i++)
	{
		for (UINT j = 0; j < allDirs[i]->m_subDirs.size(); j++)
		{
			HTREEITEM item = m_TreePrjDir.InsertItem(allDirs[i]->m_subDirs[j]->m_sName, 0, 0, allItems[i]);
			allDirs.push_back(allDirs[i]->m_subDirs[j]);
			allItems.push_back(item);
		}
	}
	m_TreePrjDir.Expand(hTreeItem, TVE_EXPAND);
}

// CProjectManagementDlg 消息处理程序

static void UIFileUpCBFunc(CUpDownFilePara* p_fileUpdownPara)
{
	//更新当前文件的状态
	g_projectManagementDlg->FillPjtGridCtrl(g_projectManagementDlg->m_selectedDir);
}

void CProjectManagementDlg::OnBnClickedButtonUpload()
{
	CString FileName;
	CString filter = L"参数文件(*.dwg)|*.dwg|All Files(*.*)|*.*||";
    CFileDialog dlg(FALSE, L"dwg", L"*.dwg", NULL, filter);
	if(dlg.DoModal() == IDOK)
	{
		USES_CONVERSION;
		CString PathName = dlg.GetPathName();
		
		FileName = FilePathToFileName(PathName);
		CString ParentPath = m_pPrjData->GetDirString(L"", m_selectedDir);//返回文件夹的路径

		m_pPrjData->AddFile(PathName, ParentPath, UIFileUpCBFunc);
	}

	FillPjtGridCtrl(m_selectedDir);
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

	m_StcPrjName.SetWindowTextW(m_pPrjData->GetPrjInfo().m_sName);
	m_StcArea.SetWindowTextW(m_pPrjData->GetPrjInfo().m_sArea);
	m_StcCompany.SetWindowTextW(m_pPrjData->GetPrjInfo().m_sCityCompany);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CProjectManagementDlg::InitGridCtrl()
{
	m_PjtManagementGridCtrl.DeleteAllItems();
	
	m_PjtManagementGridCtrl.SetFixedRowCount(1);
	m_PjtManagementGridCtrl.SetColumnCount(10);
	m_PjtManagementGridCtrl.SetItemText(0, 1, L"文件名称");
	m_PjtManagementGridCtrl.SetItemText(0, 2, L"创建人");
	m_PjtManagementGridCtrl.SetItemText(0, 3, L"创建时间");
	m_PjtManagementGridCtrl.SetItemText(0, 4, L"更新人员");
	m_PjtManagementGridCtrl.SetItemText(0, 5, L"更新时间");
	m_PjtManagementGridCtrl.SetItemText(0, 6, L"文件大小");
	m_PjtManagementGridCtrl.SetItemText(0, 7, L"文件状态");

	m_PjtManagementGridCtrl.SetHeaderWidth(L"3.33;+;8.33;15.56;8.33;15.56;8.89;8.33;4.44;4.44");

	/*m_PjtManagementGridCtrl.SetColumnWidth(0, 25);
	m_PjtManagementGridCtrl.SetColumnWidth(1, 205);
	m_PjtManagementGridCtrl.SetColumnWidth(2, 75);
	m_PjtManagementGridCtrl.SetColumnWidth(3, 140);
	m_PjtManagementGridCtrl.SetColumnWidth(4, 75);
	m_PjtManagementGridCtrl.SetColumnWidth(5, 140);
	m_PjtManagementGridCtrl.SetColumnWidth(6, 80);
	m_PjtManagementGridCtrl.SetColumnWidth(7, 75);
	m_PjtManagementGridCtrl.SetColumnWidth(8, 40);
	m_PjtManagementGridCtrl.SetColumnWidth(9, 40);*/

	m_PjtManagementGridCtrl.SetSingleRowSelection(TRUE);

	//COLORREF color=RGB(220,220,220);
}

void CProjectManagementDlg::FillPjtGridCtrl(CProjectDir* SelectedDir)
{
	CAutoLock a(&m_Lock);

	m_PjtManagementGridCtrl.DeleteAllItems();
	m_PjtManagementGridCtrl.SetRowCount((int)SelectedDir->m_subFiles.size() + 1);
	for (UINT i = 1; i <= SelectedDir->m_subFiles.size(); i++)
	{
		m_PjtManagementGridCtrl.SetCellType(i, 0, RUNTIME_CLASS(CGridCellCheck));
		m_PjtManagementGridCtrl.SetItemText(i, 1, SelectedDir->m_subFiles[i - 1].m_sName);
		m_PjtManagementGridCtrl.SetItemText(i, 2, SelectedDir->m_subFiles[i - 1].m_sCreator);
		m_PjtManagementGridCtrl.SetItemText(i, 3, SelectedDir->m_subFiles[i - 1].m_sCreateTime);
		m_PjtManagementGridCtrl.SetItemText(i, 4, SelectedDir->m_subFiles[i - 1].m_sUpdator);
		m_PjtManagementGridCtrl.SetItemText(i, 5, SelectedDir->m_subFiles[i - 1].m_sUpdateTime);
		m_PjtManagementGridCtrl.SetItemText(i, 6, SelectedDir->m_subFiles[i - 1].m_sFileSize);
		m_PjtManagementGridCtrl.SetItemText(i, 7, EProjectFileStateToCString(SelectedDir->m_subFiles[i - 1].m_fileState));
		m_PjtManagementGridCtrl.SetItemText(i, 8, L"下载");
		CGridCtrlUtil::SetCellButtonType(m_PjtManagementGridCtrl, i, 8);
		m_PjtManagementGridCtrl.SetItemState(i, 8, GVIS_READONLY);

		m_PjtManagementGridCtrl.SetItemText(i, 9, L"删除");
		CGridCtrlUtil::SetCellButtonType(m_PjtManagementGridCtrl, i, 9);
		m_PjtManagementGridCtrl.SetItemState(i, 9, GVIS_READONLY);
	}

	UpdateData();
}
void CProjectManagementDlg::UpdateGridCtrlState(CProjectDir* SelectedDir)
{
	CAutoLock a(&m_Lock);
	
	UINT rowCount = (UINT)(m_PjtManagementGridCtrl.GetRowCount());
	for (UINT i = 1; i <= SelectedDir->m_subFiles.size() && i<rowCount; i++)
	{
		m_PjtManagementGridCtrl.SetItemText(i, 7, EProjectFileStateToCString(SelectedDir->m_subFiles[i - 1].m_fileState));	
	}

	UpdateData();
}

CProjectDir* CProjectManagementDlg::FindClkDir(HTREEITEM CurClkItem)
{

	CProjectDir* RootDir = (CProjectDir *)m_pPrjData->GetRootDir();
	if (CurClkItem == NULL)
	{
		return RootDir;
	}
	if (CurClkItem == m_TreePrjDir.GetRootItem())
	{
		return RootDir;
	}
	CProjectDir* pParentDir = FindClkDir(m_TreePrjDir.GetParentItem(CurClkItem));
	assert(pParentDir);
	CProjectDir* pDir = pParentDir->GetSubFolder(m_TreePrjDir.GetItemText(CurClkItem));
	//assert(pDir);
	return pDir;
}


void CProjectManagementDlg::OnNMClickTreePrjdir(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	CPoint CurClkPoint;
	GetCursorPos(&CurClkPoint);
	m_TreePrjDir.ScreenToClient(&CurClkPoint);
	HTREEITEM CurClkItem;
	/*UINT flag;
	CurClkItem = m_treeProjectManagement.HitTest(CurClkPoint, &flag);*/
	CurClkItem = m_TreePrjDir.GetSelectedItem();
	/*if (m_TreePrjDir.ItemHasChildren(CurClkItem))
	{
		InitGridCtrl();
		return;
	}*/
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


void CProjectManagementDlg::OnBnClickedButtonNewdir()
{
	CNewDirDlg dlg;
	if(IDOK != dlg.DoModal())
	{
		return;
	}

	CString sNewDir = dlg.m_sNewDir;
	if (sNewDir == L"")
	{
		AfxMessageBox(L"请输入目录名！");
		return;
	}
	CPoint CurClkPoint;
	GetCursorPos(&CurClkPoint);
	m_TreePrjDir.ScreenToClient(&CurClkPoint);
	HTREEITEM CurClkItem;
	CurClkItem = m_TreePrjDir.GetSelectedItem();
	m_selectedDir = FindClkDir(CurClkItem);
	m_selectedDir->AddFolder(sNewDir);
	m_pPrjData->AddFolder(m_pPrjData->GetDirString(L"", m_selectedDir), sNewDir);
	m_TreePrjDir.InsertItem(sNewDir, 0, 0, CurClkItem);
	m_TreePrjDir.Expand(CurClkItem, TVE_EXPAND);
}

void CProjectManagementDlg::OnGridClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNMHDR;
	m_nClkRow = pItem->iRow;
	m_nClkCol = pItem->iColumn;
	CString sSelectedFileName = m_PjtManagementGridCtrl.GetItemText(m_nClkRow, 1);
	if (m_selectedDir == NULL)
	{
		return;
	}
	CString sSelectedFileParentPath = m_pPrjData->GetDirString(L"", m_selectedDir);
	CProjectFile SelectedFile;

	m_selectedDir->FindFile(sSelectedFileName, SelectedFile);
	m_StcDocName.SetWindowTextW(SelectedFile.m_sName);
	//m_StcDocSize.SetWindowTextW(SelectedFile.GetFileSize()); 
	m_StcUpdaterName.SetWindowTextW(SelectedFile.m_sCreator); 
	m_StcUpdateTime.SetWindowTextW(SelectedFile.m_sCreateTime); 
	m_StcUploaderName.SetWindowTextW(SelectedFile.m_sUpdator); 
	m_StcUploadTime.SetWindowTextW(SelectedFile.m_sUpdateTime);

	if (m_nClkCol == 8)//下载
	{
		CString FolderPath = SelFilePath();
		if (FolderPath == _T(""))
		{
			return;
		}
		CString FileFullPath;
		FileFullPath = FolderPath + L"\\" + sSelectedFileName;
		m_pPrjData->DownloadFile(sSelectedFileParentPath, sSelectedFileName, FileFullPath, UIFileUpCBFunc);
		FillPjtGridCtrl(m_selectedDir);
	}

	if (m_nClkCol == 9)//删除
	{
		if (IDYES == AfxMessageBox(L"确定删除?"), MB_YESNO)
		{
			m_pPrjData->DeleteFile(sSelectedFileName,sSelectedFileParentPath);
			FillPjtGridCtrl(m_selectedDir);
		}
	}
}

void CProjectManagementDlg::OnBnClickedButtonDeletedir()
{
	CString sDeleteDir;
	sDeleteDir = m_pPrjData->GetDirString(L"", m_selectedDir);
	m_pPrjData->DeleteFolder(sDeleteDir);
	HTREEITEM DeleteTreeItem;
	DeleteTreeItem = m_TreePrjDir.GetSelectedItem();
	m_TreePrjDir.DeleteItem(DeleteTreeItem);
}


void CProjectManagementDlg::OnBnClickedButtonDownloadall()
{
	CString sFolderPath = SelFilePath();
	if (sFolderPath == _T(""))
	{
		return;
	}

	CString sDirPathInProject2 = m_selectedDir->GetDirFullPath();
	CString sDirPathInProject = m_pPrjData->GetDirString(L"", m_selectedDir);
	assert(sDirPathInProject == sDirPathInProject2);
	
	vector<CString> fileNames;
	for(int i = 1; i < m_PjtManagementGridCtrl.GetRowCount(); i++)
	{
		CGridCellBase* pCell = m_PjtManagementGridCtrl.GetCell(i, 0);
		if(((CGridCellCheck* )pCell)->GetCheck())
		{
			CString sCheckedFileName = m_PjtManagementGridCtrl.GetItemText(i, 1);
			CString sFileSavePath = sFolderPath + L"\\" + sCheckedFileName;
			fileNames.push_back(sCheckedFileName);

			//判断文件是否存在，若存在，则提示并退出

		}
	}


	for (int i = 0; i < fileNames.size(); i++)
	{
		CString sFileSavePath = sFolderPath + L"\\" + fileNames[i];
		m_pPrjData->DownloadFile(sDirPathInProject, fileNames[i], sFileSavePath, UIFileUpCBFunc);
	}
}



void CProjectManagementDlg::OnBnClickedButtonDeleteall()
{
	for(int i = 1; i < m_PjtManagementGridCtrl.GetRowCount(); i++)
	{
		CGridCellBase* pCell = m_PjtManagementGridCtrl.GetCell(i, 0);
		if(((CGridCellCheck* )pCell)->GetCheck())
		{
			CString sCheckedFileName = m_PjtManagementGridCtrl.GetItemText(i, 1);
			CString sCheckedParentPath = m_pPrjData->GetDirString(L"", m_selectedDir);
			m_pPrjData->DeleteFile(sCheckedFileName, sCheckedParentPath);
		}
	}
	FillPjtGridCtrl(m_selectedDir);
}

CProjectManagementDlg* g_projectManagementDlg = NULL;

BOOL CloseProjectManagementDlg()
{
	if (g_projectManagementDlg == NULL)
		return TRUE;
	BOOL ret = g_projectManagementDlg->DestroyWindow();
	if (ret)
	{
		delete g_projectManagementDlg;
		g_projectManagementDlg = NULL;
	}
	return ret;
}
