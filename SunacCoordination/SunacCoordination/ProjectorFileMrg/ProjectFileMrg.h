#pragma once
#include <vector>
#include "ProjectFile.h"
#include "ProjectInfo.h"


//项目管理类，管理所有项目
class CProjectFileMrg
{
	CProjectFileMrg();
public:
	static CProjectFileMrg* Instance();
	~CProjectFileMrg();

	CProjectData* GetProject(CString p_sPrjName);
	vector<CProjectData*> GetProjectsByArea(CString p_sArea);

protected:
	void Clear();

protected:
	vector<CProjectData*> m_projects;
};

