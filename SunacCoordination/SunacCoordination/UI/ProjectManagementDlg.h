#pragma once

#include "ProjectorFileMrg\ProjectInfo.h"
#include "GridCtrlEx.h"


// CProjectManagementDlg 对话框

class CProjectManagementDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CProjectManagementDlg)

public:
	CProjectManagementDlg(CProjectData* pPrjData, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProjectManagementDlg();
	void FillPjtMngTreeCtrl();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROJECTMANAGEMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProjectData* m_pPrjData;

	CButton m_BtnDeleteAll;
	CButton m_BtnDownloadAll;
	CButton m_BtnUpload;
	CStatic m_StcArea;
	CStatic m_StcCompany;
	CStatic m_StcDocName;
	CStatic m_StcDocSize;
	CStatic m_StcPrjName;
	CStatic m_StcUpdaterName;
	CStatic m_StcUpdateTime;
	CStatic m_StcUploaderName;
	CStatic m_StcUploadTime;
	CTreeCtrl m_TreePrjDir;
	CGridCtrl m_PjtManagementGridCtrl;
	afx_msg void OnBnClickedButtonUpload();
	virtual BOOL OnInitDialog();
	CStatic m_StcRootName;
};
