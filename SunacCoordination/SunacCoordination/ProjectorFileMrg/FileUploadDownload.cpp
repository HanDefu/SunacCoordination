#include "StdAfx.h"
#include < afxinet.h>
#include "FileUploadDownload.h"

CFileUpDownLoad* CFileUpDownLoad::Instance()
{
	static CFileUpDownLoad s_fileUpDown;
	return &s_fileUpDown;
}

CFileUpDownLoad::CFileUpDownLoad()
{
}

CFileUpDownLoad::~CFileUpDownLoad()
{
}

//strFileURLInServer待下载文件的URL,  strFileLocalFullPath存放到本地的路径
bool CFileUpDownLoad::DownloadFile(const CString& strFileURLInServer, const CString & strFileLocalFullPath)
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

bool CFileUpDownLoad::UploadFile(CString p_sFilePath, CString p_ftpSaveName, CString p_ftpDir)
{
	CFtpConnection *pFtpConnection = NULL;
	CInternetSession *m_pInetsession = new CInternetSession(NULL, 1, PRE_CONFIG_INTERNET_ACCESS);
	try
	{
		//TODO 服务器地址和用户名、密码需设置
#ifdef WORK_LOCAL
		pFtpConnection = m_pInetsession->GetFtpConnection(L"192.168.13.13", L"test", L"1234", 21);
#else
		pFtpConnection = m_pInetsession->GetFtpConnection(L"10.4.64.91", L"caduploader", L"ty19@scad", 2121); //测试环境
#endif // WORK_LOCAL

		if (pFtpConnection ==  NULL)
		{
			assert(false);
			return false; 
		}

		BOOL bDirExsit = pFtpConnection->SetCurrentDirectory(p_ftpDir);
		if (bDirExsit == FALSE)
		{
			BOOL bCreateSuc = pFtpConnection->CreateDirectory(p_ftpDir);
			BOOL bDirExsit = pFtpConnection->SetCurrentDirectory(p_ftpDir);
			if (bCreateSuc == FALSE || bDirExsit ==FALSE)
			{
				return false;
			}
		}

		pFtpConnection->PutFile(p_sFilePath, p_ftpSaveName, FTP_TRANSFER_TYPE_BINARY, 1);
	}
	catch (CInternetException *pEx)
	{
		TCHAR szError[1024];
		if (pEx->GetErrorMessage(szError, 1024))
			AfxMessageBox(szError);
		else
			AfxMessageBox(L"There was an exception");
		pEx->Delete();
		pFtpConnection = NULL;
		return false;
	}

	return true;
}

void CFileUpDownLoad::UploadFileByThread(CString p_sFilePath, CString p_ftpSaveName, CString p_ftpDir)
{

}