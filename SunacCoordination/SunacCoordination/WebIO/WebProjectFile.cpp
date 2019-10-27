#include "StdAfx.h"
#include "WebProjectFile.h"

CWebProjectFile* CWebProjectFile::Instance()
{
	static CWebProjectFile instance;
	return &instance;
}

CWebProjectFile::CWebProjectFile()
{
}

CWebProjectFile::~CWebProjectFile()
{
}

//获取当前用户下的项目
bool CWebProjectFile::GetAllProjectInfo()
{

	//GetProjectInfo()


	CProjectFileMrg::Instance()->Clear();


	return true;
}

bool CWebProjectFile::UpdateFile(CString p_prjectId, CString p_dir, CString p_fileSaveName, CString p_fileName)
{
	return true;
}
bool CWebProjectFile::DeleteFile(CString p_prjectId, CString p_dir, CString p_fileName)
{
	return true;
}

bool CWebProjectFile::NewFolder(CString p_prjectId, CString p_dir)
{
	return true;
}
bool CWebProjectFile::RenameFolder(CString p_prjectId, CString p_dir, CString p_sNewDirName)
{
	return true;
}
bool CWebProjectFile::DeleteFolder(CString p_prjectId, CString p_dir)
{
	return true;
}