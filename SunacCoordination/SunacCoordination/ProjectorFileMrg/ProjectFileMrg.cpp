#include "StdAfx.h"
#include "ProjectFileMrg.h"

CProjectFileMrg* CProjectFileMrg::Instance()
{
	static CProjectFileMrg instance;
	return &instance;
}
CProjectFileMrg::CProjectFileMrg()
{
}

CProjectFileMrg::~CProjectFileMrg()
{
	Clear();
}

void CProjectFileMrg::Clear()
{
	for (UINT i = 0; i < m_projects.size(); i++)
	{
		delete m_projects[i];
	}

	m_projects.clear();
}

