// ProjectManagementDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProjectManagementDlg.h"
#include "afxdialogex.h"
#include "..\ProjectorFileMrg\ProjectFileMrg.h"


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
	DDX_Control(pDX, IDC_STATIC_ROOTNAME, m_StcRootName);
}


BEGIN_MESSAGE_MAP(CProjectManagementDlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CProjectManagementDlg::OnBnClickedButtonUpload)
END_MESSAGE_MAP()

void CProjectManagementDlg::FillPjtMngTreeCtrl()
{
	HICON Icon[3];
	CImageList *ImageList = new CImageList();
	ImageList->Create(16,16,ILC_COLOR24,12,12);
	Icon[0] = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	Icon[1] = AfxGetApp()->LoadIcon(IDI_ICON_DOCUMENT);

	for (int i = 0; i < 2; i++)
	{
		ImageList->Add(Icon[i]);
	}
	m_TreePrjDir.SetImageList(ImageList, TVSIL_NORMAL);

	//vector <HTREEITEM> Areas;
	for (UINT i = 0; i < m_pPrjData->m_rootDir.m_subDirs.size(); i++)
	{
		HTREEITEM hFolder = m_TreePrjDir.InsertItem(m_pPrjData->m_rootDir.m_subDirs[i]->m_sName, 0, 0);
		for (UINT j = 0; j < m_pPrjData->m_rootDir.m_subDirs[i]->m_subDirs.size(); j++)
		{
			m_TreePrjDir.InsertItem(m_pPrjData->m_rootDir.m_subDirs[i]->m_subDirs[j]->m_sName, 1, 1, hFolder);
		}
	}
}

// CProjectManagementDlg 消息处理程序


void CProjectManagementDlg::OnBnClickedButtonUpload()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL CProjectManagementDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	CFont Font;
	Font.CreatePointFont(160, L"");
	m_StcRootName.SetFont(&Font);
	Font.Detach();

	FillPjtMngTreeCtrl();

	m_StcPrjName.SetWindowTextW(m_pPrjData->m_prjInfo.m_sName);
	m_StcArea.SetWindowTextW(m_pPrjData->m_prjInfo.m_sArea);
	m_StcCompany.SetWindowTextW(m_pPrjData->m_prjInfo.m_sCityCompany);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
