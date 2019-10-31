#include "StdAfx.h"
#include < afxinet.h>
#include "ProjectInfo.h"
#include "FileUploadDownload.h"
#include "..\Common\ComFun_String.h"
#include "..\WebIO\WebProjectFile.h"


CProjectInfo::CProjectInfo()
{
}

CProjectInfo::~CProjectInfo()
{
}

//////////////////////////////////////////////////////////////////////////

CProjectData::CProjectData()
{
}

CProjectData::~CProjectData()
{
}

CString CProjectData::GenerateGuid()
{
	GUID guid;
	CoCreateGuid(&guid);

	CString sGuid;
	sGuid.Format(_T("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"),
		guid.Data1, guid.Data2,
		guid.Data3, guid.Data4[0],
		guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4],
		guid.Data4[5], guid.Data4[6],
		guid.Data4[7]);
	return sGuid;
}


//目录层级用\分割，例如：A区\施工图 表示A区文件夹下的施工图文件夹
CProjectDir* CProjectData::FindDir(CString p_dirName)
{
	p_dirName.Trim(_T('\\'));

	int nPos1 = 0;
	int npos2 = 0; 
	
	CProjectDir* pParentDir = &m_rootDir; //父目录节点

	do 
	{
		npos2 = p_dirName.Find(_T('\\'), npos2);
		if (npos2<0)
		{
			break;
		}
		else
		{
			CString sFolderName = p_dirName.Mid(nPos1, npos2 - nPos1);
			pParentDir = pParentDir->GetSubFolder(sFolderName);
			if (pParentDir==NULL)
			{
				return NULL;
			}
			nPos1 = npos2 + 1;
			npos2++;
		}
	} while (npos2>0);


	CString lastFolderName = p_dirName.Mid(nPos1);
	return pParentDir->GetSubFolder(lastFolderName);	
}

bool CProjectData::AddFile(CString p_sFilePath, CString  p_sDirPathInProject, FileUpDownCB p_cbFunc) //p_sDirPathInProject是指上传到哪个目录下,目录层级用\分割，例如：A区\施工图 表示A区文件夹下的施工图文件夹
{
	//1上传文件到ftp中
	CString sFileName = FilePathToFileName(p_sFilePath);

	int nPos = sFileName.ReverseFind(_T('.'));
	if (nPos<0)
		return false;
	CString sExtensionName = sFileName.Mid(nPos);

	CString sSaveName = GenerateGuid() + sExtensionName; //实际存储的名字

	//以日期为文件夹名
	CString sDir;
	CTime nowtime = CTime::GetCurrentTime();
	sDir.Format(_T("%d%2d%2d"), nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay());

#if 0
	CFileUpDownLoad::UploadFile(p_sFilePath, sSaveName, sDir); 

	//////////////////////////////////////////////////////////////////////////
	//2.加到本地的项目文件中
	CProjectDir* p_pParentDir = FindDir(p_sDirPathInProject);	//找到目录
	if (p_pParentDir == NULL)
		return false;

	CProjectFile prjfile;
	prjfile.m_sName = sFileName;
	prjfile.m_sSaveName = sSaveName;

	p_pParentDir->AddFile(prjfile);	

	//3.调用web接口添加到web数据库中
	CWebProjectFile::Instance()->UpdateFile(GetProjectId(), p_sDirPathInProject, sSaveName, sFileName);

#else  //使用多线程
	CUpDownFilePara upFilePara;
	upFilePara.bUpload = true;
	upFilePara.sFileLocalPath = p_sFilePath; //本地文件名
	upFilePara.sFileName = FilePathToFileName(p_sFilePath);		//包含后缀
	upFilePara.sDirInProject = p_sDirPathInProject; //在项目中的目录
	//upFilePara.sFileUrl;
	upFilePara.ftpSaveName = sSaveName;
	upFilePara.ftpDir = sDir;

	upFilePara.cbFunc = CProjectData::FileUpCBFunc; //回调函数
	upFilePara.uiCBFunc = p_cbFunc;
	upFilePara.progress = 0; //进度，0-100
	upFilePara.pCaller = this; //调用者

	CFileUpDownLoad::Instance()->UploadFileByThread(upFilePara);
#endif

	return true;
}

