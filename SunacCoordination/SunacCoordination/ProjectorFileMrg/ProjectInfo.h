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


//////////////////////////////////////////////////////////////////////////
class CProjectData //项目的数据
{
public:
	CProjectData();
	~CProjectData();

	static CString GenerateGuid();
	static bool DownloadFile(const CString& strFileURLInServer, const CString & strFileLocalFullPath);
	static bool DownloadFile(CProjectFile p_prjFile);
	static bool UploadFile(CString p_sFileName, CString p_saveName);

	bool UploadFile(CString p_sFileName, CString p_saveName, CString  p_sParentDir); //p_sParentDir 为完整的父目录
	bool UploadFile(CString p_sFileName, CString p_saveName, CProjectDir*  p_parentDir);


	bool AddFile(CString p_sFileName, CString  p_sParentDir); //p_sParentDir是指上传到哪个目录下
	void DeleteFile();

public:
	CProjectInfo m_prjInfo;
	CProjectDir m_rootDir;	
};

