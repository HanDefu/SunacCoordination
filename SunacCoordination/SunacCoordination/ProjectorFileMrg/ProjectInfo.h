#pragma once
#include <vector>
#include "ProjectFile.h"

//项目基本信息
class CProjectInfo
{
public:
	CProjectInfo();
	~CProjectInfo();

public:
	int m_id;
	CString m_sName; //项目名称
	CString m_sArea; //区域
	CString m_sCityCompany;//城市公司
};

class CProjectFileMrg;
class CWebProjectFile;
//////////////////////////////////////////////////////////////////////////
class CProjectData //项目的数据
{
	friend CProjectFileMrg;
	friend CWebProjectFile;
public:
	CProjectData();
	~CProjectData();

	CString GetDirString(CString sName, CProjectDir* p_parentDir);

	static CString GenerateGuid();

	//以下目录参数需输入完整的目录
	bool AddFile(CString p_sFilePath, CString  p_sParentFolderPath); //p_sParentDirPath是指上传到哪个目录下
	//bool AddFile(CString p_sFilePath, CProjectDir* p_pParentDir); //p_pParentDir是指上传到哪个目录下
	bool DeleteFile(CString p_sFileName, CString  p_sParentFolderPath);

	bool AddFolder(CString  p_sParentFolderPath, CString p_sFolderName);
	bool DeleteFolder(CString  p_sFolderPath);
	bool RenameFolder(CString p_sFolderPath, CString p_newName);

	//目录层级用\分割，例如：A区\施工图 表示A区文件夹下的施工图文件夹
	CProjectDir* FindDir(CString p_dirName);

	const CProjectDir* GetRootDir() { return &m_rootDir; }
	CProjectInfo GetPrjInfo()const { return m_prjInfo; }
	CString GetProjectId()const;

protected:
	CProjectInfo m_prjInfo;
	CProjectDir m_rootDir;	
};

