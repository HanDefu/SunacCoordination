#include "StdAfx.h"
#include < afxinet.h>
#include "ProjectInfo.h"
#include "FileUploadDownload.h"
#include "..\Common\ComFun_String.h"
#include "..\WebIO\WebProjectFile.h"
#include "..\WebIO\WebIO.h"


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
CProjectDir* CProjectData::FindDir(CString p_dirPath)
{
	CAutoLock a(&m_Lock);
	p_dirPath.Trim(_T('\\'));

	int nPos1 = 0;
	int npos2 = 0; 
	
	CProjectDir* pParentDir = &m_rootDir; //父目录节点

	do 
	{
		npos2 = p_dirPath.Find(_T('\\'), npos2);
		if (npos2<0)
		{
			break;
		}
		else
		{
			CString sFolderName = p_dirPath.Mid(nPos1, npos2 - nPos1);
			pParentDir = pParentDir->GetSubFolder(sFolderName);
			if (pParentDir==NULL)
			{
				return NULL;
			}
			nPos1 = npos2 + 1;
			npos2++;
		}
	} while (npos2>0);


	CString lastFolderName = p_dirPath.Mid(nPos1);
	return pParentDir->GetSubFolder(lastFolderName);	
}
CProjectFile* CProjectData::GetFileByDirAndName(CString p_sDirPathInProject, CString p_fileName)
{
	CAutoLock a(&m_Lock);
	CProjectDir* pDir = FindDir(p_sDirPathInProject);
	if (pDir == NULL)
	{
		return NULL;
	}
	else
	{
		return pDir->FindFile(p_fileName);
	}
}

bool CProjectData::DownloadFile(CString p_sDirPathInProject, CString p_fileName, CString p_sFileLocalPath, FileUpDownCB p_cbFunc)
{
	CProjectFile* pFile = GetFileByDirAndName(p_sDirPathInProject, p_fileName);
	if (pFile==NULL)
	{
		return false;
	}

	pFile->m_fileState = E_ProjectFile_Downloading;

	CUpDownFilePara downFilePara;
	downFilePara.bUpload = false;
	downFilePara.sFileLocalPath = p_sFileLocalPath; //本地文件名
	downFilePara.sFileName = FilePathToFileName(p_fileName);		//包含后缀
	downFilePara.sDirInProject = p_sDirPathInProject; //在项目中的目录

	downFilePara.sFileUrl = pFile->m_sFileUrl;
	downFilePara.ftpSaveName = pFile->m_sSaveName;

	downFilePara.cbFunc = CProjectData::FileDownCBFunc; //回调函数
	downFilePara.uiCBFunc = p_cbFunc;
	downFilePara.progress = 0; //进度，0-100
	downFilePara.pCaller = this; //调用者

	CFileUpDownLoadWeb::Instance()->DownloadFileByThread(downFilePara);

	return true;
}

