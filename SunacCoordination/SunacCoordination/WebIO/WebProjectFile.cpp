#include "StdAfx.h"
#include "WebProjectFile.h"
#include "SunacCadWeb/soapStub.h"
#include "WebIO.h"
#include "../Common/ComFun_String.h"

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
	CProjectFileMrg::Instance()->Clear();


	CMarkup xml;	
	xml.SetDoc((*(nsResponse.GetProjectInfoResult)).c_str());

	ParseProjectInfoFromXML(xml);

	return true;
}

bool CWebProjectFile::UpdateFile(CString p_prjectId, CString p_dir, CString p_fileSaveName, CString p_fileName)
{
	CString cUID;
	std::wstring sUID, sFileSaveName, sUpdateFileName, sParentDir, sPrjID;

	cUID.Format(L"%d", WebIO::GetInstance()->GetUserID());
	p_dir = p_dir.Trim(L"\\");

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
	std::wstring sUID, sNewDirName, sParentDir, sPrjID;

	cUID.Format(L"%d", WebIO::GetInstance()->GetUserID());
	cNewDirName = FilePathToFileName(p_sNewDirPath.Trim(L"\\"));
	if (cNewDirName != p_sNewDirPath.Trim(L"\\"))//是否根目录
	{
		int nPos = p_sNewDirPath.Find(cNewDirName);
		cParentDir = p_sNewDirPath.Left(nPos - 1);
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
	return true;
}

void CWebProjectFile::ParseProjectInfoFromXML(CMarkup xml)
{
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
					CProjectData* TempData = new CProjectData;
					xml.IntoElem();
					{
						if (xml.FindElem(_T("ProjectInfo")))
						{
							xml.IntoElem();
							{
								if (xml.FindElem(_T("Id")))
								{
									TempData->m_prjInfo.m_id = _ttoi(xml.GetData());
								}
								if (xml.FindElem(_T("Name")))
								{
									TempData->m_prjInfo.m_sName = xml.GetData();
								}
								if (xml.FindElem(_T("Area")))
								{
									TempData->m_prjInfo.m_sArea = xml.GetData();
								}
								if (xml.FindElem(_T("CityCompany")))
								{
									TempData->m_prjInfo.m_sCityCompany = xml.GetData();
								}
							}
							xml.OutOfElem();
						}
						while (xml.FindElem(_T("FileDir")))
						{
							/*CProjectDir* TempDir = new CProjectDir;
							xml.IntoElem();
							{
								if (xml.FindElem(_T("Id")))
								{
									TempDir->m_id = _ttoi(xml.GetData());
								}
								if (xml.FindElem(_T("Name")))
								{
									TempDir->m_sName = xml.GetData();
								}
								GetDirsFromWeb(xml, TempDir);//递归
								while (xml.FindElem(_T("File")))
								{
									CProjectFile TempFile;
									xml.IntoElem();
									{
										if (xml.FindElem(_T("Id")))
										{
											TempFile.m_id = _ttoi(xml.GetData());
										}
										if (xml.FindElem(_T("FileName")))
										{
											TempFile.m_sName = xml.GetData();
										}
										if (xml.FindElem(_T("SaveName")))
										{
											TempFile.m_sSaveName = xml.GetData();
										}
										if (xml.FindElem(_T("FileUrl")))
										{
											TempFile.m_sFileUrl = xml.GetData();
										}
										if (xml.FindElem(_T("Creator")))
										{
											TempFile.m_sCreator = xml.GetData();
										}
										if (xml.FindElem(_T("CreateTime")))
										{
											TempFile.m_sCreateTime = xml.GetData();
										}
										if (xml.FindElem(_T("Updator")))
										{
											TempFile.m_sUpdator = xml.GetData();
										}
										if (xml.FindElem(_T("UpdateTime")))
										{
											TempFile.m_sUpdateTime = xml.GetData();
										}
										
									}
									xml.OutOfElem();
									TempDir->AddFile(TempFile);
								}
								
							}
							xml.OutOfElem();*/
							GetDirsFromWeb(xml)->m_parent = &TempData->m_rootDir;
						}
					}
					xml.OutOfElem();
					CProjectFileMrg::Instance()->m_projects.push_back(TempData);
				}
			}
			xml.OutOfElem();
		}
	}
	xml.OutOfElem();
	
	
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

CProjectDir* CWebProjectFile::GetDirsFromWeb(CMarkup xml)
{
	if (xml.FindElem(_T("FileDir")))
	{
		CProjectDir* TempSubDir = new CProjectDir;
		xml.IntoElem();
		{
			if (xml.FindElem(_T("Id")))
			{
				TempSubDir->m_id = _ttoi(xml.GetData());
			}
			if (xml.FindElem(_T("Name")))
			{
				TempSubDir->m_sName = xml.GetData();
			}

			while (xml.FindElem(_T("FileDir")))
			{
				GetDirsFromWeb(xml)->m_parent = TempSubDir;
			}
			
			while (xml.FindElem(_T("File")))
			{
				CProjectFile TempFile;
				xml.IntoElem();
				{
					if (xml.FindElem(_T("Id")))
					{
						TempFile.m_id = _ttoi(xml.GetData());
					}
					if (xml.FindElem(_T("FileName")))
					{
						TempFile.m_sName = xml.GetData();
					}
					if (xml.FindElem(_T("SaveName")))
					{
						TempFile.m_sSaveName = xml.GetData();
					}
					if (xml.FindElem(_T("FileUrl")))
					{
						TempFile.m_sFileUrl = xml.GetData();
					}
					if (xml.FindElem(_T("Creator")))
					{
						TempFile.m_sCreator = xml.GetData();
					}
					if (xml.FindElem(_T("CreateTime")))
					{
						TempFile.m_sCreateTime = xml.GetData();
					}
					if (xml.FindElem(_T("Updator")))
					{
						TempFile.m_sUpdator = xml.GetData();
					}
					if (xml.FindElem(_T("UpdateTime")))
					{
						TempFile.m_sUpdateTime = xml.GetData();
					}

				}
				xml.OutOfElem();
				TempSubDir->AddFile(TempFile);
			}

		}
		xml.OutOfElem();
		return TempSubDir;
	}
}