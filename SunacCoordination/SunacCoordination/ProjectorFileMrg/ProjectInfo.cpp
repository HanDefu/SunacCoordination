#include "StdAfx.h"
#include < afxinet.h>
#include "ProjectInfo.h"
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
	sGuid.Format(_T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
		guid.Data1, guid.Data2,
		guid.Data3, guid.Data4[0],
		guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4],
		guid.Data4[5], guid.Data4[6],
		guid.Data4[7]);
	return sGuid;
}

bool CProjectData::DownloadFile(const CString& strFileURLInServer, //待下载文件的URL
	const CString & strFileLocalFullPath)//存放到本地的路径
{
	ASSERT(strFileURLInServer != "");
	ASSERT(strFileLocalFullPath != "");
	CInternetSession session;
	CHttpConnection* pHttpConnection = NULL;
	CHttpFile* pHttpFile = NULL;
	CString strServer, strObject;
	INTERNET_PORT wPort;

	DWORD dwType;
	const int nTimeOut = 2000;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //重试之间的等待延时
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);   //重试次数
	char* pszBuffer = NULL;

	try
	{
		AfxParseURL(strFileURLInServer, dwType, strServer, strObject, wPort);
		pHttpConnection = session.GetHttpConnection(strServer, wPort);
		pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);
		if (pHttpFile->SendRequest() == FALSE)
			return false;
		DWORD dwStateCode;

		pHttpFile->QueryInfoStatusCode(dwStateCode);
		if (dwStateCode == HTTP_STATUS_OK)
		{
			HANDLE hFile = CreateFile(strFileLocalFullPath, GENERIC_WRITE,
				FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
				NULL);  //创建本地文件
			if (hFile == INVALID_HANDLE_VALUE)
			{
				pHttpFile->Close();
				pHttpConnection->Close();
				session.Close();
				return false;
			}

			char szInfoBuffer[1000];  //返回消息
			DWORD dwFileSize = 0;   //文件长度
			DWORD dwInfoBufferSize = sizeof(szInfoBuffer);
			BOOL bResult = FALSE;
			bResult = pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH,
				(void*)szInfoBuffer, &dwInfoBufferSize, NULL);

			dwFileSize = atoi(szInfoBuffer);
			const int BUFFER_LENGTH = 1024 * 10;
			pszBuffer = new char[BUFFER_LENGTH];  //读取文件的缓冲
			DWORD dwWrite, dwTotalWrite;
			dwWrite = dwTotalWrite = 0;
			UINT nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH); //读取服务器上数据

			while (nRead > 0)
			{
				WriteFile(hFile, pszBuffer, nRead, &dwWrite, NULL);  //写到本地文件
				dwTotalWrite += dwWrite;
				nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH);
			}

			delete[]pszBuffer;
			pszBuffer = NULL;
			CloseHandle(hFile);
		}
		else
		{
			delete[]pszBuffer;
			pszBuffer = NULL;
			if (pHttpFile != NULL)
			{
				pHttpFile->Close();
				delete pHttpFile;
				pHttpFile = NULL;
			}
			if (pHttpConnection != NULL)
			{
				pHttpConnection->Close();
				delete pHttpConnection;
				pHttpConnection = NULL;
			}
			session.Close();
			return false;
		}
	}
	catch (...)
	{
		delete[]pszBuffer;
		pszBuffer = NULL;
		if (pHttpFile != NULL)
		{
			pHttpFile->Close();
			delete pHttpFile;
			pHttpFile = NULL;
		}
		if (pHttpConnection != NULL)
		{
			pHttpConnection->Close();
			delete pHttpConnection;
			pHttpConnection = NULL;
		}
		session.Close();
		return false;
	}

	if (pHttpFile != NULL)
		pHttpFile->Close();
	if (pHttpConnection != NULL)
		pHttpConnection->Close();
	session.Close();
	return true;
}

bool CProjectData::UploadFile(CString p_sFilePath, CString p_saveName)
{
	CFtpConnection *m_pFtpConnection = NULL;
	CInternetSession *m_pInetsession = new CInternetSession(NULL, 1, PRE_CONFIG_INTERNET_ACCESS);
	try
	{
		//TODO 服务器地址和用户名、密码需设置
		m_pFtpConnection = m_pInetsession->GetFtpConnection(L"192.168.13.13", L"test", L"1234", 21);
		//MessageBox("连接成功");

		m_pFtpConnection->PutFile(p_sFilePath, p_saveName, FTP_TRANSFER_TYPE_BINARY, 1);
	}
	catch (CInternetException *pEx)
	{
		TCHAR szError[1024];
		if (pEx->GetErrorMessage(szError, 1024))
			AfxMessageBox(szError);
		else
			AfxMessageBox(L"There was an exception");
		pEx->Delete();
		m_pFtpConnection = NULL;
		return false;
	}

	return true;
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

bool CProjectData::AddFile(CString p_sFilePath, CString  p_sParentFolderPath) //p_sParentFolderPath是指上传到哪个目录下,目录层级用\分割，例如：A区\施工图 表示A区文件夹下的施工图文件夹
{
	//找到目录
	CProjectDir* p_pParentDir = FindDir(p_sParentFolderPath);
	if (p_pParentDir == NULL)
		return false;

	//1上传文件到ftp中
	CString sFileName = FilePathToFileName(p_sFilePath);
	int nPos = sFileName.ReverseFind(_T('.'));
	if (nPos<0)
	{
		return false;
	}

	CString sExtensionName = sFileName.Mid(nPos);
	CString sSaveName = GenerateGuid() + sExtensionName; //实际存储的名字
	sSaveName.Delete(sSaveName.Find(_T("{")));
	sSaveName.Delete(sSaveName.Find(_T("}")));

	//上传到ftp中
	UploadFile(p_sFilePath, sSaveName);

	//2.加到本地的项目文件中
	CProjectFile prjfile;
	prjfile.m_sName = sFileName;
	prjfile.m_sSaveName = sSaveName;

	p_pParentDir->AddFile(prjfile);
	

	//3.调用web接口添加到web数据库中
	CWebProjectFile::Instance()->UpdateFile(GetProjectId(), p_sParentFolderPath, sSaveName, sFileName);

	return true;
}
CString CProjectData::GetProjectId()const
{
	CString sId;
	sId.Format(_T("%d"), m_prjInfo.m_id);
	return sId;
}
bool CProjectData::DeleteFile(CString p_sFileName, CString  p_sParentFolderPath)
{
	//找到目录
	CProjectDir* pDir = FindDir(p_sParentFolderPath);
	if (pDir == NULL)
		return false;

	pDir->DeleteFile(p_sFileName);

	//web端更新 
	CWebProjectFile::Instance()->DeleteFile(GetProjectId(), p_sParentFolderPath, p_sFileName);

	return true;
}
bool CProjectData::AddFolder(CString  p_sParentFolderPath, CString p_sFolderName)
{
	//找到目录
	CProjectDir* pDir = NULL;
	if (p_sParentFolderPath.IsEmpty() == false)
	{
		pDir = FindDir(p_sParentFolderPath);
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