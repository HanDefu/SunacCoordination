#pragma once
#include <vector>

using namespace std;

class CProjectorDir;

class CPrjNode
{
public:
	CPrjNode();
	virtual ~CPrjNode();

	virtual bool IsDirectory()const = 0;

public:
	int m_id;	//ID主要用于从web后台操作处理用
	CString m_sName; //文件名或文件夹名
	CProjectorDir* m_parent;  //父文件夹
};

//////////////////////////////////////////////////////////////////////////
class CProjectFile :public CPrjNode
{
public:
	CProjectFile();
	~CProjectFile();
	bool IsDirectory()const { return false; }

public:
	CString m_sSaveName; //实际存储从名称，通常为GUID
	CString m_sFileUrl;  //文件下载地址

	CString m_sCreator;		//创建者
	CString m_sCreateTime;  //创建时间
	CString m_sUpdator;		//更新人员
	CString m_sUpdateTime;	//更新时间
	CString m_sFileSize;	//文件大小
};

//////////////////////////////////////////////////////////////////////////
class CProjectorDir :public CPrjNode
{
public:
	CProjectorDir();
	~CProjectorDir();
	bool IsDirectory()const { return true; }

	bool AddFile(CProjectFile p_file);
	bool FindFile(CString p_fileName, CProjectFile &p_fileOut);
	bool DeleteFile(CString p_fileName);

	bool AddFolder(CString sFolderName);
	bool DeleteFolder(CString p_folder);

	void Rename(CString newName);

protected:
	vector<CProjectFile> m_subFiles; //子文件
	vector<CProjectorDir> m_subDirs;//子文件夹
};



