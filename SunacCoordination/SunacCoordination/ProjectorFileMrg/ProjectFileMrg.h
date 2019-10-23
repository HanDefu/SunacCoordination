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

	void InitTestData();

	CProjectData* GetProject(CString p_sPrjName);
	vector<CProjectData*> GetProjectsByArea(CString p_sArea); //查询区域下有哪些项目

	vector<CString> GetAllAreas()const;

protected:
	void Clear();

public:
	vector<CProjectData*> m_projects;
};

