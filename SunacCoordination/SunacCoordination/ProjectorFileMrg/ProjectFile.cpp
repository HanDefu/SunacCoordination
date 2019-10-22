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
}


bool CProjectorDir::FindFile(CString p_fileName, CProjectFile &p_fileOut)
{
	for (UINT i = 0; i < m_subFiles.size(); i++)
	{
		if (m_subFiles[i].m_sName.CompareNoCase(p_fileName)==0)
		{
			p_fileOut =(m_subFiles[i]);
			return true;
		}
	}

	return false;
}

bool CProjectorDir::AddFile(CProjectFile p_file)
{
	//首先看是否存在
	for (UINT i = 0; i < m_subFiles.size(); i++)
	{
		if (m_subFiles[i].m_sName.CompareNoCase(p_file.m_sName) == 0)
		{
			m_subFiles[i] = p_file;
			return true;
		}
	}

	p_file.m_parent = this;
	m_subFiles.push_back(p_file);
	return true;
}