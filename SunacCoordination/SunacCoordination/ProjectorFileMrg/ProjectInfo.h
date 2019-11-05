#pragma once
#include <vector>
#include "ProjectFile.h"
#include "FileUploadDownload.h"

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

	//以下函数p_sDirPathInProject表示完整的在项目中的目录,使用\分割
	bool AddFile(CString p_sFilePath, CString  p_sDirPathInProject, FileUpDownCB p_cbFunc, long long n_fileSize); //p_sParentDirPath是指上传到哪个目录下
	bool DeleteFile(CString p_sFileName, CString  p_sDirPathInProject);
	bool DownloadFile(CString p_sDirPathInProject, CString p_fileName, CString p_sFileLocalPath, FileUpDownCB p_cbFunc);

	bool AddFolder(CString  p_sDirPathInProject, CString p_sFolderName);  //p_sFolderName为文件分类名，非完整路径
	bool DeleteFolder(CString  p_sFolderPath);
	bool RenameFolder(CString p_sFolderPath, CString p_newName);

	//目录层级用\分割，例如：A区\施工图 表示A区文件夹下的施工图文件夹
	CProjectDir* FindDir(CString p_dirPath);
	CProjectFile* GetFileByDirAndName(CString p_sDirPathInProject, CString p_fileName);

	const CProjectDir* GetRootDir() { return &m_rootDir; }
	CProjectInfo GetPrjInfo()const { return m_prjInfo; }
	CString GetProjectId()const;


protected:
	static void FileUpCBFunc(CUpDownFilePara*);
	static void FileDownCBFunc(CUpDownFilePara*);

protected:
	CProjectInfo m_prjInfo;
	CProjectDir m_rootDir;	
};