void CProjectData::FileUpCBFunc(CUpDownFilePara* p_upFilePara)
{
	if (p_upFilePara->progress<100) //上传完成了才更新web端数据
	{
		return; 
	}

	CProjectData* pProjectData = (CProjectData*)(p_upFilePara->pCaller);
	if (pProjectData==NULL)
	{
		assert(false);
		return;
	}

	//2.加到本地的项目文件中
	CProjectDir* p_pParentDir = pProjectData->FindDir(p_upFilePara->sDirInProject);	//找到目录
	if (p_pParentDir == NULL)
	{
		assert(false);
		return;
	}

	CProjectFile prjfile;
	prjfile.m_sName = p_upFilePara->sFileName;
	prjfile.m_sSaveName = p_upFilePara->ftpDir + _T("\\") + p_upFilePara->ftpSaveName;
	p_pParentDir->AddFile(prjfile);

	//3.调用web接口添加到web数据库中
	CWebProjectFile::Instance()->UpdateFile(pProjectData->GetProjectId(), p_upFilePara->sDirInProject, prjfile.m_sSaveName, prjfile.m_sName);
}

CString CProjectData::GetProjectId()const
{
	CString sId;
	sId.Format(_T("%d"), m_prjInfo.m_id);
	return sId;
}
bool CProjectData::DeleteFile(CString p_sFileName, CString  p_sDirPathInProject)
{
	//找到目录
	CProjectDir* pDir = FindDir(p_sDirPathInProject);
	if (pDir == NULL)
		return false;

	pDir->DeleteFile(p_sFileName);

	//web端更新 
	CWebProjectFile::Instance()->DeleteFile(GetProjectId(), p_sDirPathInProject, p_sFileName);

	return true;
}
bool CProjectData::AddFolder(CString  p_sDirPathInProject, CString p_sFolderName)
{
	//找到目录
	CProjectDir* pDir = NULL;
	if (p_sDirPathInProject.IsEmpty() == false)
	{
		pDir = FindDir(p_sDirPathInProject);
	}
	else
	{
		pDir = &m_rootDir;
	}

	if (pDir == NULL)
		return false;

	pDir->AddFolder(p_sFolderName);

	return true;
}
bool CProjectData::DeleteFolder(CString  p_sFolderPath)
{
	//找到目录
	CProjectDir* pDir = FindDir(p_sFolderPath);
	if (pDir == NULL)
		return false;

	if (pDir->m_parent==NULL)
	{
		assert(false);
		return false;
	}

	bool bSuc = pDir->m_parent->DeleteSubFolder(pDir->m_sName);
	if (bSuc==false)
	{
		return false;
	}

	//web端更新 
	CWebProjectFile::Instance()->DeleteFolder(GetProjectId(), p_sFolderPath);

	return bSuc; 
}

bool CProjectData::RenameFolder(CString p_sFolderPath, CString p_newName)
{
	//找到目录
	CProjectDir* pDir = FindDir(p_sFolderPath);
	if (pDir == NULL)
		return false;

	pDir->Rename(p_newName);

	//web端更新 
	CWebProjectFile::Instance()->RenameFolder(GetProjectId(), p_sFolderPath, p_newName);

	return true; 
}

CString CProjectData::GetDirString(CString sName, CProjectDir* p_parentDir)
{
	CString temp = sName;
	CProjectDir* tempDir = p_parentDir;
	while (tempDir != &m_rootDir)
	{
		CString sDirString = tempDir->m_sName;
		temp = sDirString + L"\\" + temp;
		tempDir = tempDir->m_parent;
	}
	return temp;
}