bool CProjectData::AddFile(CString p_sFilePath, CString  p_sDirPathInProject, FileUpDownCB p_cbFunc, long long n_fileSize) //p_sDirPathInProject是指上传到哪个目录下,目录层级用\分割，例如：A区\施工图 表示A区文件夹下的施工图文件夹
{
	//1上传文件到ftp中
	CString sFileName = FilePathToFileName(p_sFilePath);

	int nPos = sFileName.ReverseFind(_T('.'));
	if (nPos<0)
		return false;
	CString sExtensionName = sFileName.Mid(nPos);

	CString sSaveName = GenerateGuid() + sExtensionName; //实际存储的名字

	//1.加到本地的项目文件中
	CProjectDir* p_pParentDir = FindDir(p_sDirPathInProject);	//找到目录
	if (p_pParentDir == NULL)
		return false;

	CTime nowtime = CTime::GetCurrentTime();
	CString sCreateTime;
	sCreateTime.Format(_T("%d/%02d/%02d %02d:%02d:%02d"), nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(), nowtime.GetHour(), nowtime.GetMinute(), nowtime.GetSecond());

	CProjectFile prjfile;
	prjfile.m_sCreator = WebIO::GetInstance()->GetUserName();
	prjfile.m_sName = sFileName;
	prjfile.m_sSaveName = sSaveName;
	prjfile.m_sCreateTime = sCreateTime;
	prjfile.m_sFileSize = FileSizeToString(n_fileSize);
	prjfile.m_sUpdator = WebIO::GetInstance()->GetUserName();
	prjfile.m_fileState = E_ProjectFile_Uploading;
	p_pParentDir->AddFile(prjfile);	

	//////////////////////////////////////////////////////////////////////////
	//以日期为文件夹名
	CString sDir;
	sDir.Format(_T("%d%02d%02d"), nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay());

#if 0
	//2. 上传
	CFileUpDownLoad::UploadFile(p_sFilePath, sSaveName, sDir); 

	//////////////////////////////////////////////////////////////////////////

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

	CFileUpDownLoadFtp::Instance()->UploadFileByThread(upFilePara);
#endif

	return true;
}

void CProjectData::FileDownCBFunc(CUpDownFilePara* p_upFilePara)
{
	if (p_upFilePara->progress > 0 && p_upFilePara->progress<100) //上传完成了才更新web端数据
	{
		return;
	}
	CProjectData* pProjectData = (CProjectData*)(p_upFilePara->pCaller);
	if (pProjectData == NULL)
	{
		assert(false);
		return;
	}

	CAutoLock a(&pProjectData->m_Lock);
	CProjectFile* pPrjfile = pProjectData->GetFileByDirAndName(p_upFilePara->sDirInProject, p_upFilePara->sFileName);
	if (pPrjfile!=NULL)
	{
		bool bLoadFail = p_upFilePara->progress < 0;
		pPrjfile->m_fileState = bLoadFail ? E_ProjectFile_DownloadFailed : E_ProjectFile_DownloadSuccess;
	}
}

void CProjectData::FileUpCBFunc(CUpDownFilePara* p_upFilePara)
{
	if (p_upFilePara->progress>0 && p_upFilePara->progress<100) //上传完成了才更新web端数据
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

	bool bLoadFail = p_upFilePara->progress < 0;
	if (bLoadFail)
	{
		p_pParentDir->DeleteFile(p_upFilePara->sFileName);
		return;
	}
	else
	{
		CProjectFile* pPrjfile = p_pParentDir->FindFile(p_upFilePara->sFileName);
		if (pPrjfile == NULL)
		{
			assert(false);
			return;
		}

		pPrjfile->m_fileState = E_ProjectFile_UploadSuccess;

		//3.调用web接口添加到web数据库中
		CWebProjectFile::Instance()->UpdateFile(pProjectData->GetProjectId(), p_upFilePara->sDirInProject, pPrjfile->m_sSaveName, pPrjfile->m_sName);
	}
}

CString CProjectData::GetProjectId()const
{
	CString sId;
	sId.Format(_T("%d"), m_prjInfo.m_id);
	return sId;
}
bool CProjectData::DeleteFile(CString p_sFileName, CString  p_sDirPathInProject)
{
	//web端更新 
	CWebProjectFile::Instance()->DeleteFile(GetProjectId(), p_sDirPathInProject, p_sFileName);

	//找到目录
	CProjectDir* pDir = FindDir(p_sDirPathInProject);
	if (pDir == NULL)
		return false;

	pDir->DeleteFile(p_sFileName);


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


	CString sFullPath = p_sDirPathInProject + p_sFolderName;

	CWebProjectFile::Instance()->NewFolder(GetProjectId(), sFullPath);

	return true;
}
bool CProjectData::DeleteFolder(CString  p_sFolderPath)
{
	
	//web端更新 
	CWebProjectFile::Instance()->DeleteFolder(GetProjectId(), p_sFolderPath);
	
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

