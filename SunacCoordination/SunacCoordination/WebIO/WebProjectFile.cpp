#include "StdAfx.h"
#include "WebProjectFile.h"
#include "SunacCadWeb/soapStub.h"
#include "WebIO.h"
#include "../Common/ComFun_String.h"
#include "../UI/ProjectManagementDlg.h"

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
	CString cUID;
	cUID.Format(L"%d", WebIO::GetInstance()->GetUserID());
	std::wstring sUID;
	sUID = cUID;
	
	//GetProjectInfo()
	_ns1__GetProjectInfo ns;
	ns.UID = &sUID;
	_ns1__GetProjectInfoResponse nsResponse;
	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);
	int nRet = cadWeb.GetProjectInfo(&ns, nsResponse);

	if (nsResponse.GetProjectInfoResult == NULL)
	{
		return false;
	}

	ParseProjectInfoFromXML(nsResponse.GetProjectInfoResult);

	return true;
}

bool CWebProjectFile::UpdateFile(CString p_prjectId, CString p_dir, CString p_fileSaveName, CString p_fileName)
{
	CString cUID;
	std::wstring sUID, sFileSaveName, sUpdateFileName, sParentDir, sPrjID;
	

	cUID.Format(L"%d", WebIO::GetInstance()->GetUserID());
	p_dir = p_dir.Trim(L"\\");

	int ParentDirID;
	CProjectDir* TempDir;
	TempDir = g_projectManagementDlg->m_pPrjData->FindDir(p_dir);
	ParentDirID = TempDir->m_id;

	sUID = cUID;
	sUpdateFileName = p_fileName;
	sParentDir = p_dir;
	sPrjID = p_prjectId;
	sFileSaveName = p_fileSaveName;

	_ns1__UpdateCadDrawing ns;
	ns.UID = &sUID;
	ns.OID = &sPrjID;
	ns.DrawingDir = &sParentDir;
	ns.DrawingFile = &sUpdateFileName;
	ns.FileSaveName = &sFileSaveName;

	_ns1__UpdateCadDrawingResponse nsResponse; 
	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);
	int nRet = cadWeb.UpdateCadDrawing(&ns, nsResponse);

	if (nsResponse.UpdateCadDrawingResult == NULL)
	{
		return false;
	}

	CMarkup xml;	
	xml.SetDoc((*(nsResponse.UpdateCadDrawingResult)).c_str());
	ParseUpdateFileFromXML(xml);

	return true;
}
bool CWebProjectFile::DeleteFile(CString p_prjectId, CString p_dir, CString p_fileName)
{
	CString cUID;
	std::wstring sUID, sDeleteFileName, sParentDir, sPrjID;

	cUID.Format(L"%d", WebIO::GetInstance()->GetUserID());
	p_dir = p_dir.Trim(L"\\");

	int ParentDirID;
	CProjectDir* TempDir;
	TempDir = g_projectManagementDlg->m_pPrjData->FindDir(p_dir);
	ParentDirID = TempDir->m_id;

	sUID = cUID;
	sDeleteFileName = p_fileName;
	sParentDir = p_dir;
	sPrjID = p_prjectId;

	_ns1__DeleteCadDrawing ns;
	ns.UID = &sUID;
	ns.OID = &sPrjID;
	ns.DrawingDir = &sParentDir;
	ns.DrawingFile = &sDeleteFileName;

	_ns1__DeleteCadDrawingResponse nsResponse; 
	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);
	int nRet = cadWeb.DeleteCadDrawing(&ns, nsResponse);

	if (nsResponse.DeleteCadDrawingResult == NULL)
	{
		return false;
	}

	CMarkup xml;	
	xml.SetDoc((*(nsResponse.DeleteCadDrawingResult)).c_str());
	ParseDeleteFileFromXML(xml);

	return true;
}

