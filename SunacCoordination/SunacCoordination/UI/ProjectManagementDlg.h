#pragma once

#include "..\ProjectorFileMrg\ProjectInfo.h"
#include "../UI/GridCtrl_src/NewCellTypes/GridCellCheck.h"
#include "GridCtrlEx.h"
#include "../Common/ThreadUtil.h"


// CProjectManagementDlg 对话框

class CProjectManagementDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CProjectManagementDlg)

public:
	CProjectManagementDlg(CProjectData* pPrjData, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProjectManagementDlg();
	void FillPjtMngTreeCtrl(); //填充树控件
	void InitGridCtrl(); //初始化GridCtrl
	void FillPjtGridCtrl(CProjectDir* SelectedDir); //填充GridCtrl
	CProjectDir* FindClkDir(HTREEITEM CurClkItem); //通过递归找到树控件对应的文件夹位置

// 对话框数据
	enum { IDD = IDD_DIALOG_PROJECTMANAGEMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
public:
	CProjectData* m_pPrjData;
	CProjectDir* m_selectedDir;
	int m_nClkRow;//当前点击的单元格的行数
	int m_nClkCol;//当前点击的单元格的列数
	vector<CGridCellCheck*> m_vCheckBox;//选中的checkbox
	CCritSec m_Lock;

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

	afx_msg void OnGridClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonUpload();
	virtual BOOL OnInitDialog();
	CStatic m_StcRootName;
	afx_msg void OnNMClickTreePrjdir(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonNewdir();
	afx_msg void OnBnClickedButtonDeletedir();
	afx_msg void OnBnClickedButtonDownloadall();
	afx_msg void OnBnClickedButtonDeleteall();
};

extern CProjectManagementDlg* g_projectManagementDlg;
BOOL CloseProjectManagementDlg();
