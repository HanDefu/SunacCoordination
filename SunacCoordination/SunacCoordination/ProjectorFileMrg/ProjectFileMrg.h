#pragma once
#include <vector>
#include "ProjectFile.h"
#include "ProjectInfo.h"



class CProjectFileMrg : public CProjectorDir
{
public:
	CProjectFileMrg();
	~CProjectFileMrg();

	CProjectInfo& GetProject(CString p_sPrjName);

protected:
	vector<CProjectInfo> m_projects;
};

