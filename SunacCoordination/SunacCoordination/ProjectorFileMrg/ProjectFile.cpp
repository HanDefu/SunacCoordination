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
	m_fileState = E_ProjectFile_Normal;
}

CProjectFile::~CProjectFile()
{
}

CString CProjectFile::FileSizeToString(long long p_size)
{
	long long kSize = p_size >> 10; //转为k
	long long mSize = p_size >> 20; //转为M
	long long gSize = mSize >> 10; //转为G

	CString str;
	if (gSize>0)
	{
		str.Format(_T("%dM"), (int)mSize);
	}
	else if (mSize>0)
	{
		str.Format(_T("%.3fM"), ((double)kSize)/1000);
	}
	else
	{
		str.Format(_T("%dK"), (int)kSize);
	}

	return str;
}

CString CProjectFile::GetFileSize()
{
	CString sFileSize = this->FileSizeToString(m_fileSize);
	return sFileSize;
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

bool CProjectDir::FindFile(CString p_fileName, CProjectFile &p_fileOut) const
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

CProjectFile* CProjectDir::FindFile(CString p_fileName)
{
	for (UINT i = 0; i < m_subFiles.size(); i++)
	{
		if (m_subFiles[i].m_sName.CompareNoCase(p_fileName) == 0)
		{
			return &m_subFiles[i];
		}
	}

	return NULL;
}
bool CProjectDir::DeleteFile(CString p_fileName)
{
	for (vector<CProjectFile>::iterator it = m_subFiles.begin(); it < m_subFiles.end(); it++)
	{
		if ((*it).m_sName.CompareNoCase(p_fileName) == 0)
		{
			m_subFiles.erase(it);
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

CProjectDir* CProjectDir::GetSubFolder(CString p_sFolderName)
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
const CProjectDir* CProjectDir::GetSubFolderConst(CString p_sFolderName)
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

bool CProjectDir::DeleteSubFolder(CString p_sFolderName)
{
	for (vector<CProjectDir*>::iterator it = m_subDirs.begin(); it < m_subDirs.end(); it++)
	{
		if ((*it)->m_sName.CompareNoCase(p_sFolderName) == 0)
		{
			delete (*it);			
			m_subDirs.erase(it);
			return true;
		}
	}
	return false;
}
void CProjectDir::Rename(CString newName)
{
	m_sName = newName;
}

CString CProjectDir::GetDirFullPath()const
{
	CString sDirPath = m_sName;

	const CProjectDir* pParent = m_parent;
	while (pParent!=NULL)
	{
		if (pParent->m_sName.IsEmpty())
			break;//根节点

		sDirPath = pParent->m_sName + _T("\\") + sDirPath;
	}

	return sDirPath;
}

bool CProjectDir::AddFolder(CProjectDir* p_dir)
{
	if (p_dir==NULL)
	{
		return false;
	}

	p_dir->m_parent = this;
	m_subDirs.push_back(p_dir);
	return true;
}

CString EProjectFileStateToCString(EProjectFileState state)
{
	switch(state)
	{
	case E_ProjectFile_Normal:
		return L"准备就绪";
		break;
	case E_ProjectFile_Downloading:
		return L"下载中...";
		break;
	case E_ProjectFile_DownloadSuccess:
		return L"下载成功！";
		break;
	case E_ProjectFile_DownloadFailed:
		return L"下载失败";
		break;
	case E_ProjectFile_Uploading:
		return L"上传中...";
		break;
	case E_ProjectFile_UploadSuccess:
		return L"上传成功！";
		break;
	case E_ProjectFile_UploadFailed:
		return L"上传失败";
		break;
	default: return L"";
	}
}
