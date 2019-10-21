#include "stdafx.h"
#include "ProjectFile.h"

CPrjNode::CPrjNode()
{
	m_id = -1;
	m_parent = NULL;
}

CPrjNode::~CPrjNode()
{
}

//////////////////////////////////////////////////////////////////////////
CProjectFile::CProjectFile()
{
}

CProjectFile::~CProjectFile()
{
}


//////////////////////////////////////////////////////////////////////////
CProjectorDir::CProjectorDir()
{
}

CProjectorDir::~CProjectorDir()
{
	for (UINT i = 0; i < m_subDirs.size(); i++)
	{
		delete m_subDirs[i];
	}
	m_subDirs.clear();
}