bool CWebProjectFile::NewFolder(CString p_prjectId, CString p_sNewDirPath)
{
	CString cUID, cNewDirName, cParentDir;
	int ParentDirID;
	std::wstring sUID, sNewDirName, sParentDir, sPrjID;

	cUID.Format(L"%d", WebIO::GetInstance()->GetUserID());
	cNewDirName = FilePathToFileName(p_sNewDirPath.Trim(L"\\"));
	if (cNewDirName != p_sNewDirPath.Trim(L"\\"))//是否根目录
	{
		int nPos = p_sNewDirPath.Find(cNewDirName);
		cParentDir = p_sNewDirPath.Left(nPos - 1);
		CProjectDir* TempDir;
		TempDir = g_projectManagementDlg->m_pPrjData->FindDir(cParentDir);
		ParentDirID = TempDir->m_id;
	}
	else cParentDir = L"";


	sUID = cUID;
	sNewDirName = cNewDirName;
	sParentDir = cParentDir;
	sPrjID = p_prjectId;

	_ns1__NewCadFileDir ns;
	ns.UID = &sUID;
	ns.OID = &sPrjID;
	ns.DrawingDir = &sNewDirName;
	ns.ParentDir = &sParentDir;

	_ns1__NewCadFileDirResponse nsResponse; 
	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);
	int nRet = cadWeb.NewCadFileDir(&ns, nsResponse);

	if (nsResponse.NewCadFileDirResult == NULL)
	{
		return false;
	}

	CMarkup xml;	
	xml.SetDoc((*(nsResponse.NewCadFileDirResult)).c_str());
	ParseNewFileDirFromXML(xml);

	return true;
}
bool CWebProjectFile::RenameFolder(CString p_prjectId, CString p_sRenameDirPath, CString p_sNewDirName)
{
	CString cUID, cNewDirName, cOriginDirName, cParentPath;
	std::wstring sUID, sNewDirName, sOriginDirName, sPrjID, sParentPath;

	cOriginDirName = FilePathToFileName(p_sRenameDirPath.Trim(L"\\"));
	if (cOriginDirName != p_sRenameDirPath.Trim(L"\\"))//是否根目录
	{
		int nPos = p_sRenameDirPath.Find(cNewDirName);
		cParentPath = p_sRenameDirPath.Left(nPos - 1);
	}
	cUID.Format(L"%d", WebIO::GetInstance()->GetUserID());

	int ParentDirID;
	CProjectDir* TempDir;
	TempDir = g_projectManagementDlg->m_pPrjData->FindDir(cParentPath);
	ParentDirID = TempDir->m_id;

	sUID = cUID;
	sNewDirName = cNewDirName;
	sOriginDirName = cOriginDirName;
	sPrjID = p_prjectId;
	sParentPath = cParentPath;

	_ns1__RenameCadFileDir ns;
	ns.UID = &sUID;
	ns.OID = &sPrjID;
	ns.DrawingDir = &sOriginDirName;
	ns.NewDrawingDir = &sNewDirName;
	ns.ParentDir = &sParentPath;

	_ns1__RenameCadFileDirResponse nsResponse; 
	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);
	int nRet = cadWeb.RenameCadFileDir(&ns, nsResponse);

	if (nsResponse.RenameCadFileDirResult == NULL)
	{
		return false;
	}

	CMarkup xml;	
	xml.SetDoc((*(nsResponse.RenameCadFileDirResult)).c_str());
	ParseRenameFileDirFromXML(xml);

	return true;
}
bool CWebProjectFile::DeleteFolder(CString p_prjectId, CString p_dir)
{
	CString cUID, cDirName, cParentPath;
	std::wstring sUID, sNewDirName, sDirName, sPrjID, sParentPath;

	cUID.Format(L"%d", WebIO::GetInstance()->GetUserID());

	cDirName = FilePathToFileName(p_dir.Trim(L"\\"));
	if (cDirName != p_dir.Trim(L"\\"))//是否根目录
	{
		int nPos = p_dir.Find(cDirName);
		cParentPath = p_dir.Left(nPos - 1);
	}

	sUID = cUID;
	sDirName = cDirName;
	sPrjID = p_prjectId;
	sParentPath = cParentPath;

	_ns1__DeleteCadFileDir ns;
	ns.UID = &sUID;
	ns.OID = &sPrjID;
	ns.DrawingDir = &sDirName;
	ns.ParentDir = &sParentPath;

	_ns1__DeleteCadFileDirResponse nsResponse; 
	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);
	int nRet = cadWeb.DeleteCadFileDir(&ns, nsResponse);

	if (nsResponse.DeleteCadFileDirResult == NULL)
	{
		return false;
	}

	CMarkup xml;	
	xml.SetDoc((*(nsResponse.DeleteCadFileDirResult)).c_str());
	ParseDeleteDirFromXML(xml);

	return true;
}

