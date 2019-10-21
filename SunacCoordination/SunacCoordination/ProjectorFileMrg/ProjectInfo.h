#pragma once
#include <vector>
#include "ProjectFile.h"


class CProjectInfo
{
public:
	CProjectInfo();
	~CProjectInfo();

protected:
	CString m_sName; //项目名称
	CString m_sArea; //区域
	CString m_sCityCompany;//城市公司
};

//////////////////////////////////////////////////////////////////////////
class CProjectData : public CProjectorDir
{
public:
	CProjectData();
	~CProjectData();

	static bool DownloadFile(CProjectFile p_prjFile);
	static bool UploadFile(CString p_sFileName, CString p_saveName);

	bool UploadFile(CString p_sFileName, CString p_saveName, CProjectorDir*  p_sParentDir);
		
protected:
	CProjectInfo m_prjInfo;
};

