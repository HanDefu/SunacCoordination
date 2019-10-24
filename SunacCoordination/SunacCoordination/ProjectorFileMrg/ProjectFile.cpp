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
CProjectDir::CProjectDir()
{
}

CProjectDir::~CProjectDir()
{
	for (UINT i = 0; i < m_subDirs.size(); i++)
	{
		delete m_subDirs[i];
	}

	m_subDirs.clear();
}


bool CProjectDir::FindFile(CString p_fileName, CProjectFile &p_fileOut)
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

bool CProjectDir::AddFile(CProjectFile p_file)
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

CProjectDir* CProjectDir::AddFolder(CString p_sFolderName)
{
	//首先看是否存在
	for (UINT i = 0; i < m_subDirs.size(); i++)
	{
		if (m_subDirs[i]->m_sName.CompareNoCase(p_sFolderName) == 0)
		{
			return (m_subDirs[i]);
		}
	}

	CProjectDir* fileDir = new CProjectDir();
	fileDir->m_sName = p_sFolderName;
	fileDir->m_parent = this;

	m_subDirs.push_back(fileDir);
	return fileDir;
}

CProjectDir* CProjectDir::GetFolder(CString p_sFolderName)
{
	for (UINT i = 0; i < m_subDirs.size(); i++)
	{
		if (m_subDirs[i]->m_sName.CompareNoCase(p_sFolderName) == 0)
		{
			return m_subDirs[i];
		}
	}

	return NULL;
}