void CWebProjectFile::ParseProjectInfoFromXML(std::wstring * p_str)
{
	if (p_str==NULL)
		return;

	CMarkup xml;
	xml.SetDoc(p_str->c_str());
	CProjectFileMrg::Instance()->Clear();

	xml.ResetMainPos();
	xml.FindElem();	//根节点

	xml.IntoElem();
	{
		if (xml.FindElem(_T("Code")))
		{
			if(_ttoi(xml.GetData()) != 100)
			{
				return ;
			}
		}

		if (xml.FindElem(_T("Projects")))
		{
			xml.IntoElem();
			{
				while (xml.FindElem(_T("Project")))
				{
					CProjectData* TempData = LoadOnePrjData(xml);
					CProjectFileMrg::Instance()->m_projects.push_back(TempData);
				}
			}
			xml.OutOfElem();
		}
	}
	xml.OutOfElem();
}

CProjectData* CWebProjectFile::LoadOnePrjData(CMarkup xml)
{
	CProjectData* pPrjData = new CProjectData;
	xml.IntoElem();
	{
		//读取项目信息
		if (xml.FindElem(_T("ProjectInfo")))
		{
			xml.IntoElem();
			{
				if (xml.FindElem(_T("Id")))
				{
					pPrjData->m_prjInfo.m_id = _ttoi(xml.GetData());
				}
				if (xml.FindElem(_T("Name")))
				{
					pPrjData->m_prjInfo.m_sName = xml.GetData();
				}
				if (xml.FindElem(_T("Area")))
				{
					pPrjData->m_prjInfo.m_sArea = xml.GetData();
				}
				if (xml.FindElem(_T("CityCompany")))
				{
					pPrjData->m_prjInfo.m_sCityCompany = xml.GetData();
				}
			}
			xml.OutOfElem();
		}

		while (xml.FindElem(_T("File")))
		{
			CProjectFile prjFile = LoadFileData(xml);
			pPrjData->m_rootDir.AddFile(prjFile);
		}

		//读取项目文件夹信息
		while (xml.FindElem(_T("FileDir")))
		{
			CProjectDir* pDir = LoadDirData(xml);
			pDir->m_parent = &(pPrjData->m_rootDir);
			pPrjData->m_rootDir.AddFolder(pDir);
		}
	}
	xml.OutOfElem();
	return pPrjData;
}


CProjectDir* CWebProjectFile::LoadDirData(CMarkup xml)
{
	CProjectDir* pDir = new CProjectDir;
	xml.IntoElem();
	{
		if (xml.FindElem(_T("Id")))
		{
			pDir->m_id = _ttoi(xml.GetData());
		}
		if (xml.FindElem(_T("Name")))
		{
			pDir->m_sName = xml.GetData();
		}
	
	    while (xml.FindElem(_T("File")))
		{
			CProjectFile prjFile = LoadFileData(xml);
			pDir->AddFile(prjFile);
		}

		while (xml.FindElem(_T("FileDir")))
		{
			CProjectDir* pSubDir = LoadDirData(xml);
			pDir->AddFolder(pSubDir);
		}
		
	}
	xml.OutOfElem();
	return pDir;
}

CProjectFile CWebProjectFile::LoadFileData(CMarkup xml)
{
	CProjectFile prjFile;
	xml.IntoElem();
	{
		if (xml.FindElem(_T("Id")))
		{
			prjFile.m_id = _ttoi(xml.GetData());
		}
		if (xml.FindElem(_T("FileName")))
		{
			prjFile.m_sName = xml.GetData();
		}
		if (xml.FindElem(_T("SaveName")))
		{
			prjFile.m_sSaveName = xml.GetData();
		}
		if (xml.FindElem(_T("FileUrl")))
		{
			prjFile.m_sFileUrl = xml.GetData();
		}
		if (xml.FindElem(_T("Creator")))
		{
			prjFile.m_sCreator = xml.GetData();
		}
		if (xml.FindElem(_T("CreateTime")))
		{
			prjFile.m_sCreateTime = xml.GetData();
		}
		if (xml.FindElem(_T("Updator")))
		{
			prjFile.m_sUpdator = xml.GetData();
		}
		if (xml.FindElem(_T("UpdateTime")))
		{
			prjFile.m_sUpdateTime = xml.GetData();
		}
	}
	xml.OutOfElem();

	return prjFile;
}


void CWebProjectFile::ParseDeleteFileFromXML(CMarkup xml)
{

}

void CWebProjectFile::ParseNewFileDirFromXML(CMarkup xml)
{

}

void CWebProjectFile::ParseRenameFileDirFromXML(CMarkup xml)
{

}

void CWebProjectFile::ParseUpdateFileFromXML(CMarkup xml)
{

}

void CWebProjectFile::ParseDeleteDirFromXML(CMarkup xml)
{

}